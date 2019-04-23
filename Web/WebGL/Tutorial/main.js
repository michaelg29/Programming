var cubeRotation = 0.0;

main();

//
// start here
//
function main() {
    // Vertex shader program
    const vsSource = `
        attribute vec4 aVertexPosition;
        attribute vec2 aTextureCoord;

        uniform mat4 uModelViewMatrix;
        uniform mat4 uProjectionMatrix;

        varying lowp vec2 vTextureCoord;

        void main(void) {
            gl_Position = uProjectionMatrix * uModelViewMatrix * aVertexPosition;
            vTextureCoord = aTextureColor;
        }
    `;

    // Fragment shader program
    const fsSource = `
        varying lowp vec2 vTextureCoord;

        uniform sampler2D uSampler;

        void main(void) {
            gl_FragColor = texture2D(uSampler, vTextureCoord);
        }
    `;

    const canvas = document.querySelector("#glCanvas");

    // Initialize GL Context
    const gl = canvas.getContext("webgl");

    if (gl === null) {
        alert("Unable to initialize WebGL. May not be supported.");
        return;
    }

    // set clear color to black, fully opaque
    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    // clear the color buffer with specified clear color
    gl.clear(gl.COLOR_BUFFER_BIT);

    const shaderProgram = initShaderProgram(gl, vsSource, fsSource);

    const programInfo = {
        program: shaderProgram,
        attribLocations: {
            vertexPosition: gl.getAttribLocation(shaderProgram, 'aVertexPosition'),
            vertexColor: gl.getAttribLocation(shaderProgram, 'aTextureCoord'),
        },
        uniformLocations: {
            projectionMatrix: gl.getUniformLocation(shaderProgram, 'uProjectionMatrix'),
            modelViewMatrix: gl.getUniformLocation(shaderProgram, 'uModelViewMatrix'),
            uSampler: gl.getUniformLocation(shaderProgram, 'uSampler');
        }
    }

    const buffers = initBuffers(gl);

    const texture = loadTexture(gl, 'cubetexture.png');

    var then = 0;

    // draw scene repeatedly
    function render(now) {
        now *= 0.001;       // convert to seconds
        const dt = now - then;
        then = now;

        drawScene(gl, programInfo, buffers, dt);

        requestAnimationFrame(render);
    }

    requestAnimationFrame(render);
}

//
// Initialize shader program so WebGL can draw data
//
function initShaderProgram(gl, vsSource, fsSource) {
    const vertexShader = loadShader(gl, gl.VERTEX_SHADER, vsSource);
    const fragmentShader = loadShader(gl, gl.FRAGMENT_SHADER, fsSource);

    // create shader program
    const shaderProgram = gl.createProgram();
    gl.attachShader(shaderProgram, vertexShader);
    gl.attachShader(shaderProgram, fragmentShader);
    gl.linkProgram(shaderProgram);

    // check for creation
    if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
        alert('Unable to initialize shader program: ' + gl.getProgramInfoLog(shaderProgram));
        return null;
    }

    return shaderProgram;
}

//
// creates shader of given type, uploads source, compiles source
//
function loadShader(gl, type, source) {
    const shader = gl.createShader(type);

    // send source to shader
    gl.shaderSource(shader, source);

    // compile shader program
    gl.compileShader(shader);

    // check for compilation
    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        alert('An error occurred compiling the shaders: ' + gl.getShaderInfoLog(shader));
        gl.deleteShader(shader);
        return null;
    }

    return shader;
}

//
// create buffer with vertex positions
//
function initBuffers(gl) {
    // create buffer for square's positions
    const positionBuffer = gl.createBuffer();

    // select positionBuffer as one to apply buffer ops to
    gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);

    // create array of positions
    const positions = [
        // Front face
        -1.0, -1.0,  1.0,
         1.0, -1.0,  1.0,
         1.0,  1.0,  1.0,
        -1.0,  1.0,  1.0,
        
        // Back face
        -1.0, -1.0, -1.0,
        -1.0,  1.0, -1.0,
         1.0,  1.0, -1.0,
         1.0, -1.0, -1.0,
        
        // Top face
        -1.0,  1.0, -1.0,
        -1.0,  1.0,  1.0,
         1.0,  1.0,  1.0,
         1.0,  1.0, -1.0,
        
        // Bottom face
        -1.0, -1.0, -1.0,
         1.0, -1.0, -1.0,
         1.0, -1.0,  1.0,
        -1.0, -1.0,  1.0,
        
        // Right face
         1.0, -1.0, -1.0,
         1.0,  1.0, -1.0,
         1.0,  1.0,  1.0,
         1.0, -1.0,  1.0,
        
        // Left face
        -1.0, -1.0, -1.0,
        -1.0, -1.0,  1.0,
        -1.0,  1.0,  1.0,
        -1.0,  1.0, -1.0,
    ];

    // pass position list to webgl to build shape
    gl.bufferData(gl.ARRAY_BUFFER,
        new Float32Array(positions),
        gl.STATIC_DRAW);

    // set up texture coordinates for faces
    const textureCoordBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, textureCoordBuffer);

    const textureCoordinates = [
        // Front
        0.0,  0.0,
        1.0,  0.0,
        1.0,  1.0,
        0.0,  1.0,
        // Back
        0.0,  0.0,
        1.0,  0.0,
        1.0,  1.0,
        0.0,  1.0,
        // Top
        0.0,  0.0,
        1.0,  0.0,
        1.0,  1.0,
        0.0,  1.0,
        // Bottom
        0.0,  0.0,
        1.0,  0.0,
        1.0,  1.0,
        0.0,  1.0,
        // Right
        0.0,  0.0,
        1.0,  0.0,
        1.0,  1.0,
        0.0,  1.0,
        // Left
        0.0,  0.0,
        1.0,  0.0,
        1.0,  1.0,
        0.0,  1.0,
    ];
    
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(textureCoordinates), gl.STATIC_DRAW);
    
    const indexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer);

    // array defines face as two triangles
    const indices = [
        0,  1,  2,      0,  2,  3,    // front
        4,  5,  6,      4,  6,  7,    // back
        8,  9,  10,     8,  10, 11,   // top
        12, 13, 14,     12, 14, 15,   // bottom
        16, 17, 18,     16, 18, 19,   // right
        20, 21, 22,     20, 22, 23,   // left
    ];

    // send array to gl
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER,
        new Uint16Array(indices), gl.STATIC_DRAW);

    return {
        position: positionBuffer,
        textureCoord: textureCoordBuffer,
        indices: indexBuffer,
    }
}

//
// render scene
//
function drawScene(gl, programInfo, buffers, dt) {
    gl.clearColor(0.0, 0.0, 0.0, 1.0);  // clear to black
    gl.clearDepth(1.0);                 // clear everything
    gl.enable(gl.DEPTH_TEST);           // enable depth testing
    gl.depthFunc(gl.LEQUAL);            // near things obscure far things

    // clear canvas before drawing
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    // create perspective matrix to stimulate distortion of perspecitve
    // fov is 45 deg, width:height matches that of canvas
    // see units between 0.1 and 100 units away from camera
    const fieldOfView = 45 * Math.PI / 180;
    const aspect = gl.canvas.clientWidth / gl.canvas.clientHeight;
    const zNear = 0.1;
    const zFar = 100.0;
    const projectionMatrix = mat4.create();

    mat4.perspective(projectionMatrix,
                    fieldOfView,
                    aspect,
                    zNear,
                    zFar);

    // set drawing position to "identity" point (center of screen)
    const modelViewMatrix = mat4.create();

    // move drawing position to square
    mat4.translate(modelViewMatrix,     // destination matrix
                    modelViewMatrix,    // matrix to translate
                    [-0.0, 0.0, -6.0]); // amount to translate
    // rotate cube
    mat4.rotate(modelViewMatrix,    // destination matrix
        modelViewMatrix,            // matrix to rotate
        cubeRotation,               // amount to rotate in radians
        [0, 0, 1]);                 // axis to rotate around (Z)
    mat4.rotate(modelViewMatrix,    // destination matrix
            modelViewMatrix,        // matrix to rotate
        cubeRotation * .7,          // amount to rotate in radians
            [0, 1, 0]);             // axis to rotate around (X)

    // tell webgl how to parse positions from position buffer
    {
        const numComponents = 3;    // pull out 2 vals per iteration
        const type = gl.FLOAT;      // data in buffer is 32bit float
        const normalize = false;    // don't normalize
        const stride = 0;           // how many bytes to get from one set of values to next
        
        const offset = 0;           // bytes inside buffer to start from
        gl.bindBuffer(gl.ARRAY_BUFFER, buffers.position);
        gl.vertexAttribPointer(
            programInfo.attribLocations.vertexPosition,
            numComponents,
            type,
            normalize,
            stride,
            offset
        );
        gl.enableVertexAttribArray(
            programInfo.attribLocations.vertexPosition
        );
    }

    // tell webgl how to parse colors
    {
        const numComponents = 4;
        const type = gl.FLOAT;
        const normalize = false;
        const stride = 0;
        const offset = 0;
        gl.bindBuffer(gl.ARRAY_BUFFER, buffers.color);
        gl.vertexAttribPointer(
            programInfo.attribLocations.vertexColor,
            numComponents,
            type,
            normalize,
            stride,
            offset
        );
        gl.enableVertexAttribArray(
            programInfo.attribLocations.vertexColor
        );
    }

    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, buffers.indices);

    // tell webgl to use program
    gl.useProgram(programInfo.program);

    // set shader uniforms
    gl.uniformMatrix4fv(
        programInfo.uniformLocations.projectionMatrix,
        false,
        projectionMatrix
    );
    gl.uniformMatrix4fv(
        programInfo.uniformLocations.modelViewMatrix,
        false,
        modelViewMatrix
    );

    {
        const vertexCount = 36;
        const type = gl.UNSIGNED_SHORT;
        const offset = 0;
        gl.drawElements(gl.TRIANGLES, vertexCount, type, offset);
    }

    // update rotation for next draw
    cubeRotation += dt;
}

//
// initialize texture and load image
//
function loadTexture(gl, url) {
    const texture = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D, texture);

    // put single pixel as placeholder for image while it loads
    const level = 0;
    const internalFormat = gl.RGBA;
    const width = 1;
    const height = 1;
    const border = 0;
    const srcFormat = gl.RGBA;
    const srcType = gl.UNSIGNED_BYTE;
    const pixel = new Uint8Array([0, 0, 255, 255]); // opaque blue
    gl.texImage2D(gl.TEXTURE_2D, level, internalFormat,
        width, height, border, srcFormat, srcType,
        pixel);

    const image = new Image();
    image.onload = function() {
        gl.bindTexture(gl.TEXTURE_2D, texture);
        gl.texImage2D(gl.TEXTURE_2D, level, internalFOrmat,
            srcFormat, srcType, image);

        // different requirements for power of 2 vs non power of 2
        if (isPowerOf2(image.width) && isPowerOf2(image.height)) {
            // is power of 2, generate mips
            gl.generateMipmap(gl.TEXTURE_2D);
        } else {
            // not power of 2, turn off mips, set wrapping to clamp to edge
            // gl.NEAREST is also allowed, instead of gl.LINEAR, as neither mipmap.
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
            // Prevents s-coordinate wrapping (repeating).
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
            // Prevents t-coordinate wrapping (repeating).
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
        }
    }
    image.src = url;

    return texture;
}

function isPowerOf2(value) {
    return (value & (value - 1)) === 0;
}