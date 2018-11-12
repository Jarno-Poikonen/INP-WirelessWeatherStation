<script type="text/javascript">

  // Load the Visualization API and the corechart package.
  google.charts.load('current', {'packages':['corechart', 'controls']});

  // Set a callback to run when the Google Visualization API is loaded.
  google.charts.setOnLoadCallback(drawDashboard);

  // Callback that creates and populates a data table,
  // instantiates the pie chart, passes in the data and

  function drawDashboard() {
    // Instantiate and draw our chart, passing in some options.
    var myDashboard = new google.visualization.Dashboard(document.getElementById('dashboard_div'));

    var myOptions = {
      chartArea : {width:'90%', height:'85%'},
      backgroundColor: 'transparent',
      colors:['orange','green'],
      hAxis: {
        gridlines: {
          color: 'white',
          count: 8
        },
        minorGridlines: {
          count: 0
        },
        textStyle: {
          color: 'white'
        }
      },
      vAxis: {
        gridlines: {
          color: 'white',
          count: 8
        },
        minorGridlines: {
          count: 0
        },
        textStyle: {
          color: 'white'
        }
      },
      legend: {
        position: 'top',
        textStyle: {
          color: 'white'
        }
      }
    }

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
      'chartType' : 'LineChart',
      'containerId' : 'line_div',
      options : myOptions
    });

    // ===============
    // DATA
    // ===============
    var data = new google.visualization.DataTable();
    data.addColumn('date', 'Date');
    data.addColumn('number', 'Temp');
    data.addRows([
      <?php
              foreach($toVisualize as $row){
                echo "[new Date('".str_replace(' ','T', $row["timestamp"])."'), ".$row["reading"]."],";
              }
              ?>
      ]);
    // ===============

    // Bind myLine to the dashboard, and to the controls
    // this will make sure our line chart is update when our date changes
    myDashboard.bind(myDateSlider, myLine);

    myDashboard.draw(data);
  }

</script>

<div class="content">
  <form action="<?php echo site_url('stations/data_visualization'); ?>" method="post">
    Id <input type="text" name="id" required></br>
    Timeperiod <input type="text" name="period"></br>
    <input type="submit" value="Get">
  </form>
  <div id="dashboard_div"></div>
    <div id="control_div" style="width: 800px; height: 100px"></div>
    <div id="line_div" style="height: 400px"></div>
</div>
