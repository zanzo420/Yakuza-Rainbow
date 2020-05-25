<?php

// Include database connection
include("../connection.php");

// Grab username and password from POST request
$uid = mysqli_real_escape_string($obj, $_POST["username"]);

if(isset($_POST['username']))
{
    // Valid request
    
    // Reset Hwid for user
    $obj->query("UPDATE `userlist` SET `cpu`='',`computer_name`='',`physical_hdd_serial`='',`hwid_hash`='' WHERE username='$uid'");
    
    // Redirect back to userlist
    header("Location: user.php");
} 
else
{
    // Invalid request
    header("Location: ../");
}