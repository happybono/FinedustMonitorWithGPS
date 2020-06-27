<script>
  
// set your channel id here
var channel_id = [ThingSpeak Channel ID];

// set your channel's read api key here if necessary
var api_key = '[ThingSpeak Read API Key]';

// variable for the data point
var p_pm10,
  p_pm25,
  p_temp,
  p_lat,
  p_lng,
  p_address,
  p_timestamp,
  timestamp;

function initPage() {
  loadData();
  setInterval(loadData, 15000);
}

function loadData() {
  p_pm10 = 0;
  p_pm25 = 0;
  p_temp = 0;
  p_lat = 0;
  p_lng = 0;
  p_address = 0;
  p_timestamp = 0;

  // get the data from thingspeak
  $.getJSON('https://api.thingspeak.com/channels/' + channel_id + '/feed/last.json?results=1&api_key=' + api_key, function(data) {
    // get the data points
    p_pm25 = parseFloat(data.field1);
    p_pm10 = parseFloat(data.field2);
    p_temp = parseFloat(data.field3);
    p_lat = parseFloat(data.field4);
    p_lng = parseFloat(data.field5);
    p_timestamp = new Date(data.created_at);
    timestamp = p_timestamp.toLocaleString();
    revGeocode();
  });
}

function revGeocode() {
  var geocoder = new google.maps.Geocoder();
  var latlng = {
    lat: parseFloat(p_lat.toFixed(6)),
    lng: parseFloat(p_lng.toFixed(6))
  };

  geocoder.geocode({
    'location': latlng
  }, function(results, status) {
    if (status === 'OK') {
      if (results[3]) {
        p_address = results[3].formatted_address;
      } else {
        console.log('No results found');
      }
    } else {
      console.log('Geocoder failed due to: ' + status);
    }

    outputData();
  });
}

function outputData() {
  // update page
  document.getElementById('pm25').innerHTML = p_pm25 + ' ㎍ / ㎥ (' + (p_pm25 / 20 * 100).toFixed(0) + '%)';
  document.getElementById('pm10').innerHTML = p_pm10 + ' ㎍ / ㎥ (' + (p_pm10 / 50 * 100).toFixed(0) + '%)';
  document.getElementById('temperature').innerHTML = p_temp.toFixed + ' °C';
  document.getElementById('latlng').innerHTML = p_lat.toFixed(6) + '˚, ' + p_lng.toFixed(6) + '˚';
  document.getElementById('address').innerHTML = p_address;
  document.getElementById('timestamp').innerHTML = '@ ' + timestamp;
}

</script>
