#include "stdafx.h"

#include "DeclarationTable.h"

namespace mray{
namespace script{


DeclarationTable::ScopInfo::~ScopInfo(){
	m_declarations.clear();
	for(int i=0;i<subScopes.size();++i)
		delete subScopes[i];
	subScopes.clear();
}
DeclarationTable::ScopInfo*DeclarationTable::ScopInfo::addScope(){
	ScopInfo*scope=new ScopInfo();
	scope->level=level+1;
	scope->parent=this;
	scope->subScopes.push_back(scope);
	return scope;
}
int DeclarationTable::ScopInfo::registerDeclaration(const core::string &ident,int id,EDeclarationKind kind){

	DeclarationInfo* newDI=0;
	switch(kind){
		case DT_Const:
			newDI=new DeclarationConst();
			break;
		case DT_Data:
			newDI=new DeclarationData();
			break;
		case DT_Function:
			newDI=new DeclarationFunction();
			break;
		default:
			return E_UnkownDataType;
	}
	newDI->id=id;
	newDI->ident=ident;
	newDI->scope=level;
	m_declarations.insert(DeclarationList::value_type(ident,newDI));
	return 0;

}

DeclarationInfo*DeclarationTable::ScopInfo::retrive(const core::string&ident,bool searchParent){

	DeclarationList::iterator it=m_declarations.find(ident);
	if(it!=m_declarations.end())
		return it->second;
	if(searchParent && parent)
		return parent->retrive(ident,searchParent);
	return 0;
}


const mchar*DeclarationTable::getErrorMessage(ErrorCode e){
	switch (e)
	{
	case E_DuplicateDeclaration:
		return mT("Duplicate Declaration");
		break;
	}
	return mT("Unknown Error Code");
}

DeclarationTable::DeclarationTable(){
	m_currScope=new ScopInfo();
	m_currScope->level=0;
	m_currScope->parent=0;

	m_lastID=-1;
}
DeclarationTable::~DeclarationTable(){
	if(m_currScope)
		delete m_currScope;
}
int DeclarationTable::getID(){
	return ++m_lastID;
}

int DeclarationTable::registerDeclaration(const core::string &ident,EDeclarationKind kind,bool allowDuplicatedLevel){
	DeclarationInfo*di=0;
	if(allowDuplicatedLevel)
		di=retriveCurrScope(ident);
	else
		di=retrive(ident);
	if(di){
		return E_DuplicateDeclaration;
	}
	int id=getID();
	m_currScope->registerDeclaration(ident,id,kind);

	return id;
}
DeclarationInfo* DeclarationTable::retrive(const core::string &ident){
	return m_currScope->retrive(ident,true);
}

DeclarationInfo* DeclarationTable::retriveCurrScope(const core::string &ident){
	return m_currScope->retrive(ident,false);
}
int DeclarationTable::openScop(){
	m_currScope=m_currScope->addScope();
	return m_currScope->level;
}
int DeclarationTable::closeScop(){
	m_currScope=m_currScope->parent;
	return m_currScope->level;

}

}

}
