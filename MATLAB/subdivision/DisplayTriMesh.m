function hh = DisplayTriMesh(tris,pts,colorFace)
   hh=trisurf(tris,pts(:,1),pts(:,2),pts(:,3));
   
 % Additional bit to control color of each patch individually
set(gca,'CLim',[min(colorFace), max(colorFace)]);
set(hh,'FaceColor','flat',...
       'FaceVertexCData',colorFace,...
       'CDataMapping','scaled'); 
   axis equal;
end

