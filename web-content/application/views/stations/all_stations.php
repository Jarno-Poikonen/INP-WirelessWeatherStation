<div class="content">
  <p> Tässä sitten kaikki asemat </p>
  <table class="dataframe">
    <thread>
      <tr>
        <th>ID</th><th>Designation</th><th>Location</th><th>coordinates</th><th>Show on map</th>
      </tr>
    </thread>
    <tbody>
      <?php
         foreach ($a_stations as $s) {
           echo '<tr>';
           echo '<td>'.$s['idStation'].'</td>';
           echo '<td>'.$s['designation'].'</td>';
           echo '<td>'.$s['location'].'</td>';
           echo '<td>'.$s['latitude'].','.$s['longitude'].'</td>';
           echo '</tr>';
         }
         ?>
    </tbody>
  </table>
</div>
