


/********************************************************************
	created:	2012/02/09
	created:	9:2:2012   16:06
	filename: 	d:\Development\mrayEngine\Tools\MayaExporter\MayaMaterial.h
	file path:	d:\Development\mrayEngine\Tools\MayaExporter
	file base:	MayaMaterial
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __MayaMaterial__
#define __MayaMaterial__


namespace mray
{
namespace exporter
{
	class ExporterParams;

enum MaterialType{MT_SURFACE_SHADER,MT_LAMBERT,MT_PHONG,MT_BLINN,MT_CGFX} ;

enum TexOpType{TOT_REPLACE,TOT_MODULATE,TOT_ADD,TOT_ALPHABLEND} ;

enum TexAddressMode{TAM_CLAMP,TAM_BORDER,TAM_WRAP,TAM_MIRROR} ;

class MayaTexture
{
public:
	MayaTexture()
	{
		scale_u = scale_v = 1;
		scroll_u = scroll_v = 0;
		rot = 0;
		tam_u = tam_v = TAM_CLAMP;
	}
	virtual~MayaTexture(){}

	static core::string convertAddressMode(TexAddressMode a)
	{
		switch(a)
		{
		case TAM_CLAMP:
			return "Clamp";
		case TAM_BORDER:
			return "Border";
		case TAM_WRAP:
			return "Wrap";
		case TAM_MIRROR:
			return "Mirror";
		}
		return "";
	}

	MString fileName;
	MString absFileName;
	TexOpType opType;
	MString uvSetName;
	int uvSetIndex;
	TexAddressMode tam_u,tam_v;
	double scale_u,scale_v;
	double scroll_u,scroll_v;
	double rot;

	xml::XMLElement* saveXML(xml::XMLElement*parent);
};

class MayaMaterial
{
private:
public:

	MString m_name;
	MaterialType m_type;
	MColor m_diffuse,m_ambient,m_specular,m_emissive;
	bool m_lightingOff;
	bool m_isTransparent;
	bool m_isTextured;
	bool m_isMultiTextured;
	std::vector<MayaTexture> m_textures;

protected:

	MStatus loadSurfaceShader(MFnDependencyNode* pShader);
	MStatus loadLambert(MFnDependencyNode* pShader);
	MStatus loadPhong(MFnDependencyNode* pShader);
	MStatus loadBlinn(MFnDependencyNode* pShader);
	MStatus loadCgFxShader(MFnDependencyNode* pShader);
	//write material data to Ogre material script
	MStatus writeOgreScript(ExporterParams &params);
	//copy textures to path specified by params
	MStatus loadTexture(MFnDependencyNode* pTexNode,TexOpType& opType,MStringArray& uvsets,ExporterParams& params);

public:
	MayaMaterial();
	virtual~MayaMaterial();

	const MString& GetName()const{return m_name;}
	void clear();
	MStatus read(MFnDependencyNode*pShader,MStringArray& uvsets,ExporterParams& params);


	bool copyTextures(ExporterParams &params);
	xml::XMLElement* saveXML(xml::XMLElement*parent);
};

}
}

#endif