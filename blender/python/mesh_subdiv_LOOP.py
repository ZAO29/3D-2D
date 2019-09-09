# This example assumes we have a mesh object selected

# EXECUTION
# exec(open("D:\\Repos\\2D-3D\\blender\\python\\mesh_subdiv_LOOP.py").read())


import bpy
import bmesh
import math
import os
import mathutils





#############################################
def findIdVert(vert, vertBuffer):
    vertBuffer.ensure_lookup_table()
    for i in range(0,len(vertBuffer)):
        v = vertBuffer[i];
        if v.co == vert.co :
            return i

    return -1

########################################
def computeNewPt(vert, b):
    listVoisin = []
    for e in vert.link_edges:
            listVoisin.append(e.other_vert(vert))
    

    
    newco = mathutils.Vector()
    nb_voisin = len(listVoisin)
    
    if nb_voisin < 3 :
        b = 4/16
    else:
        b = 4/(8*nb_voisin)
    
    #b = 0.1
        
    newco += (1.-nb_voisin*b)*vert.co
    
    for voisin in listVoisin :
        newco += b*voisin.co
    
    return newco



def computeNewEdgePt(vertx,verty):
    listVoisin = []
    for e in vertx.link_edges:
            if(e.other_vert(vertx) == verty):
                selectedEdge = e
            

    newco = mathutils.Vector()
    

    if len(selectedEdge.link_faces) < 2:       
        newco += 1/2*(vertx.co+verty.co)
        return newco
    
    for v in selectedEdge.link_faces[0].verts:
        if (v != vertx and v != verty):
            vxyN = v     
        
    for v in selectedEdge.link_faces[1].verts:
        if (v != vertx and v != verty):
            vxyS = v
    

    newco += 3/8*(vertx.co+verty.co)
    newco += 1/8*(vxyN.co+vxyS.co) 
    
    return newco




# Get the active mesh
me = bpy.context.object.data

bpy.ops.object.duplicate(linked=0,mode='TRANSLATION')

translateMat = mathutils.Matrix.Translation((2.,0.,0.))
me.transform(translateMat)

# Get a BMesh representation
bm = bmesh.new()   # create an empty BMesh
  # create an empty BMesh
bm.from_mesh(me)   # fill it in from a Mesh

bmInit = bmesh.new()
#init pyramid


#
initPyramid = True;

if(initPyramid):
    vZ  = bmInit.verts.new((0.,0.,1.));
    v1  = bmInit.verts.new((1.,0.,0.));
    v2  = bmInit.verts.new((math.cos(2.*math.pi/3.),math.sin(2.*math.pi/3.),0.));
    v3  = bmInit.verts.new((math.cos(-2.*math.pi/3.),math.sin(-2.*math.pi/3.),0.));

    f1 = bmInit.faces.new((v1,v2,v3));
    f2 = bmInit.faces.new((v1,vZ,v2));
    f3 = bmInit.faces.new((v2,vZ,v3));
    f4 = bmInit.faces.new((v3,vZ,v1));
    # Modify the BMesh, can do anything here...

    f1.material_index = 0;
    f2.material_index = 0;   
    f3.material_index = 0;   
    f4.material_index = 0;      
else:
    bmInit.from_mesh(me)

n_subdiv = 0
beta = 0.0


for i in range(0,n_subdiv):
    
    bmNew = bmesh.new() 
    
    newVertList = []
    r = range(len(bmInit.verts))
    
    AdjMat = [[-1 for x in r] for y in r]
    for f in bmInit.faces:
        v1 = f.verts[0]
        v2 = f.verts[1]
        v3 = f.verts[2]
        
        
        
       
        
        
            
        id1 = findIdVert(v1, bmInit.verts)
        id2 = findIdVert(v2, bmInit.verts)
        id3 = findIdVert(v3, bmInit.verts)


           
        idPresent= [0 for x in range(4)]   
              
        idPresent[1] = (AdjMat[id1][id1] != -1);  
        idPresent[2] = (AdjMat[id2][id2] != -1);
        idPresent[3] = (AdjMat[id3][id3] != -1);
        
        
        ###### UPDATE FORMER POINT #########                
        if idPresent[1] :      
            v1c  = AdjMat[id1][id1];
            
        else:
            v1NewCo = computeNewPt(v1,beta);
            v1c  = bmNew.verts.new(v1NewCo);
            AdjMat[id1][id1] = v1c
            
        if idPresent[2]:       
            v2c  = AdjMat[id2][id2];
        else:
            v2NewCo = computeNewPt(v2,beta);
            v2c  = bmNew.verts.new(v2NewCo);
            AdjMat[id2][id2] = v2c
                       
        if idPresent[3]:        
            v3c  = AdjMat[id3][id3];
        else:
            v3NewCo = computeNewPt(v3,beta);
            v3c  = bmNew.verts.new(v3NewCo);
            AdjMat[id3][id3] = v3c
          
          
          
          
        #### NOUVEAU POINT ARETE ######  
        idPresent12 = (AdjMat[id1][id2] != -1);  
        idPresent23 = (AdjMat[id2][id3] != -1);
        idPresent31 = (AdjMat[id3][id1] != -1);
        
        
          
        if idPresent12 :
            v12c = AdjMat[id1][id2]
        else: 
            #v12co = v1.co.lerp(v2.co,0.5);
            v12co = computeNewEdgePt(v1,v2)
            v12c = bmNew.verts.new(v12co);
            AdjMat[id1][id2] = v12c
            AdjMat[id2][id1] = v12c
            
        if idPresent23 :
            v23c = AdjMat[id2][id3]
        else: 
            #v23co = v2.co.lerp(v3.co,0.5);
            v23co = computeNewEdgePt(v2,v3)
            v23c = bmNew.verts.new(v23co);
            AdjMat[id2][id3] = v23c
            AdjMat[id3][id2] = v23c
            
        if idPresent31 :
            v31c = AdjMat[id3][id1]
        else: 
            #v31co = v3.co.lerp(v1.co,0.5);
            v31co=computeNewEdgePt(v3,v1)
            v31c = bmNew.verts.new(v31co);
            AdjMat[id3][id1] = v31c
            AdjMat[id1][id3] = v31c  
        
        
        
        
        
        

             

        

        
        
      
        
        
        
        nf1 = bmNew.faces.new((v1c,v12c,v31c));
        nf2 = bmNew.faces.new((v2c,v23c,v12c));
        nf3 = bmNew.faces.new((v3c,v31c,v23c));
        nf4 = bmNew.faces.new((v31c,v12c,v23c));
        
        
        UseCenterColor = False;
        if (UseCenterColor) :
            nf1.material_index = f.material_index;
            nf2.material_index = f.material_index;
            nf3.material_index = f.material_index;
            nf4.material_index = i+1; 
        else :
            nf1.material_index = i*3+1;
            nf2.material_index = i*3+2;
            nf3.material_index = i*3+3;
            nf4.material_index = f.material_index;
        
    bmInit = bmNew;
    
    


# Finish up, write the bmesh back to the mesh
if (n_subdiv  == 0):
    bmInit.to_mesh(me)
else :
    bmNew.to_mesh(me)


#adding materials

nbMaterial = 10

for i in range(nbMaterial):
    mat = bpy.data.materials.new("mat"+str(i));
    byteRandom = os.urandom(3)
    #mat.diffuse_color = (i/nbMaterial, 1-i/nbMaterial, i/nbMaterial)
    color=[0,0,0];
    #color[i % 3] = byteRandom[0]/255
    #color[int(i/2) % 3] = byteRandom[1]/255
    color[i%3]=1
    color[2*i%3]=1
    color[int(i/2)%3]=1
    
    mat.diffuse_color = color
    me.materials.append(mat)


bm.free()
if (n_subdiv  != 0):
    bmNew.free()  # free and prevent further access
