% Read PM 10.0 value data from a ThingSpeak channel for seven seperate days 
% and visualize the data in a single plot using the PLOT function. 

% Channel ID to read data from 
readChannelID = [ThingSpeak Channel ID]; 
% PM 10.0 Field ID 
pm10FieldID = [ThingSpeak Field ID]; 

% Channel Read API Key 
% If your channel is private, then enter the read API 
% Key between the '' below: 
readAPIKey = '[ThingSpeak Read API Key]'; 

% Specify date range
dateRange = [datetime('today')-days(6),datetime('now')];
% Read data including the timestamp, and channel information.
[data,time,channelInfo] = thingSpeakRead(readChannelID,'Fields',1:2,...
                          'DateRange',dateRange);
% Create variables to store different sorts of data
pm25Data = data(:,1);
pm10Data = data(:,2);

% Create a day range vector
dayRange = day(dateRange(1):dateRange(2));
% Pre-allocate matrix
AQData = zeros(length(dayRange),24);

% Generate PM 10.0 value 3D bar chart
% Get PM 10.0 value per whole clock for each day
for m = 1:length(dayRange) % Loop over all days
    for n = 1:24 % Loop over 24 hours
        if any(day(time)==dayRange(m) & hour(time)==n); % Check if data exist for this specific time
            hourlyData = pm10Data((day(time)==dayRange(m) & hour(time)==n)); % Pull out the hourly PM 10.0 value from the matrix
            AQData(m,n) = hourlyData(1); % Assign the PM 10.0 value at the time closest to the whole clock
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
title('PM 10.0 Distribution')
xlabel('Hour of Day')
ylabel('Days')
datetick('y','ddd') % Change the Y-Tick to display specified date format
% datetick('y','mmm dd')
ax = gca;
ax.XTick = 1:24
ax.XTickLabels = 1:24
ax.YTickLabelRotation = 30; % Rotate label for better display
colorbar % Add a color bar to indicate the scaling of color
