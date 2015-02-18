
/********************************************************************
	created:	2009/03/01
	created:	1:3:2009   23:23
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\Param.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	Param
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___Param___
#define ___Param___

#include "CompileConfig.h"
#include "mstring.h"

namespace mray{

class MRAY_DLL Param
{
public:
	enum EParamType{
		EPT_NULL,
		EPT_INT,
		EPT_FLOAT,
		EPT_BOOL,
		EPT_VOID,
		EPT_STRING,
	};
private:
	union{
		int i;
		float f;
		bool b;
		void*v;
	};
	core::string m_str;

	EParamType m_type;
public:
	Param();
	Param(const Param&o);
	virtual~Param();

	Param(int o){setInt(o);}
	Param(float o){setFloat(o);}
	Param(bool o){setBool(o);}
	Param(void* o){setVoid(o);}
	Param(const mchar* o){setStr(o);}

	void copy(const Param&o);

	Param& operator =(const Param&o);
	bool operator ==(const Param&o)const;

	// t : is the type
	// 'i' :int
	// 'f' :float
	// 'b' :bool
	// 'v' :void*
	// 's' :string
	void setType(char t);
	bool isValid(char t);
	
	EParamType getType()const;
	char getTypeChar()const;

	void setNull();

	void setInt(int o);
	void setFloat(float o);
	void setBool(bool o);
	void setVoid(void* o);
	void setStr(const core::string& o);

	int getInt()const;
	float getFloat()const;
	bool getBool()const;
	void* getVoid()const;
	const core::string& getStr()const;

	void clearData();
};

}


#endif //___Param___
