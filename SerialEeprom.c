/***************************************************************************************
 *                                                                                     *
 *     Gestione EEPROM su BUS-I²C => 256K I2C CMOS Serial EEPROM ( Microchip )    *
 *                                                                                     *
 ***************************************************************************************/
#include "string.h"
#include "defs.h"
#include "prot.h"
#include "191barpsp.h"


extern volatile UCHAR Polling;



// ********************************** EEPROM su BUS-I²C *********************************


#define   EEPROM_SCK    p3_5                // serial clock (out)
#define   EEPROM_SDA    p3_4                // serial data  (in/out)


#define   EEPROM_SDA_OUT     pd3_4 = 1     // dal microcontrollore alla memoria => out
#define   EEPROM_SDA_IN      pd3_4 = 0     // dalla memoria al microcontrollore => in

#define   EEPROM_SDA_HIGH    p3_4 = 1;pd3_4 = 0      // SDA = 1, in
#define   EEPROM_SDA_LOW     pd3_4 = 1;p3_4 = 0      // out, SDA = 0

#define   EEPROM_SCK_OUT    pd3_5 = 1      // dal microcontrollore alla memoria => out
#define   EEPROM_SCK_IN     pd3_5 = 0      // dalla memoria al microcontrollore => in

#define   EEPROM_SCK_HIGH   p3_5 = 1;pd3_5 = 0      // SCK = 1, in
#define   EEPROM_SCK_LOW    pd3_5 = 1;p3_5 = 0      // out, SCK = 0


#define   EEPROM_SU_TIME    NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP   // set-up time
#define   EEPROM_HD_TIME    NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP   // hold time

#define   EEPROM_CK_TIME_HIGH    NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP    // clock high time
#define   EEPROM_CK_TIME_LOW     NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP    // clock low time

#define   CLOCK_CYCLE     EEPROM_CK_TIME_LOW; EEPROM_SCK_HIGH; EEPROM_CK_TIME_HIGH; EEPROM_SCK_LOW

#define   WRITE_CNTRL    0xA6     // 10100110
#define   READ_CNTRL     0xA7     // 10100111







#define YES_ACK   1
#define NO_ACK    0
#define WRITE_TIME  15

BYTE  BufferEEPROM[ 32 ];






// Generazione del bit di start
void StartBitEEPROM ( void )
{
  EEPROM_SDA_HIGH;
  EEPROM_CK_TIME_LOW;
  EEPROM_SCK_HIGH;
  EEPROM_SU_TIME;
  EEPROM_SDA_LOW;
  EEPROM_HD_TIME;
  EEPROM_SCK_LOW;                       // CLK remain LOW
}


// Generazione del bit di stop
void StopBitEEPROM ( void )
{
  EEPROM_SDA_LOW;
  EEPROM_CK_TIME_LOW;
  EEPROM_SCK_HIGH;
  EEPROM_SU_TIME;
  EEPROM_SDA_HIGH;
  EEPROM_HD_TIME;                       // CLK remain HIGH
}


// Acknowledge della memoria
UC AckInEEPROM ( void )                 // Ack dalla memoria al micro
{
  UC Ack;

  EEPROM_SDA_IN;
  EEPROM_CK_TIME_LOW;
  EEPROM_SCK_HIGH;
  EEPROM_CK_TIME_HIGH;
  Ack = EEPROM_SDA;
  EEPROM_SCK_LOW;

  return Ack;
}


// Acknowledge del micro
void AckOutEEPROM ( UC Ack )             // Ack dal micro alla memoria
{
  if ( Ack ) {
    EEPROM_SDA_LOW;                     // Yes Ack
  } else {
    EEPROM_SDA_HIGH;                    // No Ack
  }
  CLOCK_CYCLE;
  EEPROM_SDA_IN;
}






















//DOME 24/09/05
//---------------------------------------- Funzioni a BASSO LIVELLO

// ******** Scrive un Buffer di Dati sull'EeProm
// *** restituisce 0x8000 in caso di errore, 1 altrimenti
WORD PageWrite_EEPROM ( WORD Address, BYTE *DataByte, UC NumByte )
{
  UC cnt;
  UC AckKo;
  BYTE TmpData;


  Polling = WRITE_TIME;
  TmpData = WRITE_CNTRL;
  do {
      StartBitEEPROM();                 // send start bit
      for ( cnt=0; cnt<8; cnt++ ) {     // send write-control-word
        if ( TmpData & 0x80 ) {
          EEPROM_SDA_HIGH;
        } else {
            EEPROM_SDA_LOW;
        }
        TmpData <<= 1;
        CLOCK_CYCLE;
      }
      AckKo = AckInEEPROM();            // receive acknowledge bit
      TmpData = WRITE_CNTRL;
  } while ( AckKo && Polling );         // acknowledge polling

  if ( AckKo ) {
    StopBitEEPROM();                    // send stop bit
    return 0x8000;                      // esce con 0x8000 in caso di errore
  }

  for ( cnt=0; cnt<8; cnt++ ) {         // send Address high byte
    if ( Address & 0x8000 ) {
      EEPROM_SDA_HIGH;
    } else {
        EEPROM_SDA_LOW;
    }
    Address <<= 1;
    CLOCK_CYCLE;
  }

  AckKo = AckInEEPROM();                        // receive acknowledge bit
  if ( AckKo ) {
    StopBitEEPROM();                    // send stop bit
    return 0x8000;                      // esce con 0x8000 in caso di errore
  }

  for ( cnt=0; cnt<8; cnt++ ) {         // send Address low byte
    if ( Address & 0x8000 ) {
      EEPROM_SDA_HIGH;
    } else {
        EEPROM_SDA_LOW;
    }
    Address <<= 1;
    CLOCK_CYCLE;
  }

  AckKo = AckInEEPROM();                        // receive acknowledge bit
  if ( AckKo ) {
    StopBitEEPROM();                    // send stop bit
    return 0x8000;                      // esce con 0x8000 in caso di errore
  }

  for ( ; NumByte; NumByte--  ) {
    TmpData = *DataByte;
    for ( cnt=0; cnt<8; cnt++ ) {       // send data byte
      if ( TmpData & 0x80 ) {
        EEPROM_SDA_HIGH;
      } else {
          EEPROM_SDA_LOW;
      }
      TmpData <<= 1;
      CLOCK_CYCLE;
    }

    AckKo = AckInEEPROM();                        // receive acknowledge bit
    if ( AckKo ) {
      StopBitEEPROM();                    // send stop bit
      return 0x8000;                      // esce con 0x8000 in caso di errore
    }

    DataByte ++;
  }

  StopBitEEPROM();                      // send stop bit


  return 1;                             // esce con 1 in caso di successo
}





// ******** Legge un Buffer di Dati dall'EeProm
// *** restituisce 0x8000 in caso di errore, 1 altrimenti
WORD SequentialRead_EEPROM ( WORD Address, BYTE *DataByte, UC NumByte )
{
  UC cnt,cntbyte;
  UC AckKo;
  BYTE TmpData;

  if ( NumByte > 32 ) {
    NumByte = 32;
  }

  TmpData = WRITE_CNTRL;
  Polling = WRITE_TIME;
  do {
      StartBitEEPROM();                 // send start bit
      for ( cnt=0; cnt<8; cnt++ ) {     // send write-control-word
        if ( TmpData & 0x80 ) {
          EEPROM_SDA_HIGH;
        } else {
            EEPROM_SDA_LOW;
        }
        TmpData <<= 1;
        CLOCK_CYCLE;
      }
      AckKo = AckInEEPROM();            // receive acknowledge bit
      TmpData = WRITE_CNTRL;
  } while ( AckKo && Polling );         // acknowledge polling

  if ( AckKo ) {                        // ecse con 0x8000 in caso di errore
    StopBitEEPROM();                      // send stop bit
    return 0x8000;
  }

  for ( cnt=0; cnt<8; cnt++ ) {         // send Address high byte
    if ( Address & 0x8000 ) {
      EEPROM_SDA_HIGH;
    } else {
        EEPROM_SDA_LOW;
    }
    Address <<= 1;
    CLOCK_CYCLE;
  }

  AckKo = AckInEEPROM();                // receive acknowledge bit
  if ( AckKo ) {
    StopBitEEPROM();                    // send stop bit
    return 0x8000;                      // esce con 0x8000 in caso di errore
  }

  for ( cnt=0; cnt<8; cnt++ ) {         // send Address low byte
    if ( Address & 0x8000 ) {
      EEPROM_SDA_HIGH;
    } else {
        EEPROM_SDA_LOW;
    }
    Address <<= 1;
    CLOCK_CYCLE;
  }

  AckKo = AckInEEPROM();                // receive acknowledge bit
  if ( AckKo ) {
    StopBitEEPROM();                    // send stop bit
    return 0x8000;                      // esce con 0x8000 in caso di errore
  }


  TmpData = READ_CNTRL;
  Polling = WRITE_TIME;
  do {
      StartBitEEPROM();                 // send start bit
      for ( cnt=0; cnt<8; cnt++ ) {     // send read-control-word
        if ( TmpData & 0x80 ) {
          EEPROM_SDA_HIGH;
        } else {
            EEPROM_SDA_LOW;
        }
        TmpData <<= 1;
        CLOCK_CYCLE;
      }

      AckKo = AckInEEPROM();            // receive acknowledge bit

      TmpData = READ_CNTRL;
  } while ( AckKo && Polling );         // acknowledge polling

  if ( AckKo ) {
    StopBitEEPROM();                      // send stop bit
    return 0x8000;
  }

  for ( cntbyte=0; cntbyte<NumByte; cntbyte++ ) {
    for ( cnt=0; cnt<8; cnt++ ) {       // receive data byte
      EEPROM_SDA_IN;
      EEPROM_CK_TIME_LOW;
      EEPROM_SCK_HIGH;
      EEPROM_CK_TIME_HIGH;
      TmpData <<= 1;
      if ( EEPROM_SDA )
          TmpData |= 0x01;
      EEPROM_SCK_LOW;
    }
    *DataByte = TmpData;
    DataByte ++;
    if ( cntbyte<NumByte-1 ) {
      AckOutEEPROM( YES_ACK );
    }
  }

  AckOutEEPROM( NO_ACK );

  StopBitEEPROM();                      // send stop bit

  return 1;                             // esce con 1 in caso di successo
}








// ******** Confronta il Buffer con il Contenuto dell'EeProm
// *** restituisce 0x8000 in caso di errore I2c
// *** restituisce 0x4000 in caso di errore di Dati
// *** restituisce 0x0000 in caso di Successo
WORD SequentialVerifyEEPROM ( WORD Address, BYTE *DataByte, UC NumByte )
{
  UC cnt,cntbyte;
  UC AckKo;
  BYTE TmpData;
  WORD Ret;

  if ( NumByte > 32 ) {
    NumByte = 32;
  }

  TmpData = WRITE_CNTRL;
  Polling = WRITE_TIME;
  do {
      StartBitEEPROM();                 // send start bit
      for ( cnt=0; cnt<8; cnt++ ) {     // send write-control-word
        if ( TmpData & 0x80 ) {
          EEPROM_SDA_HIGH;
        } else {
            EEPROM_SDA_LOW;
        }
        TmpData <<= 1;
        CLOCK_CYCLE;
      }
      AckKo = AckInEEPROM();            // receive acknowledge bit
      TmpData = WRITE_CNTRL;
  } while ( AckKo && Polling );         // acknowledge polling

  if ( AckKo ) {                        // ecse con 0x8000 in caso di errore
    StopBitEEPROM();                      // send stop bit
    return 0x8000;
  }

  for ( cnt=0; cnt<8; cnt++ ) {         // send Address high byte
    if ( Address & 0x8000 ) {
      EEPROM_SDA_HIGH;
    } else {
        EEPROM_SDA_LOW;
    }
    Address <<= 1;
    CLOCK_CYCLE;
  }

  AckKo = AckInEEPROM();                // receive acknowledge bit
  if ( AckKo ) {
    StopBitEEPROM();                    // send stop bit
    return 0x8000;                      // esce con 0x8000 in caso di errore
  }

  for ( cnt=0; cnt<8; cnt++ ) {         // send Address low byte
    if ( Address & 0x8000 ) {
      EEPROM_SDA_HIGH;
    } else {
        EEPROM_SDA_LOW;
    }
    Address <<= 1;
    CLOCK_CYCLE;
  }

  AckKo = AckInEEPROM();                // receive acknowledge bit
  if ( AckKo ) {
    StopBitEEPROM();                    // send stop bit
    return 0x8000;                      // esce con 0x8000 in caso di errore
  }


  TmpData = READ_CNTRL;
  Polling = WRITE_TIME;
  do {
      StartBitEEPROM();                 // send start bit
      for ( cnt=0; cnt<8; cnt++ ) {     // send read-control-word
        if ( TmpData & 0x80 ) {
          EEPROM_SDA_HIGH;
        } else {
            EEPROM_SDA_LOW;
        }
        TmpData <<= 1;
        CLOCK_CYCLE;
      }
      AckKo = AckInEEPROM();            // receive acknowledge bit
      TmpData = READ_CNTRL;
  } while ( AckKo && Polling );         // acknowledge polling

  if ( AckKo ) {
    StopBitEEPROM();                      // send stop bit
    return 0x8000;
  }

  Ret = 0x0000;                        // esce con 0x0000 in caso di successo
  for ( cntbyte=0; cntbyte<NumByte; cntbyte++ ) {

    for ( cnt=0; cnt<8; cnt++ ) {       // receive data byte
      EEPROM_SDA_IN;
      EEPROM_CK_TIME_LOW;
      EEPROM_SCK_HIGH;
      EEPROM_CK_TIME_HIGH;
      TmpData <<= 1;
      if ( EEPROM_SDA )
          TmpData |= 0x01;
      EEPROM_SCK_LOW;
    }

    // Cnfronto
    if ( *DataByte != TmpData )
      Ret = 0x4000;                    // Dati NON CORRISPONDENTI
    DataByte ++;

    if ( cntbyte<NumByte-1 ) {
      AckOutEEPROM( YES_ACK );
    }

  }

  AckOutEEPROM( NO_ACK );

  StopBitEEPROM();                      // send stop bit

  return Ret;
}




























//---------------------------------------  Funzioni ad ALTO LIVELLO

UC ProblemiWrEeProm=0;    // Flag Utilizzabile come segnale di Problemi con l'EeProm (Write)
UC ProblemiRdEeProm=0;    // Flag Utilizzabile come segnale di Problemi con l'EeProm (Read)
UC TentativiEeProm=0;     // Numero MAX di Tentativi mai Provati

// Numero di Tentativi di Scrittura prima di Abbandonare
#define       MAX_RETRY         5

// ***** Scrive un Buffer in EeProm Controllando la correttezza della Scrittura
WORD PageWriteEEPROM ( WORD Address, BYTE *DataByte, UC NumByte )
{
  WORD Ret;
  UC Cnt;


  for ( Cnt = 0 ; Cnt < MAX_RETRY ; Cnt++ ) {

    // Scrivo
    Ret = PageWrite_EEPROM ( Address, DataByte, NumByte );

    // Se la Scrittura è andata Bene
    if ( Ret == 1 ) {

      // Verifico ciò che ho scritto
      Ret = SequentialVerifyEEPROM ( Address, DataByte, NumByte );
      if ( Ret == 0x0000 )
        break;                          // Sono Riuscito a scrivere la Cosa Giusta

    }

  }

  if ( TentativiEeProm < (Cnt+1) )
    TentativiEeProm = Cnt+1;
  if ( Cnt >= MAX_RETRY ) {

    // Tentativi Terminati
    ProblemiWrEeProm = 1;                 // Problemi ( Scrittura Fallita )
    return 0x8000;

  } else {

    // Tuutto OK
    return 0x0001;

  }
}


// ***** Legge un Buffer da EeProm Controllando la corretteza della Lettura
WORD SequentialReadEEPROM ( WORD Address, BYTE *DataByte, UC NumByte )
{
  //return SequentialRead_EEPROM ( Address, DataByte, NumByte );
  WORD Ret;
  UC Cnt;


  for ( Cnt = 0 ; Cnt < MAX_RETRY ; Cnt++ ) {

    // Scrivo
    Ret = SequentialRead_EEPROM ( Address, DataByte, NumByte );

    // Se la Scrittura è andata Bene
    if ( Ret == 1 ) {

      // Verifico ciò che ho scritto
      Ret = SequentialVerifyEEPROM ( Address, DataByte, NumByte );
      if ( Ret == 0x0000 )
        break;                          // Sono Riuscito a scrivere la Cosa Giusta

    }

  }

  if ( TentativiEeProm < (Cnt+1) )
    TentativiEeProm = Cnt+1;
  if ( Cnt >= MAX_RETRY ) {

    // Tentativi Terminati
    ProblemiRdEeProm = 1;                 // Problemi ( Scrittura Fallita )
    return 0x8000;

  } else {

    // Tutto OK
    return 0x0001;

  }
}
//DOME 24/09/05


