var vertexShaderText = 
[
'precision highp float;',
'varying vec2 fragColor;',
'attribute vec2 vertPosition;',
'void main()',
'{',
'gl_Position = vec4(vertPosition, 0.0, 1.0);',
'fragColor = vertPosition;',
'}'

].join('\n');

var fragmentShaderText = 
[
'precision highp float;',
'varying vec2 fragColor;',
'void main()',
'{',
'gl_FragColor = vec4(fragColor,0.0, 1.0);',
'}'

].join('\n');

var Init = function() {

    
    var canvas = document.getElementById('Render_Surface');
    var gl=canvas.getContext('webgl');
    
    if (!gl) {
        console.log("WebGL not supported, using experimental version");
        gl = canvas.getContext('experimental-webgl');
    }
    
    if (!gl) {
        alert("Your browser does not support webGL");        
    }
    
    
    //Start color
    gl.clearColor(1.0, 0.0, 1.0, 1.0);
    gl.clear(gl.COLOR_BUFFER_BIT);
    console.log("This  is Working");
    
    //Create shader
    var vertexShader = gl.createShader(gl.VERTEX_SHADER);
    var fragmentShader = gl.createShader(gl.FRAGMENT_SHADER);
    
    gl.shaderSource(vertexShader, vertexShaderText);
    gl.shaderSource(fragmentShader, fragmentShaderText);
    
    //Check if worked
    gl.compileShader(vertexShader);
    if (!gl.getShaderParameter(vertexShader, gl.COMPILE_STATUS)) {
        console.error('ERROR compiling vertex shader!', gl.getShaderInfoLog(vertexShader));
        return;
    }
    
    gl.compileShader(fragmentShader);
    //Check if worked
    gl.compileShader(fragmentShader);
    if (!gl.getShaderParameter(fragmentShader, gl.COMPILE_STATUS)) {
        console.error('ERROR compiling fragment shader!', gl.getShaderInfoLog(fragmentShader));
        return;
    }
    
    //Now create the pipeline
    var program = gl.createProgram();
    gl.attachShader(program, vertexShader);
    gl.attachShader(program, fragmentShader);
    gl.linkProgram(program);
    if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
        console.error('ERROR linking program!', gl.getProgramInfoLog(program));
        return;
    }
    
    //Validate the Program
    gl.validateProgram(program);
    if (!gl.getProgramParameter(program, gl.VALIDATE_STATUS)) {
        console.error('ERROR validating program!', gl.getProgramInfoLog(program));
        return;
    }
    
    //Create Buffers
    var Vertices = 
    [ // X, Y
        0.0, 0.5,
        -0.5, -0.5,
        0.5, -0.5
    ];
    
    var VBO = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, VBO);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(Vertices), gl.STATIC_DRAW);
    
    var positionAttribLocation = gl.getAttribLocation(program, 'vertPosition');
    gl.vertexAttribPointer(
    positionAttribLocation, //Attribute location
    2,// Number of elements per attribute (1,2,3,4)
    gl.FLOAT,//Type of elements
    gl.FALSE, //Is normed ?
    2*Float32Array.BYTES_PER_ELEMENT,//Size of a an individual vertex
    0//Offser from the beginning of a single vertex to the end  
    );
    gl.enableVertexAttribArray(positionAttribLocation);
    
    // Main loop
    gl.useProgram(program);
    gl.drawArrays(gl.TRIANGLES, 0, 3);
    
    
};
