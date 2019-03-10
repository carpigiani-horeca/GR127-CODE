#include "string.h"
#include <stdio.h>
#include "defs.h"
#include "prot.h"







// *** Calcola e aggiorna il CheckSum della struttura passata come parametro
//      DimStr =  sizeof( struct xxxxx )
UC CheckSum ( UC *PntStr, UC DimStr )
{
  WORD Chk;
  WORD *PntChk;

  PntChk = (WORD *)PntStr;                // PntChk punta al campo CheckSum della Struttura
  PntStr += 2;                            // nel calcolo del CheckSum non va incluso il CheckSum stesso
  DimStr = DimStr - 2;                    // nel calcolo del CheckSum non va incluso il CheckSum stesso

  Chk = 0;
  for ( ; DimStr ; DimStr -- )
    Chk += *PntStr++;                     // calcola il check-sum

  if ( Chk != *PntChk ) {
    *PntChk = Chk;                        // se il chechsum e' non-ok lo sovrascrive
      return 0;                           // chechsum "NON-OK" se lettura, non importa se scrittura
  } else
      return 1;                           // checksum "OK" se lettura, non importa se scrittura

} // END CheckSum()


//  n.b.: il checksum deve essere il primo campo della struttura e deve essere una WORD


