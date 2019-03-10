//-------------------------------------------------------------
//-------------------------------------------------------------
//
//
// Gestione Potenziometro Digitale
//
//
//-------------------------------------------------------------
//-------------------------------------------------------------
#include "Sfr_r825.h"
#include "defsLow.h"
#include "protLow.h"
#include "Interrupt.h"








// Procedura di scrittura su Digital-Potenziometer-0
void WriteDigitalPot0 ( UCHAR data )
{
  UCHAR i;
  UCHAR Command;


  data = 255 - data;                    // conversione perche' sono scambiati A e B del Digital-Pot
  Command = 0x11;                       // Command-Byte per la scrittura sul pot 0

  // Inizio scrittura
  SCK = 1;                              // prima SCK = 1
  nCS = 0;                              // poi !CS = 0

  // Scrittura Comando
  for ( i = 0 ; i < 8 ; i++ )
  {
    SCK = 0;                            // fronte negativo del clock
    if ( Command & 0x80 )               // command byte -> Write-Data-Pot
      SI = 1;
    else
      SI = 0;
    SCK = 1;                            // fronte positivo del clock => lettura dato
    Command <<= 1;
  }
  // Scrittura Dato
  for ( i = 0 ; i < 8 ; i++ )
  {
    SCK = 0;                            // fronte negativo del clock
    if ( data & 0x80 )                  // data byte
      SI = 1;
    else
      SI = 0;
    SCK = 1;                            // fronte positivo del clock => lettura dato
    data <<= 1;
  }

  // Fine Scrittura
  SCK = 0;                              // prima SCK = 0
  nCS = 1;                              // poi !CS = 1
}


