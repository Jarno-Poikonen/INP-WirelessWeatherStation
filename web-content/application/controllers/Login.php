<?php

class Login extends CI_Controller{

  public function siteLogin(){
    $this->load->model('Login_model');
    $credentials['uname'] = $this->input->post('loginID');
    $credentials['pw'] = $this->input->post('loginPw');
    $stored = $this->Login_model->get_password($credentials);
    if (empty($stored)) {
     echo '<p>No user</p>';
   }else if ($credentials['pw'] == $stored[0]['password']) {
      $_SESSION['user']=$this->input->post('loginID');
      $_SESSION['loggedIn']=true;
      echo '<p>OK</p>';
    } else {
      echo '<p>Invalid credentials</p>';
    }
    //redirect($_SERVER['HTTP_REFERER']);
  }

  public function siteLogout(){
    $_SESSION['loggedIn']=false;
    redirect($_SERVER['HTTP_REFERER']);
  }
}
?>
