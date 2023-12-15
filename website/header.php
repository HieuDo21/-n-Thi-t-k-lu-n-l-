
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
<link rel="stylesheet" type="text/css" href="css/header.css">

</head>
<header>
<div class="header">
	<div class="logo">
		<a href="index.php">Biometric Attendance</a>
	</div>
</div>




<div class="topnav" id="myTopnav">
	<a href="index.php">Users</a>
    <a href="UsersLog.php">Users Log</a>
    <a href="ManageUsers.php">Manage Users</a>
	<?php  
    	if (isset($_SESSION['Admin-name'])) {
    
    		echo '<a href="logout.php">Log Out</a>';
    	}
    	else{
    		echo '<a href="login.php">Log In</a>';
    	}
    ?>

    <a href="javascript:void(0);" class="icon" onclick="navFunction()">
	  <i class="fa fa-bars"></i></a>
</div>


</header>
<script>
	function navFunction() {
	  var x = document.getElementById("myTopnav");
	  if (x.className === "topnav") {
	    x.className += " responsive";
	  } else {
	    x.className = "topnav";
	  }
	}
</script>



