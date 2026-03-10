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
// 	FILE NAME: 	screens.c
// 	AUTHOR:		G1OJS - Alan Robinson
// 	DESCRIPTION	Display Screens for different modes and results
// 	HISTORY
//	NAME   		DATE		REMARKS
//	AJR			Feb 2025	G1OJS - creation
//*****************************************************************************/
#include "screens.h"

//#include <stdlib.h>
#include <string.h>
#include <m8c.h>
#include "Lcd.h"
#include "control.h"
#include "Msg_generic.h"
#include "glb_data.h"
#include "keypad.h"
#include "timers.h"

char FormatNumberRJ(unsigned long dwVal, unsigned char bFieldLen, unsigned char bDP);
static void FormatNumberRJ_Sliding(unsigned long dwVal, unsigned char bFieldLen, unsigned char bDP);

#define LCD_CLEAR			0x01
#define LCD_ON_CURSOR		0x0e
#define LCD_ON_BLINK		0x0d

//-----------------------------------------------------------------------------
// Simpler one-line functions to pass through Clear display, Display on no cursor, write strings to LCD
//-----------------------------------------------------------------------------
void Screen_Clear(void) 													{ LCD_Control(LCD_CLEAR); 							}
void Screen_HideCursor(void) 												{ LCD_Control(LCD_CURSOR_OFF);								}
void Screen_CStr(const char * sRomString)									{ LCD_PrCString(sRomString);}
void Screen_Str(char * sString)												{ LCD_PrString(sString);	}
void Screen_CStrAtRowCol(BYTE bRow, BYTE bCol, const char * sRomString)		{ LCD_Position(bRow, bCol); LCD_PrCString(sRomString);}
void Screen_StrAtRowCol(BYTE bRow, BYTE bCol, char * sString)				{ LCD_Position(bRow, bCol); LCD_PrString(sString);	}


//-----------------------------------------------------------------------------
// Longer functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  FUNCTION NAME: 	Screen_Frequency
//  DESCRIPTION:	Display VFO frequency g_dwCurrHz in the current cursor position
//  ARGUMENTS:		none
//  RETURNS:		none.
//-----------------------------------------------------------------------------
void Screen_Frequency(SCREEN_FREQ_UNITS xDisplayRes)
{
	// display planner ...
	// 0123456789012345
	//        56295.350

	if(xDisplayRes == Display_Hz) FormatNumberRJ(g_dwCurrHz,9,3); else FormatNumberRJ(g_dwCurrHz/1000,5,0);
	Screen_StrAtRowCol(0, (xDisplayRes == Display_Hz)? 7:11 , g_buffer16);	
	
	// show the increment cursor unless in VFO power setting mode or 
	// a function calling here without using full Hz resolution (this is a 
	// shortcut in the logic & really needs separating into another argument)
	if((!g_bUP_DOWN_SelectsVFOPower) && (xDisplayRes==Display_Hz)){
		LCD_Position(0, g_xIncCtrl[g_bIncDigit].bCol);  
		if (g_bUP_DOWN_SelectsIncDigit) {LCD_Control(LCD_ON_BLINK);} else {LCD_Control(LCD_ON_CURSOR);}
	}
}
//-----------------------------------------------------------------------------
//  FUNCTION NAME: 	Screen_SRXZ()
//  DESCRIPTION:	Display current SWR, R, X and |Z|
//  ARGUMENTS: 		none
//  RETURNS: 		nothing
//-----------------------------------------------------------------------------
void Screen_SRXZ (void)
{
	// display planner ...
	// SWR0.0 51000.000
	// |999-j999|o=999o
	// 0123456789012345
	
	// display top row for current mode & clear bottom row
	Screen_CStrAtRowCol(0,0, gModeStr[g_bMode]);
	Screen_CStrAtRowCol(1, 0, gBlankStr_16);
	
	// SWR top left
	FormatNumberRJ_Sliding(((DWORD)g_wSwr100+5)/10, 3, 1);
	Screen_StrAtRowCol(0, 3, g_buffer16);
	// R bottom left
	Screen_CStrAtRowCol(1, 0, "|");
	FormatNumberRJ_Sliding((DWORD)g_wR10, 3, 1);
	Screen_Str(g_buffer16);
	// X bottom middle
	FormatNumberRJ_Sliding((DWORD)g_wX10, 3, 1);
	Screen_CStr("  ");			// placeholder for +j / -j
	Screen_Str(g_buffer16);
	Screen_CStr("| =");
	// Z bottom right
	FormatNumberRJ_Sliding((DWORD)g_wZ10, 3, 1);
	Screen_Str(g_buffer16);
	Screen_CStr("\xf4");
	
	// overwrite placeholder with +j / -j, moving one to the right for short numbers
	Screen_CStrAtRowCol(1, (g_wX10>=100 && g_wX10<1000)? 5:4, (g_bSgnX=='+')? "+j":"-j");
}


//-----------------------------------------------------------------------------
//  FUNCTION NAME: 	Screen_Capacitance
//  DESCRIPTION:	Calculate and display capacitance
//
//           C_pF	 =	10^9/(2*PI*freq_kHz * X_ohms)
//
//           10*C_pF =	100*10^9/(2*PI*freq_kHz * 10*X_ohms)
//
//			 		 = 	10^9 / ( ((freq_Hz/1000) * 10*X_ohms) / 100 ) 
//				   		* 1000	(10*pF, 10* ohms)				
//				   		/6282
// 	ARGUMENTS:		uses global values:
//						WORD g_wX10		10 x (X in ohms)
//						WORD g_wR10		10 x (R in ohms)
//  RETURNS: none.
//-----------------------------------------------------------------------------
void Screen_Capacitance (void)
{
	DWORD dw_pFx10;

	Screen_CStrAtRowCol(0,0, gModeStr[g_bMode]);				// display top row for current mode
	// clear bottom row
	Screen_CStrAtRowCol(1, 0, gBlankStr_16);
	if(g_wX10==0) {
		Screen_CStrAtRowCol(1, 0, gZeroReactanceStr);
	} else {
		dw_pFx10 =  g_dwCurrHz / 1000;
		dw_pFx10 =  ((DWORD)(dw_pFx10 * (DWORD)g_wX10)) / 100;
		dw_pFx10 = 	(DWORD)1000000000 / dw_pFx10;
		dw_pFx10 = 	(dw_pFx10*1000) / 6282;
		FormatNumberRJ_Sliding(dw_pFx10,4, 1);
		Screen_StrAtRowCol(1, 0, g_buffer16);
		Screen_CStr("pF ");
		FormatNumberRJ_Sliding(g_wR10,4 ,1);
		Screen_Str(g_buffer16);
		Screen_CStr("\xf4");
	}
}

//-----------------------------------------------------------------------------
//  FUNCTION NAME: 	Screen_Inductance
//  DESCRIPTION: 	Calculate and display inductance
//  ARGUMENTS:		uses global values:
//						WORD g_wX10	10 x (X in Ohms)
//						WORD g_wR10	10 x (R in Ohms)
//
//   	L uH 	=	10^6 * X / 2*PI*freq_kHz
//   	10*L uH	=	10^6 * (X*10) / 2*PI*freq_kHz
//				= 1000
//				  * X*10
//				  / 6282
//				  * 1000
//				  / (fHz/1000)
//
//  RETURNS:  nothing
//-----------------------------------------------------------------------------
void Screen_Inductance ( void )
{
	DWORD dwL_uH;
	dwL_uH 		=	1000 * (DWORD)g_wX10;
	dwL_uH		=	dwL_uH / 6282;
	dwL_uH		=	dwL_uH * 1000;
	dwL_uH		=	dwL_uH / ( g_dwCurrHz / 1000 );
	
	Screen_CStrAtRowCol(0,0, gModeStr[g_bMode]);// display top row for current mode
	Screen_CStrAtRowCol(1, 0, gBlankStr_16);	// clear bottom row
	FormatNumberRJ_Sliding(dwL_uH,4, 1);
	Screen_StrAtRowCol(1, 0, g_buffer16);
	Screen_CStr("uH ");
	FormatNumberRJ_Sliding(g_wR10,4, 1);
	Screen_Str(g_buffer16);
	Screen_CStr("\xf4");
}

//-----------------------------------------------------------------------------
//  FUNCTION NAME: 	Screen_Power
//  DESCRIPTION: 	Display power at measurement port
//					Uses mVRMS = Vz / CAL_Vz_TO_mVRMS  for mVRMS display
//						 dBm   = 20*(Vz - CAL_Vz_TO_dBm) / (Vz + CAL_Vz_TO_dBm)
//
//1 MHz					
//		dBm(RSP1a)	Vrms_true	Vz_ADC	Vz_ADC*1000=Vrms x		n		20*(VzADC-n)/(VzADC+n)	
//		-8.8		79.9		311		3893					750		-9	
//		0.0			220.0		771		3504					750		0	
//		6.8			481.3		1747	3629					750		7	
//							
//10MHz	
//		dBm(RSP1a)	Vrms_true	Vz_ADC	Vz_ADC*1000=Vrms x		n		20*(VzADC-n)/(VzADC+n)	
//		-11.7		57.2		208		3636					750		-12	
//		-0.1		217.5		709		3260					750		-1	
//		7.2			504.0		1677	3327					750		7	
//							
//								Average	3542					750		
//										= CAL_Vz_TO_mVRMS		=CAL_Vz_TO_dBm
//
//  ARGUMENTS:		WORD: Measured Vz
//  RETURNS:  		none
//-----------------------------------------------------------------------------

void Screen_Power(WORD wVz)
{	// display planner ...
	// 9999mV  +15dBm
	// 0123456789012345
	
	signed char dBm;
	Screen_CStrAtRowCol(0,0, gModeStr[g_bMode]);	// display top row for current mode
	Screen_CStrAtRowCol(1, 0, gBlankStr_16);		// clear bottom row
	
	#define CAL_Vz_TO_mVRMS 	3542
	FormatNumberRJ((INT)( ((LONG)wVz*wUNITY)/CAL_Vz_TO_mVRMS  ),4,0); 
	Screen_StrAtRowCol(1, 0, g_buffer16);
	Screen_CStr("mV");
	
	#define CAL_Vz_TO_dBm 	750
	#define LOG_TO_dB		20
	dBm = (signed char)( (((long)wVz-CAL_Vz_TO_dBm)*LOG_TO_dB) / (wVz+CAL_Vz_TO_dBm) ); 
	Screen_CStrAtRowCol(1, 8, (dBm<0)? "-":"+"); 
	FormatNumberRJ((WORD)abs(dBm),2,0); 
	Screen_Str(g_buffer16);
	Screen_CStr("dBm");
	
	Screen_CStrAtRowCol(1, 15, (g_bUP_DOWN_SelectsVFOPower)? "<":" ");

}

// Test floating point version of Screen_Power 
// Uses 2,367 BYTES more than integer version!
/*
void Screen_Power_f(WORD wVz)
{
	#include <math.h>
	#define CAL_Vz_TO_mVRMS 	1638
	float mVRMS = (float)wVz/CAL_Vz_TO_mVRMS;
	Screen_CStrAtRowCol(1,0, gPowerLabelsStr);
	itoa(g_buffer16,(INT)mVRMS,10); 
	Screen_StrAtRowCol(1, 3, g_buffer16);
	itoa(g_buffer16,(INT)(8.6859*log(mVRMS)-46.9897) ,10); 
	Screen_StrAtRowCol(1, 12, g_buffer16);
}
*/

void Screen_CableLength(WORD wLencm)
{	
	// display planner ...
	// Cable len, cm:
	// VF81:  201.25m
	// 0123456789012345
	BYTE bCnt;
	Screen_HideCursor();
	Screen_Clear();
	Screen_CStrAtRowCol(0, 0, gCableLabels1Str);
	Screen_CStrAtRowCol(1, 0, "VF");
	FormatNumberRJ(g_bVF[g_xConf.bVF],2,0); Screen_StrAtRowCol(1, 2,  g_buffer16);
	Screen_CStr(": ");
	FormatNumberRJ(wLencm,5,2); Screen_StrAtRowCol(1, 7,  g_buffer16);
	Screen_CStr("m");
	
}

static void FormatNumberRJ_Sliding(unsigned long dwVal, unsigned char bFieldLen, unsigned char bDP)
{
   char index;
   if(bDP==1 && dwVal >=100)  {dwVal = (5 + dwVal)/10; bDP -=1;};
   if(bDP==2 && dwVal >=1000) {dwVal = (5 + dwVal)/10; bDP -=1;};
   if(bDP==1 && dwVal >=1000) {dwVal = (5 + dwVal)/10; bDP -=1;};
   if(!FormatNumberRJ(dwVal, bFieldLen, bDP)) {
       for(index=0;index<bFieldLen;index++) g_buffer16[index]='*'; 
   }
}
