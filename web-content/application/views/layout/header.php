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
      display: flex;
      padding: 0px 20px 0px 20px;
      justify-content: space-between;
      align-items: center;
    }
    #userid {
      display: inline-block;
      font-size: 14px;
      margin: 9px 2px;
    }
    #topnav h2 {
      font-size: 20px;
      color: inherit;
      display: inline-block;
    }
    #topnav input {
      box-sizing: border-box;
      width: 120px;
      border: none;
      padding: 3px 2px;
      background-color: #54575f;
      margin: 8px 0px;
    }
    #loginbtn {
      font-size: 15px;
      width:  90px;
      margin-left: 3px;
      padding: 8px 18px;
      border: 2px solid #995c00;
      border-radius: 2px;
      color: inherit;
      float: right;
      background-color: inherit;
    }
    #loginbtn span {
      display: inline-block;
      position: relative;
      transition: 0.5s;
    }
    #loginbtn span:after {
      content: '\00bb';
      position: absolute;
      opacity: 0;
      top: 0;
      right: -11px;
      transition: 0.5s;
    }
    #loginbtn:hover span {
      padding-right: 15px;
    }
    #loginbtn:hover span:after {
      opacity: 1;
      right: 0;
    }
    #sidenav {
      float: left;
      width: 200px;
      background-color: #54575f;
      height: 80vh;
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
    .active{

    }
    </style>
</head>
<body>
  <script>
    function openForm() {
      document.getElementById("loginForm").style.display = "block";
    }
    function closeForm() {
        document.getElementById("loginForm").style.display = "none";
    }
</script>
