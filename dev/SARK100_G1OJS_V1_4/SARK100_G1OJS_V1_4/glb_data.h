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
// 	FILE NAME: 	GLB_DATA.H
// 	AUTHOR:		EA4FRB - Melchor Varela
//
// 	DESCRIPTION
//	Global constants and variables
//
// 	HISTORY
//
//	NAME   		DATE		REMARKS
//	MVM	   		DEC 2009	Creation
//  AJR			FEB 2025	Align data with refactoring and functional changes	
//*****************************************************************************/
#ifndef __GLB_DATA_H__
#define __GLB_DATA_H__

#include <stdlib.h>
#include <m8c.h>

#include "PSoCAPI.h"

//-----------------------------------------------------------------------------
//  Version string
//-----------------------------------------------------------------------------
#define VERSION_STR			"V1.4"
#define PRODUCT_NAME_STR	"SARK100"

//-----------------------------------------------------------------------------
//  Defines
//-----------------------------------------------------------------------------

// Calibration definitions
#define CAL_START_kHz		999		// start cal freqs, offset to avoid whole MHz (especially odd ones) as these are bad for calibration repeatability
#define CAL_FREQ_STEP_kHz	5999		// Frequency interval between calibration freqs
#define CAL_FREQS_NUM		11			// Number of calibration frequencies 
#define CAL_LOAD_NUM		4 			// Number of calibration loads
#define CALMG_L				667			// 10R
#define CALMZ_L 			200			//
#define CALMZ_M				1000		// 50R
#define CALMG_M			 	15			// allows small nonzero measurements around matched load to be processed
#define CALMG_H				500			// 150R
#define CALMZ_H 			3000		//
#define CALMG_H2			630			// 220R
#define CALMZ_H2 			4400		//
#define VF_REFERENCE_LEVEL	3900 		// Full scale value with a little headroom for upward fluctuations with frequency
									
// Measurements
#define VR_OVER_VF_MAX		996			// largest value of wUNITYxVr/Vf that gives SWRx100 storable in WORD. 996 corresponds to VSWR=499.
#define SWR_BW_THRESH		200			// SWR threshold (SWRx100) for swr BW and match freq scanning
#define SWR_BW_THRESH_PROXY 333			// for fast scanning using Vr as a proxy for SWR
#define	MEASURE_PERIOD		32			// Measurements sample period, units (1/16 sec)
#define BAND_FREQ_ToHz		100000		// Frequency multiplier for frequency tables
#define DIZZLING_OFFSET_PERCENT	5		// Percent change to apply to frequency when dizzling 

// Port settings
#define VfPort AMUX4_ADC_PORT0_1
#define VrPort AMUX4_ADC_PORT0_3
#define VzPort AMUX4_ADC_PORT0_5
#define VaPort AMUX4_ADC_PORT0_7

// Constants for user preferences
#define USER_IDLE_NUM 		4			// Number of idle timeout settings
#define FLT_WT_NUM 			4			// Number of filter weight settings
#define CWPitch_NUM			5			// Number of CW Pitch settings
#define VF_NUM 				3			// Number index of velocity factors for line length display
typedef struct							// Configuration data stored in EEPROM
{
	BYTE bUserIdle;
	BYTE bCWPitch;
	BYTE bVF;
	BYTE bFltWt;
} CONFIG_DATA;

// Frequency and Bands 
#define FREQ_MIN_Hz			1000000
#define FREQ_MAX_Hz			60000000
#define FREQ_ABS_MAX_Hz		61000000
#define BAND_NUM 			16			// Number of bands

// Code readability definitions
#define	wUNITY				1000		// General use multiplier for storing float in WORD
#define WORD_MAX			65535		// largest number available in WORD
typedef enum {PWROFF, PWRON} POWER_STATE;
typedef enum {MODZ,  MODGAMMA} RATIO_TYPE;	
typedef enum {STEPUP,STEPDOWN} STEP_DIRECTION;

//Keyboard / UI settings
#define TIME_WAIT_KEY_S				30		// Time to wait for key when dialog, units of seconds
#define TIME_DELAY_TEXT				3		// Temporary screen texts cancellable by keypress: units of 1 sec
#define TIME_FLASH_MSG				96		// Time to display flash (not important) messages, units 1/64 seconds up to 255
#define BAND_ATPWRON				6		// initial band
#define KEY_DEBOUNCE_TIME			4		// Units of 1/16 sec
#define LONG_PRESS_DET_TIME_S		2		// Units of seconds
#define SPEED_KEY_CLICKRATE_64ths	2		// Units of 1/64th s
#define NUM_INCREMENT				8		// Number of increment cursor positions

typedef struct							// Cursor-based frequency increment data
{
	BYTE bCol;
	DWORD dwInc;
} INCREMENT_CONTROL;

// DDS Gain Control
#define GAIN_SETTINGS_NUM	17			// Number of gain settings for DDS PGAs 
typedef struct							// PGA's DDS gain settings
{
	BYTE bGain1;
	BYTE bGain2;
} GAIN_DDS;

// Menu items definitions
// Menu strings are in msg_eng.c (or msg_xxx.c for other languages)
typedef enum {							// Configuration menu
	CONFIG_PCLINK = 0,
	CONFIG_Idle,
	CONFIG_FltWt,
	CONFIG_CWPitch,
	CONFIG_VF,
	CONFIG_CableLength,
	CONFIG_CALIB,
	CONFIG_SW_LOAD,
	
	CONFIG_NUM
} CONFIG_DEFS;

typedef enum {							// Mode menu
	MODE_IMP = 0,
	MODE_CAP,
	MODE_IND,
	MODE_SIG,
	MODE_VFO,

	MODE_NUM
} MODE_DEFS;



// Counters
extern volatile BYTE g_bIdleCounter;
extern volatile BYTE g_bMeasureCounter;
extern volatile BYTE g_bDebounceCounter;
extern volatile BYTE g_bLongPressKeyCounter;
extern volatile BYTE g_bSixtyFourthSecondCounter;

// 
extern 			BYTE g_buffer16[16];
extern 			BYTE g_bMode;

// Calibration
extern const 	WORD 		g_wCalFreqs[CAL_FREQS_NUM];
//extern const 	WORD 		g_wBrgRtsTrue[CAL_LOAD_NUM][2];
extern 		 	BYTE 		g_bIsCalibrated;
//extern 		 	WORD 		g_wBrgRtsArr[CAL_LOAD_NUM][2];
extern 		 signed char	g_scCalTable[CAL_FREQS_NUM][CAL_LOAD_NUM][2];

// Measurements
extern 			WORD 	g_wBrgRts[2];
extern 			WORD 	g_wSwr100;
extern 			WORD 	g_wZ10;
extern 			WORD 	g_wX10;
extern 			WORD 	g_wR10;
extern 			BYTE 	g_bSgnX;

//User Preferences
extern const 	BYTE 	g_bCWPitches[CWPitch_NUM];
extern const 	BYTE 	g_bFltWd100kHz[FLT_WT_NUM];
extern const 	BYTE 	g_bUserIdle[USER_IDLE_NUM];
extern const	BYTE 	g_bVF[VF_NUM];
extern CONFIG_DATA 		g_xConf;

//Frequencies and Bands
extern const 	WORD 	g_wBandBoundaries[BAND_NUM+1];
extern const 	WORD 	g_wBandInitialFreq[BAND_NUM];
extern 			BYTE 	g_bBandIndex;
extern 			DWORD 	g_dwCurrHz;

//Keyboard / UI
extern const 	INCREMENT_CONTROL g_xIncCtrl[NUM_INCREMENT];
extern 			BYTE 	g_bUP_DOWN_SelectsIncDigit;
extern 			BYTE 	g_bIncDigit;
extern 			BYTE 	g_bUP_DOWN_SelectsVFOPower;
extern 			BYTE 	g_bLongPress;

// DDS Gain Control
extern const GAIN_DDS 	g_xGainDds[GAIN_SETTINGS_NUM];
extern 			BYTE 	g_bDDS_GainStep;


//-----------------------------------------------------------------------------
#endif