<!--Load the AJAX API-->
<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
<!--jquery-->
<script src="https://code.jquery.com/jquery-3.3.1.min.js"></script>
<script type="text/javascript">
  // Load the Visualization API and the corechart package.
  google.charts.load('current', {'packages':['corechart', 'controls']});

  // Set a callback to run when the Google Visualization API is loaded.
  //google.charts.setOnLoadCallback(load_data);

  function drawDashboard(dbResponse, type){
    var data = new google.visualization.DataTable();
    data.addColumn('date', 'Date'); // X-axis
    data.addColumn('number', 'Temp'); //Y-axis
    data.addColumn('number', 'Humidity'); //Y-axis
    data.addColumn('number', 'Pressure'); //Y-axis
    data.addColumn('number', 'Illuminance'); //Y-axis

    var jsonData = $.parseJSON(dbResponse);

    for (var i = 0; i < jsonData.length; i++) {
      data.addRow([new Date(jsonData[i].timestamp.replace(" ", "T")), parseInt(jsonData[i].temperature), parseInt(jsonData[i].humidity), parseInt(jsonData[i].pressure), parseInt(jsonData[i].illuminance)]);
    }

    // ===============
    // CHART OPTIONS
    // ===============
    var myOptions = {
      chartArea : {width:'90%', height:'85%'},
      backgroundColor: 'transparent',
      colors:['orange','green', 'purple', 'blue'],
      vAxes: {
        0:{title: "Temperature", gridlines: {count: 5}},
        1:{title: "Illuminance",  gridlines: {count: 5, color: 'transparent'}},
      },
      series: {
        0: {type: 'line', targetAxisIndex: 0},
        1: {type: 'line', targetAxisIndex: 1}
      },
      hAxis: {
        gridlines: {
          color: 'grey'
        },
        minorGridlines: {
          count: 0
        },
        textStyle: {
          color: 'grey'
        }
      },
      vAxis: {
        titleTextStyle:{
          color: 'grey'
        },
        gridlines: {
          color: 'grey'
        },
        minorGridlines: {
          count: 0
        },
        textStyle: {
          color: 'grey'
        }
      },
      legend: {
        position: 'top',
        textStyle: {
          color: 'grey'
        }
      }
    }

    var overrides={
      colors:['blue', 'purple'],
      vAxes: {
        0:{title: "Humidity", gridlines: {count: 5}},
        1:{title: "Pressure",  gridlines: {count: 5, color: 'transparent'}},
      },
      series: {
        0: {type: 'line', targetAxisIndex: 0},
        1: {type: 'bars', targetAxisIndex: 1}
      },
    }

    var line2opts = $.extend({}, myOptions, overrides);

    var myDashboard = new google.visualization.Dashboard(document.getElementById('dashboard_div'));

    var myDateSlider = new google.visualization.ControlWrapper({
      'controlType': 'ChartRangeFilter',
      'containerId': 'control_div',
      'options': {
        'filterColumnLabel': 'Date',
        ui: {
          chartOptions: myOptions
        }
      }
    });
    var myLine = new google.visualization.ChartWrapper({
      view: {'columns': [0,1,4]},
      chartType : 'ComboChart',
      containerId : 'line_div',
      options : myOptions
    });
    var myLine2 = new google.visualization.ChartWrapper({
      view: {'columns': [0,2,3]},
      chartType : 'LineChart',
      containerId : 'line2_div',
      options : line2opts
    });
    // Bind myLine to the dashboard, and to the controls
    // this will make sure our line chart is update when our date changes
    myDashboard.bind(myDateSlider, [myLine, myLine2]);
    myDashboard.draw(data);

    var hideTemp = document.getElementById("chkbox_tmp");
    hideTemp.onclick = function(){
      view = new google.visualization.DataView(data);
      if (this.checked){
        myOptions.series[0].lineWidth = 2.0;
      } else {
        myOptions.series[0].lineWidth = 0.0;
      }
      myDashboard.draw(view);
    }
    var hideHum = document.getElementById("chkbox_hum");
    hideHum.onclick = function(){
      view = new google.visualization.DataView(data);
      if (this.checked){
        myOptions.series[1].lineWidth = 2.0;
      } else {
        myOptions.series[1].lineWidth = 0.0;
      }
      myDashboard.draw(view);
    }

  }
</script>
<script>
  $(function () {
    $('form').on('submit', function (e) {
      e.preventDefault();  //prevent normal submit action, AJAX will handle this

      //Get the data from database
      $.ajax({
        type: 'POST',
        url: '<?php echo site_url('stations/ajaxtest'); ?>',
        data: $('form').serialize(),
        success: function (dbResponse) {
          drawDashboard(dbResponse);  //Draw the chart and give the data to it.
        }
      });
    });

    //$("#chkbox_tmp").click(function(){
    //  asd();
    //});
  });

</script>

<div id="main" style="width:85%">
  WIP
  <p><b>Search station by ID and optionally specify a timeframe.<br>
     By default data is fetched from past 7 days.
  </b></p>
  <form method="post">
    Id <input id="station_id" type="text" name="id" required>
    Start <input type="date" name="start"> End <input type="date" name="end"></br>
    <input type="submit" value="Get">
  </form>
  <br>

  <input id="chkbox_tmp" type="checkbox" name="temperature" checked>Temperature
  <input id="chkbox_hum" type="checkbox" name="Humidity" checked> Illuminance

  <br>
  <div id="dashboard_div" style=""></div>
    <div id="control_div" style="height: 130px"></div>
    <div id="line_div" style="display:inline-block; width:95%; height: 350px"></div>
    <div id="line2_div" style="display:inline-block; width:95%;height: 350px"></div>
</div>
