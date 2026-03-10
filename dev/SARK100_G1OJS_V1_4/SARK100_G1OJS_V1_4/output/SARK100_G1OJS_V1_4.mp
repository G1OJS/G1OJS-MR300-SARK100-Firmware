Project:      output/SARK100_G1OJS_V1_4
Build Number: 0
Date:         Tue Apr  1 21:31:53 2025
Compiler:     ICCM8C
Version:      7.02.004

  ROM 87% full. 28506 out of 32768 bytes used (does not include absolute areas).
  RAM 16% full. 273 bytes used (does not include stack usage).

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                             lit   01A0   06E5 =   1765. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       01A0  _g_bUserIdle
       01A4  _g_bCWPitches
       01A9  _g_bVF
       01AC  _g_bFltWd100kHz
       01B0  _g_wBandBoundaries
       01D2  _g_wBandInitialFreq
       01F2  _g_xIncCtrl
       021A  _g_xGainDds
       023C  _gBlankStr_16
       024D  _gBlankStr_4
       0252  _g_welcome1Str
       0263  _g_welcome2Str
       026E  _gLoad1Str
       027A  _gLoad2Str
       0286  _gLoad3Str
       0293  _gLoad4Str
       02A0  _gLoad5Str
       02AD  _gDisconnectLoadStr
       02BE  _gSettingGainStr
       02CB  _gCalibratingStr
       02DA  _gDoneRestartingStr
       02EB  _gZeroReactanceStr
       02FA  _gCableLabels1Str
       030B  _gErrorAdjustVfStr
       031A  _gErrorUncalibratedStr
       0328  _gPressAnyKeyStr
       0339  _gConfirmStr
       0342  _gSwLoadingStr
       034D  _gScanCoarseStr
       0355  _gScanFineStr
       035D  _gScanThreshStr
       0365  _gMinimaStr
       0371  _gValueSavedStr
       0379  _gNothingChangedStr
       038A  _gConfigPcLinkStr
       0392  _gConfigIdleStr
       03A0  _gConfigFltWtStr
       03AE  _gConfigCWPitchStr
       03BB  _gConfigVFStr
       03CA  _gConfigCableLengthStr
       03DA  _gConfigDoCalStr
       03E4  _gConfigSwLoadStr
       03F3  _gModeImpStr
       03F7  _gModeCapStr
       03FF  _gModeIndStr
       0407  _gModeSigStr
       0418  _gModeVfoStr
       0429  _gIdleOffStr
       042D  _gIdle30SStr
       0432  _gIdle60SStr
       0437  _gIdle90SStr
       043C  _gFlt0Str
       0440  _gFlt1Str
       0446  _gFlt2Str
       044D  _gFlt3Str
       0454  _gVF99Str
       0458  _gVF81Str
       045C  _gVF66Str
       0460  _gCWPitch600Str
       0467  _gCWPitch1000Str
       046F  _gCWPitch1200Str
       0477  _gCWPitch2000Str
       047F  _gCWPitch2400Str
       0487  pclink.c:bufCmdOn
       048A  pclink.c:bufCmdOff
       048E  pclink.c:bufCmdFreq
       0493  pclink.c:bufCmdMeasImp
       0497  pclink.c:bufCmdMeasRaw
       049B  pclink.c:bufCmdScan
       04A0  pclink.c:bufCmdScanRaw
       04A6  pclink.c:bufCmdScanBR
       04AD  pclink.c:bufCmdScanV
       04B3  pclink.c:bufCmdSweep
       04B9  pclink.c:bufWelcome
       04D7  pclink.c:bufCmdPrompt
       04DC  pclink.c:bufWaitLink
       04E9  pclink.c:bufErrCmdNotFound1
       04F5  pclink.c:bufErrCmdNotFound2
       0509  pclink_cmds.c:bufOk
       0510  pclink_cmds.c:bufStart
       051A  pclink_cmds.c:bufEnd
       0520  pclink_cmds.c:bufErrFreqNotSet
       0538  pclink_cmds.c:bufErrExpectFreq
       0555  pclink_cmds.c:bufErrExpectStep
       0572  pclink_cmds.c:bufErrInvalidFreq
       058A  pclink_cmds.c:bThousandCommaStr
       058F  pclink_cmds.c:bThousandStr
       0593  pclink_cmds.c:bCommaStr
       05C9  LoadConfigTBL_sark100_g1ojs_v1_4_Bank0
       0656  LoadConfigTBL_sark100_g1ojs_v1_4_Bank1
       0699  LoadConfigTBL_sark100_g1ojs_v1_4_Ordered
       0784  __ctype
       0885  __lit_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                        func_lit   0885   0002 =      2. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       0885  __func_lit_start
       0887  __func_lit_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                     psoc_config   0887   006B =    107. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       0887  LoadConfigInit
       0887  _LoadConfigInit
       0897  _LoadConfig_sark100_g1ojs_v1_4
       0897  LoadConfig_sark100_g1ojs_v1_4
       08F2  __psoc_config_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                     UserModules   08F2   0904 =   2308. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       08F2  _UART_TX_ISR
       08F3  _UART_RX_ISR
       0967  UART_EnableInt
       0967  _UART_EnableInt
       096E  UART_DisableInt
       096E  _UART_DisableInt
       0975  _UART_SetTxIntMode
       0975  UART_SetTxIntMode
       0987  _UART_Start
       0987  UART_Start
       0990  _UART_Stop
       0990  UART_Stop
       0997  _UART_SendData
       0997  UART_SendData
       099A  _UART_bReadTxStatus
       099A  _bUART_ReadTxStatus
       099A  UART_bReadTxStatus
       099A  bUART_ReadTxStatus
       099D  UART_bReadRxData
       099D  _bUART_ReadRxData
       099D  _UART_bReadRxData
       099D  bUART_ReadRxData
       09A0  bUART_ReadRxStatus
       09A0  UART_bReadRxStatus
       09A0  _bUART_ReadRxStatus
       09A0  _UART_bReadRxStatus
       09A3  UART_TxIntMode
       09A3  _UART_TxIntMode
       09B6  UART_IntCntl
       09B6  _UART_IntCntl
       09E1  _UART_PutSHexByte
       09E1  UART_PutSHexByte
       09F4  _UART_PutSHexInt
       09F4  UART_PutSHexInt
       09FB  _UART_PutChar
       09FB  UART_PutChar
       0A03  UART_cGetChar
       0A03  _UART_cGetChar
       0A0B  _UART_cReadChar
       0A0B  UART_cReadChar
       0A22  _UART_iReadChar
       0A22  UART_iReadChar
       0A38  UART_PutString
       0A38  _UART_PutString
       0A4A  _UART_Write
       0A4A  UART_Write
       0A72  UART_CWrite
       0A72  _UART_CWrite
       0A9E  UART_CPutString
       0A9E  _UART_CPutString
       0AB0  _UART_PutCRLF
       0AB0  UART_PutCRLF
       0AB9  UART_CmdReset
       0AB9  _UART_CmdReset
       0ACF  UART_bCmdCheck
       0ACF  _UART_bCmdCheck
       0AD7  UART_bErrCheck
       0AD7  _UART_bErrCheck
       0AE2  _UART_bCmdLength
       0AE2  UART_bCmdLength
       0AE8  _UART_szGetParam
       0AE8  UART_szGetParam
       0B47  UART_szGetRestOfParams
       0B47  _UART_szGetRestOfParams
       0B52  _PWM8_BUZZ_ISR
       0B53  PWM8_BUZZ_EnableInt
       0B53  _PWM8_BUZZ_EnableInt
       0B57  PWM8_BUZZ_DisableInt
       0B57  _PWM8_BUZZ_DisableInt
       0B5B  _PWM8_BUZZ_Start
       0B5B  PWM8_BUZZ_Start
       0B5F  _PWM8_BUZZ_Stop
       0B5F  PWM8_BUZZ_Stop
       0B63  PWM8_BUZZ_WritePeriod
       0B63  _PWM8_BUZZ_WritePeriod
       0B66  PWM8_BUZZ_WritePulseWidth
       0B66  _PWM8_BUZZ_WritePulseWidth
       0B69  bPWM8_BUZZ_ReadPulseWidth
       0B69  _bPWM8_BUZZ_ReadPulseWidth
       0B69  _PWM8_BUZZ_bReadPulseWidth
       0B69  PWM8_BUZZ_bReadPulseWidth
       0B6C  PWM8_BUZZ_bReadCounter
       0B6C  bPWM8_BUZZ_ReadCounter
       0B6C  _PWM8_BUZZ_bReadCounter
       0B6C  _bPWM8_BUZZ_ReadCounter
       0B99  PGA_DDS_2_SetPower
       0B99  _PGA_DDS_2_SetPower
       0B99  PGA_DDS_2_Start
       0B99  _PGA_DDS_2_Start
       0BAB  PGA_DDS_2_SetGain
       0BAB  _PGA_DDS_2_SetGain
       0BD5  _PGA_DDS_2_Stop
       0BD5  PGA_DDS_2_Stop
       0BD9  PGA_DDS_1_Start
       0BD9  _PGA_DDS_1_Start
       0BD9  PGA_DDS_1_SetPower
       0BD9  _PGA_DDS_1_SetPower
       0BEB  PGA_DDS_1_SetGain
       0BEB  _PGA_DDS_1_SetGain
       0C15  PGA_DDS_1_Stop
       0C15  _PGA_DDS_1_Stop
       0C19  PGA_ADC_Start
       0C19  _PGA_ADC_SetPower
       0C19  _PGA_ADC_Start
       0C19  PGA_ADC_SetPower
       0C2B  _PGA_ADC_SetGain
       0C2B  PGA_ADC_SetGain
       0C55  _PGA_ADC_Stop
       0C55  PGA_ADC_Stop
       0C59  _LCD_PrCString
       0C59  LCD_PrCString
       0C6B  LCD_HEX_STR
       0C7B  _LCD_PrHexByte
       0C7B  LCD_PrHexByte
       0C8E  LCD_PrHexInt
       0C8E  _LCD_PrHexInt
       0C95  _LCD_PrString
       0C95  LCD_PrString
       0CA7  _LCD_WriteData
       0CA7  LCD_WriteData
       0CA7  _LCD_Write_Data
       0CA7  LCD_Write_Data
       0CBB  LCD_Control
       0CBB  _LCD_Control
       0D6B  LCD_Start
       0D6B  _LCD_Start
       0D6B  LCD_Init
       0D6B  _LCD_Init
       0DDA  LCD_ROW_OFFSET
       0DDE  LCD_Position
       0DDE  _LCD_Position
       0DEE  _LCD_Delay50uTimes
       0DEE  LCD_Delay50uTimes
       0DF4  LCD_Delay50u_Table
       0DFC  _LCD_Delay50u
       0DFC  LCD_Delay50u
       0E10  _LCD_DrawBG
       0E41  _LCD_DrawVBG
       0E75  _LCD_InitVBG
       0E75  LCD_InitVBG
       0EA6  LCD_BG_TYPE1
       0EAD  LCD_BG_TYPE2
       0EB4  LCD_InitBG
       0EB4  _LCD_InitBG
       0EE8  _E2PROM_Start
       0EE8  E2PROM_Start
       0EE9  _E2PROM_Stop
       0EE9  E2PROM_Stop
       0EEA  _E2PROM_bE2Write
       0EEA  E2PROM_bE2Write
       0EF6  _E2PROM_E2Read
       0EF6  E2PROM_E2Read
       0F01  _AMUX4_ADC_InputSelect
       0F01  AMUX4_ADC_InputSelect
       0F1C  AMUX4_ADC_Start
       0F1C  _AMUX4_ADC_Start
       0F1D  _AMUX4_ADC_Stop
       0F1D  AMUX4_ADC_Stop
       0F1E  _ADCINC12_CNT_ISR
       0F21  _ADCINC12_TMR_ISR
       0F95  _ADCINC12_Start
       0F95  ADCINC12_Start
       0F95  _ADCINC12_SetPower
       0F95  ADCINC12_SetPower
       0FA9  _ADCINC12_Stop
       0FA9  ADCINC12_Stop
       0FAD  _ADCINC12_GetSamples
       0FAD  ADCINC12_GetSamples
       0FD1  ADCINC12_StopAD
       0FD1  _ADCINC12_StopAD
       0FE9  ADCINC12_fIsData
       0FE9  _ADCINC12_fIsData
       0FE9  _ADCINC12_fIsDataAvailable
       0FE9  ADCINC12_fIsDataAvailable
       0FEF  _ADCINC12_iGetData
       0FEF  ADCINC12_iGetData
       0FF7  ADCINC12_ClearFlag
       0FF7  _ADCINC12_ClearFlag
       0FFE  bE2Write
       0FFE  _bE2Write
       1191  E2Read
       1191  _E2Read
       11F6  __UserModules_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            text   11F6   5394 =  21396. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       11F6  _MeasureBrgRts
       11F6  __text_start
       12FB  _DDS_Autolevel
       1375  _wRead_ADC_DDSON
       13B7  _wRead_ADC_DDSUNCHANGED
       1415  _wDivide
       152C  _Set_DDS
       1541  _SetPowerDigitalAndAnalogue
       15CE  _SetDDSGain
       1615  _SetPowerBackLightAndOsc
       1635  correctionmodel.c:min_long
       168F  correctionmodel.c:max_long
       16E9  _MeasureUncorrectedRatios
       18FD  correctionmodel.c:iRatioFromCalibration
       1D0B  _CorrectBrgRts
       1ED5  _wLincor
       203B  _wCalRatio
       22D7  _wTrueLoadRatios
       237C  correctionmodel.c:lLinMap
       247F  _DDS_Init
       249F  _DDS_Set
       259D  dds.c:divluh1
       2809  dds.c:muldwu1
       2A9E  derive.c:max_WORD
       2ACD  _Do_SZRX_Calcs
       2E64  derive.c:Calc_Sqrt
       2FAF  _KEYPAD_Get
       303F  _KEYPAD_WaitKey
       3082  _KEYPAD_Scan
       311E  _KEYPAD_SysSuspendAndWakeFromPress
       3176  _main
       3339  main.c:handleKeypress
       3502  main.c:PowerSave
       3533  main.c:CycleBand
       35D9  main.c:ChangeCurrHz
       37FD  main.c:cycleParameter
       38AD  main.c:Do_DizzlingMeasureCorrectCalc
       39A5  main.c:Do_Config_Menu
       3AF2  main.c:Config_ByteSettingItem
       3BF2  main.c:Load_Firmware
       3C13  _getUserConfirmation
       3C4F  _MeasureCorrectCalc
       3C59  _FormatNumberRJ
       3DD7  _Morse_U
       3DDE  _Morse_K
       3DE5  _Morse_Err
       3DFB  _Morse_End
       3E17  _Morse_Dit
       3E5F  _Morse_Dah
       3EA7  _Tone
       3F7B  _PcLink
       41D5  _Cmd_On
       41F6  _Cmd_Off
       420E  _Cmd_Freq
       427C  _pcLinkPrintToSerial
       451F  _pcLinkFreqScan
       49FB  _getFreqArg
       4A49  _PrintWdToSerial
       4A88  _filter
       4B80  _Do_VSWR_Scan
       4F47  _Do_LineLength_Scan
       50E7  scans.c:findGlobalMinimum
       5212  scans.c:findNextGoodNotch
       5411  scans.c:findSWRThreshold
       54D1  _Screen_Clear
       54D9  _Screen_HideCursor
       54E1  _Screen_CStr
       54F1  _Screen_Str
       5501  _Screen_CStrAtRowCol
       551C  _Screen_StrAtRowCol
       5537  _Screen_Frequency
       562C  _Screen_SRXZ
       57E9  _Screen_Capacitance
       59B9  _Screen_Inductance
       5B3F  _Screen_Power
       5D46  _Screen_CableLength
       5DF9  screens.c:FormatNumberRJ_Sliding
       5F4F  _STR_SaveCalibration
       5FAB  _STR_SaveConfig
       5FCB  _STR_Restore
       60B5  _SleepTimerINT
       6125  _Delay_64ths
       6141  __divmod_16X16_16
       618F  __divmod_32X32_32
       61CC  __divmodu_16X16_16
       61D7  __divmodu_32X32_32
       61E6  __i_div16_block_util
       621F  __i_div32_block_util
       6282  __i_div32_not_util
       629A  __i_mulu8_block_util
       62B7  __mul16
       62F5  __mul_32X32_32
       637B  _cstrcmpLMM
       6386  _cstrcmp
       63B1  _atol
       6511  _isdigit
       6532  _isspace
       6553  _abs
       65FB  __xidata_end
       658A  __text_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                  BootLoaderArea   7400   0B70 =   2928. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       745C  _Boot_ASCIItoBYTE
       74B9  _Boot_UART_cGetChar
       74DF  _Boot_UART_PutChar
       74F9  _Boot_UART_CPutString
       7527  _Boot_PerformWrite
       783C  _BootLoader
       7AC3  _Boot_LoadConfigInit
       7AC3  Boot_LoadConfigInit
       7CB0  _Boot_RxD_Start
       7CB0  Boot_RxD_Start
       7CB5  _Boot_Counter_Stop
       7CB5  Boot_Counter_Stop
       7CB9  Boot_RxD_bReadRxData
       7CB9  _Boot_RxD_bReadRxData
       7CBC  _Boot_RxD_bReadRxStatus
       7CBC  Boot_RxD_bReadRxStatus
       7CBF  _Boot_TxD_Start
       7CBF  Boot_TxD_Start
       7CC4  Boot_TxD_SendData
       7CC4  _Boot_TxD_SendData
       7CC7  Boot_TxD_bReadTxStatus
       7CC7  _Boot_TxD_bReadTxStatus
       7CCA  Boot_Counter_DisableInt
       7CCA  _Boot_Counter_DisableInt
       7CCE  Boot_Counter_Start
       7CCE  _Boot_Counter_Start
       7CD2  Boot_Counter_WritePeriod
       7CD2  _Boot_Counter_WritePeriod
       7CD8  Boot_Counter_WriteCompareValue
       7CD8  _Boot_Counter_WriteCompareValue
       7CDE  Boot_Counter_wReadCounter
       7CDE  _Boot_Counter_wReadCounter
       7D1C  _Boot_Is_Program_Good
       7D1C  Boot_Is_Program_Good
       7D43  _FlashCheckSum
       7D43  FlashCheckSum
       7D8B  bFlashWriteBlock
       7D8B  _bFlashWriteBlock
       7DF1  _FlashBlockLocal1
       7E33  _FlashBlockLocal2
       7E85  _FlashBlockLocal3
       7F15  FlashReadBlock
       7F15  _FlashReadBlock
       7F70  __BootLoaderArea_end

Area                               Start  End    Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                             TOP   00A6   017B =    379. bytes (abs,ovr,rom)

       Addr  Global Symbol
      -----  --------------------------------
       00A6  __Start
       0178  bGetPowerSetting
       0178  _bGetPowerSetting

Area                               Start  End    Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                          memory   7200   7400 =  29696. bytes (abs,ovr,rom)

       Addr  Global Symbol
      -----  --------------------------------
       7200  _BootCheckSumBlock
       73FE  _LastBlock_To_Check

Area                               Start  End    Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                      SSCParmBlk   00F7   0100 =    256. bytes (abs,ovr,ram)

Area                               Start  End    Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                     E2PROM_AREA   7200   7380 =  29568. bytes (abs,ovr,rom)

       Addr  Global Symbol
      -----  --------------------------------
       7200  E2PROM_RESERVED
       7200  _E2PROM_RESERVED

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                    InterruptRAM   0000   000B =     11. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0000  UART_fStatus
       0000  _UART_fStatus
       0001  _UART_bRxCnt
       0001  UART_bRxCnt
       0002  _NO_SHADOW
       0002  _Port_2_Data_SHADE
       0002  Port_2_Data_SHADE
       0002  NO_SHADOW
       0003  Port_2_DriveMode_0_SHADE
       0003  _Port_2_DriveMode_0_SHADE
       0004  Port_2_DriveMode_1_SHADE
       0004  _Port_2_DriveMode_1_SHADE
       0005  ADCINC12_cTimerU
       0006  ADCINC12_cCounterU
       0007  ADCINC12_iIncr
       0007  _ADCINC12_iIncr
       0009  _ADCINC12_fIncr
       0009  ADCINC12_fIncr
       000A  ADCINC12_bIncrC

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                        UART_RAM   0000   0021 =     33. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0063  UART_aRxBuffer
       0063  _UART_aRxBuffer

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
               virtual_registers   00A3   000F =     15. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       00A3  __rZ
       00A4  __rY
       00A5  __rX
       00A6  __r11
       00A7  __r10
       00A8  __r9
       00A9  __r8
       00AA  __r7
       00AB  __r6
       00AC  __r5
       00AD  __r4
       00AE  __r3
       00AF  __r2
       00B0  __r1
       00B1  __r0

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            data   0000   00D6 =    214. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0000  __data_start
       000B  _g_scCalTable
       0083  _g_buffer16
       0093  _gConfigStr
       00B2  _gCalLoadStr
       00BC  _gModeStr
       00C6  _gCWPitchStr
       00D0  _gIdleStr
       00D8  _gFltWtStr
       00E0  _gVFStr
       00E6  pclink_cmds.c:wVals
       00EA  _g_wBrgRts
       00EE  _g_dwCurrHz
       00F2  _g_xConf
       00F6  _g_wX10
       0100  _g_wR10
       0102  _g_wZ10
       0104  _g_wSwr100
       0106  timers.c:gDivider_2
       0108  _g_bIdleCounter
       0109  _g_bMeasureCounter
       010A  _g_bDebounceCounter
       010B  _g_bLongPressKeyCounter
       010C  _g_bSixtyFourthSecondCounter
       010D  _g_bMode
       010E  _g_bIsCalibrated
       010F  _g_bDDS_GainStep
       0110  _g_bSgnX
       0111  _g_bLongPress
       0112  _g_bUP_DOWN_SelectsIncDigit
       0113  _g_bUP_DOWN_SelectsVFOPower
       0114  _g_bIncDigit
       0115  _g_bBandIndex
       0116  keypad.c:bLastKey
       0117  _bWeight
       0118  timers.c:gDivider_1

Files Linked      [ module(s) ]

./obj/boot.o	[ boot.asm ]
./obj/bootloader.o	[ bootloader.c ]
./obj/bootloaderconfig.o	[ bootloaderconfig.asm ]
./obj/bridge.o	[ bridge.c ]
./obj/control.o	[ control.c ]
./obj/correctionmodel.o	[ correctionmodel.c ]
./obj/dds.o	[ dds.c ]
./obj/derive.o	[ derive.c ]
./obj/flashapi.o	[ flashapi.asm ]
./obj/glb_data.o	[ glb_data.c ]
./obj/keypad.o	[ keypad.c ]
./obj/main.o	[ main.c ]
./obj/morse.o	[ morse.c ]
./obj/msg_eng.o	[ msg_eng.c ]
./obj/pclink.o	[ pclink.c ]
./obj/pclink_cmds.o	[ pclink_cmds.c ]
./obj/scans.o	[ scans.c ]
./obj/screens.o	[ screens.c ]
./obj/storage.o	[ storage.c ]
./obj/timers.o	[ timers.c ]
<library>	[ uartint.asm, uart.asm, pwm8_buzzint.asm, pwm8_buzz.asm, psocconfigtbl.asm, psocconfig.asm, pga_dds_2.asm ]
[ pga_dds_1.asm, pga_adc.asm, lcd.asm, e2prom.asm, amux4_adc.asm, adcinc12int.asm, adcinc12.asm ]
[ E2PROMLIB.ASM, rZ.asm, rY.asm, rX.asm, r11.asm, r10.asm, r9.asm, r8.asm, r7.asm, r6.asm, r5.asm, r4.asm ]
[ r3.asm, r2.asm, r1.asm, r0.asm, divmod_16x16_16_util.asm, divmod_32x32_32_util.asm, divmodu_16x16_16_util.asm ]
[ divmodu_32x32_32_util.asm, i_div16_block_util.asm, i_div32_block_util.asm, i_div32_not_util.asm ]
[ i_mulu8_block_util.asm, mul16_util.asm, mul32_util.asm, cstrcmp.s, atol.s, isdigit.s, isspace.s ]
[ ctype.s, abs.s ]

Area Definitions

-blit:0x1a0.0x7fff
-bdata:0.0x6ff
-bSSCParmBlk:0xf7.0xff
-bBootCheckSum:0x7200.0x73ff
-bBootLoaderArea:0x7400.0x7fff
-BInterruptRAM:0	allocated at: 0
-Bvirtual_registers	allocated at: 0xa3
-Bsark100_g1ojs_v1_4_RAM	allocated at: 0

