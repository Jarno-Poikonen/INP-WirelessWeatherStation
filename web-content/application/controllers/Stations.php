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
        "designation" =>$this->input->post('des'),
        "location"    =>$this->input->post('loc'),
        "latitude"    =>$this->input->post('lat'),
        "longitude"   =>$this->input->post('lon')
    );
    $this->Station_model->add_station_to_database($dataToAdd);
    redirect("Stations/show_all_stations");
  }

  public function station_modify(){
    $this->load->model('Station_model');
    $id = $this->input->post("id");
    foreach (array_slice($this->input->post(), 1) as $key => $value) {
      if (!empty($value)){
        $newData[$key] = $value;
      }
    }
    $this->Station_model->modify_station_in_database($newData, $id);
    redirect("Stations/show_all_stations");
  }

  public function station_remove(){
    $this->load->model('Login_model');
    if ($this->Login_model->verify_password($_SESSION['user'])) {
      $this->load->model('Station_model');
      $id = $this->input->post('id');
      $this->Station_model->remove_station_from_database($id);
      redirect("Stations/show_all_stations");
    } else {
      echo "<p> nope</p>";
    }
  }
  public function data_visualization(){
    $this->load->model('Station_model');
    $stationId=$this->input->post('id');
    $data['toVisualize'] = $this->Station_model->testquery($stationId);
    $data['selected']="visualization";
    $data['view']='stations/visualization';
		$this->load->view('layout/content', $data);
  }

}
?>
