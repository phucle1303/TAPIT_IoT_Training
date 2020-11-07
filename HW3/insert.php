<?php include 'connectDB.php';
?>

<!DOCTYPE html>
<html lang="en">
<head>
<style>
p
{
    font-style: italic;
    font-size: 90%;
    text-align: center;
}
body
{
    font-size: 170%;
}
h1
{
    text-align: center;
}
</style>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Insert value</title>
</head>
<body>
<h1>Welcome</h1>
<p>Insert new value by using /insert.php?temp=x&light=y and refresh the page</p>
<?php
if (isset($_GET['temp'], $_GET['light']))
{
    $temp = $_GET['temp'];
    $light = $_GET['light'];
    $sql = "INSERT INTO db_room (id, temp, light)
    VALUES (NULL, $temp, $light);";
    echo "The temperature sensor value is $temp <br>";
    echo "The light sensor value is $light";
}
else
{
    $sql = 'SELECT * FROM db_room';
    $result = mysqli_multi_query($con, $sql);
}
echo "<br>";
if (mysqli_multi_query($con, $sql))
{
    echo "New data inserted successfully";
}
else
{
    echo "Error: " . $sql . "<br>" . mysqli_error($con);
}

mysqli_close($con);
?>

<p>Author: Phuc Le</p>
</body>
</html>