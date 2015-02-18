#include "stdafx.h"


#include "GUIProfile.h"

#include "ImageSetResourceManager.h"

namespace mray{
namespace GUI{

	

GUIProfile::GUIProfile(const core::string&name){
	m_name=name;
}
GUIProfile::~GUIProfile(){
	m_regions.clear();
}

const core::string&GUIProfile::getName(){
	return m_name;
}

void GUIProfile::addRegion(const GUIRegion& r){
	m_regions[r.id]=r;
}

GUIProfile::GUIRegion* GUIProfile::getRegion(int id)
{
	RegionMap::iterator it= m_regions.find(id);
	if(it==m_regions.end())
		return 0;
	return &it->second;
}/*
GUIProfile::GUIRegion* GUIProfile::getRegion(int id)
{

	RegionMap::iterator it= m_regions.begin();
	for (;it!=m_regions.end();++it)
	{
		if(it->second.id==id)
			return &it->second;
	}
	return 0;
}*/

int GUIProfile::getRegionCount(){
	return m_regions.size();
}


video::ImageSet* GUIProfile::GetImageSet()
{
	return m_imageSet.pointer();
}
const core::string& GUIProfile::GetImageSetName()
{
	return m_imageSetName;
}

void GUIProfile::SetImageSetName(const core::string& name)
{
	m_imageSetName=name;
	m_imageSet=gImageSetResourceManager.getOrCreate(name);
}

}
}

