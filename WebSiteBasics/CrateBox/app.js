var vertexShaderText = 
[
'precision highp float;',
'varying vec2 fragTexCoord;',
'attribute vec3 vertPosition;',
'attribute vec2 texturePos;',
'uniform mat4 TTO;',
'uniform mat4 RTA;',
'uniform mat4 Proj;',
'highp mat4 transpose(in highp mat4 inMatrix) {highp vec4 i0 = inMatrix[0];highp vec4 i1 = inMatrix[1];highp vec4 i2 = inMatrix[2];highp vec4 i3 = inMatrix[3];',
'highp mat4 outMatrix = mat4(vec4(i0.x, i1.x, i2.x, i3.x),vec4(i0.y, i1.y, i2.y, i3.y),vec4(i0.z, i1.z, i2.z, i3.z),vec4(i0.w, i1.w, i2.w, i3.w) );',

'return outMatrix;}',
'void main()',
'{',
//vec4 Pos = RTA*TTO*vec4(vertPosition.xyz, 1.0)
'gl_Position = transpose(Proj)*transpose(RTA)*transpose(TTO)*vec4(vertPosition, 1.0);',
'fragTexCoord = texturePos;',
'}'

].join('\n');

var fragmentShaderText = 
[
'precision highp float;',
'varying vec2 fragTexCoord;',
'uniform sampler2D sampler0;',
'void main()',
'{',
'gl_FragColor = texture2D(sampler0, fragTexCoord);',
'}'

].join('\n');

//Using this camera is always facing in -Z with right the x-axis, but scene moves around camera
var TTO32 = function(cam_pos) //Transform to origin(matrix)
{
    //cam_pos is of form [1,3,2]
    //returns as float 32
    
    var x = cam_pos[0];
    var y = cam_pos[1];
    var z = cam_pos[2];
    
    return new Float32Array([1.0,0.0,0.0,-x,
                             0.0,1.0,0.0,-y,
                             0.0,0.0,1.0,-z,
                             0.0,0.0,0.0,1.0]);
}

var RTA32 = function(u, v, n) //Rotate to Align(matrix)
{
    // u is the direction to the right of camera 
    // v is the up direction 
    // n points backwards (point negative direction camera is facing)
    
    return new Float32Array([u[0],u[1],u[2],0.0,
                             v[0],v[1],v[2],0.0,
                             n[0],n[1],n[2],0.0,
                             0.0 ,0.0 ,0.0 ,1.0]);
}

var OrthoProj32 = function(s) //OrthographicProjection Matrix
{
    // s = size is the width of the viewport [x_width, y_width, z_width]
    // z part is minus as camera looking -z direction, and webGL prioritses z value closest to -1
    return new Float32Array([1/s[0], 0.0, 0.0, 0.0,
                             0.0, 1/s[1], 0.0, 0.0,
                             0.0, 0.0,-1/s[2], 0.0,
                             0.0, 0.0, 0.0,    1.0]);
}

var lookAt = function(a) //return RTA32 by calculating other directions
{
    //Easy and quick to use, but cannot look vertically up or down
    // a is the direction of the camera 
    var my_length = (a[0]**2 + a[1]**2 + a[2]**2)**0.5;
    a[0] = a[0]/my_length;
    a[1] = a[1]/my_length;
    a[2] = a[2]/my_length;
    
    //Get Horizontal
    var u = [a[1], -a[0], 0];
    var u_length = (a[0]**2+a[1]**2)**0.5;
    
    u[0] = u[0]/u_length;
    u[1] = u[1]/u_length;
    
    //Get Vertical
    var v = [u[1]*a[2]-u[2]*a[1], u[2]*a[0]-u[0]*a[2], u[0]*a[1] - u[1]*a[0]];
    var n = [-a[0],-a[1],-a[2]];
    return RTA32(u,v,n);
    
}
    
var Init = function() {

    console.log(RTA32([1.0,0.0,0.0],[0.0,1.0,0.0],[0.0,0.0,1.0]));
    console.log(lookAt([1.0,1.0,0.3]));
    
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
    gl.clearColor(0.0, 0.0, 1.0, 1.0);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    gl.enable(gl.DEPTH_TEST);
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
    
    gl.useProgram(program);
    
    //Create Buffers
    var Vertices = 
    [ // X, Y, Z, textCoords X,Y
        0.0, 0.0, 0.0, 0.0, 1.0, //0
        0.0, 0.0, 1.0, 0.25, 0.67, //1
        0.0, 1.0, 0.0, 1.0, 0.0, //2
        0.0, 1.0, 1.0, 0.50, 0.67, //3
        1.0, 0.0, 0.0, 1.0, 0.0, //4
        1.0, 0.0, 1.0, 0.25, 0.33, //5
        1.0, 1.0, 0.0, 1.0, 0.0, //6
        1.0, 1.0, 1.0, 0.50, 0.33  //7
        
    ];
    
    var Indices = 
    [
        //Bottom
        0, 4, 2,
        4, 2, 6,
        
        //Front
        0, 4, 1,
        4, 1, 5,
        
        //Right
        4, 5, 6,
        5, 6, 7,
        
        //Left
        0, 2, 1,
        2, 1, 3,
        
        //Back
        2, 6, 3,
        6, 3, 7,
        
        //Top
        1, 5, 3,
        5, 3, 7   
    ]
    
    //Send data to GPU buffer
    var VBO = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, VBO);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(Vertices), gl.STATIC_DRAW);
    

    
    //Tell GPU what the format of the data is
    var positionAttribLocation = gl.getAttribLocation(program, 'vertPosition');
    var textPosAttribLocation = gl.getAttribLocation(program, 'texturePos');
    

    
    
    gl.vertexAttribPointer(
    positionAttribLocation, //Attribute location
    3,// Number of elements per attribute (1,2,3,4)
    gl.FLOAT,//Type of elements
    gl.FALSE, //Is normed ?
    5*4,//Size of a an individual vertex, number of data per vertex, stride distance
    0//Offset from the beginning of a single vertex 
    );
    
    gl.vertexAttribPointer(
    textPosAttribLocation, //Attribute location
    2,// Number of elements per attribute (1,2,3,4)
    gl.FLOAT,//Type of elements
    gl.FALSE, //Is normed ?
    5*4,//Size of a an individual vertex, number of data per vertex, stride distance
    3*4//Offset from the beginning of a single vertex 
    );
    
    
    gl.enableVertexAttribArray(textPosAttribLocation);
    gl.enableVertexAttribArray(positionAttribLocation);
    
    
    //Define index Buffer
    var IBO = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, IBO);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(Indices), gl.STATIC_DRAW);
    
    //Define texture
    var boxTexture = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D, boxTexture);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
    
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE,
    document.getElementById('DirtBlock'));

    //Get Uniform Locations
    var mat4TTOUniformLocation = gl.getUniformLocation(program, 'TTO');
    var mat4RTAUniformLocation = gl.getUniformLocation(program, 'RTA');
    var mat4ProjUniformLocation = gl.getUniformLocation(program, 'Proj');
    
    //Set Uniforms
    
    var TTO = TTO32([-0.25,-0.25,1.25]);
    var RTA = lookAt([1,1,-1]);
    var Proj = OrthoProj32([2,2,2]);
    
    gl.uniformMatrix4fv(mat4TTOUniformLocation, gl.FALSE, TTO);
    gl.uniformMatrix4fv(mat4RTAUniformLocation, gl.FALSE, RTA);
    gl.uniformMatrix4fv(mat4ProjUniformLocation, gl.FALSE, Proj);
    
    // Main loop
    var loop = function () {
        gl.useProgram(program);
        
        gl.clearColor(0.0, 1.0, 1.0, 1.0);
		gl.clear(gl.DEPTH_BUFFER_BIT | gl.COLOR_BUFFER_BIT);
        
        gl.bindTexture(gl.TEXTURE_2D, boxTexture);
		gl.activeTexture(gl.TEXTURE0);

        
        gl.drawElements(gl.TRIANGLES, Indices.length, gl.UNSIGNED_SHORT, 0);
        requestAnimationFrame(loop)
    };
    requestAnimationFrame(loop);
    
};
