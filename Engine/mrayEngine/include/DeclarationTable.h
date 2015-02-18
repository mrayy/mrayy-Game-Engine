
#ifndef ___DeclarationTable___
#define ___DeclarationTable___

#include "mString.h"



namespace mray{
namespace script{

enum EDeclarationKind{
	DT_Data,
	DT_Function,
	DT_Const,
	DT_Unknown
};
class DeclarationInfo{
public:
	EDeclarationKind kind;
	core::string ident;
	int id;
	int scope;

	DeclarationInfo(){
		ident="";
		id=-1;
		scope=0;
		kind=DT_Unknown;
	}

	bool operator <(const DeclarationInfo&o)const{
		return ident<o.ident;
	}
	virtual const mchar*getType()=0;
};

class DeclarationData:public DeclarationInfo{
public:
	virtual const mchar*getType(){return mT("Data");}
};
class DeclarationFunction:public DeclarationInfo{
public:
	virtual const mchar*getType(){return mT("Function");}
};
class DeclarationConst:public DeclarationInfo{
public:
	virtual const mchar*getType(){return mT("Const");}
};

// DeclarationTable support Nested block structure
class MRAY_DLL DeclarationTable
{
public:
	enum ErrorCode{
		E_DuplicateDeclaration=-1,
		E_UnkownDataType
	};
	static const mchar*getErrorMessage(ErrorCode e);
protected:

	struct ScopInfo;
	typedef std::map<core::string,DeclarationInfo*> DeclarationList;
	typedef std::vector<ScopInfo*> ScopeList;

	struct ScopInfo{
		ScopInfo(){
			level=0;
			parent=0;
		}
		virtual~ScopInfo();

		int level;

		ScopInfo*parent;
		DeclarationList m_declarations;
		ScopeList subScopes;

		ScopInfo*addScope();
		int registerDeclaration(const core::string &ident,int id,EDeclarationKind kind);

		DeclarationInfo*retrive(const core::string&d,bool searchParent);

		bool operator <(const ScopInfo&o)const{
			return level<o.level;
		}
	};

	ScopInfo *m_currScope;

	int m_lastID;

	int getID();
public:
	DeclarationTable();
	virtual~DeclarationTable();

	int registerDeclaration(const core::string &ident,EDeclarationKind kind,bool allowDuplicatedLevel=false);
	DeclarationInfo* retrive(const core::string &ident);
	DeclarationInfo* retriveCurrScope(const core::string &ident);

	//return opened scope level
	int openScop();
	//return closed scope level
	int closeScop();
};

}
}
#endif //___DeclarationTable___


