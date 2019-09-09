function [newTris,newPts,newColorFace] = multisubdivLoopUnicPt(tris,pts,colorFace,method,nbiter)
if (nbiter == 0)
    newTris = tris;
    newPts= pts;
    newColorFace = colorFace;
    return;
end

 [newTris,newPts,newColorFace] = subdivLoopUnicPt(tris,pts,colorFace,method);

for i=2:nbiter
   [newTris,newPts,newColorFace] = subdivLoopUnicPt(newTris,newPts,newColorFace,method);
end

end