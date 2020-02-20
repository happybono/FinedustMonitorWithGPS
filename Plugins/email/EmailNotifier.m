data = thingSpeakRead([Your ThingSpeak Channel ID],"NumMinutes", 60);
dust25 = mean(data(:, 1));
dust10 = mean(data(:, 2));

apiKey = '[Your ThingSpeak Alerts API Key]';
alertURL = "https://api.thingspeak.com/alerts/send";

options = weboptions("HeaderFields", ["ThingSpeak-Alerts-API-Key", apiKey ]);
alertBody = sprintf("Everyone may begin to experience health effects, members of sensitive groups may experience more serious health effects. People at risk should avoid to go outside. Not recommended for outdoor activities.", dust25, dust10);
alertSubject = sprintf("Outdoor Activities are discouraged!");

if dust25 > 34.0 || dust10 > 68.0 
    webwrite(alertURL, "body", alertBody, "subject", alertSubject, options);
end
