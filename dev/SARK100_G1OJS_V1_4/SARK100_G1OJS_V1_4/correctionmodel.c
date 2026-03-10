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
// 	FILE NAME: 	correctionmodel.c
// 	AUTHOR:		Alan Robinson
//
// 	DESCRIPTION: Characterises the bridge by recording the measured bridge ratios
//				 across frequency for a set of known loads. Encodes the characterisation
//				 data using a Moebius Transform for compact storage. Decodes the 
//				 characterisation data and corrects measurements.
//
// 	HISTORY
//	NAME   	DATE		REMARKS
//	AJR		JAN 2025	Creation
//*****************************************************************************/
#include "correctionmodel.h"

#include "control.h"
#include "storage.h"
#include "glb_data.h"
#include "keypad.h"
#include "msg_generic.h"
#include "morse.h"
#include "screens.h"
#include "bridge.h"
#include "derive.h"
#include "timers.h"
#include "UART.h"
#include "pclink_cmds.h"

typedef enum {STORE, RETRIEVE} ENCODE_DIRECTION;

//-----------------------------------------------------------------------------
//  Prototypes
//-----------------------------------------------------------------------------
WORD wCalRatio(BYTE bLoad, RATIO_TYPE rRatio);
static long lLinMap(long lVal, long lSource1, long lSource2, long lDest1, long lDest2);
WORD wLincor(WORD X, WORD M0, WORD M1, WORD T0, WORD T1);
WORD wTrueLoadRatios(BYTE bLoad, RATIO_TYPE rRatio);
static INT iRatioFromCalibration(ENCODE_DIRECTION xDir, RATIO_TYPE xType, INT iRatio_true, INT iRatio);
BYTE getUserConfirmation(void);
static long min_long(long a, long b);
static long max_long(long a, long b);

//-----------------------------------------------------------------------------
//  FUNCTION NAME:	min_long
//-----------------------------------------------------------------------------
static long min_long(long a, long b) {
    return (a > b) ? b : a;
}

//-----------------------------------------------------------------------------
//  FUNCTION NAME:	max_long
//-----------------------------------------------------------------------------
static long max_long(long a, long b) {
    return (a > b) ? a : b;
}

//-----------------------------------------------------------------------------
//  FUNCTION NAME:	MeasureUncorrectedRatios()
//  DESCRIPTION:	Calibration routine
//  ARGUMENTS: 		none.
//  RETURNS:		none.
//-----------------------------------------------------------------------------
void MeasureUncorrectedRatios (void)
{
	BYTE bCalFreqIndex;
	BYTE bFirst;
	RATIO_TYPE rRatio;
	BYTE bLoad;
	M8C_EnableGInt ;
	
	do
	{
		Screen_Clear();
		Screen_HideCursor();
	
		// Find DDS Gain step needed for reasonable Vf 
		// This is also an opportunity to abort the calibration without changing anything:
		Screen_Clear(); Screen_CStrAtRowCol(0,0, gDisconnectLoadStr);
		if(!getUserConfirmation()) break;
  		Screen_CStrAtRowCol(1, 0, gSettingGainStr);
		g_dwCurrHz=FREQ_MIN_Hz;
		DDS_Autolevel();
	
		// now measure and store the uncalibrated ratios Vz/Va and Vr/Vf at each cal frequency and load			
		for (bLoad=0; bLoad < CAL_LOAD_NUM; bLoad++)
		{
			Morse_Dah();
			Screen_Clear(); Screen_CStrAtRowCol(0,0, gCalLoadStr[bLoad]);
			if(!getUserConfirmation()) break;
			Screen_Clear(); Screen_CStrAtRowCol(1, 0, gCalibratingStr);		
			for (bCalFreqIndex=0; bCalFreqIndex<CAL_FREQS_NUM;bCalFreqIndex++)
			{
				g_dwCurrHz=CAL_START_kHz*1000 + (DWORD)bCalFreqIndex * CAL_FREQ_STEP_kHz*1000 ;
				Screen_Frequency(Display_Hz);
				MeasureBrgRts();
				for (rRatio=MODZ; rRatio <= MODGAMMA; rRatio++) {
					g_scCalTable[bCalFreqIndex][bLoad][rRatio] = iRatioFromCalibration(STORE, rRatio, wTrueLoadRatios(bLoad,rRatio), g_wBrgRts[rRatio]);
				}
			}
		}

		// Store cal factors in EEPROM
		g_bIsCalibrated=TRUE; // this needs to be set to True here, and only here
		STR_SaveCalibration();
	
		// Finish, alert user
		Screen_Clear();
		Screen_CStrAtRowCol(0,0, gConfigDoCalStr);
		Screen_CStrAtRowCol(1,0, gDoneRestartingStr);
		Morse_End();
		Delay_64ths(TIME_FLASH_MSG);
		M8C_DisableGInt;
		asm ("ljmp 0x0000"); // Perform Software Reset
	
	} while (FALSE);

}
//-----------------------------------------------------------------------------
//  FUNCTION NAME:	iRatioFromCalibration
//
//  DESCRIPTION:	Encodes or decodes a ratio measured during calibration 
//					so that the encoded result can be stored in the
//					BYTE arrays [bCalFreqIndex][bLoad] 
//					The measured ratio is first scaled to the true ratio, and then
//					log-like encoding is used to allow (measured/true) to range from
//						1/2 to 2 for (Vr/Vf measured) / (Vr/Vf true) -> -127 to 127 
//						1/5 to 5 for (Vz/Va measured) / (Vz/Va true) -> -127 to 127
//					hence the result can be encoded in an array of signed char.
//					Using signed innteger for the output and inputs means that the
//					single function can be used in each direction (encode & decode)
//
//  ARGUMENTS:		xDir		= STORE | RETRIEVE
//					xType	 	= MODZ | MODGAMMA
//					iRatio_true	= true ratio for the cal point scaled by wUNITY
//					iRatio		= ratio scaled by wUNITY, or encoded value, to convert 
//
//  RETURNS: 		signed int	= encoded value (range -127 to 127) when encoding
//					signed int	= Measured ratio (range 0 to 32767 scaled by wUNITY) when decoding
//
//	NOTES:			Whilst |Z|/50 can range to 65537/wUNITY, 32767 is enough for
//					the largest calibration load (560 ohms, true ratio 11200/wUNITY)
//					|Gamma| ranges 0 to 1000/wUNITY or slightly over with measurement error
//					The encoded output has zero indicating measured ratio = true ratio, so
//					the calibration table is initialised to zero by default
//-----------------------------------------------------------------------------

static INT iRatioFromCalibration(ENCODE_DIRECTION xDir, RATIO_TYPE xType, INT iRatio_true, INT iRatio) {
	long lNum;
	long lDenom;
	INT iScale;
	BYTE bDyRNG = (xType==MODGAMMA)? 3:6; 	// dynamic range 1/n to n
	
	#define ENC_MAX 127						// encoded range -127 to +127
	
	iScale = (ENC_MAX*(int)(bDyRNG+1)) /(int)(bDyRNG-1);

	if(xDir==STORE) {
    	lNum = iScale*(long)iRatio - (long)iScale*(long)iRatio_true;
    	lDenom = (long)iRatio + (long)iRatio_true;
		if (lDenom==0) {return 0;} else {				// lim x,y->0 of (x-y)/(x+y) is 0
			lNum /=lDenom;								// re-using the numerator var for the result
			lNum = (lNum>ENC_MAX)? ENC_MAX:lNum;		// clamp at +/- ENC_MAX; better than wrapped if stored in 8 bits
			lNum = (lNum<-ENC_MAX)? -ENC_MAX:lNum;		
    		return lNum; 
		}
	} else {
	    lNum = (long)iRatio_true*(long)iScale+(long)iRatio_true*(long)iRatio;
    	lDenom = (long)iScale - (long)iRatio;
    	return lNum / lDenom;  
	}
}
//-----------------------------------------------------------------------------
//  FUNCTION NAME:	CorrectBrgRts()
//  DESCRIPTION:	Use a mapping function to map the current measured ratios
//					to the true values that they should represent.
//  ARGUMENTS:		none
//  RETURNS: 		nothing
//-----------------------------------------------------------------------------

void CorrectBrgRts (void)
{
	// 1) Mod Gamma
	g_wBrgRts[MODGAMMA] = wLincor(g_wBrgRts[MODGAMMA], wCalRatio(2,MODGAMMA), wCalRatio(3,MODGAMMA), CALMG_H, CALMG_H2) ;
	g_wBrgRts[MODGAMMA] = min_long(VR_OVER_VF_MAX,g_wBrgRts[MODGAMMA]);
		
	// 2) Mod Z
	if(g_wBrgRts[MODZ]<wUNITY) {
		g_wBrgRts[MODZ] = lLinMap(g_wBrgRts[MODZ], wCalRatio(0,MODZ), wCalRatio(1,MODZ), CALMZ_L,  CALMZ_M);
	} else {
		g_wBrgRts[MODZ] = lLinMap(g_wBrgRts[MODZ], wCalRatio(1,MODZ), wCalRatio(2,MODZ), CALMZ_M,  CALMZ_H);	
	}
}

WORD wLincor(WORD X, WORD M0, WORD M1, WORD T0, WORD T1)
{
    WORD m;
	int c;
	WORD r;
	
	m = (DWORD)(wUNITY*(T1-T0))/(M1-M0);
	c = T0 - (signed long)(m * M0)/wUNITY;
	r = (long)(r * m)/wUNITY + c;
	
	return r;
}

//-----------------------------------------------------------------------------
//  FUNCTION NAME:	wCalRatio(BYTE bLoad, RATIO_TYPE rRatio)
//  DESCRIPTION:	Retrieves the ratio measured during calibration
//					interpolated to the current frequency g_dwCurrHz
//  ARGUMENTS:		bLoad - the load index
//					rRatio - the ratio type
//  RETURNS:		(rRatio = MODGAMMA): Vr/Vf; (rRatio=MODZ): Va/Vz
//-----------------------------------------------------------------------------
WORD wCalRatio(BYTE bLoad, RATIO_TYPE rRatio) 
{
	BYTE bCalFreqIndex = (g_dwCurrHz/1000-CAL_START_kHz) / (CAL_FREQ_STEP_kHz);
	return lLinMap(g_dwCurrHz/1000-CAL_START_kHz, (DWORD)bCalFreqIndex * CAL_FREQ_STEP_kHz, (DWORD)(1+bCalFreqIndex) * CAL_FREQ_STEP_kHz,
		iRatioFromCalibration(RETRIEVE, rRatio, wTrueLoadRatios(bLoad, rRatio), g_scCalTable[bCalFreqIndex][bLoad][rRatio]), 
		iRatioFromCalibration(RETRIEVE, rRatio, wTrueLoadRatios(bLoad, rRatio), g_scCalTable[bCalFreqIndex+1][bLoad][rRatio]) );		
}

WORD wTrueLoadRatios(BYTE bLoad, RATIO_TYPE rRatio)
{
	if (bLoad==0) return ((rRatio==MODZ)? CALMZ_L:CALMG_L);
	if (bLoad==1) return ((rRatio==MODZ)? CALMZ_M:CALMG_M);
	if (bLoad==2) return ((rRatio==MODZ)? CALMZ_H:CALMG_H);
	if (bLoad==3) return ((rRatio==MODZ)? CALMZ_H2:CALMG_H2);
	return -1;
}

//-----------------------------------------------------------------------------
//  FUNCTION NAME:	lLinMap(long lVal, long lSource1, long lSource2, long lDest1, long lDest2)
//  DESCRIPTION:	Maps the range lSource1 .. lSource2 onto lDest1 .. lDest2
//  				The function works to the precision set by the wUNITY #def
//  ARGUMENTS:
//		lVal - the value to map
//		lSource1, lSource2, lDest1, lDest2 - end points of the mapping ranges
//  RETURNS:
//     	long - the input value mapped into the destination range
//-----------------------------------------------------------------------------
static long lLinMap(long lVal, long lSource1, long lSource2, long lDest1, long lDest2)
{
	long lAlpha = ((long)wUNITY*(lVal - lSource1))/(lSource2-lSource1);
	return lDest1 + (lAlpha * (lDest2-lDest1)) / wUNITY;
}


