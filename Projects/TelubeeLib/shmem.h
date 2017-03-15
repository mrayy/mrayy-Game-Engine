#ifndef _INC_T5_SHMEM_H
#define _INC_T5_SHMEM_H

#include <string>

/*--------------------------------------------------------------------------*/
// Shared Memory map definition shared mem access class
//
// Written by Charith@tachilab.org
//
// WIN32 must be defined when compiling for Windows.
// For Visual C++ this is normally already defined.
//
// Copyright (C) 2012, TACHILAB <www.tachilab.org>
//
/*--------------------------------------------------------------------------*/


//#define SHMEM_NAME_PREFIX	"T5MA_MMF_"
//#define SHMEM_SIZE			0x00001900
//#define SHMEM_HEAD_SIZE		0x00000100



class shmem {

public:
	shmem() :data(0), size(0), hMap(0), pMap(0)
	{
	}
	shmem(const std::string& name, unsigned int size)
	{
		m_name = name;
		this->size = size;
	}

	void SetName(const std::string& name)
	{
		m_name = name;
	}
	const std::string& GetName()
	{
		return m_name;
	}

	void SetDataSize( unsigned int size)
	{
		this->size = size;
	}

	template <class T>
	T*GetData(){ return (T*)data; }
	unsigned int GetDataSize(){ return size; }

	bool Attach(void);
	bool Detach(void);
	bool attached(void); 
	int createRead(void); 
	int createWrite(void); 
	int openRead(void); 
	int openWrite(void); 
private: 
	std::string m_name;
	void* data;
	unsigned int size;
	void* hMap;
	void* pMap;
}; 


#endif
