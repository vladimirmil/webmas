Chart.defaults.color = "#EFB700";
Chart.defaults.borderColor = "#513E01";


const TEMPERATURE_MAX_VALUE = 50;
const TEMPERATURE_MIN_VALUE = 0;
const LIGHT_MAX_VALUE = 480;
const LIGHT_MIN_VALUE = 0;

let i = 0;
let air_tempeature = [];
let air_humidity = [];
let light_intensity = [];
let soil_humidity = [];
let date_time = [];
let moist_hum_values = [0, 0];


/****************  DATA  *****************/
function setLineChartData(txt, d, d_date){
  let chart_data = {
    labels: [d_date],
    datasets: [{
      label: txt,
      backgroundColor: 'rgba(239, 183, 0, 0.1)',
      borderColor: 'rgba(220, 170, 0, 0.8)',
      pointBackgroundColor: 'rgba(239, 183, 0, 1)',
      data: [d],
      fill: true,
      pointRadius: 4
    }]
  };
  return chart_data;
}


function setDoughnutChartData(txt, d){
  let chart_data = {
    labels: [txt],
    datasets: [{
    label: txt,
    data: [d],
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
  return chart_data;
}


/***************  OPTIONS  *****************/
function setDoughnutChartOptions(){
  let options= {
    color: '#EFB700',
    rotation: 86 * Math.PI
  };
  return options;
}

function setLineChartOptions(min_y, max_y){
  let options = {
    color: '#EFB700',
    animation: {duration: 50, easing: 'linear'},
    scales: {
      x: {
        display: false
      },
      y: {
        min: min_y,
        max: max_y
      }
    }
  };
  return options;
}


/****************  CONFIG  *****************/
function setChartConfig(t, d, o){
  let config = {
    type: t,
    data: d,
    options: o
  };
  return config;
}


const temperatureData = setLineChartData('Temperature [°C]', air_tempeature, date_time)
const lightData = setLineChartData('Light intensety [mW/cm2]', light_intensity, date_time)
const moistureData = setDoughnutChartData('Soil moisture [%]', soil_humidity)
const humidityData = setDoughnutChartData('Air humidity [%]', air_humidity)

const temperatureOptions = setLineChartOptions(TEMPERATURE_MIN_VALUE, TEMPERATURE_MAX_VALUE)
const lightOptions = setLineChartOptions(LIGHT_MIN_VALUE, LIGHT_MAX_VALUE)
const moistureOptions = setDoughnutChartOptions()
const humidityOptions= setDoughnutChartOptions()


const temperatureConfig = setChartConfig('line', temperatureData, temperatureOptions)
const lightConfig = setChartConfig('line', lightData, lightOptions)
const humidityConfig = setChartConfig('doughnut', humidityData, humidityOptions)
const moistureConfig = setChartConfig('doughnut', moistureData, moistureOptions)



/****************  RENDER  *****************/
const moistureChart = new Chart(
  document.getElementById('moistureChart'),
  moistureConfig
);

const temperatureChart = new Chart(
  document.getElementById('temperatureChart'),
  temperatureConfig
);

const humidityChart = new Chart(
  document.getElementById('humidityChart'),
  humidityConfig
);

const lightChart = new Chart(
  document.getElementById('lightChart'),
  lightConfig
);
/******************************/


function adddata(){
  let getData = $.get('/data1');
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

      document.getElementById('moisture-value').innerText = String(moist_hum_values[0]) + "%";
      document.getElementById('humidity-value').innerText = String(moist_hum_values[1]) + "%";

      moistureChart.update();
      temperatureChart.update();
      humidityChart.update();
      lightChart.update();
  });
}

window.setInterval(function(){
  adddata();
}, 200);