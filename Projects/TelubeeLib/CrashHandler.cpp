
#include "stdafx.h"
#include "CrashHandler.h"
#include "StackWalker.h"

#include <signal.h>

namespace mray
{

	class CrashHandlerStackWalker :public StackWalker
	{
	protected:
		FILE* m_fout;
		core::string m_reportFileName;


		virtual void OnOutput(LPCSTR szText)
		{
			fprintf(m_fout, "%s", szText);
			printf( "%s", szText);
		}
	public:
		CrashHandlerStackWalker(const core::string& fname)
		{
			m_reportFileName = fname;
		}

		void Report()
		{
			fopen_s(&m_fout,m_reportFileName.c_str(), "w");
			if (!m_fout)
			{
				exit(1);
			}
			fprintf(m_fout, "Application crashed!\nCall-stack report:\n");
			printf("Application crashed!\nCall-stack report:\n");
			ShowCallstack();
			fclose(m_fout);
			exit(1);
		}
	};

	CrashHandler::CrashHandler(const core::string& reportFile)
	{
		m_reportFileName = reportFile;
		signal(SIGSEGV, CrashHandlerFunc);
	}
	CrashHandler::~CrashHandler()
	{

	}

	void CrashHandler::CrashHandlerFunc(int signal)
	{
		CrashHandler::getInstance()._handleSignal(signal);
	}

	void CrashHandler::_handleSignal(int signal)
	{
		CrashHandlerStackWalker w(m_reportFileName);
		w.Report();
	}
}