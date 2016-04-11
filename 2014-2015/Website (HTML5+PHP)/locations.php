<h3>Manage Locations</h3><div id="adminContent"><br>
<button>Manage Existing</button> | <button>Create New</button>
<div class="mngLoc" id="existing">
MANAGE LOCATIONS<br>
<table>

<!--BEGIN MANAGE LOCATION PHP CODE-->
<?php $sql = "SELECT * FROM places";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
// output data of each row
while($row = $result->fetch_assoc()) {

echo "<tr id='locRow'><td><input type='checkbox' id='" ;
echo $row["id"];
echo "'></td><td id='locName'>" ;
echo $row["name"] ;
echo "</td><td id='locLink'><a href='#edit";
echo $row["id"];
echo"'>(edit)</a></td><td id='locLink'><a href='delete/";
echo $row["id"];
echo "'>(delete)</a></td></tr>";
}
} else {
echo "0 results";
}
?>


<!--END LOCATION CODE-->
</table>
</div>
<br>
<br>
</div>
