//-------------------------------------------------------------
//-------------------------------------------------------------
//
//
//      NOTE :
//
//          La direttiva '#pragma SECTION program Codice' serve per
//              utilizzare l'area di Flash 'Codice' di 16K
//              (*** Questa zona e' utilizzata anche dalle Costanti ***)
//              (come esempio il main.c utilizza quella parte di Flash)
//              Il codice di default va in un'area di 48K
//
//          Le funzioni :  Timer1mSec, Timer10mSec, Timer100mSec, Timer1Sec
//              vengono chiamate ai tempi prefissati ( inserisci il tuo codice )
//              (ALL'INTERNO VENGONO GIA CHIAMATE LE TUE FUNZIONI DI ALTO LIVELLO)
//
//          La Funzione : ImpostaRitardoZC()
//              deve ritornare un valore pari al ritardo che si deve avere sullo
//              ZeroCrossing (CONTROLLA CHE IL CONTENUTO SIA CORRETTO)
//
//          La funzione : ZeroCrossing
//              viene chiamata allo ZeroCrossing (gia Sfasato)
//              (LA FUNZIONE CHIAMA LA 'Int_Int3' CHE CONTIENE IL TUO SW ALTO LIVELLO)
//
//
//          Come accedere alle Periferiche :
//              - LED_# : Stato dei Led
//              - MASK_P# : Stato dei Tasti
//              - KEY_P# : Tasti ( da 1 a 3 Per il TastierinoLed, da 1 a 5 per la Tastiera a Membrana )
//              - Digit[#] : Digit 7 segmenti
//              - INP_I1, INP_I2, INP_I3, INP_BK1, INP_BK2 : Ingressi
//              - PtcValue : Valore Ptc
//              - NtcValue : Valore Ntc
//              - TaValue : Valore TA
//              - OUT_K#, OUT_TK# : Uscite
//              - In010Value : Valore Ingresso 0V-10V (A/D)
//              - Input0_10V : Valore Ingresso 0V-10V espresso in mVolt
//              - LcdRiga1 / LcdRiga2 : Per acedere alle righe del display 2x16 (RefreshBuffer -> Aggiorna il Display)
//                                       (Puntatori alle righe del Display)
//              - SetPwmOut(Perc) : Setta l'uscita PWM ad una percentuale 'Perc' da 0% a 100%
//
//          Init.c -> Inizializzazione del micro e periferiche
//          Interrupt.c -> Funzioni di Interrupt ( Timer / Interrupt )
//          Buzzer.c -> Sostituisce l'originale al 100%
//          AD.c -> Calcolo A/D ( Ntc, Ptc, TA )
//          EPot.c -> Gestione Potenziometro Digitale per TA
//          ExtLedKey.c -> Gestione Schedino Dsp/Key/Led
//          InOut.c -> Gestione Ingressi / Uscite
//          MainLow.c -> Funzioni che chiamano le TUE ad alto livello ( TIMER/MAIN )
//          Lcd.c -> Funzioni per la Gestione del Display 2x16
//           - LcdInit() -> Inizializzazione
//           - RefreshBuffer() -> Rinfresca l'LCD (Utilizzare 'LcdRiga#' per inserire il caratteri)
//           - Print(...) -> Styampa diretta su LCD
//
//
//
//          Note sulla transizione :
//              IN24_1:     La variabile NON e' associata a NULLA
//                            creare un riferimento ad in Input esistente
//              IN24_4:     La variabile NON e' associata a NULLA
//                            creare un riferimento ad in Input esistente
//              IN24_5:     La variabile NON e' associata a NULLA
//                            creare un riferimento ad in Input esistente
//              ENABLE_V_OUT : La variabile NON e' associata a NULLA
//                            creare un riferimento ad in Input esistente
//              T_E_1 :     Utilizza un Ntc che NON esiste
//                          (l'ho trasformata in Variabile per poter compilare )
//              T_E_2 :     Utilizza un Ntc che NON esiste
//                          (l'ho trasformata in Variabile per poter compilare )
//              ProbeValue[] :      L'array non e' associato a NULLA
//                                    elimina i riferimenti dal programma
//
//              Aggiornamento via seriale ( AutoRiprogrammazione )
//                - Non e' possibile riprogrammare il micro da seriale in AutoRiprogrammazione
//                  in quanto non ci sono piu blocchi di Flash sufficenti per utilizzarne uno come BOOT
//                  (non modificabile) che riprograma gli altri (si dovrebbe rinunciare a meta' dello spazio)
//
//-------------------------------------------------------------
//-------------------------------------------------------------




// Direttiva per mettere il codice nella sezione 'Codice'
//  Dimensione:  da 0x10000 a 0x13FFF => 16K
//..................................................................................
#pragma SECTION program Codice
//..................................................................................



//-------------------------------------------------------------
//-------------------------------------------------------------
//
//
//      Programma Principale
//
//
//-------------------------------------------------------------
//-------------------------------------------------------------

// Include da aggiungere a tutti i file
#include "Sfr_r825.h"
#include "defsLow.h"
#include "protLow.h"
#include "Interrupt.h"
#ifdef      COLLAUDO_MACROTEK
#include <stdio.h>
#include <string.h>
#endif




#ifndef      DISABLE_HILEV

void MainLoop ( void );

void main ( void )
{
  MainLoop();
}


#else

extern volatile UCHAR Delay100;
extern UCHAR Input;
extern const UCHAR Digit_Table_1_2[64];
extern const UCHAR Digit_Table_3[64];
extern UCHAR HiLevDisplay;


char Buf[3];
UINT PageWrite_EEPROM ( UINT Address, UCHAR *DataByte, UCHAR NumByte );
UINT SequentialRead_EEPROM ( UINT Address, UCHAR *DataByte, UCHAR NumByte );
//UCHAR ReadEeprom ( struct StrTabProg *PntData );
//UCHAR WriteEeprom ( struct StrTabProg *PntData );


//struct StrTabProg Tab;

void DigitValProg ( int Val );
void Collaudo ( void );

void main ( void )
{
  char Tmp;
  char Temp;


  //--- Inizializzazione del micro
  asm("FCLR I");                                          /* Interrupt disable */
  SystemInit();
  PeripheralInit();
  asm("FSET I");                                          /* Interrupt enable */



  // Inizializzazione BUS-I²C per REAL-TIME-CLOCK
  //ExtTimerInit();


#ifdef      COLLAUDO_MACROTEK
  Collaudo();
#endif


  ////////////////////////////////////////////
  /////////////             DA QUI PUOI CANCELLARE TUTTO
  Buzzer_Enab = 1;

  /*
  ToByte(Digit_1) = 0x00; ToByte(Digit_2) = 0x00; ToByte(Digit_3) = 0xFF; ToByte(Led) = 0x00;
  Delay100 = 10; while ( Delay100 ) ;

  ToByte(Digit_1) = 0x00; ToByte(Digit_2) = 0xFF; ToByte(Digit_3) = 0x00; ToByte(Led) = 0x00;
  Delay100 = 10; while ( Delay100 ) ;

  ToByte(Digit_1) = 0xFF; ToByte(Digit_2) = 0x00; ToByte(Digit_3) = 0x00; ToByte(Led) = 0x00;
  Delay100 = 10; while ( Delay100 ) ;

  ToByte(Digit_1) = 0x00; ToByte(Digit_2) = 0x00; ToByte(Digit_3) = 0x00; ToByte(Led) = 0xFF;
  Delay100 = 10; while ( Delay100 ) ;
  */

  WriteDigitalPot0 ( 0 ); DigitValProg ( 0 );
  LED_1 = 1; DOT_1 = 1; Delay100 = 10; while ( Delay100 ) ; LED_1 = 0; DOT_1 = 0;
  WriteDigitalPot0 ( 64 ); DigitValProg ( 64 );
  LED_2 = 1; DOT_2 = 1; Delay100 = 10; while ( Delay100 ) ; LED_2 = 0; DOT_2 = 0;
  WriteDigitalPot0 ( 128 ); DigitValProg ( 128 );
  LED_3 = 1; DOT_3 = 1; Delay100 = 10; while ( Delay100 ) ; LED_3 = 0; DOT_3 = 0;
  WriteDigitalPot0 ( 192 ); DigitValProg ( 192 );
  LED_4 = 1; Delay100 = 10; while ( Delay100 ) ; LED_4 = 0;
  WriteDigitalPot0 ( 255 ); DigitValProg ( 255 );
  Delay100 = 10; while ( Delay100 ) ;



  LampOn = 1;
  ToByte(LampEnable) = 0xFF;
  ToByte(LampEnableLed) = 0x00;
  ToByte(SetLed) = 0x00;
  ToByte(Dot) = 0x07;
  Digit[2] = Digit[1] = Digit[0] = 0x00;

  WriteDigitalPot0 ( 128 );         // 50% -> 25K


  //ReadEeprom ( &Tab );
  //Digit[0] = Tab.Digit[0];
  //Digit[1] = Tab.Digit[1];
  //Digit[2] = Tab.Digit[2];
  //DigitValProg ( Tab.Digit[0] );

  //SequentialRead_EEPROM ( 0x100, Buf, 3 );
  //Digit[0] = Buf[0];
  //Digit[1] = Buf[1];
  //Digit[2] = Buf[2];
  Delay100 = 50; while ( Delay100 ) ;

  //--- Main Loop
  Tmp = 0x01;
  while ( 1 )
  {
    /*
    ToByte(Digit_1) = KeyState;
    if ( IS_KEY(KEY_P1) ) ToByte(Digit_2) = Digit_Table_1_2[1];
    if ( IS_KEY(KEY_P2) ) ToByte(Digit_2) = Digit_Table_1_2[2];
    if ( IS_KEY(KEY_P3) ) ToByte(Digit_2) = Digit_Table_1_2[3];
    ToByte(Digit_3) = Input;
    */

    if ( IS_KEY(KEY_P1) || INP_BK1 ) LED_1 = 1; else LED_1 = 0;
    if ( IS_KEY(KEY_P2) || INP_BK2 ) LED_2 = 1; else LED_2 = 0;
    if ( IS_KEY(KEY_P3) ) LED_3 = 1; else LED_3 = 0;

    if ( INP_I1 ) DOT_1 = 1; else DOT_1 = 0;
    if ( INP_I2 ) DOT_2 = 1; else DOT_2 = 0;
    if ( INP_I3 ) DOT_3 = 1; else DOT_3 = 0;

    if ( !Delay100 ) {
      Delay100 = 10;

      if ( IS_KEY(KEY_P1) )
        DigitValProg ( NtcValue );
      else
      if ( IS_KEY(KEY_P2) )
        DigitValProg ( PtcValue );
      else
      if ( IS_KEY(KEY_P3) )
        DigitValProg ( TaValue );
      else
        DigitValProg ( Tmp );


      //Buf[0] = Digit[0];
      //Buf[1] = Digit[1];
      //Buf[2] = Digit[2];
      //PageWrite_EEPROM ( 0x100, Buf, 3 );
      //Tab.Digit[0] = Digit[0];
      //Tab.Digit[1] = Digit[1];
      //Tab.Digit[2] = Digit[2];
      //Temp = WriteEeprom ( &Tab );

      ToByte(OutPut) = Tmp;
      Tmp <<= 1;
      if ( !Tmp ) Tmp = 0x01;

      /*
      Digit[2] = Digit[0] = Tmp;
      ToByte(SetLed) = Tmp;
      Tmp++; if ( Tmp >= 63 ) Tmp = 0;
      */
      if ( Temp )
        Play_Buzzer ( 4000, 10 );

      //ToByte(Digit_1) = Tmp;
      //ToByte(Digit_2) = Tmp;
      //ToByte(Digit_3) = Tmp;
      //Tmp >>= 1;
      //if ( !Tmp ) {
      //  Tmp = 0x80;
      //  Play_Buzzer ( 4000, 10 );
      //}
    }

  }
}


#endif









//-------------------------------------------------------------
//-------------------------------------------------------------
//
//
//      Funzioni per Eventi Temporizzati
//      Devono essere definite in uno file .c
//        vengoo chiamate in automatico
//
//
//-------------------------------------------------------------
//-------------------------------------------------------------


void Timer1mSec ( void )
{
#ifndef     DISABLE_HILEV
  Timer_A1();
#endif
}

UCHAR HiLevDisplay = 1;

void Timer10mSec ( void )
{
#ifndef     DISABLE_HILEV
  Timer_A3();
#else
  //////////////////////////////////////        DA ELIMINARE
  // Rinfresco del DIGIT e dei LED
  SetDigitLed();                        // TO CARPIGINAI -> Puoi chiamarlo nella DoIdle ('Is10mSec')

#ifdef      COLLAUDO_MACROTEK
  if ( HiLevDisplay )
#endif
  SendToDigit();                        // TO CARPIGINAI -> Lascialo nella DoIdle ('Is10mSec' o 'Is100mSec')

  // Lettura dei tasti con antirimbalzo
  RimbKey();                            // TO CARPIGIANI -> Lascialo nella DoIdle ('Is10mSec')

  // Controllo Ingressi Esterni
  CheckIngressi();                      // TO CARPIGINAI -> Puoi chiamarlo nella DoIdle ('Is10mSec')
  //////////////////////////////////////        DA ELIMINARE
#endif
}

void Timer100mSec ( void )
{
#ifndef     DISABLE_HILEV
  Timer_A0();
  Timer_A4();
#endif
}

void Timer1Sec ( void )
{
}

#ifdef      COLLAUDO_MACROTEK
volatile UCHAR TempZC;
#endif


void ZeroCrossing ( void )
{
#ifndef     DISABLE_HILEV
  Int_Int3();
#else
#ifdef      COLLAUDO_MACROTEK
  TempZC++;
#endif
#endif
}






