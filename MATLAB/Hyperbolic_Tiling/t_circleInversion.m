clear all
close all;

%% generate a regular sampled polygon
nbVertex = 4;
nb_sample = 50;
angles = 0:2*pi/nbVertex:2*pi;
vertices = [cos(angles);sin(angles)];
steps = diff(vertices,1,2)/(nb_sample-1);

steps2 = repmat(steps,[1,1,nb_sample]);
coeffs2 = repmat(reshape(0:(nb_sample-1),1,1,nb_sample),[2,nbVertex,1]);
vertices2 = repmat(vertices(:,1:(end-1)),1,1,nb_sample);
shape=steps2.*coeffs2+vertices2;
shape = permute(shape,[1,3,2]);
shape = reshape(shape,[2,nbVertex*nb_sample]);

circle.center = [0.3;0.1];
circle.radius = 2;
anglesCircle = 0:2*pi/100:2*pi;
sampledCircle = circle.radius*[cos(anglesCircle);sin(anglesCircle)]+repmat(circle.center,[1,length(anglesCircle)]);

invShape = circleInversion(circle,shape);

hold on
plot(shape(1,:),shape(2,:));

plot(invShape(1,:),invShape(2,:),'r');

plot(sampledCircle(1,:),sampledCircle(2,:),'c');

axis equal