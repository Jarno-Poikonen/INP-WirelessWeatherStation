<?php

class Station_model extends CI_Model{

  public function get_all_stations(){
    $this->db->select('*');
    $this->db->from('stations');
    return $this->db->get()->result_array();
  }

  public function get_recent_measurements(){
    $this->db->select('designation, location, type, reading, timestamp');
    $this->db->join('sensors', 'measurements.sensors_idSensor = sensors.idSensor');
    $this->db->join('stations', 'measurements.stations_idStation = stations.idStation');
    $this->db->from('measurements');
    $this->db->order_by("idMeasurement", "desc");
    $this->db->limit(10);
    return $this->db->get()->result_array();
  }

  public function add_station_to_database($stationInfo){
    $this->db->set($stationInfo);
    $this->db->insert('stations');
  }
  public function remove_station_from_database($stationId){
    $this->db->where('idStation', $stationId);
    $this->db->delete('stations');
  }
  public function modify_station_in_database($data, $stationId){
    $this->db->where('idStation', $stationId);
    $this->db->update('stations', $data);
  }

  public function get_measurement_type_by_id($queryParams, $id){
    $this->db->select('timestamp, reading');
    $this->db->from('measurements');
    $this->db->where('stations_idStation', $queryParams['id']);
    if (!empty($queryParams['start']) && !empty($queryParams['end'])){
      $this->db->where('timestamp >=', $queryParams['start']);
      $this->db->where('timestamp <=', $queryParams['end']);
    }
    $this->db->where('sensors_idSensor', $id);
    //echo json_encode($this->db->get()->result_array());
    return $this->db->get()->result_array();
  }
}
?>
