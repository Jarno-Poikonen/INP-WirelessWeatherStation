<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>INP weatherstations</title>
<head>
  <style type="text/css">
    body {
      background-color: #1d1d1d;
      color: #cccccc;
      margin: 0;
      font-family: Verdana;
    }

    #topnav{
      padding: 19px 5px 19px 20px;
    }
    #topnav a {
      font-size: 20px;
      text-decoration: none;
      color: inherit;
    }
    #topnav a.loginbtn {
      font-size: 15px;
      border: 2px solid #995c00;
      border-radius: 2px;
      color: inherit;
      padding: 8px 18px;
      background-color: inherit;
    }
    #topnav a.loginbtn span {
      display: inline-block;
      position: relative;
      transition: 0.5s;
    }
    #topnav a.loginbtn span:after {
      content: '\00bb';
      position: absolute;
      opacity: 0;
      top: 0;
      right: -14px;
      transition: 0.5s;
    }
    #topnav a.loginbtn:hover span {
      padding-right: 15px;
    }
    #topnav a.loginbtn:hover span:after {
      opacity: 1;
      right: 0;
    }

    #sidenav {
      float: left;
      width: 200px;
      background-color: #54575f;
      height: 100vh;
    }
    #sidenav a {
      display: block;
      font-size: 20px;
      text-decoration: none;
      font-family: Consolas;
      color: black;
      padding: 4px 1px 4px 16px;
    }
    #sidenav a:hover {
      background-color: #1d1d1d;
      color: #a0a4ab;
    }
    #main {
      margin-left: 200px;
      padding: 0px 16px 0px 16px;
    }
    .right {
      float: right;
      vertical-align: middle;
    }
    .container {
      border: 1px solid #D0D0D0;
    }
    .top {
        padding: 20px;
        font-size: 30px;
        text-align: center;
    }
    .sidebar {

    }
    .content {
        float: left;
        padding: 15px;
    }
    .footer {
        text-align: center;
        padding: 50px;
    }
    </style>
</head>
<body>
