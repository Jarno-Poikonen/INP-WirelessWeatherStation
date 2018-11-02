<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Stations extends CI_Controller {

  public function show_all_stations(){
    $data['view']='stations/all_stations';
		$this->load->view('layout/content', $data);
	}

  public function station_search(){
    $data['view']='stations/station_search';
		$this->load->view('layout/content', $data);
	}

}
