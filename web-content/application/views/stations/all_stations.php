<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
<script type="text/javascript">
  google.charts.load('current', { 'packages': ['map'] });
  //google.charts.setOnLoadCallback(drawMap);
  function drawMap(asd) {
    console.log(asd);
    var array = asd.split(",");
    var eka = parseFloat(array[0]);
    var toka = parseFloat(array[1]);
    console.log(eka);
    console.log(toka);
    var data = new google.visualization.DataTable();
      data.addColumn('number', 'Lat');
      data.addColumn('number', 'Long');
      data.addColumn('string', 'Name');
      data.addRow([eka, toka, "ASD"]);

    var options = {
      showTooltip: true,
      showInfoWindow: true,
      zoomLevel:16
    };

    var map = new google.visualization.Map(document.getElementById('map_div'));

    map.draw(data, options);
  };
</script>

<div id="main">
  <p> Stations  </p>
  <table class="dataframe">
    <thread>
      <tr>
        <th>ID</th><th>Designation</th><th>Region</th><th>Description</th><th>Coordinates</th><th>Show on map</th>
      </tr>
    </thread>
    <tbody>
      <?php
         foreach ($a_stations as $s) {
           echo '<tr>';
           echo '<td>'.$s['idStation'].'</td>';
           echo '<td>'.$s['designation'].'</td>';
           echo '<td>'.$s['city'].'</td>';
           echo '<td>'.$s['description'].'</td>';
           echo '<td>'.$s['latitude'].','.$s['longitude'].'</td>';
           echo '<th> <button id="'.$s['latitude'].','.$s['longitude'].'" onclick="drawMap(this.id)">Show</button>';
           echo '</tr>';
         }
         ?>
    </tbody>
  </table>
<div id="map_div"></div>
</div>
