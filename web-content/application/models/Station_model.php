<?php

class Station_model extends CI_Model{

  public function get_all_stations(){
    $this->db->select('*');
    $this->db->from('stations');
    return $this->db->get()->result_array();
  }

  public function add_station_to_database($stationInfo){
    $this->db->set($stationInfo);
    $this->db->insert('stations');
  }
}
?>
