# This example assumes we have a mesh object selected

# EXECUTION
# exec(open("D:\\Repos\\2D-3D\\blender\\python\\mesh_subdiv_LOOP.py").read())


import bpy
import bmesh
import math
import os
import mathutils








############################# Class BBOX ###############
class BBox:
        
    def __init__(self,co_min,co_max):
        self._box = [mathutils.Vector((0,0,0))]*2;
        self._box[0] = co_min.copy();
        self._box[1] = co_max.copy();
        
    def Union(self,co):
        for i in range(3):
            if(self._box[0][i] > co[i]):
                self._box[0][i] = co[i];
            if(self._box[1][i] < co[i]):
                self._box[1][i] = co[i];
                
    def RectangleSize(self):
        return (self._box[1] - self._box[0]);
    
    def Squarified(self):
        rect = self.RectangleSize();
        M = max(rect);
        middle = self.GetMiddle();
        for i in range(3):
            self._box[0][i] = middle[i] - M/2;
            self._box[1][i] = middle[i] + M/2;       
        
    def Compute(self,mesh):
        self._box[0] = mesh.vertices[0].co.copy();
        self._box[1] = mesh.vertices[0].co.copy();
        
        for v in mesh.vertices:
            self.Union(v.co);
                
    def Print(self):
        print("********");
        print("min  : "+str(self._box[0]))
        print("max  : "+str(self._box[1]))   
        print("********");
    
    
    def IsInside(self,co):
        for i in range(3):
            if(self._box[0][i] > co[i]):
                return False;
            if(self._box[1][i] < co[i]):
               return False;
        return True;
        
    def Copy(self):
        copy = BBox(self._box[0],self._box[1]);
        return copy; 
    
    def SetVal(self,id_minMax,id_vec,val):
        self._box[id_minMax][id_vec] = val;
        
    def GetMiddle(self):
        return  (self._box[0] + self._box[1])/2.;
       
    def Scale(self,scale):
        middle = self.GetMiddle();
        for i in range(2):
            self._box[i] = middle + scale*(self._box[i]-middle);
    
     
        
    def AddToMesh(self, bmesh):
        for dir in range(3):
            for side in range(2):
                v0 = bmesh.verts.new((0,0,0));
                v1 = bmesh.verts.new((0,0,0));
                v2 = bmesh.verts.new((0,0,0));
                v3 = bmesh.verts.new((0,0,0));
                
                v0.co = self._box[side].copy();
                v1.co = v0.co.copy();
                v1.co[(dir+1)%3] = self._box[(side+1)%2][(dir+1)%3];
                v2.co = v1.co.copy();
                v2.co[(dir+2)%3] = self._box[(side+1)%2][(dir+2)%3];
                v3.co = v2.co.copy();
                v3.co[(dir+1)%3] = self._box[(side)%2][(dir+1)%3];
    
                bmesh.faces.new((v0,v1,v2,v3));
             
    def Subdivide(self):
        listSplitBox = [];
        middle = self.GetMiddle();
        listPt = [mathutils.Vector((0,0,0))]*3;
        listPt[0] = self._box[0].copy();
        listPt[1] = middle.copy();
        listPt[2] = self._box[1].copy();
        A = BBox(listPt[0],listPt[1]);
        listSplitBox.append(A);
        for id_vec in range(3):
            B = A.Copy();
            for imM in range(2):
                B.SetVal(imM,id_vec,listPt[imM+1][id_vec]);
                listSplitBox.append(B);
        
        A = BBox(listPt[1],listPt[2]);
        listSplitBox.append(A);
        for id_vec in range(3):
            B = A.Copy();
            for imM in range(2):
                B.SetVal(imM,id_vec,listPt[imM][id_vec]);
                listSplitBox.append(B);
        
        return listSplitBox;              
            





######################### CLASS StoppingCriteria ###################### 
class StoppingCriteria:
    
    def __init__(self,nbMinVertBox,nbMaxLevelTree):
        self._mVertBox = nbMinVertBox;
        self._MLevelTree = nbMaxLevelTree;
        
    def mustStop(self,idVertices,verts,level):
        crit = (len(idVertices) < self._mVertBox)
        crit = crit | (level > self._MLevelTree);
        return crit;    
        
        
        
########################### OctTree algorithm ########################
def OctTree(bbox, idVertices, verts, stopCriteria,level):
    if stopCriteria.mustStop(idVertices,verts,level) :
        if len(idVertices) > 0 :
            return [bbox];
        else:
            return [];
    
    listBox = bbox.Subdivide();
    listBBoxTree=[];
    
    for bb in listBox :
        idVInsideBox=[-1]*len(idVertices);
        idCurrent = 0;
        for idV in idVertices:
            if bb.IsInside(verts[idV].co):
                #idVInsideBox.append(idV);
                idVInsideBox[idCurrent]=idV;
                idCurrent = idCurrent + 1;
                
        del idVInsideBox[idCurrent:len(idVInsideBox)]
                 
        listBBoxTreeExplored = OctTree(bb,idVInsideBox,verts, stopCriteria,level+1); 
        listBBoxTree = listBBoxTreeExplored + listBBoxTree;
        
    
    return listBBoxTree;
        




# Get the active mesh
me = bpy.context.selected_objects[0].data

bpy.ops.object.duplicate(linked=0,mode='TRANSLATION')


bbox = BBox(mathutils.Vector((0.,0.,0.)),mathutils.Vector((0.,0.,0.)));
bbox.Compute(me);
bbox.Print()
d = (bbox._box[1].x-bbox._box[0].x);
print ("d " + str(d));
translateMat = mathutils.Matrix.Translation((2.*d,0.,0.))
me.transform(translateMat)


bbox.Compute(me)
bbox.Squarified()    
bbox.Print()
   
   
bm = bmesh.new();

###### test AddToMesh
# bbox.AddToMesh(bm);


####### test BBox.Subdivide
#listSplitBox = bbox.Subdivide();

#for bb in listSplitBox :
#    bb.Scale(0.5);
#    bb.AddToMesh(bm)
    


######## test OctTree
stopCrit = StoppingCriteria(25,7);
idVert = [*range(0,len(me.vertices))];
listBBox = OctTree(bbox,idVert,me.vertices,stopCrit,0);

for bb in listBBox :
    bb.Scale(1.0);
    bb.AddToMesh(bm)



bm.to_mesh(me)

bm.free();

 
        
        
 