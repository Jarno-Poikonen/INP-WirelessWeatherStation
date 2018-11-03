<?php

class Login extends CI_Controller
{
  public function siteLogin(){ //demon vuoksi tämä hyväksyy automaattisesti
    $_SESSION['user']=$this->input->post('loginID');
    $_SESSION['loggedIn']=true;
    redirect('stations/stations_summary');
  }
  public function siteLogout(){
    $_SESSION['loggedIn']=false;
    redirect('stations/stations_summary');
  }
}
