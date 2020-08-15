const { app } = require("electron");

app.on("ready", () => {
    // set initial global variables
    global.activeFile = "";

    // get app data
    global.appData = require("./app.json");

    // start main process
    require("./main/main").createWindow();
});