function [curveSample,ts] = sampleSpline(ctrlPt,degre,step)

knotVector = 0:(degre + size(ctrlPt,2));

ts= degre:step:(knotVector(size(knotVector,2)) - degre -step);

curveSample = zeros(1,length(ts));
i=1;
for t=ts
v = EvalSplineCurve(ctrlPt,knotVector,degre,t);
if(prod(size(v)) == 0)
    keyboard;
end
curveSample(i)=v;
i=i+1;
end
end

