close all;
clear all;

C1.r = 1.;
C1.isInside = false;
C1.color = 'b';

C2.r = 1/3;
C2.isInside = true;
C2.color = 'r';

C3.r =1/6;
C3.isInside = false;
C3.color = 'g';

C4.r =1/18;
C4.isInside = false;
C4.color = 'm';

C5.r = 1.;
C5.isInside = true;
C5.color = 'y';


listCircle = [C1,C2,C3,C4];

step = pi/1000;
thetas = 0:step:2*pi*200;
nPt = length(thetas);
posCenterDep = zeros(2,nPt);
posPointDep = [ones(1,nPt);zeros(1,nPt)];
[posCenter,posPoint] = Epicycloid(listCircle,thetas,posCenterDep,posPointDep);
figure;
nbCircle = length(listCircle);

for i=1:size(posCenter,3)
plot(posCenter(1,:,i),posCenter(2,:,i),listCircle(i).color);
hold on
plot(posPoint(1,:,i),posPoint(2,:,i),listCircle(i).color);
end

sample = 0:0.1:2*pi;
circle = [cos(sample);sin(sample)];

axis equal;

figure;
plot(posPoint(1,:,end),posPoint(2,:,end),listCircle(end).color);
axis equal;
set(gca,'xtick',[]);
set(gca,'ytick',[]);


figure;
for i=1:length(thetas)
    hold off;
    plot([0],[0]);
    hold on;
    for j=1:nbCircle
        posCircle = posCenter(:,i,j);
        currentCircle = repmat(posCircle,1,size(circle,2)) + listCircle(j).r*circle;
        plot(currentCircle(1,:),currentCircle(2,:),[listCircle(j).color '--']);
        plot(posPoint(1,:,j),posPoint(2,:,j),listCircle(j).color);
        plot(posPoint(1,i,j),posPoint(2,i,j),[listCircle(j).color '+']);
    
    end
    axis equal;
    set(gca,'xtick',[]);
    set(gca,'ytick',[]);
    pause;
    
    
end