function   continuousSubdiv(tris,pts,nb_subdiv,nbIm)

colorFace = zeros(size(tris,1),1);
f=figure,
for i=1:nb_subdiv
   
   [trisB,ptsB,colorFaceB] = multisubdivLoopUnicPt(tris,pts,colorFace,'middle',i-1);
   
   [newTrisM,newPtsM,newColorFaceM] = subdivLoop(trisB,ptsB,colorFaceB,'middle');
   [newTrisS,newPtsS,newColorFaceS] = subdivLoop(trisB,ptsB,colorFaceB,'simple');
   
   
   for k=0:nbIm
    alpha = k/nbIm;
    ptsmean = alpha * newPtsM+ (1.-alpha) * newPtsS ;
    hhloop=trisurf(newTrisM,ptsmean(:,1),ptsmean(:,2),ptsmean(:,3));
   
    % Additional bit to control color of each patch individually
    set(gca,'CLim',[0, 3*nb_subdiv]);
    set(hhloop,'FaceColor','flat',...
       'FaceVertexCData',newColorFaceM,...
       'CDataMapping','scaled'); 
    axis equal;
    set(gca,'Visible','off');
    saveas(f,['output/figure' int2str(k+(i-1)*nbIm)] ,'jpeg');

    %%pause 
   end
end


end

