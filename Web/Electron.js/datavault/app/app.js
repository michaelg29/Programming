const { app } = require("electron");
const fs = require("fs");

function processAppData(err, data) {
    if (err) {
        app.quit();
    }

    // load app data
    const appData = JSON.parse(data);
    global.appData = appData;

    // start main process
    require("./main/main").createWindow();
}

app.on("ready", () => {
    console.log(__filename);

    global.appData = require("./app.json");

    require("./main/main").createWindow();

});