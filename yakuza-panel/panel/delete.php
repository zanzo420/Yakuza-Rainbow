<?php

include("../check.php");
include("../connection.php");

session_start();

if(!isset($_SESSION['username']) && !isset($_SESSION['password']) && $_SESSION['suwoo'] != 'suwoo')
{
    header("Location: https://yakuza.dev/yakuza-panel/");
}

include("../header.php");

?>


<table>
    <tbody>

        <tr>
            <td>
                <strong>Username</strong>
            </td>
            <td>
                <strong>Computer Name</strong>
            </td>
            <td>
                <strong>HWID</strong>
            </td>
            <td>
                <strong>Delete User</strong>
            </td>

        </tr>


        <?php
        $result = $obj->query("SELECT * FROM userlist");

        while($row = $result->fetch_assoc())
        {
            if($row['banned'] != '1') {
		        echo '<tr style="font-family: monospace, monospace;">
			            <td>' . $row['username'] . "</td>
			            <td>" . $row['computer_name'] . "</td>
			            <td>" . $row['hwid_hash'] . '</td>

                        <td><form action="delete_user.php" method="post">
                            <input name="username" type="hidden" value="' . $row['username'] . '">
                            <input name="submit" type="submit" value="Delete" class="button primary">
                        </form>
                        </td>
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