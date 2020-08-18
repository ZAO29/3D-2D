% https://team.inria.fr/imagine/files/2015/02/tp3_bspline.pdf
%  DeBoor-Cox algorithm

function sample = EvalSplineCurve(ctrlPt,knotVector,n,t)

% 
% if (size(knotVector,2) ~= (size(ctrlPt,2)-1))
%     error("invalid knot vector or ctrlPt");
% end

interval =  (t <=  knotVector);
idInterval = find(diff(interval)~= 0);
  
% case t == 0
if (sum(interval) == length(knotVector))
 idInterval = 1;
end



if(n == 0)
sample = ctrlPt(:,idInterval);
return;
end

sample=dnj(n,knotVector,ctrlPt,idInterval,n,t);

end


function v=dnj(n,knotVector,ctrlPt,i,r,t)

if (r == 0)
    if((i < 1) + (i > size(ctrlPt,2)))
        v = zeros(size(ctrlPt,1),1);
        return;
    end
    v = ctrlPt(:,i);
    return;
end


ti = knotVector(i);
tikr = knotVector(i+n+1-r);
difft = tikr-ti;
v =  (t - ti) * dnj(n,knotVector,ctrlPt,i,r-1,t);
v = v + (tikr -t)* dnj(n,knotVector,ctrlPt,i-1,r-1,t);
v = v/difft;


end