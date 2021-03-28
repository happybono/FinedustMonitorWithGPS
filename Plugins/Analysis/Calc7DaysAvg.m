% Read data over the 7 consecutive days from a ThingSpeak channel and write 
% the average to another ThingSpeak channel. 

% Channel 920137 contains data from the HAPPYBONO-DMS-GPS I : PM2.5 and PM10.0 sensors with 
% Air Quality Index (AQI), located in Jeongwang 3-dong, South Korea.
% The data is collected once every minute. 

% Channel ID to read data from 
readChannelID = [ThingSpeak Channel ID]; 

% PM25, PM10, Temp Field ID 
PM25FieldID = 1; 
PM10FieldID = 2;
TempFieldID = 3;

% Channel Read API Key 
% If your channel is private, then enter the read API Key between the '' below: 
readAPIKey = '[ThingSpeak Read API Key]'; 

% Get data for the last seven consecutive days from the HAPPYBONO-DMS-GPS I :
% PM2.5 and PM10.0 sensors with Air Quality Index (AQI) channel.

PM25 = thingSpeakRead(readChannelID,'Fields',PM25FieldID,'NumDays',7,'ReadKey',readAPIKey); 
PM10 = thingSpeakRead(readChannelID,'Fields',PM10FieldID,'NumDays',7,'ReadKey',readAPIKey);
Temp = thingSpeakRead(readChannelID,'Fields',TempFieldID,'NumDays',7,'ReadKey',readAPIKey);

% Calculate the average PM25, PM10, and Temperature. 
avgPM25 = mean(PM25); 
avgPM10 = mean(PM10); 
avgTemp = mean(Temp);

display(avgPM25,'Average PM 2.5'); 
display(avgPM10,'Average PM 10.0'); 
display(avgTemp,'Average Temperature'); 

fprintf(['Note: To write data to another channel, assign the write channel ID \n',... 
    'and API Key to ''writeChannelID'' and  ''writeAPIKey'' variables. Also \n',...
    'uncomment the line of code containing ''thingSpeakWrite'' \n',... 
    '(remove ''%%'' sign at the beginning of the line.)']); 
     
% To store the calculated average data, write it to a channel other 
% than the one used for reading data. To write to a channel, assign the 
% write channel ID to the 'writeChannelID' variable, and the write API Key 
% to the 'writeAPIKey' variable below. Find the write API Key in the right 
% side pane of this page. 

% Replace the [] with channel ID to write data to: 
writeChannelID = [ThingSpeak Channel ID]; 
% Enter the Write API Key between the '' below: 
writeAPIKey = '[ThingSpeak Write API Key]'; 

% Writing values to corresponding fields in specified channel below. 
thingSpeakWrite([ThingSpeak Channel ID], [avgPM25, avgPM10, avgTemp],'Fields', [1, 2, 3], 'WriteKey', '[ThingSpeak Write API Key]');
