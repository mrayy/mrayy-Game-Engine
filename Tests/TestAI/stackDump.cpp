
#include "stackDump.h"
#include <macros.h>
#include <string.h>
#include <LogManager.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#ifndef __specstrings
#define __specstrings
#	define __in
#	define __out
#	define __inout
#	define __in_opt
#	define __out_opt
#	define __inout_opt
#	define __in_ecount
#	define __out_ecount(A)
#	define __inout_ecount
#	define __in_bcount
#	define __out_bcount(A)
#	define __inout_bcount
#	define __deref_opt_out
#	define __deref_out
#	define __out_xcount(A)
#endif



#include <imagehlp.h>

typedef BOOL    (__stdcall *tSymInitialize)( IN HANDLE hProcess, IN PSTR UserSearchPath, IN BOOL fInvadeProcess );
typedef DWORD   (__stdcall *tSymGetOptions)( VOID );
typedef DWORD   (__stdcall *tSymSetOptions)( IN DWORD SymOptions );
typedef BOOL    (__stdcall *tSymGetSymFromAddr)( IN HANDLE hProcess, IN DWORD dwAddr, OUT PDWORD pdwDisplacement, OUT PIMAGEHLP_SYMBOL Symbol );
typedef BOOL    (__stdcall *tStackWalk64)( DWORD MachineType, HANDLE hProcess, HANDLE hThread, LPSTACKFRAME64 StackFrame, PVOID ContextRecord, PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine, PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine, PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine, PTRANSLATE_ADDRESS_ROUTINE64 TranslateAddress );
typedef PVOID   (__stdcall *tSymFunctionTableAccess64)( HANDLE hProcess, DWORD64 AddrBase );
typedef DWORD64 (__stdcall *tSymGetModuleBase64)( IN HANDLE hProcess, IN DWORD64 dwAddr );
typedef BOOL    (__stdcall *tSymGetLineFromAddr64)( IN HANDLE hProcess, IN DWORD64 dwAddr, OUT PDWORD pdwDisplacement, OUT PIMAGEHLP_LINE64 Line );

static HINSTANCE hDbgHelpDll;
static int s_DllReferences;

static tSymInitialize pSymInitialize;
static tSymGetOptions pSymGetOptions;
static tSymSetOptions pSymSetOptions;
static tSymGetSymFromAddr pSymGetSymFromAddr;
static tStackWalk64 pStackWalk64;
static tSymFunctionTableAccess64 pSymFunctionTableAccess64;
static tSymGetModuleBase64 pSymGetModuleBase64;
static tSymGetLineFromAddr64 pSymGetLineFromAddr64;

#define LOAD_FUNCTION(A) if(1) { p##A = (t##A) GetProcAddress(hDbgHelpDll, #A);\
	FATAL_ERROR(!p##A, mT("Could not load symbol from imghelp.dll")); }

namespace mray{
const mchar* hkStackTracerDbghelpPath = mT("dbghelp.dll");


stackDump::stackDump(){

	if( hDbgHelpDll == 0 )
	{
		// If you do not get symbols in the stack trace, it could be because the version of dbghelp.dll
		// in your path is too old. You can download a new one from
		// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/debug/base/dbghelp_versions.asp
		// And set an explicit path for the dll to load
		// extern const char* hkStackTracerDbghelpPath;
		// hkStackTracerDbghelpPath = "C:\\Program Files\\Debugging Tools for Windows\\dbghelp.dll";
		hDbgHelpDll = LoadLibrary(hkStackTracerDbghelpPath);
		FATAL_ERROR(!hDbgHelpDll, mT("Unable to load dbghelp.dll"));

		s_DllReferences = 1;
		LOAD_FUNCTION(SymInitialize);
		LOAD_FUNCTION(SymGetOptions);
		LOAD_FUNCTION(SymSetOptions);
		LOAD_FUNCTION(SymGetSymFromAddr);
		LOAD_FUNCTION(StackWalk64);
		LOAD_FUNCTION(SymFunctionTableAccess64);
		LOAD_FUNCTION(SymGetModuleBase64);
		LOAD_FUNCTION(SymGetLineFromAddr64);

		DWORD symOptions = pSymGetOptions();
		symOptions |= SYMOPT_LOAD_LINES | SYMOPT_DEBUG;
		pSymSetOptions( symOptions );

		BOOL initsymbols = pSymInitialize( GetCurrentProcess(), 0, TRUE);
		FATAL_ERROR(!initsymbols,mT(""));
	}
	else
	{
		s_DllReferences += 1;
	}
}

stackDump::~stackDump()
{
	s_DllReferences -= 1;
	if( s_DllReferences == 0 )
	{
		pSymInitialize = 0;
		pSymGetOptions = 0;
		pSymSetOptions = 0;
		pSymGetSymFromAddr = 0;
		pStackWalk64 = 0;
		pSymFunctionTableAccess64 = 0;
		pSymGetModuleBase64 = 0;
		pSymGetLineFromAddr64 = 0;
		FreeLibrary( hDbgHelpDll );
		hDbgHelpDll = 0;
	}
}

void stackDump::inner_dumpStack(DWORD64 *trace,int length){
 
 	char buf[2048];
 	HANDLE curproc = GetCurrentProcess();
 
 	int maxtrace=30;
 	core::string s;
 
 	for( int i = 0; i < length; ++i)
 	{
 		DWORD64 symaddress =trace[i];
 
 		IMAGEHLP_SYMBOL& sym = *(IMAGEHLP_SYMBOL*)buf;
 		memset(&sym, 0, sizeof(IMAGEHLP_SYMBOL));
 		sym.SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
 		sym.MaxNameLength = 2048 - sym.SizeOfStruct;
 
 		DWORD displacement = 0;
 		BOOL ok_symbol = pSymGetSymFromAddr( 
 			curproc, 
 			symaddress,
 			&displacement,
 			&sym);
 
 		if(! ok_symbol)
 		{
 			static bool haveWarned;
 			if( ! haveWarned )
 			{
 				gLogManager.log(mT("**************************************************************\n" )\
 					mT("* Cannot find symbol for an address\n") \
 					mT("* Either debug information was not found or your version of\n") \
 					mT("* dbghelp.dll may be too old to understand the debug format.\n") \
 					mT("* See the comments in hkStackTracer::hkStackTracer()\n") \
 					mT("* ") mT(__FILE__) mT("\n") \
 					mT("**************************************************************\n"), ELL_INFO);
 				haveWarned = 1;
 			}
 
 			::strcpy(sym.Name, "(unknown)");
 		}
 		else if (::strcmp(sym.Name, "WinMain") == 0 )
 		{
 			break;
 		}
 
 		IMAGEHLP_LINE64 line;
 		::memset(&line, 0, sizeof(line));
 		line.SizeOfStruct = sizeof(line);
 
 		pSymGetLineFromAddr64(
 			curproc,
 			DWORD(symaddress),
 			&displacement,
 			&line);
 
 		char trace[2048];
 		_snprintf(trace, 2048, "%s(%i):'%s'\n", line.FileName, line.LineNumber, sym.Name);
 		core::char_to_string(trace,s);
 		gLogManager.log(s,ELL_INFO);
 	}
 	gLogManager.log(mT("-------------------------------------------------------------------\n\n"),ELL_INFO);
}
void stackDump::dumpStack(){


	int maxtrace=100;
	DWORD64 trace[100];

	HANDLE curproc   = GetCurrentProcess();
	HANDLE curthread = GetCurrentThread();

	CONTEXT context;
	::memset(&context, 0, sizeof(context));
	/*
	__asm
	{
	call GetEIP
	jmp Done
	GetEIP:
	pop eax
	mov context.Eip, eax
	mov context.Ebp, ebp
	push eax
	ret
	Done:
	}
	*/
	context.ContextFlags = CONTEXT_FULL;
	BOOL ok_context =  GetThreadContext(curthread, &context);
	FATAL_ERROR(!ok_context,mT("!ok_context"));

	STACKFRAME64 sf;
	::memset(&sf, 0, sizeof(sf));
	sf.AddrPC.Offset    = context.Eip;
	sf.AddrPC.Mode      = AddrModeFlat;
	sf.AddrFrame.Offset = context.Ebp;
	sf.AddrFrame.Mode   = AddrModeFlat;

	int frame = -2;

	for( ; frame < maxtrace; ++frame )
	{
		if( pStackWalk64(
			0,//PIXEL_FILE_MACHINE_I386,
			curproc, 
			curthread,
			&sf,
			&context,
			0,
			pSymFunctionTableAccess64,
			pSymGetModuleBase64,
			0) )
		{
			if(frame >= 0)
			{
				trace[frame] = (DWORD64)sf.AddrPC.Offset;
			}
		}
		else
		{
			break;
		}
	}

	inner_dumpStack(trace,frame);
}

}

