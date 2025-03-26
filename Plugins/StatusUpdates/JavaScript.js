<script>
// set your channel id here
var channel_id = '[ThingSpeak-Channel-ID]';

// set your channel's read api key here if necessary
var api_key = '[ThingSpeak-Read-API-Key]';

// variables for the data point
var p_pm10,
    p_pm25,
    p_temp,
    p_lat,
    p_lng,
    p_address,
    p_timestamp,
    timestamp;

// Object to store previous values for per-character comparison.
var previousValues = {
    pm25: '',
    pm10: '',
    temperature: '',
    latlng: '',
    address: '',
    timestamp_text: ''
};

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
    p_address = '';
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

    geocoder.geocode({ 'location': latlng, 'language': 'ko' }, function(results, status) {
        if (status === 'OK') {
            if (results[0]) {
                // Extract the full Korean address from the result
                var fullKoreanAddress = results[0].formatted_address;

                // Optionally, extract a specific part of the address if needed
                // Example: Display only the district or city
                var addressComponents = results[0].address_components;
                var shortAddress = fullKoreanAddress; // Default to full address
                
                // Iterate through address components to extract desired details
                addressComponents.forEach(function(component) {
                    if (component.types.includes('locality')) {
                        shortAddress = component.long_name; // City or district
                    }
                });

                // Set the Korean address to the address variable
                p_address = fullKoreanAddress;
            } else {
                console.log('No results found');
                p_address = 'No results found';
            }
        } else {
            console.log('Geocoder failed due to: ' + status);
            p_address = 'Geocoder failed due to: ' + status;
        }

        // Update the UI with the Korean address
        outputData();
    });
}
  
/* Helper to check if a character is a digit */
function isDigit(ch) {
    return /\d/.test(ch);
}

/* Animate a single digit from old to new.
   The animation updates the content of 'span' gradually, incrementing or decrementing by 1 every 50ms.
*/
function animateDigit(span, start, end) {
    var current = start;
    var step = (end > start) ? 1 : -1;
    var delay = 50; // milliseconds

    function update() {
        current += step;
        // If we haven't reached the target yet:
        if ((step > 0 && current < end) || (step < 0 && current > end)) {
            span.innerHTML = current;
            setTimeout(update, delay);
        } else {
            // Ensure final value is set and remove the flipping class.
            span.innerHTML = end;
            span.classList.remove("flipping");
        }
    }
    // Start only if the numbers differ.
    if (start !== end) {
        setTimeout(update, delay);
    }
}

/* Update an element’s content with per-character flip animation.
   - Splits the new value into characters.
   - Generates a span for each character with a unique id so we can update it later.
   - For the address field, wrap the content in a hyperlink while preserving proper encoding.
*/
function updateValue(elementId, newValue) {
    var element = document.getElementById(elementId);
    var previousText = previousValues[elementId] || '';
    var previousChars = previousText.split('');
    var newChars = newValue.split('');
    var html = '';

    for (var i = 0; i < newChars.length; i++) {
        var char = newChars[i];
        var needsFlip = (previousChars[i] === undefined || char !== previousChars[i]);
        var displayChar = (char === " ") ? "&nbsp;" : char;
        // Assign a unique id for each character span.
        html += '<span id="' + elementId + '_span_' + i + '" class="' + (needsFlip ? 'flipping' : '') + '">' + displayChar + '</span>';
    }

    // For the "address" field, wrap the animated content in a hyperlink.
    if (elementId === 'address') {
        html = '<a href="http://google.com/maps/place/?q=' +
               encodeURIComponent(newValue.normalize('NFC')) +
               '" target="_blank">' + newValue + '</a>';
    }

    element.innerHTML = html;
    // Now, for each character index that represents a digit and that needs updating,
    // run the counting animation if applicable.
    for (var i = 0; i < newChars.length; i++) {
        if (previousChars[i] !== undefined && isDigit(previousChars[i]) && isDigit(newChars[i]) && previousChars[i] !== newChars[i]) {
            var spanElement = document.getElementById(elementId + '_span_' + i);
            animateDigit(spanElement, parseInt(previousChars[i], 10), parseInt(newChars[i], 10));
        }
    }
    previousValues[elementId] = newValue;
}

function outputData() {
    // Ensure temperature displays with two decimal places
    updateValue("temperature", p_temp.toFixed(2) + " °C");
    updateValue("pm25", p_pm25 + " μg / m³  ( " + ((p_pm25 / 20) * 100).toFixed(0) + " % )");
    updateValue("pm10", p_pm10 + " μg / m³  ( " + ((p_pm10 / 50) * 100).toFixed(0) + " % )");
    updateValue("latlng", p_lat.toFixed(6) + "˚, " + p_lng.toFixed(6) + "˚");
    updateValue("address", p_address);
    // Update timestamp text so that the fixed icon remains intact
    updateValue("timestamp_text", timestamp);
}

// Expose initPage globally.
window.initPage = initPage;
</script>
