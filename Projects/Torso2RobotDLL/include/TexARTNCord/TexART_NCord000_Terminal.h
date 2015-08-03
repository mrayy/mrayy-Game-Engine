#ifndef		_TEXARTNCORDTERMINAL_H_
#define		_TEXARTNCORDTERMINAL_H_
//*******************************************/
//*    include								*/
//*******************************************/
#include	<windows.h>
#include	<stdio.h>

class	TexART_NCord000_Terminal{
	public:
		TexART_NCord000_Terminal(void);
		~TexART_NCord000_Terminal();
		void	setValue(int drv, int ad, int counter, short param[], char moter[]);
		int		N_Drv;
		int		N_AD;
		int		N_Counter;
		int		*Drv_Command;
		int		*AD_Value;
		int		*Counter_Value;
		short	*P_Gain;
		char	*P_Moter;			// モータドライバ種別 '09.09.17 M.S
};
#endif
