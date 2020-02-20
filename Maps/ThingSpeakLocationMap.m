% Read the last 200 data points from a ThingSpeak Channel 
data = thingSpeakRead([Channel ID],'ReadKey',[Read API Key],'NumPoints',200); 

% Assign latitude to field4
latitude = data(:,4); 

% Assign longitude to field5 
longitude = data(:,5); 

% Load map image 
url = 'https://upload.wikimedia.org/wikipedia/commons/thumb/7/74/Mercator-projection.jpg/773px-Mercator-projection.jpg'; 
img = imread(url); 
[imgH,imgW,~] = size(img); 
image(img/2+127) 
axis off 

% Adjust lat/long for map image 
mercatorLon = @(lon) mod((lon+180)*imgW/360, imgW); 
mercatorLat = @(lat) imgH/2-log(tan((lat+90)*pi/360))*imgW/(2*pi); 
x = mercatorLon(longitude); 
y = mercatorLat(latitude); 

% Plot markers on map 
hold on 
plot(x, y, 'r.', 'MarkerSize', 10) 
plot(x(end), y(end), 'ro', 'MarkerSize', 10) 
hold off 
axis equal 
axis([325 719 84 395])
