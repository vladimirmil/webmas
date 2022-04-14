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
    id = id.split("-").reverse().join("-");
    id += ' ' + timeControl.value
    id = id.replace('-', '/')
    id = id.replace('-', '/')
    sender = id;
    xhr.open('POST', URL);
    xhr.send(sender);
}

function printData() {
    var getData = $.get('/data2');
        getData.done(function(results){

        if(results != null) {
            document.getElementsByName('soil_hum')[0].value = String(results[2]);
            document.getElementsByName('air_hum')[0].value = String(results[3]);
            document.getElementsByName('temp')[0].value = String(results[4]);
            document.getElementsByName('illumi')[0].value = String(results[5]);
        }
        else{
            document.getElementsByName('soil_hum')[0].value = "nema podataka" ;
            document.getElementsByName('air_hum')[0].value = "nema podataka" ;
            document.getElementsByName('temp')[0].value = "nema podataka" ;
            document.getElementsByName('illumi')[0].value = "nema podataka" ;
        }
    });
}

window.setInterval(function(){
    printData()   
}, 1000);
