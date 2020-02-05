/**
* This function returns the latitude and longitude of a given address using the Geocoder class. 
*
* @param {A2} address A cell that contains an address.
* @return The latitude and the longitude of the given address.
* @customfunction
*/
function getCoordinates(address)
{  
  // Initialize the geocoder object.
  var geocoder = Maps.newGeocoder().geocode(address);
  
  // Initialize the latitude / longitude variables.
  var lat = 0;
  var long = 0
  
  // Check if the response returned without a problem.
  if (geocoder.status == 'OK') 
  {
      // Retrieve the latitude / longitude information.
      lat = geocoder["results"][0]["geometry"]["location"]["lat"];
      long = geocoder["results"][0]["geometry"]["location"]["lng"];
  }
  
  // Return the latitude / longitude information as string.  
  return lat + ", " + long;
}
  
// --------------------------------------------------------------------------------------------------------------
  
/**
* This function returns the latitude and longitude of a given address as an array using the Geocoder class. 
*
* @param {A2} address A cell that contains an address.
* @return The latitude and the longitude of the given address.
* @customfunction
*/
function getCoordinatesArray(address)
{  
  // Initialize the geocoder object.
  var geocoder = Maps.newGeocoder().geocode(address);
  
  // Initialize the latitude / longitude variables.
  var lat = 0;
  var long = 0
  
  // Check if the response returned without a problem.
  if (geocoder.status == 'OK') 
  {
      // Retrieve the latitude / longitude information.
      lat = geocoder["results"][0]["geometry"]["location"]["lat"];
      long = geocoder["results"][0]["geometry"]["location"]["lng"];
  }
  
  // Return the latitude / longitude information as an array.  
  return [lat, long];
}
  
// --------------------------------------------------------------------------------------------------------------
  
/**
* This function returns the latitude of the given address using the Geocoder class.
*
* @param {A2} address A cell that contains an address.
* @return The latitude of the given address.
* @customfunction
*/
function getLatitude(address)
{        
  // Initialize the geocoder object.
  var geocoder = Maps.newGeocoder().geocode(address);
  
  // Initialize the latitude variable.
  var lat = 0;
  
  // Get the latitude if the response returned without a problem.
  if (geocoder.status == 'OK')   
      lat = geocoder["results"][0]["geometry"]["location"]["lat"];      
  
  // Return the latitude as double.
  return lat;
}
  
// --------------------------------------------------------------------------------------------------------------
  
/**
* This function returns the longitude of the given address using the Geocoder class.
*
* @param {A2} address A cell that contains an address.
* @return The longitude of the given address.
* @customfunction
*/
function getLongitude(address)
{    
  // Initialize the geocoder object.
  var geocoder = Maps.newGeocoder().geocode(address);
  
  // Initialize the longitude variable.
  var long = 0;
  
  // Get the latitude if the response returned without a problem.
  if (geocoder.status == 'OK')   
      long = geocoder["results"][0]["geometry"]["location"]["lng"];
  
  // Return the longitude as double.  
  return long;
}
  
// --------------------------------------------------------------------------------------------------------------
  
/**
* This function returns the address from a given pair of latitude and longitude using the Geocoder class.
* In other words, it performs reverse geocoding.
*
* @param {A2} lat A cell that contains a latitude value between -90 and +90 degrees.
* @param {B2} long A cell that contains a longitude value between -180 and +180 degrees.
* @return The address from a given pair of latitude and longitude.
* @customfunction
*/
function getAddress(lat, long)
{
  
  // Checking the input variables:
  // The valid range of latitude in degrees is -90 and +90 for the Southern and Northern hemisphere respectively.   
  if(lat < -90 || lat > 90)
    return "Invalid Latitude";
 
  // Longitude is in the range -180 and +180 specifying coordinates West and East of the Prime Meridian, respectively.
  if(long < -180 || long > 180)
    return "Invalid Longitude";
    
  var response = Maps.newGeocoder().reverseGeocode(lat, long);
  var address = "";
  
  if (response.status == 'OK')
    address = response["results"][0]["formatted_address"];
  
  return address;
}
 
