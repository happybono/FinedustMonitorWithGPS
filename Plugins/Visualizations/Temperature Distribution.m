% Enter your MATLAB code below

% Read temperature data from a ThingSpeak channel for seven seperate days 
% and visualize the data in a single plot using the PLOT function. 

% Channel ID to read data from 
readChannelID = 920137; 
% Temperature Field ID 
TempFieldID = 3; 

% Channel Read API Key 
% If your channel is private, then enter the read API 
% Key between the '' below: 
readAPIKey = 'KS8FDMUY0NZ8VECV'; 

% Specify date range
dateRange = [datetime('today')-days(6),datetime('now')];
% Read data including the timestamp, and channel information.
[data,time,channelInfo] = thingSpeakRead(readChannelID,'Fields',1:3,...
                          'DateRange',dateRange);
% Create variables to store different sorts of data
pm25Data = data(:,1);
pm10Data = data(:,2);
tempData = data(:,3);

% Create a day range vector
dayRange = day(dateRange(1):dateRange(2));
% Pre-allocate matrix
AQData = zeros(length(dayRange),23);

% Generate Temperature 3D bar chart
% Get Temperature value per whole clock for each day
for m = 1:length(dayRange) % Loop over all days
    for n = 1:23 % Loop over 24 hours
        if any(day(time)==dayRange(m) & hour(time)==n); % Check if data exist for this specific time
            hourlyData = tempData((day(time)==dayRange(m) & hour(time)==n)); % Pull out the hourly Temperature value from the matrix
            AQData(m,n) = hourlyData(1); % Assign the temperature value at the time closest to the whole clock
        end
    end
end

% Plot
figure
h = bar3(datenum(dateRange(1):dateRange(2)), AQData);
for k = 1:length(h) % Change the face color for each bar
    h(k).CData = h(k).ZData;
    h(k).FaceColor = 'interp';
end
title('Temperature Distribution')
xlabel('Hour of Day')
ylabel('Days')
datetick('y','ddd') % Change the Y-Tick to display specified date format
% datetick('y','mmm dd')
ax = gca;
ax.XTick = 1:23
ax.XTickLabels = 1:23
ax.YTickLabelRotation = 30; % Rotate label for better display
colorbar % Add a color bar to indicate the scaling of color
