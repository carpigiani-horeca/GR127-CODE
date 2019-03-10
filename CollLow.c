//-------------------------------------------------------------
//-------------------------------------------------------------
//
//
// Funzioni di Collaudo
//
//
//-------------------------------------------------------------
//-------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include "Sfr_r825.h"
#include "defsLow.h"
#include "protLow.h"
#include "Interrupt.h"



#ifdef      COLLAUDO_MACROTEK



extern volatile UCHAR Delay100;
extern UCHAR Input;
extern const UCHAR Digit_Table_1_2[64];
extern const UCHAR Digit_Table_3[64];

extern UCHAR HiLevDisplay;


UINT PageWrite_EEPROM ( UINT Address, UCHAR *DataByte, UCHAR NumByte );
UINT SequentialRead_EEPROM ( UINT Address, UCHAR *DataByte, UCHAR NumByte );
void DigitValProg ( int Val );

void ExtTimerInit ( void );

// Definizione della struttura dati per l'orario da REAL-TIME-CLOCK ***** 7 BYTE ******
struct TimerData {
  UCHAR Second;                            // Second counter
  UCHAR Minute;                            // Minute counter
  UCHAR Hour;                              // Hour counter
  UCHAR DayWeek;                           // Day of Week counter
  UCHAR DayMonth;                          // Day of Month counter
  UCHAR Month;                             // Month
  UCHAR Year;                              // Year
};

struct TimerData MyTime;









extern volatile UCHAR TempZC;

void StringVal ( const char *c );

void CCheckTastierino ( void );
void CCheckZC ( void );
void CCheckIngressi ( void );
void CCheckUscite ( void );
void CCheckSonde ( void );
void CCheckTA ( void );
void CCheckEE ( void );
void CCheckLcd ( void );
void CCheckTK ( void );
void CCheckPWM ( void );
void CCheckI010 ( void );


extern volatile UCHAR I2cStatus;

UCHAR BcdToBin( UCHAR Val );
UCHAR BinToBcd( UCHAR Val );
void I2c_WriteByte ( UCHAR FAddr, UCHAR Addr, UCHAR Dato );
UCHAR I2c_ReadByte ( UCHAR FAddr, UCHAR Addr );
UCHAR ReadByteI2c ( UCHAR FAddr, UCHAR Addr );
void WriteByteI2c ( UCHAR FAddr, UCHAR Addr, UCHAR Dato );
void I2cInit ( void );
UCHAR WaitBusy ( UCHAR FAddr, UCHAR Wait );


void Collaudo ( void )
{
  // Inizializzazione I2C
  I2cInit();
  // Inizializzazione Control-Register-2 del REAL-TIME-CLOCK
  WriteByteI2c ( 0x64, 0xF0, 0x20 );     // imposta l'orologio a 24h

  // Tastierino
  CCheckTastierino();

#ifdef      INCLUDE_DISPLAY
  // Lcd Esterno
  StringVal ( "Lcd" );
  Delay100 = 10; while ( Delay100 ) ;
  CCheckLcd();
#endif

  // Orologio
  StringVal ( "OrA" );
  Delay100 = 10; while ( Delay100 ) ;
  CCheckTK();

  // Azzeramento Orologio
  memset ( &MyTime, 0x00, sizeof ( struct TimerData ) );
  WriteByteI2c ( 0x64, 0x00, BinToBcd( MyTime.Second ) );
  Delay1 = 20; while ( Delay1 ) ;
  WriteByteI2c ( 0x64, 0x10, BinToBcd( MyTime.Minute ) );
  Delay1 = 20; while ( Delay1 ) ;
  WriteByteI2c ( 0x64, 0x20, BinToBcd( MyTime.Hour ) );
  Delay1 = 20; while ( Delay1 ) ;
  WriteByteI2c ( 0x64, 0x30, MyTime.DayWeek );
  Delay1 = 20; while ( Delay1 ) ;
  WriteByteI2c ( 0x64, 0x40, BinToBcd ( MyTime.DayMonth ) );
  Delay1 = 20; while ( Delay1 ) ;
  WriteByteI2c ( 0x64, 0x50, BinToBcd ( MyTime.Month ) );
  Delay1 = 20; while ( Delay1 ) ;
  WriteByteI2c ( 0x64, 0x60, BinToBcd ( MyTime.Year ) );

#ifdef      INCLUDE_PWM
  // Uscita PWM
  StringVal ( "Pun" );
  Delay100 = 10; while ( Delay100 ) ;
  CCheckPWM();
#endif

  // Ingresso 0V % 10V
  StringVal ( "EHt" );
  Delay100 = 10; while ( Delay100 ) ;
  CCheckI010();

  // Orologio
  StringVal ( "OrA" );
  Delay100 = 10; while ( Delay100 ) ;
  CCheckTK();

  // Zero Crossing
  StringVal ( "Zc " );
  Delay100 = 10; while ( Delay100 ) ;
  CCheckZC();

  // Controllo delll'EeProm
  StringVal ( "E2 " );
  Delay100 = 10; while ( Delay100 ) ;
  CCheckEE();

  // Controllo Ingressi : I# e BK#
  StringVal ( "INP" );
  Delay100 = 10; while ( Delay100 ) ;
  CCheckIngressi();

  // Controllo Uscite TK# e K#
  StringVal ( "Out" );
  Delay100 = 10; while ( Delay100 ) ;
  CCheckUscite();

  // Controllo Sensori Ntc / Ptc
  StringVal ( "Snd" );
  Delay100 = 10; while ( Delay100 ) ;
  CCheckSonde();

  // Controllo del TA e del Potenziometro Digitale
  StringVal ( "tA " );
  Delay100 = 10; while ( Delay100 ) ;
  CCheckTA();


  // FINE
  StringVal ( "End" );
  Play_Buzzer ( 4000, 50 );

  while ( 1 ) ;
}


UCHAR TabSegmenti[8] = {
  0x01, 0x02, 0x04, 0x08, 0x20, 0x10, 0x80, 0x40
};

//--- Collaudo del Tastierino Esterno
void CCheckTastierino ( void )
{
  UCHAR Cnt, Esci;


  HiLevDisplay = 0;
  ToByte(Digit_1) = 0xFF;
  ToByte(Digit_2) = 0xFF;
  ToByte(Digit_3) = 0xFF;
  Delay100 = 10; while ( Delay100 ) ;
  ToByte(Digit_1) = 0x00;
  ToByte(Digit_2) = 0x00;
  ToByte(Digit_3) = 0x00;
  LED_1 = LED_2 = LED_3 = LED_4 = 0;


  // Test dei Segmenti
  Esci = 0; Cnt = 0;
  while ( !Esci )
  {
    // Attesa Tasto
    while ( !KeyState ) ;

    // Visualizzazione Segmenti
    if ( IS_KEY(KEY_P2) ) {
      ToByte(Digit_1) = TabSegmenti[Cnt & 0x07];
      ToByte(Digit_2) = TabSegmenti[Cnt & 0x07];
      ToByte(Digit_3) = TabSegmenti[Cnt & 0x07];
      Cnt++;
    }
    if ( KeyState == (KEY_P1 | KEY_P3) )
      if ( Cnt >= 8 )
        Esci = 1;

    // Attesa Rilascio
    while ( KeyState ) ;
  }


  // Test dei Led
  DigitValProg ( 0 );
  HiLevDisplay = 1;
  Delay100 = 10; while ( Delay100 ) ;
  LED_1 = 1; Delay100 = 10; while ( Delay100 ) ; LED_1 = 0;
  LED_4 = 1; Delay100 = 10; while ( Delay100 ) ; LED_4 = 0;
  LED_3 = 1; Delay100 = 10; while ( Delay100 ) ; LED_3 = 0;
  LED_2 = 1; Delay100 = 10; while ( Delay100 ) ; LED_2 = 0;
#ifdef      INCLUDE_EXTKEY
  LED_EXT_1 = 1; Delay100 = 10; while ( Delay100 ) ; LED_EXT_1 = 0;
  LED_EXT_2 = 1; Delay100 = 10; while ( Delay100 ) ; LED_EXT_2 = 0;
#endif


  // Test dei Tasti
  Esci = 0;
#ifdef      INCLUDE_EXTKEY
  Cnt = 0xE0;
#else
  Cnt = 0xF8;
#endif
  Buzzer_Enab = 1;
  while ( !Esci )
  {
    // Attesa Tasto
    while ( !KeyState ) ;

    // Visualizzazione Tasto
    if ( KeyState == KEY_P1 ) {
      Play_Buzzer ( 3000, 10 );
      DigitValProg ( 1 );
      Cnt |= 0x01;
    }
    if ( KeyState == KEY_P2 ) {
      Play_Buzzer ( 4000, 10 );
      DigitValProg ( 2 );
      Cnt |= 0x02;
    }
    if ( KeyState == KEY_P3 ) {
      Play_Buzzer ( 5000, 10 );
      DigitValProg ( 3 );
      Cnt |= 0x04;
    }

#ifdef      INCLUDE_EXTKEY

    if ( KeyState == KEY_P4 ) {
      Play_Buzzer ( 3500, 10 );
      DigitValProg ( 4 );
      Cnt |= 0x08;
    }
    if ( KeyState == KEY_P5 ) {
      Play_Buzzer ( 4500, 10 );
      DigitValProg ( 5 );
      Cnt |= 0x10;
    }

#endif


    if ( KeyState == (KEY_P1|KEY_P3) )
      if ( Cnt == 0xFF )
        Esci = 1;

    // Attesa Rilascio
    while ( KeyState ) ;
  }
}

volatile UCHAR TempZC;

//--- Controllo ZeroCrossing
void CCheckZC ( void )
{
  UCHAR Esci;


  Esci = 0;
  while ( !Esci )
  {
    // Attesa Tasto
    while ( !KeyState ) {

      TempZC = 0;
      Delay100 = 10; while ( Delay100 ) ;
      DigitValProg ( TempZC );
      if ( LED_4 ) LED_4 = 0; else LED_4 = 1;

    };

    if ( IS_KEY(KEY_P2) )
      if ( (TempZC >= 48) && (TempZC <= 52) )
        Esci = 1;

    // Attesa Rilascio
    while ( KeyState ) ;
  }
  LED_4 = 0;

}



//--- Controllo Ingressi : I# e BK#
void CCheckIngressi ( void )
{
  UCHAR Esci, Mask;


  Esci = 0;
  Mask = 0x00;
  while ( !Esci )
  {
    // Visualizzazione Stato Ingressi
    if ( INP_BK1 ) {
      StringVal ( "bH1" );
      Mask |= 0x01;
    } else
    if ( INP_BK2 ) {
      StringVal ( "bH2" );
      Mask |= 0x02;
    } else
    if ( INP_I1 ) {
      StringVal ( "i-1" );
      Mask |= 0x04;
    } else
    if ( INP_I2 ) {
      StringVal ( "i-2" );
      Mask |= 0x08;
    } else
    if ( INP_I3 ) {
      StringVal ( "i-3" );
      Mask |= 0x10;
    } else
      StringVal ( "   " );

    // Controllo Uscita
    if ( IS_KEY(KEY_P2) )
      if ( Mask == 0x1F )
        Esci = 1;
  }

  // Attesa Rilascio
  while ( KeyState ) ;
}



const char *COutName[] = {
  "Oh1",
  "Oh2",
  "Oh3",
  "Oh4",
  "th1",
  "th2",
};

//--- Controllo Uscite TK# e K#
void CCheckUscite ( void )
{
  UCHAR Esci, Mask;
  UCHAR Cnt;


  Esci = 0;
  Cnt = 0; Mask = 0x01;
  while ( !Esci ) {

    // Nome Uscita
    StringVal ( COutName[Cnt] );
    LED_4 = 0;

    // Attende Comando
    while ( !KeyState ) ;
    if ( KeyState == (KEY_P1|KEY_P3) )
      if ( Mask == 0x3F )
        Esci = 1;

    // Attiva Uscita
    ToByte(OutPut) = 1 << Cnt;
    LED_4 = 1;

    // Attende Rilascio
    while ( KeyState ) ;

    // Disattiva Uscita
    ToByte(OutPut) = 0x00;
    LED_4 = 0;

    // Passa alla Successiva
    Mask |= 1 << Cnt;
    Cnt++;
    if ( Cnt >= 6 ) Cnt = 0;

  }

}




//--- Controllo Sensori Ntc / Ptc
void CCheckSonde ( void )
{
  UCHAR Esci;


  Esci = 0;
  while ( !Esci ) {

    // Ptc
    StringVal ( "Ptc" );
    Delay100 = 10; while ( Delay100 ) ;

    while ( !KeyState )
      DigitValProg ( PtcValue );

    while ( KeyState ) ;


    // Ntc
    StringVal ( "ntc" );
    Delay100 = 10; while ( Delay100 ) ;

    while ( !KeyState )
      DigitValProg ( NtcValue );

    if ( KeyState == (KEY_P1|KEY_P3) )
      Esci = 1;
    while ( KeyState ) ;

  }

  // Attesa Rilascio
  while ( KeyState ) ;
}



//--- Controllo del TA e del Potenziometro Digitale
void CCheckTA ( void )
{
  UCHAR Esci, Mask;
  UCHAR EPot;


  Esci = 0; Mask = 0x00;
  EPot = 128; LED_3 = 1;
  while ( !Esci ) {

    // Regolazione E-Pot
    WriteDigitalPot0 ( EPot );

    // Visualizzazione Valore
    while ( !KeyState )
      DigitValProg ( TaValue );

    // Controllo Tasto
    if ( IS_KEY(KEY_P2) ) {
      // 10%
      EPot = 26;
      LED_2 = 1;
      Mask |= 0x01;
      LED_3 = LED_4 = 0;
    }
    if ( IS_KEY(KEY_P1) ) {
      // 50%
      EPot = 128;
      LED_3 = 1;
      Mask |= 0x02;
      LED_2 = LED_4 = 0;
    }
    if ( IS_KEY(KEY_P3) ) {
      // 90%
      EPot = 230;
      LED_4 = 1;
      Mask |= 0x04;
      LED_2 = LED_3 = 0;
    }
    if ( KeyState == (KEY_P1|KEY_P3) )
      if ( Mask == 0x07 )
        Esci = 1;

    while ( KeyState ) ;

  }
}



//--- Controllo E2
void CCheckEE ( void )
{
  UCHAR Buf1[10], Buf2[10];



  // Stringa di Prova 1
  strcpy ( Buf1, "Collaudo" );
  // Prova di Scrittura 1
  PageWrite_EEPROM ( 0x100, Buf1, 8 );

  // Stringa di Prova 2
  strcpy ( Buf1, "odualloC" );
  // Prova di Scrittura 2
  PageWrite_EEPROM ( 0x200, Buf1, 8 );


  // Prova di Rilettura 1
  SequentialRead_EEPROM ( 0x100, Buf2, 8 );
  if ( memcmp ( "Collaudo", Buf2, 8 ) ) {
    StringVal ( "Err" );
    while ( KeyState != (KEY_P1|KEY_P3) ) ;
    while ( KeyState ) ;
  }

  // Prova di Rilettura 2
  SequentialRead_EEPROM ( 0x200, Buf2, 8 );
  if ( memcmp ( "odualloC", Buf2, 8 ) ) {
    StringVal ( "Err" );
    while ( KeyState != (KEY_P1|KEY_P3) ) ;
    while ( KeyState ) ;
  }
}




#ifdef      INCLUDE_DISPLAY

//--- Controllo Display 2x16
void CCheckLcd ( void )
{
  // Inizializzazione Lcd
  LcdInit();

  while ( KeyState ) ;

  // Stampa di Prova (Diretta)
  Print ( "Prima riga DIR. ", 16, 0 );
  Print ( "Seconda riga DIR", 16, 16 );
  while ( KeyState ) ;
  while ( !KeyState ) ;

  // Stampa di Prova (Indiretta)
  sprintf ( LcdRiga1, "Prova %04d      ", 1234 );
  sprintf ( LcdRiga2, "Stampa Indiretta" );
  RefreshBuffer();

  while ( KeyState ) ;
  while ( !KeyState ) ;
}

#endif




//--- Controllo orologio I2c
void CCheckTK ( void )
{
  UCHAR Esci, Ret;



  Print ( "Test Orologio:  ", 16, 0 );
  Print ( "  Ora: 00:00:00 ", 16, 16 );

  Esci = 0;
  Delay100 = 0;
  while ( !Esci ) {


    if ( !Delay100 ) {
      Delay100 = 10;

      Ret = ReadByteI2c ( 0x64, 0x00 );
      MyTime.Second = BcdToBin ( Ret );
      Ret = ReadByteI2c ( 0x64, 0x10 );
      MyTime.Minute = BcdToBin ( Ret );
      Ret = ReadByteI2c ( 0x64, 0x20 );
      MyTime.Hour = BcdToBin ( Ret );
      Ret = ReadByteI2c ( 0x64, 0x30 );
      MyTime.DayWeek = Ret;
      Ret = ReadByteI2c ( 0x64, 0x40 );
      MyTime.DayMonth = BcdToBin ( Ret );
      Ret = ReadByteI2c ( 0x64, 0x50 );
      MyTime.Month = BcdToBin ( Ret );
      Ret = ReadByteI2c ( 0x64, 0x60 );
      MyTime.Year = BcdToBin ( Ret );

      // Stampa
      sprintf ( LcdRiga1, "Test Orologio:  " );
      sprintf ( LcdRiga2, "  Ora: %02d:%02d:%02d ", MyTime.Hour, MyTime.Minute, MyTime.Second );
      RefreshBuffer();

    }

    // Controllo Tasto
    if ( KeyState == (KEY_P1|KEY_P3) )
      Esci = 1;

  }

  while ( KeyState ) ;
}





#ifdef      INCLUDE_PWM

//--- Controllo Pilotaggio uscita PWM
void CCheckPWM ( void )
{
  UCHAR Esci, Mask;
  UCHAR Pwm;


  Esci = 0; Mask = 0x00;
  Pwm = 50; LED_3 = 1;
  while ( !Esci ) {

    // Regolazione Pwm
    SetPwmOut ( Pwm );

    // Visualizzazione Valore
    while ( !KeyState )
      DigitValProg ( TaValue );

    // Controllo Tasto
    if ( IS_KEY(KEY_P2) ) {
      // 10%
      Pwm = 10;
      LED_2 = 1;
      Mask |= 0x01;
      LED_3 = LED_4 = 0;
    }
    if ( IS_KEY(KEY_P1) ) {
      // 50%
      Pwm = 50;
      LED_3 = 1;
      Mask |= 0x02;
      LED_2 = LED_4 = 0;
    }
    if ( IS_KEY(KEY_P3) ) {
      // 90%
      Pwm = 90;
      LED_4 = 1;
      Mask |= 0x04;
      LED_2 = LED_3 = 0;
    }

    if ( IS_KEY(KEY_P4) ) {
      // 0%
      Pwm = 0;
      LED_2 = LED_3 = LED_4 = 0;
    }
    if ( IS_KEY(KEY_P5) ) {
      // 100%
      Pwm = 100;
      LED_2 = LED_3 = LED_4 = 1;
    }

    if ( KeyState == (KEY_P1|KEY_P3) )
      if ( Mask == 0x07 )
        Esci = 1;

    while ( KeyState ) ;

  }
}

#endif



//--- Controllo Sensore 0V % 10V
void CCheckI010 ( void )
{
  // Attesa Rilascio
  while ( KeyState ) ;


  // Visualizzazione Valore
  while ( !KeyState ) {

    // Stampa valore A/D
    DigitValProg ( In010Value );

    // Stampa valore in mVolt
    sprintf ( LcdRiga1, "Ingresso 0V-10V " );
    sprintf ( LcdRiga2, "     mVolt:%05d", Input0_10V );
    RefreshBuffer();
  }


  // Attesa Rilascio
  while ( KeyState ) ;
}







/*-----------------16/01/02 14.04-------------------
   Scrive un Byte ad un Indirizzo
	 FAddr = Indirizzo fisico dello Slave
   Addr = Inidizzo interno su cui scrivere
	 Dato = Dato da Scrivere
--------------------------------------------------*/
void WriteByteI2c ( UCHAR FAddr, UCHAR Addr, UCHAR Dato )
{
  // Scrive il Dato
  I2c_WriteByte ( FAddr, Addr, Dato );

  if ( !(I2cStatus & 0x02) ) {         // Solo se NON ci sono stati Errori
    // Controllo fine scrittura
    WaitBusy ( FAddr, 1 );
  }
}



/*-----------------16/01/02 14.04-------------------
   Legge un Byte ad un Indirizzo
	 FAddr = Indirizzo fisico dello Slave
   Addr = Inidizzo interno su cui leggere
	 Return : Dato Letto
--------------------------------------------------*/
UCHAR ReadByteI2c ( UCHAR FAddr, UCHAR Addr )
{
  UCHAR Ret;


  // Lettura del Dato
  Ret = I2c_ReadByte ( FAddr, Addr );

  return Ret;
}






#endif
