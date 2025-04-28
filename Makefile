compile:
	em++ -std=c++20 main.cpp src/base/instance.cpp src/base/base.cpp src/game/map.cpp src/game/snake.cpp -o page.html --preload-file shaders  -s MIN_WEBGL_VERSION=2 -s EXPORTED_FUNCTIONS=['_init','_main'] -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] -sINITIAL_MEMORY=16777216
	mv build/app_wasm.data ./
debug:
	EMCC_DEBUG=1 emcc -std=c++20 -g main.cpp src/base/instance.cpp src/base/base.cpp src/game/map.cpp src/game/snake.cpp src/game/Game.cpp -o build/page.html --preload-file shaders -s MIN_WEBGL_VERSION=2 --memoryprofiler -s USE_WEBGL2=1 -s USE_SDL_IMAGE=2 -s USE_SDL=2
	mv build/app_wasm.data ./