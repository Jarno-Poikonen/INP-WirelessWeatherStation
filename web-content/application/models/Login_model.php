<?php

class Login_model extends CI_Model{

  public function get_password($credentials){
    $this->db->select('password');
    $this->db->from('webusers');
    $this->db->where('username', $credentials['uname']);
    $data = $this->db->get()->result_array();
    return $data;
  }
}
?>
