var Ref_URL = "" //Different to gh-pages allows to test using local server

var x_size;
var y_size;
var steps = 1; //steps per frame
var Play = false;
var restart = false;

var drawfragmentShaderText = 
[
'#version 300 es',
'precision highp float;',
'in vec2 fragTexCoord;',
'uniform highp isampler2D sampler0;',
'out vec4 FragColor;',
'void main()',
'{',
'FragColor = vec4(float(texture(sampler0, fragTexCoord).x), 0.0,0.0, 1.0);',
'//FragColor = texture(sampler0, fragTexCoord);',
'}'

].join('\n');


var Play_Stop = function()
{
    Play = !Play; 
    console.log(Play);
}




var Init = function()
{
    loadTextResource(Ref_URL + '/ShaderVS.txt', function (vsError, vsText) 
    {
       if (vsError) {alert('vs shader failed to load'); console.error(vsText);}
       else { loadTextResource(Ref_URL + '/ShaderFS.txt', function (fsError, fsText) {
       
       if (fsError) {alert('fs shader failed to load'); console.error(fsText);}
       else { Start(vsText, fsText)}               
       })}       
    }
    )
    
}

function getRandomInt(max) {
  return Math.floor(Math.random() * max);
}

var random_array = function(num){
    var my_array = [];
    for (let i = 0; i < num; i++) {
        my_array.push(getRandomInt(2));
    }   
    return my_array;    
}
    
var Start = function(vertexShaderText, fragmentShaderText) {


    
    
    var canvas = document.getElementById('Game_Of_Life_Grid');
    var btnStartStop = document.getElementById('btnStartStop');
    var btnResetGrid = document.getElementById('btnResetGrid');
    var numSteps = document.getElementById('numSteps');
    var numXSize = document.getElementById('numXSize');
    var numYSize = document.getElementById('numYSize');
    var lblCanvas = document.getElementById('lblCanvas');
    var lblFPS = document.getElementById('lblFPS');
    
    var gl = canvas.getContext("webgl2");
    if (!gl) { 
        console.log("No webgl2");
        alert("No webgl2");
        gl=canvas.getContext('webgl');
    }
    
    
    if (!gl) {
        console.log("WebGL not supported, using experimental version");
        gl = canvas.getContext('experimental-webgl');
    }
    
    if (!gl) {
        alert("Your browser does not support webGL");        
    }
    
    //Functions for buttons
    var Reset_Grid = function()
    {
        Play = false;
        
        //Update Size of Grid
        x_size = numXSize.value;
        y_size = numYSize.value;
        gl.useProgram(next_step_program);
        gl.uniform2f(sizeUniformLocation, x_size, y_size);
        
        //Update texture data
        gl.bindTexture(gl.TEXTURE_2D, Grid1);
        var new_initial_data = new Int8Array(random_array(x_size*y_size));
        console.log(new_initial_data);
        gl.texImage2D(gl.TEXTURE_2D, 0, gl.R8I, x_size,y_size,0, gl.RED_INTEGER, gl.BYTE, new_initial_data);
        
        
        gl.bindFramebuffer(gl.FRAMEBUFFER, fbo1);
        gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0, gl.TEXTURE_2D,
        Grid1, 0);
    
        var e = gl.checkFramebufferStatus(gl.FRAMEBUFFER);
        if (gl.FRAMEBUFFER_COMPLETE !== e) {
          console.log('Frame buffer object 1 is incomplete: ' + e.toString());
        }
        
                
        gl.bindTexture(gl.TEXTURE_2D, Grid2);
        gl.texImage2D(gl.TEXTURE_2D, 0, gl.R8I, x_size,y_size,0, gl.RED_INTEGER, gl.BYTE, null);
        
        gl.bindFramebuffer(gl.FRAMEBUFFER, fbo2);
        gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0, gl.TEXTURE_2D,
        Grid2, 0);
        
        
        var e = gl.checkFramebufferStatus(gl.FRAMEBUFFER);
        if (gl.FRAMEBUFFER_COMPLETE !== e) {
          console.log('Frame buffer object 2 is incomplete: ' + e.toString());
        }
        
        //Display Texture
        Clear_Canvas();
        Draw(Grid1);
    
    }
    
    //Add event listeners
    btnStartStop.addEventListener('click', Play_Stop);
    btnResetGrid.addEventListener('click', Reset_Grid);
    numSteps.addEventListener('input', function() {steps = this.value; console.log(this.value);});

    
    //Start color
    gl.clearColor(0.0, 0.0, 1.0, 1.0);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    gl.enable(gl.DEPTH_TEST);
    
    gl.pixelStorei(gl.UNPACK_ALIGNMENT, 1);
    console.log("This  is Working");
    console.log("This  is Working");
    console.log(random_array(5));
    
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
    
    //Check if worked
    gl.compileShader(fragmentShader);
    if (!gl.getShaderParameter(fragmentShader, gl.COMPILE_STATUS)) {
        console.error('ERROR compiling fragment shader!', gl.getShaderInfoLog(fragmentShader));
        return;
    }
    
    //Now create the pipeline
    var next_step_program = gl.createProgram();
    gl.attachShader(next_step_program, vertexShader);
    gl.attachShader(next_step_program, fragmentShader);
    gl.linkProgram(next_step_program);
    if (!gl.getProgramParameter(next_step_program, gl.LINK_STATUS)) {
        console.error('ERROR linking next_step_program!', gl.getProgramInfoLog(next_step_program));
        return;
    }
    
    //Validate the next_step_program
    gl.validateProgram(next_step_program);
    if (!gl.getProgramParameter(next_step_program, gl.VALIDATE_STATUS)) {
        console.error('ERROR validating next_step_program!', gl.getProgramInfoLog(next_step_program));
        return;
    }
    
    gl.useProgram(next_step_program);
    
    //Create Draw program
    var drawfragmentShader = gl.createShader(gl.FRAGMENT_SHADER);
    gl.shaderSource(drawfragmentShader, drawfragmentShaderText);
    
    //Check if worked
    gl.compileShader(drawfragmentShader);
    if (!gl.getShaderParameter(drawfragmentShader, gl.COMPILE_STATUS)) {
        console.error('ERROR compiling draw fragment shader!', gl.getShaderInfoLog(drawfragmentShader));
        return;
    }
    
    //Create post program pipeline
    var drawprogram = gl.createProgram();
    gl.attachShader(drawprogram, vertexShader);
    gl.attachShader(drawprogram, drawfragmentShader);
    gl.linkProgram(drawprogram);
    if (!gl.getProgramParameter(drawprogram, gl.LINK_STATUS)) {
        console.error('ERROR linking drawprogram!', gl.getProgramInfoLog(drawprogram));
        return;
    }

    //Create Buffers
    var Vertices = 
    [ // X, Y textCoords X,Y
		// Top
		-1.0, 1.0,    0.0, 1.0,
		1.0, 1.0,     1.0, 1.0,
		1.0, -1.0,    1.0, 0.0,
		-1.0, -1.0,   0.0, 0.0

    ];
    
    var Indices = 
    [
		// Top
		0, 1, 2,
		0, 2, 3
    ];
    
    //Send data to GPU buffer
    var VBO = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, VBO);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(Vertices), gl.STATIC_DRAW);
    

    
    //Tell GPU what the format of the data is
    var positionAttribLocation = gl.getAttribLocation(next_step_program, 'vertPosition');
    var textPosAttribLocation = gl.getAttribLocation(next_step_program, 'texturePos');
    

    
    
    gl.vertexAttribPointer(
    positionAttribLocation, //Attribute location
    2,// Number of elements per attribute (1,2,3,4)
    gl.FLOAT,//Type of elements
    gl.FALSE, //Is normed ?
    4*4,//Size of a an individual vertex, number of data per vertex, stride distance
    0//Offset from the beginning of a single vertex 
    );
    
    gl.vertexAttribPointer(
    textPosAttribLocation, //Attribute location
    2,// Number of elements per attribute (1,2,3,4)
    gl.FLOAT,//Type of elements
    gl.FALSE, //Is normed ?
    4*4,//Size of a an individual vertex, number of data per vertex, stride distance
    2*4//Offset from the beginning of a single vertex 
    );
    
    
    gl.enableVertexAttribArray(textPosAttribLocation);
    gl.enableVertexAttribArray(positionAttribLocation);
    
    
    //Define index Buffer
    var IBO = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, IBO);
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(Indices), gl.STATIC_DRAW);
    
    //Define texture
    var Grid1 = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D, Grid1);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.REPEAT);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.REPEAT);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
    
    //Store random life grid
    gl.pixelStorei(gl.UNPACK_ALIGNMENT, 1);
    // Add initial data to texture in reset function
    
    
    var Grid2 = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D, Grid2);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.REPEAT);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.REPEAT);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);   
    //gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, x_size,y_size,0, gl.RGB, gl.UNSIGNED_BYTE, null);
    gl.pixelStorei(gl.UNPACK_ALIGNMENT, 1);
    // Add initial data to texture in reset function
    //Define frame buffer
    var fbo1 = gl.createFramebuffer();
    
    var fbo2 = gl.createFramebuffer();
    
    //Get Uniform Locations
    var sizeUniformLocation = gl.getUniformLocation(next_step_program, 'size');
    
    //Set Uniforms
    gl.uniform2f(sizeUniformLocation, x_size, y_size);
    
    
    gl.bindFramebuffer(gl.FRAMEBUFFER, null);
    //gl.activeTexture(gl.TEXTURE0);
    //gl.drawElements(gl.TRIANGLES, Indices.length, gl.UNSIGNED_SHORT, 0);
    // Main loop
    var Draw = function(texture, render_to = null)
    {

            
        gl.useProgram(drawprogram);
        gl.bindFramebuffer(gl.FRAMEBUFFER, render_to);
        gl.bindTexture(gl.TEXTURE_2D, texture);
        gl.activeTexture(gl.TEXTURE0);
        gl.viewport(0,0,canvas.width,canvas.height);
        gl.drawElements(gl.TRIANGLES, Indices.length, gl.UNSIGNED_SHORT, 0);    
        
    }
    
    var Clear_Canvas = function()
    {
        gl.viewport(0,0, canvas.width,canvas.height);
        gl.bindFramebuffer(gl.FRAMEBUFFER, null);
        gl.clearColor(0.0, 1.0, 1.0, 1.0);
        gl.clear(gl.DEPTH_BUFFER_BIT | gl.COLOR_BUFFER_BIT);        
        
    }
    
    var calc_next = function()
    {
        //Calc next time step
        gl.useProgram(next_step_program);                
        gl.bindTexture(gl.TEXTURE_2D, Grid1);
		gl.activeTexture(gl.TEXTURE0);
    

       
        //Draw to frame buffer object
        gl.bindFramebuffer(gl.FRAMEBUFFER, fbo2);
        //Call viewport and set to size of render
        gl.viewport(0,0, x_size,y_size);
        gl.drawElements(gl.TRIANGLES, Indices.length, gl.UNSIGNED_SHORT, 0);                
    }
    
    var update_Canvas_Size = function()
    {
        canvas.width  = window.innerWidth -10;
        canvas.height  = window.innerHeight - 100;  //Room for controls
        lblCanvas.innerHTML = "Canvas Resolution is (" + String(window.innerWidth -10) + 
                              ", " + String(window.innerHeight - 100) + ")";
    }
    
    
    var switchtext;
    var switchfbo;
    Reset_Grid();
    Clear_Canvas();
    
    //Update canvas size
    update_Canvas_Size();    
    Draw(Grid1);
    
    
    var then = 0;
    var deltaTime = 1.0;
    var loop = function (now) {
        
        //Calculate fps
        now *= 0.001; //Convert to seconds
        deltaTime = now - then;
        then = now;
        //console.log(lblFPS.innerHTML);
        lblFPS.innerHTML = "FPS: " + String(1/deltaTime);
        
        if (Play){
            for (let i = 0; i < steps; i++) {
                calc_next();
                
                //Swap
                switchtext = Grid1;
                switchfbo = fbo1;
                Grid1 = Grid2;
                fbo1 = fbo2;
                Grid2 = switchtext; 
                fbo2 = switchfbo;
            }

         
            //Display Texture
            Clear_Canvas();
            Draw(Grid1);
        }
        
        //Update canvas size
        //update_Canvas_Size();
                
        requestAnimationFrame(loop);
                       
    };
    requestAnimationFrame(loop);
    
};
