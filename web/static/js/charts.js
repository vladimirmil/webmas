Chart.defaults.color = "#EFB700";
Chart.defaults.borderColor = "#513E01";


var i = 0;
var air_tempeature = [];
var air_humidity = [];
var light_intensity = [];
var soil_humidity = [];
var date_time = [];
var moist_hum_values = [0, 0]
num = 5;

const labels = [
  'January',
  'February',
  'March',
  'April',
  'May',
  'June',
];

const moistureLabels = [
  'test1',
  'test2',
]

const humidityLabels = [
  'test1',
  'test2',
]

/****************  DATA  *****************/
const moistureData = {
  labels: ['Soil moisture [%]'],
  datasets: [{
    label: 'Soil moisture [%]',
    data: [soil_humidity],
    backgroundColor: [
        'rgba(239, 183, 0, 1)',
        'rgba(38, 38, 38, 1)'
    ],
    borderColor: [
        'rgba(228,174,0,1)',
        'rgba(228,174,0,1)',
    ],
    borderWidth: 1,
  }]
};

const temperatureData = {
  labels: [date_time],
  datasets: [{
    label: 'Temperature [°C]',
    backgroundColor: 'rgba(239, 183, 0, 0.1)',
    borderColor: 'rgba(220, 170, 0, 0.8)',
    pointBackgroundColor: 'rgba(239, 183, 0, 1)',
    data: [air_tempeature],
    fill: true,
    pointRadius: 4
  }]
};

const humidityData = {
  labels: ['Air humidity [%]'],
  datasets: [{
    label: 'Air humidity [%]',
    data: [air_humidity],
    backgroundColor: [
      'rgba(239, 183, 0, 1)',
      'rgba(38, 38, 38, 1)'
    ],
    borderColor: [
      'rgba(228,174,0,1)',
      'rgba(228,174,0,1)',
    ],
    borderWidth: 1
  }]
};

const lightData = {
  labels: [date_time],
  datasets: [{
    label: 'Light intensety [mW/cm2]',
    backgroundColor: 'rgba(239, 183, 0, 0.1)',
    borderColor: 'rgba(220, 170, 0, 0.8)',
    pointBackgroundColor: 'rgba(239, 183, 0, 1)',
    data: [light_intensity],
    fill: true,
    pointRadius: 4
  }]
};


/***************  OPTIONS  *****************/

const moistureOptions= {
  color: '#EFB700',
  rotation: 86 * Math.PI
}


const temperatureOptions = {
  color: '#EFB700',
  animation: {duration: 50, easing: 'linear'},
  scales: {
    x: {
      display: false
    },
    y: {
      min: 0,
      max: 50
    }
  }
}


const humidityOptions= {
  color: '#EFB700',
  rotation: 86 * Math.PI
  /*circumference: 1 * Math.PI*/
}

const lightOptions = {
  color: '#EFB700',
  animation: {duration: 50, easing: 'linear'},
  scales: {
    x: {
      display: false
    },
    y: {
      min: 0,
      max: 480
    }
  }
}

/****************  CONFIG  *****************/
const moistureConfig = {
  type: 'doughnut',
  data: moistureData,
  options: moistureOptions
};

const temperatureConfig = {
  type: 'line',
  data: temperatureData,
  options: temperatureOptions
};

const humidityConfig = {
  type: 'doughnut',
  data: humidityData,
  options: humidityOptions
};

const lightConfig = {
  type: 'line',
  data: lightData,
  options: lightOptions
};

/****************  RENDER  *****************/
var moistureChart = new Chart(
  document.getElementById('moistureChart'),
  moistureConfig
);

var temperatureChart = new Chart(
  document.getElementById('temperatureChart'),
  temperatureConfig
);

var humidityChart = new Chart(
  document.getElementById('humidityChart'),
  humidityConfig
);

var lightChart = new Chart(
  document.getElementById('lightChart'),
  lightConfig
);
/******************************/


function adddata(){
  var getData = $.get('/data1');
  getData.done(function(results){
      i = 19
      for(var a = 0; a < 20; a++){
          date_time[i] = results[a][1];

          moistureChart.data.datasets[0].data[0] = results[0][2];
          moistureChart.data.datasets[0].data[1] = 100 - results[0][2];
          moist_hum_values[0] = results[0][2]

          air_tempeature[i] = results[a][4];
          temperatureChart.data.datasets[0].data[a] = air_tempeature[a];
          temperatureChart.data.labels[i] = date_time[i];

          humidityChart.data.datasets[0].data[0] = results[0][3];
          humidityChart.data.datasets[0].data[1] = 100 - results[0][3];
          moist_hum_values[1] = results[0][3]

          light_intensity[i] = results[a][5];
          lightChart.data.datasets[0].data[a] = light_intensity[a];
          lightChart.data.labels[i] = date_time[i];

          i--;
      }

      document.getElementById('chart1Text').innerText = String(moist_hum_values[0]) + "%";
      document.getElementById('chart2Text').innerText = String(moist_hum_values[1]) + "%";
      
      moistureChart.update();
      temperatureChart.update();
      humidityChart.update();
      lightChart.update();
  });
}

window.setInterval(function(){
  adddata();
}, 200);