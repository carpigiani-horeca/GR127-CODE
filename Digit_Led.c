/* *******************************************************************************************
 *                                                                                           *
 *                         GESTIONE SCHEDA DIGIT-LED-PULSANTI                                *
 *                                                                                           *
 * *******************************************************************************************/




#include "string.h"
#include "defs.h"
#include "prot.h"
#include "191barpsp.h"













// *************************** Gestione dei DIGIT e LED ***************************
volatile UC CntDsp;

// *** Spegne tutto il tastierino
void AllDigitLedOff ( void )
{
  StringVal("   ");

  DOT_1 = 0;
  DOT_2 = 0;
  DOT_3 = 0;

  LED_4 = 0;
  LED_1 = 0;
  LED_2 = 0;
  LED_3 = 0;

} // END AllOff()

// *** Accende tutto il tastierino
void AllDigitLedOn ( void )
{
  DigitVal( 888 );

  DOT_1 = 1;
  DOT_2 = 1;
  DOT_3 = 1;

  LED_4 = 1;
  LED_1 = 1;
  LED_2 = 1;
  LED_3 = 1;

} // END AllOn()

// *** Attiva tutto il lampeggio
void LampAllDigitLedOn( void )
{
  LAMP_DIGIT_1 = 1;
  LAMP_DIGIT_2 = 1;
  LAMP_DIGIT_3 = 1;

  LAMP_DOT_1 = 1;
  LAMP_DOT_2 = 1;
  LAMP_DOT_3 = 1;

  LAMP_LED_STOP = 1;
  LAMP_LED_PRODUZIONE = 1;
  LAMP_LED_CONSERVAZIONE = 1;
  LAMP_LED_PULIZIA = 1;

} // END LampAllOn()

// *** Disattiva tutto il lampeggio
void LampAllDigitLedOff ( void )
{
  LAMP_DIGIT_1 = 0;
  LAMP_DIGIT_2 = 0;
  LAMP_DIGIT_3 = 0;

  LAMP_DOT_1 = 0;
  LAMP_DOT_2 = 0;
  LAMP_DOT_3 = 0;

  LAMP_LED_STOP = 0;
  LAMP_LED_PRODUZIONE = 0;
  LAMP_LED_CONSERVAZIONE = 0;
  LAMP_LED_PULIZIA = 0;

} // END LampAllOff()

// *** Inserisce un numero di 3 cifre nell' array Digit[] ( senza visualizzare le stringhe degli allarmi )
void DigitValProg ( int Val )
{

  if ( ( Val >= 0 ) && ( Val <= 999 ) ) {
    Digit[0] = Val % 10; // unita'
    Val /= 10;
    Digit[1] = Val % 10; // decine
    Val /= 10;
    Digit[2] = Val % 10; // centinaia
  } else
        if ( ( Val >= -99 ) && ( Val < 0 ) ) {
          Val = - Val;
          Digit[0] = Val % 10; // unita'
          Val /= 10;
          Digit[1] = Val % 10; // decine
          Val /= 10;
          Digit[2] = 63;
        } else {
                 Digit[0] = 63;
                 Digit[1] = 63;
                 Digit[2] = 63;
        }

} // END DigitValProg()


// *** Inserisce 3 caratteri nell' array Digit[]
void StringVal ( const char *c )
{
  if ( *c >= '0' && *c <= '9' )
      Digit[2] = *c - '0';
  if ( *c >= 'A' && *c <= 'Z' )
      Digit[2] = *c - 'A' + 10;
  if ( *c >= 'a' && *c <= 'z' )
      Digit[2] = *c - 'a' + 36;
  if ( *c == ' ' )
      Digit[2] = *c - ' ' + 62;
  if ( *c == '-' )
      Digit[2] = *c - '-' + 63;

  c++;

  if ( *c >= '0' && *c <= '9' )
      Digit[1] = *c - '0';
  if ( *c >= 'A' && *c <= 'Z' )
      Digit[1] = *c - 'A' + 10;
  if ( *c >= 'a' && *c <= 'z' )
      Digit[1] = *c - 'a' + 36;
  if ( *c == ' ' )
      Digit[1] = *c - ' ' + 62;
  if ( *c == '-' )
      Digit[1] = *c - '-' + 63;

  c++;

  if ( *c >= '0' && *c <= '9' )
      Digit[0] = *c - '0';
  if ( *c >= 'A' && *c <= 'Z' )
      Digit[0] = *c - 'A' + 10;
  if ( *c >= 'a' && *c <= 'z' )
      Digit[0] = *c - 'a' + 36;
  if ( *c == ' ' )
      Digit[0] = *c - ' ' + 62;
  if ( *c == '-' )
      Digit[0] = *c - '-' + 63;

} // END StringVal()


const char *c1;
const char *c2;
const char *c3;

// *** Inserisce un numero a 3 cifre nell' array Digit[] e visualizza le stringhe degli allarmi
void DigitVal ( int Val )
{
  if ( c1 != Null ) {                    // se esiste la stringa c1 la visualizza - precedenza 1
    StringVal( c1 );
  } else

  if ( c3 != Null ) {                    // se esiste la stringa c2 la visualizza - precedenza 3
    if ( strcmp( c3,"W-n" ) == 0 ) {
      Val = WASH_Days;
      Digit[2] = 'W' - 'A' + 10;
      Digit[0] = Val % 10; // unita' del parametro WASH_Days
      Val /= 10;
      Digit[1] = Val % 10; // decine del parametro WASH_Days
    } else if ( strcmp( c3,"C-n" ) == 0 ) {
			    Val = Coni_LIV;
      			Digit[2] = 'C' - 'A' + 10;
      			Digit[0] = Val % 10; // unita' del parametro Coni_LIV
      			Val /= 10;
      			Digit[1] = Val % 10; // decine del parametro Coni_LIV
			} else {
        			StringVal( c3 );
    				}
  } else

  		if ( c2 != Null ) {                    // se esiste la stringa c2 la visualizza - precedenza 2
  		  StringVal( c2 );
  		} else

  if ( ( Val >= 0 ) && ( Val <= 999 ) ) {
    Digit[0] = Val % 10; // unita'
    Val /= 10;
    Digit[1] = Val % 10; // decine
    Val /= 10;
    Digit[2] = Val % 10; // centinaia
  } else
        if ( ( Val >= -99 ) && ( Val < 0 ) ) {
          Val = - Val;
          Digit[0] = Val % 10; // unita'
          Val /= 10;
          Digit[1] = Val % 10; // decine
          Val /= 10;
          Digit[2] = 63;
        } else {
                 Digit[0] = 63;
                 Digit[1] = 63;
                 Digit[2] = 63;
        }

} // END DigitVal()












int OldVal;                             // variabile per l' isteresi sul HOT




// *** visualizzazione HOT con isteresi e con limite a 100 per l' Autotaratura()
void DisplayHotAutotaratura ( int Val )
{
  if ( Val > 100 ) {
    DigitVal( 100 );
  } else {
      if ( ( Val > OldVal ) || ( Val < OldVal-4 ) ) {
        DigitVal( Val );
        OldVal = Val;
      } else {
          DigitVal( OldVal );
      }
  }
} // END DisplayHotAutotaratura()







// *** Visualizzazione HOT con isteresi per la Produzione()
void DisplayHot_Barre ( int Val )
{

	if ( Val > ( TabProg.HOB + 2*Step_HOT ) ) StringVal( "---" );
	else if ( Val > ( TabProg.HOB + Step_HOT ) ) StringVal( "-- " );
	else if ( ( Val > ( TabProg.HOB ) ) && ( Val <= ( TabProg.HOB + Step_HOT ) ) ) StringVal( "-  " );
	else DisplayHot( H_O_T );

/*  if ( ( Val > OldVal ) || ( Val < OldVal-4 ) ) {
    DigitVal( Val );
    OldVal = Val;
  } else {
      DigitVal( OldVal );
  }
*/
} // END DisplayHot()




// *** Visualizzazione HOT con isteresi per la Produzione()
void DisplayHot ( int Val )
{

  if ( ( Val > OldVal ) || ( Val < OldVal-4 ) ) {
    DigitVal( Val );
    OldVal = Val;
  } else {
      DigitVal( OldVal );
  }

} // END DisplayHot()













// ****************************** Gestione dei PULSANTI *******************************






// Inizializzazione variabili per la Gestione dei PULSANTI
//    NewKey = 0;   // indica se un nuovo tasto viene premuto
//    NoKey = 1;    // indica se nessun tasto e' premuto
//    PressCnt = 0; // conta il tempo di pressione di un tasto








/*
__________________________________________________________________________________________________

                           ____         ____
                          \    \       \    \
                          \ P1 \       \ P2 \
                          \____\       \____\



    // ************** Gestione dei tasti (pulsanti): P1, P2, P1+P2 **************

    DoIdle();

    if ( NewKey ) { // Se e' stato premuto un tasto...


      // ********************** Gestione dei tasti P1+P2 ************************

      if ( IS_KEY( KEY_P1P2 ) ) { // Se il tasto premuto e' P1+P2...
        NewKey = 0;
        while ( IS_KEY( KEY_P1P2 ) ) { // Controllo del tempo di pressione del P1+P2
            DoIdle();
            if ( PressCnt >= LONGPRES3 )
              break;
        }
        if ( IS_KEY( KEY_P1P2 ) ) { // Se la pressione del P1+P2 e' prolungata...

            // Azioni eseguite con P1+P2 premuti per un tempo lungo LONGPRES3
            StringVal( "333" );
            Wait10mSec( 100 );
            StringVal( "   " );

        } else { // Se la pressione del P1+P2 e' breve...

            // Azioni eseguite con P1+P2 premuti per un tempo breve
            StringVal( " 3 " );
            Wait10mSec( 100 );
            StringVal( "   " );

        }

      } // END gestore tasti P1+P2 **********************************************


      // ********************** Gestione del tasto P2 ***************************

			if ( IS_KEY( KEY_P2 ) ) { // Se il tasto premuto e' P2...
        NewKey = 0;
				while ( IS_KEY( KEY_P2 ) ) { // Controllo del tempo di pressione del P2
					DoIdle();
          if ( PressCnt >= LONGPRES2 )
						break;
				}
        if ( IS_KEY( KEY_P2 ) ) { // Se la pressione del P2 e' prolungata...

            // Azioni eseguite con P2 premuto per un tempo lungo LONGPRES2
            StringVal( "222" );
            Wait10mSec( 100 );
            StringVal( "   " );

        } else { // Se la pressione del P2 e' breve...

            // Azioni eseguite con P2 premuto per un tempo breve
            StringVal( " 2 " );
            Wait10mSec( 100 );
            StringVal( "   " );

        }

      } // END gestore tasto P2 *************************************************


      // ********************** Gestione del tasto P1 ***************************

			if ( IS_KEY( KEY_P1 ) ) { // Se il tasto premuto e' P1...
        NewKey = 0;
				while ( IS_KEY( KEY_P1 ) ) { // Controllo del tempo di pressione del P1
					DoIdle();
          if ( PressCnt >= LONGPRES1 )
						break;
				}
        if ( IS_KEY( KEY_P1 ) ) { // Se la pressione del P1 e' prolungata...

            // Azioni eseguite con P1 premuto per un tempo lungo LONGPRES1
            StringVal( "111" );
            Wait10mSec( 100 );
            StringVal( "   " );

        } else { // Se la pressione del P1 e' breve...

            // Azioni eseguite con P1 premuto per un tempo breve
            StringVal( " 1 " );
            Wait10mSec( 100 );
            StringVal( "   " );

        }

      } // END gestore tasto P1 *************************************************


    } // END gestione della pressione di un tasto *******************************



______________________________________________________________________________________________________
*/
