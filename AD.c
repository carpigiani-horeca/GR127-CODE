//-------------------------------------------------------------
//-------------------------------------------------------------
//
//
// Gestione Conversioni A/D
//
//
//-------------------------------------------------------------
//-------------------------------------------------------------
#include "Sfr_r825.h"
#include "defsLow.h"
#include "protLow.h"
#include "Interrupt.h"






//-------------------------------------
//      Imposta il Canale A/D da Convertire
void SetADCh ( UCHAR Channel )
{
  adcon0 &= ~0x07;
  adcon0 |= Channel & 0x07;
}








UINT ValAD[8];
UCHAR OldChannel;


//----------------------------------------
//      Legge un Valore A/D ed inizializza la Prossima Conversione
UINT ReadAD ( UCHAR NewChannel )
{
  UINT Ret;


  if ( StartAD == 0 ) {                                     // A/D conversion stop?

    if ( ir_adic == 1 ) {                                   // A/D interrupt request?

      // Legge il Valore Precedente
      Ret = ad & 0x3ff;                                     // Read the A/D conversion data(10bits)
      ValAD[OldChannel] = Ret;                              // Memorizza il Valore

      ir_adic = 0;                                          // A/D interrput request flag clear

    }

    // Prepara la Prossima Conversione
    OldChannel = NewChannel;
    SetADCh ( NewChannel );

    // Inizia una Nuova Conversione
    StartAD = 1;                                            // A/D convarsion start

  }

  return Ret;
}






// Numero di Medie di Primo livello per ogni Canale
const UCHAR NUMSAMPLE_AD[4] = { 16, 16, 4, 8 };

// Media di Primo Livello
UINT SumAdValue[4];
UCHAR SumAdCnt[4];

// Media di Secondo Livello
UINT AllAdValue[4][AD_AVERAGE];
UCHAR AllAdCnt[4];

//-------------------------------------
//      Procedura di creazione delle medie
void AD_MakeAverage ( UCHAR Channel, UINT Val )
{
  SumAdValue[Channel] += Val;                                       // Accumulo di un nuovo valore AD

  SumAdCnt[Channel]++;
  if ( SumAdCnt[Channel] >= NUMSAMPLE_AD[Channel] ) {               // Accumulati NumSample campioni...

    SumAdCnt[Channel] = 0;                                          // Reset di SampleIndex[]
    SumAdValue[Channel] /= NUMSAMPLE_AD[Channel];

    AllAdValue[Channel][AllAdCnt[Channel]] = SumAdValue[Channel];   // Riempimento dell' array di "medie di primo livello"
                                                                    //   per la "media di secondo livello"
    SumAdValue[Channel] = 0;

    AllAdCnt[Channel]++;
    if ( AllAdCnt[Channel] >= AD_AVERAGE )
      AllAdCnt[Channel] = 0;                                        // Reset di AverageIndex[]

    AD_FinalAverage ( Channel );                                    // Realizza le "medie di secondo livello" con isteresi

  }
}





// Valore finale Mediato
UINT AdValue[4];

UINT AdOld[4];

//----------------------------------------
//      Procedura che realizza le "medie di secondo livello" con isteresi sui TA
void AD_FinalAverage ( UCHAR Channel )
{
  UCHAR Index;
  UINT New = 0;


  // Realizzazione delle medie di secondo livello su un canale
  for ( Index = 0 ; Index < AD_AVERAGE ; Index++ )
    New += AllAdValue[Channel][Index];
  New /= AD_AVERAGE;

  // Realizzazione dell'isteresi +/-1 su un canale
  if ( New == (AdValue[Channel] + 1) ) {                        // le medie sono crescenti
    if ( AdValue[Channel] != (AdOld[Channel] - 1) ) {           // le medie sono monotone
      AdOld[Channel] = AdValue[Channel];
      AdValue[Channel] = New;
    }
  } else
    if ( New == (AdValue[Channel] - 1) ) {                      // le medie sono decrescenti
      if ( AdValue[Channel] != (AdOld[Channel] + 1) ) {         // le medie sono monotone
        AdOld[Channel] = AdValue[Channel];
        AdValue[Channel] = New;
      }
    } else {                                                    // maggiore di +/-1
      AdOld[Channel] = AdValue[Channel];
      AdValue[Channel] = New;
    }
}




UCHAR ChannelCount = 0xFF;


//------------------------------
//  Controlla la Conversione dei Canali A/D
void AdGest ( void )
{
  switch ( ChannelCount ) {

    case 0 :        // TA
      ReadAD ( AD_PIN_TA );
      break;

    case 1 :        // ---
      break;

    case 2 :        // PTC
      ReadAD ( AD_PIN_PTC );
      AD_MakeAverage ( AD_CH_TA, ValAD[AD_PIN_TA] );
      break;

    case 3 :        // NTC
      ReadAD ( AD_PIN_NTC );
      AD_MakeAverage ( AD_CH_PTC, ValAD[AD_PIN_PTC] );
      break;

    case 4 :        // 0 % 10
      ReadAD ( AD_PIN_010 );
      AD_MakeAverage ( AD_CH_NTC, ValAD[AD_PIN_NTC] );
      break;

    case 5 :
      ReadAD ( 0x00 );
      AD_MakeAverage ( AD_CH_010, ValAD[AD_PIN_010] );
      break;

  }
}
