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
           echo '</tr>';
         }
         ?>
    </tbody>
  </table> 
</div>
