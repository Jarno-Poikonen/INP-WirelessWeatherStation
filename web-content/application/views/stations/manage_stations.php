<div id="main">
  <h3> Station management </h3>
  <div class="tab-container">

    <div class="tab">
      <input type="radio" id="tab-1" name="tab-group1" checked>
      <label for="tab-1">Add station</label>
      <div class="tab-content">
        <form action="<?php echo site_url('stations/station_add'); ?>" method="post">
         <table>
             <tr><td>Designation</td><td><input type="text" name="desig" required></td></tr>
             <tr><td>Region</td><td><input type="text" name="city" required></td></tr>
             <tr><td>Description</td><td><input type="text" name="descr" required></td></tr>
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
        <form action="<?php echo site_url('stations/station_modify'); ?>" method="post">
          Id to modify <input type="text" name="id" required></br>
          </br>
          To preserve old value, leave field empty.
         <table>
             <tr><td>Designation</td><td><input type="text" name="designation"></td></tr>
             <tr><td>Region</td><td><input type="text" name="city"></td></tr>
             <tr><td>Latitude</td><td><input type="text" name="latitude"></td></tr>
             <tr><td>Longitude</td><td><input type="text" name="longitude"></td></tr>
             <tr><td>Description</td><td><input type="text" name="description"></td></tr>
             <tr><td></td><td><input type="submit" value="Modify"></td></tr>
         </table>
        </form>
      </div>
    </div>

    <div class="tab">
      <input type="radio" id="tab-3" name="tab-group1">
      <label for="tab-3">Remove station</label>
      <div class="tab-content">
        <form action="<?php echo site_url('stations/station_remove'); ?>" method="post">
          <table>
              <tr><td>Id to remove </td><td><input type="text" name="id" required></td><td><input type="submit" value="Remove"></td></tr>
          </table>
          <p class="warn">Note that removing a station also ERASES MEASUREMENT DATA related to it.<br>
          Verify with your password below</p>
          <input type="password" name="loginPw" required></td>
        </form>
      </div>
    </div>

  </div>

</div>
