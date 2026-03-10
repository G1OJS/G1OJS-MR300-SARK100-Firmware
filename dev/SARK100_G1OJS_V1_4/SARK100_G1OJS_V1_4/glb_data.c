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
//
//	PROJECT:	SARK100 SWR Analyzer
// 	FILE NAME: 	GLB_DATA.C
// 	AUTHOR:		EA4FRB - Melchor Varela
//
// 	DESCRIPTION
//
//	Global constants and variables
//
// 	HISTORY
//
//	NAME   	DATE		REMARKS
//
//	MVM	   	DEC 2009	Creation
//
//  AJR			JAN 2025	Align data with refactoring and functional changes	
//
//*****************************************************************************
// 	IMAGECRAFT COMPILER TYPE DEFINITIONS
// 	From page 17 of the ImageCraft C Compiler Guide, Document # 001-44476 Rev *A
//
//	Type				Bytes		Range
//	[unsigned] char			1		0 .. 255
//      signed char			1		-128 .. 127
//
//	unsigned int			2		0 .. 65535
//	[signed] int			2		-32786 .. 32767
//
//	unsigned int|short		2		0 .. 65535
//	[signed] int|short		2		-32786 .. 32767
//
//	unsigned long			4		0 .. 4,294,967,295
//	[signed] long			4		-2,147,483,648 .. 2,147,483,647
//
//	float | double			4		1.175e-38 .. 3.40e+38
//
//	enum					1		if enum < 256
//							2		if enum >= 256
//
//	BOOL  = unsigned char   1		0 .. 255
//	BYTE  = unsigned char	1		0 .. 255
//	CHAR  = signed char		1		-128 .. 127
//	WORD  = unsigned int	2		0 .. 65535
//	INT   = signed int		2		-32786 .. 32767
//	DWORD = unsigned long	4		0 .. 4,294,967,295
//	LONG  = signed long		4		-2,147,483,648 .. 2,147,483,647
//
//*****************************************************************************
//
//
//
#include "glb_data.h"
#include "PSoCAPI.h"

//-----------------------------------------------------------------------------
//  Public data:
//-----------------------------------------------------------------------------

// counters decremented by sleep timer
volatile BYTE g_bIdleCounter = 0;
volatile BYTE g_bMeasureCounter = 0;
volatile BYTE g_bDebounceCounter = 0;
volatile BYTE g_bLongPressKeyCounter = 0;
volatile BYTE g_bSixtyFourthSecondCounter = 0;


BYTE g_buffer16[16];					// global generic string buffer
BYTE g_bMode = MODE_IMP;				// current measurement mode

// storage for measured calibration data and calibration state	
signed char g_scCalTable[CAL_FREQS_NUM][CAL_LOAD_NUM][2] = {0}; 
BYTE g_bIsCalibrated=FALSE;				// TRUE if calibrated
BYTE g_bDDS_GainStep=8;					// Default DDS gain step

// Current results of impedance measurements
WORD g_wBrgRts[2];				// Measured bridge ratios			
WORD g_wSwr100;							// SWR x 100
WORD g_wZ10;							// |Z|_ohms x 10 
WORD g_wX10;							// X_ohms x 10
WORD g_wR10;							// R_ohms x 10
BYTE g_bSgnX = '?';						// sign of reactance

// Storage, choices and defaults for user preferences
CONFIG_DATA g_xConf;										// Storage structure
const BYTE g_bUserIdle[USER_IDLE_NUM]	= {0,30,60,90};		
const BYTE g_bCWPitches[CWPitch_NUM]	= {6,10,12,20,24};  
const BYTE g_bVF[VF_NUM] 				= {99,81,66};		
const BYTE g_bFltWd100kHz[FLT_WT_NUM] 	= {0,2,10,40};		

// frequency and band settings
BYTE g_bBandIndex;
DWORD g_dwCurrHz;
const WORD g_wBandBoundaries[BAND_NUM+1]= {9,20,40,60,80, 110,130,170,190,230,260,280,310,400,490,550,650};
const WORD g_wBandInitialFreq[BAND_NUM] = {18,37,53,71,101,120,141,181,211,249,270,282,355,445,503,570};

// Keyboard & key mode flags 
BYTE g_bLongPress = 0;									// global flag for long key press
BYTE g_bUP_DOWN_SelectsIncDigit 	= FALSE;			// select the frequency digit to use with UP/DOWN
BYTE g_bUP_DOWN_SelectsVFOPower		= FALSE;			// select VFO output power with UP/DOWN

// Frequency setting by UP/DOWN keys: cursor column to inrement in Hz
BYTE g_bIncDigit=5;
const INCREMENT_CONTROL g_xIncCtrl[NUM_INCREMENT] =
{
	{15, 1},
	{14, 10},
	{13, 100},		// skip decimal point 
	{11, 1000},
	{10, 10000},
	{9,  100000},
	{8,  1000000},
	{7,  10000000}
};

// PGA DDS gain settings table
const GAIN_DDS g_xGainDds[GAIN_SETTINGS_NUM] =
{
   {PGA_DDS_1_G1_33,PGA_DDS_2_G0_25},  // 1.33x0.25=0.33 Iset = 0.101 amplified Vout = 1.298pp = 6.2 dBm
   {PGA_DDS_1_G1_78,PGA_DDS_2_G0_18},  // 1.78x0.18=0.32 Iset = 0.109 amplified Vout = 1.401pp = 6.9 dBm
   {PGA_DDS_1_G0_50,PGA_DDS_2_G0_62},  // 0.50x0.62=0.31 Iset = 0.116 amplified Vout = 1.490pp = 7.4 dBm
   {PGA_DDS_1_G0_81,PGA_DDS_2_G0_37},  // 0.81x0.37=0.30 Iset = 0.123 amplified Vout = 1.578pp = 7.9 dBm
   {PGA_DDS_1_G1_14,PGA_DDS_2_G0_25},  // 1.14x0.25=0.29 Iset = 0.133 amplified Vout = 1.703pp = 8.6 dBm
   {PGA_DDS_1_G0_43,PGA_DDS_2_G0_62},  // 0.43x0.62=0.27 Iset = 0.145 amplified Vout = 1.860pp = 9.4 dBm
   {PGA_DDS_1_G0_50,PGA_DDS_2_G0_50},  // 0.50x0.50=0.25 Iset = 0.156 amplified Vout = 2.002pp = 10.0 dBm
   {PGA_DDS_1_G0_75,PGA_DDS_2_G0_31},  // 0.75x0.31=0.23 Iset = 0.168 amplified Vout = 2.151pp = 10.6 dBm
   {PGA_DDS_1_G1_78,PGA_DDS_2_G0_12},  // 1.78x0.12=0.21 Iset = 0.181 amplified Vout = 2.313pp = 11.3 dBm
   {PGA_DDS_1_G1_60,PGA_DDS_2_G0_12},  // 1.60x0.12=0.19 Iset = 0.195 amplified Vout = 2.497pp = 11.9 dBm
   {PGA_DDS_1_G0_68,PGA_DDS_2_G0_25},  // 0.68x0.25=0.17 Iset = 0.210 amplified Vout = 2.685pp = 12.6 dBm
   {PGA_DDS_1_G1_23,PGA_DDS_2_G0_12},  // 1.23x0.12=0.15 Iset = 0.225 amplified Vout = 2.876pp = 13.2 dBm
   {PGA_DDS_1_G0_68,PGA_DDS_2_G0_18},  // 0.68x0.18=0.12 Iset = 0.241 amplified Vout = 3.091pp = 13.8 dBm
   {PGA_DDS_1_G1_60,PGA_DDS_2_G0_06},  // 1.60x0.06=0.10 Iset = 0.259 amplified Vout = 3.316pp = 14.4 dBm
   {PGA_DDS_1_G0_56,PGA_DDS_2_G0_12},  // 0.56x0.12=0.07 Iset = 0.278 amplified Vout = 3.562pp = 15.0 dBm
   {PGA_DDS_1_G0_62,PGA_DDS_2_G0_06},  // 0.62x0.06=0.04 Iset = 0.298 amplified Vout = 3.818pp = 15.6 dBm
   {PGA_DDS_1_G0_43,PGA_DDS_2_G0_06},  // 0.43x0.06=0.03 Iset = 0.306 amplified Vout = 3.915pp = 15.8 dBm
};
