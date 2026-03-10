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
//	PROJECT:	SARK100 SWR Analyzer
// 	FILE NAME: 	Main.c
// 	AUTHOR:		G1OJS - Alan Robinson
// 	DESCRIPTION	Main program logic
// 	HISTORY
//	NAME   		DATE		REMARKS	
//	AJR			FEB 2025	G1OJS  - creation 
//*****************************************************************************/
//#include <stdlib.h>
#include <m8c.h>
#include "control.h"
#include "screens.h"
#include "glb_data.h"
#include "msg_generic.h"
#include "storage.h"
#include "keypad.h"
#include "morse.h"
#include "correctionmodel.h"
#include "pclink.h"
#include "bridge.h"
#include "timers.h"
#include "scans.h"
#include "derive.h"
#include "string.h"

//-----------------------------------------------------------------------------
//  Defines
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  Prototypes
//-----------------------------------------------------------------------------

static void Do_Config_Menu (void);
static void Do_DizzlingMeasureCorrectCalc(void);
static void Load_Firmware(void);
static void Config_ByteSettingItem(const char **sSettingStrings, BYTE *bSetting, BYTE bMaxValue);
static void CycleBand(void);
static void ChangeCurrHz(STEP_DIRECTION xDir);
static void cycleParameter(BYTE *bSetting, BYTE bMinValue, BYTE bMaxValue, STEP_DIRECTION xDir, BYTE bCycle);
static void handleKeypress(BYTE bKey);
static void PowerSave(void);
extern void MeasureCorrectCalc(void);
extern BYTE getUserConfirmation(void);
extern char FormatNumberRJ(unsigned long dwVal, unsigned char bFieldLen, unsigned char bDP);
//-----------------------------------------------------------------------------
//  MAIN
//-----------------------------------------------------------------------------
void main(void)
{
	BYTE bKey;

	M8C_ClearWDTAndSleep;		
	M8C_EnableIntMask(INT_MSK0, INT_MSK0_SLEEP);
	M8C_EnableGInt;					
	LCD_Start();						// Initialise display
	
	SetPowerDigitalAndAnalogue(PWRON);
	
	// Display welcome screen
	Screen_CStrAtRowCol(0,0, g_welcome1Str);
	Screen_CStrAtRowCol(1,0, g_welcome2Str);
	Delay_64ths(TIME_FLASH_MSG);
	
	//set initial band (scope to store band initial freqs in EEPROM - make g_wBandInitialFreq var not const)
	g_bBandIndex = BAND_ATPWRON;
	g_dwCurrHz=BAND_FREQ_ToHz * g_wBandInitialFreq[g_bBandIndex];	// current frequency in Hz
	
	// Check Vf level
	if (wRead_ADC_DDSON(VfPort)<500)	{
		Morse_Err();
		Screen_CStrAtRowCol(0,0, gErrorAdjustVfStr);
		Screen_CStrAtRowCol(1,0, gPressAnyKeyStr);
		KEYPAD_WaitKey(TIME_WAIT_KEY_S);	// Important, so wait for key press
	}

	STR_Restore();					// Load cal and config data from EEPROM, with warning message if not calibrated
	SetDDSGain();					// it was set at power on but now STR_Restore() has given us a new setting
	
	// clear screen and start main loop
	Screen_Clear();
	g_bIdleCounter = g_bUserIdle[g_xConf.bUserIdle];			// Initial set Idle counter
	do {
		if((g_bMode==MODE_SIG) || (g_bMode==MODE_VFO)) {
			Screen_Power(wRead_ADC_DDSUNCHANGED(VzPort));	
		} else {
			Do_DizzlingMeasureCorrectCalc();
			if(g_bMode==MODE_IMP) {Screen_SRXZ();}
			else if(g_bMode==MODE_CAP) {Screen_Capacitance();}
			else if(g_bMode==MODE_IND) {Screen_Inductance();}
		}
		if(g_bMode!=MODE_SIG) Screen_Frequency(Display_Hz);							

		// wait here until key pressed or measurement timer expires
		g_bMeasureCounter = MEASURE_PERIOD;		
		do {	
			if ((g_bIdleCounter==0) && (g_bUserIdle[g_xConf.bUserIdle] != 0)) PowerSave();
			if (g_bMeasureCounter==0) break;	
			bKey = KEYPAD_Get();
		} while(!bKey);

		if (bKey) handleKeypress(bKey);
		
	} while (TRUE);

}

//-----------------------------------------------------------------------------
//  Main's functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  FUNCTION NAME:	handleKeypress()
//  DESCRIPTION:	Function to do actions depending on key pressed
//  ARGUMENTS:		BYTE bKey - key pressed code
//  RETURNS: 		Nothing
//-----------------------------------------------------------------------------
static void handleKeypress(BYTE bKey){

	g_bIdleCounter = g_bUserIdle[g_xConf.bUserIdle];

	if(bKey == KBD_CONFIG)  {
		if (g_bUP_DOWN_SelectsIncDigit) { g_bUP_DOWN_SelectsIncDigit = 0; }			// Config turns off inc control digit select if it is on
		else if (g_bMode == MODE_VFO) 	{ g_bUP_DOWN_SelectsVFOPower = ~g_bUP_DOWN_SelectsVFOPower;}	// If in VFO mode, config key toggles power setting & monitor options
		else {Do_Config_Menu();}
	}
	
	if((bKey == KBD_SCAN) && (g_bMode==MODE_IMP)) Do_VSWR_Scan(); 
	
	if(bKey == KBD_MODE)    {
		Screen_Clear();
		g_bUP_DOWN_SelectsIncDigit  = FALSE;			// clear the "alt use of arrow keys" flags
		g_bUP_DOWN_SelectsVFOPower  = FALSE;
		cycleParameter(&g_bMode, 0, MODE_NUM-1, STEPUP, TRUE); 			// cycle to next MODE
		if (g_bMode == MODE_VFO) Set_DDS(g_dwCurrHz);		// if going into VFO mode, turn on DDS
	}
			
	if((bKey == KBD_UP_DWN) && (g_bMode != MODE_SIG)) g_bUP_DOWN_SelectsIncDigit = ~g_bUP_DOWN_SelectsIncDigit;    // Toggle between cursor or frequency change modes
	if((bKey == KBD_BAND) && (g_bMode != MODE_SIG)) CycleBand(); 

	if((bKey==KBD_UP) && (g_bMode != MODE_SIG)){								
		if (g_bUP_DOWN_SelectsIncDigit) {cycleParameter(&g_bIncDigit, 0,  NUM_INCREMENT-1, STEPUP, TRUE);} 		// Move frequency cursor left & cycle back to right
		else if (g_bUP_DOWN_SelectsVFOPower) { cycleParameter(&g_bDDS_GainStep, 0, GAIN_SETTINGS_NUM-1, STEPUP, FALSE); SetDDSGain();}	// increase DDS level
		else ChangeCurrHz(STEPUP); 
	}
	if((bKey==KBD_DWN) && (g_bMode != MODE_SIG)){								
		if (g_bUP_DOWN_SelectsIncDigit) {cycleParameter(&g_bIncDigit, 0, NUM_INCREMENT-1, STEPDOWN, TRUE); }  	// Move frequency cursor right & cycle back to left
		else if (g_bUP_DOWN_SelectsVFOPower) {cycleParameter(&g_bDDS_GainStep, 0, GAIN_SETTINGS_NUM-1, STEPDOWN, FALSE); SetDDSGain(); }	// decrease DDS level
		else ChangeCurrHz(STEPDOWN); 
	}

	// speedkeys: if not SIG mode, and UP or DOWN have long press, enter and stay in the loop below until key released
	// note - could also use g_bLongPressKeyCounter in place of g_bLongPress (opposite logic) & delete g_bLongPress 
	if( ((bKey == KBD_UP) || (bKey == KBD_DWN)) && (g_bLongPress) && (g_bMode != MODE_SIG) && !g_bUP_DOWN_SelectsIncDigit && !g_bUP_DOWN_SelectsVFOPower){ 
		Screen_Clear();
		while (g_bLongPress && bKey){
			bKey = KEYPAD_Get();
			if( bKey == KBD_UP	) { ChangeCurrHz(STEPUP);  }
			if( bKey == KBD_DWN ) { ChangeCurrHz(STEPDOWN);}
		}
	}

}
//-----------------------------------------------------------------------------
//  FUNCTION NAME:	PowerSave
//  DESCRIPTION:  	Turn off / reduce power and wait in 
//					KEYPAD_SysSuspendAndWakeFromPress until key pressed
//					then restore power
//  ARGUMENTS:		none
//  RETURNS:		Nothing
//-----------------------------------------------------------------------------
static void PowerSave(void){
	SetPowerDigitalAndAnalogue(PWROFF);
	KEYPAD_SysSuspendAndWakeFromPress();
	SetPowerDigitalAndAnalogue(PWRON);
	g_bIdleCounter = g_bUserIdle[g_xConf.bUserIdle];
}
//-----------------------------------------------------------------------------
//  FUNCTION NAME:	CycleBand
//  DESCRIPTION:  	Cycle up through bands then back to first band
//  ARGUMENTS:		none
//  RETURNS:		Nothing
//-----------------------------------------------------------------------------
static void CycleBand(void){
	cycleParameter(&g_bBandIndex, 0, BAND_NUM-1, STEPUP, TRUE);
	g_dwCurrHz = g_wBandInitialFreq[g_bBandIndex] * BAND_FREQ_ToHz ;
	if(!g_bBandIndex) Morse_Dah();
	// In VFO mode we don't call Do_DizzlingMeasureCorrectCalc, so we need to set frequency here
	if(g_bMode==MODE_VFO) Set_DDS(g_dwCurrHz);
}
//-----------------------------------------------------------------------------
//  FUNCTION NAME:	ChangeCurrHz
//  DESCRIPTION:  	Change frequency by step g_xIncCtrl[g_bIncDigit].dwInc Hz
//					Does not move frequency outside current band
//
//  ARGUMENTS:		xDir (STEPUP|STEPDOWN)
//  RETURNS:		Nothing
//-----------------------------------------------------------------------------
static void ChangeCurrHz(STEP_DIRECTION xDir){
	DWORD dwNewHz = (xDir == STEPUP)? (g_dwCurrHz + g_xIncCtrl[g_bIncDigit].dwInc):(g_dwCurrHz - g_xIncCtrl[g_bIncDigit].dwInc);
	if(    (dwNewHz < g_wBandBoundaries[g_bBandIndex] * BAND_FREQ_ToHz ) 
		|| (dwNewHz > g_wBandBoundaries[g_bBandIndex+1] * BAND_FREQ_ToHz ) ) return;	
	g_dwCurrHz = dwNewHz;
	// In VFO mode we don't call Do_DizzlingMeasureCorrectCalc, so we need to set frequency here
	if(g_bMode==MODE_VFO) Set_DDS(g_dwCurrHz);
	if(g_bLongPress) Screen_Frequency(Display_Hz);	// fast frequency change doesn't pass through frequency display so update that here
}

//-----------------------------------------------------------------------------
//  FUNCTION NAME:	cycleParameter
//  DESCRIPTION:  	Function to increment or decrement a parameter until it reaches 
//					its max value or min value. If the parameter hits max||min and 
//					bCycle is true, the parameter cycles round to min||max
//  ARGUMENTS:		*bSetting - pointer to the parameter to change
//					BbMinValue, bMaxValue, xDir (STEPUP|STEPDOWN), bCycle
//  RETURNS:		Nothing
//-----------------------------------------------------------------------------
 
static void cycleParameter(BYTE *bSetting, BYTE bMinValue, BYTE bMaxValue, STEP_DIRECTION xDir, BYTE bCycle)
{
	if(xDir == STEPDOWN) {
		if ((--*bSetting+1) < (bMinValue+1)) *bSetting = (bCycle)? bMaxValue:bMinValue; 
	} else {
		if (++*bSetting > bMaxValue) *bSetting = (bCycle)? bMinValue:bMaxValue; 
	}
}

//-----------------------------------------------------------------------------
//  FUNCTION NAME:	Do_DizzlingMeasureCorrectCalc
//  DESCRIPTION:  	Briefly step up in frequency & measure reactance, then return
//					to current frequency and measure again (both measurements also
//					give full set of VSWR, |Z|, R and X). If reactance increased 
//					with frequency, set sign of reactance to +ve else -ve, except
//					if the change is small, leave sign of reactance as it is.
//  ARGUMENTS:		none
//  RETURNS:		Nothing
//-----------------------------------------------------------------------------
static void Do_DizzlingMeasureCorrectCalc(void)
{
	DWORD dwDizzlingMemoryHz 	= g_dwCurrHz;
	WORD wX10_DizzledReactance;
	g_dwCurrHz = g_dwCurrHz*(100+DIZZLING_OFFSET_PERCENT) / 100;	// change to dizzled frequency
	MeasureCorrectCalc();					// find reactance (incl full set) at dizzling frequency
	wX10_DizzledReactance 	= g_wX10;
	g_dwCurrHz=dwDizzlingMemoryHz;			// change back to non-dizzled frequency
	MeasureCorrectCalc();					// measure full set at non-dizzled frequency
	if ( (int)(wX10_DizzledReactance - g_wX10) > 5 || (g_wX10==0) ) 
		{g_bSgnX = '+';}
	else if ( (int)(g_wX10-wX10_DizzledReactance) > 5) 
		{g_bSgnX = '-';}
}

//-----------------------------------------------------------------------------
//  FUNCTION NAME:	Do_Config_Menu
//  DESCRIPTION: 	Configuration routine
//  ARGUMENTS: 		none.
//  RETURNS: 		none.
//-----------------------------------------------------------------------------
static void Do_Config_Menu (void)
{
	BYTE bMenu = 0;
	BYTE bKey;
	DWORD dwSaveHz;

	Screen_Clear(); Screen_HideCursor();
	
	do  {	
		Screen_CStrAtRowCol(0, 0, gBlankStr_16);
		Screen_CStrAtRowCol(0, 0, gConfigStr[bMenu]);
		bKey = KEYPAD_WaitKey(TIME_WAIT_KEY_S);
		if (bKey==KBD_UP) 		break;
		if (bKey==KBD_CONFIG) 	cycleParameter(&bMenu, 0, CONFIG_NUM-1, STEPUP, TRUE);	
		if (bKey==KBD_DWN) {
			if (bMenu==CONFIG_PCLINK) 		{ dwSaveHz=g_dwCurrHz; PcLink(); g_dwCurrHz=dwSaveHz; break; }
			if (bMenu==CONFIG_Idle) 		{ Config_ByteSettingItem(gIdleStr, &g_xConf.bUserIdle, USER_IDLE_NUM-1); break;}
			if (bMenu==CONFIG_FltWt) 		{ Config_ByteSettingItem(gFltWtStr, &g_xConf.bFltWt, FLT_WT_NUM-1); break;}
			if (bMenu==CONFIG_CWPitch) 		{ Config_ByteSettingItem(gCWPitchStr, &g_xConf.bCWPitch, CWPitch_NUM-1); break;}
			if (bMenu==CONFIG_VF) 			{ Config_ByteSettingItem(gVFStr, &g_xConf.bVF, VF_NUM); break;}
			if (bMenu==CONFIG_CALIB) 		{ MeasureUncorrectedRatios();	break;	}	
			if (bMenu==CONFIG_SW_LOAD) 		{ Load_Firmware(); break; }
		}
		if ((bKey==KBD_DWN) || (bKey == KBD_SCAN)) { 
			if(bMenu==CONFIG_CableLength)	{ Do_LineLength_Scan(); break;}	
		}
	} while (TRUE);
	
	Screen_Clear();
}
//-----------------------------------------------------------------------------
//  FUNCTION NAME:	Config_ByteSettingItem
//
//  DESCRIPTION:	On keypress 'config' steps through the values of a parameter
//					defined by a BYTE setting. At any point, pressing UP cancels 
//					the operation, pressing down confirms saving the displayed value.
//  ARGUMENTS:
//      const char **sSettingStrings, pointer to the array of pointers to the setting 
//									  strings for the parameter from glb_data.h
//		BYTE *bSetting,	pointer to the parameter to be set
//		BYTE bMaxValue, maximum value at which the value returns to zero
//
//  RETURNS: none.
//-----------------------------------------------------------------------------
static void Config_ByteSettingItem(const char **sSettingStrings, BYTE *bSetting, BYTE bMaxValue)
{
	BYTE bCurrValue = *bSetting;
	BYTE bKey;
	Screen_CStrAtRowCol(1, 0, sSettingStrings[bCurrValue]);
	do {
		Screen_CStrAtRowCol(1, 0, sSettingStrings[bCurrValue]);
		Screen_CStr(gBlankStr_4);
		bKey = KEYPAD_WaitKey(TIME_WAIT_KEY_S);
		if (bKey==KBD_CONFIG) 	cycleParameter(&bCurrValue, 0, bMaxValue, STEPUP, TRUE);
		if (bKey==KBD_UP) 		break;
		if (bKey==KBD_DWN){
			*bSetting = bCurrValue;
			STR_SaveConfig();
			Screen_CStrAtRowCol(1, 8, gValueSavedStr);
			Delay_64ths(TIME_FLASH_MSG);
			return;
		}
	} while(TRUE);
	
	Screen_CStrAtRowCol(1, 0, gNothingChangedStr);
	Delay_64ths(TIME_FLASH_MSG);
}

//-----------------------------------------------------------------------------
//  FUNCTION NAME:	Load_Firmware()
//  DESCRIPTION:	Function to handle the load firmware branch of the config menu
//  ARGUMENTS:		nont
//  RETURNS: 		TRUE if select KBD_DWN, FALSE other key or timeout
//-----------------------------------------------------------------------------
static void Load_Firmware(void)
{
	if(getUserConfirmation()) {
		Screen_Clear(); Screen_CStrAtRowCol(0, 0, gSwLoadingStr);
		M8C_DisableGInt;
		asm ("ljmp 0x0000"); 	// Perform Software Reset
	}
}

//-----------------------------------------------------------------------------
//  FUNCTION NAME:	getUserConfirmation()
//  DESCRIPTION:	Function to display confirmStr in second row and then wait 
//					for confirm key or timeout
//  ARGUMENTS:		none
//  RETURNS: 		TRUE if select KBD_DWN, FALSE other key or timeout
//-----------------------------------------------------------------------------
BYTE getUserConfirmation(void)
{
	Screen_HideCursor();
	Screen_CStrAtRowCol(1, 0, gConfirmStr);
	return (KEYPAD_WaitKey(TIME_WAIT_KEY_S) == KBD_DWN);
}

//-----------------------------------------------------------------------------
//  FUNCTION NAME:	MeasureCorrectCalc()
//  DESCRIPTION: 	Measure, do correction, do calcs
//  ARGUMENTS: 		none
//  RETURNS: 		none
//-----------------------------------------------------------------------------
void MeasureCorrectCalc(void)
{	
	MeasureBrgRts();
	CorrectBrgRts();
	Do_SZRX_Calcs();
}

//-----------------------------------------------------------------------------
//  FUNCTION NAME:	FormatNumberRJ
//  DESCRIPTION: 	Bespoke ultoa eliminates library calls & saves 700 bytes ROM
//                  Results are right-justified if bFieldLen > len needed.
//  ARGUMENTS: 		dwVal 		- number to convert
//					bFieldLen	- number of digits (including the decimal point)
//                                beyond which **** is the result
//					bDP		 	- number of decimal places
//                              - FIXED - fixes the number of decimal places
//  RETURNS: 		true        - OK
//                  false       - Overrange (digits won't fit in bFieldLen)
//-----------------------------------------------------------------------------
char FormatNumberRJ(unsigned long dwVal, unsigned char bFieldLen, unsigned char bDP)
{
	// sequentially extract the digits into the buffer, inserting the 
	// decimal point at the appropriate place, then terminate the string
    // Convert number to string with correct decimal placement
    unsigned char index=0;
    while (index < bFieldLen ) {  
        g_buffer16[bFieldLen-1-index++] = '0' + (dwVal % 10);
        if(index==bDP) g_buffer16[bFieldLen-1-index++]='.';
        dwVal /= 10;
    }
    g_buffer16[bFieldLen]='\0';
    // if dwVal still has a value to encode,  return false
  	if(dwVal>0) return 0;
    // tidy up any unnecessary leading zeros (right justify so don't move digits)
	index=0;
	while (g_buffer16[index]=='0' && g_buffer16[index+1]!='.' && g_buffer16[index+1]!='\0') {g_buffer16[index++]=' ';}
	
	return 1;
}


