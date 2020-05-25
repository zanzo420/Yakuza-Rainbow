<?php

// Include our database files
include("../connection.php");

// Start our session
session_start();

// Grab username and password from POST request
$uid = mysqli_real_escape_string($obj, $_POST["username"]);
$pwd = mysqli_real_escape_string($obj, $_POST["password"]);

if(isset($_POST["username"]))
{
    
    // Username or password is empty
    if(strlen($uid) == 0 || strlen($pwd) == 0)
    {
        // Redirect with empty field error
        header("Location: ../index.php?error=empty-field");
    } 
    else
    {
        // Check if the user exists
        $result = $obj->query("SELECT * FROM admin_users WHERE username='$uid'");
        
        // User exists
        if($result->num_rows > 0)
        {
            // Create the array of the row
            $row = $result->fetch_assoc();
            
            // Check if password is correct
            if($pwd == $row['password']) 
            {   
                // Check if user is banned
                if($row['banned'] == '1')
                {
                    header("Location: ../index.php?error=banned");
                    die();
                }
                // Now, check if the user is an admin
                else if($row['perms'] != 'admin')
                {
                    header("Location: ../index.php?error=not-admin");
                }
                else 
                {
                    // Is admin
                   // Set sessions
                  $_SESSION['username'] = $uid;
                  $_SESSION['password'] = $pwd;
                  $_SESSION['suwoo'] = 'suwoo';
                  $_SESSION['ip'] = $_SERVER['REMOTE_ADDR'];
                 
                   // Redirect to the userlist
                   header("Location: user.php");
                }
            } 
            else 
            {
                // Incorrect password
                header("Location: ../index.php?error=incorrect&pwd=$t");
            }
        }
        else
        {
            // User does not exist
            header("Location: ../index.php?error=incorrect");
        }
    }
}
else 
{
    // If no data was sent in the request
    header("Location: ../index.php");
}
