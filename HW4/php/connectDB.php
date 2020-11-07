<?php
$servername = "localhost";
$username = "root";
$password = "";	//default is empty password
$dbname = "db_tapit";

$con = mysqli_connect($servername,$username,$password,$dbname);
if (!$con) {
  die('Could not connect to database ' );
}
?>
