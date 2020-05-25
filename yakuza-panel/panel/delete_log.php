<?php

// Include database connection
include("../connection.php");

// Grab username and password from POST request
$timestamp = mysqli_real_escape_string($obj, $_POST["timestamp"]);

if(isset($_POST['timestamp']))
{
    // Valid request

    // Ban user
    $obj->query("DELETE FROM `yakuza_logs` WHERE time_stamp='$timestamp'");

    // Redirect back to userlist
    header("Location: logs.php");
}
else
{
    // Invalid request
    header("Location: ../");
}