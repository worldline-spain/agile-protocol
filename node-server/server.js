const agileSDK = require('agile-sdk');
const Promise = require('promise');
const _ = require('underscore');
const request = require('request');

const CONSTANTS = {
    API: 'http://10.130.2.31:8080',
    HEADERS: {
        'Content-Type': 'application/json'
    },
    DEVICES_REGISTER: [{
            "name": "Z1_ARD2_sensor_temp_stock",
            "id": "0013a20040e7d747",
            "protocol": "XBee_ZigBee",
            "status": ""
        },
        {
            "name": "ZP1_ARD3_alarma",
            "id": "0013a2004155bedb",
            "protocol": "XBee_ZigBee",
            "status": ""
        },
        {
            "name": "Z4_RASP_screen",
            "id": "0013a20040e7d742",
            "protocol": "XBee_ZigBee",
            "status": ""
        },
        {
            "name": "Z2_ARD1_sensor_pres",
            "id": "0013a20040f9a03c",
            "protocol": "XBee_ZigBee",
            "status": ""
        }
    ],
    DEVICE_TYPE: 'ARDUINO',
    INPUT_SENSOR_METRIC: {
        xbee_zigbee0013a20040e7d747: ["temperature", "stock"],
        xbee_zigbee0013a20040f9a03c: ["presence"]
    },
    ALERT_SENSOR: {
        id: "0013a2004155bedb"
    },
    SCREEN_DISPLAY: {
        id: "0013a20040e7d742"
    }
};

const agile = agileSDK({
    api: CONSTANTS.API
});

initialize();

function initialize() {
    Promise.all(createDevices()).then(() => {
        getDevices().then((idDevices) => {
            getDataDevices(idDevices);
            setInterval(() => getDataDevices(idDevices), 1000);
        }, () => {
            console.log('ERROR: Getting devices, re-trying create in 60 seconds...');
            setTimeout(initialize, 60000); // 60sec
        });
    }, (err) => {
        console.log(err);
    });
}

function createDevices() {
    let arrayPromise = [];
    CONSTANTS.DEVICES_REGISTER.forEach(device => arrayPromise.push(_createDevice(device)));
    return arrayPromise;
}

function _createDevice(device) {
    return new Promise((resolve, reject) => {
        agile.deviceManager.create(device, CONSTANTS.DEVICE_TYPE).then(() => resolve(), () => resolve());
    });
}

function getDevices() {
    let idDevices = [];
    return new Promise((resolve, reject) => {
        agile.deviceManager.get().then(devices => {
            CONSTANTS.DEVICES_REGISTER.every((deviceConst) => {
                var deviceFound = _.find(devices, device => device.address === deviceConst.id)
                if (deviceFound) {
                    console.log('INFO: device with name ' + deviceConst.name + ' found!');
                    if (CONSTANTS.INPUT_SENSOR_METRIC[deviceFound.deviceId]) {
                        idDevices.push(deviceFound.deviceId);
                    }
                    return true; // continue with the iteration
                } else {
                    return false; // break iteration
                    reject();
                }
            });
            resolve(idDevices);
        }, (err) => reject());
    });
}

function getDataDevices(idDevices) {
    idDevices.forEach(idDevice => {
        let arrayMetric = CONSTANTS.INPUT_SENSOR_METRIC[idDevice];
        arrayMetric.forEach(metric => {
            agile.device.get(idDevice, metric).then(function (deviceComponent) {
                console.log(deviceComponent);
                sendScreen(JSON.parse(deviceComponent.value));
            });
        });
    });
}

function sendAlert() {
    _write(CONSTANTS.ALERT_SENSOR.id,{}).then(() => {
        console.log('INFO: Alert send.');
    }, () => {
        console.log('ERROR: cannot send the message to the device.');
    })
}

function sendScreen(data) {
    _write(CONSTANTS.SCREEN_DISPLAY.id, data).then(() => {
        console.log('INFO: Metric send.');
    }, () => {
        console.log('ERROR: cannot send the message to the device.');
    })
}

function _write(idDevice, data) {
    return new Promise((resolve, reject) => {
        let options = {
            url: CONSTANTS.API + '/api/device/' + idDevice,
            headers: CONSTANTS.HEADERS,
            body: JSON.stringify(data)
        }
        
        request.post(options, (error, response, body) => {
            if(error) {
                reject();
            } else {
                resolve();
            }
        })
    });    
}