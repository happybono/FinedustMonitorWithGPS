data = thingSpeakRead([Your ThingSpeak Channel ID], "NumMinutes", 60);
dust25 = mean(data(:, 1));
dust10 = mean(data(:, 2));

apiKey = '[Your ThingSpeak Alerts API Key]';
alertURL = "https://api.thingspeak.com/alerts/send";

options = weboptions("HeaderFields", ["ThingSpeak-Alerts-API-Key", apiKey ]);

alertBodyLv5 = sprintf("Everyone may begin to experience health effects, members of sensitive groups may experience more serious health effects. People at risk should avoid to go outside. Not recommended for outdoor activities.", dust25, dust10);
alertSubjectLv5 = sprintf("Not recommended for outdoor activities.");

alertBodyLv6 = sprintf("Air quality is severe. Everyone may experience more serious health effects. People at risk should be avoided to go outside and should limit the outdoor activities to minimum. Outdoor activities are discouraged.", dust25, dust10);
alertSubjectLv6 = sprintf("Everyone may experience more serious health effects.");

alertBodyLv7 = sprintf(" Health warnings of emergency conditions. People at risk should be avoided to go outside and should limit the outdoor activities to minimum. Outdoor activities are strongly discouraged.", dust25, dust10);
alertSubjectLv7 = sprintf("Outdoor activities are strongly discouraged!");


if dust25 >= 35.0 || dust10 >= 69.0 
    webwrite(alertURL, "body", alertBodyLv5, "subject", alertSubjectLv5, options);
elseif dust25 >= 44.0 || dust10 >= 85.0 
    webwrite(alertURL, "body", alertBodyLv6, "subject", alertSubjectLv6, options);
elseif dust25 >= 52.0 || dust10 >= 102.0 
    webwrite(alertURL, "body", alertBodyLv7, "subject", alertSubjectLv7, options);
end
