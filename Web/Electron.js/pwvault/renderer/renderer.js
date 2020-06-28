const jQuery = require("jQuery");
const { ipcRenderer, remote } = require("electron");
const { dialog } = remote;

const appData = remote.getGlobal("appData");
const routes = require("./html/routes.json");

/*
    Routing
*/

// load html page
function goto(route) {

    let path;
    if (typeof route == "string") {
        // route passed in as string
        path = route;
    }
    else {
        // passed in dictionary format
        path = route.route;
    }

    if (Object.keys(routes).includes(path)) {
        // path contained in route dictionary
        jQuery("main").load(`${routes[path]}`);
        if (Object.keys(route).includes("args")) {
            // data passed in with route
            console.log("init req");
            triggerWithData(appData["events.renderer.page.init"], route.args);
        }
        return true;
    }
    return false;
}

// listener for main process signal
setResponse(appData["events.renderer.route.request"], (e, route) => {
    sendData(goto(route) ? appData["events.renderer.route.success"] :
        appData["events.renderer.route.success"], route);
});

/*
    Signals
*/

// send alert to backend
function sendAlert(alert) {
    ipcRenderer.send(alert);
}

// send data to backend
function sendData(msg, data) {
    ipcRenderer.send(msg, data);
}

// set response to event
function setResponse(trigger, response) {
    ipcRenderer.on(trigger, response);
}

// trigger frontend event
function trigger(event) {
    remote.BrowserWindow.getFocusedWindow().webContents.send(event);
}

// trigger frontend event with data
function triggerWithData(event, data) {
    remote.BrowserWindow.getFocusedWindow().webContents.send(event, data);
}

// tell backend that renderer is ready
sendAlert(appData["events.renderer.ready"]);


/*
    Exports
*/

module.exports = {
    appData,
    goto,
    sendAlert,
    sendData,
    setResponse,
    trigger,
    triggerWithData
};