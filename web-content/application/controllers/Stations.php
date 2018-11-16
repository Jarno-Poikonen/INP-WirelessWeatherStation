<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Stations extends CI_Controller {
  public function stations_summary(){
    $this->load->model('Station_model');
    $data['selected']="stations_summary";
    $data['view']='stations/summary';
    $data['measurements']      = $this->Station_model->get_recent_measurements();
    $data['station_count']     = $this->Station_model->get_station_count();
    $data['measurement_count'] = $this->Station_model->get_measurement_count();
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
    $this->load->model('Station_model');
    $data['selected']="station_search";
    $data['view']='stations/station_search';
    if (isset($this->input->post()['data'])){
      //print_r($this->input->post()['data']);
      $queryParams=$this->input->post()['data'];
      $data['results'] = $this->Station_model->search_stations_from_database($queryParams);
      //print_r($data['results']);
    } else {
      $data['results'] = [];
    }
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
        "designation" =>$this->input->post('desig'),
        "city"        =>$this->input->post('city'),
        "description" =>$this->input->post('descr'),
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
    $data['selected']="visualization";
    $data['view']='stations/visualization';
		$this->load->view('layout/content', $data);
  }
  public function ajaxtest(){
    $this->load->model('Station_model');
    $queryParams['id']    =$this->input->post('id');
    $queryParams['start'] =$this->input->post('start');
    $queryParams['end']   =$this->input->post('end');
    $data = $this->Station_model->get_measurement_by_station_id($queryParams);
    print_r(json_encode($data, true));
  }
}
?>
