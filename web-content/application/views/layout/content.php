<?php
  $this->load->helper('url');
  $this->load->view('layout/header');
  $this->load->view('layout/left_header');
  $this->load->view($view);
  $this->load->view('layout/footer');
?>
