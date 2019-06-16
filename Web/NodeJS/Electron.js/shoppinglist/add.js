const electron = require('electron');
const url = require('url');
const path = require('path');

const { app, BrowserWindow, Menu, ipcMain } = electron;

var exports = module.exports = {};

exports.addWindow;

exports.createAddWindow = function() {
    // create window
    exports.addWindow = new BrowserWindow({
        width: 300,
        height: 200,
        title: 'Add Shopping List Item',
        webPreferences: {
            nodeIntegration: true
        }
    })

    // load html into window
    exports.addWindow.loadURL(url.format({
        pathname: path.join(__dirname, 'addWindow.html'),
        protocol: 'file:',
        slashes: true,
    }));

    // garbage collection
    exports.addWindow.on('close', function() {
        addWindow = null;
    });
}

exports.ready = function() {

};

exports.finished = function() {

};