% Enter your MATLAB code below

readChannelID = [ThingSpeak Channel ID];

readAPIKey = '[ThingSpeak Read API Key]';

trailingdays = 7

[data,timestamps] = thingSpeakRead(readChannelID,'ReadKey',readAPIKey,'Fields',[2],'NumDays',trailingdays);

TT1 = array2timetable(data,'RowTimes',timestamps);
TT2 = retime(TT1,'hourly','mean');

mytime=(TT2.Time);
mydata=(TT2.data);

mytimeTZ = datetime(mytime, 'TimeZone','UTC');
mytimeTZ.TimeZone = 'Asia/Seoul';

myhour=hour(mytimeTZ)
myday=day(mytimeTZ)

h=zeros(24,trailingdays);

t=datetime
tTZ = datetime(t, 'TimeZone','UTC');
tTZ.TimeZone = 'Asia/Seoul';

tominus=(day(tTZ)-trailingdays-1)

for i = 1:(length(myhour))
x = int8((myhour(i))+1)
y = int8((myday(i))-tominus);
z = mydata(i);
h(x,y) = z;
end

for i = 1:trailingdays+1
[DayNumber, DayName] = weekday(t-trailingdays-1+i)
xnames{i}=DayName
end

bar3(h)

Z = magic(5);
b = bar3(h);
colorbar

for h = 1:length(b)
    zdata = b(h).ZData;
    b(h).CData = zdata;
    b(h).FaceColor = 'interp';
end

az = 27.5;
el = 20.5;
view(az, el);
ylim([0 25])
ynames = {'06:00'; '12:00'; '18:00'; '00:00'};
set(gca,'ytick',[2 8 14 20],'yticklabel',ynames)
set(gca,'xtick',[1:trailingdays+1],'xticklabel',xnames)
xtickangle(90)

title(['PM 10.0 Distribution (',num2str(trailingdays),' days trailing)'])
xlabel('Days')
ylabel('Time (UTC)')
zlabel('PM 10.0')
