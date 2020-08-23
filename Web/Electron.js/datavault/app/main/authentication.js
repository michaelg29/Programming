const fs = require("fs");

const mainProcess = require("./mainProcess");

// create sheet
mainProcess.setResponse("sheet:create", (e, data) => {
    console.log(data);
});