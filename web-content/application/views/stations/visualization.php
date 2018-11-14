<script src="https://code.jquery.com/jquery-3.3.1.min.js"></script>
<script type="text/javascript">

  // Load the Visualization API and the corechart package.
  google.charts.load('current', {'packages':['corechart', 'controls']});

  // Set a callback to run when the Google Visualization API is loaded.
  //google.charts.setOnLoadCallback(drawDashboard);

  // Callback that creates and populates a data table,
  // instantiates the pie chart, passes in the data and
  $(function () {
    $('form').on('submit', function (e) {
      e.preventDefault();

      $.ajax({
        type: 'POST',
        url: '<?php echo site_url('stations/ajaxtest'); ?>',
        data: $('form').serialize(),
        success: function (dbResponse) {
          var data = new google.visualization.DataTable();
          data.addColumn('date', 'Date'); // X-axis
          data.addColumn('number', 'Temp'); //Y-axis

          var jsonData = $.parseJSON(dbResponse);

          for (var i = 0; i < jsonData.length; i++) {
            data.addRow([new Date(jsonData[i].timestamp.replace(" ", "T")), parseInt(jsonData[i].reading)]);
          }

          // ===============
          // CHART OPTIONS
          // ===============
          var myOptions = {
            chartArea : {width:'90%', height:'85%'},
            backgroundColor: 'transparent',
            colors:['orange','blue'],
            series: {
              0: {type: 'line', targetAxisIndex: 0},
              1: {type: 'line', targetAxisIndex: 1}
            },
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
            'chartType' : 'ComboChart',
            'containerId' : 'line_div',
            options : myOptions
          });
          // Bind myLine to the dashboard, and to the controls
          // this will make sure our line chart is update when our date changes
          myDashboard.bind(myDateSlider, myLine);
          myDashboard.draw(data);
        }
      });
    }); //function drawDashboard() end
  });
</script>

<div class="content">
  <form action="<?php echo site_url('stations/data_visualization'); ?>" method="post">
    Id <input type="text" name="id" required></br>
    Start <input type="text" placeholder="YYYY-MM-DD HH:MM:SS" name="start"> End <input type="text" placeholder="YYYY-MM-DD HH:MM:SS" name="end"></br>
    <select name="types">
      <option value="1">Temperature</option>
      <option value="2">Humidity</option>
    </select>
    <input type="submit" value="Get">
  </form>
  <div id="dashboard_div"></div>
    <div id="control_div" style="width: 800px; height: 100px"></div>
    <div id="line_div" style="height: 400px"></div>
</div>
