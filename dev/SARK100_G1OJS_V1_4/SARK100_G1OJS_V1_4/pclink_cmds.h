//*****************************************************************************/
//  This file is a part of the "SARK100 SWR Analyzer firmware"
//
//	Copyright © 2025 Alan Robinson G1OJS Hampshire England G1OJS@yahoo.com with 
//  acknowledgement & thanks to Melchor Varela © 2010, EA4FRB Madrid, Spain 
//	(melchor.varela@gmail.com) for the overall software and ideas.
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
// 	FILE NAME: 	pclink_cmds.h
// 	AUTHOR:		G1OJS - Alan Robinson
// 	DESCRIPTION	pclink commands headers
// 	HISTORY
//	NAME   		DATE		REMARKS	
//	AJR			FEB 2025	G1OJS  - creation 
//*****************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "control.h"
#include "keypad.h"
#include "UART.h"
#include "screens.h"
#include "msg_generic.h"
#include "glb_data.h"
#include "bridge.h"
#include "morse.h"
#include "derive.h"
#include "correctionmodel.h"
#include "timers.h"

typedef enum {SRXZ, VFVRVZVA} PRINT_WHAT;
typedef enum {CMD, SCAN, SCANR, SCANBR, SCANV} MEASMODE;

void Cmd_On (void);
void Cmd_Off (void);
void Cmd_Freq (void);
void pcLinkFreqScan (MEASMODE mMode);
void pcLinkPrintToSerial (MEASMODE mMode, PRINT_WHAT pwPrintWhat);
void PrintWdToSerial(WORD wVal, BYTE bDecimals, const BYTE * termStr);
DWORD getFreqArg(void);


