function [fractalShape,levels] = hyperbolicTiling(circle,shape,level,forbiddenSide)


if(level == 0)
    fractalShape=shape;
    levels = 0;
else
    nbVertex = size(shape,2)-1;
    
    invertedShape=circleInversion(circle,shape);
    fractalShape=shape;
    levels=level;
    for i=1:nbVertex
        if (i ~= forbiddenSide)
            p1 = invertedShape(:,i);
            p2 = shape(:,i);
            p3 = shape(:,i+1);
        
            newcircle = circonscritCircle(p1,p2,p3);
            invShape = circleInversion(newcircle,shape);
            
            [fractalShapetoConcat,levelstoConcat]=hyperbolicTiling(circle,invShape,level-1,i);
            fractalShape=cat(3,fractalShape,fractalShapetoConcat);
            levels = [levels,levelstoConcat];
        end
            
        
        
    end
    
end

