clear all
close all;

%% generate a regular sampled polygon
nbVertex = 5;
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

circle.center = [0.0;0.0];
circle.radius = 2;
anglesCircle = 0:2*pi/100:2*pi;
sampledCircle = circle.radius*[cos(anglesCircle);sin(anglesCircle)]+repmat(circle.center,[1,length(anglesCircle)]);

invShape = circleInversion(circle,shape);

hold on
plot(shape(1,:),shape(2,:));

plot(invShape(1,:),invShape(2,:),'r');

plot(sampledCircle(1,:),sampledCircle(2,:),'c');

axis equal

%%
% p2 = [0;4];
% p1 = [2;5];
% p3 = [5;0];
% 
% circle = circonscritCircle(p1,p2,p3);
% anglesCircle = 0:2*pi/100:2*pi;
% sampledCircle = circle.radius*[cos(anglesCircle);sin(anglesCircle)]+repmat(circle.center,[1,length(anglesCircle)]);
% pts=[p1,p2,p3];
% figure;
% hold on;
% plot(pts(1,:),pts(2,:));
% plot(sampledCircle(1,:),sampledCircle(2,:),'c');


%%
level = 2;
forbiddenSide = 0; 
[fractalShape,levels] = hyperbolicTiling(circle,vertices,level,forbiddenSide);

nbShape = size(fractalShape,3);

figure;
color = {'k','b','c','g','m','r'};
linewidth = [2,1,0.5,0.25,0.125,0.0625];
scale = 0.8;
for i=1:nbShape
  hold on
  level = levels(i)+1;
  shape = fractalShape(:,:,i);
  mean_pt = mean(shape,2);
  mean_ptRep = repmat(mean_pt,[1,nbVertex+1]);
  shape = (shape - mean_ptRep)*scale+mean_ptRep;
  plot(shape(1,:),shape(2,:),color{level}); 
end





