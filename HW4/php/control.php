<?php include 'connectDB.php';
?>

<!DOCTYPE html>
<html>
<head>
</head>
<body>
<a href="control.php?LED=ON"><button>ON</button></a>
<a href="control.php?LED=OFF"><button>OFF</button></a>
<br>
<?php
if(isset($_GET['LED'])){
    $action = $_GET['LED'];
if ($action == 'ON') {
	$sql = 'UPDATE led SET status = "1"';
	echo "LED Status: ON";
	}
else if ($action == 'OFF') {
	$sql = 'UPDATE led SET status = "0"';
	echo "LED Status: OFF";
	}
	$result = mysqli_query($con,$sql);
}

else {
$sql = 'SELECT * FROM led';
$result = mysqli_query($con,$sql);
$row 	= mysqli_fetch_assoc($result);
$action = $row['status'];
if($action == 1) 	  echo "LED Status: ON";
else if($action == 0) echo "LED Status: OFF";
  }
mysqli_close($con);
?>
</body>
</html>