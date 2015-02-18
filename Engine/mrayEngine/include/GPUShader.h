

#ifndef ___GPUShader___
#define ___GPUShader___


#include "mString.h"
#include "GCPtr.h"
#include "videoCommon.h"
#include "IGPUShaderProgram.h"
#include "IResourceHolder.h"

namespace mray{
namespace video{

class MRAY_DLL GPUShader:public IResourceHolder
{
protected:
	core::string m_name;

	IGPUShaderProgramPtr m_geometryShader;
	IGPUShaderProgramPtr m_vertexShader;
	IGPUShaderProgramPtr m_fragmentShader;
public :
	
	GPUShader();
	virtual ~GPUShader();

	void SetName(const core::string&name){m_name=name;}
	const core::string& GetName()const{return m_name;}

	void SetGeometryShader(const IGPUShaderProgramPtr& s);
	void SetVertexShader(const IGPUShaderProgramPtr& s);
	void SetFragmentShader(const IGPUShaderProgramPtr& s);

	const IGPUShaderProgramPtr& GetGeometryShader();
	const IGPUShaderProgramPtr& GetVertexShader();
	const IGPUShaderProgramPtr& GetFragmentShader();

	virtual void use();
	virtual void unuse();


	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);

};

MakeSharedPtrType(GPUShader)

}
}





#endif

