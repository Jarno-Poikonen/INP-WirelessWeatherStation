<div id="topnav" class="container">
  <h2>INP-Weather stations</h2>
  <?php
  if(isset($_SESSION['loggedIn'])){

  }else{
    $_SESSION['loggedIn'] = false; //find a proper place to initialize this
  }

  if($_SESSION['loggedIn']==false){
    echo '<form action="'.site_url('login/siteLogin').'" method="post">
          <input type="text" placeholder="User ID" name="loginID">
          <input type="password" placeholder="Password" name="loginPw">
          <button id="loginbtn" type="submit" value="Login"><span>Login</span></button>
          </form>';
  }else{
    echo '<form action="'.site_url('login/siteLogout').'" method="post">
          <p id=userid>'.$_SESSION['user'].'</p>
          <button id="loginbtn" type="submit" value="Logout"><span>Logout</span></button>
          </form>';
  }
  ?>
</div>
