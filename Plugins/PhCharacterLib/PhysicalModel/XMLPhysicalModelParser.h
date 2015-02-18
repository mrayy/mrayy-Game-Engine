

/********************************************************************
	created:	2009/10/16
	created:	16:10:2009   15:46
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject\XMLPhysicalModelParser.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestProject
	file base:	XMLPhysicalModelParser
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___XMLPhysicalModelParser___
#define ___XMLPhysicalModelParser___


namespace mray{


namespace PhysicsBasedCharacter
{
	class SegmentDesc;

	class XMLPhysicalModelParser
	{
	private:
	protected:
	public:
		XMLPhysicalModelParser();
		virtual~XMLPhysicalModelParser();

		bool LoadPhysicalModelFile(const core::string &path,SegmentDesc&desc);
		bool LoadCollisionModelFile(const core::string &path,SegmentDesc&desc);
	};
}
}


#endif //___XMLPhysicalModelParser___
