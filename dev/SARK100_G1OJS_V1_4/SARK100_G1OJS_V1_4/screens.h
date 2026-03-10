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
// 	FILE NAME: 	screens.h
// 	AUTHOR:		G1OJS - Alan Robinson
// 	DESCRIPTION	
// 	HISTORY
//	NAME   		DATE		REMARKS
//	AJR			Feb 2025	G1OJS - creation
//*****************************************************************************/

#ifndef __Screen_H__
#define __Screen_H__


#include "Lcd.h"
typedef enum {Display_Hz, Display_kHz} SCREEN_FREQ_UNITS;

//-----------------------------------------------------------------------------
//  Public functions
//-----------------------------------------------------------------------------

// functions to display outputs using specific screen layouts
void Screen_Frequency(SCREEN_FREQ_UNITS xDisplayRes);
void Screen_SRXZ (void);
void Screen_Capacitance (void);
void Screen_Inductance (void);
void Screen_Power(WORD vVz);
void Screen_CableLength(WORD wHWkHz);

// low level functions
void Screen_Setup (void);
void Screen_Clear (void);
void Screen_HideCursor(void);
void Screen_CStr(const char * sRomString);
void Screen_Str(char * sString);
void Screen_CStrAtRowCol(BYTE bRow, BYTE bCol, const char * sRomString);
void Screen_StrAtRowCol(BYTE bRow, BYTE bCol, char * sString);


//*****************************************************************************
#endif




