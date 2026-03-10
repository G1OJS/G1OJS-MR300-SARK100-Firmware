//*****************************************************************************/
//  This file is a part of the "SARK100 SWR Analyzer firmware"
//
//  Copyright © 2010 Melchor Varela - EA4FRB.  All rights reserved.
//  Melchor Varela, Madrid, Spain.
//  melchor.varela@gmail.com
//
//  Modifications Copyright © 2024,2025 Alan Robinson G1OJS 
//	Alan Robinson, Hampshire England G1OJS@yahoo.com 
//
//  "SARK100 SWR Analyzer firmware" is free software: you can redistribute it
//  and/or modify it under the terms of the GNU General Public License as
//  published by the Free Software Foundation, either version 3 of the License,
//  or (at your option) any later version.
//
//  "SARK100 SWR Analyzer firmware" is distributed in the hope that it will be
//  useful,  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with "SARK100 SWR Analyzer firmware".  If not,
//  see <http://www.gnu.org/licenses/>.
//*****************************************************************************/
//*****************************************************************************/
//
//	PROJECT:	SARK100 SWR Analyzer
// 	FILE NAME: 	PCLINK.C
// 	AUTHOR:		EA4FRB - Melchor Varela
//
// 	DESCRIPTION
//
//	PC link routines for update measurements
//
// 	HISTORY
//
//	NAME   		DATE		REMARKS
//
//	MVM	   		DEC 2009	Creation
//  AJR			JAN 2025	Align with refactoring and functional changes	
//*****************************************************************************/
#include "pclink.h"
#include "pclink_cmds.h"

#include <stdlib.h>
#include <string.h>
#include "control.h"
#include "UART.h"
#include "screens.h"


//-----------------------------------------------------------------------------
//  Private data:
//-----------------------------------------------------------------------------
static BYTE const bufCmdOn		[] = "on";
static BYTE const bufCmdOff		[] = "off";
static BYTE const bufCmdFreq	[] = "freq";
static BYTE const bufCmdMeasImp	[] = "imp";
static BYTE const bufCmdMeasRaw	[] = "raw";
static BYTE const bufCmdScan	[] = "scan";
static BYTE const bufCmdScanRaw	[] = "scanr";
static BYTE const bufCmdScanBR	[] = "scanbr";
static BYTE const bufCmdScanV	[] = "scanv";
static BYTE const bufCmdSweep	[] = "sweep";

static BYTE const bufWelcome	[] = "\r\n" PRODUCT_NAME_STR " SWR Analyzer " VERSION_STR "\r\n";
static BYTE const bufCmdPrompt	[] = "\r\n>>";
static BYTE const bufWaitLink	[] = "Waiting Link";
static BYTE const bufErrCmdNotFound1[] = "\r\nCommand <";
static BYTE const bufErrCmdNotFound2[] = "> not found\r\n";

//-----------------------------------------------------------------------------
//  Prototypes
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  FUNCTION NAME:	PcLink
//	DESCRIPTION:	PC link routines for update measurements
//					UART Speed is VC3 / 8, e.g.
//					24MHz SysClk / 52 / 8 = 57692 (57700)
//  ARGUMENTS:		none.
//  RETURNS:		none.
//-----------------------------------------------------------------------------
void PcLink (void)
{
	char * strPtr; 						// Parameter pointer
	UART_CmdReset(); 					// Initialize receiver/cmd buffer
	UART_IntCntl(UART_ENABLE_RX_INT); 	// Enable RX interrupts
	UART_Start(UART_PARITY_NONE); 		// Enable UART
	M8C_EnableGInt ;

	// Clear the terminal screen & display welcome message
    UART_PutChar(12); UART_CPutString(bufWelcome); UART_CPutString(bufCmdPrompt);

	// Brief message for LCD then main loop
	Screen_CStrAtRowCol(1,0, bufWaitLink);
	while(TRUE) {
		if (KEYPAD_Get() == KBD_UP) break;
		if(UART_bCmdCheck()) 							// Wait for command
		{							
			if(strPtr = UART_szGetParam())
			{
				Screen_CStrAtRowCol(1,0, gBlankStr_16);
				Screen_StrAtRowCol(1,0, strPtr);
    			
				if(!cstrcmp((const char*)bufCmdOn,(char*)strPtr)) 				{Cmd_On();}
    			else if(!cstrcmp((const char*)bufCmdOff,(char*)strPtr))			{Cmd_Off();}
    			else if(!cstrcmp((const char*)bufCmdFreq,(char*)strPtr))		{Cmd_Freq();}
    			else if(!cstrcmp((const char*)bufCmdMeasImp,(char*)strPtr))		{pcLinkPrintToSerial(CMD, SRXZ);}
    			else if(!cstrcmp((const char*)bufCmdMeasRaw,(char*)strPtr))		{pcLinkPrintToSerial(CMD, VFVRVZVA);}
				else if(!cstrcmp((const char*)bufCmdScan,(char*)strPtr))		{pcLinkFreqScan(SCAN);}
    			else if(!cstrcmp((const char*)bufCmdScanRaw,(char*)strPtr))		{pcLinkFreqScan(SCANR);}
				else if(!cstrcmp((const char*)bufCmdScanBR,(char*)strPtr))		{pcLinkFreqScan(SCANBR);}					
				else if(!cstrcmp((const char*)bufCmdScanV,(char*)strPtr))		{pcLinkFreqScan(SCANV);}					
				else{ UART_CPutString(bufErrCmdNotFound1); UART_PutString(strPtr); UART_CPutString(bufErrCmdNotFound2);}
			}
			UART_CmdReset(); // Reset command buffer
			UART_CPutString(bufCmdPrompt);
		}
		Screen_CStrAtRowCol(0,11, "     ");
	}
	Set_DDS(0);
	UART_Stop();
}
