clear all;
close all;

%ctrlPt = [0,0,0,1,0,0,0];
ctrlPt = zeros(1,20);
ctrlPt = [ ctrlPt, 1, ctrlPt ];
step = 0.01;

% i=1;
% degres = [0:2,5];
% for degreId = 1:length(degres)
% [curveSample,ts] = sampleSpline(ctrlPt,degres(degreId),step);
% nonzeros= (curveSample ~= 0);
% 
% m=mean(ts(nonzeros));
% plot(ts(nonzeros)-m,curveSample(nonzeros),'Linewidth',2);
% axis equal 
% 
% hold on
% i = i + 1;
% end
% 
% 
% set(gca,'Visible','off');
% 
% degres = [1:4];
% figure;
% for degreId = 1:length(degres)
% [curveSample,ts] = sampleSpline(ctrlPt,degres(degreId),step);
% nonzeros= (curveSample ~= 0);
% nonzeros = [1:size(curveSample,2)];
% %nonzeros = (nonzeros >= 2) .* (nonzeros <= 2);
% m=mean(ts(nonzeros));
% plot(ts(nonzeros)-m,[diff(curveSample(nonzeros))/step,0],'Linewidth',2);
% axis equal 
% 
% hold on
% i = i + 1;
% end
% 
% set(gca,'Visible','off');


degdepart=3;
degres = [degdepart,degdepart+1,degdepart+3,degdepart+5];
idF=figure;

idF1=figure;

for degreId = 1:length(degres)
figure(idF1);
hold on
[curveSample,ts] = sampleSpline(ctrlPt,degres(degreId),step);
%curveSample(abs(curveSample) < 0.05)=0;    
nonzeros= (curveSample ~= 0);
%nonzeros = [1:size(curveSample,2)];
%nonzeros = (nonzeros >= 2) .* (nonzeros <= 2);
m=mean(ts(nonzeros));
plot(ts(nonzeros)-m,curveSample(nonzeros),'Linewidth',2);
axis equal 
title("base");



figure(idF);
newCtrlPt = ctrlPt;

nbder = 3;
    for i=1:nbder
%         curveSample= [diff(curveSample)/step,0];
        newCtrlPt = derivateSpline(newCtrlPt,degres(degreId)+1-i);
    end

    [curveSample,ts] = sampleSpline(newCtrlPt,degres(degreId)-nbder,step);

%curveSample(abs(curveSample) < 0.05)=0;    
nonzeros= (curveSample ~= 0);
%nonzeros = [1:size(curveSample,2)];
%nonzeros = (nonzeros >= 2) .* (nonzeros <= 2);
m=mean(ts(nonzeros));
nonzeros=find(nonzeros == 1);
nonzeros=[max(min(nonzeros)-100:min(nonzeros),1),nonzeros,min(max(nonzeros):max(nonzeros+100),size(ts,2))];
plot(ts(nonzeros)-m,curveSample(nonzeros),'Linewidth',2);
%axis equal 

hold on
i = i + 1;
end

set(gca,'Visible','off');

