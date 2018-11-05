<div class="content">
  <p> Tässä sitten kaikki asemat </p>
  <table>
    <thread>
      <tr>
        <th>Station</th><th>Location</th>
      </tr>
    </thread>
    <tbody>
      <?php
         foreach ($a_stations as $s) {
           echo '<tr>';
           echo '<td>'.$s['designation'].'</td>';
           echo '<td>'.$s['location'].'</td>';
           echo '</tr>';
         }
         ?>
    </tbody>
  </table>
</div>
