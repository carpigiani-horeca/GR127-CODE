/***************************************************************************
 *                                                                         *
 *      Prototipi delle funzioni  - dichiarazioni                          *
 *                                                                         *
 ***************************************************************************/



// *** Main.c
//________________________________________


// *** Init.c
//________________________________________
void Init_Micro ( void );



// *** 191barpsp.c
//________________________________________
void TestDisplay ( void );
void CaricaTabellaProgrammazione ( void );
void Programmazione ( void );
void SalvaConi ( void );
void VisualizzaConi ( void );
void AutoSetup ( void );
void Setup ( void );
void SalvaStatoMacchina ( UC State );
void SalvaPastoEnd ( void );
UC Stop ( void );
UC Produzione ( void );
UC Pulizia ( void );
UC Monitoring ( void );
//UC Pastorizzazione ( void );
UC Conservazione ( void );
UC CountDown ( void );
UC Exit ( void );
UC Select ( void );
UC InviaLog ( void );
UC InviaTabProg ( void );
UC RiceviTabProg ( void );
void All_Out_Off ( void );
void ReadDataBlackOut ( void );
void LeggiPagina0 ( void );
UC HOT ( void );


// *** Digit_Led.c
//________________________________________
void RimbKey ( void );
void SetDigitLed ( void );
void SendToDigit ( void );
void AllDigitLedOff ( void );
void AllDigitLedOn ( void );
void LampAllDigitLedOn( void );
void LampAllDigitLedOff ( void );
void ProvaStringhe ( void );
void StringVal ( const char *c);
void DigitValProg ( int Val );
void DigitVal ( int Val );
void DisplayHot ( int Val );
void DisplayHot_Barre ( int Val );
void DisplayHotAutotaratura ( int Val );






// *** Probe.c
//________________________________________
void SetADCh ( UC ch );
void SumADValue ( UC ch );
void ResetAccADValue ( UC Channel );
void SetADFlags ( WORD flags );
void AverageADValue ( UC ch );
void FillAccAverageValue ( UC Channel, UC AverageIndex );
void MakeAverage ( UC Channel );
void AverageProbeValue ( void );
void ResetIndex ( void );
void ResetAllAccADValue ( void );
int TempPTC ( WORD val, int offset, int slope, int pzt );
int TempNTC ( long int val, int offset );
void WriteDigitalPot0 ( UC data );
//void WriteDigitalPot1 ( UC data );
//void WriteDigitalPot2 ( UC data );
WORD TAtoHOT ( WORD val );
WORD OHMtoLEVEL ( WORD val );
int Cel_Far ( int gradi );
int Cel_Far_Delta ( int gradi );



// *** Timers.c
//________________________________________
void WaitSec ( UC Time );
void WaitmSec ( UC Time );
void Wait10mSec ( UC Time );
void Wait100mSec ( UC Time );
void DoIdle ( void );



// *** Seriale.c
//________________________________________
void MakeMsg ( UC Id, const UC *Buf, UC LenData );
void SendMsg ( UC Len );
void WaitTxEnd ( void );
void BinToAscii ( UC *Buf, WORD Val, UC Dim );
void HexToAscii ( UC *Buf, WORD Val, UC Dim );
void AppendToBuffer ( UC far *Buf, UC Dim, UC Space );
void SendBuffer ( UC Wait );
BYTE ControllaRS232 ( UC lettomessaggio );



// *** Buzzer.c
//________________________________________
void Play_Buzzer ( WORD Freq, WORD Duration );
void NoSound ( void );
void WaitBuzz ( UC Wait );
void BepBep ( WORD Freq1, WORD Freq2 );



// *** Flash.c
//________________________________________
UC WriteEeprom ( struct StrTabProg *PntData );
UC ReadEeprom ( struct StrTabProg *PntData );
void EraseFlashBlock1 ( void );
void WriteFlashBlock1 ( void );
UC ReadFlashBlock1 ( void );



// *** Flashcrd.c
//________________________________________
UC CheckSum ( UC *PntStr, UC DimStr );
UC CheckFlashCard ( void );
UC ReadByteBF ( WORD Addr );
UC ReadByteDF ( DWORD Addr );
void WriteByteDF ( DWORD Addr, UC Val );
void WriteByteBF ( WORD Addr, UC Val );
void WritePageDF ( WORD Page, UC *PageBuf, WORD Cnt );
void WritePageBF ( WORD Addr, UC *PageBuf, WORD Cnt );
void ReadPageDF ( WORD Page, UC *PageBuf, WORD Cnt );
void WaitBusyDF ( UC Time );
UC ReadStatusDF ( UC LeaveCS );
UC VerifyPageDF ( WORD Page, UC *PageBuf, UC *TempBuf );
void FlashAutoRewrite ( WORD Page );
void FlashPageErase ( WORD Page );
void FlashToBuffer ( WORD Page );
void BufferToFlash ( WORD Page );
UC GetByteData ( void );
void SendByteData ( UC Dato );




// *** I2C.c
//________________________________________
UC I2cReadByte ( UC FAddr, UC Addr );
void I2cWriteByte ( UC FAddr, UC Addr, UC Dato );
UC I2c_Read ( UC Last );
UC I2c_Write ( UC Data );
UC WaitBusy ( UC FAddr, UC Wait );
void I2c_Stop ( void );
UC I2c_Start ( UC Addr, UC R_W );
UC I2c_RepStart ( UC Addr, UC R_W );
void I2cDeInit ( void );
void I2cInit ( void );



// *** Time.c
//________________________________________
void ExtTimerInit ( void );
void TimerWrData2 ( struct TimerData *Pnt1, struct TimerData *Pnt2 );
void TimerWrData ( struct TimerData *Pnt );
void TimerWrAlarm_A ( struct TimerAlarm *Pnt );
void TimerRdData2 ( struct TimerData *Pnt1, struct TimerData *Pnt2 );
void TimerRdData ( struct TimerData *Pnt );
void TimerRdAlarm_A ( struct TimerAlarm *Pnt );



// *** Controllo orologio
//________________________________________
void TimerRefresh ( struct TimerData *Pnt, UC Mask );
UC ControlloOrario ( struct TimerData *Ora );
UC ControllaDifferenze ( struct TimerData *Ora, struct TimerData *Campione );
void SettaOrario ( struct TimerData *PntTo, struct TimerData *PntFrom );

UC I2c_ReadByte ( UC FAddr, UC Addr );
void I2c_WriteByte ( UC FAddr, UC Addr, UC Dato );

UC DurataMese ( UC Mese, WORD Anno );
void Incrementa1Minuto ( struct TimerData *Ora );
UC ControllaOrario ( struct TimerData *Ora );
void CopiaOrario ( struct TimerData *PntTo, struct TimerData *PntFrom );


// *** SerialEprom.c
//________________________________________
WORD ByteWriteEEPROM ( WORD Address, BYTE Data );
WORD RandomReadEEPROM ( WORD Address );

//Basso livello
WORD PageWrite_EEPROM ( WORD Address, BYTE *DataByte, UC NumByte );
WORD SequentialRead_EEPROM ( WORD Address, BYTE *DataByte, UC NumByte );
WORD SequentialVerify_EEPROM ( WORD Address, BYTE *DataByte, UC NumByte );

//Alto livello
WORD PageWriteEEPROM ( WORD Address, BYTE *DataByte, UC NumByte );
WORD SequentialReadEEPROM ( WORD Address, BYTE *DataByte, UC NumByte );

UC WriteStructEEPROM ( WORD Address, UC *PntStr, UC DimStr );
UC ReadStructEEPROM ( WORD Address, UC *PntStr, UC DimStr );








