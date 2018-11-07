<div id="main">
  <div class="space-out">
    <div style="text-align: center">
      <h4>Number of stations</h4>
      <?php echo '<h1>X</h1>'?>
    </div>
    <div style="text-align: center">
    <h4>Measurements done</h4>
      <?php echo '<h1>X</h1>'?>
    </div>
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
