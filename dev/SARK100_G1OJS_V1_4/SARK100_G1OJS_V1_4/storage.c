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
// 	FILE NAME: 	STORAGE.C
// 	AUTHOR:		EA4FRB - Melchor Varela
//
// 	DESCRIPTION
//
//	EEPROM storage functions
//
// 	HISTORY
//
//	NAME   		DATE		REMARKS
//
//	MVM	   		DEC 2009	Creation
//
//  AJR			JAN 2025	Align data with refactoring and functional changes	
//*****************************************************************************/
#include "storage.h"

#include <stdlib.h>
#include <string.h>
#include <m8c.h>
#include "PSoCAPI.h"

#include "glb_data.h"
#include "control.h"
#include "screens.h"
#include "morse.h"
#include "msg_generic.h"
#include "timers.h"

//-----------------------------------------------------------------------------
//  Macros
//-----------------------------------------------------------------------------
#define offsetof(st, m) \
    ((size_t) ( (char *)&((st *)(0))->m - (char *)0 ))

//-----------------------------------------------------------------------------
//  Defines
//-----------------------------------------------------------------------------
#define MAGIC_NUMBER	0x55ab

//-----------------------------------------------------------------------------
//  Typedefs
//-----------------------------------------------------------------------------
typedef struct							// Structure stored in EEPROM
{
	INT rec_scCalTable[CAL_FREQS_NUM][CAL_LOAD_NUM][2];				// Cal table
	BYTE rec_bDDS_GainStep;											// DDS Gain step
	BYTE rec_bIsCalibrated;											// Identify defaults or calibrated
	CONFIG_DATA rec_xConf;											// Configuration data
	WORD rec_wMagic;												// Integrity control
	
} RECORD_DATA;
// 
// Size = CalFreqs x CalLoads x (2+2) + 1+ 1+ 2 + 2
//		= CalFreqs x CalLoads x 4 + 6
//
//-----------------------------------------------------------------------------
//  FUNCTION NAME:	STR_SaveCalibration
//  DESCRIPTION:	Save calibration data in EEPROM
//  ARGUMENTS: 		none.
//  RETURNS:  		none.
//-----------------------------------------------------------------------------
void STR_SaveCalibration ( void )
{
	E2PROM_bE2Write(offsetof(RECORD_DATA,rec_scCalTable), (unsigned char*) &g_scCalTable, sizeof(g_scCalTable), 25);
	E2PROM_bE2Write(offsetof(RECORD_DATA,rec_bDDS_GainStep), &g_bDDS_GainStep, sizeof(g_bDDS_GainStep), 25);
	E2PROM_bE2Write(offsetof(RECORD_DATA,rec_bIsCalibrated), &g_bIsCalibrated, sizeof(g_bIsCalibrated), 25);
}
//-----------------------------------------------------------------------------
//  FUNCTION NAME:	STR_SaveConfig
//  DESCRIPTION:	Save configuration data
//  ARGUMENTS:    	none.
//  RETURNS:		none.
//-----------------------------------------------------------------------------
void STR_SaveConfig ( void )
{
	E2PROM_bE2Write(offsetof(RECORD_DATA,rec_xConf), (unsigned char*)&g_xConf, sizeof(g_xConf), 25);
}
//-----------------------------------------------------------------------------
//  FUNCTION NAME:	STR_Restore
//  DESCRIPTION:	Restores EEPROM data. In case of not initialized save defaults
//  ARGUMENTS:  	none.
//  RETURNS: 		none.
//-----------------------------------------------------------------------------
void STR_Restore ( void )
{
	WORD magic;
	E2PROM_E2Read(offsetof(RECORD_DATA,rec_wMagic), (unsigned char*) &magic, sizeof(WORD));
	if (magic != MAGIC_NUMBER)
	{	
		STR_SaveCalibration();  // Save defaults set in glb_data.c
		g_xConf.bCWPitch=0;
		g_xConf.bFltWt=0;
		g_xConf.bUserIdle=0;
		g_xConf.bVF=2;
		STR_SaveConfig();		// Save defaults set above
		magic = MAGIC_NUMBER;
		E2PROM_bE2Write(offsetof(RECORD_DATA,rec_wMagic), (unsigned char*) &magic, sizeof(magic), 25);
	}
	
	E2PROM_E2Read(offsetof(RECORD_DATA,rec_scCalTable), (unsigned char*) &g_scCalTable, sizeof(g_scCalTable));
	E2PROM_E2Read(offsetof(RECORD_DATA,rec_bDDS_GainStep), (unsigned char*)&g_bDDS_GainStep, sizeof(g_bDDS_GainStep));
	E2PROM_E2Read(offsetof(RECORD_DATA,rec_bIsCalibrated), &g_bIsCalibrated, sizeof(g_bIsCalibrated));
	E2PROM_E2Read(offsetof(RECORD_DATA,rec_xConf), (unsigned char*)&g_xConf, sizeof(g_xConf));
	
	if (g_bIsCalibrated == FALSE)		// If not calibrated presents warning text
	{
		Screen_Clear(); Screen_CStrAtRowCol(0,0, gErrorUncalibratedStr);
		Morse_U();
		Delay_64ths(TIME_FLASH_MSG);
	}

}
