var directionsDisplay;
var directionsService = new google.maps.DirectionsService();

function initialize() {
  directionsDisplay = new google.maps.DirectionsRenderer();
  var mapOptions = {
    zoom: 15,
    center: new google.maps.LatLng(34.008295, -81.027019),
    disableDefaultUI: true,
    draggable: false,
    scrollWheel: false,
    mapTypeControl: false,
    navigationControl: false,
    disableDoubleClickZoom: true,
    zoomControl: false
  };
  var map = new google.maps.Map(document.getElementById('map-canvas'),
      mapOptions);
      var map2 = new google.maps.Map(document.getElementById('map-canvas2'),
      mapOptions);
      var map3 = new google.maps.Map(document.getElementById('map-canvas3'),
      mapOptions);
      var map4 = new google.maps.Map(document.getElementById('map-canvas4'),
      mapOptions);
var map5 = new google.maps.Map(document.getElementById('map-canvas5'),
      mapOptions);
      var map6 = new google.maps.Map(document.getElementById('map-canvas6'),
      mapOptions);
      
  directionsDisplay.setMap(map);
  directionsDisplay.setPanel(document.getElementById('directions-panel'));

  var control = document.getElementById('control');
  control.style.display = 'block';
  map.controls[google.maps.ControlPosition.BOTTOM_CENTER].push(control);
  
  var marker1 = new google.maps.Marker({
      position: new google.maps.LatLng(34.008295, -81.027019),
      map: map,
      title: 'Woodrow Wilson House'
  });
  
  var marker2 = new google.maps.Marker({
      position: new google.maps.LatLng(34.009679, -81.027719),
      map: map,
      title: 'Township Auditorium'
  });

  var marker3 = new google.maps.Marker({
      position: new google.maps.LatLng(34.012879, -81.031085),
      map: map,
      title: 'Seibels House'
  });

  var marker4 = new google.maps.Marker({
      position: new google.maps.LatLng(34.009481, -81.028075),
      map: map,
      title: 'Robert Mills House'
  });

  var marker5 = new google.maps.Marker({
      position: new google.maps.LatLng(34.0121131, -81.032734),
      map: map,
      title: 'Site of Former Duffie-Verner House'
  });
  
    var contentString = '<div id="content">'+
      '<div id="siteNotice">'+
      '</div>'+
      '<h1 id="firstHeading" class="firstHeading">Uluru</h1>'+
      '<div id="bodyContent">'+
      '<p><b>Uluru</b>, also referred to as <b>Ayers Rock</b>, is a large ' +
      'sandstone rock formation in the southern part of the '+
      'Northern Territory, central Australia. It lies 335&#160;km (208&#160;mi) '+
      'south west of the nearest large town, Alice Springs; 450&#160;km '+
      '(280&#160;mi) by road. Kata Tjuta and Uluru are the two major '+
      'features of the Uluru - Kata Tjuta National Park. Uluru is '+
      'sacred to the Pitjantjatjara and Yankunytjatjara, the '+
      'Aboriginal people of the area. It has many springs, waterholes, '+
      'rock caves and ancient paintings. Uluru is listed as a World '+
      'Heritage Site.</p>'+
      '<p>Attribution: Uluru, <a href="http://en.wikipedia.org/w/index.php?title=Uluru&oldid=297882194">'+
      'http://en.wikipedia.org/w/index.php?title=Uluru</a> '+
      '(last visited June 22, 2009).</p>'+
      '</div>'+
      '</div>';

  var infowindow = new google.maps.InfoWindow({
      content: contentString
  });
  
  google.maps.event.addListener(marker1, 'click', function() {
    infowindow.open(map,marker1);
  });

}

function calcRoute() {
  var start = RobertMillsHouse.localeAddress;
  var end = "1703 Taylor St, Columbia, SC 29201";
  var request = {
    origin: start,
    destination: end,
    travelMode: google.maps.TravelMode.WALKING
  };
  directionsService.route(request, function(response, status) {
    if (status == google.maps.DirectionsStatus.OK) {
      directionsDisplay.setDirections(response);
    }
  });
  
}

function location(name, address)
{
   this.localeName = name;
   this.localeAddress = address;
}

var RobertMillsHouse = new location("Robert Mills House","1616 Blanding St, Columbia, SC 29201");
var TownshipAuditorium = new location("Township Auditorium", "1703 Taylor St, Columbia, SC 29201");
var SeibelsHouse = new location("Seibels House", "1601 Richland St, Columbia, SC 29201");
var DVHouse = new location("Site of Former Duffie-Verner House", "1502 Richland St, Columbia, SC 29201");
var archSites = [RobertMillsHouse, TownshipAuditorium, SeibelsHouse, DVHouse];
