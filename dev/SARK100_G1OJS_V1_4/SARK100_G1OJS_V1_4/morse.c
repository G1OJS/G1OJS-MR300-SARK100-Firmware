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
// 	FILE NAME: 	morse.c
// 	AUTHOR:		G1OJS - Alan Robinson
// 	DESCRIPTION	Uses the PWM_Buzzer to create morse code notifications
// 	HISTORY
//	NAME   		DATE		REMARKS
//	AJR			Feb 2025	G1OJS  - creation
//*****************************************************************************/
#include "morse.h"
#include "control.h"
#include "glb_data.h"
#include "timers.h"

#define CW_WPM 20
#define CW_DitNum 1200
#define CW_DahNum 3600


// to do - define MORSE_ENSIGN DIT|DAH, have one function Morse(MORSE_ENSIGN) OR classic Morse("-.-");

void Morse_U(void)			{ Morse_Dit(); Morse_Dit(); Morse_Dah();		}
void Morse_K(void)			{ Morse_Dah(); Morse_Dit(); Morse_Dah();		}
void Morse_Err(void)		{ BYTE bCnt=0; for(;bCnt<8;bCnt++) {Morse_Dit(); }		}
void Morse_End(void)		{ BYTE bCnt=0; for(;bCnt<3;bCnt++) {Morse_Dit();} Morse_Dah(); Morse_Dit(); Morse_Dah(); }

void Morse_Dit(void) 		{ Tone(100*g_bCWPitches[g_xConf.bCWPitch], CW_DitNum / CW_WPM); Tone(0, CW_DitNum / CW_WPM);	}
void Morse_Dah(void)		{ Tone(100*g_bCWPitches[g_xConf.bCWPitch], CW_DahNum / CW_WPM); Tone(0, CW_DitNum / CW_WPM);  }

void Tone(WORD wPitch_Hz, BYTE bLen_ms)
{
	#define ticks_per_us 15625
	BYTE b64ths;
	if(wPitch_Hz>16000)  wPitch_Hz=16000; 	
	if(wPitch_Hz>0) {								// b_Pitch_kHz==0 signifies silent delay
 		PWM8_BUZZ_WritePeriod(32000/wPitch_Hz);		// Set period in clock ticks (clock 32Khz). Must be at least 2!
		PWM8_BUZZ_WritePulseWidth(16000/wPitch_Hz);	// Set pulse width to generate a 50% duty cycle
		PWM8_BUZZ_DisableInt();						// Ensure interrupt is disabled
		PWM8_BUZZ_Start(); 							// Start the buzzer
	}
	b64ths=(BYTE)(  ((DWORD)bLen_ms*1000) / ticks_per_us  );
	if(b64ths<1) b64ths=1;
	Delay_64ths(b64ths);							// wait approx bLen_ms
	if(wPitch_Hz>0) PWM8_BUZZ_Stop();				// Stop the buzzer if we started it	
}
