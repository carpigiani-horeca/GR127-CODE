//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//              Gestione :      DSIPLAY LCD
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
#include <string.h>
#include "Sfr_r825.h"
#include "defsLow.h"
#include "protLow.h"
#include "Interrupt.h"



#ifdef      INCLUDE_DISPLAY



UCHAR *DspPnt;     // Puntatore dati da scrivere
UCHAR DspNum;      // Numero di dati da scrivere * 2

struct BitStruct LcdState;
struct BitStruct LcdStates;

UCHAR CurPos;
UCHAR Bufedi[34];              /* Buffer del display */
UCHAR TempLcdBuf[17];          /* Buffer Temporaneo per le scritte su Display ( es. Numeri ) */

volatile UCHAR TimeOutLcd;



#define         FALSE       0
#define         TRUE        1






















//-----------------------------------------------------------------------------------

//              LOWLEVEL

//-----------------------------------------------------------------------------------





/*-----------------12-04-96 12:33pm-----------------
   attende il Busy dal display
--------------------------------------------------*/
void WaitBusyLCD ( void )
{
  UCHAR Dati;


  // TimeOut
  TimeOutLcd = 20;

  // Sistemazione Pin
  LcdPortDir = 0x00;
  LcdRead;
  LcdComandi;

  // Lettura BusyFlag
  do {

    EnaLcd; EnaLcd;
    Dati = LcdPort;
    DisLcd; DisLcd;

  } while ( (Dati & 0x80) && TimeOutLcd );


  // Controlla lo Stato del Display
  if ( Dati & 0x80 )
	ReqInit = 1;					/* time out => richiedo reset del display */
}







/*-----------------12-03-96 05:57pm-----------------
  invia un comando al display
--------------------------------------------------*/
void LcdCmd ( UCHAR Carattere , UCHAR Wait_BF )
{
  if ( Wait_BF )       /* attende il busy falg */
    WaitBusyLCD();


  // Sistemazione Pin
  LcdWrite;
  LcdComandi;
  LcdPortDir = 0xFF;

  // Invio Dati
  EnaLcd; EnaLcd;
  LcdPort = Carattere;      // Invia il comando
  DisLcd; DisLcd;
}





/*-----------------12-04-96 09:25am-----------------
  invia un dato al display ( dato da visualizzare )
--------------------------------------------------*/
void LcdOut ( UCHAR Carattere )
{
  WaitBusyLCD();      /* attende il BUSY */

  // Sistemazione Pin
  LcdWrite;
  LcdDati;
  LcdPortDir = 0xFF;

  // Invio Dati
  EnaLcd; EnaLcd;
  LcdPort = Carattere;      // Invia il Dato
  DisLcd; DisLcd;
}






/*-----------------12-04-96 09:25am-----------------
  Preleva un dato dal display ( dato visualizzato )
--------------------------------------------------*/
UCHAR LcdIn ( void )
{
  UCHAR Dati;


  WaitBusyLCD();      /* attende il BUSY */

  // Sistemazione Pin
  LcdPortDir = 0x00;
  LcdRead;
  LcdDati;

  // Lettura
  EnaLcd; EnaLcd;
  Dati = LcdPort;      // Legge il Dato
  DisLcd; DisLcd;

  return Dati;
}



/*-----------------12-04-96 12:33pm-----------------
   Legge il Busy dal display
--------------------------------------------------*/
UCHAR LcdRdBusy ( void )
{
  UCHAR Dati;



  // Sistemazione Pin
  LcdPortDir = 0x00;
  LcdRead;
  LcdComandi;

  // Lettura BusyFlag
  EnaLcd; EnaLcd;
  Dati = LcdPort;
  DisLcd; DisLcd;

  // Controlla lo Stato del Display
  return Dati & 0x80;
}






















//-----------------------------------------------------------------------------------

//              Inizializzazione

//-----------------------------------------------------------------------------------


void WaitLcd ( UCHAR Wait )
{
  TimeOutLcd = Wait;
  while ( TimeOutLcd ) ;
}


/*-----------------12-03-96 05:14pm-----------------
  inizializzazione del display ( da fare in accensione
  o nel caso in cui il display si blocchi )
--------------------------------------------------*/
void LcdInit ( void )
{
  UINT i;
  UCHAR cnt;




  //---  Inizializzazione Hardware
  // IsInInit = 1;             /* sto inizializzando = ferma ogni accesso al display */



  //---  Inizializzazione Software 1/2
  /* ora non e' essere testato il bit Busy */
  WaitLcd ( 15 );
  LcdCmd ( 0x30 , FALSE );     /* Function Set to 8 bit interface  */

  WaitLcd ( 5 );               /* > 4.1 */
  LcdCmd ( 0x30 , FALSE );     /* Function Set to 8 bit interface  */

  WaitLcd ( 2 );               /* > 1 */
  LcdCmd( 0x30 , FALSE );      /* Function Set to 8 bit interface  */


  //---  Inizializzazione Software 2/2
  /* Dopo queste istruzioni puo' essere testato il bit Busy */
  LcdCmd ( 0x38 , TRUE );      /* Function Set to 8 bit interface */
                               /* N = 1 (LCD 2 linee) */
                               /* F = 0 (5 * 7 dot character font) */
  LcdCmd ( 0x38 , TRUE );      /* Function Set to 8 bit interface */
                               /* N = 1 (LCD 2 linee) */
                               /* F = 0 (5 * 7 dot character font) */
  LcdCmd ( 0x01 , TRUE );      /* Clear Display  */
  LcdCmd ( 0x06 , TRUE );      /* Entry Mode Set */




  //---  SetUp Flag
  ToByte(LcdState) = 8;
  Chars_On = TRUE;
  LcdCmd ( ToByte(LcdState) , TRUE );    /* Start with no cursor, no blink */
  CurPos = 0;

  ReqInit = 0;                 /* ho espletato il reset */
  // IsInInit = 0;             /* fine inizializzazione */


  //** mette ' ' al carattere 21 ( equivale allo spazio tra gli ultimi 3 caratteri RIGA1 )
  LcdCmd ( 0x94 , TRUE );
  LcdOut ( ' ' );

  //** mette ' ' al carattere 41 ( equivale allo spazio tra gli ultimi 3 caratteri RIGA2 )
  LcdCmd ( 0xD4 , TRUE );
  LcdOut ( ' ' );
}



















//-----------------------------------------------------------------------------------

//              Funzioni di Stampa ( LowLevel )

//-----------------------------------------------------------------------------------





/*-----------------12-03-96 05:52pm-----------------
  visualizza o meno il cursore
--------------------------------------------------*/
void Cursor ( UCHAR Toggle )
{
  if ( Toggle )
    ToByte( LcdState ) = 15;        /* cursore ON */
  else
    ToByte( LcdState ) = 12;        /* cursore OFF */

  LcdCmd ( ToByte( LcdState ) , TRUE );
}






/*-----------------12-03-96 05:52pm-----------------
  visualizza o meno oil cursore
--------------------------------------------------*/
void HCursor ( UCHAR Toggle )
{
  if ( Toggle )
    ToByte( LcdState ) = 14;        /* cursore ON */
  else
    ToByte( LcdState ) = 12;        /* cursore OFF */

  LcdCmd ( ToByte( LcdState ) , TRUE );
}






/*-----------------12-04-96 09:30am-----------------
  posiziona il cursore sul display
--------------------------------------------------*/
void SetCursor ( UCHAR Position )
{
   CurPos = Position;
   if ( Position > 15 )
   {
      Position &= 0x2F;
      Position |= 0x40;
   }
   Position |= 0x80;
   LcdCmd ( Position , TRUE );

   LcdCmd ( ToByte( LcdState ) , TRUE );  /* mantiene lo stato del lampeggio */
}


































//-----------------------------------------------------------------------------------

//              Funzioni di Stampa ( HiLevel )

//-----------------------------------------------------------------------------------


/*-----------------09-02-96 02:51pm-----------------
//  Copia sul Buffer del Display anche passando da un riga all'altra
--------------------------------------------------*/
void DspMemCopy ( const UCHAR *Pnt, UCHAR Len , UCHAR Pos )
{
  if ( Pos < 16 ) {

    // Riga 1
    if ( (Pos+Len) > 16 ) {
      memcpy ( &Bufedi[Pos], Pnt, 16-Pos );
      Pnt += 16-Pos;
      Len -= 16-Pos;
    } else {
      memcpy ( &Bufedi[Pos], Pnt, Len );
      Len = 0;
    }

  }

  if ( Len ) {

    // Riga 2
    Pos -= 16;
    if ( (Pos+Len) > 16 )
      Len = 16-Pos;
    memcpy ( &Bufedi[16+Pos], Pnt, Len );

  }
}



/*-----------------09-02-96 02:51pm-----------------
  Trasferisce sull'Lcd il contenuto del Buffer
--------------------------------------------------*/
void RefreshBuffer ( void )
{
  // Prima Riga
  Print ( LcdRiga1, 16, 0 );
  // Seconda Riga
  Print ( LcdRiga2, 16, 16 );
}


/*-----------------09-02-96 02:51pm-----------------
  Stampa una stringa semplice direttamente su Lcd
--------------------------------------------------*/
void Print ( const UCHAR *pnt , UCHAR Len , UCHAR Col )
{
  SetCursor ( Col );
  do {
    LcdOut ( *pnt );
    pnt++;
    Col++;
    SetCursor ( Col );
  } while ( --Len );
  CurPos = Col;
}



/** usata da printf **/
void PutCar ( UCHAR Car )
{
  LcdOut ( Car );
  CurPos++;
  SetCursor ( CurPos );
}











#endif
