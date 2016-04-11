
<h3>Manage Categories</h3>
<div id="adminContent">
<?php	
$sql1 = "SELECT * FROM categoryNames";
$sql2 = "SELECT * FROM places";
$sqlUpdateCat ="";
$sqlUpdateLocation ="";
$sqlNewLocation ="";
$id = 0;

$result1 = $conn->query($sql1);

if ($result1->num_rows > 0) {
// output data of each row
while($row1 = $result1->fetch_assoc()) {
$category = $row1["catName"];
$categoryDescription = $row1["catDesc"];
$id = $row1["catId"];
?>
<div id='catRow'>
<a id='catHeader' data-toggle="collapse" href="#collapse<?php echo $row1["catId"] ?>" aria-expanded="false" aria-controls="collapse<?php echo $row1["catId"] ?>">
	<?php echo $row1["catName"] ?>
</a>

<a data-toggle="collapse" href="#editCat<?php echo $row1["catId"] ?>" aria-expanded="false" aria-controls="editCat<?php echo $row1["catId"] ?>">
	(edit name)
</a>
<!--EDIT CATEGORY START-->
<div class="collapse" id="editCat<?php echo $row1["catId"] ?>">
  <div class="well">
	<form method="POST">
	<table id='catEdit'><tr>
		<td>EDIT CATEGORY NAME</td>
		<td><input type='text' value='<?php echo $category ?>' name='categoryName' onfocus="this.value='';"></td>
	</tr><tr>
		<td>EDIT CATEGORY DESCRIPTION</td>
		<td><input type='text' value='<?php echo $categoryDescription ?>' name='categoryDesc' ></td>
	</tr></table>
	<button input type='submit' value='submit'>Submit</button>
	</form>
  </div>
</div>
<!--EDIT CATEGORY END-->
<!--EDIT LOCATIONS START-->
<div class="collapse" id="collapse<?php echo $row1["catId"] ?>">
  <div class="well">
	{List Places}, with edit icons
  </div>
</div>
<!--EDIT LOCATIONS END-->
</div> <!--ends catRow-->
<?php
}
//end category php, begin modal
} else {
echo "0 results";
}
?>

<br>
      <button class='btn btn-primary' data-toggle="modal" data-target="#catModal">ADD CATEGORY</button>
      <button class='btn btn-primary' data-toggle="modal" data-target="#locModal">ADD LOCATION</button>
	<br><br>

<!-- New Category Modal -->
<div class="modal fade" id="catModal" tabindex="-1" role="dialog" aria-labelledby="catModalLabel" aria-hidden="true">
  <div class="modal-dialog">
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        <h4 class="modal-title" id="catModalLabel">New Category</h4>
      </div>
      <div class="modal-body">
	<form method="POST">
	<table><tr>
		<td>NAME</td>
		<td><input type='text' name='newCatName' value=''></td>
	</tr><tr>
		<td>DESCRIPTION</td>
		<td><input type='text' name='newCatDesc' value=''></td>
	</tr><tr>
		<td>ADD PLACES</td>
		<td>
			<select name="placeID">
<?php
$result1 = $conn->query($sql2);
if ($result1->num_rows > 0) {
// output data of each row
while($row1 = $result1->fetch_assoc()) { ?>
<option value='<?php echo $row1["id"] ?>' id='<?php echo $row1["id"] ?>' name='<?php echo $row1["name"] ?>'>
<?php echo $row1["name"] ?>
</option>

<?php
}
}
else{
echo "<option>0 Places</option>";
}

?>
			</select>			
		</td>
	</tr>
	</table>
      </div>
      <div class="modal-footer">
        <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
        <button type="submit" name="addNewCat" class="btn btn-primary">Save changes</button>
      </div>
	</form>
    </div>
  </div>
</div>


<!-- New Location Modal -->
<div class="modal fade" id="locModal" tabindex="-1" role="dialog" aria-labelledby="locModalLabel" aria-hidden="true">
  <div class="modal-dialog">
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        <h4 class="modal-title" id="locModalLabel">New Location</h4>
      </div>
      <div class="modal-body">
	<form method="POST">
	<table><tr>
		<td>NAME</td>
		<td><input type='text' name='newLocName' value=''></td>
	</tr><tr>
		<td>IMAGE URL</td>
		<td><input type='text' name='newLocImg' value=''></td>
	</tr><tr>
		<td>ADDRESS</td>
		<td><input type='text' name='newLocAdd' value=''></td>
	</tr><tr>
		<td>LATITUDE</td>
		<td><input type='text' name='newLocLat' value=''></td>
	</tr><tr>
		<td>LONGITUDE</td>
		<td><input type='text' name='newLocLong' value=''></td>
	</tr><tr>
		<td>DESCRIPTION</td>
		<td><input type='text' name='newLocDesc' value=''></td>
	</tr><tr>
		<td>SELECT CATEGORY</td>
		<td>
			<select>
<?php
$result1 = $conn->query($sql1);
if ($result1->num_rows > 0) {
// output data of each row
while($row1 = $result1->fetch_assoc()) { ?>
<option value='<?php echo $row1["catName"] ?>'>
<?php echo $row1["catName"] ?>
</option>

<?php
}
}
else{
echo "<option>0 Categories</option>";
}
?>
			</select>
		</td>
	</tr>
</table>

      </div>
      <div class="modal-footer">
        <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
        <button type="button" class="btn btn-primary">Save changes</button>
      </div>
	</form>
    </div>
  </div>
</div>


<!--END PHP CATEGORY CODE-->
</div>
<?php
if($_POST['addNewCat'] and $_SERVER["REQUEST_METHOD"] == "POST")
{	
	addNewCat();	
}
function addNewCat()
{
	$newCatId = $id+1; 
	$newCatName = $_POST["newCatName"];
	$newCatDesc = $_POST["newCatDesc"];
	$newPlaceId = $_POST["placeID"];

	$sqlNewCategoryNames = "INSERT INTO categoryNames(catId, catName, catDesc) VALUES('$newCatId','$newCatName','$newCatDesc')";
	$sqlNewCategory = "INSERT INTO category(type_id, places_Id) VALUES('$newCatId','$newPlaceId')";

	if(($conn->query($sqlNewCategoryNames) === TRUE) and ($conn->query($sqlNewCategory) === TRUE))
	{
		echo "Successfully added a new category";
	}
	else
	{
		echo "sql is bad";
	}
}
?>
