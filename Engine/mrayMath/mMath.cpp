
#include "mMath.h"


namespace mray{
namespace math{

bool MTrigTable::m_initlized=false;
unsigned short MTrigTable::m_trigTableSize=0;
double  MTrigTable::m_trigTableFactor=0;

double *MTrigTable::m_sinTable=0;
double *MTrigTable::m_tanTable=0;

bool MTrigTable::m_useTable=false;

MTrigTable::MTrigTable(unsigned short tableSize){
	if(!m_initlized){
		tableSize*=2;
		m_initlized=true;
		m_trigTableSize=tableSize;
		m_trigTableFactor=m_trigTableSize/(2*PI64);

		m_sinTable=new double[m_trigTableSize];
		m_tanTable=new double[m_trigTableSize];

		//m_useTable=true;

		buildTables();
	}
}
MTrigTable::~MTrigTable(){
	delete [] m_sinTable;
	delete [] m_tanTable;
	m_initlized=false;
}

void MTrigTable::useTable(bool use){
	m_useTable=use;
}
bool MTrigTable::isUseTable(){
	return m_useTable;
}
double MTrigTable::sinTable(double angle){

	if(!m_initlized || !m_useTable)return ::sin(angle);

	int index=0;
	if(angle>=0){
		index=(unsigned short)(angle*(m_trigTableFactor+1))%m_trigTableSize;
	}else
	{
		index=m_trigTableSize - ((unsigned short)(-angle*m_trigTableFactor)%m_trigTableSize) -1;
	}

	return m_sinTable[index];
}

double MTrigTable::tanTable(double angle){
	
	if(!m_initlized || !m_useTable)return ::tan(angle);

	int index=0;
	if(angle>=0){
		index=(unsigned short)(angle*m_trigTableFactor)%m_trigTableSize;
	}else
	{
		index=m_trigTableSize - ((unsigned short)(-angle*m_trigTableFactor)%m_trigTableSize) -1;
	}

	return m_tanTable[index];
}

void MTrigTable::buildTables(){
	double angle=0;
	for(int i=0;i<m_trigTableSize;++i){
		angle=(double)(2*i*PI64)/(double)m_trigTableSize;
		m_sinTable[i]=::sin(angle);
		m_tanTable[i]=::tan(angle);
	}
}





}
}
