var Module = {
    onRuntimeInitialized: function () {
        var canvas = document.querySelector('canvas');

        canvas.width = 800;
        canvas.height = 600;

        console.log("[JS] initializing WebGL");

        const result = Module.ccall(
            "init",
            'number',
            ['number', 'number', 'bool'],
            [canvas.width, canvas.height, false]);

        console.log("[JS] WebGL initialization result: ", result);
    },
};