close all;
clear all;
ctrlPt = zeros(1,30);
ctrlPt = [ ctrlPt, 1, ctrlPt ];
step = 0.01;

globalScaleX = 1;
globalScaleY = 0;
scaleglob = [ 0 0];

degreMax = 6;
f=figure;
outputFolder="D:\Pictures\spline\";
curveGrid=cell(degreMax+1,degreMax+1);
colors = {'b','c','g','m','r','y','k'};
for i=0:degreMax
   subplot(degreMax+2,degreMax+2,i*(degreMax+2)+2);
    
    [curveSample,ts] = sampleSpline(ctrlPt,i,step);
    
    %curveSample(abs(curveSample) < 0.05)=0;    
    nonzeros= (curveSample ~= 0);
    %nonzeros = [1:size(curveSample,2)];
    %nonzeros = (nonzeros >= 2) .* (nonzeros <= 2);
    m=mean(ts(nonzeros));
    nonzeros=find(nonzeros == 1);
    nonzeros=[max(min(nonzeros-1/step):min(nonzeros),1),nonzeros,min(max(nonzeros):max(nonzeros+1/step),size(ts,2))];
    plot(ts(nonzeros)-m,curveSample(nonzeros),'Linewidth',2,'Color',colors{i+1});
    alpha = max ([-min(curveSample(nonzeros)),max(curveSample(nonzeros))]);
    scaleglob(2) = max([ scaleglob(2) alpha]);
    scaleglob(1) = max([ scaleglob(1) max(ts(nonzeros)-m)]); 
%   axis([min(ts(nonzeros)-m),max(ts(nonzeros)-m),min(curveSample(nonzeros)),max(curveSample(nonzeros))]);
    axis([min(ts(nonzeros)-m),max(ts(nonzeros)-m),-alpha,alpha]);
    set(gca,'Visible','off');
    v.sample = curveSample(nonzeros);
    v.ts = ts(nonzeros)-m;
    curveGrid{i+1,1} = v;
    
    newCtrlPt = ctrlPt;
    for j=1:i
        disp([int2str(i) '/' int2str(degreMax) ' : ' int2str(j)]);
        newCtrlPt = derivateSpline(newCtrlPt,i-j+1);

         subplot(degreMax+2,degreMax+2,i*(degreMax+2)+2+j);

         [curveSample,ts] = sampleSpline(newCtrlPt,i-j,step);

        %curveSample(abs(curveSample) < 0.05)=0;    
        nonzeros= (curveSample ~= 0);
        %nonzeros = [1:size(curveSample,2)];
        %nonzeros = (nonzeros >= 2) .* (nonzeros <= 2);
        m=mean(ts(nonzeros));
        nonzeros=find(nonzeros == 1);
        nonzeros=[max(min(nonzeros-1/step):min(nonzeros),1),nonzeros,min(max(nonzeros):max(nonzeros+1/step),size(ts,2))];
         plot(ts(nonzeros)-m,curveSample(nonzeros),'Linewidth',2,'Color',colors{i+1-j});
        alpha = max ([-min(curveSample(nonzeros)),max(curveSample(nonzeros))]);
        scaleglob(2) = max([ scaleglob(2) alpha]);
        scaleglob(1) = max([ scaleglob(1) max(ts(nonzeros)-m)]); 
   %   axis([min(ts(nonzeros)-m),max(ts(nonzeros)-m),min(curveSample(nonzeros)),max(curveSample(nonzeros))]);
       axis([min(ts(nonzeros)-m),max(ts(nonzeros)-m),-alpha,alpha]);
        set(gca,'Visible','off');
        v.sample = curveSample(nonzeros);
        v.ts = ts(nonzeros)-m;
        curveGrid{i+1,j+1} = v;
    end
   
end


for j=1:(degreMax+1)
    subplot(degreMax+2,degreMax+2,(degreMax+1)*(degreMax+2)+j+1);
    ymax = 0;
    xmax = 0;
    for i=j:(degreMax+1)
        
        ts=curveGrid{i,j}.ts;
        sample = curveGrid{i,j}.sample;
        y = max([-min(sample),max(sample)]);
        ymax = max([y ymax]);
        x = max([-min(ts),max(ts)]);
        xmax = max([x xmax]);
        
        if (globalScaleX)
           xmax = scaleglob(1); 
        end
        if (globalScaleY)
           ymax = scaleglob(2); 
        end
        
       plot(ts,sample,'Linewidth',2,'color',colors{abs(i-j)+1});
       axis([-xmax,xmax,-ymax,ymax]);
        
       
       hold on
    end
     set(gca,'Visible','off');
end

for i=1:(degreMax+1)
    subplot(degreMax+2,degreMax+2,(i-1)*(degreMax+2)+1);
    ymax = 0;
    xmax = 0;
    for j=i:-1:1
        
        ts=curveGrid{i,j}.ts;
        sample = curveGrid{i,j}.sample;
        y = max([-min(sample),max(sample)]);
        ymax = max([y ymax]);
        x = max([-min(ts),max(ts)]);
        xmax = max([x xmax]);
       plot(ts,sample,'Linewidth',2,'color',colors{abs(i-j)+1});
        if (globalScaleX)
           xmax = scaleglob(1); 
        end
        if (globalScaleY)
           ymax = scaleglob(2); 
        end
        axis([-xmax,xmax,-ymax,ymax]);
       hold on
    end
     set(gca,'Visible','off');
end


for i=1:(degreMax+1)
    subplot(degreMax+2,degreMax+2,(degreMax+1)*(degreMax+2)+j+1);
    ymax = 0;
    xmax = 0;
    for j=1:i
        subplot(degreMax+2,degreMax+2,(i-1)*(degreMax+2)+j+1);
        ts=curveGrid{i,j}.ts;
        sample = curveGrid{i,j}.sample;
        y = max([-min(sample),max(sample)]);
        ymax = max([y ymax]);
        x = max([-min(ts),max(ts)]);
        xmax = max([x xmax]);
       plot(ts,sample,'Linewidth',2,'color',colors{abs(i-j)+1});
       if (globalScaleX)
           xmax = scaleglob(1); 
        end
        if (globalScaleY)
           ymax = scaleglob(2); 
        end
        axis([-xmax,xmax,-ymax,ymax]);
       set(gca,'Visible','off'); 
    end
     
end


%namefile = [outputFolder  'tab_spline_deg_' int2str(degreMax) '.png'];
%print -r2048 -RGBImage;
%saveas(f,[outputFolder  'tab_spline_deg_' int2str(degreMax) '.png']);
%print(f,[outputFolder  ,'tab_spline_deg_', int2str(degreMax), '.png'],'-dpng','-r1024'); 
%print(namefile,'-dpdf','-r300'); 
%print(f,namefile,'-painters','-dpdf', '-r300')