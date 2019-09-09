close all;
clear all;

addpath(genpath(pwd));
inputFolder='input/';
obj = readObj([inputFolder 'icosphere.obj']);

tris = obj.f.v;
pts = obj.v;
colorFace = zeros(size(tris,1),1);

 hhloop=trisurf(tris,pts(:,1),pts(:,2),pts(:,3));
 axis equal;
 
 continuousSubdiv(tris,pts,3,30);
 
 