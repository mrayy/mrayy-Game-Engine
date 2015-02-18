
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <mrayEngine.h>
#include <mrayWinApp.h>

#include "VegetationBuilder.h"
#include "GrassRenderNode.h"
#include "MeshSerializer.h"
#include "MeshBufferData.h"
#include "GUIBatchRenderer.h"
#include <ParticleEmitter.h>
#include <CPUParticleBillboardRenderer.h>
#include "PAVelocityInterpolater.h"
#include "PAScaleInterpolater.h"
#include "PAColorInterpolater.h"
#include "BoxParticleEmitter.h"
#include "ThreadJobManager.h"
#include "ParsedShaderPP.h"
#include "SBillboardChain.h"
#include "SphereParticleEmitter.h"
#include "GUIManager.h"
#include "IGUIPanelElement.h"
#include "LParser.h"
#include "MeshFileCreatorManager.h"
#include "MeshAnimatorManager.h"
#include <mrayPhysX.h>
#include "GameEntityManager.h"
#include "IPhysicUserContactReport.h"
#include "DebugDrawManager.h"
#include "PhysicContactPair.h"
#include "IPhysicContactStreamIterator.h"
#include "IPhysicSceneQueryReport.h"
#include "IPhysicSceneQuery.h"
#include "IterativeIK.h"
#include "BonesJointQuery.h"
#include "BoneNode.h"
#include "TJacobian.h"
#include "AnimationController.h"
#include "RTPickBuffer.h"
#include "ISystemProcess.h"
#include "IPhysicalClothDef.h"
#include "IPhysicalCloth.h"
#include "ISystemProcess.h"
#include "BenchmarkItem.h"
#include "XMLTree.h"
#include "XMLComment.h"
#include "XMLTextNode.h"
#include <Matcher.h>
//#include <IClassSerialize.h>
//#include <SerializationManager.h>
#include <ObjectSerializer.h>
#include <Pattern.h>
#include <GZipCompress.h>




#include "SGFunctionCall.h"
#include "SGOperation.h"
#include "SGVariable.h"
#include "SGMultiValue.h"
#include "SGDirectValue.h"

#include "SGProgram.h"
#include "SGProgramGeneratorCG.h"
#include "SGXMLVisitor.h"