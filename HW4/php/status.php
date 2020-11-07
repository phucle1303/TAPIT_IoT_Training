<?php
include 'connectDB.php';

$sql = 'SELECT * FROM led';
$result = mysqli_query($con,$sql);
while($row = mysqli_fetch_assoc($result)) {
echo "|";
echo $row['status'];
}

mysqli_close($con);
?>