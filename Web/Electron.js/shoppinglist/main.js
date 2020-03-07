const electron = require('electron');
const url = require('url');
const path = require('path');
const net = require('net');

const {app, BrowserWindow, Menu, ipcMain, dialog} = electron;

var exports = module.exports = {};

var add = require('./add.js');
var networking = require('./send.js');

exports.sendAlert = function(code) {
    if (mainWindow) {
        mainWindow.webContents.send(code);
    }
};

exports.sendMsg = function(code, data) {
    if (mainWindow) {
        mainWindow.webContents.send(code, data);
    }
};

// SET ENV
//process.env.NODE_ENV = 'production';

let mainWindow;

// listen for app to be ready
app.on('ready', function() {
    // create window
    mainWindow = new BrowserWindow({
        webPreferences: {
            nodeIntegration: true
        }
    });
    // load html into window
    mainWindow.loadURL(url.format({
        pathname: path.join(__dirname, 'mainWindow.html'),
        protocol: 'file:',
        slashes: true,
    }));

    // quit app when closed
    mainWindow.on('closed', function() {
        app.quit();
    });

    // build menu from template
    const mainMenu = Menu.buildFromTemplate(mainMenuTemplate);
    // insert menu
    Menu.setApplicationMenu(mainMenu);
});

app.on('before-quit', function() {
    add.finished();
    networking.finished();
});

// catch window ready
ipcMain.on('window:ready', function(e) {
    add.ready();
    networking.ready();
});

// create menu template
const mainMenuTemplate = [
    {
        label: 'File',
        submenu: [
            {
                label: 'Add Item',
                accelerator: process.platform == 'darwin' ? 'Command+P' : 'Ctrl+P',
                click() {
                    add.createAddWindow();
                }
            },
            {
                label: 'Clear Items',
                accelerator: process.platform == 'darwin' ? 'Command+W' : 'Ctrl+W',
                click() {
                    mainWindow.webContents.send('item:clear');
                }
            },
            {
                label: 'Send Message',
                accelerator: process.platform == 'darwin' ? "Command+S" : "Ctrl+S",
                click() {
                    
                    if (networking.offline) {
                        mainWindow.webContents.send('server:connection-error');
                        const options = {
                            type: 'error', // none, info, error, question
                            buttons: ['Ok'],
                            defaultId: 0,
                            title: 'Alert',
                            message: 'You are currently offline',
                          };
                        
                        dialog.showMessageBox(mainWindow, options);
                    } else {
                        networking.createSendWindow();
                    }
                }
            },
            {
                label: 'Quit',
                accelerator: process.platform == 'darwin' ? 'Command+Q' : 'Ctrl+Q', // shortcuts, darwin is macos
                click() {
                    app.quit();
                }
            }
        ]
    }
];

// if mac, add empty object to menu
if (process.platform == 'darwin') {
    mainMenuTemplate.unshift({}); // add to beginning
}

// add dev tools if not in production
if (process.env.NODE_ENV !== 'production') {
    mainMenuTemplate.push({
        label: 'Developer Tools',
        submenu: [
            {
                label: 'Toggle DevTools',
                accelerator: process.platform == 'darwin' ? 'Command+I' : 'Ctrl+I', // shortcuts, darwin is macos
                click(item, focusedWindow) {
                    focusedWindow.toggleDevTools();
                }
            },
            {
                role: 'reload'
            }
        ]
    })
}