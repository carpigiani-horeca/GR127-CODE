//-------------------------------------------------------------
//-------------------------------------------------------------
//
//
//          Prototipi di Funzioni
//
//
//-------------------------------------------------------------
//-------------------------------------------------------------







//--- Main.c
void Main ( void );
void DoIdle ( void );
void Timer1mSec ( void );
void Timer10mSec ( void );
void Timer100mSec ( void );
void Timer1Sec ( void );
UINT ImpostaRitardoZC ( void );
void ZeroCrossing ( void );



//--- Init.c
void SystemInit ( void );
void PeripheralInit ( void );


//--- Buzzer.c
void Play_Buzzer ( UINT Freq, UINT Duration );
void NoSound ( void );
void WaitBuzz ( UCHAR Wait );
void BepBep ( UINT Freq1, UINT Freq2 );


//--- ExtLedKey.c
UCHAR IntOut ( UCHAR Out );
void SetDigitLed ( void );
void SendToDigit ( void );
void RimbKey ( void );

//--- EPot.c
void WriteDigitalPot0 ( UCHAR data );

//--- Ad.c
void SetADCh ( UCHAR Channel );
UINT ReadAD ( UCHAR NewChannel );
void AD_MakeAverage ( UCHAR Channel, UINT Val );
void AD_FinalAverage ( UCHAR Channel );
void AdGest ( void );

//--- InOut.c
void MediaInput ( UCHAR Chan, UCHAR State );
void CheckIngressi ( void );
void CheckOut ( void );
void SetPwmOut ( UCHAR Perc );



//--- Timers.c
void Timer_A0 ( void );
void Timer_A1 ( void );
void Timer_A2 ( void );
void Timer_A3 ( void );
void Timer_A4 ( void );
void Int_Int3 ( void );



//--- Lcd.c
void WaitBusyLCD ( void );
void LcdCmd ( UCHAR Carattere , UCHAR Wait_BF );
void LcdOut ( UCHAR Carattere );
UCHAR LcdIn ( void );
UCHAR LcdRdBusy ( void );
void WaitLcd ( UCHAR Wait );
void LcdInit ( void );
void Cursor ( UCHAR Toggle );
void HCursor ( UCHAR Toggle );
void SetCursor ( UCHAR Position );
void RefreshBuffer ( void );
void Print ( const UCHAR *pnt , UCHAR Len , UCHAR Col );
void PutCar ( UCHAR Car );















