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
// 	FILE NAME: 	bridge.c
// 	AUTHOR:		G1OJS - Alan Robinson
// 	DESCRIPTION	Bridge measurement routines
// 	HISTORY
//	NAME   		DATE		REMARKS
//	AJR			Feb 2025	G1OJS  - creation
//*****************************************************************************/
#include "bridge.h"

#include "PSoCAPI.h"
#include "glb_data.h"
#include "dds.h"
#include "timers.h"


//-----------------------------------------------------------------------------
//  Prototypes
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  FUNCTION NAME:	MeasureBrgRts
//  DESCRIPTION: Read all bridge voltages and set global ratios Vr/Vf and Vz/Va
//  NOTES:
//    - WORD with wUNITY=1000 can encode ratios from 0.001 to 65.535 
//	    which represents impedance from zero via 0.05 to 3276.75 ohms and 
//	    VSWR from 1.00:1 via 1.002:1 in 2% steps to ~8000:1 where Vr=3999 and Vf=4000 
//    - 3k ohms can be seen on open circuit at lower frequencies, so we need to 
//      cap the ratio 1000*Vz/Va at WORD_MAX, which is done in wDivide(WORD w1, WORD w2)
//    - Vr can sometimes slightly exceed Vf due to measurement error, but
//      this is capped after correction in CorrectBrgRts and the raw (uncorrected)
//      ratio is only ever seen if verbose pcLink is used in which case assume expert user 
//      and don't hide the measurement errors, but still use wDivide for WORD_MAX safety
//  ARGUMENTS: None
//  RETURNS: Nothing, but sets global variables:
//     g_xBridge.wVzVa (wUNITY x "Vz/Va")
//     g_xBridge.wVrVf (wUNITY x "Vr/Vf")
//-----------------------------------------------------------------------------
void MeasureBrgRts ( void )
{
	DDS_Set(g_dwCurrHz);  // DDS is off by default, so turn it on to correct freq
	//DDS_Autolevel();
	Delay_64ths(1);	  // Bridge capacitor settling time
	#define pos(n) (n==0)? 1:n
	g_wBrgRts[MODZ] 		= wDivide( wRead_ADC_DDSUNCHANGED(VzPort), pos(wRead_ADC_DDSUNCHANGED(VaPort)) );
	g_wBrgRts[MODGAMMA] 	= wDivide( wRead_ADC_DDSUNCHANGED(VrPort), pos(wRead_ADC_DDSUNCHANGED(VfPort)) );
	DDS_Set(0); // Turn DDS off (this function isn't called from VFO mode)
	// cap g_wBrgRts[MODGAMMA] so that g_wSwr100 won't overflow in WORD (this cap is also applied after correction)
	if(g_wBrgRts[MODGAMMA] > VR_OVER_VF_MAX) g_wBrgRts[MODGAMMA] = VR_OVER_VF_MAX;
}

void DDS_Autolevel(void)
{
	for (g_bDDS_GainStep=0; g_bDDS_GainStep<GAIN_SETTINGS_NUM; g_bDDS_GainStep++)
	{
		PGA_DDS_1_SetGain(g_xGainDds[g_bDDS_GainStep].bGain1);
		PGA_DDS_2_SetGain(g_xGainDds[g_bDDS_GainStep].bGain2);
		if ( (wRead_ADC_DDSUNCHANGED(VfPort) >= VF_REFERENCE_LEVEL) || (g_bDDS_GainStep==(GAIN_SETTINGS_NUM-1) ) ) break;
	}
}

//-----------------------------------------------------------------------------
//  FUNCTION NAME:	wRead_ADC_DDSON
//  DESCRIPTION: 	Turn on DDS and read ADC, then turn DDS Off
//  ARGUMENTS: 		None
//  RETURNS: 		WORD = Vx
//-----------------------------------------------------------------------------
WORD wRead_ADC_DDSON(BYTE VxPort) {	
	WORD wVal;
	DDS_Set(g_dwCurrHz);  
	wVal=wRead_ADC_DDSUNCHANGED(VxPort);
	DDS_Set(0);
	return wVal;
}
//-----------------------------------------------------------------------------
//  FUNCTION NAME:	wRead_ADC
//  DESCRIPTION:	Set the specified port and read the ADC
//					Note that Vf is measured at the top of the bridge, so 
//					is double that in the common definition of the input
//					divider. Hence we have to double Vr too, which is 
//					convenient because it allows doubling the gain to 
//					reflect the halved 'rail' swing of Vr (better S/N)
//  ARGUMENTS:		BYTE VxPort - which port (see #defines in glb_data.h)
//  RETURNS: 		Measured value
//-----------------------------------------------------------------------------
WORD wRead_ADC_DDSUNCHANGED(BYTE VxPort)
{
	WORD wVal;
	
	AMUX4_ADC_InputSelect(VxPort);					// Select the input port needed to be read
	if(VxPort==VrPort) PGA_ADC_SetGain(PGA_ADC_G5_33);	// double detector amp gain for Vr
	ADCINC12_GetSamples(1);               	    	// Ask for 1 sample
    while(ADCINC12_fIsDataAvailable() == 0);    	// Wait until next reading ready  
	wVal=(WORD)ADCINC12_iGetData()+2048;			// Take the reading
    ADCINC12_ClearFlag();                       	// Clear ADC flag  
	if(VxPort==VrPort) PGA_ADC_SetGain(PGA_ADC_G2_67);	// set detector amp gain back to nominal
  	
	return wVal; 	// Return the reading
}

//-----------------------------------------------------------------------------
//  FUNCTION NAME: 	wDivide(WORD num, WORD denom)
//  DESCRIPTION: 	Returns (num*wUNITY)/denom capped at WORD_MAX
//					Note that the capping logic also excludes divide by zero
//  ARGUMENTS:  	WORD num, WORD denom
//  RETURNS:  		(WORD)((num*wUNITY)/denom)
//-----------------------------------------------------------------------------
WORD wDivide(WORD num, WORD denom)
{
	return (WORD)((DWORD)wUNITY*num >= (DWORD)WORD_MAX*denom)? WORD_MAX:((WORD)(((DWORD)wUNITY*num)/denom));
}


