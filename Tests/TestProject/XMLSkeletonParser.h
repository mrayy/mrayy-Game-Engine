

/********************************************************************
	created:	2009/10/16
	created:	16:10:2009   15:46
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject\XMLSkeletonParser.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject
	file base:	XMLSkeletonParser
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___XMLSkeletonParser___
#define ___XMLSkeletonParser___


namespace mray{

	class SegmentDesc;

class XMLSkeletonParser
{
private:
protected:
public:
	XMLSkeletonParser();
	virtual~XMLSkeletonParser();

	bool LoadFile(const core::string &path,SegmentDesc&desc);
};

}


#endif //___XMLSkeletonParser___
