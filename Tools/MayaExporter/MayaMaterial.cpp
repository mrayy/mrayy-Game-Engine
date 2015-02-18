

#include "stdafx.h"
#include "MayaMaterial.h"
#include "ExporterParams.h"
#include "RenderMaterial.h"
#include "RenderTechnique.h"
#include "Renderpass.h"


namespace mray
{
namespace exporter
{

MayaMaterial::MayaMaterial()
{
	m_name = "";
	m_type = MT_LAMBERT;
	m_lightingOff = false;
	m_isTransparent = false;
	m_isTextured = false;
	m_isMultiTextured = false;
	m_ambient = MColor(0,0,0,0);
	m_diffuse = MColor(0,0,0,0);
	m_specular = MColor(0,0,0,0);
	m_emissive = MColor(0,0,0,0);
	m_textures.clear();
}

MayaMaterial::~MayaMaterial()
{
	clear();
}

void MayaMaterial::clear()
{
}
MStatus MayaMaterial::read(MFnDependencyNode*pShader,MStringArray& uvsets,ExporterParams& params)
{
	MStatus stat;
	clear();

	//read material name, adding the requested prefix
	MString tmpStr = params.matPrefix;
	if (tmpStr != "")
		tmpStr += "/";
	tmpStr += pShader->name();
	m_name=ExporterParams::fixName(tmpStr);
	/*
	MStringArray tmpStrArray;
	tmpStr.split(':',tmpStrArray);
	m_name = "";
	for (int i=0; i<tmpStrArray.length(); i++)
	{
		m_name += tmpStrArray[i];
		if (i < tmpStrArray.length()-1)
			m_name += "_";
	}*/

	//check if we want to export with lighting off option
	m_lightingOff = params.lightingOff;

	// GET MATERIAL DATA

	// Check material type
	if (pShader->object().hasFn(MFn::kPhong))
	{
		stat = loadPhong(pShader);
	}
	else if (pShader->object().hasFn(MFn::kBlinn))
	{
		stat = loadBlinn(pShader);
	}
	else if (pShader->object().hasFn(MFn::kLambert))
	{
		stat = loadLambert(pShader);
	}
	else if (pShader->object().hasFn(MFn::kPluginHwShaderNode))
	{
		stat = loadCgFxShader(pShader);
	}
	else
	{
		stat = loadSurfaceShader(pShader);
	}

	// Get textures data
	MPlugArray colorSrcPlugs;
	MPlugArray texSrcPlugs;
	MPlugArray placetexSrcPlugs;
	if (m_isTextured)
	{
		// Translate multiple textures if material is multitextured
		if (m_isMultiTextured)
		{
			// Get layered texture node
			MFnDependencyNode* pLayeredTexNode = NULL;
			if (m_type == MT_SURFACE_SHADER)
				pShader->findPlug("outColor").connectedTo(colorSrcPlugs,true,false);
			else
				pShader->findPlug("color").connectedTo(colorSrcPlugs,true,false);
			for (int i=0; i<colorSrcPlugs.length(); i++)
			{
				if (colorSrcPlugs[i].node().hasFn(MFn::kLayeredTexture))
				{
					pLayeredTexNode = new MFnDependencyNode(colorSrcPlugs[i].node());
					continue;
				}
			}

			// Get inputs to layered texture
			MPlug inputsPlug = pLayeredTexNode->findPlug("inputs");

			// Scan inputs and export textures
			for (int i=inputsPlug.numElements()-1; i>=0; i--)
			{
				MFnDependencyNode* pTextureNode = NULL;
				// Search for a connected texture
				inputsPlug[i].child(0).connectedTo(colorSrcPlugs,true,false);
				for (int j=0; j<colorSrcPlugs.length(); j++)
				{
					if (colorSrcPlugs[j].node().hasFn(MFn::kFileTexture))
					{
						pTextureNode = new MFnDependencyNode(colorSrcPlugs[j].node());
						continue;
					}
				}

				// Translate the texture if it was found
				if (pTextureNode)
				{
					// Get blend mode
					TexOpType opType;
					short bm;
					inputsPlug[i].child(2).getValue(bm);
					switch(bm)
					{				
					case 0:
						opType = TOT_REPLACE;
						break;
					case 1:
						opType = TOT_ALPHABLEND;
						break;				
					case 4:
						opType = TOT_ADD;
						break;
					case 6:
						opType = TOT_MODULATE;
						break;
					default:
						opType = TOT_MODULATE;
					}

					stat = loadTexture(pTextureNode,opType,uvsets,params);
					delete pTextureNode;
					if (MS::kSuccess != stat)
					{
						std::cout << "Error loading layered texture\n";
						std::cout.flush();
						delete pLayeredTexNode;
						return MS::kFailure;
					}
				}
			}
			if (pLayeredTexNode)
				delete pLayeredTexNode;
		}
		// Else translate the single texture
		else
		{
			// Get texture node
			MFnDependencyNode* pTextureNode = NULL;
			if (m_type == MT_SURFACE_SHADER)
				pShader->findPlug("outColor").connectedTo(colorSrcPlugs,true,false);
			else
				pShader->findPlug("color").connectedTo(colorSrcPlugs,true,false);
			for (int i=0; i<colorSrcPlugs.length(); i++)
			{
				if (colorSrcPlugs[i].node().hasFn(MFn::kFileTexture))
				{
					pTextureNode = new MFnDependencyNode(colorSrcPlugs[i].node());
					continue;
				}
			}
			if (pTextureNode)
			{
				TexOpType opType = TOT_MODULATE;
				stat = loadTexture(pTextureNode,opType,uvsets,params);
				delete pTextureNode;
				if (MS::kSuccess != stat)
				{
					std::cout << "Error loading texture\n";
					std::cout.flush();
					return MS::kFailure;
				}
			}
		}
	}

	return MS::kSuccess;

}



// Load a surface shader
MStatus MayaMaterial::loadSurfaceShader(MFnDependencyNode *pShader)
{
	m_type = MT_SURFACE_SHADER;
	MPlugArray colorSrcPlugs;
	// Check if material is textured
	pShader->findPlug("outColor").connectedTo(colorSrcPlugs,true,false);
	for (int i=0; i<colorSrcPlugs.length(); i++)
	{
		if (colorSrcPlugs[i].node().hasFn(MFn::kFileTexture))
		{
			m_isTextured = true;
			continue;
		}
		else if (colorSrcPlugs[i].node().hasFn(MFn::kLayeredTexture))
		{
			m_isTextured = true;
			m_isMultiTextured = true;
			continue;
		}
	}

	// Check if material is transparent
	float trasp;
	pShader->findPlug("outTransparencyR").getValue(trasp);
	if (pShader->findPlug("outTransparency").isConnected() || trasp>0.0f)
		m_isTransparent = true;

	// Get material colours
	if (m_isTextured)
		m_diffuse = MColor(1.0,1.0,1.0,1.0);
	else
	{
		pShader->findPlug("outColorR").getValue(m_diffuse.r);
		pShader->findPlug("outColorG").getValue(m_diffuse.g);
		pShader->findPlug("outColorB").getValue(m_diffuse.b);
		float trasp;
		pShader->findPlug("outTransparencyR").getValue(trasp);
		m_diffuse.a = 1.0 - trasp;
	}
	m_ambient = MColor(0,0,0,1);
	m_emissive = MColor(0,0,0,1);
	m_specular = MColor(0,0,0,1);
	return MS::kSuccess;
}

// Load a lambert shader
MStatus MayaMaterial::loadLambert(MFnDependencyNode *pShader)
{
	MPlugArray colorSrcPlugs;
	m_type = MT_LAMBERT;
	MFnLambertShader* pLambert = new MFnLambertShader(pShader->object());
	// Check if material is textured
	pLambert->findPlug("color").connectedTo(colorSrcPlugs,true,false);
	for (int i=0; i<colorSrcPlugs.length(); i++)
	{
		if (colorSrcPlugs[i].node().hasFn(MFn::kFileTexture))
		{
			m_isTextured = true;
			continue;
		}
		else if (colorSrcPlugs[i].node().hasFn(MFn::kLayeredTexture))
		{
			m_isTextured = true;
			m_isMultiTextured = true;
			continue;
		}
	}

	// Check if material is transparent
	if (pLambert->findPlug("transparency").isConnected() || pLambert->transparency().r>0.0f)
		m_isTransparent = true;

	// Get material colours
	//diffuse colour
	if (m_isTextured)
		m_diffuse = MColor(1.0,1.0,1.0,1.0);
	else
	{
		m_diffuse = pLambert->color();
		m_diffuse.a = 1.0 - pLambert->transparency().r;
	}
	//ambient colour
	m_ambient = pLambert->ambientColor();
	//emissive colour
	m_emissive = pLambert->incandescence();
	//specular colour
	m_specular = MColor(0,0,0,1);
	delete pLambert;
	return MS::kSuccess;
}

// Load a phong shader
MStatus MayaMaterial::loadPhong(MFnDependencyNode *pShader)
{
	MPlugArray colorSrcPlugs;
	m_type = MT_PHONG;
	MFnPhongShader* pPhong = new MFnPhongShader(pShader->object());
	// Check if material is textured
	pPhong->findPlug("color").connectedTo(colorSrcPlugs,true,false);
	for (int i=0; i<colorSrcPlugs.length(); i++)
	{
		if (colorSrcPlugs[i].node().hasFn(MFn::kFileTexture))
		{
			m_isTextured = true;
			continue;
		}
		else if (colorSrcPlugs[i].node().hasFn(MFn::kLayeredTexture))
		{
			m_isTextured = true;
			m_isMultiTextured = true;
			continue;
		}
	}

	// Check if material is transparent
	if (pPhong->findPlug("transparency").isConnected() || pPhong->transparency().r>0.0f)
		m_isTransparent = true;

	// Get material colours
	//diffuse colour
	if (m_isTextured)
		m_diffuse = MColor(1.0,1.0,1.0,1.0);
	else
	{
		m_diffuse = pPhong->color();
		m_diffuse.a = 1.0 - pPhong->transparency().r;
	}
	//ambient colour
	m_ambient = pPhong->ambientColor();
	//emissive colour
	m_emissive = pPhong->incandescence();
	//specular colour
	m_specular = pPhong->specularColor();
	m_specular.a = pPhong->cosPower()*1.28;
	delete pPhong;
	return MS::kSuccess;
}

// load a blinn shader
MStatus MayaMaterial::loadBlinn(MFnDependencyNode *pShader)
{
	MPlugArray colorSrcPlugs;
	m_type = MT_BLINN;
	MFnBlinnShader* pBlinn = new MFnBlinnShader(pShader->object());
	// Check if material is textured
	pBlinn->findPlug("color").connectedTo(colorSrcPlugs,true,false);
	for (int i=0; i<colorSrcPlugs.length(); i++)
	{
		if (colorSrcPlugs[i].node().hasFn(MFn::kFileTexture))
		{
			m_isTextured = true;
			continue;
		}
		else if (colorSrcPlugs[i].node().hasFn(MFn::kLayeredTexture))
		{
			m_isTextured = true;
			m_isMultiTextured = true;
			continue;
		}
	}

	// Check if material is transparent
	if (pBlinn->findPlug("transparency").isConnected() || pBlinn->transparency().r>0.0f)
		m_isTransparent = true;

	// Get material colours
	//diffuse colour
//	if (m_isTextured)
// 		m_diffuse = MColor(1.0,1.0,1.0,1.0);
//	else
	{
		m_diffuse = pBlinn->color();
		m_diffuse.a = 1.0 - pBlinn->transparency().r;
	}
	//ambient colour
	m_ambient = pBlinn->ambientColor();
	//emissive colour
	m_emissive = pBlinn->incandescence();
	//specular colour
	m_specular = pBlinn->specularColor();
	m_specular.a = 128.0-(128.0*pBlinn->eccentricity());
	delete pBlinn;
	return MS::kSuccess;
}

// load a cgFx shader
MStatus MayaMaterial::loadCgFxShader(MFnDependencyNode *pShader)
{
	m_type = MT_CGFX;
	// Create a default white lambert
	m_isTextured = false;
	m_isMultiTextured = false;
	m_diffuse = MColor(1.0,1.0,1.0,1.0);
	m_specular = MColor(0,0,0,1);
	m_emissive = MColor(0,0,0,1);
	m_ambient = MColor(0,0,0,1);
	return MS::kSuccess;
}


// Load texture data from a texture node
MStatus MayaMaterial::loadTexture(MFnDependencyNode* pTexNode,TexOpType& opType,MStringArray& uvsets,ExporterParams& params)
{
	MayaTexture tex;
	// Get texture filename
	MString filename, absFilename;
	MRenderUtil::exactFileTextureName(pTexNode->object(),absFilename);
	filename = absFilename.substring(absFilename.rindex('/')+1,absFilename.length()-1);
	MString command = "toNativePath(\"";
	command += absFilename;
	command += "\")";
	MGlobal::executeCommand(command,absFilename);
	tex.absFileName = absFilename;
	tex.fileName = filename;
	tex.uvSetIndex = 0;
	tex.uvSetName = "";
	// Set texture operation type
	tex.opType = opType;
	// Get connections to uvCoord attribute of texture node
	MPlugArray texSrcPlugs;
	pTexNode->findPlug("uvCoord").connectedTo(texSrcPlugs,true,false);
	// Get place2dtexture node (if connected)
	MFnDependencyNode* pPlace2dTexNode = NULL;
	for (int j=0; j<texSrcPlugs.length(); j++)
	{
		if (texSrcPlugs[j].node().hasFn(MFn::kPlace2dTexture))
		{
			pPlace2dTexNode = new MFnDependencyNode(texSrcPlugs[j].node());
			continue;
		}
	}
	// Get uvChooser node (if connected)
	MFnDependencyNode* pUvChooserNode = NULL;
	if (pPlace2dTexNode)
	{
		MPlugArray placetexSrcPlugs;
		pPlace2dTexNode->findPlug("uvCoord").connectedTo(placetexSrcPlugs,true,false);
		for (int j=0; j<placetexSrcPlugs.length(); j++)
		{
			if (placetexSrcPlugs[j].node().hasFn(MFn::kUvChooser))
			{
				pUvChooserNode = new MFnDependencyNode(placetexSrcPlugs[j].node());
				continue;
			}
		}
	}
	// Get uvset index
	if (pUvChooserNode)
	{
		bool foundMesh = false;
		bool foundUvset = false;
		MPlug uvsetsPlug = pUvChooserNode->findPlug("uvSets");
		MPlugArray uvsetsSrcPlugs;
		for (int i=0; i<uvsetsPlug.evaluateNumElements() && !foundMesh; i++)
		{
			uvsetsPlug[i].connectedTo(uvsetsSrcPlugs,true,false);
			for (int j=0; j<uvsetsSrcPlugs.length() && !foundMesh; j++)
			{
				if (uvsetsSrcPlugs[j].node().hasFn(MFn::kMesh))
				{
					uvsetsSrcPlugs[j].getValue(tex.uvSetName);
					for (int k=0; k<uvsets.length() && !foundUvset; k++)
					{
						if (uvsets[k] == tex.uvSetName)
						{
							tex.uvSetIndex = k;
							foundUvset = true;
						}
					}
				}
			}
		}
	}
	// Get texture options from Place2dTexture node
	if (pPlace2dTexNode)
	{
		// Get address mode
		//U
		bool wrapU, mirrorU;
		pPlace2dTexNode->findPlug("wrapU").getValue(wrapU);
		pPlace2dTexNode->findPlug("mirrorU").getValue(mirrorU);
		if (mirrorU)
			tex.tam_u = TAM_MIRROR;
		else if (wrapU)
			tex.tam_u = TAM_WRAP;
		else
			tex.tam_u = TAM_CLAMP;
		// V
		bool wrapV,mirrorV;
		pPlace2dTexNode->findPlug("wrapV").getValue(wrapV);
		pPlace2dTexNode->findPlug("mirrorV").getValue(mirrorV);
		if (mirrorV)
			tex.tam_v = TAM_MIRROR;
		else if (wrapV)
			tex.tam_v = TAM_WRAP;
		else
			tex.tam_v = TAM_CLAMP;
		// Get texture scale
		double covU,covV;
		pPlace2dTexNode->findPlug("coverageU").getValue(covU);
		pPlace2dTexNode->findPlug("coverageV").getValue(covV);
		tex.scale_u = covU;
		if (fabs(tex.scale_u) < PRECISION)
			tex.scale_u = 0;
		tex.scale_v = covV;
		if (fabs(tex.scale_v) < PRECISION)
			tex.scale_v = 0;
		// Get texture scroll
		double transU,transV;
		pPlace2dTexNode->findPlug("translateFrameU").getValue(transU);
		pPlace2dTexNode->findPlug("translateFrameV").getValue(transV);
		tex.scroll_u = -0.5 * (covU-1.0)/covU - transU/covU;
		if (fabs(tex.scroll_u) < PRECISION)
			tex.scroll_u = 0;
		tex.scroll_v = 0.5 * (covV-1.0)/covV + transV/covV;
		if (fabs(tex.scroll_v) < PRECISION)
			tex.scroll_v = 0;
		// Get texture rotation
		double rot;
		pPlace2dTexNode->findPlug("rotateFrame").getValue(rot);
		tex.rot = -rot;
		if (fabs(rot) < PRECISION)
			tex.rot = 0;
	}
	// add texture to material texture list
	m_textures.push_back(tex);
	// free up memory
	if (pUvChooserNode)
		delete pUvChooserNode;
	if (pPlace2dTexNode)
		delete pPlace2dTexNode;

	return MS::kSuccess;
}

bool MayaMaterial::copyTextures(ExporterParams &params)
{
	for(int i=0;i<m_textures.size();++i)
	{
		MString cmd="copy \"";
		cmd+=m_textures[i].absFileName;
		cmd+="\" \"";
		cmd+=params.basePath;
		cmd+="\"";
		system(cmd.asChar());
	}
	return true;
}
xml::XMLElement* MayaMaterial::saveXML(xml::XMLElement*parent)
{
// 	video::RenderMaterial* mat=new video::RenderMaterial();
// 	video::RenderPass*p= mat->CreateTechnique("Default")->CreatePass("DefaultPass");


	xml::XMLElement* ret=new xml::XMLElement("Material");
	if(parent)
		parent->addSubElement(ret);
	std::string val;
	xml::XMLElement* teq=new xml::XMLElement("Technique");
	ret->addSubElement(teq);
	xml::XMLElement* pass=new xml::XMLElement("Pass");
	teq->addSubElement(pass);

	teq->addAttribute("Name","Default");
	ret->addAttribute("Name",m_name.asChar());
	val=core::StringConverter::toString(video::SColor(m_ambient.r,m_ambient.g,m_ambient.b,1)).c_str();
	pass->addAttribute("Ambient",val.c_str());
	val=core::StringConverter::toString(video::SColor(m_diffuse.r,m_diffuse.g,m_diffuse.b,m_diffuse.a)).c_str();
	pass->addAttribute("Diffuse",val.c_str());
	val=core::StringConverter::toString(video::SColor(m_specular.r,m_specular.g,m_specular.b,1)).c_str();
	pass->addAttribute("Specular",val.c_str());
	val=core::StringConverter::toString(video::SColor(m_emissive.r,m_emissive.g,m_emissive.b,1)).c_str();
	pass->addAttribute("Emissive",val.c_str());

	pass->addAttribute("Shininess",core::StringConverter::toString(m_specular.a));
	pass->addAttribute("Alpha",core::StringConverter::toString(m_diffuse.a));

	if(m_textures.size()>0)
	{
		xml::XMLElement* texUnits=new xml::XMLElement("TexUnits");
		pass->addSubElement(texUnits);
		for(int i=0;i<m_textures.size();++i)
		{
			xml::XMLElement* unit=new xml::XMLElement("Unit");
			texUnits->addSubElement(unit);
			unit->addAttribute("ID",core::StringConverter::toString(i));
			m_textures[i].saveXML(unit);
		}
	}

	return ret;
}

xml::XMLElement* MayaTexture::saveXML(xml::XMLElement*parent)
{
	xml::XMLElement* ret=new xml::XMLElement("TextureUnit");
	if(parent)
		parent->addSubElement(ret);

	ret->addAttribute("Translate",core::StringConverter::toString(math::vector3d(scroll_u,scroll_v,0)));
	ret->addAttribute("Scale",core::StringConverter::toString(math::vector3d(scale_u,scale_v,0)));
	ret->addAttribute("Rotate",core::StringConverter::toString(math::vector3d(0,0,rot)));
	ret->addAttribute("TextureType","2DTex");
	ret->addAttribute("Texture",fileName.asChar());

	ret->addAttribute("AddressU",convertAddressMode(tam_u));
	ret->addAttribute("AddressV",convertAddressMode(tam_v));
	return ret;
}


}
}
