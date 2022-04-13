var dateControl = document.querySelector('input[type="date"]');
var timeControl = document.querySelector('input[type="time"]');

const URL = '/this-route'
const xhr = new XMLHttpRequest();

function formatDate(input) {
    var datePart = input.match(/\d+/g),
    year = datePart[0].substring(0,4)
    month = datePart[1], day = datePart[2];
    return year + '-' + month + '-' + day;
}

function getData() {
    var id = formatDate(dateControl.value)
    id += ' ' + timeControl.value
    console.log(id);
    sender = id;
    xhr.open('POST', URL);
    xhr.send(sender);
    //setTimeout(printData(), 1000);
}

function printData() {
    var getData = $.get('/data2');
        getData.done(function(results){

        if(results != null) {
            document.getElementsByName('soil_hum')[0].value = String(results.soilhumidity);
            document.getElementsByName('air_hum')[0].value = String(results.airhumidity);
            document.getElementsByName('temp')[0].value = String(results.temperature);
            document.getElementsByName('illumi')[0].value = String(results.illuminance);
        }
        else{
            document.getElementsByName('soil_hum')[0].value = "no data" ;
            document.getElementsByName('air_hum')[0].value = "no data" ;
            document.getElementsByName('temp')[0].value = "no data" ;
            document.getElementsByName('illumi')[0].value = "no data" ;
        }
    });
}

window.setInterval(function(){
    printData()   
}, 1000);
