function newCtrlPt = derivateSpline(ctrlPt,degre)
knotVector = 0:(degre + size(ctrlPt,2));
newCtrlPt=degre*diff(ctrlPt);
a= unique(min((1:size(knotVector,2))+degre+1,size(knotVector,2)));
v=knotVector(a)-knotVector(a-degre);
newCtrlPt = newCtrlPt ./ v(1:size(newCtrlPt,2));
end

