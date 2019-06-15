const electron = require('electron');
const url = require('url');
const path = require('path');
var net = require('net');

const {app, BrowserWindow, Menu, ipcMain} = electron;

// client
var client = new net.Socket();
var offline = true;

// SET ENV
//process.env.NODE_ENV = 'production';

let mainWindow;
let addWindow;
let sendWindow;

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
    client.destroy();
});

// handle create add window
function createAddWindow() {
    // create window
    addWindow = new BrowserWindow({
        width: 300,
        height: 200,
        title: 'Add Shopping List Item',
        webPreferences: {
            nodeIntegration: true
        }
    })
    // load html into window
    addWindow.loadURL(url.format({
        pathname: path.join(__dirname, 'addWindow.html'),
        protocol: 'file:',
        slashes: true,
    }));

    // garbage collection
    addWindow.on('close', function() {
        addWindow = null;
    });
}

// handle create send window
function createSendWindow() {
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
}

// catch window ready
ipcMain.on('window:ready', function(e) {
    // connect client
    client.connect(5500, '127.0.0.1', function() {
        offline = false;
    });
});

// catch item add
ipcMain.on('item:add', function(e, item) {
    mainWindow.webContents.send('item:add', item);
    addWindow.close();
});

// catch send
ipcMain.on('server:send', function(e, data) {
    let send = {
        password: data
    };
    console.log('sending ' + JSON.stringify(send));
    client.write(JSON.stringify(send));
    sendWindow.close();
});

// on data received
client.on('data', function(data) {
    console.log('received:\n' + data);
    var obj = JSON.parse(data);
    if (obj['password']) {
        mainWindow.webContents.send('server:alert', "logged in");
    } else {
        mainWindow.webContents.send('server:alert', "failed login");
    }
});

// on client error
client.on('error', function() {
    mainWindow.webContents.send('server:connection-error');
});

// on client close
client.on('close', function() {
    mainWindow.webContents.send('server:connection-error');
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
                    createAddWindow();
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
                    if (offline) {
                        alert("Cannot connect");
                    } else {
                        createSendWindow();
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