<script type="text/javascript">

    // Load the Visualization API and the corechart package.
    google.charts.load('current', {'packages':['corechart']});

    // Set a callback to run when the Google Visualization API is loaded.
    google.charts.setOnLoadCallback(drawChart);

    // Callback that creates and populates a data table,
    // instantiates the pie chart, passes in the data and
    // draws it.
    function drawChart() {


      var data = new google.visualization.DataTable();
      data.addColumn('string', 'Col1');
      data.addColumn('number', 'Lämmöt');
      data.addRows([
                <?php
                  foreach($toVisualize as $row){
                      echo "['".$row["idMeasurement"]."', ".$row["reading"]."],";
                  }
                ?>
              ]);
      var options = {'title':'AAAAAAAA',
                     'width':600,
                     'height':300};

      // Instantiate and draw our chart, passing in some options.
      var chart = new google.visualization.LineChart(document.getElementById('chart_div'));
      chart.draw(data, options);
    }
  </script>

<div class="content">
  <div id="chart_div"></div>
</div>
