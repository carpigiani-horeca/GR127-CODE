/**************************************************************************
 *                                                                        *
 *                                                                        *
 *                            Funzione MainLoop()                         *
 *                                                                        *
 *                                                                        *
 **************************************************************************/
#include "defs.h"
#include "prot.h"
#include "191barpsp.h"






UINT ImpostaRitardoZC ( void )
{
	return (TabProg.TIR * 10) + 50;       // +50 => compensazione per TIR=500
}








void MainLoop ( void )
{
	//--- Inizializzazione del micro
	asm("FCLR I");                                          /* Interrupt disable */
	SystemInit();
	PeripheralInit();
	asm("FSET I");                                          /* Interrupt enable */




	/* ******************** Inizializzazioni ********************************* */


	// Inizializzazione variabili della macchina a stati
	State = 0;                          // stato STOP (di default)
	OldState = 0;
	StatoSelect = S0_IN;
	AutosetupFlag = FALSE;              // flag che ricorda se e' avvenuto l'autosetup
	ConiLivFlag = FALSE;                // flag che permette di azzerare Coni_LIV nel monitoring
	CountDownFlag = FALSE;              // flag per il conteggio delle 24h
	BlackOut = FALSE;                   // flag che ricorda se e' avvenuto un Black-Out
	Str_Allarme1 = Null;
	Str_Allarme2 = Null;
	Str_Allarme3 = Null;

	// Inizializzazione variabili per la Gestione dei PULSANTI
	NewKey = 0;                         // variabile usata nella RimbKey
	NoKey = 1;                          // variabile usata nella RimbKey
	PressCnt = 0;                       // conta il tempo di pressione di un tasto

	TastoSelezioneBreve = FALSE;
	TastoSelezioneLungo = FALSE;
	TastoSelezioneExtraLungo = FALSE;
	TastoStopBreve = FALSE;
	TastoStopLungo = FALSE;
	TastoOffBreve = FALSE;
	TastoOffLungo = FALSE;

	TastoSelezioneBreveMon = FALSE;
	TastoSelezioneLungoMon = FALSE;
	TastoSelezioneExtraLungoMon = FALSE;
	TastoStopBreveMon = FALSE;
	TastoStopLungoMon = FALSE;


	// Inizializzazione variabili per il conteggio dei coni
	NumeroConi = 0;                     // variabile per il conteggio dei coni

	// Inizializzazione variabili Buzzer
	Buzzer_Enab = True;                 // abilitazione buzzer

	// Inizializzazione BUS-I�C per REAL-TIME-CLOCK
	ExtTimerInit();



	DoIdle();
	Wait100mSec( 15 );








	/* ***********************  Funzioni eseguite una sola volta  *********************************** */

	// Legge dal Real-Time-Clock
	TimerRdData( &Ora );
	SettaOrario ( &Campione, &Ora );

	// Lettura dalla struttura BlackOutData dall'EEPROM
	ReadDataBlackOut();

	// Lettura della tabella di programmazione da EEPROM
	CaricaTabellaProgrammazione();

	// Azzeramento-Contaconi / Autosetup (all'accensione premendo STOP / SELEZIONE)
	Setup();

	// Lettura della tabella di programmazione da EEPROM
	CaricaTabellaProgrammazione();

	// Test display
	TestDisplay();

	// Lettura dalla struttura BlackOutData dall'EEPROM
	ReadDataBlackOut();

	// Lettura pagina 0 della Card esterna (se presente)
	LeggiPagina0();


	// Condizioni di scelta stato iniziale
  State = 0;
	BlackoutState = 0;

	// Condizioni di scelta stato iniziale
	if ( ( AutosetupFlag == TRUE ) || ( OldState == 0) || ( OldState == 3 ) ) {            // va in STOP
		BlackoutState = 0;
	} else
		if ( ( AutosetupFlag == FALSE ) && ( OldState == 1 ) ) {                           // va in PRODUZIONE
			BlackoutState = 1;
		} else
			if ( ( AutosetupFlag == FALSE ) && ( OldState == 2 ) ) {                       // va in CONSERVAZIONE
				BlackoutState = 2;
			} else
				if ( ( AutosetupFlag == FALSE ) && ( OldState == 4 ) ) {                   // va in PASTORIZZAZIONE
					BlackoutState = 4;
				}
	if ( AutosetupFlag == TRUE ) {
		AutosetupFlag = FALSE;
		// ConiLivFlag = TRUE;         non serve !!! l'autosetup non deve resettare i ConiLiv
		// WashDaysFlag = TRUE;        non serve !!! l'autosetup non deve resettare i WashDays
		// PastoEndFlag = TRUE;        non serve !!! l'autosetup non deve resettare il PastoEnd
	}


	/* ****************************** Loop principale ***************************** */

	while ( 1 ) {

		DoIdle();                                     // funzione di servizio

		Monitoring();                                 // MONITORING
		// Selezione dello stato attivo
		switch ( State ) {
		case 0 : State = Stop();            break;  // STOP             => 0
		case 1 : State = Produzione();      break;  // PRODUZIONE       => 1
		case 2 : State = Conservazione();   break;  // CONSERVAZIONE    => 2
		case 3 : State = Pulizia();         break;  // PRODUZIONE SHAKE          => 4
		case 4 : State = Produzione();      break;  // PULIZIA          => 3
		case 6 : State = Select();          break;
		}





	}  // END while principale



} // END main
