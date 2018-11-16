<div id="main">
  <p> Station search </p>
  <form action="<?php echo site_url('stations/station_search'); ?>" method="post">
   <table>
       <tr><td>Designation</td><td><input type="text" name="data[designation]"></td>
          <td>Region</td><td><input type="text" name="data[city]"></td>
          <td>Latitude</td><td><input type="text" name="data[latitude]"></td>
          <td>Longitude</td><td><input type="text" name="data[longitude]"></td>
          <td>Description</td><td><input type="text" name="data[description]"></td></tr>
       <tr><td></td><td><input type="submit" value="Search"></td></tr>
   </table>
  </form>
  <table class="dataframe">
    <?php
     foreach ($results as $r) {
       echo '<tr>';
       echo '<td>'.$r['idStation'].'</td>';
       echo '<td>'.$r['designation'].'</td>';
       echo '<td>'.$r['city'].'</td>';
       echo '<td>'.$r['description'].'</td>';
       echo '<td>'.$r['latitude'].','.$r['longitude'].'</td>';
       echo '</tr>';
     }
    ?>
   </table>
</div>
