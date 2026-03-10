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
// 	FILE NAME: 	scans.c
// 	AUTHOR:		G1OJS - Alan Robinson
// 	DESCRIPTION	Scanning functions
// 	HISTORY
//	NAME   		DATE		REMARKS	
//	AJR			FEB 2025	G1OJS  - creation 
//*****************************************************************************/

#include <stdlib.h>
#include <m8c.h>
#include "scans.h"
#include "morse.h"
#include "control.h"
#include "screens.h"
#include "glb_data.h"
#include "msg_generic.h"
#include "keypad.h"
#include "bridge.h"
#include "correctionmodel.h"


void MeasureCorrectCalc(void);
char FormatNumberRJ(unsigned long dwVal, unsigned char bFieldLen, unsigned char bDP);

// TODO - the latest function here is Do_LineLength_Scan which used to call ScanFreqs
// but now has a better (more memory-efficient, more robust to noise tbc) minimum
// detection algorithm
// Next step is to use this algorithm in VSWR scanning, and factor it into a 
// mindetect function if that makes sense to do
// 


//-----------------------------------------------------------------------------
//  FUNCTION NAME:	Do_VSWR_Scan
//  DESCRIPTION:  	Scan within band limits to find minimum VSWR that is also below SWR_BW_THRESH
//					If found, scan up and down (including beyond band limits) to
//					find SWR bandwidth at SWR_BW_THRESH
//  ARGUMENTS:		none
//  RETURNS:		Nothing
//-----------------------------------------------------------------------------
void Do_VSWR_Scan (void)
{
	#define COARSE_STEP_Hz 51000
	#define FINE_STEP_Hz 1000

	DWORD dwEntryFreqHz=g_dwCurrHz;
	DWORD dwMinSWRHz;
	DWORD dwHzLow;
	DWORD dwHzHigh;

	Screen_Clear();
	Screen_HideCursor();

	// STEP 1: scan the whole band for best minimum Vr (fast)
	// coarse scan
	Screen_CStrAtRowCol(0, 0, gScanCoarseStr);
	g_dwCurrHz = (DWORD)BAND_FREQ_ToHz*g_wBandBoundaries[g_bBandIndex];
	findGlobalMinimum(SCANPARAM_Vruc, (DWORD)BAND_FREQ_ToHz*g_wBandBoundaries[g_bBandIndex+1], COARSE_STEP_Hz);
	dwMinSWRHz = g_dwCurrHz;
	Morse_Dit(); 
	Screen_Frequency(Display_Hz); 
	
	// fine scan around coarse minimum
	Screen_CStrAtRowCol(1, 0, gScanFineStr);
	g_dwCurrHz -= COARSE_STEP_Hz;	// reverse one step
	findGlobalMinimum(SCANPARAM_SWR, g_dwCurrHz+(DWORD)2*COARSE_STEP_Hz, FINE_STEP_Hz);
	dwMinSWRHz = g_dwCurrHz;
	Morse_Dit(); 
	Screen_Frequency(Display_Hz); 
		
	if(g_wSwr100 < SWR_BW_THRESH) { 
		// STEP 2: search downwards and upwards from the global minimum to find the bandwidth,
		// display the bandwidth and return with global variables set to display full measurement results at match frequency
		Screen_CStrAtRowCol(0, 0, gScanFineStr);
		Screen_CStrAtRowCol(1, 0, gScanThreshStr);

		findSWRThreshold(SCANDIRECTION_DOWN, FREQ_MIN_Hz, COARSE_STEP_Hz);	
		g_dwCurrHz 	= g_dwCurrHz + COARSE_STEP_Hz;	// reverse one step
		findSWRThreshold(SCANDIRECTION_DOWN, g_dwCurrHz - COARSE_STEP_Hz, FINE_STEP_Hz);	
		dwHzLow=g_dwCurrHz;
	
		g_dwCurrHz=dwMinSWRHz;	// set freq to match freq and scan up to SWR>threshold
		findSWRThreshold(SCANDIRECTION_UP, FREQ_MAX_Hz, COARSE_STEP_Hz);	
		g_dwCurrHz 	= g_dwCurrHz - COARSE_STEP_Hz;	// reverse one step
		findSWRThreshold(SCANDIRECTION_UP, g_dwCurrHz + COARSE_STEP_Hz, FINE_STEP_Hz);	
		dwHzHigh=g_dwCurrHz;
	
		// Set g_dwCurrHz to min swr freq, measure and display results (live SWR and Frequency plus BW in bottom row) and wait for keypress
		Screen_Clear();
		g_dwCurrHz=dwMinSWRHz;
		Screen_Frequency(Display_Hz); 
		MeasureCorrectCalc();
		Screen_SRXZ(); 
		Screen_CStrAtRowCol(1, 0, gBlankStr_16);
		FormatNumberRJ(dwHzLow /1000, 5, 0); Screen_StrAtRowCol(1, 0, g_buffer16);
		FormatNumberRJ(dwHzHigh /1000, 5, 0); Screen_StrAtRowCol(1, 8, g_buffer16);
	
		KEYPAD_WaitKey(TIME_WAIT_KEY_S);
	}
	
	Screen_Clear();
	g_dwCurrHz = dwMinSWRHz;

}

//-----------------------------------------------------------------------------
//  FUNCTION NAME:	Do_LineLength_Scan
//  DESCRIPTION:  	Scanning routine
//  ARGUMENTS:		none
//  RETURNS:		Nothing
//-----------------------------------------------------------------------------
void Do_LineLength_Scan (void)
{
	#define STEPHZ 233000
	BYTE 	bMinCount;
	WORD 	wLastMinFreqkHz;
	DWORD 	dwEntryFreqHz=g_dwCurrHz;
	
	do { // this loop gives opportunity to repeat the whole process without going through menus again

  		Screen_Clear();
		Screen_CStrAtRowCol(0, 0, gMinimaStr);
		Screen_HideCursor();

		// scan upwards from device min freq and stop on each minimum of *either* Vz or Va and record frequency
		g_dwCurrHz = FREQ_MIN_Hz;
		bMinCount=0;
		do {
			if(findNextGoodNotch(SCANPARAM_VzucORVauc, FREQ_MAX_Hz, STEPHZ))	{
				Morse_Dit();
				wLastMinFreqkHz=g_dwCurrHz/1000;
				bMinCount++;
				Screen_Frequency(Display_kHz);
			}
			g_dwCurrHz += STEPHZ*2; // start the next min search above the current minimum

		} while (g_dwCurrHz <= FREQ_MAX_Hz) ;
		
		Screen_CableLength(    ( (DWORD)75000*(DWORD)g_bVF[g_xConf.bVF]*(DWORD)bMinCount ) / wLastMinFreqkHz  );
		Morse_K();
		
		// opportunity to repeat the whole process without going through menus again (press any key but UP)
		if( KEYPAD_WaitKey(TIME_WAIT_KEY_S) == KBD_UP ) break;		
	} while (TRUE);
	g_dwCurrHz = dwEntryFreqHz;
}	


//-----------------------------------------------------------------------------
//  FUNCTION NAME:	findGlobalMinimum
//  DESCRIPTION:  	Scan up in frequency and stop at dwFreqLimitHz
//					Return with g_dwCurrHz set to frequency of lowest value of specified parameter
//
//  ARGUMENTS:		SCAN_SCANPARAM 		= SCANPARAM_SWR | SCANPARAM_Vruc | SCANPARAM_Vzuc | SCANPARAM_Vauc | SCANPARAM_VzucORVauc
//										NOTE: scanning on a single voltage gives a speed increase of approx 3:1 at the expense of using 
//										the uncalibrated proxy
//					WORD bStepStepHz	-> scan step in Hz
//
//  RETURNS:		nothing
static void findGlobalMinimum(SCAN_SCANPARAM xScanParam, DWORD dwFreqLimitHz, DWORD dwStepHz)
{
	
	DWORD 	dwEntryFreqHz=g_dwCurrHz;
	WORD	wParam;
	WORD	wParamGlobalMin=WORD_MAX;
	DWORD	dwMinParamHz;
	
	g_dwCurrHz -= dwStepHz; // reverse so that we have a delta for the first frequency
	
	do {
		Set_DDS(g_dwCurrHz);
		if(xScanParam == SCANPARAM_VzucORVauc) {
			#define min(a, b) ((a<b)? a:b)
			wParam=min(wRead_ADC_DDSUNCHANGED(VaPort), wRead_ADC_DDSUNCHANGED(VzPort));
		} 
		if(xScanParam == SCANPARAM_Vruc){
			wParam = wRead_ADC_DDSUNCHANGED(VrPort);
		}
		if(xScanParam == SCANPARAM_SWR){
			MeasureCorrectCalc();
			wParam = g_wSwr100;			
		}	
		if(wParam < wParamGlobalMin) {
			dwMinParamHz    = g_dwCurrHz;
			wParamGlobalMin = wParam;
		}
		g_dwCurrHz += dwStepHz;	
	} while ( g_dwCurrHz <= dwFreqLimitHz ) ;
	
	g_dwCurrHz=dwMinParamHz;
}



//-----------------------------------------------------------------------------
//  FUNCTION NAME:	findNextGoodNotch
//  DESCRIPTION:  	Scan up in frequency and stop as specified by 
//					arguments leaving final frequency and measurement in global measurement variables
//					In all cases, a full measurement and calculation is done prior to return.
//					The final frequency is that of the condition that triggered the return: (lowest) minimum or the band edge or global limit
//					Frequency and impedance etc is displayed, but only if SCAN_SCANPARAM == SCANPARAM_SWR
//					Currently, searching for maxima is not coded.
//
//  ARGUMENTS:		SCAN_SCANPARAM 		= SCANPARAM_SWR | SCANPARAM_Vruc | SCANPARAM_Vzuc | SCANPARAM_Vauc | SCANPARAM_VzucORVauc
//										-> parameter to trigger return TRUE 
//										NOTE: scanning on a single voltage gives a speed increase of approx 3:1 at the expense of using 
//										the uncalibrated proxy
//					WORD bStepStepHz	-> scan step in Hz
//
//  RETURNS:		TRUE if minimum found before wFreqLimitHz, otherwise FALSE
static BYTE findNextGoodNotch(SCAN_SCANPARAM xScanParam, DWORD dwFreqLimitHz, DWORD dwStepHz)
{
	#define HISTLEN 8
	#define HISTPAT0 240
	#define HISTPAT1 242
	#define HISTPAT2 184
	
	//#define DEBUG true
	
	DWORD 	dwEntryFreqHz=g_dwCurrHz;
	WORD	wParam;
	WORD	wParamPrev=WORD_MAX;
	BYTE 	bDeltaHist;

	#ifdef DEBUG 
		UART_CmdReset(); 					// Initialize receiver/cmd buffer
		UART_IntCntl(UART_ENABLE_RX_INT); 	// Enable RX interrupts
		UART_Start(UART_PARITY_NONE); 		// Enable UART
		M8C_EnableGInt ;
		UART_PutCRLF();UART_PutCRLF();UART_PutCRLF();UART_PutCRLF();UART_PutCRLF();UART_PutCRLF();UART_PutCRLF();UART_PutCRLF();
	#endif
	
	if(dwStepHz == 0) return FALSE;
	
	g_dwCurrHz -= dwStepHz*(HISTLEN/2-1); // reverse so that the history buffer can gather data before the start frequency
	bDeltaHist=0;
	
	do {
		Set_DDS(g_dwCurrHz);
		if(xScanParam == SCANPARAM_VzucORVauc) {
			#define min(a, b) ((a<b)? a:b)
			wParam=min(wRead_ADC_DDSUNCHANGED(VaPort), wRead_ADC_DDSUNCHANGED(VzPort));
		} 
		if(xScanParam == SCANPARAM_Vruc){
			wParam = wRead_ADC_DDSUNCHANGED(VrPort);
		}
		if(xScanParam == SCANPARAM_SWR){
			MeasureCorrectCalc();
			wParam = g_wSwr100;			
		}	
		bDeltaHist <<= 1;
		bDeltaHist |= (wParamPrev >= wParam);
		bDeltaHist &= (1 << HISTLEN) - 1;  
		wParamPrev = wParam;
		
		#ifdef DEBUG
			ultoa(g_buffer16, g_dwCurrHz ,10); UART_PutString(g_buffer16); UART_PutChar(',');
			ultoa(g_buffer16, wParam ,10); UART_PutString(g_buffer16); UART_PutChar(',');
			ultoa(g_buffer16, bDeltaHist ,10); UART_PutString(g_buffer16);
			UART_PutCRLF();
		#endif
		
		if((bDeltaHist == HISTPAT0) || (bDeltaHist == HISTPAT1) || (bDeltaHist == HISTPAT2)) 	{
			g_dwCurrHz -= dwStepHz*(HISTLEN/2); // reverse to the min of the search pattern
			MeasureCorrectCalc();
			return TRUE;
		}
		g_dwCurrHz += dwStepHz;	
	} while ( g_dwCurrHz <= dwFreqLimitHz ) ;
	
	g_dwCurrHz=dwEntryFreqHz;
	return FALSE;
}

//-----------------------------------------------------------------------------
//  FUNCTION NAME:	findSWRThreshold
//  DESCRIPTION:  	Scan up or down in frequency and stop at SWR Threshold 
//	ARGUMENTS:		SCAN_DIRECTION		= SCANDIRECTION_UP | SCANDIRECTION_DOWN		-> which way to scan
//					WORD wStepStepHz	-> scan step in Hz
//  RETURNS:		nothing
//-----------------------------------------------------------------------------
static void findSWRThreshold(SCAN_DIRECTION xScanDirection, DWORD dwFreqLimitHz, DWORD dwStepHz)
{
	do {
		Set_DDS(g_dwCurrHz);
		MeasureCorrectCalc();
		if(xScanDirection == SCANDIRECTION_UP) {g_dwCurrHz += dwStepHz;} else {g_dwCurrHz -= dwStepHz;} 
	} while ( (g_wSwr100 < SWR_BW_THRESH) && ( (xScanDirection == SCANDIRECTION_UP)? (g_dwCurrHz <= dwFreqLimitHz):(g_dwCurrHz >= dwFreqLimitHz))  ) ;
}

