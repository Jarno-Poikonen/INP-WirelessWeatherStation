<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Measurement extends CI_Controller {

  public function add_new_measurement(){
    $this->load->model('Management_model');
    print_r($_POST);
    $measdata['idStation']=htmlspecialchars($_POST['idStation']);
    $measdata['timestamp']=htmlspecialchars($_POST['timestamp']);
    $measdata['temperature']=htmlspecialchars($_POST['temperature']);
    $measdata['humidity']=htmlspecialchars($_POST['humidity']);
    $measdata['pressure']=htmlspecialchars($_POST['pressure']);
    $measdata['illuminance']=htmlspecialchars($_POST['illuminance']);
    $this->Management_model->add_measurement_to_database($measdata);
  }
  public function verify_signature($received, $signature, $shared_secret) {
    return hash_equals(hash_hmac($received, $shared_secret), $signature);
  }
?>
