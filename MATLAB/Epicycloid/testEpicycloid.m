close all;
clear all;

r1 = 9.0;
r2 = 3;
step = pi/50;
thetas = 0:step:2*pi*r1*r2;
isInside = true;

coeff = 1.;

if(isInside)
   coeff = -1; 
end


posCenterR2 = (r1+coeff*r2)*[cos(thetas);
               sin(thetas)];
           
normalVec = -normc(posCenterR2);
normalVec1 = normalVec(2:-1:1,:);
normalVec1(1,:)=-normalVec1(1,:);

circle1 = r1 * [cos(thetas);sin(thetas)];
circle2 = r2 * [cos(thetas);sin(thetas)];

%% r1 dTheta1 = r2 dTheta2
dTheta2 = coeff*r1/r2 * thetas;

posFinal = posCenterR2 + r2 * (repmat(cos(dTheta2),2,1).* normalVec + repmat(sin(dTheta2),2,1) .* normalVec1);



for i=1:length(thetas)
    plot(posFinal(1,:),posFinal(2,:));
    axis equal;
    hold on;
    plot(circle1(1,:), circle1(2,:),'r');
    circle2real = circle2 + posCenterR2(:,i);
    plot(circle2real(1,:), circle2real(2,:),'g');
    plot(posFinal(1,i),posFinal(2,i),'m+');
     plot(posFinal(1,1:i),posFinal(2,1:i),'m'); 
    hold off;
    pause
end