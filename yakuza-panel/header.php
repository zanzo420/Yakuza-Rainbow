<?php
    session_start();
    ?>

<html lang="en"><head>
        <title>
            yakuza / Panel
        </title>
        
        <meta name="viewport" content="width=device-width">
        <link rel="stylesheet" href="styles.css">
        <link rel="icon" type="image/png" href="https://www.yakuza.dev/wp-content/uploads/2020/05/yakuza-site-logo.png">
        <link rel="apple-touch-icon" href="https://www.yakuza.dev/wp-content/uploads/2020/05/yakuza-site-logo.png">
        
        
    </head>

    <body>

        <div class="container">

            <div class="row navigation">
                <a href="https://yakuza.dev/yakuza-panel/">
                    <img src="https://www.yakuza.dev/wp-content/uploads/2020/05/yakuza-site-logo.png" class="navigation-icon">
                </a>
                
                <?php
                    if(isset($_SESSION['username']))
                    {
                        echo '<a href="user.php">Users</a>';
                        echo '<a href="addusers.php">Add User</a>';
                        echo '<a href="delete.php">Delete User</a>';
                        echo '<a href="bannedusers.php">Banned</a>';
                        echo '<a href="logs.php">Logs</a>';
                        echo '<span class="aside">
			<a href="logout.php">logout</a>
		</span>';
                        
                    }
                    else
                    {
                        echo '<a href="https://yakuza.dev/yakuza-panel/">home</a>';
                    }
                
                    
                ?>
                

            </div>
