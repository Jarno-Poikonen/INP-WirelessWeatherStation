<?php

class Login extends CI_Controller{

  public function siteLogin(){
    $this->load->model('Login_model');
    $credentials['uname'] = $this->input->post('loginID');
    //$credentials['pw'] = $this->input->post('loginPw');
    if (!empty($this->Login_model->get_password($credentials)[0]['password'])){
      if (password_verify($this->input->post('loginPw'), $this->Login_model->get_password($credentials)[0]['password'])) {
        $_SESSION['user']=$this->input->post('loginID');
        $_SESSION['loggedIn']=true;
      }
    }
    redirect($_SERVER['HTTP_REFERER']);
  }

  public function siteLogout(){
    $_SESSION['loggedIn']=false;
    redirect($_SERVER['HTTP_REFERER']);
  }
}
?>
