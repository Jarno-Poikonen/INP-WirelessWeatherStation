<div id="main">
  <h3> Station management </h3>
  <div class="container">
    <p>Add station</p>
    <form class="" action="<?php echo site_url('stations/station_add'); ?>" method="post">
      <table>
          <tr><td>Designation</td><td><input type="text" name="des" required></td></tr>
          <tr><td>Location</td><td><input type="text" name="loc"required></td></tr>
          <tr><td></td><td><input type="submit" value="Add"></td></tr>

      </table>
  </div>
</div>
