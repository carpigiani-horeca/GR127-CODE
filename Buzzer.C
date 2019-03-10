             //-------------------------------------------------------------
//-------------------------------------------------------------
//
//
//      Programma Principale
//
//
//-------------------------------------------------------------
//-------------------------------------------------------------
#include "Sfr_r825.h"
#include "defsLow.h"
#include "protLow.h"
#include "Interrupt.h"


struct BitStruct BuzzStatus;

UCHAR C_Buzzer;     // Contatore Durata Beep




/*-----------------04/07/00 16.37-------------------
  Aziona il Buzzer
--------------------------------------------------*/
void Play_Buzzer ( UINT Freq, UINT Duration )
{
  if (!F_Buzzer && Buzzer_Enab)
  {
#ifdef      BUZZER_AUTO_OSCILLANTE
    BuzzerOut = 1;
#else
    tstart_trecr1 = 0;                          // TreStart = STOP
    tremin = (UINT)(250000 / (long)Freq);       // Conteggio per Buzzer Output
    trecr1 = 0x04;                              // #### #1## Output Enable
    tstart_trecr1 = 1;                          // TreStart = START
#endif

    C_Buzzer = Duration;
    F_Buzzer = 1;
  }
}



/*-----------------04/07/00 16.37-------------------
  Sospende il Beep del buzzer
--------------------------------------------------*/
void NoSound ( void )
{
  if (F_Buzzer)
  {
    F_Buzzer = 0;
    C_Buzzer = 0;
#ifdef      BUZZER_AUTO_OSCILLANTE
    BuzzerOut = 0;
#else
    tstart_trecr1 = 0;              // TreStart = STOP
    trecr1 = 0x00;                  // #### #0## NO Output Enable
#endif
  }
}



/*-----------------04/07/00 16.38-------------------
  Attende la fine del Beep con del ritardo aggiuntivo
--------------------------------------------------*/
void WaitBuzz ( UCHAR Wait )
{
  while ( C_Buzzer )
    DoIdle();

  Delay1 = Wait;
  while ( Delay1 )
    DoIdle();
}




/*-----------------15/02/00 08.44-------------------
  Esegue il classico tlin-tlon
--------------------------------------------------*/
void BepBep ( UINT Freq1, UINT Freq2 )
{
  Play_Buzzer ( Freq1, 10 );
  WaitBuzz ( 50 );

  Play_Buzzer ( Freq2, 10 );
  WaitBuzz ( 20 );
}
