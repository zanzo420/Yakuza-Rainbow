<?php

include("../check.php");
include("../connection.php");

include("../header.php");

?>

<p class="hint" style="color: red;">
    <?php
        if($_GET['error'] == "existing-username")
        {
            echo 'Username is already taken.';
        }
    
    ?>
</p>

<table>
		<tbody>
		  
		  <tr>
		      <td><strong>Username</strong></td>
		      <td><strong>Password</strong></td>
		      <td><strong>Perms</strong></td>
		  </tr>
		  
		  <tr style="font-family: monospace, monospace;">
		          <form action="add.php" method="post">
		              
		              <td><input type="text" required="" name="username"></td>
		              
		              <td><input type="text" required="" name="password" ></td>
		              		              
		              <td><input name="submit" type="submit" class="button primary" value="add" /></td>
		              
                  </form>
        </tbody>
        
        
</table>

<p class="hint">
            HWID and IP will be set on the first inject.
        </p>

<?php

include("../footer.php");

?>