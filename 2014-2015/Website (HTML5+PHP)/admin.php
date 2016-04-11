<!DOCTYPE html>

<html lang="en">
<head>
    <?php include("header.php"); ?>
</head>
<body id="acpForm">
<script type="text/javascript">
function isTrue()
{
    	//hides login Form Element
	document.getElementById('logInForm').style.display="none";
	document.getElementById('logInForm').style.visibility="hidden";
	document.getElementById('logInForm').style.opacity="0";
	//shows admin Block
    	document.getElementById('adminBlock').style.display="inline";
    	document.getElementById('adminBlock').style.visibility="visible";
    	document.getElementById('adminBlock').style.opacity="1";
    
}
</script>
<?php
//defining the variables, setting values to empty
$servername = "columbia1872.chir6uoioknn.us-west-2.rds.amazonaws.com";
$username = "admin";
$password = "columbia1872";
$dbname = "1872db";

	
// Create connection
$conn = new mysqli("columbia1872.chir6uoioknn.us-west-2.rds.amazonaws.com", "admin", "columbia1872", "1872db");

// Check connection
if ($mysqli->connect_error) {
    die('Connect Error (' . $mysqli->connect_errno . ') '
            . $mysqli->connect_error);
}

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$username = $password = "dummy";
$sql = "SELECT * FROM users";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) {
        
$correctLogInID = $row["username"];
$correctPassword = $row["password"];        
        //echo "<img src=" $row["image"]. ">";
    }
} else {
    echo "0 results";
}

if($_POST['logIn'] and $_SERVER["REQUEST_METHOD"] == "POST"){
 logIn();
}

function logIn()
{
$username = test_input($_POST["username"]);
$password = test_input($_POST["password"]);
}

function test_input($data){
 $data = trim($data);
   $data = stripslashes($data);
   $data = htmlspecialchars($data);
   return $data;
};
?>

<center>
<div id="logInForm"><form method="POST" action="<?php echo htmlspecialchars($_SERVER['PHP_SELF']);?>">
<!--BEGIN LOGIN FORM-->
<div id="acpTitle">Administration CP</div>
<div id="acpTitle2">Authorization is required. Please login below.</div>
<div id="acpFormC">
<center><table>
<tr><td>Username:</td><td><input type="text" size="20" name="username" value=""></td></tr>
<tr><td>Password:</td><td><input type="password" size="20" name="password" value=""></td></tr>
<tr><td colspan="2" align="center"><input value="log in" id="button" type="submit" name="logIn"></td></tr>
</table></center>
</div>
<!--END LOGIN FORM-->
</form>
</div>



<!--BEGIN ADMIN BLOCK-->
<div id = "adminBlock">
<div id="adminControl">
<h1>Columbia 1872: Admin Control Area</h1>
<br>
<h2>What would you like to do?</h2>
<br>

<!--Begin Category Management-->
<?php include("categories.php"); ?>
<!--End Category Management-->



<br>
</div>
</div>
<!--END ADMIN BLOCK-->
<center>
<?php
if($_SERVER["REQUEST_METHOD"] == "POST")
{
	$username = test_input($_POST["username"]);
	$password = test_input($_POST["password"]);
	if(($username == $correctLogInID) && ($password == $correctPassword))
	{
		echo "<script type='text/javascript'>isTrue();</script>";
	}
	elseif(($username == "") && ($password == ""))
    {
		echo "Please enter a username and password";
    }
	elseif($username == "")
	{
		echo "Please enter a username";
	}
	elseif($password == "")
	{
		echo "Please enter a password";
	}
	elseif(($username != $correctLogInID) || ($password != $correctPassword))
	{
		echo "Incorrect username and/or password.";
	}
}
?>
<?php include("footer.php"); ?>
</body>
</html>
