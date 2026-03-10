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
// 	FILE NAME: 	MSG_ENG.C
// 	AUTHOR:		EA4FRB - Melchor Varela
//
// 	DESCRIPTION
//
//	Display texts: English
//
// 	HISTORY
//
//	NAME   		DATE		REMARKS
//
//	MVM	   		DEC 2009	Creation
//
//  AJR			JAN 2025	Align data with refactoring and functional changes	
//*****************************************************************************/
#include "msg_generic.h"
#include "glb_data.h"

BYTE const gBlankStr_16	[] 		= "                ";
BYTE const gBlankStr_4	[] 		= "    ";

BYTE const g_welcome1Str	[] 	= PRODUCT_NAME_STR " ANALYZER";
BYTE const g_welcome2Str	[] 	= "G1OJS " VERSION_STR;


//-----------------------------------------------------------------------------
//  Calibration
//-----------------------------------------------------------------------------
BYTE const gLoad1Str 	    [] = "Connect 10\xf4";
BYTE const gLoad2Str    	[] = "Connect 50\xf4";
BYTE const gLoad3Str    	[] = "Connect 150\xf4";
BYTE const gLoad4Str      	[] = "Connect 220\xf4";
BYTE const gLoad5Str     	[] = "Connect 470\xf4";
BYTE const *gCalLoadStr[] = {
							gLoad1Str,
							gLoad2Str,
							gLoad3Str,
							gLoad4Str,
							gLoad5Str,
							};
							
BYTE const gDisconnectLoadStr	[] = "Disconnect load ";
BYTE const gSettingGainStr		[] = "Setting gain";
BYTE const gCalibratingStr		[] = "Calibrating...";
BYTE const gDoneRestartingStr	[] = "Done: restarting";

//-----------------------------------------------------------------------------
//  Displays
//-----------------------------------------------------------------------------
BYTE const gZeroReactanceStr [] = "Reactance = 0\xf4";
BYTE const gCableLabels1Str [] 	= "Line length:    ";


//-----------------------------------------------------------------------------
//  Dialogues / warnings
//------------------------------------0123456789012345-------------------------
BYTE const gErrorAdjustVfStr	[] = "Error Vf Level";
BYTE const gErrorUncalibratedStr[] = "Uncalibrated!";
BYTE const gPressAnyKeyStr		[] = "Press any key   ";
BYTE const gConfirmStr			[] = "Confirm?";
BYTE const gSwLoadingStr		[] = "FW Loading";
BYTE const gScanCoarseStr		[] = "Coarse:";
BYTE const gScanFineStr			[] = "Fine:  ";
BYTE const gScanThreshStr		[] = "Thresh:";
BYTE const gMinimaStr			[] = "|Z|min/max ";


//-----------------------------------------------------------------------------
//  Menus
//-----------------------------------------------------------------------------
BYTE const gValueSavedStr		[] = "<-saved";
BYTE const gNothingChangedStr	[] = "Nothing changed ";

BYTE const gConfigPcLinkStr	[] 	= "PC Link";
BYTE const gConfigIdleStr	[] 	= "Set Idle Time";
BYTE const gConfigFltWtStr	[] 	= "Set Filter Wt";
BYTE const gConfigCWPitchStr[] 	= "Set CW Pitch";
BYTE const gConfigVFStr		[] 	= "Set Vel factor";
BYTE const gConfigCableLengthStr	[] 	= "Meas line lenth";
BYTE const gConfigDoCalStr	[] 	= "Calibrate";
BYTE const gConfigSwLoadStr	[] 	= "Load Firmware ";
BYTE const *gConfigStr[] = {
							gConfigPcLinkStr,
							gConfigIdleStr,
							gConfigFltWtStr,
							gConfigCWPitchStr,
							gConfigVFStr,
							gConfigCableLengthStr,
							gConfigDoCalStr,
							gConfigSwLoadStr
							};

BYTE const gModeImpStr	[] = "SWR";
BYTE const gModeCapStr	[] = "CAP    ";
BYTE const gModeIndStr	[] = "IND    ";
BYTE const gModeSigStr	[] = "Power lvl (RMS) ";
BYTE const gModeVfoStr	[] = "VFO             ";
BYTE const *gModeStr[] = {
							gModeImpStr,
							gModeCapStr,
							gModeIndStr,
							gModeSigStr,
							gModeVfoStr,
							};

BYTE const gIdleOffStr    [] = "Off";
BYTE const gIdle30SStr    [] = "30 S";
BYTE const gIdle60SStr    [] = "60 S";
BYTE const gIdle90SStr    [] = "90 S";
BYTE const *gIdleStr[] = {
							gIdleOffStr,
							gIdle30SStr,
							gIdle60SStr,
							gIdle90SStr
							};

BYTE const gFlt0Str    	  	[] = "Off";
BYTE const gFlt1Str    		[] = "Light";
BYTE const gFlt2Str    		[] = "Medium";
BYTE const gFlt3Str    		[] = "Strong";
BYTE const *gFltWtStr[] = {
							gFlt0Str,
							gFlt1Str,
							gFlt2Str,
							gFlt3Str
							};
							
BYTE const gVF99Str	       	[] = "99%";
BYTE const gVF81Str	       	[] = "81%";
BYTE const gVF66Str	       	[] = "66%";
BYTE const *gVFStr[] = {
							gVF99Str,
							gVF81Str,
							gVF66Str
							};
							
BYTE const gCWPitch600Str    [] = "600 Hz";
BYTE const gCWPitch1000Str   [] = "1000 Hz";
BYTE const gCWPitch1200Str   [] = "1200 Hz";
BYTE const gCWPitch2000Str   [] = "2000 Hz";
BYTE const gCWPitch2400Str   [] = "2400 Hz";
BYTE const *gCWPitchStr[] = {
							gCWPitch600Str,
							gCWPitch1000Str,
							gCWPitch1200Str,
							gCWPitch2000Str,
							gCWPitch2400Str
							};
							
							
