close all;
clear all;
ctrlPt = zeros(1,30);
ctrlPt = [ ctrlPt, 1, ctrlPt ];
step = 0.001;

globalScaleX = 0;
globalScaleY = 0;
scaleglob = [ 0 0];

degreMax = 5;
f=figure;
outputFolder="D:\Pictures\spline\";
curveGrid=cell(degreMax+1,degreMax+1);
colors = {'b','c','g','m','r','y','k'};
width = 2;
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

%% convolution
for i=1:(degreMax+1)
   subplot(degreMax+2,degreMax+2,i*(degreMax+2)+2);
    

    
    %sample = curveGrid{i,i}.sample;
    sample = curveGrid{mod(i+1,degreMax+1)+1,mod(i+1,degreMax+1)+1}.sample;
    
    newCtrlPt = ctrlPt;
    for j=i+1:(degreMax+1)
        
        

         subplot(degreMax+2,degreMax+2,(i-1)*(degreMax+2)+j+1);

         sample2=curveGrid{j-1,i}.sample;
         %sample2=[sample2(2/step:end),sample2(1:(2/step-1))];
         convol=conv(sample,sample2);

        %curveSample(abs(curveSample) < 0.05)=0;    
        nonzeros= (curveSample ~= 0);
        t=1:length(convol);
        t = t - mean(t);
        %nonzeros = [1:size(curveSample,2)];
        %nonzeros = (nonzeros >= 2) .* (nonzeros <= 2);
%         m=mean(ts(nonzeros));
%         nonzeros=find(nonzeros == 1);
%         nonzeros=[max(min(nonzeros-1/step):min(nonzeros),1),nonzeros,min(max(nonzeros):max(nonzeros+1/step),size(ts,2))];
         plot(t,convol,'Linewidth',2,'color',colors{abs(j-i+1)});
%         alpha = max ([-min(curveSample(nonzeros)),max(curveSample(nonzeros))]);
%         scaleglob(2) = max([ scaleglob(2) alpha]);
%         scaleglob(1) = max([ scaleglob(1) max(ts(nonzeros)-m)]); 
%    %   axis([min(ts(nonzeros)-m),max(ts(nonzeros)-m),min(curveSample(nonzeros)),max(curveSample(nonzeros))]);
          alpha=max([max(convol),-min(convol)]);
          axis([min(t),max(t),-alpha,alpha]);
         set(gca,'Visible','off');

    end
   
end
         nbRow = 4;
         figure;   
         subplot(nbRow,3,1);   
         sample=curveGrid{1,1}.sample;
         Myplot(sample,colors{1},width);
         
         subplot(nbRow,3,2);
         sample2=curveGrid{2,2}.sample;
         Myplot(sample2,colors{1},width);

         subplot(nbRow,3,3);
         sample3=conv(sample,sample2);
          Myplot(sample3,colors{2},width);
          
          
           subplot(nbRow,3,4);   
         sample=curveGrid{2,2}.sample;
         Myplot(sample,colors{1},width);
         
         subplot(nbRow,3,5);
         sample2=-curveGrid{2,2}.sample;
         Myplot(sample2,colors{1},width);
         

         subplot(nbRow,3,6);
         sample3=conv(sample,sample2);
          Myplot(sample3,colors{2},width);
          
         subplot(nbRow,3,7);   
         sample=-curveGrid{1,1}.sample;
         Myplot(sample,colors{1},width);
         
         subplot(nbRow,3,8);
         sample2=curveGrid{2,1}.sample;
         Myplot(sample2,colors{2},width);

         subplot(nbRow,3,9);
         sample3=conv(sample,sample2);
          Myplot(sample3,colors{3},width);
          
          
         subplot(nbRow,3,10);   
         sample=curveGrid{2,2}.sample;
         Myplot(sample,colors{1},width);
         
         subplot(nbRow,3,11);
         sample2=curveGrid{3,2}.sample;
         Myplot(sample2,colors{2},width);

         subplot(nbRow,3,12);
         sample3=conv(sample,sample2);
          Myplot(sample3,colors{3},width);  
          
          
%% table de convolution
nb_side_i = 6;
nb_side_j = 6;
multiplication_tab = cell(nb_side_i+1,nb_side_j+1);

for i=1:(nb_side_j-2)
   curveGrid{i,i}.d = 1; 
   multiplication_tab{1,i+2}=curveGrid{i,i}; 
end
   multiplication_tab{1,end}=curveGrid{2,1};
   multiplication_tab{1,end}.d = 2;
   

   % derivee DIRAC
   v.sample = [zeros(1,100),1,-1,zeros(1,100)];
   v.d = -1;
   multiplication_tab{1,2}=v;
   v.sample = [0,1,-1,0];
   v.d=-1;
   multiplication_tab{2,1}=v;
   

   
   % DIRAC
   v.sample = [0,1,0];
   v.d=0;
   multiplication_tab{3,1}=v;
   
   
   % CRENEAU x derivee DIRAC
   v.sample = conv(curveGrid{1,1}.sample,[0,1,-1,0]);
   %v.sample = [0,0,1,0,0,-1,0,0];
   v.d = 0;
   multiplication_tab{4,1}=v;
   
   %CRENEAU
   multiplication_tab{5,1}=curveGrid{1,1};
   curveGrid{3,2}.d = 2;
   multiplication_tab{6,1}=curveGrid{3,2};
   curveGrid{5,3}.d = 3;
   multiplication_tab{7,1}=curveGrid{5,3};
   
   
   for i=1:nb_side_i
       I = multiplication_tab{i+1,1}; 
       for j=1:nb_side_j
            J = multiplication_tab{1,j+1};
            v.sample = conv(I.sample,J.sample);
            v.d = I.d + J.d;
            multiplication_tab{i+1,j+1}=v; 
            
       end
   end
   
   v.sample = [zeros(1,10000),1,zeros(1,10000)];
   v.d = 0;
   multiplication_tab{3,1}=v;
   
   
   v.sample = [zeros(1,100),1,-1,zeros(1,100)];
   v.d = -1;
   multiplication_tab{1,2}=v;
   multiplication_tab{2,1}=v;
   
   
   multiplication_tab{6,2} = multiplication_tab{1,4}; 
   
   colors = {[0.5,0.5,0.5],'k','b','c','g','m','r','y','k'};
   
   figure('Renderer', 'painters', 'Position', [10 10 1079 1079]);
   for i=1:(nb_side_i+1)
       for j=1:(nb_side_j+1)
           if(~isempty(multiplication_tab{i,j}))
               currentwidth = width;
               if(i==1) + (j==1)
                currentwidth = 3* width;
               else
                   currentwidth = 2*width;
               end
               
               
               subplot(nb_side_j+1,nb_side_i+1,(i-1)*(nb_side_i+1)+j);
               if(multiplication_tab{i,j}.d > -2)
               couleur = colors{min(max(1,multiplication_tab{i,j}.d+2),length(colors))};
                Myplot(multiplication_tab{i,j}.sample, couleur,currentwidth);
               end
               set(gca,'Visible','off');
           end  
          
       end
   end
   
   






%namefile = [outputFolder  'tab_spline_deg_' int2str(degreMax) '.png'];
%print -r2048 -RGBImage;
%saveas(f,[outputFolder  'tab_spline_deg_' int2str(degreMax) '.png']);
%print(f,[outputFolder  ,'tab_spline_deg_', int2str(degreMax), '.png'],'-dpng','-r1024'); 
%print(namefile,'-dpdf','-r300'); 
%print(f,namefile,'-painters','-dpdf', '-r300')


function Myplot(sample,coloration,width)
        t=1:length(sample);
        t = t - mean(t);        
        plot(t,sample,'Linewidth',width,'color',coloration);
        alpha=max([max(sample),-min(sample)]);
        axis([min(t),max(t),-alpha,alpha]);
        set(gca,'Visible','off');
end