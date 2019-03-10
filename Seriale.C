/***********************************************************************************************
*                                                                                              *
*                    Procedure per la gestione della seriale   UART-1                          *
*                                                                                              *
************************************************************************************************/









#include "defs.h"
#include "prot.h"










void CheckRx ( void );





//____________________
// **** Interrupt ****

void PcRxInt ();
#pragma INTERRUPT  PcRxInt

void PcTxInt ();
#pragma INTERRUPT  PcTxInt









UC SerByte;                             // dato seriale

// Variabili per la Trasmissione
volatile struct ByteBit UartStat;
UC TxBuf[MAXLEN + 8];                   // buffer di trasmissione
UC ByteToSend;                          // byte da mandare
UC ByteSended;                          // byte mandati

// Variabili per la Ricezione
struct StrRxData RxData;
UC RxCnt = 0;
WORD RxCheckSum;





//_________________________________________________________
// *** Interrupt per la ricezione di un dato seriale dal PC
void PcRxInt ( void )
{
/*
  UC Status;

  // Legge il dato Seriale
  SerByte = u1rbl;
  Status = u1rbh;

  if ( Status & 0x20 )
  {
                          // Esce con overframe error
  } else {
      CheckRx();          // Prossimo byte
  }
*/
} // END PcRxInt()






//________________________________________________
// *** Interrupt per la trasmissione di dati al PC
void PcTxInt ( void )
{
/*
  ByteSended++;
  if ( ByteSended < ByteToSend  ) {
    u1tbl = TxBuf[ ByteSended ];        // invio del byte 1, 2, 3,...
  } else {
      MsgTxEnd = 1;                     // messaggio inviato
  }
*/
} // END PcTxInt()






// *** Procedura di attese della fine trasmissione dei dati
void WaitTxEnd ( void )
{
/*
    while ( MsgTxEnd == 0 ) {
     DoIdle();
    }
*/
} // END WaitTxEnd()






// *** Routine di controllo dei dati ricevuti da Seriale
void CheckRx ( void )
{
  UC Dato;

  Dato = SerByte;

  switch ( RxCnt ) {
    case 0 :                                          // StartByte
      if ( (Dato == STARTBYTE) && !MsgRxEnd ) {
        RxData.Start = Dato;
        RxCheckSum = 0;
        RxCnt = 1;
      }
      break;

    case 1 :                                          // Len
      if ( Dato > (MAXLEN+4) ) {
        RxCnt = 0;
      } else {
        RxCheckSum += Dato;
        RxData.Len = Dato;
        RxCnt = 2;
      }
      break;

    case 2 :                                          // Code
      RxCheckSum += Dato;
      RxData.Code = Dato;
      RxCnt = 3;
      break;

    case 3 :                                          // Address H
      RxCheckSum += Dato;
      RxData.Address = ((WORD)Dato) << 8;
      RxCnt = 4;
      break;

    case 4 :                                          // Address L
      RxCheckSum += Dato;
      RxData.Address |= Dato;
      RxCnt = 5;
      break;

    default :                                         // Dati o CheckSum
      if ( RxCnt <= RxData.Len ) {
        // Dato
        RxCheckSum += Dato;
        RxData.Data[ RxCnt-5 ] = Dato;
        RxCnt++;
      } else {
        // CheckSum
        if ( RxCnt == (RxData.Len+1) ) {
          // CheckSumH
          RxData.CheckSum = ((WORD)Dato) << 8;
          RxCnt++;
        } else
          if ( RxCnt == (RxData.Len+2) ) {
            // CheckSumL
            RxData.CheckSum |= Dato;
            // Controllo CheckSum
            if ( RxData.CheckSum == RxCheckSum )
              MsgRxEnd = 1;           // CheckSum OK
            RxCnt = 0;
          } else
            RxCnt = 0;
      }
      break;
  }

} // END CheckRx()







// *** Invia un messaggio tramite seriale
void SendMsg ( UC Len )
{
/*
  if ( Len ) {
    ByteToSend = Len + 7;               // in MakeMsg() vengono aggiunti 7 byte
    ByteSended = 0;
    MsgTxEnd = 0;
    u1tbl = TxBuf[ ByteSended ];        // invio del byte 0
  }
*/
} // END SendMsg()






// *** Compone il messaggio per la spedizione via seriale
void MakeMsg ( UC Id, const UC *Buf, UC LenData )
{
  UC Cnt;
  WORD Check;

  if ( LenData > MAXLEN )
    LenData = MAXLEN;                           // controllo sulla lunghezza massima
  TxBuf[ 0 ] = STARTBYTE;                       // byte di start
  TxBuf[ 1 ] = 4 + LenData;                     // byte di lunghezza
  TxBuf[ 2 ] = Id;                              // byte di identificazione
  TxBuf[ 3 ] = RxData.Address >> 8;             // byte di indirizzo alto
  TxBuf[ 4 ] = (UC)RxData.Address;              // byte di indirizzo basso
  for ( Cnt = 0 ; Cnt < LenData ; Cnt++ )       // inserimento dei dati
    TxBuf[ 5 + Cnt ] = Buf[ Cnt ];
  Check = 0;
  for ( Cnt = 1 ; Cnt < LenData + 5 ; Cnt++ )   // creazione del check-sum
    Check += TxBuf[ Cnt ];
  TxBuf[ LenData + 5 ] = Check >> 8;            // byte di check-sum basso
  TxBuf[ LenData + 6 ] = (UC)Check;             // byte di chech-sum alto
  // n.b.: vengono aggiunti 7 byte

} // END MakeMsg()




UC HexAscii ( UC Dato );


// *** Converte da esadecimale ad ASCII
void HexToAscii ( UC *Buf, WORD Val, UC Dim )
{
  UC Dato;

  if ( Dim == 4 ) {
    Dato = Val >> 8;
    Buf[0] = HexAscii ( Dato >> 4 );
    Buf[1] = HexAscii ( Dato & 0x0F );
    Dato = Val;
    Buf[2] = HexAscii ( Dato >> 4 );
    Buf[3] = HexAscii ( Dato & 0x0F );
  } else {
    Dato = Val;
    Buf[0] = HexAscii ( Dato >> 4 );
    Buf[1] = HexAscii ( Dato & 0x0F );
  }
} // END HexToAscii()



// *** Converte da esadecimale ad ASCII
UC HexAscii ( UC Dato )
{
  if ( Dato > 9 )
    return 'A' + Dato - 0x0A;
  else
    return '0' + Dato;
} // END HexAscii()





// *** Converte da binario ad ASCII
void BinToAscii ( UC *Buf, WORD Val, UC Dim )
{
  UC Dato;

  Buf += Dim - 1;
  for ( ; Dim ; Dim-- )
  {
    Dato = Val % 10;
    *Buf-- = '0' + Dato;
    Val /= 10;
  }
} // END BinToAscii()





UC TxBufLength;

// *** Aggiunge al buffer "TxBuf" il buffer "Buf"
void AppendToBuffer ( UC far *Buf, UC Dim, UC Space )
{
  for ( ; Dim ; Dim-- ) {
    TxBuf[TxBufLength] = *Buf++;
    TxBufLength++;
  }

  if ( Space == 1 ) {
    TxBuf[TxBufLength] = ' ';
    TxBufLength++;
  }

} // END AppendToBuffer()





// *** Invia su seriale il buffer "TxBuf"
void SendBuffer ( UC Wait )
{
/*
  ByteToSend = TxBufLength;
  ByteSended = 0;
  MsgTxEnd = 0;
  u1tbl = TxBuf[ByteSended];

  if ( Wait )
    WaitTxEnd();

  TxBufLength = 0;                      // reinizializza TxBufLength
*/
} // END SendBuffer()






// *** Controlla i pacchetti via RS232
BYTE ControllaRS232 ( UC lettomessaggio )
{
  BYTE Code;

  if ( MsgRxEnd ) {
    Code = RxData.Code;
  }
  if ( lettomessaggio ) {
    MsgRxEnd = 0;
  }
  return Code;

} // END ControllaRS232()
