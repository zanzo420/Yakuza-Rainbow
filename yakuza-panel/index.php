<?php

session_start();

if(isset($_SESSION['username']))
{
    header("Location: panel/user.php");
}

include 'header.php';

$error = $_GET['error'];

?>




    <h3 class="call-to-action" style="text-align: center;">
        Login to <strong>yakuza.</strong>
     </h3>
     
     
    <form action="panel/login.php" method="post" class="loginform" style="font-size: 18px; margin-top: -3em; margin-left:9em;">	
	<div class="loginrow">
	    <?php
     
     
     if($error=="incorrect") 
     {
        echo '<p class="hint" style="color: red;">Incorrect username or password.</p>';
     } 
     
     else if ($error=="banned")
     {
         echo '<p class="hint" style="color: red;">This account has been banned.</p>';
     }
     else if ($error=="not-admin")
     {
         echo '<p class="hint" style="color: red;">User is not admin.</p>';
     } 
     
     
     ?>
		<div class="loginrow" style="padding-top: 2em">
			<label>Username</label>
			<input type="text" required="" name="username" class="wide" value="">
		</div>
	</div>
	<div class="loginrow" style="padding-bottom: -2em; ">
		<div class="loginrow">
			<label>Password</label>
			<input type="password" required="" name="password" class="wide" >
		</div>
	</div>
		
	<div class="row form-row">
		<div class="loginrow">
			<label>&nbsp;</label>
			<input type="submit" class="button primary" value="Login">
		</div>
	</div>
</form>
    
<?php
include 'footer.php';
?>