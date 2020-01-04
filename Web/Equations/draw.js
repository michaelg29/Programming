inelement = document.querySelector('#input');
outelement = document.querySelector('#output');
btn = document.querySelector('#btn');

var canvas = document.querySelector("#canvas");
var ctx = canvas.getContext("2d");

var rect = canvas.getBoundingClientRect();

//ctx.lineWidth = 1;
ctx.translate(0, canvas.height);
ctx.rotate(Math.PI);
ctx.scale(-1, 1);

let equations = [];
let global_scale = 100; // pixels per unit on axis

var minX = -1;
var minY = -2;
var dx;

var mouse_down;

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

canvas.addEventListener("mousedown", (e) => {
    mouse_down = true;
    lastX = e.clientX - rect.left;
    lastY = rect.bottom - e.clientY;
});
canvas.addEventListener("mouseup", () => mouse_down = false);

var lastX;
var lastY;

canvas.addEventListener("mousemove", (e) => {
    var cursor_x = e.clientX - rect.left;
    var cursor_y = rect.bottom - e.clientY;

    outelement.innerHTML = `${minX + cursor_x / global_scale} ${minY + cursor_y / global_scale}`;

    if (!mouse_down) {
        return;
    }

    // translate graph
    var _dx = cursor_x - lastX;
    var _dy = cursor_y - lastY;

    minX -= _dx / global_scale;
    minY -= _dy / global_scale;

    lastX = cursor_x;
    lastY = cursor_y;

    redraw();
});

canvas.addEventListener("wheel", (e) => {
    var dy = -e.deltaY;

    var cursor_x = e.clientX - rect.left;
    var cursor_y = rect.bottom - e.clientY;
    var x = minX + cursor_x / global_scale;
    var y = minY + cursor_y / global_scale;

    outelement.innerHTML = `${minX + cursor_x / global_scale} ${minY + cursor_y / global_scale}`;

    if (dy > 0) {
        // scale up
        global_scale *= 1.05;
    } else if (dy < 0) {
        // scale down
        global_scale *= .95;
    }

    minX = x - cursor_x / global_scale;
    minY = y - cursor_y / global_scale;

    redraw();
});

function draw(tree) {
    variables.X = minX;
    let previousY = eval(tree);

    dx = 1 / global_scale;

    for (var i = 1, width = canvas.width; i < width; i++) {
        variables.X = minX + i * dx;
        let y = (eval(tree) - minY) * global_scale;

        ctx.beginPath();
        ctx.moveTo(i - 1, previousY);
        ctx.lineTo(i, y);
        ctx.lineWidth = 2;
        ctx.stroke();
        previousY = y;
    }
}

function drawAxes() {
    if (minX >= -canvas.width / global_scale && minX <= canvas.width / global_scale) {
        // draw y-axis
        ctx.beginPath();
        ctx.moveTo(-minX * global_scale, 0);
        ctx.lineTo(-minX * global_scale, canvas.height);
        ctx.lineWidth = 5;
        ctx.stroke();
    }

    if (minY >= -canvas.height / global_scale && minY <= canvas.height / global_scale) {
        // draw x-axis
        ctx.beginPath();
        ctx.moveTo(0, -minY * global_scale);
        ctx.lineTo(canvas.width, -minY * global_scale);
        ctx.lineWidth = 5;
        ctx.stroke();
    }
}

function clearCanvas() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
}

function redraw() {
    clearCanvas();

    drawAxes();

    Array.from(equations).forEach((eqn) => draw(eqn.tree));
}

function reset() {
    equations = [];

    redraw();
}

drawAxes();