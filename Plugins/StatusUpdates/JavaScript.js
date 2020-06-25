<script type='text/javascript' src='https://ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js'></script>
<script type='text/javascript' src='https://www.google.com/jsapi'></script>
<script type='text/javascript'>
  
  // set your channel id here.
  var channel_id = [ThingSpeak Channel ID];

  // set your channel's read api key here if necessary.
  var api_key = '[ThingSpeak Read API Key]';

  // load the data
  function loadData() {
    // variable for the data point
    var p_pm10;
    var p_pm25;
    var p_temp;
    var p_lat;
    var p_lng;
    var p_timestamp;
    
    // get the data from thingspeak
    $.getJSON('https://api.thingspeak.com/channels/' + channel_id + '/feed/last.json?results=1&api_key=' + api_key, function(data) {
      console.log("PM sensor data: ", data);
      // get the data points
      p_pm25 = parseFloat(data.field1);
      p_pm10 = parseFloat(data.field2);
      p_temp = parseFloat(data.field3);
      p_lat = parseFloat(data.field4);
      p_lng = parseFloat(data.field5);
      p_timestamp = new Date(data.created_at);
      var timestamp = p_timestamp.toLocaleString();
      // update page
      document.getElementById('pm25').innerHTML = p_pm25 + ' ㎍ / ㎥ (' + (p_pm25 / 20 * 100).toFixed(0) + '%)';
      document.getElementById('pm10').innerHTML = p_pm10 + ' ㎍ / ㎥ (' + (p_pm10 / 50 * 100).toFixed(0) + '%)';
      document.getElementById('temperature').innerHTML = p_temp.toFixed(2) + ' ℃';
      document.getElementById('latitude').innerHTML = p_lat.toFixed(6) + '˚';
      document.getElementById('longitude').innerHTML = p_lng.toFixed(6) + '˚';
      document.getElementById('timestamp').innerHTML = '@ ' + timestamp;
    });
  }
  $(document).ready(function() {
    loadData();

    // load new data every 15 seconds
    setInterval('loadData()', 15000);
  });

</script>   
