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
// 	FILE NAME: 	pclink_cmds.c
// 	AUTHOR:		G1OJS - Alan Robinson
// 	DESCRIPTION	Refactored (to save memory) commands from original pclink plus new commands
// 	HISTORY
//	NAME   		DATE		REMARKS	
//	AJR			FEB 2025	G1OJS  - creation 
//*****************************************************************************/
#include "pclink_cmds.h"
#include <stdlib.h>
#include "control.h"
#include "UART.h"
#include "glb_data.h"
#include "bridge.h"
#include "derive.h"
#include "dds.h"
#include "screens.h"
#include "correctionmodel.h"

static BYTE const bufOk				[] = "\r\nOK\r\n";
static BYTE const bufStart			[] = "\r\nStart\r\n";
static BYTE const bufEnd			[] = "End\r\n";
static BYTE const bufErrFreqNotSet	[] = "\r\nError: freq not set\r\n";
static BYTE const bufErrExpectFreq	[] = "\r\nError: expected freq val\r\n";
static BYTE const bufErrExpectStep	[] = "\r\nError: expected step val\r\n";
static BYTE const bufErrInvalidFreq	[] = "\r\nError: invalid freq\r\n";
static BYTE const bThousandCommaStr	[] = "000,";
static BYTE const bThousandStr		[] = "000";
static BYTE const bCommaStr 		[] = ",";

#define NFILTERS 2
static WORD wVals[NFILTERS];

BYTE bWeight;

//-----------------------------------------------------------------------------
//  Prototypes
//-----------------------------------------------------------------------------
void MeasureCorrectCalc(void);
char FormatNumberRJ(unsigned long dwVal, unsigned char bFieldLen, unsigned char bDP);
WORD filter(WORD wVal, RATIO_TYPE rRatio);

//-----------------------------------------------------------------------------
//  FUNCTION NAME:	Cmd_On
//  DESCRIPTION:	Process on (enable DDS) command
//	ARGUMENTS: 		none. 
//	RETURNS: 		none.
//-----------------------------------------------------------------------------
void Cmd_On (void)
{
	Set_DDS(g_dwCurrHz);
	UART_CPutString(bufOk);
}
//-----------------------------------------------------------------------------
//  FUNCTION NAME:	Cmd_Off
//  DESCRIPTION:	Process on command
//  ARGUMENTS: 		none.
//  RETURNS:  		none.
//-----------------------------------------------------------------------------
void Cmd_Off (void)
{
	Set_DDS(0);
	UART_CPutString(bufOk);
}
//-----------------------------------------------------------------------------
//  FUNCTION NAME:	Cmd_Freq
//  DESCRIPTION:	Process freq (set frequency) command
//  ARGUMENTS: 		none.
//  RETURNS:		none.
//-----------------------------------------------------------------------------
void Cmd_Freq (void)
{
	DWORD dwfreqRequest = getFreqArg(); 
	if (dwfreqRequest >= FREQ_MIN_Hz && dwfreqRequest <= FREQ_MAX_Hz) { 
		g_dwCurrHz=dwfreqRequest;
		UART_CPutString(bufOk);
	} else {
		UART_CPutString(bufErrInvalidFreq);
	}
}
//-----------------------------------------------------------------------------
//  FUNCTION NAME:	pcLinkPrintToSerial
//  DESCRIPTION: 	Print out all four bridge voltages and/or SWR,R,X,Z
//					depending on PRINT_WHAT
//					NOTE that individual voltages are no longer used in the bulk
//					of the calculations (converted to ratios just after measurement)
//					but 4 values need to be provided for compatibility with 
//					companion software that expects them via Cmd_Raw
//					Hence, measure Vf and Vr again (uncorrected) and print out
//					Vf_uncorrected, Vf_uncorrected x (Vr/Vf)_corrected, 
//					Va_uncorrected x (Vz/Va)_corrected, Va_uncorrected
//  ARGUMENTS:		MEASMODE = CMD | SCANNING
//						CMD - called directly from pclink, likely 3rd party software
//						SCANNING - called from pcLinkFreqScan & using averaging
//					PRINT_WHAT = VRVFVZVA | SRXZ | COMBI
//
// 	RETURNS:	 
//
//	- if PRINT_WHAT == VRVFVZVA		(this emulates the old Cmd_Raw)
//		Response: corrected voltages  Vf, 'Vr' = (Vr/Vf)*Vf, 
//									  'Vz'     = (Vz/Va)*Va, Va
//
//  	Example: 4000000,165000,31800,1000000 
//
//	- if PRINT_WHAT == SRXZ			(this emulates the old Cmd_Imp)
//		Response: corrected parameters {SWR},{R},{X},{Z}
//  	Example: 1.05,52,10,51 
//
//  NOTE: used by some third party software. The expected format is as the example above
//-----------------------------------------------------------------------------

void pcLinkPrintToSerial (MEASMODE mMode, PRINT_WHAT pwPrintWhat)
{
	WORD wVf;
	WORD wVa;
	// TODO is it important to respect the original DDS condition on entry? 
	// if so, how to detect DDS on or off easily ...
	
	// if scanning, use ratios set by scan routine
	// if not, get new ones now & note CMD measurement is always corrected
	if(mMode == CMD) MeasureCorrectCalc(); 

	if(pwPrintWhat == VFVRVZVA){	
		wVf=wRead_ADC_DDSON(VfPort);
		wVa=wRead_ADC_DDSON(VaPort);
		PrintWdToSerial(wVf ,0, bThousandCommaStr);  
		PrintWdToSerial(  ((DWORD)wVf*(DWORD)g_wBrgRts[MODGAMMA]) / wUNITY ,0, bThousandCommaStr);
		PrintWdToSerial(  ((DWORD)wVa*(DWORD)g_wBrgRts[MODZ])     / wUNITY ,0, bThousandCommaStr);
		PrintWdToSerial(wVa ,0, bThousandStr);
	}
		
	if(pwPrintWhat == SRXZ){	
		// Use 1 Ohm resolution on R,X,Z unless mMode=SCANBR, then use 0.1 Ohm resolution 
		PrintWdToSerial(g_wSwr100, 											      2, 	bCommaStr);	
		PrintWdToSerial((mMode==SCANBR)? g_wR10:((g_wR10+5)/10), (mMode==SCANBR)? 1:0,	bCommaStr);
		PrintWdToSerial((mMode==SCANBR)? g_wX10:((g_wX10+5)/10), (mMode==SCANBR)? 1:0,	bCommaStr);
		PrintWdToSerial((mMode==SCANBR)? g_wZ10:((g_wZ10+5)/10), (mMode==SCANBR)? 1:0,	"");
	}
	
}
//-----------------------------------------------------------------------------
//  FUNCTION NAME:	pcLinkFreqScan
//  DESCRIPTION:	Scan through frequencies specified and call appropriate 
//					Print_XXX routine to display requested outputs
//  ARGUMENTS:    	PRINT_WHAT = SRXZ | VRVFVZVA | COMBI
//  RETURNS:		none.
//-----------------------------------------------------------------------------
void pcLinkFreqScan(MEASMODE mMode)
{
	DWORD dwStartFreqHz;
	DWORD dwEndFreqHz;
	DWORD dwStepFreqHz;
	DWORD dwOffsetHz;
	DWORD dwCurrHz_scan;
	
	do
	{
		// Get start, end and step frequencies
		dwStartFreqHz = getFreqArg(); if(dwStartFreqHz == -1) break;
		dwEndFreqHz	= getFreqArg(); if(dwEndFreqHz   == -1) break;
		dwStepFreqHz	= getFreqArg(); if(dwStepFreqHz  == -1) break;			

		// Get filter settings. Let weight range from 1 to 255.
		#define min(a,b) ((a)>(b))? (b):(a)
		bWeight  = min(255, ((DWORD)100000*g_bFltWd100kHz[g_xConf.bFltWt])/dwStepFreqHz);
		if((g_xConf.bFltWt>0) &&  (bWeight==0)) bWeight=1;
		dwOffsetHz = dwStepFreqHz*(DWORD)bWeight; 	// slightlty inaccurate for b<~5 but error is less than one step, and at least offset = int x step
		UART_CPutString(bufStart);

		// now step through frequencies 
		dwCurrHz_scan = dwStartFreqHz;	

		do
		{
			// need to separate g_dwCurrHz from the scan loop variable so we can cap g_dwCurrHz at FREQ_MAX_Hz
			// so that we don't go beyond cal max freq when using filters and needing to scan ahead
			g_dwCurrHz=(dwCurrHz_scan<FREQ_MAX_Hz)? dwCurrHz_scan:FREQ_MAX_Hz;
			
			Screen_Frequency(Display_kHz);		// might be better to replace this with a % complete indicator
			
			// measure bridge ratios and reconstruct voltages using filter
			// then reconstruct g_wBrgRts so all 6 params are based on filtered values			
			MeasureBrgRts();	
			g_wBrgRts[MODGAMMA] = filter(g_wBrgRts[MODGAMMA],MODGAMMA);
			g_wBrgRts[MODZ] = filter(g_wBrgRts[MODZ],MODZ);
					
			if (dwCurrHz_scan >= (dwStartFreqHz + dwOffsetHz))	// if scanning ahead for filters, wait until we 'catch up'
			{
			
			// power level scan to do 
				if (mMode==SCANV)  {
					PrintWdToSerial(g_dwCurrHz/1000 - dwOffsetHz/1000,0, ", ");
					PrintWdToSerial(wRead_ADC_DDSON(VfPort),3, ", ");
					PrintWdToSerial(wRead_ADC_DDSON(VrPort),3, ", ");				
					PrintWdToSerial(wRead_ADC_DDSON(VzPort),3, ", ");
					PrintWdToSerial(wRead_ADC_DDSON(VaPort),3, ", ");
					PrintWdToSerial(g_bDDS_GainStep,0, "");
//					PrintWdToSerial(g_xGainDds[g_bDDS_GainStep].bGain1,0, "");
//					PrintWdToSerial(g_xGainDds[g_bDDS_GainStep].bGain2,0, "");					
				}
				if (mMode==SCANBR) {
					PrintWdToSerial(g_dwCurrHz/1000 - dwOffsetHz/1000,0, ", ");
                    PrintWdToSerial(g_wBrgRts[MODZ],3, ", ");
                    PrintWdToSerial(g_wBrgRts[MODGAMMA],3, ", ");
				}
				// now with corrected bridge ratios
				CorrectBrgRts(); 
				if (mMode==SCANBR) {
                    PrintWdToSerial(g_wBrgRts[MODZ],3, ", ");
                    PrintWdToSerial(g_wBrgRts[MODGAMMA],3, "");
				}
				if (mMode==SCAN) {
					Do_SZRX_Calcs();
					pcLinkPrintToSerial(mMode, SRXZ);
				}
				if (mMode==SCANR) {
					pcLinkPrintToSerial(mMode, VFVRVZVA);
				}
				UART_PutCRLF();
			}
			dwCurrHz_scan += dwStepFreqHz;
		} while ( dwCurrHz_scan <= (dwEndFreqHz + dwOffsetHz) );

	} while (FALSE);

	UART_CPutString(bufEnd);

	Morse_End();
}
//-----------------------------------------------------------------------------
//  FUNCTION NAME:	getFreqArg
//  DESCRIPTION: 	Get frequency argument from command line
//  ARGUMENTS:		none
// 	RETURNS: 		DWORD freq in Hz
//-----------------------------------------------------------------------------
DWORD getFreqArg(void)
{
	char * strPtr; 						// Parameter pointer
	strPtr = UART_szGetParam();
	if (strPtr==NULL) {					// missing argument
		UART_CPutString(bufErrExpectFreq);
		return -1;
	}
	return atol(strPtr);	// good frequency
}
//-----------------------------------------------------------------------------
//  FUNCTION NAME:	PrintWORDtoSerial
//  DESCRIPTION: 	Given a number dwVal x wDivisor, prints to serial with appropriate 
//					decimal point 
//  ARGUMENTS:		WORD wVal = Number x wDivisor to print
//					BYTE bDecimals - number of dps
//  RETURNS: 		none.
//-----------------------------------------------------------------------------
void PrintWdToSerial(WORD wVal, BYTE bDecimals, const BYTE * termStr)
{
	FormatNumberRJ(wVal , 6, bDecimals); 	
	UART_PutString(g_buffer16);
	UART_CPutString(termStr);
}
//-----------------------------------------------------------------------------
//  FUNCTION NAME:	filter
//  DESCRIPTION: 	Implements exponential-type filters for MODZ and MODGAMMA
//  ARGUMENTS:		WORD wVal = value to filter
//					RATIO_TYPE rFilter = which 'channel' of the filters to use
//  RETURNS: 		filtered value for channel rFilter
//-----------------------------------------------------------------------------
WORD filter(WORD wVal, RATIO_TYPE rFilter){
	wVals[rFilter] = (DWORD)((DWORD)bWeight * wVals[rFilter] + (DWORD)wVal) / (1+bWeight);
	return wVals[rFilter];
}