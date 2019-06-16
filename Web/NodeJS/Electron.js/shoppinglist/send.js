const electron = require('electron');
const url = require('url');
const path = require('path');
const net = require('net');

const {app, BrowserWindow, Menu, ipcMain} = electron;

var exports = module.exports = {};

var main = require('./main.js');

// client
var client = new net.Socket();
exports.offline = true;

exports.sendWindow;

exports.createSendWindow = function() {
    // create window
    sendWindow = new BrowserWindow({
        width: 300,
        height: 200,
        title: 'Send Message',
        webPreferences: {
            nodeIntegration: true
        }
    })
    // load html into window
    sendWindow.loadURL(url.format({
        pathname: path.join(__dirname, 'sendWindow.html'),
        protocol: 'file:',
        slashes: true,
    }));

    // garbage collection
    sendWindow.on('close', function() {
        sendWindow = null;
    });
};

exports.ready = function() {
    // connect client
    client.connect(5500, '127.0.0.1', function() {
        offline = false;
    });
};

exports.finished = function() {
    client.destroy();
};

// catch send
ipcMain.on('server:send', function(e, data) {
    let send = {
        password: data
    };
    console.log('sending ' + JSON.stringify(send));
    client.write(JSON.stringify(send));
    exports.sendWindow.close();
});

// on data received
client.on('data', function(data) {
    console.log('received:\n' + data);
    var obj = JSON.parse(data);
    if (obj['password']) {
        main.sendMsg('server:alert', "logged in");
    } else {
        main.sendMsg('server:alert', "failed login");
    }
});

// on client error
client.on('error', function() {
    main.sendAlert('server:connection-error');
});

// on client close
client.on('close', function() {
    main.sendAlert('server:connection-error');
    exports.offline = true;
});