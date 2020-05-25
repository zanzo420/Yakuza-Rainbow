<?php

include("../check.php");
include("../connection.php");

include("../header.php");

?>

<table >
		<tbody>
		  
		  <tr>
		      <td><strong>Username</strong></td>
		      <td><strong>Computer Name</strong></td>
		      <td><strong>HWID</strong></td>
		      <td><strong>Unban</strong></td>
		  </tr>
		  
		  
            <?php
		  $result = $obj->query("SELECT * FROM userlist");

		  while($row = $result->fetch_assoc())
		  {
		      if($row['banned'] != '0') {
		        echo '<tr style="font-family: monospace, monospace; ">
			            <td>' . $row['username'] . "</td>
			            <td>" . $row['computer_name'] . "</td>
			            <td>" . $row['hwid_hash'] . '</td>

			            <td><form action="unban.php" method="post">
                            <input name="username" type="hidden" value="' . $row['username'] . '">
                            <input name="submit" type="submit" value="unban" class="button primary"">
                        </form>
                        </td>

		            </tr>';
		      }
		  }

            ?>
		  
		
	    </tbody>
	</table>


<?php

include("../footer.php");

?>
