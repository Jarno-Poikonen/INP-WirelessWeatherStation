<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Management extends CI_Controller {
  function __construct(){
    parent::__construct();
    if($_SESSION['loggedIn']== true){
      //pass
    }else{
      $data['selected']="station_management";
      redirect('Error/no_login');
    }
  }

  public function station_management(){
    $data['selected']="station_management";
    $data['view']='stations/manage_stations';
    $this->load->view('layout/content', $data);
  }

  public function station_add(){
    $this->load->model('Management_model');
    $dataToAdd=$this->input->post();
    $this->Management_model->add_station_to_database($dataToAdd);
    redirect("Stations/show_all_stations");
  }

  public function station_modify(){
    $this->load->model('Management_model');
    $id = $this->input->post("id");
    foreach (array_slice($this->input->post(), 1) as $key => $value) {
      if (!empty($value)){
        $newData[$key] = $value;
      }
    }
    if (!empty($newData)){
      $this->Management_model->modify_station_in_database($id, $newData);
      redirect("Stations/show_all_stations");
    }
    echo "<p>You did not change anything...</p>";
  }

  public function station_remove(){
    $this->load->model('Login_model');
    if ($this->Login_model->verify_password($_SESSION['user'])) {
      $this->load->model('Management_model');
      $id = $this->input->post('id');
      $this->Management_model->remove_station_from_database($id);
      redirect("Stations/show_all_stations");
    } else {
      echo "<p> nope</p>";
    }
  }
}
?>
