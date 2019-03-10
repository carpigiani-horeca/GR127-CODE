//-------------------------------------------------------------
//-------------------------------------------------------------
//
//
// Gestione Ingressi / Uscite
//
//
//-------------------------------------------------------------
//-------------------------------------------------------------
#include "Sfr_r825.h"
#include "defsLow.h"
#include "protLow.h"
#include "Interrupt.h"











//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------            Ingressi



// Stato ingressi RealTime
extern UCHAR Input;


// Contatori per le Medie
UCHAR CntInput[5];
UCHAR OldInput[5];




// Stato mediato degli Ingressi
struct BitStruct ExtInput;





//-------------------------------------------
//      Calcola la Media di un ingresso
void MediaInput ( UCHAR Chan, UCHAR State )
{
  if ( OldInput[Chan] == State ) {
    if ( CntInput[Chan] )
      CntInput[Chan]--;
    else
      if ( State )
        ToByte(ExtInput) |= (1 << Chan);
      else
        ToByte(ExtInput) &= ~(1 << Chan);
  } else {
    CntInput[Chan] = 10-1;                  // Media su 10 Valori
    OldInput[Chan] = State;
  }
}



//--------------------------------------
//      Controlla gli Ingressi Facendone una Media
//        chiamata ogni 10mSec
void CheckIngressi ( void )
{
  MediaInput ( CH_INP_I1, !(Input & 0x01) );
  MediaInput ( CH_INP_I2, !(Input & 0x02) );
  MediaInput ( CH_INP_I3, !(Input & 0x04) );

  MediaInput ( CH_INP_BK1, !p0_4 );
  MediaInput ( CH_INP_BK2, !p0_3 );
}














//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------            Uscite


//--- Struttura per le Uscite
struct BitStruct OutPut;




//-----------------------------
//      Controllo delle Uscite
void CheckOut ( void )
{
  if ( OUT_K1 ) K1OUT = 1; else K1OUT = 0;
  if ( OUT_K2 ) K2OUT = 1; else K2OUT = 0;
  if ( OUT_K3 ) K3OUT = 1; else K3OUT = 0;
  if ( OUT_K4 ) K4OUT = 1; else K4OUT = 0;

  if ( OUT_TK1 ) TK1OUT = 0; else TK1OUT = 1;
  if ( OUT_TK2 ) TK2OUT = 0; else TK2OUT = 1;
}














//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------            Uscita a PWM ( Analogica 0%10Volt )


#ifdef      INCLUDE_PWM

void SetPwmOut ( UCHAR Perc )
{
  if ( Perc == 0 ) {
    // Fermo a 0
    tstart_trbcr = 0;
    topl_trbioc = 1;            // Alto quando Fermo
  } else
    if ( Perc >= 100 ) {
      // Fermo a 1
      tstart_trbcr = 0;
      topl_trbioc = 0;          // Basso quando Fermo
    } else {
      // Attivo al ##%
      tstart_trbcr = 0;
      topl_trbioc = 0;
      trbsc = Perc-1;               // On = ##/100
      trbpr = (100-Perc)-1;         // Off = (100-##)/100
      tstart_trbcr = 1;
    }
}

#endif
