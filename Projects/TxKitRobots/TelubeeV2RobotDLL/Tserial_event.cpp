

//#define STRICT
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <conio.h>
#include <windows.h>


#include "Tserial_event.h"

#define SIG_POWER_DOWN     0
#define SIG_READER         1
#define SIG_READ_DONE      2    // data received has been read
#define SIG_WRITER         3
#define SIG_DATA_TO_TX     4    // data waiting to be sent
#define SIG_MODEM_EVENTS   5
#define SIG_MODEM_CHECKED  6

void Tserial_event_thread_start(void *arg);

typedef unsigned (WINAPI *PBEGINTHREADEX_THREADFUNC) (LPVOID lpThreadParameter);
typedef unsigned *PBEGINTHREADEX_THREADID;



void Tserial_event_thread_start(void *arg){
	class Tserial_event *serial_unit;

	serial_unit = (Tserial_event *) arg;

	if (serial_unit!=0)
		serial_unit->run();
}


Tserial_event::Tserial_event(){
	int i;

	ready            = false;
	parityMode       = SERIAL_PARITY_NONE;
	port[0]          = 0;
	rate             = 0;
	threadid         = 0;
	serial_handle    = INVALID_HANDLE_VALUE;
	thread_handle    = 0;
	owner            = 0;
	tx_in_progress   = 0;
	rx_in_progress   = 0;
	max_rx_size      = 1;
	tx_size          = 0;
	received_size    = 0;
	check_modem      = false;

	manager          = 0;

	/* -------------------------------------------------------------- */
	// creating Events for the different sources
	for (i=0; i<SERIAL_SIGNAL_NBR; i++)
	{
		if ((i==SIG_READER) || (i==SIG_WRITER) || (i==SIG_MODEM_EVENTS))
			serial_events[i] = CreateEvent(NULL, TRUE, FALSE, NULL);  // Manual Reset
		else
			serial_events[i] = CreateEvent(NULL, FALSE, FALSE, NULL); // Auto reset
	}
}


Tserial_event::~Tserial_event(){
	int i;

	if (thread_handle!=0)
		WaitForSingleObject(thread_handle, 2000);
	thread_handle = 0;

	/* -------------------------------------------------------- */
	for (i=0; i<SERIAL_SIGNAL_NBR; i++)         // deleting the events
	{
		if (serial_events[i]!=INVALID_HANDLE_VALUE)
			CloseHandle(serial_events[i]);
		serial_events[i] = INVALID_HANDLE_VALUE;
	}

	if (serial_handle!=INVALID_HANDLE_VALUE)
		CloseHandle(serial_handle);
	serial_handle = INVALID_HANDLE_VALUE;
}

void Tserial_event::disconnect(void){
	ready = false;
	SetEvent(serial_events[SIG_POWER_DOWN]);

	if (thread_handle!=0)
		WaitForSingleObject(thread_handle, 2000);
	if (serial_handle != INVALID_HANDLE_VALUE)
		CloseHandle(serial_handle);
	serial_handle = 0;
	thread_handle = 0;
}


int  Tserial_event::connect (char *port_arg, int  rate_arg,  int parity_arg,
	char ByteSize , bool modem_events, bool flowControl)
{
	int  erreur;
	DCB  dcb;
	int  i;
	bool hard_handshake;
	COMMTIMEOUTS cto = { 0, 0, 0, 0, 0 };

	/* --------------------------------------------- */
	if (serial_handle!=INVALID_HANDLE_VALUE)
		CloseHandle(serial_handle);
	serial_handle = INVALID_HANDLE_VALUE;

	if (port_arg!=0)
	{
		strncpy(port, port_arg, 10);
		rate        = rate_arg;
		parityMode  = parity_arg;
		check_modem = modem_events;
		hard_handshake = flowControl;

		erreur      = 0;
		ZeroMemory(&ovReader   ,sizeof(ovReader)   );  // clearing the overlapped
		ZeroMemory(&ovWriter   ,sizeof(ovWriter)   );
		ZeroMemory(&ovWaitEvent,sizeof(ovWaitEvent));
		memset(&dcb,0,sizeof(dcb));

		/* -------------------------------------------------------------------- */
		// set DCB to configure the serial port
		dcb.DCBlength       = sizeof(dcb);                   

		/* ---------- Serial Port Config ------- */
		dcb.BaudRate        = rate;

		switch(parityMode)
		{
		case SERIAL_PARITY_NONE:
			dcb.Parity      = NOPARITY;
			dcb.fParity     = 0;
			break;
		case SERIAL_PARITY_EVEN:
			dcb.Parity      = EVENPARITY;
			dcb.fParity     = 1;
			break;
		case SERIAL_PARITY_ODD:
			dcb.Parity      = ODDPARITY;
			dcb.fParity     = 1;
			break;
		}


		dcb.StopBits        = ONESTOPBIT;
		dcb.ByteSize        = (BYTE) ByteSize;

		// -------------- modified 2005-05-16 ---------------
		dcb.fInX            = FALSE;
		dcb.fOutX           = FALSE;

		//hard_handshake = flowControl;
		//hard_handshake = false;

		if (hard_handshake)
		{
			// modified by charith on 2014.03.13
			dcb.fInX = TRUE;
			dcb.fOutX = TRUE;

			dcb.fOutxDsrFlow    = FALSE;
			dcb.fOutxCtsFlow    = FALSE;

			dcb.fDsrSensitivity = FALSE;

			dcb.fRtsControl     = RTS_CONTROL_DISABLE;
			dcb.fDtrControl = DTR_CONTROL_DISABLE;
		}
		else
		{
			dcb.fOutxDsrFlow    = FALSE;
			dcb.fOutxCtsFlow    = FALSE;
			/* */
			dcb.fRtsControl     = RTS_CONTROL_ENABLE;
			dcb.fDtrControl     = DTR_CONTROL_ENABLE;
		}

		dcb.fErrorChar      = 0;
		dcb.fBinary         = 1;
		dcb.fNull           = 0;
		dcb.fAbortOnError   = 0;
		dcb.wReserved       = 0;
		dcb.XonLim          = 2;
		dcb.XoffLim         = 4;
		dcb.XonChar         = 0x13;
		dcb.XoffChar        = 0x19;
		dcb.EvtChar         = 0;

		/* -------------------------------------------------------------------- */
		serial_handle    = CreateFileA(port, GENERIC_READ | GENERIC_WRITE,
			0, NULL, OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);
		// opening serial port

		ovReader.hEvent    = serial_events[SIG_READER];
		ovWriter.hEvent    = serial_events[SIG_WRITER];
		ovWaitEvent.hEvent = serial_events[SIG_MODEM_EVENTS];

		if (serial_handle    != INVALID_HANDLE_VALUE)
		{
			if (check_modem)
			{
				if(!SetCommMask(serial_handle, EV_RING | EV_RLSD))
					erreur = 1;
			}
			else
			{
				if(!SetCommMask(serial_handle, 0))
					erreur = 1;
			}


			// set timeouts
			if(!SetCommTimeouts(serial_handle,&cto))
				erreur = 2;

			// set DCB
			if(!SetCommState(serial_handle,&dcb))
				erreur = 4;
		}
		else
			erreur = 8;
	}
	else
		erreur = 16;


	/* --------------------------------------------- */
	for (i=0; i<SERIAL_SIGNAL_NBR; i++)
	{
		if (serial_events[i]==INVALID_HANDLE_VALUE)
			erreur = 32;
	}

	/* --------------------------------------------- */
	if (erreur!=0)
	{
		CloseHandle(serial_handle);
		serial_handle = INVALID_HANDLE_VALUE;
	}
	else
	{
		// start thread
		thread_handle = (HANDLE) _beginthreadex(NULL,0,
			(PBEGINTHREADEX_THREADFUNC) Tserial_event_thread_start,
			this, 0, &threadid);
	}

	/* --------------------------------------------- */
	return(erreur);
}

void Tserial_event::setManager(type_myCallBack manager_arg){
	manager = manager_arg;
}

void Tserial_event::setRxSize(int size){
	max_rx_size = size;
	if (max_rx_size>SERIAL_MAX_RX)
		max_rx_size = SERIAL_MAX_RX;
}
/* -------------------------------------------------------------------- */
/* ---------------------           setManager     --------------------- */
/* -------------------------------------------------------------------- */
char *      Tserial_event::getDataInBuffer(void)
{
	return(rxBuffer);
}
/* -------------------------------------------------------------------- */
/* ---------------------           setManager     --------------------- */
/* -------------------------------------------------------------------- */
int      Tserial_event::getDataInSize(void)
{
	return(received_size);
}
/* -------------------------------------------------------------------- */
/* ---------------------           setManager     --------------------- */
/* -------------------------------------------------------------------- */
void   Tserial_event::dataHasBeenRead(void)
{
	SetEvent(serial_events[SIG_READ_DONE]);
}
/* -------------------------------------------------------------------- */
/* -----------------------   getNbrOfBytes  --------------------------- */
/* -------------------------------------------------------------------- */
int Tserial_event::getNbrOfBytes    (void)
{
	struct _COMSTAT status;
	int             n;
	unsigned long   etat;

	n = 0;

	if (serial_handle!=INVALID_HANDLE_VALUE)
	{
		ClearCommError(serial_handle, &etat, &status);
		n = status.cbInQue;
	}
	return(n);
}
/* -------------------------------------------------------------------- */
/* --------------------------    sendData     ------------------------- */
/* -------------------------------------------------------------------- */
void Tserial_event::sendData (char *buffer, int size)
{
	if ((!tx_in_progress) && (size<SERIAL_MAX_TX) && (buffer!=0))
	{
		tx_in_progress = 1;
		memcpy(txBuffer, buffer, size);
		tx_size = size;
		SetEvent(serial_events[SIG_DATA_TO_TX]);
		// indicating data to be sent
	}
}
void Tserial_event::OnEvent (unsigned long events)
{
	unsigned long ModemStat;

	GetCommModemStatus(serial_handle, &ModemStat);

	if ((events & EV_RING)!=0)
	{
		if ((ModemStat &  MS_RING_ON)!= 0)
		{
			if (manager!=0)
				manager((uint32) this, SERIAL_RING);
		}
	}

	if ((events & EV_RLSD)!=0)
	{
		if ((ModemStat &  MS_RLSD_ON)!= 0)
		{
			if (manager!=0)
				manager((uint32) this, SERIAL_CD_ON);
		}
		else
		{
			if (manager!=0)
				manager((uint32) this, SERIAL_CD_OFF);
		}
	}
}

void Tserial_event::run(void)
{
	bool          done;
	long          status;
	unsigned long read_nbr, result_nbr;
	char          success;

	ready                   = true;
	done                    = false;
	tx_in_progress          = 0;
	rx_in_progress          = 0;
	WaitCommEventInProgress = 0;

	if (manager!=0)
		manager((uint32) this, SERIAL_CONNECTED);

	GetLastError();               // just to clear any pending error
	SetEvent(serial_events[SIG_READ_DONE]);
	if (check_modem)
		SetEvent(serial_events[SIG_MODEM_CHECKED]);

	while(!done)
	{
		status = WaitForMultipleObjects(SERIAL_SIGNAL_NBR, serial_events,
			FALSE, INFINITE);
		status = status - WAIT_OBJECT_0;
		if ((status<0) || (status>=SERIAL_SIGNAL_NBR))
			done=true;   // error
		else
		{
			switch(status)
			{
			case SIG_POWER_DOWN:
				done = true;
				break;
			case SIG_READ_DONE:
				if (!rx_in_progress)
				{
					rx_in_progress = 1;
					success = (char) ReadFile(serial_handle,&rxBuffer,
						max_rx_size,&read_nbr,&ovReader);
					if (!success)
					{
						if(GetLastError() != ERROR_IO_PENDING )
						{
							done = true;
#ifdef DEBUG_EVENTS
							printf("Readfile error (not pending)\n");
#endif DEBUG_EVENTS
						}
#ifdef DEBUG_EVENTS
						else
							printf("ReadFile pending\n");
#endif DEBUG_EVENTS
					}
#ifdef DEBUG_EVENTS
					else
					{
						printf("ReadFile immediate success\n");
					}
#endif
				}
				break;

			case SIG_READER:

				if (GetOverlappedResult(serial_handle, &ovReader,
					&result_nbr, FALSE))
				{
#ifdef DEBUG_EVENTS
					printf("ReadFile => GetOverlappedResult done\n");
#endif DEBUG_EVENTS
					ResetEvent(serial_events[SIG_READER]);
					received_size  = result_nbr;
					rx_in_progress = 0; // read has ended
					if ((result_nbr!=0) &&(manager!=0))
						manager((uint32) this, SERIAL_DATA_ARRIVAL);
				}
				else
				{
					if(GetLastError()!= ERROR_IO_PENDING )
						done = 1;  // failure
				}
				break;

			case SIG_DATA_TO_TX:
				success = (char) WriteFile(serial_handle, txBuffer, tx_size,
					&result_nbr, &ovWriter);
				if (!success)
				{
					if(GetLastError() != ERROR_IO_PENDING )
					{
						done = true;
#ifdef DEBUG_EVENTS
						printf("WriteFile error (not pending)\n");
#endif DEBUG_EVENTS
					}
#ifdef DEBUG_EVENTS
					else
						printf("WriteFile pending\n");
#endif DEBUG_EVENTS
				}
#ifdef DEBUG_EVENTS
				else
				{
					printf("WriteFile immediate success\n");
				}
#endif
				break;
			case SIG_WRITER:
				if (GetOverlappedResult(serial_handle, &ovWriter,
					&result_nbr, FALSE))
				{
					ResetEvent(serial_events[SIG_WRITER]);
					tx_in_progress = 0;
					if (manager!=0)
						manager((uint32) this, SERIAL_DATA_SENT);
				}
				else
				{
					if(GetLastError() != ERROR_IO_PENDING )
						done = 1;  // failure
				}
				break;

			case SIG_MODEM_CHECKED:
				if ((!WaitCommEventInProgress) && check_modem)
				{            
					WaitCommEventInProgress=1;
					success = (char) WaitCommEvent(serial_handle,&dwCommEvent,
						&ovWaitEvent);
					if (!success)
					{
						if(GetLastError() != ERROR_IO_PENDING )
						{
							done = true;
#ifdef DEBUG_EVENTS
							printf("WaitCommEvent error (not pending)\n");
#endif DEBUG_EVENTS
						}
#ifdef DEBUG_EVENTS
						else
							printf("WaitCommEvent pending\n");
#endif DEBUG_EVENTS
					}
#ifdef DEBUG_EVENTS
					else
					{
						printf("WaitCommEvent immediate success\n");
					}
#endif
				}
				break;
			case SIG_MODEM_EVENTS:
				if (GetOverlappedResult(serial_handle, &ovWaitEvent,
					&result_nbr, FALSE))
				{
					ResetEvent(serial_events[SIG_MODEM_EVENTS]);
					WaitCommEventInProgress = 0;
					OnEvent(dwCommEvent);
					SetEvent(serial_events[SIG_MODEM_CHECKED]);
				}
				else
				{
					if(GetLastError() != ERROR_IO_PENDING )
						done = 1;  // failure
				}
				break;
			}
		}
	};

	ready = false;
	if (serial_handle!=INVALID_HANDLE_VALUE)
		CloseHandle(serial_handle);
	serial_handle = INVALID_HANDLE_VALUE;

	if (manager!=0)
		manager((uint32) this, SERIAL_DISCONNECTED);
}

