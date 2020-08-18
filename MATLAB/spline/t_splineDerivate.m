ctrlPt = zeros(1,20);
ctrlPt = [ ctrlPt, 1, ctrlPt ];
step = 0.01;

degreMax = 5;
f=figure;
outputFolder="D:\Pictures\spline\";

for i=0:degreMax
   subplot(degreMax+1,degreMax+1,i*(degreMax+1)+1);
   
    [curveSample,ts] = sampleSpline(ctrlPt,i,step);

    %curveSample(abs(curveSample) < 0.05)=0;    
    nonzeros= (curveSample ~= 0);
    %nonzeros = [1:size(curveSample,2)];
    %nonzeros = (nonzeros >= 2) .* (nonzeros <= 2);
    m=mean(ts(nonzeros));
    nonzeros=find(nonzeros == 1);
    nonzeros=[max(min(nonzeros-1/step):min(nonzeros),1),nonzeros,min(max(nonzeros):max(nonzeros+1/step),size(ts,2))];
    plot(ts(nonzeros)-m,curveSample(nonzeros),'Linewidth',2,'Color','black');
    alpha = max ([-min(curveSample(nonzeros)),max(curveSample(nonzeros))]);
%   axis([min(ts(nonzeros)-m),max(ts(nonzeros)-m),min(curveSample(nonzeros)),max(curveSample(nonzeros))]);
    axis([min(ts(nonzeros)-m),max(ts(nonzeros)-m),-alpha,alpha]);
    set(gca,'Visible','off');
    
    
    newCtrlPt = ctrlPt;
    for j=1:i
        disp([int2str(i) '/' int2str(degreMax) ' : ' int2str(j)]);
        newCtrlPt = derivateSpline(newCtrlPt,i-j+1);

         subplot(degreMax+1,degreMax+1,i*(degreMax+1)+1+j);

         [curveSample,ts] = sampleSpline(newCtrlPt,i-j,step);

        %curveSample(abs(curveSample) < 0.05)=0;    
        nonzeros= (curveSample ~= 0);
        %nonzeros = [1:size(curveSample,2)];
        %nonzeros = (nonzeros >= 2) .* (nonzeros <= 2);
        m=mean(ts(nonzeros));
        nonzeros=find(nonzeros == 1);
        nonzeros=[max(min(nonzeros-1/step):min(nonzeros),1),nonzeros,min(max(nonzeros):max(nonzeros+1/step),size(ts,2))];
         plot(ts(nonzeros)-m,curveSample(nonzeros),'Linewidth',2,'Color','black');
        alpha = max ([-min(curveSample(nonzeros)),max(curveSample(nonzeros))]);
   %   axis([min(ts(nonzeros)-m),max(ts(nonzeros)-m),min(curveSample(nonzeros)),max(curveSample(nonzeros))]);
       axis([min(ts(nonzeros)-m),max(ts(nonzeros)-m),-alpha,alpha]);
        set(gca,'Visible','off');
    end
   
end
namefile = [outputFolder  'tab_spline_deg_' int2str(degreMax) '.png'];
%print -r2048
%saveas(f,[outputFolder  'tab_spline_deg_' int2str(degreMax) '.png']);
 print(f,[outputFolder  ,'tab_spline_deg_', int2str(degreMax), '.png'],'-dpng','-r1024'); 
%print(namefile,'-dpdf','-r300'); 
%print(f,namefile,'-painters','-dpdf', '-r300')