function [newTris,newPts,newColorFace] = subdivLoop(tris,pts,colorFace,method)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

nbTri=size(tris,1);

newTris=[];
newPts=[];
newColorFace=[];




idcolorFace = max(colorFace)+1;

for i=1:nbTri
    idPt =size(newPts,1);
   for id = 1:3
       
       %% pts normaux
       idv = tris(i,id);
       idtri = mod(find(tris==idv)-1,nbTri)+1;
       inter = tris(idtri,:);
       neighbour=unique(inter(inter~=idv));
       nbNeighbour = length(neighbour);
       if (method == "loop")
            if (nbNeighbour > 3)
                beta = 4/(8*nbNeighbour);
            else
                beta = 4/16;
            end
        else
            beta = 0.0;
        end
       newPt = (1 - nbNeighbour*beta)*pts(idv,:)+beta*sum(pts(neighbour,:));
       
       newPts = [newPts;newPt];

       
       %% pts milieux
       idnext = mod(id,3)+1;
       
       
       
       idvnext=tris(i,idnext);
       idTriNext = mod(find(tris==idvnext)-1,nbTri)+1;
       idtri=intersect(idTriNext,idtri);
       inter = tris(idtri,:);
       inter=inter(inter~=idv);
       neighbour=unique(inter(inter~=idvnext));
       
       if (method == "loop")
           newPt = 3/8*(pts(idv,:)+pts(idvnext,:))+1/8*sum(pts(neighbour,:));
           
       else
           if (method == "middle")
               newPt = 1/2*(pts(idv,:)+pts(idvnext,:));
           else
                newPt = pts(idv,:);
           end
       end
       
       newPts = [newPts;newPt];
 
       
   end
   
   
              
   tri2add = idPt+[2,3,4;
                   4,5,6;
                   6,1,2;
                   2,4,6];

               
   newTris = [newTris;tri2add];
   newColorFace = [ newColorFace;
                    idcolorFace;
                    idcolorFace+1;
                    idcolorFace+2;
                    colorFace(i)];
                    
                    
      
  
   %figure,
   
   %trimesh(newTris,newPts(:,1),newPts(:,2),newPts(:,3));
   %disp(i);
                   
   
   
   
end



end

