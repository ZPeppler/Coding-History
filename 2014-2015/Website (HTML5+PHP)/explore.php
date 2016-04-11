<!DOCTYPE html>

<html lang="en">
<head>
<script type="text/javascript">

/***********************************************
* Limit number of checked checkboxes script- by JavaScript Kit (www.javascriptkit.com)
* This notice must stay intact for usage
* Visit JavaScript Kit at http://www.javascriptkit.com/ for this script and 100s more
***********************************************/

function checkboxlimit(checkgroup, limit){
	var checkgroup=checkgroup
	var limit=limit
	for (var i=0; i<checkgroup.length; i++){
		checkgroup[i].onclick=function(){
		var checkedcount=0
		for (var i=0; i<checkgroup.length; i++)
			checkedcount+=(checkgroup[i].checked)? 1 : 0
		if (checkedcount>limit){
			alert("You can only select a maximum of "+limit+" checkboxes")
			this.checked=false
			}
		}
	}
}

</script>

    <?php include("header.php"); ?>
    <!---Fetch Data=-->
    <?php
    $servername = "columbia1872.chir6uoioknn.us-west-2.rds.amazonaws.com";
    $username = "admin";
    $password = "columbia1872";
    $dbname = "1872db";
    $header = array("NULL");
    $rowData = array($header);
    $contentStrings = array($header);
    $latitudes = array("NULL");
    $longitudes = array("NULL");
    $placeNames = array("NULL");
    $placeAdds = array("NULL");
    $catData = array($header);
    $catPlaceData = array($header);
    $index = 1;
    $arraySize = 0;
	$customLength = 0;

    // Create connection
    $conn = new mysqli("columbia1872.chir6uoioknn.us-west-2.rds.amazonaws.com", "admin", "columbia1872", "1872db");

    // Check connection

    if ($mysqli->connect_error) 
    {
        die('Connect Error (' . $mysqli->connect_errno . ') '
                . $mysqli->connect_error);
    }

    if ($conn->connect_error) 
    {
        die("Connection failed: " . $conn->connect_error);
    }

    //Begin places/Map Marker section
    $sql = "SELECT * FROM places";
    $result2 = $conn->query($sql);
    if ($result2->num_rows > 0) 
    {
        // output data of each row
        while($row = $result2->fetch_assoc()) 
        {
        	$arraySize += 1;
            array_push($rowData, $row);
            //if there is an image, input image
            if($row["image"] != "")
			{
                $contentString = "<img src=".$row["image"]." id='plcImg'><h4 id='firstHeading' class='firstHeading'>".$row["name"]."</h4><div id='plcAddress'>".$row["address"]."</div><div id = 'bodyContent'>".$row["description"]."</div>";
            }
            //otherwise, don't show an image
            else
            {
                $contentString = "<h4 id='firstHeading' class='firstHeading'>".$row["name"]."</h4><div id = 'plcAddress'>".$row["address"]."</div><div id = 'bodyContent'>".$row["description"]."</div>";
            }
            //pushes information into the MAP JS
            	array_push($contentStrings, $contentString);
                array_push($latitudes, $row["latitude"]);      
                array_push($longitudes, $row["longitude"]);
                array_push($placeNames, $row["name"]);
                array_push($placeAdds, $row["address"]);
                //end pushing info into Map JS
                    
        }
	} 
	else 
	{
    	//if no places, return this
        echo "0 results";
    }
    //End Places/Map Marker Section



    //Begin Category/Tours Section

    $sql_cat = "SELECT * FROM categoryNames";

    $result1 = $conn->query($sql_cat);
    if ($result1->num_rows > 0) 
    {
    	?>
		<section id='cat-splash'><center>
	    	<div class='col-md-12'>
		    	<div class="panel panel-default">
			    	<div class="panel-heading">
				    	<h3 class="panel-title">Tours</h3>
			    	</div>
			    	<div class="panel-body">
						<?php while($row = $result1->fetch_assoc()) 
						{
							$catArray = array();
							$boolArray = array("NULL");
		            		?>
							<!--Button to trigger modal-->
							<a href='#myModal<?php echo $row["catId"];?>' id='modalBtn' role='button' class='btn  btn-primary' data-toggle='modal'><?php echo $row["catName"];?></a>
							<div id='myModal<?php echo $row["catId"];?>' class='modal fade' tabindex='-1' role='dialog' aria-labelledby='myModalLabel<?php echo $row["id"];?>' aria-hidden='true'>
								<div class='modal-dialog' id='modal'>
									<div class='modal-content'>
										<div class='modal-header'>
											<button type='button' class='close' data-dismiss='modal' aria-hidden='true'>×</button>
											<h3 id='myModalLabel'><?php echo $row["catName"];?></h3>
										</div>
										<div class='modal-body'>
											<p><?php echo $row["catDesc"];?></p>
											<a class='btn btn-primary' data-toggle='collapse' href='#collapse<?php echo $row["catId"];?>' aria-expanded='false' aria-controls='collapse'>Places</a>
											<div class='collapse' id='collapse<?php echo $row["catId"];?>'>
												<div class='well'>
													<!--begin places sql statment-->
													<?php 
														//$sql_places = "SELECT * FROM places";
														$catID1 = $row["catId"];
														$sql_places = "SELECT category.type_id, category.places_id, places.name, places.address, places.image FROM categoryNames, category, places WHERE categoryNames.catId = category.type_id AND places.id = category.places_id AND (category.type_id = '$catID1')";

													$result2 = $conn->query($sql_places);
													if ($result2->num_rows > 0) 
													{
														while($row2 = $result2->fetch_assoc()) 
														{
															if($row2["image"] != "")
															{
																echo "<div class='placeBlob'>
																<img src='".$row2["image"]."'><b>";
																echo $row2["name"];
																echo "</b>";
																echo $row2["address"];
																echo "</div>";
			            									}//otherwise, don't show an image
															else
															{
																echo "<div class='placeBlob'>
																<img src=''><b>";
																echo $row2["name"];
																echo "</b>";
																echo $row2["address"];
																echo "<br>";
																echo $row2["id"];
																echo "</div>";
			            									}	
														}
													}
													else
													{
														echo "No places to show.";
													}//end sql statement
													?>
												</div>
											</div>
										</div>
										<div class='modal-footer'>
											<button class='btn' data-dismiss='modal' aria-hidden='false'>Close</button>
											<button class='btn' onClick='selectTour(<?php echo $row["catId"];?>,0);'data-dismiss='modal' aria-hidden='false'>Start</button>
											<button class='btn' onClick='selectTour(<?php echo $row["catId"];?>,1);' data-dismiss='modal' aria-hidden='false'>Current</button>
										</div>
									</div>
								</div>
							</div>
							<!--End Modal Test-->
							<?php
							$catID =$row["catId"];
							$sqlCatPlaces = "SELECT categoryNames.catName, category.type_id, category.`places_id`, places.name FROM categoryNames, category, places WHERE categoryNames.catId = category.type_id AND places.id = category.places_id AND (category.type_id = '$catID')";
							$catPlacesResult = $conn->query($sqlCatPlaces);
		                  
							if($catPlacesResult ->num_rows > 0)
							{
								while($row3 = $catPlacesResult -> fetch_assoc())
								{
									array_push($catArray, $row3["places_id"]);
									sort($catArray, $sort_flags = SORT_NUMERIC);
								}
		            		}
							else
							{
								echo "0 Results?";
		            		}
							$ind = 0;
							$ind2 = 1;
							while($ind2 <= $arraySize)
							{
								if($catArray[$ind] == $ind2)
								{
									array_push($boolArray, 1);
									$ind2 += 1;
									$ind += 1;
		                		} 
								else
								{   
									array_push($boolArray, 0);
									$ind2 += 1;
		                		}
		            		}
							array_push($catData, $boolArray);
							array_push($catPlaceData, $catArray);
						}//close while($row = $result1->fetch_assoc())
					    	
						//$sql_custom = "SELECT * FROM places WHERE places.name <> 'Woodrow Wilson House'";
						$sql_custom = "SELECT * FROM places";
						?>
						<br>
						<hr>
						<a href='#myModalcustom' role='button' class='btn  btn-primary' data-toggle='modal'>Create Your Own Tour!</a>
						<div id='myModalcustom' class='modal fade' tabindex='-1' role='dialog' aria-labelledby='myModalLabel<?php echo $row["id"];?>' aria-hidden='true'>
							<div class='modal-dialog' id='modal'>
								<div class='modal-content'>
									<div class='modal-header'>
										<button type='button' class='close' data-dismiss='modal' aria-hidden='true'>×</button>
										<h3 id='myModalLabel'>Create Your Own Tour!</h3>
										Select up to 5 places to create a tour.
									</div>
									<div class='modal-body'>
										<div class='btn-group-vertical' id='verticalBtn' role="group" data-toggle='buttons'>
											<form action="form_action.asp" id="checkboxes">
											<label class="btn btn-primary" style="visibility: hidden; display: none; opacity: 0;"><input id="checkbox0" name="ckb" type="checkbox" autocomplete="off" value="0" > Empty Button </label>
											<?php $result4 = $conn->query($sql_custom);
												if ($result4->num_rows > 0) 
												{
													while($row = $result4->fetch_assoc()) 
													{                    
														// Button to trigger modal
														$customLength = $customLength+1;
		            									?>
														<div><label class="btn btn-primary"<?php if($customLength == 1) echo "style='visibility: hidden; display: none; opacity: 0;'"?> id="customButton">
															<div><input id="checkbox<?php echo $row["id"];?>" name="ckb" type="checkbox" autocomplete="off" value="<?php echo $row["id"];?>" onclick="checkControl(<?php echo $row["id"]-1;?>)" style='visibility: hidden; display: none; opacity: 0;'><label for="checkbox<?php echo $row["id"];?>" id='checkboxPadding'><span><?php echo $row["name"]; ?></span><span>&nbsp;</span></label></div>
														</label></div>
														<?php
													}
												}
											?>
											<label class="btn btn-primary" style="visibility: hidden; display: none; opacity: 0;"><input id="checkbox<?php echo $customLength+1?>" name="ckb" type="checkbox" autocomplete="off" value="<?php echo $customLength+1?>" > Empty Button </label
										</form>	
										<script>
										checkboxlimit(document.forms.checkboxes.ckb, 5);
										</script>
										</div>
									</div>	
									<div class='modal-footer'>
										<button class='btn' data-dismiss='modal' aria-hidden='false'>Close</button>
										<button class='btn' onClick='selectTour(0,0);' data-dismiss='modal' aria-hidden='false'>Start Tour</button>
										<button class='btn' onClick='selectTour(0,1);' data-dismiss='modal' aria-hidden='false'>Current</button>
									</div>
								</div>
							</div>
						</div>
		    		</div>
	    		</div>
	    	</div>
			</center>
		&copy 2015 Historic Columbia
		</section>
		<?php
    } //if ($result1->num_rows > 0)
    else 
    {
	    echo "0 results";
    }
    ?>
    <!-- Map Script-->


<script type="text/javascript">
var directionsDisplay;
var directionsService = new google.maps.DirectionsService();
var lat;
var lng;
var currentstart;
var latLngs = ["NULL"];
var arraySize = <?php echo json_encode($arraySize);?>;
var catData = <?php echo json_encode($catData);?>;
var catPlaceData = <?php echo json_encode($catPlaceData);?>;
var markers = [];
markers.push("NULL");
var content = <?php echo json_encode($contentStrings);?>;
var lats = <?php echo json_encode($latitudes);?>;
var lngs = <?php echo json_encode($longitudes);?>;
var placeNames = <?php echo json_encode($placeNames);?>;
var placeAdds = <?php echo json_encode($placeAdds);?>;
var infoWindows = ["NULL"];
var map;
var windowOpened = 0;
var selectedCategory = false;
var cityCircle;
var mapCenter = new google.maps.LatLng(34.008295, -81.027019);
var togglecurrent = false;
var toggleLimit = 5;

function initialize() 
{
	directionsDisplay = new google.maps.DirectionsRenderer({suppressMarkers: true});
   var mapOptions = {
	   zoom: 15,
       center: mapCenter,
       disableDefaultUI: true,
       draggable: true,
       scrollWheel: false,
       mapTypeControl: false,
       navigationControl: false,
       disableDoubleClickZoom: true,
       zoomControl: false
   };
   map = new google.maps.Map(document.getElementById('map-canvas'),mapOptions);
   directionsDisplay.setMap(map);
   directionsDisplay.setPanel(document.getElementById('directions-panel'));
   var control = document.getElementById('control');
   control.style.display = 'block';
   map.controls[google.maps.ControlPosition.BOTTOM_LEFT].push(control);

   //Arrays to control all the markers and infowindows
        
       

   //marker creation
   for(i = 1; i < arraySize+1; i++)
   {
		var latLng = new google.maps.LatLng(lats[i],lngs[i]);
        var marker = new google.maps.Marker({
			position: latLng,
	        map: map,
            title: placeNames[i]
        });
        markers.push(marker);
        latLngs.push(latLng);
       
        infoWindows[i] = new google.maps.InfoWindow({
        	content: content[i]
        });

        google.maps.event.addListener(markers[i], 'click', (function(i) {
        	return function(){
        		if(windowOpened != 0)
                {
                	infoWindows[windowOpened].close(map, markers[windowOpened]);
                }
                infoWindows[i].open(map, markers[i]);
                windowOpened = i;
                }
           })(i));
        }
    }

    navigator.geolocation.getCurrentPosition // this gets the current location of the user
    (
        function(position)
        {
        var pos = new google.maps.LatLng(position.coords.latitude,position.coords.longitude);
        currentstart = pos;
        }
    );

// Toggles tour selection screen
function toggleCatSplash(boolInput)
{
	if(boolInput == true)
    {
	    document.getElementById('cat-splash').style.display="none";
        document.getElementById('cat-splash').style.visibility="hidden";
        document.getElementById('cat-splash').style.opacity="0";
	}
	else
	{
		document.getElementById('cat-splash').style.display="inline";
		document.getElementById('cat-splash').style.visibility="visible";
		document.getElementById('cat-splash').style.opacity="1";
	}
}
    
// Selects the category of the tour
function selectCategory(catInput)
{
	displaySelectMarkers(catInput);
	toggleCatSplash(true);  
}

// Deselects the current category and brings back tour screen
function deselectCategory()
{
	setAllVisibility(true);
	toggleCatSplash(false);
}

// Set all markers to the visibility of the input
function setAllVisibility(boolInput)
{
	for(i = 1; i < markers.length; i++)
    {
       markers[i].setVisible(boolInput);
    }
}

// Display the markers of the chosen category
function displaySelectMarkers(markerCategory)
{
	if(markerCategory == 0)
	{
		var customPlaces = document.forms[0];
       	for(var i = 2; i < markers.length; i++)
		{
			if(customPlaces[i].checked == false)
			{   
				markers[i].setVisible(false);
			}
		}
	}
	else
	{
        var markInd = 1;
        while(markInd < markers.length)
        {
            if(catData[markerCategory][markInd] == 0)
            {
                markers[markInd].setVisible(false);
            }       
            markInd++;
        }
   	}
	markers[1].setVisible(true);
}

   
// Used for troubleshooting and debugging
function print2DArray(input)
{
	var returnString = "";
	for(var i = 1; i < input.length; i++)
	{
		returnString = returnString.concat(i+": ");
	    for(var j = 0; j < input[i].length; j++)
	    {
		    returnString = returnString.concat(input[i][j]+", ");
	    }
	    returnString = returnString.concat("\n");
	}
	return returnString;
}

// Creates a waypoint array for a custom tour
function customTour(waypts)
{
	var customPlaces = document.forms[0];
	for(var i = 0; i < customPlaces.length; i++)
	{
		if(customPlaces[i].checked)
		{
			waypts.push({
				location: latLngs[i],
				stopover: true
			});
		}
		
	}
}

// Creates a tour based on category selected
function calcTourWP(markerCategory, tourSelect)
{
	directionsDisplay.suppressMarkers = true;
	var request;
	var tourStart;
	var waypts = [];
	if(markerCategory == 0)
	{
    		customTour(waypts);
	}
	else
	{
		for(var i = 0; i < catPlaceData[markerCategory].length; i++)
		{
			waypts.push({
				location: latLngs[catPlaceData[markerCategory][i]],
				stopover: true
			});
		}
	}
	if(tourSelect==0)
	{
		tourStart = latLngs[1];
	}
	if(tourSelect==1)
	{
		tourStart = currentstart;
	}
	request = {
		origin: tourStart,
		destination: latLngs[1],
		waypoints: waypts,
		optimizeWaypoints: true,
		travelMode: google.maps.TravelMode.WALKING
	};
	
	directionsService.route(request, function(response, status)
    {
	    	if(status == google.maps.DirectionsStatus.OK)
        	{
	        	directionsDisplay.setDirections(response);
        	}
    });
}

// Selects the specific tour
function selectTour(catInput, tourInput)
{
 	selectCategory(catInput);
	calcTourWP(catInput, tourInput);
}

// Toggles a circle near the current location
function toggleCurrentLocation()
{
	
	var opacity;
	var opacity2;
	if(togglecurrent==true)
	{
		clearCircles();
		map.panTo(mapCenter);
		map.setZoom(16);
		togglecurrent = false; 
	}
	else
	{
		opacity = 0.8;
		opacity2 = 0.35;
		map.panTo(currentstart);
		map.setZoom(16);
		togglecurrent = true;
	
    	var populationOptions = {
      		strokeColor: '#04B404',
    		strokeOpacity: opacity,
   		    strokeWeight: 2,
      		fillColor: '#04B404',
      		fillOpacity: opacity2,
      		map: map,
      		center: currentstart,
      		radius: 50
    	};
    	// Add the circle for this city to the map.
    	cityCircle = new google.maps.Circle(populationOptions);
  }
}

function clearCircles()
{
	cityCircle.setMap(null);
}
    google.maps.event.addDomListener(window, 'load', initialize);

    </script>
</head>

<body>
    <center>
        <!--BEGIN MAP INTERACE-->
        <section id='explore-map'>
            <div class='col-md-8' id="mapFloat">
            	<h3>Map</h3> 
                <div id="map-canvas"></div>
            <div id="control">
            	<button onclick="deselectCategory();">End Tour</button>
				<button type="button" data-toggle="modal" data-target="#directionsModal">Get Directions</button>
				<button onclick="toggleCurrentLocation();">My Location</button>
            </div>
            </div>
            
            <!-- Directions Modal -->
<div class="modal fade" id="directionsModal" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
  <div class="modal-dialog">
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        <h4 class="modal-title" id="myModalLabel">Directions</h4>
      </div>
      <div class="modal-body">
                <div id="directions-panel"></div>
      </div>
      <div class="modal-footer">
        <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
      </div>
    </div>
  </div>
</div>


        </section><!--END MAP INTERACE-->
    </center><!-- jQuery (necessary for Bootstrap's JavaScript plugins) -->
    
</script>
<br><center><?php include("footer.php");?></center>
</body>
</html>
