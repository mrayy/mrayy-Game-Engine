

/********************************************************************
	created:	2012/02/09
	created:	9:2:2012   18:47
	filename: 	d:\Development\mrayEngine\Tools\MayaExporter\MayaNode.h
	file path:	d:\Development\mrayEngine\Tools\MayaExporter
	file base:	MayaNode
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __MayaNode__
#define __MayaNode__

#include "MayaAnimationTrack.h"

namespace mray
{
namespace animation
{
	class TransAnimationGroup;
}
namespace exporter
{
	class ExporterParams;
	class AnimationTrackDesc;

	class IComponentData;

class MayaNode
{
private:
protected:

	std::vector<MayaNode*> m_children;

	MayaNode* m_parent;
	MString m_name;

	core::string m_componentType;
	IComponentData* m_compData;

	math::vector3d m_position;
	math::vector3d m_rotation;
	math::vector3d m_scale;

	MMatrix m_bindMatrix;

	struct AnimationClip
	{
		~AnimationClip()
		{
			delete pos;
			delete rot;
			delete scale;
		}
		core::string name;
		int keysCount;
		MayaAnimationTrack* pos;
		MayaAnimationTrack* rot;
		MayaAnimationTrack* scale;
	};

	std::vector<AnimationClip*> m_clips;

	MFnAnimCurve* readAnimCurve(MPlug& plug,ExporterParams& params);
	AnimationClip* loadAnimationClip(MDagPath& dPath,AnimationTrackDesc& clip);
	void GenerateTracks(MayaAnimationTrack*t, 
		std::vector<MayaAnimationTrack*>& tracks,ExporterParams& params);

	virtual void _OnLoadComponent(const MDagPath& dpath,ExporterParams& params);
public:
	MayaNode():m_scale(1),m_parent(0),m_compData(0)
	{}
	virtual~MayaNode();

	void AddChild(MayaNode* n)
	{
		m_children.push_back(n);
		n->m_parent=this;
	}

	const MString& GetName(){return m_name;}
	const std::vector<MayaNode*>& GetChildren(){return m_children;}


	const math::vector3d& GetPosition(){return m_position;}
	const math::vector3d& GetRotation(){return m_rotation;}
	const math::vector3d& GetScale(){return m_scale;}

	MStatus read(MFnTransform& dPath,ExporterParams& params);

	animation::TransAnimationGroup* ConvertTrack(ExporterParams& params);
	virtual xml::XMLElement* saveXML(xml::XMLElement* parent,ExporterParams& params);

	virtual xml::XMLElement* saveXMLAsComponent(xml::XMLElement* parent,ExporterParams& params);
};


}
}

#endif

