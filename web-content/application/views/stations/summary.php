<div id="main">
  <div class="space-out">
    <table>
      <thread>
        <tr>
          <th>Miksi tämä menee tähän?</th>
        </tr>
      </thread>
      <tbody>
        <?php
           //foreach ($measurements as $m) {
             echo '<tr>';
             echo '<td>asd</td>';
             echo '</tr>';
           //}
           ?>
      </tbody>
    </table>

    <table>
      <thread>
        <tr>
          <th>Measurements done</th>
        </tr>
      </thread>
      <tbody>
        <?php
           //foreach ($measurements as $m) {
             echo '<tr>';
             echo '<td>placeholder</td>';
             echo '</tr>';
           //}
           ?>
      </tbody>
    </table>
  </div>
  <h3> Latest readings </h3>
  <div>
    <table id="latest" class="dataframe">
      <thread>
        <tr>
          <th>Station</th><th>Location</th><th>Type</th><th>Reading</th><th>Time</th>
        </tr>
      </thread>
      <tbody>
        <?php
           foreach ($measurements as $m) {
             echo '<tr>';
             echo '<td>'.$m['designation'].'</td>';
             echo '<td>'.$m['location'].'</td>';
             echo '<td>'.$m['type'].'</td>';
             echo '<td>'.$m['reading'].'</td>';
             echo '<td>'.$m['timestamp'].'</td>';
             echo '</tr>';
           }
           ?>
      </tbody>
    </table>
  </div>
</div>
