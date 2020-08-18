clear all;
close all;

%ctrlPt = [0,0,0,1,0,0,0];
ctrlPt = zeros(1,10);
ctrlPt = [ ctrlPt, 1, ctrlPt ];
step = 0.01;

i=1;
degres = [0:2,5];
for degreId = 1:length(degres)
[curveSample,ts] = sampleSpline(ctrlPt,degres(degreId),step);
nonzeros= (curveSample ~= 0);

m=mean(ts(nonzeros));
plot(ts(nonzeros)-m,curveSample(nonzeros),'Linewidth',2);
axis equal 

hold on
i = i + 1;
end


set(gca,'Visible','off');

degres = [1:4];
figure;
for degreId = 1:length(degres)
[curveSample,ts] = sampleSpline(ctrlPt,degres(degreId),step);
nonzeros= (curveSample ~= 0);
nonzeros = [1:size(curveSample,2)];
%nonzeros = (nonzeros >= 2) .* (nonzeros <= 2);
m=mean(ts(nonzeros));
plot(ts(nonzeros)-m,[diff(curveSample(nonzeros))/step,0],'Linewidth',2);
axis equal 

hold on
i = i + 1;
end

set(gca,'Visible','off');


degdepart=8;
degres = [degdepart:degdepart+4];
figure;
for degreId = 1:length(degres)
[curveSample,ts] = sampleSpline(ctrlPt,degres(degreId),step);

    for i=1:(degdepart-4)
        curveSample= [diff(curveSample)/step,0];
    end

%curveSample(abs(curveSample) < 0.05)=0;    
nonzeros= (curveSample ~= 0);
nonzeros = [1:size(curveSample,2)];
%nonzeros = (nonzeros >= 2) .* (nonzeros <= 2);
m=mean(ts(nonzeros));
plot(ts(nonzeros)-m,[diff(curveSample(nonzeros))/step,0],'Linewidth',2);
axis equal 

hold on
i = i + 1;
end

set(gca,'Visible','off');

