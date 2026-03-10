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
// 	FILE NAME: 	timers.c
// 	AUTHOR:		G1OJS - Alan Robinson
// 	DESCRIPTION	Display Screens for different modes and results
// 	HISTORY
//	NAME   		DATE		REMARKS
//	AJR			Feb 2025	G1OJS - creation
//*****************************************************************************/

#include "m8c.h"
#include "glb_data.h"



//-----------------------------------------------------------------------------
//  FUNCTION NAME: 	SleepTimerINT
//  DESCRIPTION:	Sleep timer interrupt sevice routine. Period 64Hz.
//  ARGUMENTS:		none.
//  RETURNS: 		none.
//-----------------------------------------------------------------------------

static volatile BYTE gDivider_1 = 1;	// Counter to divide clock by 8
static volatile BYTE gDivider_2 = 1;	// Counter to divide clock by 8
#pragma interrupt_handler 	SleepTimerINT
void SleepTimerINT ( void )
{
	M8C_ClearWDTAndSleep;
	
	if (g_bSixtyFourthSecondCounter) 	g_bSixtyFourthSecondCounter--;
	
	if( --gDivider_1 ) return;	// only pass here to decrement once per sixteenth second counters
		gDivider_1 = 4; 
		if (g_bMeasureCounter) 			g_bMeasureCounter--;
		if (g_bDebounceCounter) 		g_bDebounceCounter--;
		
	if( --gDivider_2 ) return;	// only pass here to decrement once per second counters
		gDivider_2 = 16; 		
		if (g_bLongPressKeyCounter) 		g_bLongPressKeyCounter-- ;
		if (g_bIdleCounter)    			g_bIdleCounter-- ;
}
//-----------------------------------------------------------------------------
//  FUNCTION NAME: 	Delay_64ths
//  DESCRIPTION: 	Implements a blocking delay of b64ths/64 seconds
//  ARGUMENTS:  	BYTE b64ths: 1= 16ms, 255= 3984ms
//  RETURNS:  		Nothing
//-----------------------------------------------------------------------------
void Delay_64ths(BYTE b64ths)			
{
	g_bSixtyFourthSecondCounter = b64ths;
	M8C_Sleep;
	while(1){ if (g_bSixtyFourthSecondCounter ==0) break; }
}

