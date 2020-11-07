<?php
include 'connectDB.php';
?>

<!DOCTYPE html>
<html lang="en">
<head>
<style>
table
{
    background-color: #f1f1c1;
}
caption
{
    font-weight: bold;
    font-size: 130%;
}
table, th, td
{
    border: 1px solid black;
}
th, td
{
    padding: 13px;
}
body
{
    font-size: 170%;
}
td
{
    text-align: center;
}
p
{
    font-style: italic;
    font-size: 90%;
}
</style>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Full list value</title>
</head>
<body>
<center>
<table style="width:50%">
<caption>Full list of temperature and light sensor value</caption>
<center>
  <tr>
    <th>Index</th>
    <th>Temperature sensor</th>
    <th>Light sensor</th>
  </tr>

<?php
$sql = 'SELECT * FROM db_room';
$result = mysqli_query($con, $sql);
while($row = mysqli_fetch_array($result))
{
    echo "<tr>";
    echo "<td>";
    echo $row['id'];
    echo "</td>";
    echo "<td>";
    echo $row['temp'];
    echo "</td>";
    echo "<td>";
    echo $row['light'];
    echo "</td>";
    echo "</tr>";
}
mysqli_close($con);
?>

</table>
<p>Author: Phuc Le</p>
</center>
</body>
</html>


