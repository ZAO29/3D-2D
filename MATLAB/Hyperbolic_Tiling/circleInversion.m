%% http://www.malinc.se/noneuclidean/en/circleinversion.php

%equation :  0A.0A' = radius^2
function invertedShape = circleInversion(circle,shape)

centers = repmat(circle.center,[1,size(shape,2)]);
OA = shape - centers;

lambdas = circle.radius^2./(vecnorm(OA).^2);

invertedShape = repmat(lambdas,[2,1]).*OA+centers;
end

