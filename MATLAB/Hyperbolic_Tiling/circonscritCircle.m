function circle = circonscritCircle(p1,p2,p3)

normal1 = p2 - p1;
middle1 = (p1+p2)/2;
eq1=[normal1;middle1'*normal1];

normal2 = p3 - p2;
middle2 = (p2 + p3)/2;
eq2=[normal2;middle2'*normal2];


if (eq2(1) ~= 0)
    lambda = eq1(1)/eq2(1);
    y = (eq1(3)-eq2(3)*lambda)/(eq1(2)-eq2(2)*lambda);
else
    y = eq2(3)/eq2(2);
end


if (eq2(2) ~= 0)
    lambda = eq1(2)/eq2(2);
    x = (eq1(3)-eq2(3)*lambda)/(eq1(1)-eq2(1)*lambda);
else
    x = eq2(3)/eq2(1);
end

circle.center = [x;y];
circle.radius = norm (p1 - circle.center);


end

