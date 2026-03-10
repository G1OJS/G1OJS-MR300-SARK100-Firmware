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
// 	FILE NAME: 	control.h
// 	AUTHOR:		G1OJS - Alan Robinson
// 	DESCRIPTION	
// 	HISTORY
//	NAME   		DATE		REMARKS
//	AJR			Feb 2025	G1OJS - creation
//*****************************************************************************
#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <m8c.h>        		// Part specific constants and macros
#include "PSoCAPI.h"    		// PSoC API definitions for all User Modules
#include "glb_data.h"

//-----------------------------------------------------------------------------
//  Prototypes
//-----------------------------------------------------------------------------
extern void SetPowerDigitalAndAnalogue(POWER_STATE pwrState);
extern void SetDDSGain(void);
extern void SetPowerBackLightAndOsc(POWER_STATE pwrState);
extern void Set_DDS(DWORD dwFreq);
//-----------------------------------------------------------------------------
#endif