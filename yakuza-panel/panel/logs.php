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
				<strong>Command</strong>
			</td>
			<td>
				<strong>Info</strong>
			</td>
			<td>
				<strong>Time</strong>
			</td>

			<td>
				<strong>Ban</strong>
			</td>
			<td>
				<strong>Delete Log</strong>
			</td>
		</tr>


		<?php
		$result = $obj->query("SELECT * FROM yakuza_logs");

		while($row = $result->fetch_assoc())
		{
		    echo '<tr style="font-family: monospace, monospace;">
			        <td>' . $row['username'] . "</td>
			        <td>" . $row['command'] . "</td>
			        <td>" . $row['information'] . "</td>
			        <td>" . $row['time_stamp'] . '</td>


			        <td><form action="ban.php" method="post">
                        <input name="username" type="hidden" value="' . $row['username'] . '">
                        <input name="submit" type="submit" value="ban" class="button primary">
                    </form>
                    </td>
                    <td><form action="delete_log.php" method="post">
                        <input name="timestamp" type="hidden" value="' . $row['time_stamp'] . '">
                        <input name="submit" type="submit" value="Delete" class="button primary">
                    </form>
                    </td>
		        </tr>';
		}

		?>


	</tbody>
</table>


<?php
include("../footer.php");
?>