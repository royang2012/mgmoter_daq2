clear all;
%close all;
rawPT = load('C:\\Users\\jingyis\\Documents\\us_obm_data\\05242017\\PoTi_PHA_6.txt'); %position of motors w/ time
rawV = load('C:\\Users\\jingyis\\Documents\\us_obm_data\\05242017\\DAQV66.txt');  % intensity wrt time
vaLen = floor(length(rawV)/2 - 1); %length of data from each channel
acTime = rawV(vaLen*2+1); %penultimate element in intensity array, which is the aquisition start time
rawPT(:,3) = rawPT(:,3) - acTime; %compensate for waiting time between motor start and data acquisition start
rawLength = length(rawPT(:,1)); 

xResolution = 0.15;
yResolution = 0.15;
xDimension = 15.15;
yDimension = (70-1)*0.15;
xStart = 0;
%% sort the datasheet
i = 1;
j = 1;
sortPT = rawPT;
while( j < rawLength -1 )
    i = j;
    while(abs(rawPT(j+1,2) - rawPT(j,2)) < yResolution/2 && j~=rawLength-1)   %   find the turning point of scan
        j = j + 1;
    end
    if j~=rawLength - 1;
        sortPT(i:j,:) = sortrows(rawPT(i:j,:));
    else
        sortPT(i:j+1,:) = sortrows(rawPT(i:j+1,:));    % deal with the special case of the last row
    end
    j = j+1;
end
% calibrate for xStart
sortPT(:,1) = sortPT(:,1) - xStart;
%% build coordinates
colNum = ceil(xDimension / xResolution - 1);
rowNum = floor(yDimension / yResolution + 1);
positionTime = zeros(colNum * rowNum , 3); %columns: user-defined x, user-defined y,time; x & y are pixel coordinates

xPosition = xResolution;
yPosition = 0;
i = 1;
% Find the first x coordinate value that is larger than the resolution
% point.
for k = 1:rawLength-1
    if (sortPT(k,1) > xPosition || abs(sortPT(k+1,2) - sortPT(k,2)) > yResolution/2) && abs(yPosition - sortPT(k,2)) < yResolution/2
        time = sortPT(k-1,3) + (sortPT(k,3) - sortPT(k-1,3))*(xPosition - sortPT(k-1,1))/(sortPT(k,1) - sortPT(k-1,1));
        positionTime(i,:) = [xPosition yPosition time];
        i = i + 1;
        xPosition = xPosition + xResolution;

        if abs(xPosition - xDimension) < xResolution / 2
            yPosition = yPosition + yResolution;
            xPosition = xResolution;
        end
        
    end
end
positionTime(end,:) = [xDimension - xResolution, yDimension, sortPT(end,3)];    % the last row
%% DAQ acquizition data processing
samplingRate = 80; %1/s
samplingIntev = 1000 / samplingRate; % because sampling rate of cpu is in milliseconds
effLen = ceil(positionTime(end,3) / samplingIntev);    % Length of the acquried data while the scanning is in progress

% Normalization of the data from two different channels
normalV = rawV(1:effLen);
normalV(1:vaLen) = rawV(1:vaLen)/mean(rawV(1:effLen));
normalV(vaLen+1:2 * vaLen) = rawV(vaLen+1:2 * vaLen)/mean(rawV(vaLen+1:vaLen + effLen));
%******************************************************%
%   Modify the data
%******************************************************%
% for i = 1:2*vaLen
%     if(normalV(i) > 3 )
%         normalV(i) = normalV(i-1);
%     else if (normalV(i) < 0)
%             normalV(i) = 0;
%         end
%     end
% end
% Calculate the difference between two channels
chan1 = normalV(1:vaLen);   %Raw signal of two channels
chan2 = normalV(vaLen + 1:end);
% for i = 1 : vaLen
%     diffV(i) = chan1(i) - chan2(i);
%     sumV(i) = chan1(i) + chan2(i);
% end
diffV = chan2 - chan1;
sumV = abs(chan2) + abs(chan1);
pLen = length(positionTime);
%positionValue columns 1 and 2 are not filled out and are not used
positionValue = zeros(pLen, 5); %columns: x, y, ch1-ch2/(ch1+ch2), intensity_ch1, intensity_ch2
samplingIntev = 1000 / samplingRate;
%******************************************************%
%   Add deviation correction
%******************************************************%
%dev = 0 * vaLen;
% average all sampling points between two positions
% positionTime(4899, 3) = 1.2866e+06;
% positionTime(4900, 3) = 1.2868e+06;
positionPitch = positionTime(8,3) - positionTime(7,3); % time span between two pixels
for i = 1:pLen;
    startTime = floor(positionTime(i,3)/samplingIntev); %start of pixel
    endTime = floor((positionTime(i,3) + positionPitch/2)/samplingIntev); %end of pixel
    positionValue(i,4) = mean(chan1(startTime:endTime)); %average from beginning to end of pixel for ch.1
    positionValue(i,5) = mean(chan2(startTime:endTime)); 
%     if positionValue(i,5) < -5
%         positionValue(i,5) = -5+(positionValue(i,5)+5)/10
%     end
    positionValue(i,3) = mean(diffV(startTime:endTime)./(sumV(startTime:endTime)));
end

%% plot
valueM = positionValue(:,3);
valueM2 = positionValue(:,4);
valueM3 = positionValue(:,5);
plotValue = rot90(rot90(reshape(valueM, colNum, rowNum)'));
plotValue2 = rot90(rot90(reshape(valueM2, colNum, rowNum)'));
plotValue3 = rot90(rot90(reshape(valueM3, colNum, rowNum)'));
% 
% figure; imagesc(plotValue);colorbar;colormap(gray);
% figure; imagesc(plotValue2);colorbar;colormap(gray);
% figure; imagesc(plotValue3);colorbar;colormap(gray);

figure; 
subplot(2,2,1);imagesc(plotValue);colorbar;colormap(gray);title('Ch1-Ch2/sum');
subplot(2,2,2);imagesc(plotValue2);colorbar;colormap(gray);title('Ch1');
subplot(2,2,3);imagesc(plotValue2+plotValue3);colorbar;colormap(gray);title('Ch1+Ch2');
subplot(2,2,4);imagesc(plotValue3);colorbar;colormap(gray);title('Ch2');

%% Write to file

dlmwrite('DAQV2Chan1.txt', valueM2);
dlmwrite('DAQV2Chan2.txt', valueM3);