<div class="content">
  <p> Tässä sitten kaikki asemat </p>
  <table class="dataframe">
    <thread>
      <tr>
        <th>ID</th><th>Designation</th><th>Location</th><th>coordinates</th>
      </tr>
    </thread>
    <tbody>
      <?php
         foreach ($a_stations as $s) {
           echo '<tr>';
           echo '<td>'.$s['stations_id'].'</td>';
           echo '<td>'.$s['designation'].'</td>';
           echo '<td>'.$s['location'].'</td>';
           echo '</tr>';
         }
         ?>
    </tbody>
  </table>
</div>
