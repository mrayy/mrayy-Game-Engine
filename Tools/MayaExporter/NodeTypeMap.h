

/********************************************************************
	created:	2012/09/01
	created:	1:9:2012   15:34
	filename: 	d:\Development\mrayEngine\Tools\MayaExporter\NodeTypeMap.h
	file path:	d:\Development\mrayEngine\Tools\MayaExporter
	file base:	NodeTypeMap
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___NodeTypeMap___
#define ___NodeTypeMap___



namespace mray
{
	namespace exporter
	{

		class MayaNode;
		class IComponentData;

		class NodeTypeMap
		{
		protected:

			static std::map<core::string,core::string> s_map;

			NodeTypeMap();

			static NodeTypeMap s_instance;
		public:

			static const core::string& GetMappedType(const core::string& t);
			static IComponentData* CreateComponent(const core::string& t,MayaNode* owner);

		};

	}
}

#endif
