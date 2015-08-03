#ifndef		_TEXARTNCORDINTERFACE_H_
#define		_TEXARTNCORDINTERFACE_H_
//*******************************************/
//*    include								*/
//*******************************************/
#include	<windows.h>
#include	<stdio.h>
#include	"TexART_NCord000_Terminal.h"
#include	"TexARTNCordLib.h"

class	TexART_NCord000_Interface{
	private:
		HANDLE	hDev;
		BYTE	timer_flg;
		char	inifile[MAX_PATH];
	public:
		int							Periodic_Time;
		int							Number_Terminal;
		int							Use_Dummy;
		TexART_NCord000_Terminal	*Terminal;
		TexART_NCord000_Interface();
		TexART_NCord000_Interface(int nDevNum);
		TexART_NCord000_Interface(int nDevNum,char *ini);
		~TexART_NCord000_Interface();
		int		Initialize(void);
		int		Start(void);
		int		Contact(void);
		int		ContactPeriodic(void);
		int		ResetTerminal(int Num_Terminal);
		int		ResetCounter(int Num_Terminal, int Num_Counter, long Counter_Value);
		int		Free(void);
		int		Gain(void);
};

#endif
