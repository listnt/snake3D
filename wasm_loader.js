var Module = {
    onRuntimeInitialized: function() {
        var canvas = document.querySelector('canvas');

        canvas.width = 800;
        canvas.height = 600;

        console.log("[JS] initializing WebGL");

        const result = Module.ccall(
            "init",
            'number',
            ['number', 'number'],
            [canvas.width, canvas.height]);

        console.log("[JS] WebGL initialization result: ", result);
    },
};