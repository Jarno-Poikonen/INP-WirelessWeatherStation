<?php
  $this->load->view('layout/header');
  $this->load->view('layout/topnav');
  $this->load->view('layout/sidenav', $selected);
  //$this->load->view('layout/main');
  if (isset($toVisualize)){
    $this->load->view($view, $toVisualize);
  } else {
    $this->load->view($view);
  }
  $this->load->view('layout/footer');
?>
