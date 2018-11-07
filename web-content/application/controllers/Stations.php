<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Stations extends CI_Controller {
  public function stations_summary(){
    $this->load->model('Station_model');
    $data['selected']="stations_summary";
    $data['view']='stations/summary';
    $data['measurements'] = $this->Station_model->get_recent_measurements();
		$this->load->view('layout/content', $data);
	}

  public function show_all_stations(){
    $this->load->model('Station_model');
    $data['selected']="show_all_stations";
    $data['view']='stations/all_stations';
    $data['a_stations']=$this->Station_model->get_all_stations();
		$this->load->view('layout/content', $data);
	}

  public function station_search(){
    $data['selected']="station_search";
    $data['view']='stations/station_search';
		$this->load->view('layout/content', $data);
	}

  public function station_management(){
    $data['selected']="station_management";
    if($_SESSION['loggedIn']==true){
      $data['view']='stations/manage_stations';
      $this->load->view('layout/content', $data);
    }else{
      $data['view']='errors/no_login';
      $this->load->view('layout/content', $data);
    }
  }

  public function station_add(){
    $this->load->model('Station_model');
    $dataToAdd=array(
        "designation"=>$this->input->post('des'),
        "location"=>$this->input->post('loc'),
        "latitude"=>$this->input->post('lat'),
        "longitude"=>$this->input->post('lon')
    );
    $this->Station_model->add_station_to_database($dataToAdd);
    redirect ('stations/stations_summary');
  }
}
?>
