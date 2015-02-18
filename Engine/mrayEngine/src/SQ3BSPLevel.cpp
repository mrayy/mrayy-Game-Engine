

#include "SQ3BSPLevel.h"
#include <ColorConverter.h>
#include <SceneManager.h>
#include <LogManager.h>
#include <TextureResourceManager.h>
#include <IVideoDevice.h>
#include <Frustum.h>
#include <CameraNode.h>
#include <TraceManager.h>
#include <IFileSystem.h>




namespace mray{
namespace scene{

#define TOLERANCE 0.0003125f

#define FACE_POLYGON 1

#define TRACE_RAY		0
#define TRACE_SPHERE	1
#define TRACE_AABB		2

SQ3BSPLevel::SQ3BSPLevel():
	ISceneNode(mT(""),0),m_facesDrawn(0)
{
	m_traceRatio=0;
	m_traceType=0;
	m_traceRadius=0;
	m_bCollided=0;
	m_maxStepHeight=20.0f;

}
SQ3BSPLevel::~SQ3BSPLevel(){

	vertices.clear();
	indices.clear();
	faces.clear();
	texturesPath.clear();
	lightMaps.clear();

	m_pNodes.clear();
	m_pLeafs.clear();
	m_pPlanes.clear();
	m_pLeafFaces.clear();
	m_clusters.pBitsets.clear();

	m_pBrushes.clear();
	m_pBrushSides.clear();
	m_pLeafBrushes.clear();

	materials.clear();
	lightMapsTex.clear();

}

void SQ3BSPLevel::loadTextures(OS::IStreamPtr file){
	traceFunction(eScene);

	file->seek(lumps[kTextures].offset,OS::ESeek_Set);
	file->read(texturesPath.pointer(),lumps[kTextures].length);//m_numOfTextures*sizeof(sBSPTexture));

	core::string ext[2];
	ext[0]=mT(".jpg");
	ext[1]=mT(".tga");

	for(int i=0;i<m_numOfTextures;++i){
		materials[i]=new video::RenderMaterial();
		materials[i]->setRenderState(video::RS_Lighting,video::ES_DontUse);
		materials[i]->setRenderState(video::RS_Smooth,video::ES_DontUse);
		core::string name;
		core::char_to_string(texturesPath[i].strName,name);

		video::ITexturePtr tex;
		for(int e=0;e<2;e++){
			core::string strName=name;
			strName+=ext[e];
			if(gFileSystem.fileExist(strName)){
				tex=TextureResourceManager::getInstance().getOrCreate(strName);
				if(tex)
					break;
				else{
					int i=0;
				}
			}
		}
		/*
		if(tex){
			tex->setMipmapsFilter(1);
			tex->unlock();
			materials[i]->setTexture(tex,0);
			if(tex->getDepth()==32)//transparent
				materials[i]->setMaterialRenderer(video::MRT_TRANSPARENT);
		//	else 
		//		materials[i]->matRenderType=video::MRT_SOLID_2LAYERS;
		}else{
			///
		}*/
	}

}
void SQ3BSPLevel::loadNodes(OS::IStreamPtr file){
	traceFunction(eScene);
	m_pNodes.resize(m_numOfNodes);
	file->seek(lumps[kNodes].offset,OS::ESeek_Set);
	//	file->read(m_pNodes.pointer(),sizeof(sBSPNode)*m_numOfNodes);


	for(int i = 0; i < m_numOfNodes; i++)
	{
		sBSPNode temp;
		file->read(&temp,sizeof(sBSPNode));

		m_pNodes[i].back=temp.back;
		m_pNodes[i].front=temp.front;
		m_pNodes[i].plane=temp.plane;

		m_pNodes[i].bbox.MinP.x =  temp.min.x;
		m_pNodes[i].bbox.MinP.y =  temp.min.z;
		m_pNodes[i].bbox.MinP.z = -temp.min.y;

		m_pNodes[i].bbox.MaxP.x =  temp.max.x;
		m_pNodes[i].bbox.MaxP.y =  temp.max.z;
		m_pNodes[i].bbox.MaxP.z = -temp.max.y;
	}
}

void SQ3BSPLevel::loadLeafs(OS::IStreamPtr file){
	traceFunction(eScene);
	m_pLeafs.resize(m_numOfLeafs);
	file->seek(lumps[kLeafs].offset,OS::ESeek_Set);


	// Now we need to go through and convert all the leaf bounding boxes
	// to the normal OpenGL Y up axis.
	for(int i = 0; i < m_numOfLeafs; i++)
	{
		sBSPLeaf temp;
		file->read(&temp,sizeof(sBSPLeaf));

		m_pLeafs[i].area=temp.area;
		m_pLeafs[i].cluster=temp.cluster;
		m_pLeafs[i].leafBrush=temp.leafBrush;
		m_pLeafs[i].leafface=temp.leafface;
		m_pLeafs[i].numOfLeafBrushes=temp.numOfLeafBrushes;
		m_pLeafs[i].numOfLeafFaces=temp.numOfLeafFaces;

		m_pLeafs[i].bbox.MinP.x =  temp.min.x;
		m_pLeafs[i].bbox.MinP.y =  temp.min.z;
		m_pLeafs[i].bbox.MinP.z = -temp.min.y;

		m_pLeafs[i].bbox.MaxP.x =  temp.max.x;
		m_pLeafs[i].bbox.MaxP.y =  temp.max.z;
		m_pLeafs[i].bbox.MaxP.z = -temp.max.y;
	}


	m_pLeafFaces.resize(m_numOfLeafFaces);
	file->seek(lumps[kLeafFaces].offset, OS::ESeek_Set);
	file->read(m_pLeafFaces.pointer(), m_numOfLeafFaces* sizeof(int));

}
void SQ3BSPLevel::loadPlanes(OS::IStreamPtr file){
	traceFunction(eScene);

	m_pPlanes.resize(m_numOfPlanes);
	file->seek(lumps[kPlanes].offset, OS::ESeek_Set);

	for(int i=0;i<m_numOfPlanes;++i){
		sBSPPlane p;
		file->read(&p, sizeof(sBSPPlane));
		m_pPlanes[i].Normal.x=p.vNormal.x;
		m_pPlanes[i].Normal.y=p.vNormal.z;
		m_pPlanes[i].Normal.z=-p.vNormal.y;
		m_pPlanes[i].D=p.d;

	}
}
void SQ3BSPLevel::loadVisData(OS::IStreamPtr file){
	traceFunction(eScene);

	file->seek(lumps[kVisData].offset,OS::ESeek_Set);

	if(lumps[kVisData].length){
		file->read(&(m_clusters.numOfClusters),sizeof(int));
		file->read(&(m_clusters.bytesPerCluster),sizeof(int));

		int size=m_clusters.numOfClusters*m_clusters.bytesPerCluster;
		m_clusters.pBitsets.resize(size);
		file->read(m_clusters.pBitsets.pointer(),size*sizeof(byte));
	}

}
void SQ3BSPLevel::loadVertices(OS::IStreamPtr file){
	traceFunction(eScene);
	file->seek(lumps[kVertices].offset,OS::ESeek_Set);

	bool flag=0;
	for(int i=0;i<m_numOfVerts;++i){
		sBSPVertex vert;
		file->read(&vert,sizeof(sBSPVertex));
		vertices[i].Pos.x=vert.vPosition[0];
		vertices[i].Pos.y=vert.vPosition[2];
		vertices[i].Pos.z=-vert.vPosition[1];

		vertices[i].TCoord.x=vert.vTextureCoord[0];
		vertices[i].TCoord.y=vert.vTextureCoord[1];

		vertices[i].TCoord2.x=vert.vLightmapCoord[0];
		vertices[i].TCoord2.y=vert.vLightmapCoord[1];
		vertices[i].Normal.x=vert.vNormal[0];
		vertices[i].Normal.y=vert.vNormal[2];
		vertices[i].Normal.z=-vert.vNormal[1];
		vertices[i].color.setColor(vert.color);
		if(!flag){
			bbox.reset(vertices[i].Pos);
			flag=1;
		}
		else
			bbox.AddPoint(vertices[i].Pos);
	} 
}
void SQ3BSPLevel::loadShaders(OS::IStreamPtr file){
	traceFunction(eScene);
}
void SQ3BSPLevel::loadModels(OS::IStreamPtr file){
	traceFunction(eScene);
}
void SQ3BSPLevel::loadLightMaps(OS::IStreamPtr file){
	traceFunction(eScene);
	file->seek(lumps[kLightmaps].offset,OS::ESeek_Set);
	video::ImageInfo*info[1];
	info[0]=new video::ImageInfo();
	for(int i=0;i<m_numOfLightmaps;++i){
		file->read(&lightMaps[i],sizeof(sBSPLightmap));
		lightMapsTex[i]=new video::TextureUnit();
		lightMapsTex[i]->SetTexture(device->createTexture2D(1,video::EPixel_R8G8B8));
		info[0]->setData(lightMaps[i].imageBits,math::vector3d(128,128,1),video::EPixel_R8G8B8);

		video::ColorConverter::changeGamma(info[0],10);
		lightMapsTex[i]->GetTexture()->loadSurfaces(info,1);
	}
}
void SQ3BSPLevel::loadFaces(OS::IStreamPtr file){
	traceFunction(eScene);
	file->seek(lumps[kFaces].offset,OS::ESeek_Set);
	file->read(faces.pointer(),m_numOfFaces*sizeof(sBSPFace));
}
void SQ3BSPLevel::loadBrushes(OS::IStreamPtr file){
	traceFunction(eScene);

	m_numOfBrushes = lumps[kBrushes].length / sizeof(int);
	m_numOfBrushSides = lumps[kBrushSides].length / sizeof(int);
	m_numOfLeafBrushes = lumps[kLeafBrushes].length / sizeof(int);

	m_pBrushes.resize(m_numOfBrushes);
	m_pBrushSides.resize(m_numOfBrushSides);
	m_pLeafBrushes.resize(m_numOfLeafBrushes);

	file->seek(lumps[kBrushes].offset,OS::ESeek_Set);
	file->read(m_pBrushes.pointer(),sizeof(sBSPBrush)*m_numOfBrushes);

	file->seek(lumps[kBrushSides].offset,OS::ESeek_Set);
	file->read(m_pBrushSides.pointer(),sizeof(sBSPBrushSide)*m_numOfBrushSides);

	file->seek(lumps[kLeafBrushes].offset,OS::ESeek_Set);
	file->read(m_pLeafBrushes.pointer(),sizeof(int)*m_numOfLeafBrushes);

}
bool SQ3BSPLevel::loadLevel(OS::IStreamPtr file){

	traceFunction(eScene);

	file->read(&header,sizeof(header));

	if (header.strID != 0x50534249 || header.version != 0x2e)
	{
		gLogManager.log(mT("Failed loading .bsp file, unknown header."), file->getStreamName(),ELL_WARNING);
		return false;
	}


	file->read(&lumps[0],sizeof(sBSPLump)*kMaxLumps);

	m_numOfEntities =lumps[kEntities].length;

	m_numOfVerts	=lumps[kVertices].length/sizeof(sBSPVertex);
	m_numOfIndices	=lumps[kIndices].length/sizeof(int);
	m_numOfTextures	=lumps[kTextures].length/sizeof(sBSPTexture);
	m_numOfFaces	=lumps[kFaces].length/sizeof(sBSPFace);
	m_numOfLightmaps=lumps[kLightmaps].length/sizeof(sBSPLightmap);
	m_numOfModels	=lumps[kModels].length/sizeof(sBSPModel);
	m_numOfShaders	=lumps[kShaders].length/sizeof(sBSPShader);

	m_numOfNodes	=lumps[kNodes].length/sizeof(sBSPNode);
	m_numOfLeafs	=lumps[kLeafs].length/sizeof(sBSPLeaf);
	m_numOfLeafFaces=lumps[kLeafFaces].length / sizeof(int);
	m_numOfPlanes	=lumps[kPlanes].length / sizeof(sBSPPlane);

	vertices.resize(m_numOfVerts);
	indices.resize(m_numOfIndices);

	faces.resize(m_numOfFaces);

	texturesPath.resize(m_numOfTextures);
	materials.resize(m_numOfTextures);

	lightMaps.resize(m_numOfLightmaps);
	lightMapsTex.resize(m_numOfLightmaps);


	file->seek(lumps[kIndices].offset,OS::ESeek_Set);
	file->read(indices.pointer(),m_numOfIndices*sizeof(int));


	loadTextures(file);
	loadNodes(file);
	loadVisData(file);
	loadVertices(file);
	loadLeafs(file);
	loadPlanes(file);
	loadShaders(file);
	loadModels(file);
	loadLightMaps(file);
	loadFaces(file);
	loadBrushes(file);


	m_facesDrawn.resize(m_numOfFaces);
	return true;
}

void SQ3BSPLevel::RenderFace(int i){
	traceFunction(eScene);
	sBSPFace*pFace=&faces[i];
	if(pFace->textureID>=0){
		GCPtr<video::RenderMaterial>rend=materials[pFace->textureID];
		if(rend){
			int currStage=getSceneManager()->getRenderManager()->getCurrGroup();
			if(currStage==RGH_Solid && rend->getRenderState(video::RS_Blend))
				return;
			if(currStage==RGH_Transparent && !rend->getRenderState(video::RS_Blend))
				return;
		}
		device->useMaterial(materials[pFace->textureID]);
	}
	if(pFace->lightmapID>=0){
		device->useTexture(1,lightMapsTex[pFace->lightmapID]);
	}
// 	device->drawBuffer(&vertices[pFace->startVertIndex],&indices[pFace->startIndex],
// 		pFace->numOfIndices,MR_TRIANGLES,scene::EVCT_COORD2);
}


void SQ3BSPLevel::preRender(IRenderPass*pass){
	//sceneMngr->addNodeToRender(this,NRT_DETECT);
	if(!Visible)
		return;
	RenderOperation ROP;
	ROP.node=this;
	ROP.mesh=0;
	ROP.customRender=true;
	getSceneManager()->getRenderManager()->addOperation(RGH_Solid,ROP);
	getSceneManager()->getRenderManager()->addOperation(RGH_Transparent,ROP);
	ISceneNode::preRender(pass);
}

void SQ3BSPLevel::render(){
	traceFunction(eScene);

	CameraNode*cam=getSceneManager()->getActiveCamera();
	if(!cam)return;
	math::Frustum*f=cam->getViewFrustrum();
	if(!f)return;

	m_facesDrawn.clear();

	math::vector3d camP=cam->getAbsolutePosition();

	math::matrix4x4 mat;//=getAbsoluteTransformation();
	device->setTransformationState(video::TS_WORLD,mat);
//	camP=mat.inverseTransformVectPost(camP);

	math::box3d b=f->boundingBox;
	/*
	mat.inverseTransformVectPre(b.MinP);
	mat.inverseTransformVectPre(b.MaxP);
*/
	
	//  we now want to loop through	 all of the leafs.  
	// Each leaf stores a list of faces assign to it.
	// We call findLeaf() to find the current leaf index that our camera is
	// in.  This leaf will then give us the cluster that the camera is in.  The
	// cluster is then used to test visibility between our current cluster
	// and other leaf clusters.  If another leaf's cluster is visible from our 
	// current cluster, the leaf's bounding box is checked against our frustum.  
	// Assuming the bounding box is inside of our frustum, we draw all the faces
	// stored in that leaf.  

	// Grab the leaf index that our camera is in
	int leafIndex = findLeaf(camP);

	// Grab the cluster that is assigned to the leaf
	int cluster = m_pLeafs[leafIndex].cluster;

	// Initialize our counter variables (start at the last leaf and work down)
	int i = m_numOfLeafs;
	m_VisibleFaces = 0;

/*
	b.repair();
	device->drawBox(b,math::matrix4x4f(),MR_QUADS);
*/

	// Go through all the leafs and check their visibility
	while(i--)  
	{
		// Get the current leaf that is to be tested for visibility from our camera's leaf
		sBSPLeaf_new *pLeaf = &(m_pLeafs[i]);

		// If the current leaf can't be seen from our cluster, go to the next leaf
		if(!isClusterVisible(cluster, pLeaf->cluster)) 
			continue;

		// If the current leaf is not in the camera's frustum, go to the next leaf
	//	device->drawBox(pLeaf->bbox,math::matrix4x4f(),MR_LINES);

		if(!f->boxInFrustrum(pLeaf->bbox)){
		//	device->drawBox(pLeaf->bbox,mat,MR_LINES);
			continue;
		}
		
		// If we get here, the leaf we are testing must be visible in our camera's view.
		// Get the number of faces that this leaf is in charge of.
		int faceCount = pLeaf->numOfLeafFaces;

		// Loop through and render all of the faces in this leaf
		while(faceCount--)
		{
			// Grab the current face index from our leaf faces array
			int faceIndex = m_pLeafFaces[pLeaf->leafface + faceCount];

			// Before drawing this face, make sure it's a normal polygon
			if(faces[faceIndex].type != FACE_POLYGON) continue;

			// Since many faces are duplicated in other leafs, we need to
			// make sure this face already hasn't been drawn.
			if(!m_facesDrawn[faceIndex]) 
			{
				// Increase the rendered face count to display for fun
				m_VisibleFaces++;

				// Set this face as drawn and render it
				m_facesDrawn.set(faceIndex,true);
				RenderFace(faceIndex);
			}
		}			
	}

}

void SQ3BSPLevel::update(float dt){
	ISceneNode::update(dt);
}


int SQ3BSPLevel::findLeaf(const math::vector3d &vPos)
{
	traceFunction(eScene);
	int i = 0;
	float distance = 0.0f;

	// This function takes in our camera position, then goes and walks
	// through the BSP nodes, starting at the root node, finding the leaf node
	// that our camera resides in.  This is done by checking to see if
	// the camera is in front or back of each node's splitting plane.
	// If the camera is in front of the camera, then we want to check
	// the node in front of the node just tested.  If the camera is behind
	// the current node, we check that nodes back node.  Eventually, this
	// will find where the camera is according to the BSP tree.  Once a
	// node index (i) is found to be a negative number, that tells us that
	// that index is a leaf node, not another BSP node.  We can either calculate
	// the leaf node index from -(i + 1) or ~1.  This is because the starting
	// leaf index is 0, and you can't have a negative 0.  It's important
	// for us to know which leaf our camera is in so that we know which cluster
	// we are in.  That way we can test if other clusters are seen from our cluster.
	
	// Continue looping until we find a negative index
	while(i >= 0)
	{
		// Get the current node, then find the slitter plane from that
		// node's plane index.  Notice that we use a constant reference
		// to store the plane and node so we get some optimization.
		const sBSPNode_new&  node = m_pNodes[i];
		const math::Plane& plane = m_pPlanes[node.plane];

		// Use the Plane Equation (Ax + by + Cz + D = 0) to find if the
		// camera is in front of or behind the current splitter plane.
        distance =	plane.Normal.dotProduct(vPos)- plane.D;

		// If the camera is in front of the plane
        if(distance >= 0)	
		{
			// Assign the current node to the node in front of itself
            i = node.front;
        }
		// Else if the camera is behind the plane
        else		
		{
			// Assign the current node to the node behind itself
            i = node.back;
        }
    }

	// Return the leaf index (same thing as saying:  return -(i + 1)).
    return ~i;  // Binary operation
}

int SQ3BSPLevel::isClusterVisible(int current, int test)
{
	// This function is used to test the "current" cluster against
	// the "test" cluster.  If the "test" cluster is seen from the
	// "current" cluster, we can then draw it's associated faces, assuming
	// they aren't frustum culled of course.  Each cluster has their own
	// bitset containing a bit for each other cluster.  For instance, if there
	// is 10 clusters in the whole level (a tiny level), then each cluster
	// would have a bitset of 10 bits that store a 1 (visible) or a 0 (not visible) 
	// for each other cluster.  Bitsets are used because it's faster and saves
	// memory, instead of creating a huge array of booleans.  It seems that
	// people tend to call the bitsets "vectors", so keep that in mind too.

	// Make sure we have valid memory and that the current cluster is > 0.
	// If we don't have any memory or a negative cluster, return a visibility (1).
	if(!m_clusters.pBitsets.size() || current < 0) return 1;

	// Use binary math to get the 8 bit visibility set for the current cluster
	byte visSet = m_clusters.pBitsets[(current*m_clusters.bytesPerCluster) + (test / 8)];
	
	// Now that we have our vector (bitset), do some bit shifting to find if
	// the "test" cluster is visible from the "current" cluster, according to the bitset.
	int result = visSet & (1 << ((test) & 7));

	// Return the result ( either 1 (visible) or 0 (not visible) )
	return ( result );
}



math::vector3d SQ3BSPLevel::trace(math::vector3d start,math::vector3d end){
	traceFunction(eScene);
	m_traceRatio=1.0f;

// 	start=getAbsoluteTransformation().inverseTransformVectPost(start);
// 	end=getAbsoluteTransformation().inverseTransformVectPost(end);

	checkNode(0,0,1,start,end);

	if(m_traceRatio==1)
		return end;

	math::vector3d newPos= start+(end-start)*m_traceRatio;
	
	//slidding
	math::vector3d movement=end-newPos;
	
	float distance=m_vCollisionNormal.dotProduct(movement);
	
	math::vector3d endPos= end - m_vCollisionNormal * distance;

	newPos = trace(newPos,endPos);

	if(m_vCollisionNormal.y > 0.2 || m_bGrounded)
		m_bGrounded=true;
	else 
		m_bGrounded=false;

	return newPos;

	
}

math::vector3d SQ3BSPLevel::traceRay(math::vector3d start,math::vector3d end){
	traceFunction(eScene);
	m_traceType=TRACE_RAY;

	return trace(start,end);
	
}

math::vector3d SQ3BSPLevel::traceSphere(math::vector3d start,math::vector3d end,float radius){
	traceFunction(eScene);
	m_traceType=TRACE_SPHERE;
	m_bCollided=false;
	m_bGrounded=false;
	m_tryToStep=false;
	m_traceRadius=radius;

	math::vector3d newPos= trace(start,end);

	if(m_bCollided && m_tryToStep)
		newPos=tryToStep(newPos,end);

	return newPos;
	
}
math::vector3d SQ3BSPLevel::traceBox(math::vector3d start,math::vector3d end,math::box3d aabb){
	traceFunction(eScene);
	m_traceType=TRACE_AABB;
	m_bCollided=false;
	m_bGrounded=false;
	m_tryToStep=false;
	m_vTraceBox = aabb;
	m_vExtents  =aabb.getExtent();


	math::vector3d newPos= trace(start,end);

	if(m_bCollided && m_tryToStep)
		newPos=tryToStep(newPos,end);

	return newPos;
}
math::vector3d SQ3BSPLevel::traceBox(math::vector3d start,math::vector3d end,math::vector3d extent){


	return traceBox(start,end,math::box3d(-extent,extent));
	
}

void SQ3BSPLevel::checkNode(int nodeIndex, float startRatio, float endRatio, math::vector3d start, math::vector3d end){

	traceFunction(eScene);
	// the nodeIndices are stored as negative numbers when we get to a leaf, so we 
	// check if the current node is a leaf, which holds brushes.  If the nodeIndex is negative,
	// the next index is a leaf (note the: nodeIndex + 1)
	if(nodeIndex < 0)
	{
		// If this node in the BSP is a leaf, we need to negate and add 1 to offset
		// the real node index into the m_pLeafs[] array.  You could also do [~nodeIndex].
		sBSPLeaf_new *pLeaf = &m_pLeafs[-(nodeIndex + 1)];

		// We have a leaf, so let's go through all of the brushes for that leaf
		for(int i = 0; i < pLeaf->numOfLeafBrushes; i++)
		{
			// Get the current brush that we going to check
			sBSPBrush *pBrush = &m_pBrushes[m_pLeafBrushes[pLeaf->leafBrush + i]];

			// This is kind of an important line.  First, we check if there is actually
			// and brush sides (which store indices to the normal and plane data for the brush).
			// If not, then go to the next one.  Otherwise, we also check to see if the brush
			// is something that we want to collide with.  For instance, there are brushes for
			// water, lava, bushes, misc. sprites, etc...  We don't want to collide with water
			// and other things like bushes, so we check the texture type to see if it's a solid.
			// If the textureType can be binary-anded (&) and still be 1, then it's solid,
			// otherwise it's something that can be walked through.  That's how Quake chose to
			// do it.

			// Check if we have brush sides and the current brush is solid and collidable
			if((pBrush->numOfBrushSides > 0) && (texturesPath[pBrush->textureID].textureType & 1))
			{
				// Now we delve into the dark depths of the real calculations for collision.
				// We can now check the movement vector against our brush planes.
				checkBrush(pBrush, start, end);
			}
		}

		// Since we found the brushes, we can go back up and stop recursing at this level
		return;
	}

	// If we haven't found a leaf in the node, then we need to keep doing some dirty work
	// until we find the leafs which store the brush information for collision detection.

	// Grad the next node to work with and grab this node's plane data
	sBSPNode_new *pNode = &m_pNodes[nodeIndex];
	math::Plane *pPlane = &m_pPlanes[pNode->plane];
	
	// Now we do some quick tests to see which side we fall on of the node in the BSP

	// Here we use the plane equation to find out where our initial start position is
	// according the the node that we are checking.  We then grab the same info for the end pos.
	float startDistance = pPlane->Normal.dotProduct(start) - pPlane->D;
	float endDistance =  pPlane->Normal.dotProduct(end) - pPlane->D;
	float offset = 0.0f;

	// If we are doing any type of collision detection besides a ray, we need to change
	// the offset for which we are testing collision against the brushes.  If we are testing
	// a sphere against the brushes, we need to add the sphere's offset when we do the plane
	// equation for testing our movement vector (start and end position).  * More Info * For
	// more info on sphere collision, check out our tutorials on this subject.

	// If we are doing sphere collision, include an offset for our collision tests below
	if(m_traceType == TRACE_SPHERE)
		offset = m_traceRadius;
	// Here we check to see if we are working with a BOX or not
	else if(m_traceType == TRACE_AABB)
	{
		// This equation does a dot product to see how far our
		// AABB is away from the current plane we are checking.
		// Since this is a distance, we need to make it an absolute
		// value, which calls for the fabs() function (abs() for floats).
		
		// Get the distance our AABB is from the current splitter plane
		offset = (float)(fabs( m_vExtents.x * pPlane->Normal.x ) +
                         fabs( m_vExtents.y * pPlane->Normal.y ) +
                         fabs( m_vExtents.z * pPlane->Normal.z ) );
	}
	// Below we just do a basic traversal down the BSP tree.  If the points are in
	// front of the current splitter plane, then only check the nodes in front of
	// that splitter plane.  Otherwise, if both are behind, check the nodes that are
	// behind the current splitter plane.  The next case is that the movement vector
	// is on both sides of the splitter plane, which makes it a bit more tricky because we now
	// need to check both the front and the back and split up the movement vector for both sides.

	// Here we check to see if the start and end point are both in front of the current node.
	// If so, we want to check all of the nodes in front of this current splitter plane.
	if(startDistance >= offset && endDistance >= offset)
	{
		// Traverse the BSP tree on all the nodes in front of this current splitter plane
		checkNode(pNode->front, startDistance, endDistance, start, end);
	}
	// If both points are behind the current splitter plane, traverse down the back nodes
	else if(startDistance < -offset && endDistance < -offset)
	{
		// Traverse the BSP tree on all the nodes in back of this current splitter plane
		checkNode(pNode->back, startDistance, endDistance, start, end);
	}	
	else
	{
		// If we get here, then our ray needs to be split in half to check the nodes
		// on both sides of the current splitter plane.  Thus we create 2 ratios.
		float Ratio1 = 1.0f, Ratio2 = 0.0f, middleRatio = 0.0f;
		math::vector3d vMiddle;	// This stores the middle point for our split ray

		// Start of the side as the front side to check
		int side = pNode->front;

		// Here we check to see if the start point is in back of the plane (negative)
		if(startDistance < endDistance)
		{
			// Since the start position is in back, let's check the back nodes
			side = pNode->back;

			// Here we create 2 ratios that hold a distance from the start to the
			// extent closest to the start (take into account a sphere and TOLERANCE).
			// We use TOLERANCE like Quake does to compensate for float errors.  The second
			// ratio holds a distance from the other size of the extents on the other side
			// of the plane.  This essential splits the ray for both sides of the splitter plane.
			float inverseDistance = 1.0f / (startDistance - endDistance);
			Ratio1 = (startDistance - offset - TOLERANCE) * inverseDistance;
			Ratio2 = (startDistance + offset + TOLERANCE) * inverseDistance;
		}
		// Check if the starting point is greater than the end point (positive)
		else if(startDistance > endDistance)
		{
			// This means that we are going to recurse down the front nodes first.
			// We do the same thing as above and get 2 ratios for split ray.
			// Ratio 1 and 2 are switched in contrast to the last if statement.
			// This is because the start is starting in the front of the splitter plane.
			float inverseDistance = 1.0f / (startDistance - endDistance);
			Ratio1 = (startDistance + offset + TOLERANCE) * inverseDistance;
			Ratio2 = (startDistance - offset - TOLERANCE) * inverseDistance;
		}

		// Make sure that we have valid numbers and not some weird float problems.
		// This ensures that we have a value from 0 to 1 as a good ratio should be :)
		if (Ratio1 < 0.0f) Ratio1 = 0.0f;
        else if (Ratio1 > 1.0f) Ratio1 = 1.0f;

        if (Ratio2 < 0.0f) Ratio2 = 0.0f;
        else if (Ratio2 > 1.0f) Ratio2 = 1.0f;

		// Just like we do in the Trace() function, we find the desired middle
		// point on the ray, but instead of a point we get a middleRatio percentage.
		// This isn't the true middle point since we are using offset's and the TOLERANCE value.
		// We also grab the middle point to go with the ratio.
		middleRatio = startRatio + ((endRatio - startRatio) * Ratio1);
		vMiddle = start + ((end - start) * Ratio1);

		// Now we recurse on the current side with only the first half of the ray
		checkNode(side, startRatio, middleRatio, start, vMiddle);

		// Now we need to make a middle point and ratio for the other side of the node
		middleRatio = startRatio + ((endRatio - startRatio) * Ratio2);
		vMiddle = start + ((end - start) * Ratio2);

		// Depending on which side should go last, traverse the bsp with the
		// other side of the split ray (movement vector).
		if(side == pNode->back)
			checkNode(pNode->front, middleRatio, endRatio, vMiddle, end);
		else
			checkNode(pNode->back, middleRatio, endRatio, vMiddle, end);
	}
}

void SQ3BSPLevel::checkBrush(sBSPBrush *pBrush, math::vector3d start, math::vector3d end){

	traceFunction(eScene);
	float startRatio = -1.0f;		// Like in BrushCollision.htm, start a ratio at -1
    float endRatio = 1.0f;			// Set the end ratio to 1
    bool startsOut = false;			// This tells us if we starting outside the brush

	// This function actually does the collision detection between our movement
	// vector and the brushes in the world data.  We will go through all of the
	// brush sides and check our start and end ratio against the planes to see if
	// they pass each other.  We start the startRatio at -1 and the endRatio at
	// 1, but as we set the ratios to their intersection points (ratios), then
	// they slowly move toward each other.  If they pass each other, then there
	// is definitely not a collision.

	// Go through all of the brush sides and check collision against each plane
	for(int i = 0; i < pBrush->numOfBrushSides; i++)
	{
		// Here we grab the current brush side and plane in this brush
		sBSPBrushSide *pBrushSide = &m_pBrushSides[pBrush->brushSide + i];
		math::Plane *pPlane = &m_pPlanes[pBrushSide->plane];

		// Let's store a variable for the offset (like for sphere collision)
		float offset = 0.0f;

		// If we are testing sphere collision we need to add the sphere radius
		if(m_traceType == TRACE_SPHERE)
			offset = m_traceRadius;

		// Test the start and end points against the current plane of the brush side.
		// Notice that we add an offset to the distance from the origin, which makes
		// our sphere collision work.
		float startDistance	 =  pPlane->Normal.dotProduct(start) - (pPlane->D + offset);
		float endDistance	 = pPlane->Normal.dotProduct(end) - (pPlane->D + offset);

		//   In this section we need to do a few special checks to see which extents
		// we should use.  We do this by checking the x,y,z of the normal.
		// If the vNormal.x is less than zero, we want to use the Max.x
		// value, otherwise use the Min.x value.  We do these checks because
		// we want the corner of the bounding box that is closest to the plane to
		// test for collision.  Write it down on paper and see how this works.
		// We want to grab the closest corner (x, y, or z value that is...) so we
		// dont go through the wall.  This works because the bounding box is axis aligned.

		// Store the offset that we will check against the plane
		math::vector3d vOffset;

		// If we are using AABB collision
		if(m_traceType == TRACE_AABB)
		{
			// Grab the closest corner (x, y, or z value) that is closest to the plane
			vOffset.x = (pPlane->Normal.x < 0)	? m_vTraceBox.MaxP.x : m_vTraceBox.MinP.x;
			vOffset.y = (pPlane->Normal.y < 0)	? m_vTraceBox.MaxP.y : m_vTraceBox.MinP.y;
			vOffset.z = (pPlane->Normal.z < 0)	? m_vTraceBox.MaxP.z : m_vTraceBox.MinP.z;
            
			// Use the plane equation to grab the distance our start position is from the plane.
			// We need to add the offset to this to see if the box collides with the plane,
			// even if the position doesn't.
            startDistance = pPlane->Normal.dotProduct(start + vOffset) - pPlane->D;

			// Get the distance our end position is from this current brush plane
            endDistance   = pPlane->Normal.dotProduct(end + vOffset) - pPlane->D;
        }

		// Make sure we start outside of the brush's volume
		if(startDistance > 0)	startsOut = true;

		// Stop checking since both the start and end position are in front of the plane
		if(startDistance > 0 && endDistance > 0)
			return;

		// Continue on to the next brush side if both points are behind or on the plane
		if(startDistance <= 0 && endDistance <= 0)
			continue;

		// If the distance of the start point is greater than the end point, we have a collision!
		if(startDistance > endDistance)
		{
			// This gets a ratio from our starting point to the approximate collision spot
			float Ratio1 = (startDistance - TOLERANCE) / (startDistance - endDistance);

			// If this is the first time coming here, then this will always be true,
			// since startRatio starts at -1.0f.  We want to find the closest collision,
			// so we still continue to check all of the brushes before quitting.
			if(Ratio1 > startRatio)
			{
				// Set the startRatio (currently the closest collision distance from start)
				startRatio = Ratio1;
				m_bCollided = true;		// Let us know we collided!
				
				m_vCollisionNormal = pPlane->Normal;

				if((start.x!=end.x || start.z!=end.z) && pPlane->Normal.y!=1)
					m_tryToStep = true;

				if(m_vCollisionNormal.y>=0.2)
					m_bGrounded = true;

			}

		}
		else
		{
			// Get the ratio of the current brush side for the endRatio
			float Ratio = (startDistance + TOLERANCE) / (startDistance - endDistance);

			// If the ratio is less than the current endRatio, assign a new endRatio.
			// This will usually always be true when starting out.
			if(Ratio < endRatio)
				endRatio = Ratio;
		}
	}

	// If we didn't start outside of the brush we don't want to count this collision - return;
	if(startsOut == false)
	{
		return;
	}
	
	// If our startRatio is less than the endRatio there was a collision!!!
	if(startRatio < endRatio)
	{
		// Make sure the startRatio moved from the start and check if the collision
		// ratio we just got is less than the current ratio stored in m_traceRatio.
		// We want the closest collision to our original starting position.
		if(startRatio > -1 && startRatio < m_traceRatio)
		{
			// If the startRatio is less than 0, just set it to 0
			if(startRatio < 0)
				startRatio = 0;

			// Store the new ratio in our member variable for later
			m_traceRatio = startRatio;
		}
	}
}


math::vector3d SQ3BSPLevel::tryToStep(math::vector3d start,math::vector3d end){
	traceFunction(eScene);

	for(float h=1;h<=m_maxStepHeight;h+=1){
		m_tryToStep=m_bCollided=false;

		math::vector3d stepStart(start.x,start.y+h,start.z);
		math::vector3d stepEnd(end.x,start.y+h,end.z);

		math::vector3d stepPos=trace(stepStart,stepEnd);
		if(!m_bCollided)
		{

			// Return the current position since we stepped up somewhere
			return stepPos;		
		}

	}
	return start;


}

}
}



