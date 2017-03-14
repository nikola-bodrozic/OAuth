<?php
	// Get keys from https://www.yelp.com/developers
	$consumer_key = "secret";
	$consumer_secret = "secret";
	$token = "secret";
	$token_secret = "secret";

	// Token object built using the OAuth library
	$token = new OAuthToken($token, $token_secret);

	// Consumer object built using the OAuth library
	$consumer = new OAuthConsumer($consumer_key, $consumer_secret);

	// Yelp uses HMAC SHA1 encoding
	$signature_method = new OAuthSignatureMethod_HMAC_SHA1();

	// Build OAuth Request using the OAuth PHP library. Uses the consumer and token object created above.
	$oauthrequest = OAuthRequest::from_consumer_and_token($consumer, $token, 'GET', $unsigned_url);

	// Sign the request
	$oauthrequest->sign_request($signature_method, $consumer, $token);

	// Get the signed URL
	$signed_url = $oauthrequest->to_url();

	// Send Yelp API Call
	$ch = curl_init($signed_url);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	curl_setopt($ch, CURLOPT_HEADER, 0);
	$data = curl_exec($ch); // Yelp response
	curl_close($ch);

	// Handle Yelp response data
	$response = json_decode($data);
	
	// Da li je dostingut max. broj pozva ka API-ju
	if(isset($response->error->id) && $response->error->id = "EXCEEDED_REQS") die ("Gotovo za danas ka API-ju.");

	// Nema reziltata pretrage
	if($response->businesses[0]->name == NULL) die ('<h1>No search results match your query.</h1>');
	
	$x0 = mt_rand(1,8);
	$x1 = mt_rand(9,15);
	?>
<!DOCTYPE html>
<html>
<head>
<title>Yelp OAuth demo</title>
</head>
<body>

	<script type="text/javascript" src="http://maps.google.com/maps/api/js?sensor=false"></script>
<script type="text/javascript">
//<![CDATA[
function initialize() {
  
	  var t1 = <?php echo $response->businesses[$x0]->location->coordinate->latitude;?>;
	  var t2 = <?php echo $response->businesses[$x0]->location->coordinate->longitude;?>;
	  
	  var t3 = <?php echo $response->businesses[$x1]->location->coordinate->latitude;?>;
	  var t4 = <?php echo $response->businesses[$x1]->location->coordinate->longitude;?>;
	  
		var mapDiv = document.getElementById('platno');
		var map = new google.maps.Map(mapDiv, {
			//center: new google.maps.LatLng(,),
			//zoom: 8,
			disableDefaultUI: true,
			mapTypeId: google.maps.MapTypeId.ROADMAP,
			scrollwheel: false,
			navigationControl: true
		});
	  
		var myLatlng1 = new google.maps.LatLng(t1,t2);
		var myLatlng2 = new google.maps.LatLng(t3,t4);
		var bounds = new google.maps.LatLngBounds();
		bounds.extend(myLatlng1);
		bounds.extend(myLatlng2);
		map.fitBounds(bounds);
	  
	  
		var marker1 = new google.maps.Marker({
		  map: map,
		  position: myLatlng1,
		  icon: 'http://chart.apis.google.com/chart?chst=d_map_pin_letter&chld=A|FF0000|000000'
		  
		});
		
		var marker2 = new google.maps.Marker({
		  map: map,
		  position: myLatlng2,
		  icon: 'http://chart.apis.google.com/chart?chst=d_map_pin_letter&chld=B|FF0000|000000'
		});
	  }
google.maps.event.addDomListener(window, 'load', initialize);
//]]>
</script>
	<div id="platno" style="width: 516px; height: 250px"><h2>Loading Map</h2></div>
	<div style="border:inset;padding: 5px; margin-top: 5px;">
	<img src="http://chart.apis.google.com/chart?chst=d_map_pin_letter&amp;chld=A|FF0000|000000" alt="hotel" style="float:left;padding:3px"/><?php echo htmlentities($response->businesses[$x0]->name);?>
	<br/><strong>Address</strong> <?php echo htmlentities($response->businesses[$x0]->location->display_address[1]." ".$response->businesses[$x0]->location->display_address[2], ENT_QUOTES);?> 
	<hr width="80%"/><strong>Page on Yelp</strong> <a href="<?php echo $response->businesses[$x0]->url;?>" target="_blank"><?php echo $response->businesses[$x0]->url;?></a>
	</div>
	
	<div style="border:inset;padding: 5px; margin-top: 5px;">
	<img src="http://chart.apis.google.com/chart?chst=d_map_pin_letter&amp;chld=B|FF0000|000000" alt="hotel" style="float:left;padding:3px"/><?php echo htmlentities($response->businesses[$x1]->name);?>
	<br/><strong>Address</strong> <?php echo htmlentities($response->businesses[$x1]->location->display_address[1]." ".$response->businesses[$x1]->location->display_address[2], ENT_QUOTES);?> 
	<hr width="80%"/><strong>Page on Yelp</strong> <a href="<?php echo $response->businesses[$x1]->url;?>" target="_blank"> <?php echo $response->businesses[$x1]->url;?> </a>
	</div>
	</body>
</html
	