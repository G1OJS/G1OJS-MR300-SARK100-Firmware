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
// 	FILE NAME: 	control.c
// 	AUTHOR:		G1OJS - Alan Robinson
// 	DESCRIPTION	Set control lines and gain settings
// 	HISTORY
//	NAME   		DATE		REMARKS
//	AJR			Feb 2025	G1OJS - creation
//*****************************************************************************/
#include "control.h"

#include <m8c.h>        		// Part specific constants and macros
#include "PSoCAPI.h"    		// PSoC API definitions for all User Modules
#include "psocgpioint.h"
#include "glb_data.h"
#include "string.h"
#include "screens.h"
#include "Dds.h"
#include "msg_generic.h"
#include "keypad.h"
#include "morse.h"
#include "timers.h"
//-----------------------------------------------------------------------------
//  FUNCTION NAME: 	debug_IntToLCD
//  DESCRIPTION: 	Clear lcd and write the two arguments to the display
//  ARGUMENTS:  	two integers to display
//  RETURNS:  		Nothing
//-----------------------------------------------------------------------------
//void debug_IntToLCD(int iVal1, int iVal2)
//{
//	Screen_Clear();
//	itoa(g_buffer16,iVal1,10); Screen_StrAtRowCol(0, 0, g_buffer16);
//	itoa(g_buffer16,iVal2,10); Screen_StrAtRowCol(1, 0, g_buffer16);
//	Delay_64ths(TIME_FLASH_MSG);
//}

void Set_DDS(DWORD dwFreq) {DDS_Set(dwFreq);}

//-----------------------------------------------------------------------------
//  FUNCTION NAME:	SetPowerDigitalAndAnalogue
//  DESCRIPTION:	Turn on LCD, Backlight and 30MHz osc, ADC, ADC's PGAs
//					DDS, DDS's PGAs, Set DDS Gain, Set ADC PGA Gain
//					OR: if isON is false, turn all of that off
//  ARGUMENTS:		POWER_STATE pwrState
//  RETURNS:		Nothing
//-----------------------------------------------------------------------------
void SetPowerDigitalAndAnalogue(POWER_STATE pwrState)
{
    if(pwrState==PWRON)
	{
	  	LCD_Init(); LCD_Control(LCD_DISP_ON);

      	SetPowerBackLightAndOsc(PWRON);
	 	Delay_64ths(1);
	  	SetPowerBackLightAndOsc(PWROFF);
	  	Delay_64ths(1);
	  	SetPowerBackLightAndOsc(PWRON);
	  	Delay_64ths(1);
	
 	  	ADCINC12_Start(ADCINC12_HIGHPOWER); 
	  	PGA_ADC_Start(PGA_ADC_HIGHPOWER);
		PGA_ADC_SetGain(PGA_ADC_G2_67);
		DDS_Init();
		PGA_DDS_1_Start(PGA_DDS_1_HIGHPOWER);	
		PGA_DDS_2_Start(PGA_DDS_2_HIGHPOWER);
		SetDDSGain();
	} else {
   	  	SetPowerBackLightAndOsc(PWROFF);
	  	Set_DDS(0);
		PGA_DDS_1_Stop();
		PGA_DDS_2_Stop();		
	  	ADCINC12_Stop(); 
		PGA_ADC_Stop();
	  	LCD_Init(); LCD_Control(LCD_DISP_OFF);
	}
}
//-----------------------------------------------------------------------------
//  FUNCTION NAME:	SetDDSGain
//  DESCRIPTION:	Set the gain of both PGAs controlling the DDS output
//					to the setting specified by g_bDDS_GainStep and the DDS PGA
//					gain settings table
//  ARGUMENTS:		Nothing
//  RETURNS:		Nothing
//-----------------------------------------------------------------------------
void SetDDSGain(void)
{
	PGA_DDS_1_SetGain(g_xGainDds[g_bDDS_GainStep].bGain1);
	PGA_DDS_2_SetGain(g_xGainDds[g_bDDS_GainStep].bGain2);
}
//-----------------------------------------------------------------------------
//  FUNCTION NAME:	SetPowerBackLightAndOsc
//  DESCRIPTION:	Assert XO_EN which powers the backlight and 30MHz osc
//					OR: if isON is false, off
//  ARGUMENTS:		POWER_STATE pwrState
//  RETURNS:		Nothing
//-----------------------------------------------------------------------------
void SetPowerBackLightAndOsc(POWER_STATE pwrState)
{
    if(pwrState==PWRON) {
		Port_2_Data_SHADE |= XO_EN_MASK;
		XO_EN_Data_ADDR |= XO_EN_MASK;
	}  else  {
		XO_EN_Data_ADDR &= ~XO_EN_MASK;
		Port_2_Data_SHADE &= ~XO_EN_MASK;
	}
}




