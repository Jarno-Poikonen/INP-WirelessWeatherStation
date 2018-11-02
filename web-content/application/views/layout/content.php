<?php
  $this->load->view('layout/header');
  $this->load->view('layout/top_header');
  $this->load->view('layout/sidenav');
  //$this->load->view('layout/main');
  $this->load->view($view);
  $this->load->view('layout/footer');
?>
