//-------------------------------------------------------------
//-------------------------------------------------------------
//
//
// Gestione Procedure di risposta agli Interrupt
//
//
//-------------------------------------------------------------
//-------------------------------------------------------------
#include "Sfr_r825.h"
#include "defsLow.h"
#include "protLow.h"
#include "Interrupt.h"





#ifdef      INCLUDE_DISPLAY
extern volatile UCHAR TimeOutLcd;
#endif





/****************** fixed vector section ********************/

void wa2_int ( void )
{
}










/***************** variable vector section ******************/


UCHAR CntFor10mSec;
volatile UCHAR Delay1;
volatile UCHAR Delay10;
extern UCHAR ChannelCount;

//----------------------
//      Timer RD(Channel 0) interrupt(vector8)
void timer_rd0_int ( void )
{
  char Tmp;


  Tmp = trdsr0;
  trdsr0 = 0x00;        // resetta i Flag di Interrupt


  // Tempistiche a 1mSec
  Timer1mSec();         // Tempistiche a 1mSec

  if ( Delay1 ) Delay1--;

#ifdef      INCLUDE_DISPLAY
  if ( TimeOutLcd ) TimeOutLcd--;
#endif


  // Conversioni A/D successive
  ChannelCount++;
  AdGest();

  // Refresh Uscite
  CheckOut();

  if ( CntFor10mSec )
    CntFor10mSec--;
  else {
    CntFor10mSec = 10-1;


    // Tempistiche a 10mSec
    Timer10mSec();         // Tempistiche a 10mSec

    if ( Delay10 ) Delay10--;

    // Buzzer
    if ( F_Buzzer )
      if ( C_Buzzer )
        C_Buzzer--;
      else {
#ifdef      BUZZER_AUTO_OSCILLANTE
        BuzzerOut = 0;
#else
        tstart_trecr1 = 0;    // TreStart = STOP
        trecr1 = 0x00;        // #### #0## NO Output Enable
#endif
        F_Buzzer = 0;
      }


  }
}


volatile UCHAR Delay100;
UCHAR CntFor100mSec;


//----------------------
//      Timer RD(Channel 1) interrupt(vector9)
void timer_rd1_int ( void )
{
  char Tmp;


  Tmp = trdsr1;
  trdsr1 = 0x00;        // resetta i Flag di Interrupt

  Timer100mSec();       // Tempistiche a 10mSec

  if ( Delay100 ) Delay100--;

  if ( CntFor100mSec )
    CntFor100mSec--;
  else {
    CntFor100mSec = 10-1;


    // Tempistiche a 1Sec
    Timer1Sec();         // Tempistiche a 1Sec

  }
}



//----------------------
//      Timer RE interrupt(vector10)
void timer_re_int ( void )
{
  // Riservato per il Buzzer
}



//----------------------
//      Key in interrupt(vector 13)
void key_in_int ( void )
{
}



//----------------------
//      A/D interrupt(vector 14)
void ad_int ( void )
{
}



//----------------------
//      SSU/IIC interrupt(vector 15)
void ssu_int ( void )
{
}



//----------------------
//      UART0 transmit interrupt(vector 17)
void uart0_tra_int ( void )
{
}



//----------------------
//      UART0 receive interrupt(vector 18)
void uart0_rec_int ( void )
{
}



//----------------------
//      UART1 transmit interrupt(vector 19)
void uart1_tra_int ( void )
{
}



//----------------------
//      UART1 receive interrupt(vector 20)
void uart1_rec_int ( void )
{
}



//----------------------
//      INT2 interrupt(vector 21)
void int2_int ( void )
{
}



//----------------------
//      Timer RA interrupt(vector 22)
void timer_ra_int ( void )
{
  p6_4 = 0;

  // Ferma il Timer ( OneShot )
  ir_traic = 0;
  tstart_tracr = 0;

  // Z.C. Ritardato
  ZeroCrossing();

  // Conversioni A/D
  ChannelCount = 0;
  AdGest();
}



//----------------------
//      Timer RB interrupt(vector 24)
void timer_rb_int ( void )
{
}



//----------------------
//      INT1 interrupt(vector 25)
void int1_int ( void )
{
}



//----------------------
//      INT3 interrupt(vector 26)
void int3_int ( void )
{
}



//----------------------
//      INT0 interrupt(vector 29)
void int0_int ( void )
{
  UINT Ritardo;


  // Reset Interrupt Flag
  ir_int0ic = 0;


  // Controlla se e' necessario un Ritardo sullo Z.C. Hardware
#ifndef     DISABLE_HILEV
  Ritardo = ImpostaRitardoZC();
#else
  Ritardo = 0;                             // Nessun Ritardo sullo Z.C.
#endif

  p6_4 = 1;

  // Chiama la Funzione Custom
  if ( !Ritardo ) {
    // Immediatamente
    ZeroCrossing();
    // Conversioni A/D
    ChannelCount = 0;
    AdGest();
  } else {
    // In Ritardo ( vedi TimerRA )
    trapre = 32;
    tra = Ritardo / 32;
    tstart_tracr = 1;
  }
}
