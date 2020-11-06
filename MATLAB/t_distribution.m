clear all;
close all;

space = [1 999999];
nbPt = 100000;
n=10;
l = randi(space,1,nbPt);
for i=1:n
 subplot(1,n,i); 
  firstNumber = floor(l./10.^floor(log10(l)));
  histogram(firstNumber,'Normalization','probability');
  axis([0 10 0 0.5]);
 l1 = randi(space,1,nbPt);
 l = l.* l1;
 

end

l2 = randi(space,1,nbPt);
l3 = randi(space,1,nbPt);
l4 = randi(space,1,nbPt);

area = l1 .* l2 .* l3 .* l4;

