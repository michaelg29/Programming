const { appData, setResponse } = require("../renderer");
const jQuery = require("jquery");

setResponse(appData["events.renderer.page.init"], (e, data) => {
    console.log("init res");
    jQuery("#test").text(data["text"]);
});