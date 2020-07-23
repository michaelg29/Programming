const { app, BrowserWindow } = require("electron");

const mainProcess = require("./mainProcess");

app.name = global.appData["app.name"];

let win;

function createWindow() {
    win = new BrowserWindow({
        width: 1200,
        height: 800,
        webPreferences: {
            nodeIntegration: true,
            enableRemoteModule: true
        },
        icon: global.appData["app.icon"],
        title: global.appData["app.name"]
    });

    // load main file
    win.loadFile(global.appData["app.entry"]);

    // open dev tools
    win.webContents.openDevTools();

    win.on("closed", () => {
        // dereference object
        win = null;
    });
}

app.on("window-all-closed", () => {
    if (process.platform != "darwin") {
        app.quit();
    }
});

app.on("activate", () => {
    if (win == null) {
        createWindow();
    }
});

mainProcess.setResponse("events.renderer.ready", () => {
    mainProcess.sendData("events.renderer.route.request", {
        route: appData["app.initialRoute"],
        args: {
            text: "this is the test from the main bois"
        }
    });

    const express = require('express');
    const app = express();
    const port = 3000;

    app.get('/get', (req, res) => {
        global.res = res;
        console.log("got request");
    });

    app.listen(port, () => console.log(`Example app listening at http://localhost:${port}`));
});

module.exports = {
    createWindow
}