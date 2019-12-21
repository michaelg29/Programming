inelement = document.querySelector('#input');
outelement = document.querySelector('#output');
btn = document.querySelector('#btn');

var canvas = document.querySelector("#canvas");
var ctx = canvas.getContext("2d");

//ctx.lineWidth = 1;
ctx.translate(0, canvas.height);
ctx.rotate(Math.PI);
ctx.scale(-1, 1);

let equations = [];
let global_scale = 1;

function draw(tree) {
    variables.X = 0;
    let previousY = eval(tree);

    for (var i = 0, width = canvas.width; i < width; i++) {
        variables.X = minX + i * global_scale;
        let y = eval(tree);

        ctx.beginPath();
        ctx.moveTo(variables.X - 1, previousY);
        ctx.lineTo(variables.X, y);
        ctx.lineWidth = 2;
        ctx.stroke();
        previousY = y;
    }
}

btn.addEventListener('click', (e) => {
    let eqn = inelement.value;
    let rpn = RPN(eqn);

    var out;

    if (rpn) {
        let tree = parse(rpn);
        equations.push({
            eqn: eqn,
            tree: tree
        });
        draw(tree);
        out = `Graphing y = ${eqn}`;
    } else {
        out = 'could not parse equation';
    }

    outelement.innerHTML = out;
});

let mousedown = false;
var lastX;
var lastY;
var minX = 0;
var minY = 0;

document.addEventListener("mousedown", (e) => mousedown = true);
document.addEventListener("mouseup", (e) => mousedown = false);
drawAxes();

function clearCanvas() {
    ctx.clearRect(minX, minY, canvas.width, canvas.height);
    outelement.innerHTML = `${minX} ${minY}`;
}

function drawAxes() {
    if (minX < canvas.width && minX > -canvas.width) {
        ctx.beginPath();
        ctx.moveTo(0, minY);
        ctx.lineTo(0, minY + canvas.height);
        ctx.lineWidth = 5;
        ctx.stroke();
    }

    if (minY < canvas.height && minY > -canvas.height) {
        ctx.beginPath();
        ctx.moveTo(minX, 0);
        ctx.lineTo(minX + canvas.width, 0);
        ctx.lineWidth = 5;
        ctx.stroke();
    }
}

function redraw() {
    clearCanvas();

    drawAxes();

    Array.from(equations).forEach((eqn) => draw(eqn.tree));
}

canvas.addEventListener("mousemove", (e) => {
    if (mousedown) {
        var dx = lastX - e.clientX;
        var dy = lastY - e.clientY;

        ctx.translate(-dx, dy);
        lastX = e.clientX;
        lastY = e.clientY;

        minX += dx;
        minY -= dy;

        // clear canvas
        redraw();
    }
});

canvas.addEventListener("mousedown", (e) => {
    lastX = e.clientX;
    lastY = e.clientY;
});

// canvas.addEventListener("wheel", (e) => {
//     var dy = e.deltaY;
//     var scale = Math.abs(dy) / 50;
//     let x = e.clientX;
//     let y = e.clientY;

//     if (dy > 0) {
//         // scale up
//         minX += x * scale;
//         minY += y * scale;
//         global_scale -= scale;
//     } else if (dy < 0) {
//         // scale down
//         minX -= x * scale;
//         minY -= y * scale;
//         global_scale += scale;
//     }

//     clearCanvas();
// });