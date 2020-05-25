<?php

include("../connection.php");

$uid = mysqli_real_escape_string($obj, $_POST["username"]);
$pwd = mysqli_real_escape_string($obj, $_POST["password"]);


// Random key gen func
function randomKey($length) {
    $pool = array_merge(range(0,9), range('a', 'z'),range('A', 'Z'));

    for($i=0; $i < $length; $i++) {
        $key .= $pool[mt_rand(0, count($pool) - 1)];
    }
    return $key;
}

if(isset($_POST['submit']))
{
    // Gen salt and hash password
    $password = $pwd;
    
    // Select the row using the username sent
    $result = $obj->query("SELECT * FROM userlist WHERE username='$uid'");
    
    if($result->num_rows > 0)
    {
        // user exists
        header("Location: addusers.php?error=existing-username");
    }
    else
    {
        // username available
        
        // add the user
        $obj->query("INSERT INTO `userlist`(`username`, `password`, `banned`, `fail_counter`,`cpu`,`computer_name`,`physical_hdd_serial`,`hwid_hash`) VALUES ('$uid', '$password','0','1','','','','')");
    
        // redirect back to addusers
        header("Location: addusers.php");
    }

}
else
{
    header("Location: user.php");   
}