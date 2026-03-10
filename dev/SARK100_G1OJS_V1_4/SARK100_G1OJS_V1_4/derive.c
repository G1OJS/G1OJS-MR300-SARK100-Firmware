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
//
//	PROJECT:	SARK100 SWR Analyzer
// 	FILE NAME: 	derive.c
// 	AUTHOR:		G1OJS - Alan Robinson
// 	DESCRIPTION	Derive electrical parameters 
//				|R+jX|, SWR, R, X from bridge ratios
// 	HISTORY
//	NAME   		DATE		REMARKS
//	AJR			Feb 2025	G1OJS  - creation
//*****************************************************************************/
#include "derive.h"
#include <m8c.h>        // part specific constants and macros
#include <math.h>
#include "PSoCAPI.h"
#include "psocgpioint.h"
#include "glb_data.h"
#include "correctionmodel.h"

//-----------------------------------------------------------------------------
//  Prototypes
//-----------------------------------------------------------------------------
static WORD max_WORD(WORD a, WORD b);
static WORD Calc_Sqrt (DWORD dwN);

//-----------------------------------------------------------------------------
//  FUNCTION NAME:	max_WORD
//-----------------------------------------------------------------------------
static WORD max_WORD(WORD a, WORD b) {
    return (a > b) ? a : b;
}
//-----------------------------------------------------------------------------
//  FUNCTION NAME:	Do_SZRX_Calcs
//  DESCRIPTION:    Calculates SWR, |Z|, R and X from bridge ratios
//					Encoded as SWRx100, |Z|x10, Rx10 and Xx10
//  FORMULAS:
//	  SWRx100	= 100 * (1000 + g_wBrgRts[MODGAMMA])/(1000 - g_wBrgRts[MODGAMMA])
//
//	  Zx10		=	10*50*g_wBrgRts[MODZ]/1000 		= 	g_wBrgRts[MODZ]/2
//
//                 	(2500 + Z^2) * SWR			(250000 + (Zx10)^2) 
//    Rx10 		= 	------------------		=  	-------------------------------
//               	5 * (SWR^2 + 1)				5*(SWRx100)  +  50000/(SWRx100)
//
//    Xx10 		= 	SQRT ( (Z*10)^2 - (R*10)^2 )
//
//  ARGUMENTS:  None
//  RETURNS: 	Nothing
//
//-----------------------------------------------------------------------------
void Do_SZRX_Calcs (void)
{
	DWORD dwDenominator;
	DWORD dwNumerator;

	// Get SWRx100 from g_wBrgRts[MODGAMMA] which encodes Vr/Vf = 1 as 1000 
	dwNumerator = wUNITY + g_wBrgRts[MODGAMMA]; 
	dwDenominator = wUNITY - g_wBrgRts[MODGAMMA];
	g_wSwr100 = (WORD)((DWORD)100*dwNumerator/dwDenominator);
	
	// Get |Z|x10 from g_wBrgRts[MODZ] which encodes 1 as 1000 so 500*Vz/Va is 500*g_wBrgRts[MODZ]/1000 = g_wBrgRts[MODZ]/2
	// hence g_wZ10 can't exceed 32767 after correction (20480 before correction since g_wBrgRts[MODZ] uncorrected can't exceed 4096)
	// but can range in theory from 0 to 
	g_wZ10 = g_wBrgRts[MODZ]/2;
	
	// if SWR is lower than either 50/|Z| or |Z|/50, this will lead to R > |Z| and possible overflows
	// g_wSwr100 won't arrive here higher than 49900 (|Z|=24950 ohms or 0.1 ohms)
	// this equates to g_wZ10 = at least 2. g_wZ10=32767 equates to g_wSwr100=6553 which is less than max allowable SWR
	// so enforcing g_wZ10 >=2 allows enforcing the SWR >= max(50/|Z|,|Z|/50) without causing SWR>499
	// SWR>=|Z|/50 equates to g_wSwr100>=g_wZ10/5, and SWR>=50/|Z| equates to g_wSwr100>=50000/g_wZ10, 
	g_wZ10 = max_WORD(g_wZ10,2);	
	g_wSwr100=max_WORD(g_wSwr100, 1 + 50000 / g_wZ10);
	g_wSwr100=max_WORD(g_wSwr100, 1 + g_wZ10/5);
		
	// Get R from |Z| and SWRx100
	dwNumerator 	= 250000+ (DWORD)g_wZ10*g_wZ10;			// DWORD safe to >~ 6000 ohms 
	dwDenominator 	= 5*g_wSwr100 + 50000/g_wSwr100;		// safe for any real-life VSWR
   	g_wR10 = dwNumerator/dwDenominator;	
		
	// Get X from R and |Z| (reusing dwNumerator)
	dwNumerator = (DWORD)g_wZ10*(DWORD)g_wZ10 - (DWORD)g_wR10*(DWORD)g_wR10; // DWORD safe to >~ 5000 ohms 
	g_wX10 = Calc_Sqrt(dwNumerator);
}
//-----------------------------------------------------------------------------
//  FUNCTION NAME:	Calc_Sqrt
//  DESCRIPTION:	Calculates square root for integer number
//  ARGUMENTS:		DWORD dwN	Radicand
//  RETURNS:		(WORD) 		Square root
//-----------------------------------------------------------------------------
static WORD Calc_Sqrt (DWORD dwN)
{
	DWORD dwRem = 0;
	DWORD dwRoot = 0;
	BYTE ii;
	
	if (dwN > 4294836225) { // Maximum DWORD value where sqrt fits in a WORD
    	return WORD_MAX; 	// Return maximum value for WORD
	}

	for (ii=0;ii<16;ii++)
	{
		dwRoot <<= 1;
		dwRem = ((dwRem<<2)+(dwN>>30));
		dwN <<= 2;
		dwRoot++;
		if (dwRoot<=dwRem) {
			dwRem -= dwRoot;
			dwRoot++; 
		} else {
			dwRoot--;
		}
	}
	return (WORD)(dwRoot>>1);
}


