/* *************************************************************************************************
 *                                                                                                 *
 *                           GESTIONE SONDE   TA, NTC, PTC, LIVELLI                                *
 *                                                                                                 *
 ***************************************************************************************************/

#include "defs.h"
#include "prot.h"
#include "191barpsp.h"




//---------------------------------------------------------------------------------------

//  Schema delle letture dei Current-Sense, Livelli, NTC, PTC in riferimento allo zero-crossing:
//  in corrispondenza dello zero-crossing viene letto TA1 (e TA2 se c'e');
//  segue la lettura di un Level e dei PTC1,..,PTC4
//
//
//    zc                   zc                   zc                   zc                   zc
// ___|____________________|____________________|____________________|____________________|___
//    |                    |                    |                    |                    |
//    TA1                                       TA1                                       TA1
//        L1                                        L1
//           PTC1                                      PTC1
//                PTC2                                      PTC2
//                     PTC3                                      PTC3
//                          PTC4                                      PTC4

//---------------------------------------------------------------------------------------







// Conversione del valore AD 'val' in temperature per i PTC tramite linearizzazione
int TempPTC ( WORD val, int offset, int slope, int pzt )
{
  int Temperatura;

  if ( val >= offset ) {
    Temperatura = ( (WORD)(val - offset) * 100 / slope ) + pzt;
  } else
      Temperatura = - ( (WORD)(offset - val) * 100 / slope ) + pzt ;

  if ( Temperatura > 120 )
    Temperatura = 120;
  if ( Temperatura < -120)
    Temperatura = -120;

    return Temperatura;

} // END TempPTC()










// Conversione del valore AD 'val' in temperature per i NTC tramite linearizzazione
int TempNTC ( long int val, int offset )
{
  long int Temperatura;

   if ( val < 270 ) {      // tratto parabolico, passaggio abbastanza preciso per -50, -25, 0 °C
   	Temperatura = (( -(val*val*8) + (4361*val) - 623974 )/10000)+offset; // ( ( ( val - 105 ) * 15 ) / 52 ) - 25; // ( ( val / 3 ) - 60 ); // ( -(val*val*0,0008) + (0,4361*val) - 62,3974 ); // ( ( ( val - 277 ) * 25 ) / 172 );    //
   } else
   		Temperatura = (((((val-270)*1297)/97)-297)/100)+offset;
						   // tratto rettilineo, passaggio abbastanza preciso per 0 e 10 °C
  // ( ( val / 3 ) - 60 ); // (( -(val*val*8) + (4361*val) - 623974 )/10000);
  // ( -(val*val*0,0008) + (0,4361*val) - 62,3974 );
  // ( ( ( val - 277 ) * 25 ) / 172 );    //

  return (int) Temperatura;

} // END TempNTC



// Visualizzazione dei Farhenheit

int Cel_Far ( int gradi )
{

  int Temperatura;

  if ( TabProg.HMP )
  		Temperatura = ( ( gradi * 9 ) / 5 ) + 32;
   else
        Temperatura = gradi;

    return Temperatura;

}



// Visualizzazione del Delta in Farhenheit


int Cel_Far_Delta ( int gradi )
{

  int Temperatura;

  if ( TabProg.HMP )
  		Temperatura = ( ( gradi * 9 ) / 5 );
   else
        Temperatura = gradi;

    return Temperatura;

} // END TempPTC()



//---------------------------------------------------------------------------------------





// Converte il valore AD del TA (0 -> 1023) nel valore del HOT (0 -> 127)
WORD TAtoHOT ( WORD val )
{
  return ( val / 8 );
}

// N.B.: con amp-op rail-to-rail deve essere val/8, altrimenti val/6









//---------------------------------------------------------------------------------------




// Converte un valore in ohm in un valore AD del LIVEL in relazione al partitore di tensione Rrif, Rload
WORD OHMtoLEVEL ( WORD Rliv )
{
  return ( ( (DWORD)Rliv * 10 * 1023) / ( Rref + Rliv * 10 ) );
}

















//---------------------------------------------------------------------------------------


volatile UC IN24_1;
volatile UC IN24_4;
volatile UC IN24_5;


UC ENABLE_V_OUT;          // portello macchina
UC T_E_1;
UC T_E_2;
WORD ProbeValue[12];
