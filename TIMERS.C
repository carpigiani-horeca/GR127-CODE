/***********************************************************************
 *                                                                     *
 *               Interrupt e Temporizzazioni ( Timers )                *
 *                                                                     *
 ***********************************************************************/







#include "defs.h"
#include "prot.h"
#include "191barpsp.h"















// ********************* Definizione delle Variabili relative ai Timers ******************************


// *** flags per le tempistiche nella DoIdle()
volatile UC Is500mSec;
volatile UC Is200mSec;
volatile UC Is100mSec;
volatile UC Is10mSec;
volatile UC Is1mSec;
volatile UC Is1Sec;
volatile UC Is1Min;


// *** flags per le tempistiche nel Main()
volatile UC Main_100mSec;
volatile UC Main_10mSec;
volatile UC Main_1mSec;
volatile UC Main_1Sec;
volatile UC Main_1Min;

// *** flags per le tempistiche nelle funzioni della 191 Bar Pastorizzante
volatile UC Prd_10mSec;                 // per Produzione()
volatile UC Mon_100mSec;                // per Monitoring()
volatile UC Stop_100mSec;               // per Stop()
volatile UC Prd_100mSec;                // per Produzione()
volatile UC Sto_100mSec;                // per Conservazione()
volatile UC Cle_100mSec;                // per Pulizia()
volatile UC Pas_100mSec;                // per Pastorizzazione()
volatile UC Select_100mSec;             // per Selezione()
volatile UC Prd_1Sec;                   // per Produzione()
volatile UC Stop_1Min;                  // per Stop()
volatile UC Mon_1Min;                   // per Monitoring()
volatile UC Pas_1Min;                   // per Pastorizzazione()

// *** contatore decrementato in interrupt usato in ProgrammazioneUtente() e ProgrammazioneTecnico()
volatile UC Speed_100mSec;

// *** contatori decrementati in interrupt e usati nelle funzioni Wait()
volatile UC Cnt100mSec;
volatile UC Cnt10mSec;
volatile UC Cnt1mSec;
volatile UC Cnt1Sec;
volatile UC Cnt1Min;


// *** contatori decrementati in interrupt e usati nelle funzioni di interrupt - NON USARE ALTROVE !
volatile UC Cnt1mSecZC;
volatile UC ZC_Cnt = 200;
volatile UC ZC_Num;
volatile UC DurataZC;
volatile UC Cnt_PWM_Ventola;

// *** contatore per lampeggio dei LED del tastierino della 191 Bar Pastorizzante
volatile UC CntForLamp;

// *** contatore usato per la seriale
volatile UC CntForDelay;

// *** contatore usato per la EEPROM
volatile UC Polling;



// ***************************** Routines di Interrupt *****************************************


UC Delay;
UC CntDisp;
UC PWM_MV_Periodo;
UC PWM_MV_Tempo_OFF;

volatile UC CntForSec;                  // per passare da mSec a Sec - NON USARE ALTROVE !
volatile UC CntFor200mSec;              // per passare da 100 msec a 200 mSec - NON USARE ALTROVE !
volatile UC CntFor500mSec;              // per passare da 100 msec a 500 mSec - NON USARE ALTROVE !
volatile UC CntForMin;                  // per passare da Sec a Min - NON USARE  ALTROVE !







/*==================================================
  Timer_A0 (a 100 mSec convertito) a 1 Sec e a 1 Min
  ==================================================*/
void Timer_A0 ( void )
{

    // *** Timer a 100 mSec convertito a 1 Sec
    //----------------------------------------
    if ( CntForSec )
      CntForSec--;
    else {
          CntForSec = 9;

          // ******************** Tempistiche a 1 Sec ***********************************
          //_____________________________________________________________________________
          Is1Sec = 1;                   // flag a 1 sec per la DoIdle()
          Main_1Sec = 1;                // flag a 1 sec per il Main()
          Prd_1Sec = 1;                 // flag a 1 sec per la Produzione()

          // Contatore a 1 Sec per funzioni Wait()
          if ( Cnt1Sec )
              Cnt1Sec--;

          // *** Timer a 1 Sec convertito a 1 Min
          //-------------------------------------
          if ( CntForMin )
              CntForMin--;
          else {
                CntForMin = 59;

                // ************** Tempistiche a 1 Min ***********************************
                //_______________________________________________________________________
                Is1Min = 1;             // flag a 1 min per la DoIdle()
                Main_1Min = 1;          // flag a 1 min per il Main()
                Stop_1Min = 1;          // flag a 1 min per lo Stop()
                Mon_1Min = 1;           // flag a 1 min per il Monitoring()

                // Contatore a 1 Min per funzioni Wait()
                if ( Cnt1Min )
                    Cnt1Min--;
          }
    }
}





/*==================================================
   Timer_A1 a 1 mSec
  ==================================================*/
void Timer_A1 ( void )
{
    // *** Tempistiche a 1 mSec ***
    //_____________________________
    Is1mSec = 1;                          // flag a 1 millisec per la DoIdle
    Main_1mSec = 1;                       // flag a 1 msec per il main

    // Contatore a 1 mSec usato in interrupt ZC  ( per verificare la presenza dello Z.C. )
    if ( ZC_Cnt )
        ZC_Cnt --;

    // Contatore a 1 mSec usato in interrupt ZC ( per controllare la correttezza dello Z.C. )
    DurataZC ++;

    // Contatore a 1 mSec usato in interrupt ZC
    if ( Cnt1mSecZC )
        Cnt1mSecZC--;

    // Contatore a 1 mSec per funzioni Wait()
    if ( Cnt1mSec )
        Cnt1mSec--;

    // Contatore a 1mSec per funzioni su EEPROM-seriale
    if ( Polling ) {
      Polling--;
    }
}





/*==================================================
   Timer_A2 => PWM out per buzzer (NON utilizzare)
  ==================================================*/
void Timer_A2 ( void )
{
}





/*==================================================
   Timer_A3 a 10 mSec
 ===================================================*/
void Timer_A3 ( void )
{
    // *** Tempistiche a 10 mSec ***
    //______________________________
    Is10mSec = 1;                       // flag a 10 mSec per DoIdle
    Main_10mSec = 1;                    // flag a 10 msec per main()
    Prd_10mSec = 1;                     // flag a 10 mSec per Produzione()

    // Contatore a 10 mSec per funzioni Wait()
    if ( Cnt10mSec )
        Cnt10mSec--;

} // END Timer_A3()





/*==================================================
   Timer_A4 a 100 mSec
  ==================================================*/
void Timer_A4 ( void )
{
    // *** Tempistiche a 100 mSec ***
    //_______________________________
    Is100mSec = 1;                      // flag a 100 mSec per la DoIdle
    Main_100mSec = 1;                   // flag a 100 msec per il main
    Mon_100mSec = 1;                    // flag a 100 msec per il Monitoring
    Stop_100mSec = 1;                   // flag a 100 msec per lo Stop
    Prd_100mSec = 1;                    // flag a 100 msec per la Produzione
    Sto_100mSec = 1;                    // flag a 100 msec per la Conservazione
    Cle_100mSec = 1;                    // flag a 100 msec per la Pulizia
    Pas_100mSec = 1;                    // flag a 100 msec per la Pastorizzazione
    Select_100mSec = 1;                 // flag a 100 msec per la Selezione

    // Contatore a 100 mSec per ProgrammazioneUtente() e ProgrammazioneTecnico()
    if ( Speed_100mSec )
      Speed_100mSec--;

    // Contatore a 100 mSec per funzioni Wait()
    if ( Cnt100mSec )
        Cnt100mSec--;

    // Contatore per lampeggio Digit e Led
    if ( CntForLamp )
        CntForLamp--;
    else {
        CntForLamp = 3;                 // Lampeggio a CntForLamp x 100 mSec
        LampOn = !LampOn;
    }

    //--------------------------------------------------------------------------------------
    if ( CntFor200mSec )
      CntFor200mSec --;
    else {
          CntFor200mSec = 1;

          // ******************** Tempistiche a 500 mSec ***********************************
          //_____________________________________________________________________________
          Is200mSec = 1;
    }
    //--------------------------------------------------------------------------------------
    if ( CntFor500mSec )
      CntFor500mSec --;
    else {
          CntFor500mSec = 4;

          // ******************** Tempistiche a 500 mSec ***********************************
          //_____________________________________________________________________________
          Is500mSec = 1;
    }

} // END Timer_A4()







UC OK_ZC;


/*====================================================
   Interrupt Zero-Crossing ( One edge, falling edge )
  ====================================================*/
void Int_Int3 ( void )
{
  // Controllo delle tempistiche dello Z.C.
  if ( (DurataZC > 18) || (DurataZC < 22) )
    OK_ZC = 0;
  else
	OK_ZC = 1;
	   if ( ( M_V || M_V_Cabinet ) && MV_Enable )
		{
			Cnt_PWM_Ventola ++;
			if ( ( Cnt_PWM_Ventola >= ( (PWM_MV_Periodo) - (PWM_MV_Tempo_OFF) + 1 ) ) && ( Cnt_PWM_Ventola <= (PWM_MV_Periodo) ) )
			{
				// M_V_rit = 0;
			} else
				// M_V_rit = 1; // !(Cnt_PWM_Ventola % PWM_Ventola);
			if ( Cnt_PWM_Ventola == (PWM_MV_Periodo) )
			{
				Cnt_PWM_Ventola = 0;
			}
		} //else  M_V_rit = M_V_H;

  DurataZC = 0;                         // contatore incrementato nell'interrupt a 1 mSec

  // Eliminazione di evntuali ZC spuri
  if ( Cnt1mSecZC )                     // dopo il primo ZC non prende piu' ZC per 5mSec
      return;
  Cnt1mSecZC = 5;

  // Contatore per monitorare lo Zero-Crossing => se ZC_Cnt -> 0 non c'e' piu' lo Zero-Crossing
  ZC_Cnt = 30;                            // viene decrementato nell'interrupt a 1 mSec
  // Conta gli Zero-crossing dopo un black-out
  if ( ZC_Num )                           // viene inizializzato nella DoIdle() a 1 mSec
    ZC_Num --;
} // END Int_Int3()




















// ***************************** Funzioni Wait *****************************************


UC Time;



/*------------------------------------------------
   Attende un numero precisato di minuti
--------------------------------------------------*/
void WaitMin ( UC Time )
{
  Cnt1Min = Time;
  while ( Cnt1Min )
    DoIdle();
}





/*------------------------------------------------
   Attende un numero precisato di secondi
--------------------------------------------------*/
void WaitSec ( UC Time )
{
  Cnt1Sec = Time;
  while ( Cnt1Sec )
    DoIdle();
}





/*------------------------------------------------
   Attende un numero precisato di millisecondi
--------------------------------------------------*/
void WaitmSec ( UC Time )
{
  Cnt1mSec = Time;
  while ( Cnt1mSec )
    DoIdle();
}





/*---------------------------------------------------
   Attende un numero precisato di decimi di secondo
-----------------------------------------------------*/
void Wait10mSec ( UC Time )
{
  Cnt10mSec = Time;
  while ( Cnt10mSec )
    DoIdle();
}





/*------------------------------------------------------
   Attende un numero precisato di centesimi di secondo
-------------------------------------------------------*/
void Wait100mSec ( UC Time )
{
  Cnt100mSec = Time;
  while ( Cnt100mSec )
    DoIdle();
}

























// ***************************** Funzione DoIdle *****************************************





/*------------------------------------------------
   Funzione chiamata nei momenti di Idle
--------------------------------------------------*/
void DoIdle ( void )
{

    //_________________________________________________________________
    if ( Is1mSec ) {
      // *** Periodo = 1 mSec.
      Is1mSec = 0;

      // ***********************

      // Blocco della macchina in caso di black-out
      if ( ZC_Cnt == 0 ) {              // Controllo dello Zero-Crossing
        AllDigitLedOff();               // Spegne il tastierino
        ZC_Num = 20;                    // ZC_Num viene decrementato nell'Interrupt-Zero-Crossing
        while ( ZC_Num && DurataZC )    // se torna lo Zero-Crossing ne vengono contati ZC_Num prima di ripartire
          ;
      }

    }


    //_________________________________________________________________
    if ( Is10mSec ) {
      // *** Periodo = 10 mSec.
      Is10mSec = 0;

      // ***********************

      // Rinfresco del DIGIT e dei LED
      SetDigitLed();                        // TO CARPIGINAI -> Puoi chiamarlo nella DoIdle ('Is10mSec')

      // Controllo Ingressi Esterni
      CheckIngressi();                      // TO CARPIGINAI -> Puoi chiamarlo nella DoIdle ('Is10mSec')

      RimbKey();                          // Lettura dei tasti con antirimbalzo

      SendToDigit();                      // Caricamento valori sul DIGIT/LED

	  /* if ( State != 5 )                  // commentare per usare gli eventi, togliere il commento per usare hyperterminal
	  {
	  	InviaRealTimeLog();

	  } */
	      }


    //_________________________________________________________________
    if ( Is100mSec ) {
      // *** Periodo = 100 mSec.
      Is100mSec = 0;

      // ***********************

      if ( CntForDelay )                // contatore usato per mandare i dati via seriale ogni 1 Sec
        CntForDelay --;

    }


    //_________________________________________________________________
    if ( Is200mSec ) {
      // *** Periodo = 200 mSec.
      Is200mSec = 0;

      // ***********************

    }


    //_________________________________________________________________
    if ( Is500mSec ) {
      // *** Periodo = 500 mSec.
      Is500mSec = 0;

      // ***********************

    }


    //_________________________________________________________________
    if ( Is1Sec ) {
      // *** Periodo = 1 Sec.
      Is1Sec = 0;

      // ***********************
    }


    //_________________________________________________________________
    if ( Is1Min ) {
      // *** Periodo = 1 Min.
      Is1Min = 0;

      // ***********************

      Pas_1Min = 1;
    }


} // END DoIdle






