<?php
// Put your domain here eg. google.com (no http/https)
define('DB_SERVER', '34.67.8.195');

// Put the username of the user you created with all privelages
// and added to the database
define('DB_USERNAME', 'root');

// Password of that user
define('DB_PASSWORD', 'saq4w11Wn4foLg');

// The name of the database
define('DB_DATABASE', 'yakuza');
$obj = mysqli_connect(DB_SERVER,DB_USERNAME,DB_PASSWORD,DB_DATABASE);
?>
