function processData(json, upcoming) {


    json.forEach(element => {
        global.launchNames.push(element["mission_name"]);
        var data = {
            "upcoming": upcoming,
            "listIdx": parseInt(element["flight_number"]) - 1
        }
        global.nameData[element["mission_name"]] = data;
    });

    if (upcoming) {
        global.upcomingLaunches = json;
    } else {
        global.pastLaunches = json;
    }
}

export async function fetchData() {
    // reset values
    global.dataLoaded = false;
    global.upcomingLaunches = [];
    global.pastLaunches = [];
    global.launchNames = [];
    global.nameData = {};

    // get past launches
    let response = await fetch("https://api.spacexdata.com/v3/launches/past");
    processData(await response.json(), false);

    // get future launches
    response = await fetch("https://api.spacexdata.com/v3/launches/upcoming");
    processData(await response.json(), true);

    // set loaded
    global.dataLoaded = true;
}