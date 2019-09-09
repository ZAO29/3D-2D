function [newTris,newPts,newColorFace] = subdivLoopUnicPt(tris,pts,colorFace,method)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
if (size(tris,1) ~= length(colorFace))
    disp('invalid input mesh color');
end

if (max(tris(:)) < size(pts,1)) || min(tris(:) < 1)
    disp('invalid input mesh');
end


nbTri=size(tris,1);
nbPt = size(pts,1);
newTris=[];
newPts=[];
newColorFace=[];




idcolorFace = max(colorFace)+1;
PtAlreadyProcess = zeros(nbPt,nbPt);
for i=1:nbTri
    idCurrentPt = zeros(2,3);
   for id = 1:3
       
       %% pts normaux
       idv = tris(i,id);
       idAlreadyPro = PtAlreadyProcess(idv,idv);
       if(idAlreadyPro == 0)
           idtri = mod(find(tris==idv)-1,nbTri)+1;
           inter = tris(idtri,:);
           neighbour=unique(inter(inter~=idv));
           nbNeighbour = length(neighbour);
           
           if (idv < 0 || (sum(neighbour(:) < 0)) > 0)
               disp('yolo');
           end
           try   
               
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
           
           catch ME
               disp('oho');
           end
           newPts = [newPts;newPt];
     
           idAlreadyPro = size(newPts,1);
           PtAlreadyProcess(idv,idv)=idAlreadyPro;
       end
       
       idCurrentPt(1,id)=idAlreadyPro;
       if(idAlreadyPro == 0)
           keyboard
       end
       
       %% pts milieux
       idnext = mod(id,3)+1;
       idvnext=tris(i,idnext);
       idAlreadyPro = PtAlreadyProcess(idv,idvnext);
       if(idAlreadyPro == 0)
       
          
           idTriNext = mod(find(tris==idvnext)-1,nbTri)+1;
           idtri=intersect(idTriNext,idtri);
           inter = tris(idtri,:);
           inter=inter(inter~=idv);
           neighbour=unique(inter(inter~=idvnext));
           
           if(length(neighbour) > 2)
               disp('yolo ya trop de tri');
           end

           if (method == "loop")
               newPt = 3/8*(pts(idv,:)+pts(idvnext,:))+1/8*sum(pts(neighbour,:));
           else
               if (method == 'middle')
                   newPt = 1/2*(pts(idv,:)+pts(idvnext,:));
               else
                    newPt = pts(idv,:);
               end
           end
       
           newPts = [newPts;newPt];
           idAlreadyPro = size(newPts,1);
           PtAlreadyProcess(idv,idvnext)=idAlreadyPro;
           PtAlreadyProcess(idvnext,idv)=idAlreadyPro;
           
       end
       
       idCurrentPt(2,id)=idAlreadyPro;
       if(idAlreadyPro == 0)
           keyboard
       end

       
   end
   
   
              
%    tri2add = idPt+[2,3,4;
%                    4,5,6;
%                    6,1,2;
%                    2,4,6];
    
   if sum((reshape(idCurrentPt,1,6) == 0)) ~= 0
       keyboard
   end
   tri2add =      [idCurrentPt(2,1),idCurrentPt(1,2),idCurrentPt(2,2);
                   idCurrentPt(2,2),idCurrentPt(1,3),idCurrentPt(2,3);
                   idCurrentPt(2,3),idCurrentPt(1,1),idCurrentPt(2,1);
                   idCurrentPt(2,1),idCurrentPt(2,2),idCurrentPt(2,3)];


               
   newTris = [newTris;tri2add];
   
   if (i >  length(colorFace))
      disp('yolo'); 
   end
       
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

