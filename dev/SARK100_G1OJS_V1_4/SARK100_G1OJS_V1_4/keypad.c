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
//	PROJECT:	PSoC AntennaAnalyzer
// 	FILE NAME: 	KEYPAD.C
// 	AUTHOR:		Melchor Varela
// 	DESCRIPTION: Keypad driver
// 	HISTORY
//	NAME   		DATE		REMARKS
//	MVM	   		DEC 2009	Creation
//  AJR			JAN 2025	Added comments and changed function names to 
//							be more descriptive.
//*****************************************************************************/
#include "keypad.h"

#include <m8c.h>        				// Part specific constants and macros
#include "PSoCAPI.h"    				// PSoC API definitions for all User Modules
#include "psocgpioint.h"
#include "glb_data.h"
#include "morse.h"
#include "control.h"
#include "timers.h"


//-----------------------------------------------------------------------------
//  FUNCTION NAME:	KEYPAD_Get
//  DESCRIPTION:	Get key pressed value
//  ARGUMENTS: 		none.
//  RETURNS: 		Key pressed. Zero if no key.
//-----------------------------------------------------------------------------
BYTE KEYPAD_Get ( void )
{
	BYTE bKey;
	static BYTE bLastKey = 0;
	BYTE bSpeedAllowed=0;

	if (g_bDebounceCounter!=0) return 0;	// in the debounce window, return no press

	bKey = KEYPAD_Scan();
	if (bKey != KEYPAD_Scan()) bKey = 0;	// error nulling

	if (bKey) {
		Morse_Dit();
		g_bDebounceCounter = KEY_DEBOUNCE_TIME;
		if (bKey==bLastKey) {									// Key is held down
			if (g_bLongPressKeyCounter == 0) { 					// If counter hits zero, key is held down longer than LONG_PRESS_DET_TIME_S
				if((bKey == KBD_UP) || (bKey == KBD_DWN)) {		// Long press for UP/DOWN (and only UP/DOWN) means go fast, so:
					g_bDebounceCounter = 0;						// hold debounce counter at zero (this provides the speed increase)
					Delay_64ths(SPEED_KEY_CLICKRATE_64ths);		// but introduce a reasonable delay
				}
				g_bLongPress = 1;								// set global variable to indicate long press
			} 	
		} else {
			g_bLongPressKeyCounter = LONG_PRESS_DET_TIME_S;		// long press counter resets only if key is not held down
			bLastKey = bKey;
			g_bLongPress = 0; 									// turn off long press flag when key released
		}
	} else bLastKey = 0;
	
	
	
	return bKey;
}

//-----------------------------------------------------------------------------
//  FUNCTION NAME:		KEYPAD_WaitKey
//  DESCRIPTION:		Waits for key or delay.
//						Implements power saving features
//	ARGUMENTS: 			bDelayS	Wait timeout in seconds
//  RETURNS:			Key pressed.
//-----------------------------------------------------------------------------
BYTE KEYPAD_WaitKey ( BYTE bDelayS )
{
	BYTE bKey;
										// Actions to minimize power consumption
	OSC_CR0 &= ~0x07; 					// Clear Bits 0 to 2
	OSC_CR0 |= OSC_CR0_CPU_750kHz; 		// Sets CPU clock to 750Khz
	g_bIdleCounter = bDelayS;
	do
	{
		M8C_Sleep;
		bKey = KEYPAD_Get();
		if (bDelayS)
		{
			if (g_bIdleCounter==0)
				break;
		}
	} while (bKey == 0);
	OSC_CR0 &= ~0x07; 					// Clear Bits 0 to 2
	OSC_CR0 |= OSC_CR0_CPU_24MHz;  		// Set CPU Clock to SysClk/1

	return bKey;
}

//-----------------------------------------------------------------------------
//  FUNCTION NAME:	KEYPAD_Scan
//  DESCRIPTION:	Scans keypad
//  ARGUMENTS:		none.
//  RETURNS:		Key pressed. Zero if no key.
//-----------------------------------------------------------------------------
BYTE KEYPAD_Scan ( void )
{
	BYTE bKey = 0;

	COL0_Data_ADDR |= COL0_MASK;
	COL1_Data_ADDR |= COL1_MASK;
	COL2_Data_ADDR |= COL2_MASK;
	
	COL1_Data_ADDR &= ~COL1_MASK;
	if ( (ROW0_Data_ADDR & ROW0_MASK) == 0 ) bKey = KBD_SCAN;
	if ( (ROW1_Data_ADDR & ROW1_MASK) == 0 ) bKey = KBD_UP;
	COL1_Data_ADDR |= COL1_MASK;

	COL0_Data_ADDR &= ~COL0_MASK;
	if ( (ROW0_Data_ADDR & ROW0_MASK) == 0 ) bKey = KBD_CONFIG;
	if ( (ROW1_Data_ADDR & ROW1_MASK) == 0 ) bKey = KBD_BAND;
	COL0_Data_ADDR |= COL0_MASK;

	COL2_Data_ADDR &= ~COL2_MASK;
	if ( (ROW0_Data_ADDR & ROW0_MASK) == 0 ) bKey = KBD_MODE;
	if ( (ROW1_Data_ADDR & ROW1_MASK) == 0 ) bKey = (bKey==KBD_UP)?   KBD_UP_DWN : KBD_DWN ;
	COL2_Data_ADDR |= COL2_MASK;

	return bKey;
}

//-----------------------------------------------------------------------------
//  FUNCTION NAME:	KEYPAD_SysSuspendAndWakeFromPress
//  DESCRIPTION:	System suspend. Wakes up from a key press
//  ARGUMENTS: 		none.
//  RETURNS: 		Key pressed. Zero if no key.
//-----------------------------------------------------------------------------
void KEYPAD_SysSuspendAndWakeFromPress ( void )
{
	BYTE bSave_ARF_CR;
	BYTE bSave_ABF_CR0;
	M8C_DisableGInt;
	// Activate all columns
	COL0_Data_ADDR &= ~COL0_MASK;
	COL1_Data_ADDR &= ~COL1_MASK;
	COL2_Data_ADDR &= ~COL2_MASK;
	// Reduce analog power
	bSave_ARF_CR = ARF_CR;
	bSave_ABF_CR0 = ABF_CR0;
	ARF_CR &= 0xf8; 		// analog blocks Off
	ABF_CR0 &= 0xc3;	 	// analog buffer off
	// Set low level active interrupt
	PRT1IC0	|= 0x3;
	PRT1IC1	&= ~0x3;
	PRT1IE	|= 0x3;			// Enables row0&row1 interrupt
	// Disables sleep interrupt
	M8C_DisableIntMask(INT_MSK0, INT_MSK0_SLEEP);
	// Enables GPIO interrupt
	M8C_EnableIntMask(INT_MSK0, INT_MSK0_GPIO);
	INT_CLR0 &= 0x20;		// Clear Pending GPIO Interrupt
	M8C_EnableGInt;
	M8C_Sleep;				// Goes sleep
	INT_VC = 0;							// Erases vector
	PRT1IE	&= ~(0x3);		// Disables row0&row1 interrupt					
	M8C_DisableIntMask(INT_MSK0, INT_MSK0_GPIO);	// Disables GPIO interrupt			
	M8C_EnableIntMask(INT_MSK0, INT_MSK0_SLEEP);	// Restores sleep interrupt
	ARF_CR = bSave_ARF_CR;
	ABF_CR0 = bSave_ABF_CR0;
}