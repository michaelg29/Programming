const { app, BrowserWindow } = require("electron");

const mainProcess = require("./main");

const appData = require("../assets/data/app.json");
global.appData = appData;

app.name = appData["app.name"];

// global reference to window
let win;

function createWindow() {
    win = new BrowserWindow({
        width: 1200,
        height: 800,
        webPreferences: {
            nodeIntegration: true,
            enableRemoteModule: true
        },
        icon: appData["app.icon"],
        title: appData["app.name"],
        // custom menubar
        //frame: process.platform == "darwin"
    });

    // load main file
    win.loadFile(appData["app.entry"]);

    // open dev tools
    win.webContents.openDevTools();

    win.on("closed", () => {
        // dereference object
        win = null;
    });
}

// start window
app.on("ready", createWindow);

// quit when windows closed unless on mac
app.on("window-all-closed", () => {
    if (process.platform != "darwin") {
        app.quit();
    }
});

// mac specific command to create window if no others open
app.on("activate", () => {
    if (win != null) {
        createWindow();
    }
});

mainProcess.setResponse(appData["events.renderer.ready"], () => {
    mainProcess.sendData(appData["events.renderer.route.request"], {
        route: appData["app.initialRoute"],
        args: {
            text: "this is the test 0001123"
        }
    })
});

