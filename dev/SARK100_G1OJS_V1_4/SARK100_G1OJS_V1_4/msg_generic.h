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
// 	FILE NAME: 	MSG_GENERIC.H
// 	AUTHOR:		EA4FRB - Melchor Varela
//
// 	DESCRIPTION
//
//	Display texts
//
// 	HISTORY
//
//	NAME   		DATE		REMARKS
//
//	MVM	   		DEC 2009	Creation
//  AJR			JAN 2025	Align data with refactoring and functional changes	
//*****************************************************************************/
#ifndef __MSG_GENERIC_H__
#define __MSG_GENERIC_H__

#include <m8c.h>

extern BYTE const gBlankStr_16[];
extern BYTE const gBlankStr_4[];
extern BYTE const g_welcome1Str[];
extern BYTE const g_welcome2Str[];

//-----------------------------------------------------------------------------
//  Calibration
//-----------------------------------------------------------------------------
extern BYTE const gSettingGainStr[];
extern BYTE const gCalibratingStr[];
extern BYTE const gConfigDoCalStr[];
extern BYTE const gDoneRestartingStr[];
extern BYTE const gDisconnectLoadStr[];
//-----------------------------------------------------------------------------
//  Displays
//-----------------------------------------------------------------------------
extern BYTE const gZeroReactanceStr[];
extern BYTE const gCableLabels1Str[];
//-----------------------------------------------------------------------------
//  Dialogues / warnings
//-----------------------------------------------------------------------------
extern BYTE const gErrorAdjustVfStr[];
extern BYTE const gErrorUncalibratedStr[];
extern BYTE const gPressAnyKeyStr[];
extern BYTE const gConfirmStr[];
extern BYTE const gScanCoarseStr[];
extern BYTE const gScanFineStr[];
extern BYTE const gScanThreshStr[];
extern BYTE const gSwLoadingStr[];
extern BYTE const gValueSavedStr[];
extern BYTE const gNothingChangedStr[];
extern BYTE const gMinimaStr[];
//-----------------------------------------------------------------------------
//  Menus
//-----------------------------------------------------------------------------
extern BYTE const *gModeStr[];
extern BYTE const *gDDSStr[];
extern BYTE const *gIdleStr[];
extern BYTE const *gFltWtStr[];
extern BYTE const *gBandStr[];
extern BYTE const *gConfigStr[];
extern BYTE const *gCalLoadStr[];
extern BYTE const *gCWPitchStr[];
extern BYTE const *gVFStr[];



//-----------------------------------------------------------------------------
#endif
