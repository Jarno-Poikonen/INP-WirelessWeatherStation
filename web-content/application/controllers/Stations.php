<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Stations extends CI_Controller {
  public function stations_summary(){
    $data['view']='stations/summary';
		$this->load->view('layout/content', $data);
	}

  public function show_all_stations(){
    $this->load->model('Station_model');
    $data['view']='stations/all_stations';
    $data['a_stations']=$this->Station_model->get_all_stations();
		$this->load->view('layout/content', $data);
	}

  public function station_search(){
    $data['view']='stations/station_search';
		$this->load->view('layout/content', $data);
	}

  public function station_management(){
    $data['view']='stations/manage_stations';
    $this->load->view('layout/content', $data);
  }

  public function station_add(){
    $this->load->model('Station_model');
    $dataToAdd=array(
        "designation"=>$this->input->post('des'),
        "location"=>$this->input->post('loc')
    );
    $this->Station_model->add_station_to_database($dataToAdd);
    redirect ('stations/stations_summary');
  }
}
?>
