function [posCenter,posPoint,newThetas] = Epicycloid(listCircle,thetas,posCenterDep,posPointDep)

if(length(listCircle) == 1)
    posCenter = posCenterDep;
    posPoint = posPointDep;
    newThetas = thetas;
    return;
end

r1 = listCircle(1).r;
isInside = listCircle(2).isInside;
r2 = listCircle(2).r;

coeff = 1.;

if(isInside)
   coeff = -1; 
end


normalVec = -normc(posPointDep(:,:,end)-posCenterDep(:,:,end));
normalVec1 = normalVec(2:-1:1,:);
normalVec1(1,:)=-normalVec1(1,:);

%posFinal = posCenterR2 + r2 * (repmat(cos(dTheta2),2,1).* normalVec + repmat(sin(dTheta2),2,1) .* normalVec1);

%%posCenterR2 = (r1+coeff*r2)*[cos(thetas);
 %              sin(thetas)];
           

posCenterR2 = posCenterDep(:,:,end) + (r1+coeff*r2) * (repmat(cos(thetas),2,1).* normalVec + repmat(sin(thetas),2,1) .* normalVec1);

normalVec = -normc(posCenterR2);
normalVec1 = normalVec(2:-1:1,:);
normalVec1(1,:)=-normalVec1(1,:);

dTheta2 = coeff*r1/r2 * thetas;

posFinal = posCenterR2 + r2 * (repmat(cos(dTheta2),2,1).* normalVec + repmat(sin(dTheta2),2,1) .* normalVec1);



[posCenterInter,posPointInter,thetaInter] = Epicycloid(listCircle(2:end),dTheta2,posCenterR2,posFinal);
posCenter = cat(3,posCenterDep,posCenterInter);
posPoint = cat(3,posPointDep,posPointInter);
newThetas = cat(3,thetas,thetaInter);

end

