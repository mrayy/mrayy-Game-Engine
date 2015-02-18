
/*--------------------------------------------------------------------------*/
// Shared Memory fuctions for shmem class
//
// Written by Charith@tachilab.org
//
// WIN32 must be defined when compiling for Windows.
// For Visual C++ this is normally already defined.
//
// Copyright (C) 2012, TACHILAB <www.tachilab.org>
//
/*--------------------------------------------------------------------------*/

#include "shmem.h" 
#include <windows.h>



int shmem::createRead(void){

	hMap = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file		try reinterpret_cast<HANDLE>(0xffffffff)
        NULL,                    // default security
        PAGE_READWRITE,          // read/write access
        0,                       // maximum object size (high-order DWORD)
        SHMEM_SIZE,              // maximum object size (low-order DWORD)
        SHMEM_NAME_PREFIX);      // name of mapping object

	if (hMap == NULL)
		return -1;		// error!. shared memory creare failed.

	pMap = MapViewOfFile(
		hMap,				// handle to map object
		FILE_MAP_READ,		// read/write permission
		0, 
		0, 
		SHMEM_SIZE);

	if (pMap == NULL){
		CloseHandle(hMap);
		return -2;		// error! mapviewoffile failed.
	}

	data = reinterpret_cast<shmem_data_t*>(pMap);

	return 1; 

}


int shmem::createWrite(void){

	hMap = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file		try reinterpret_cast<HANDLE>(0xffffffff)
        NULL,                    // default security
        PAGE_READWRITE,          // read/write access
        0,                       // maximum object size (high-order DWORD)
        SHMEM_SIZE,              // maximum object size (low-order DWORD)
        SHMEM_NAME_PREFIX);            // name of mapping object

	if (hMap == NULL)
		return -1;		// error!. shared memory creare failed.

	void* pMap = MapViewOfFile(
		hMap,					// handle to map object
		FILE_MAP_WRITE,			// read/write permission
		0, 
		0, 
		SHMEM_SIZE);

	if (pMap == NULL) {
		CloseHandle(hMap);
		return -2;		// error! mapviewoffile failed.
	}

	data = reinterpret_cast<shmem_data_t*>(pMap);
	memset(data, 0, sizeof(shmem_data_t));
	
	return 1; 

}


int shmem::openRead(void){

	hMap = OpenFileMapping(
		FILE_MAP_READ,		// read/write access
		FALSE,				// do not inherit the name
		SHMEM_NAME_PREFIX);		// name of mapping object

	if(hMap == NULL)
		return -1;		// error!. shared memory open failed.

	void* pMap = MapViewOfFile(
		hMap,				// handle to map object
		FILE_MAP_READ,		// read/write permission
		0, 
		0, 
		SHMEM_SIZE);

	if (pMap == NULL){
		CloseHandle(hMap);
		return -2;		// error! mapviewoffile failed.
	}

	data = reinterpret_cast<shmem_data_t*>(pMap);

	return 1; 
	
}


int shmem::openWrite(void){

	hMap = OpenFileMapping(
		FILE_MAP_WRITE,		// read/write access
		FALSE,				// do not inherit the name
		SHMEM_NAME_PREFIX);		// name of mapping object

	if (hMap == NULL)
		return -1;		// error!. shared memory open failed.

	void* pMap = MapViewOfFile(
		hMap,					// handle to map object
		FILE_MAP_WRITE,		// read/write permission
		0, 
		0, 
		SHMEM_SIZE);

	if (pMap == NULL) {
		CloseHandle(hMap);
		return -2;		// error! mapviewoffile failed.


	}

	data = reinterpret_cast<shmem_data_t*>(pMap);
	memset(data, 0, sizeof(shmem_data_t));

	return 1; 

}

bool shmem::attached(void){
	if(hMap==NULL)
		return false; 
	if(pMap==NULL)
		return false; 

	return true; 

}


bool shmem::Detach(void){

	if(data!=NULL){
		UnmapViewOfFile(data);
		return 0;
	}

	if(hMap!=NULL){
		CloseHandle(hMap);
		return 0;
	}

	if(hMap!=NULL){
		CloseHandle(pMap);
		return 0;
	}

	return 1; 

}


