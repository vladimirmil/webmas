    var i = 0;
    var air_tempeature = [];
    var air_humidity = [];
    var light_intensity = [];
    var soil_humidity = [];
    var date_time = [];
    num = 5;

    Chart.defaults.global.defaultFontColor = "#fff";

    var canvas_soil_humid = document.getElementById('soilhumiditychart');
    var data_soil_humid = {
        labels: [date_time],
        datasets: [{
            label: "Vlažnost zemljišta [ % ]",
            data: [soil_humidity],
            backgroundColor: ['rgba(255, 203, 5, 0.25)'],
            borderColor: ['rgba(255, 203, 5, 1)'],
            borderWidth: 1,
            pointBorderColor: 'rgba(255, 203, 5, 1)',
            pointBackgroundColor: 'rgba(255, 203, 5, 0.25)',
            fill: true,
            pointRadius: 4
        }]
    }

    var canvas_air_temp = document.getElementById('airtempchart');
    var data_air_temp = {
        labels: [date_time],
        datasets: [{
            label: "Temperatura vazduha [ °C ]",
            data: [air_tempeature],
            backgroundColor: ['rgba(255, 203, 5, 0.25)'],
            borderColor: ['rgba(255, 203, 5, 1)'],
            borderWidth: 1,
            pointBorderColor: 'rgba(255, 203, 5, 1)',
            pointBackgroundColor: 'rgba(255, 203, 5, 0.25)',
            fill: true,
            pointRadius: 4

        }]
    }

    var canvas_air_humid = document.getElementById('airhumiditychart');
    var data_air_humid = {
        labels: [date_time],
        datasets: [{
            label: "Vlažnost vazduha[ % ]",
            data: [air_humidity],
            backgroundColor: ['rgba(255, 203, 5, 0.25)'],
            borderColor: ['rgba(255, 203, 5, 1)'],
            borderWidth: 1,
            pointBorderColor: 'rgba(255, 203, 5, 1)',
            pointBackgroundColor: 'rgba(255, 203, 5, 0.25)',
            fill: true,
            pointRadius: 4
        }]
    }

    var canvas_light = document.getElementById('lightchart');
    var data_light = {
        labels: [date_time],
        datasets: [{
            label: "Intenzitet svetlosti[ Lx ]",
            data: [light_intensity],
            backgroundColor: ['rgba(255, 203, 5, 0.25)'],
            borderColor: ['rgba(255, 203, 5, 1)'],
            borderWidth: 1,
            pointBorderColor: 'rgba(255, 203, 5, 1)',
            pointBackgroundColor: 'rgba(255, 203, 5, 0.25)',
            fill: true,
            pointRadius: 4
        }]
    }


    var option_soil_humid = {
        hover: () => {},
	    showLines: true,
        responsive: true,
        animation: {duration: 50, easing: 'linear'},
         scales: {
            yAxes: [{
                display: true,
                gridLines: {color: 'rgba(rgba(255, 203, 5, 0.15)'},
                ticks: {beginAtZero: true, min: 0, max: 100, stepSize: 5}
            }],
            xAxes:[{
                display: true,
                gridLines: {color: 'rgba(255, 203, 5, 0.15'},
				ticks: {maxTicksLimit: 10, fontSize: 8}
            }]
        }             
    };

    var option_air_temp = {
        hover: () => {},
	    showLines: true,
        responsive: true,
        animation: {duration: 50, easing: 'linear'},
         scales: {
            yAxes: [{
                display: true,
                gridLines: {color: 'rgba(rgba(255, 203, 5, 0.15)'},
                ticks: {beginAtZero: true, min: 0, max: 50, stepSize: 5}
            }],
            xAxes:[{
                display: true,
                gridLines: {color: 'rgba(255, 203, 5, 0.15'},
				ticks: {maxTicksLimit: 10, fontSize: 8}
            }]
        }             
    };

    var option_air_humid = {
        hover: () => {},
	    showLines: true,
        responsive: true,
        animation: {duration: 50, easing: 'linear'},
         scales: {
            yAxes: [{
                display: true,
                gridLines: {color: 'rgba(rgba(255, 203, 5, 0.15)'},
                ticks: {beginAtZero: true, min: 0, max: 100, stepSize: 5}
            }],
            xAxes:[{
                display: true,
                gridLines: {color: 'rgba(255, 203, 5, 0.15'},
				ticks: {maxTicksLimit: 10, fontSize: 8}
            }]
        }             
    };

    var option_light = {
        hover: () => {},
	    showLines: true,
        responsive: true,
        animation: {duration: 50, easing: 'linear'},
         scales: {
            yAxes: [{
                display: true,
                gridLines: {color: 'rgba(rgba(255, 203, 5, 0.15)'},
                ticks: {beginAtZero: true, min: 0, max: 480, stepSize: 5}
            }],
            xAxes:[{
                display: true,
                gridLines: {color: 'rgba(255, 203, 5, 0.15'},
				ticks: {maxTicksLimit: 10, fontSize: 8}
            }]
        }             
    };

    var line_chart_soil_humid = Chart.Line(canvas_soil_humid,{data:data_soil_humid,options:option_soil_humid});
    var line_chart_air_temp = Chart.Line(canvas_air_temp,{data:data_air_temp,options:option_air_temp});
    var line_chart_air_humid = Chart.Line(canvas_air_humid,{data:data_air_humid,options:option_air_humid});
    var line_chart_light = Chart.Line(canvas_light,{data:data_light,options:option_light});
    
    
    function adddata(){
        var getData = $.get('/data1');
        getData.done(function(results){
            i = 19
            for(var a = 0; a < 20; a++){
				date_time[i] = results[a][1];
				
				soil_humidity[i] = results[a][2];
                line_chart_soil_humid.data.datasets[0].data[a] = soil_humidity[a];
                line_chart_soil_humid.data.labels[i] = date_time[i];

				air_tempeature[i] = results[a][4];
                line_chart_air_temp.data.datasets[0].data[a] = air_tempeature[a];
                line_chart_air_temp.data.labels[i] = date_time[i];

				air_humidity[i] = results[a][3];
                line_chart_air_humid.data.datasets[0].data[a] = air_humidity[a];
                line_chart_air_humid.data.labels[i] = date_time[i];

				light_intensity[i] = results[a][5];
                line_chart_light.data.datasets[0].data[a] = light_intensity[a];
                line_chart_light.data.labels[i] = date_time[i];

                i--;
            }

            line_chart_soil_humid.update();
            line_chart_air_temp.update();
            line_chart_air_humid.update();
            line_chart_light.update();
        });
    }
    
    window.setInterval(function(){
        adddata();
    }, 200);
    