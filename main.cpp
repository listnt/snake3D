#include <cstdio>
#include <GLES2/gl2.h>
#include <emscripten.h>
#include <SDL/SDL.h>
#include <emscripten/html5.h>
#define GL_GLEXT_PROTOTYPES
#include <cmath>
#include <unistd.h>
#include <GLES2/gl2ext.h>
#include<EGL/egl.h>
#include <sys/time.h>
#include <cstring>

#include "src/base/base.h"
#include "src/base/instance.h"
#include "src/game/map.h"
#include "src/game/snake.h"
#include <GLFW/glfw3.h>
#include <chrono>

#include "src/game/Game.h"

EMSCRIPTEN_WEBGL_CONTEXT_HANDLE initContext() {
    EmscriptenWebGLContextAttributes attrs;
    // attrs.alpha = false;
    attrs.depth = true;
    // attrs.stencil = true;
    // attrs.antialias = true;
    // attrs.premultipliedAlpha = false;
    // attrs.preserveDrawingBuffer = false;
    // attrs.failIfMajorPerformanceCaveat = false;
    attrs.majorVersion = 2;
    attrs.minorVersion = 0;
    // attrs.enableExtensionsByDefault = false;

    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context("#canvas", &attrs);
    if (!ctx) {
        printf("Webgl ctx could not be created!\n");
        return -1;
    }

    emscripten_webgl_make_context_current(ctx);

    return ctx;
}


GLuint vertexShader, fragmentShader;

GLuint CompileShader(GLenum type, FILE *shaderFile) {
    fseek(shaderFile, 0, SEEK_END);
    long fsize = ftell(shaderFile);
    fseek(shaderFile, 0, SEEK_SET); /* same as rewind(f); */

    char *shaderSrc = (char *) malloc(fsize + 1);
    fread(shaderSrc, fsize, 1, shaderFile);
    fclose(shaderFile);

    shaderSrc[fsize] = '\0';

    // use the string, then ...

    // printf("[DEBUG] compiling shader: %d\nsource:\n%s\n", type, shaderSrc);
    GLint compiled;

    // Create the shader object
    GLuint shader = glCreateShader(type);

    if (shader == 0) {
        free(shaderSrc);
        return 0;
    }

    // Load the shader source
    glShaderSource(shader, 1, &shaderSrc, nullptr);

    // Compile the shader
    glCompileShader(shader);

    // Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLint infoLen = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1) {
            char *infoLog = static_cast<char *>(malloc(sizeof(char) * infoLen));

            glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);
            printf("Error compiling shader:\n%s\n", infoLog);

            free(infoLog);
        }

        glDeleteShader(shader);
        free(shaderSrc);
        return 0;
    }

    free(shaderSrc);
    return shader;
}


int linkProgram(const GLuint programObject) {
    GLint linked;

    // Link the program
    glLinkProgram(programObject);

    // Check the link status
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

    if (!linked) {
        GLint infoLen = 0;

        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1) {
            const auto infoLog = static_cast<char *>(malloc(sizeof(char) * infoLen));

            glGetProgramInfoLog(programObject, infoLen, nullptr, infoLog);
            printf("Error linking program:\n%s\n", infoLog);

            free(infoLog);
        }

        glDeleteProgram(programObject);
        return GL_FALSE;
    }

    return GL_TRUE;
}

GLuint initShaders() {
    FILE *vShaderFile = fopen("shaders/.vert", "rb");
    FILE *fragmentShaderFile = fopen("shaders/.frag", "rb");

    vertexShader = CompileShader(GL_VERTEX_SHADER, vShaderFile);
    fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderFile);
    if (vertexShader == 0 || fragmentShader == 0) {
        return 0;
    }

    // Create the program object
    const GLuint programObject = glCreateProgram();
    if (programObject == 0)
        return 0;
    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);

    linkProgram(programObject);
    glUseProgram(programObject);

    return programObject;
}

snake *snakeI;

std::vector<instance *> opaqueObjs;
std::vector<instance *> transparentObjs;
GLint projectMat;
Matrix4x4 projection;
Matrix4x4 camera;
Game *game;
GLint lightPos;

int oldTimestamp = 0;


void initScene() {
    camera = lookAt({0, -13, -6}, {0, 0, 0}, {0, 1, 0});
    instance *pivot = new instance();
    auto redCube = createCube({1, 0, 0, 1});

    pivot->loadModel(redCube);
    opaqueObjs.push_back(pivot);

    projectMat = glGetUniformLocation(userData->programObject, "projection");
    projection = computePerspectiveMatrix(
        M_PI * 0.3,
        static_cast<float>(userData->WindowWidth) / static_cast<float>(userData->WindowHeight),
        1,
        50.0);


    lightPos = glGetUniformLocation(userData->programObject, "lightPos");
    glUniform3f(lightPos, 0, 0, 20);
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw() {
    int timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    glDepthMask(true);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto obj: opaqueObjs) {
        obj->Render(camera);
    }

    // camera = pitch(timestamp / 1000.0) * translate(0, 0, 10 + 5 * sin(timestamp / 1000.0));


    glUniformMatrix4fv(projectMat, 1,GL_FALSE, flatten(projection).data());

    if (timestamp - oldTimestamp > 500.0) {
        oldTimestamp = timestamp;
        game->GameLoop(camera);
    }

    game->RenderLoop(camera);


    // glFlush();
}

UserData *userData;

#ifdef __cplusplus

extern "C" {
#endif
int EMSCRIPTEN_KEEPALIVE init(const int width, const int height, bool debug) {
    if (userData) {
        printf("window already initialized\n");

        return GL_TRUE;
    }


    userData = static_cast<UserData *>(malloc(sizeof(UserData)));
    userData->WindowHeight = height;
    userData->WindowWidth = width;


    if (!debug) {
        auto ctx = initContext();
    }

    const auto programObject = initShaders();
    // Store the program object
    userData->programObject = programObject;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST); //GL_BLEND | GL_CULL_FACE
    // glCullFace(GL_BACK);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // // glDepthFunc(GL_LESS);
    // glClearDepthf(1.0f);

    game = new Game(7);
    initScene();
    emscripten_set_keydown_callback("#canvas", nullptr, true,
                                    [](int k,const EmscriptenKeyboardEvent *e,void * u) -> EM_BOOL{
                                    return game->Controls(k,e,u);
                                    });
    emscripten_set_main_loop(Draw, 0, true);

    return GL_TRUE;
}

#ifdef __cplusplus
}
#endif

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(int argc, char **argv) {
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    auto window = SDL_CreateWindow("title",
                                   SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   640, 480,
                                   SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    auto gl_context = SDL_GL_CreateContext(window);

    init(640, 480, false);

    return 1;
}
