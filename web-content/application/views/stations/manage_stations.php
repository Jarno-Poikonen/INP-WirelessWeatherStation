<div id="main">
  <h3> Station management </h3>
  <div class="tab-container">

    <div class="tab">
      <input type="radio" id="tab-1" name="tab-group1" checked>
      <label for="tab-1">Add station</label>
      <div class="tab-content">
        <form action="<?php echo site_url('stations/station_add'); ?>" method="post">
         <table>
             <tr><td>Designation</td><td><input type="text" name="des" required></td></tr>
             <tr><td>Location</td><td><input type="text" name="loc" required></td></tr>
             <tr><td>Latitude</td><td><input type="text" name="lat"></td></tr>
             <tr><td>Longitude</td><td><input type="text" name="lon"></td></tr>
             <tr><td></td><td><input type="submit" value="Add"></td></tr>
         </table>
        </form>
      </div>
    </div>

    <div class="tab">
      <input type="radio" id="tab-2" name="tab-group1">
      <label for="tab-2">Modify station</label>
      <div class="tab-content">
        <p>Station modify ph</p>
      </div>
    </div>

    <div class="tab">
      <input type="radio" id="tab-3" name="tab-group1">
      <label for="tab-3">Remove station</label>
      <div class="tab-content">
        <p>Station remove ph</p>
      </div>
    </div>

  </div>

</div>
