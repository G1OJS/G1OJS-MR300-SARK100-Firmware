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
// 	FILE NAME: 	scans.h
// 	AUTHOR:		G1OJS - Alan Robinson
// 	DESCRIPTION	Scanning functions
// 	HISTORY
//	NAME   		DATE		REMARKS	
//	AJR			FEB 2025	G1OJS  - creation 
//  AJR			FEB 2025
//*****************************************************************************/


typedef enum {SCANPARAM_SWR, SCANPARAM_Vruc, SCANPARAM_Vzuc, SCANPARAM_Vauc, SCANPARAM_VzucORVauc} SCAN_SCANPARAM;
typedef enum {SCANDIRECTION_UP, SCANDIRECTION_DOWN} SCAN_DIRECTION;

void Do_VSWR_Scan (void);
void Do_LineLength_Scan (void);
static void findGlobalMinimum(SCAN_SCANPARAM xScanParam, DWORD dwFreqLimitHz, DWORD dwStepHz);
static BYTE findNextGoodNotch(SCAN_SCANPARAM xScanParam, DWORD dwFreqLimitHz, DWORD dwStepHz);
static void findSWRThreshold(SCAN_DIRECTION xScanDirection, DWORD dwFreqLimitHz, DWORD dwStepHz);
