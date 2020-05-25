<?php

// Include database connection
include("../connection.php");

// Grab username and password from POST request
$uid = mysqli_real_escape_string($obj, $_POST["username"]);

if(isset($_POST['username']))
{
    // Valid request
    
    // Ban user
    $obj->query("UPDATE `userlist` SET `banned`='1' WHERE username='$uid'");
    
    // Redirect back to userlist
    header("Location: user.php");
} 
else
{
    // Invalid request
    header("Location: ../");
}