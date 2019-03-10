/***************************************************************************************
 *                                                                                     *
 *     Gestione FLASH/EEPROM-virtuale su microcontrollore => BLOCCO_A , BLOCCO_1       *
 *                                                                                     *
 ***************************************************************************************/

#include "string.h"
#include "stdio.h"
#include "defs.h"
#include "prot.h"
#include "191barpsp.h"









// *** Prototipo di procedura che Copia una Funzione in Ram
//    ( la procedura é nel file BootFlash() )
void CopyFnt ( char far *To , char far *From , WORD Len )
{
  while ( Len-- )
    *To++ = *From++;
}









/*

// ****************** Gestione Scittura/Lettura EEPROM-VIRTUALE su microcontrollore - BLOCCO 1  *******************

#define   FLASH_BLOCK1_ADDR   0x0FE000



// *** Funzione per la scrittura della struct FlashData nel BLOCCO-1
//    ( operazione da eseguire con la funzione in RAM )
//______________________________________________________________________________
void ProgBlock1 ( void )
{

  WORD *Pnt;
  UC StrDim = ( sizeof(FlashData) + 1 ) / 2;    // ( n.ro byte + 1 ) / 2 =  n.ro word

  asm ("\tFCLR I");

  // Abilitazione CPU-ReWrite-Mode
  FMR01 = 0;
  asm ( "nop" );
  FMR01 = 1;
  asm ( "nop" );

  // Disabilitazione Lock-Bit
  FMR02 = 0;
  asm ( "nop" );
  FMR02 = 1;
  asm ( "nop" );

  // Abilitazione EW1-Mode
  asm ( "nop" );
  FMR11 = 0;
  asm ( "nop" );
  FMR11 = 1;

  //--- Software Commands START

  // Indirizzo iniziale del Blocco-1
  Addr = FLASH_BLOCK1_ADDR;

  // Scrittura 2 Bytes
  Pnt = (WORD *)&FlashData;                   // casting da BYTE a WORD per scrivere 2 BYTE alla volta
  for ( ; StrDim ; StrDim -- ) {

    // Clear-Status-Register Command
    *((WORD far *)Addr) = 0x50;

    // Program Command ( scrittura di una WORD alla volta )
    *((WORD far *)Addr) = 0x40;

    // Aggiornamento del puntatore alla memoria
    *((WORD far *)Addr) = *Pnt++;
    Addr += 2;

    // Controllo Busy/Ready-Status-Flag
    while ( !FMR00 ) ;

    // Controllo Program-Status-Flag
    Error = 0;
    if ( FMR06 )
      Error = 1;

  }

  //--- Software Commands STOP

  // Disabilitazione CPU-ReWrite-Mode ( riabilitazione ReadArray-Mode )
  asm ( "nop" );
  FMR01 = 0;
  asm ( "nop" );

}
void EndProgBlock1 ( void )
{
}





// *** Calcola e aggiorna il CheckSum della struttura FlashData
UC DoCheckBlock1 ( UC *PntData )
{
  WORD Chk;
  WORD *Pnt;
  UC StrDim = sizeof( struct StrFlashData ) - 2;  // nel calcolo del CheckSum non va incluso il CheckSum stesso

  Pnt = (WORD *)PntData;
  PntData += 2;                                   // nel calcolo del CheckSum non va incluso il CheckSum stesso

  Chk = 0;
  for ( ; StrDim ; StrDim-- )
    Chk += *PntData++;

  if ( Chk != *Pnt ) {
    *Pnt = Chk;                           // se il chechsum e' no-ok lo sovrascrive
      return 0;                           // chechsum no-ok se lettura, non importa se scrittura
  } else
      return 1;                           // checksum ok se lettura, non importa se scrittura

}









// *** Cancellazione del Blocco-1 dalla RAM ***
//     la funzione viene copiata in RAM e ivi eseguita
//_____________________________________________________

void EraseFlashBlock1 ( void )
{
  asm ("\tFCLR I");                    // disable interrupt

  CopyFnt( (UC *)FntBuffer, (UC far *)DelFlash, (WORD)( (UC far *)EndDelFlash - (UC far *)DelFlash ) );
  Addr = FLASH_BLOCK1_ADDR;                    // indirizzo iniziale del blocco da cancellare
  ((void (*) ( void ))FntBuffer)();

  asm ("\tFSET I");                    // enable interrupt
}




// *** Programmazione del Blocco-1 dalla RAM ***
//     la funzione viene copiata in RAM e ivi eseguita
//____________________________________________________

void WriteFlashBlock1 ( void )
{
   asm ("\tFCLR I");                    // disable interrupt

   DoCheckBlock1( (UC*)&FlashData );             // calcola il checksum di FlashData e lo memorizza in FlashData
   CopyFnt( (UC *)FntBuffer, (UC far *)ProgBlock1, (WORD)( (UC far *)EndProgBlock1 - (UC far *)ProgBlock1 ) );
   ((void (*) ( void ))FntBuffer)();

   asm ("\tFSET I");                    // enable interrupt
}




// *** Procedura di lettura da Blocco-1 della struttura StrFlashData ***
//______________________________________________________________________
UC ReadFlashBlock1 ( void )
{
   CopyFnt( (UC *)&FlashData, (char far *)FLASH_BLOCK1_ADDR, sizeof( struct StrFlashData ) );
   return DoCheckBlock1( (UC *)&FlashData );      // calcola il checksum di PntData e lo memorizza in PntData
}


*/













// ****************** Gestione Scittura/Lettura EEPROM-VIRTUALE su microcontrollore - BLOCCO A  *******************


#define         EE_BLOCK_ADDR1       0x2400
#define         EE_BLOCK_ADDR2       0x2400








// *** Cancellazione BLOCK-A (F000-FFFF) -> EeProm Virtuale
//       Ritorno = Errore in Cancellazione
UC DelEePromBlock ( void )
{
    unsigned char erase_result;

	asm("FCLR I");											/* If use interrupt then clear i flag */

    /* FLASH control register set */
    fmr0 = 0x01;											/* Set to ready , CPU rewrite disable */
    asm(" ");       										/* Description for preventing the abbreviation by optimization */
    fmr0 = 0x03;											/* CPU rewrite enable */
    fmr1 = 0x80;											/* EW0 mode , BLOCK0 rewrite enable , BLOCK1 rewrite enable */
    asm(" ");       										/* Description for preventing the abbreviation by optimization */
    fmr1 = 0x82;                        					/* EW1 mode , BLOCK0 rewrite enable , BLOCK1 rewrite enable */

	/* BLOCK erase */
    *((unsigned char *)0x2400) = 0x20;                                       /* BLOCK erase command write */
    *((unsigned char *)0x2400) = 0xd0;                                       /* BLOCK erase command write */
    while( fmr00 != 1 );                					/* Ready check */
    /* Erase status check */
    if( fmr07 == 1 ){										/* Erase error? */
        *((unsigned char *)0x2400) = 0x50;                                   /* Clear stutus register */
        erase_result = 1;                           /* Erase result buffering */
    }
	else{
        erase_result = 0;                            /* Erase result buffering */
    }
    /* FLASH control register reset */
    fmr0 = 0x01;                    						/* Set to ready , CPU rewrite disable */

	asm("FSET I");											/* If use interrupt then set i flag */
    return  erase_result;									/* Return : erase result */

/*
  UC Error;


  // Interrupt Disabilitati Obbligatoriamente ( RIABILITATI DAL CHIAMANTE )
  asm ("\tFCLR I");


  // Passo a ReWrite Mode
  fmr01 = 0;
  asm ( "nop" );
  fmr01 = 1;
  asm ( "nop" );

  // Disabilito il Lock
  fmr02 = 0;
  asm ( "nop" );
  fmr02 = 1;
  asm ( "nop" );

  // Mode : EW1
  asm ( "nop" );
  fmr11 = 0;
  asm ( "nop" );
  fmr11 = 1;

  //--- Software Commands  START

  // Comando di cancellazione StatusRegister
  *((WORD far *)EE_BLOCK_ADDR2) = 0x50;

  // Comando di cancellazione
  *((WORD far *)EE_BLOCK_ADDR2) = 0x20;
  *((WORD far *)EE_BLOCK_ADDR2) = 0xD0;

  // Controllo Status
  while ( !fmr00 ) ;

  // Controllo Erase Status Flag
  Error = 0;
  if ( fmr07 )
    Error = 1;   // errore

  //--- Software Commands  END

  // Disabilito ReWrite Mode ( Torno a ReadArrayMode )
  asm ( "nop" );
  fmr01 = 0;
  asm ( "nop" );

  return Error;
*/
} // END DelEePromBlock()










// *** Scrittura su BLOCK-A -> EeProm Virtuale
//     Addr = OffSet nel BLOCCO-A
UC ProgEePromBlock ( WORD Addr, UC *PntData, WORD Len )
{
    unsigned char program_result = 0;
    unsigned char i;

	asm("FCLR I");											/* If use interrupt then clear i flag */

    /* FLASH control register set */
    fmr0 = 0x01;											/* Set to ready , CPU rewrite disable */
    asm(" ");       										/* Description for preventing the abbreviation by optimization */
    fmr0 = 0x03;                        					/* CPU rewrite enable */
    fmr1 = 0x80;											/* EW0 mode , BLOCK0 rewrite enable , BLOCK1 rewrite enable */
    asm(" ");       										/* Description for preventing the abbreviation by optimization */
    fmr1 = 0x82;                        					/* EW1 mode , BLOCK0 rewrite enable , BLOCK1 rewrite enable */

    Addr += 0x2400;
    for( i = 0; i < Len; i++ ){
        *((unsigned char *)(Addr+i)) = 0x40;                     /* Program command write */
        *((unsigned char *)(Addr+i)) = PntData[i];        /* Data write */
        while( fmr00 != 1 );            					/* Ready check */
        /* Program status check */
        if( fmr06 == 1 ){									/* Program error? */
            /* Program error */
            *((unsigned char *)0x2400) = 0x50;                   /* Clear stutus register */
            program_result = 1;                   /* Program result buffering */
            break;
        }
    }
    /* FLASH control register reset */
    fmr0 = 0x01;                        					/* Set to ready , CPU rewrite disable */

	asm("FSET I");											/* If use interrupt then set i flag */
    return program_result;									/* Return : program result */

/*
  WORD *Pnt;
  UC Error;


  // Interrupt Disabilitati Obbligatoriamente ( RIABILITATI DAL CHIAMANTE )
  asm ("\tFCLR I");


  // Passo a ReWrite Mode
  fmr01 = 0;
  asm ( "nop" );
  fmr01 = 1;
  asm ( "nop" );

  // Disabilito il Lock
  fmr02 = 0;
  asm ( "nop" );
  fmr02 = 1;
  asm ( "nop" );

  // Mode : EW1
  asm ( "nop" );
  fmr11 = 0;
  asm ( "nop" );
  fmr11 = 1;

  //--- Software Commands  START

  // Sono necessari solo indirizzi pari => mascheratura
  Addr &= 0xFFFE;                       // maschera x avere indirizzi pari
  Addr += EE_BLOCK_ADDR1;               // sommo all'offset l'indirizzo di inzio blocco-A
  Pnt = (WORD*)PntData;
  Len >>= 1;                            // Numero di WORD

  Error = 0;
  for ( ; Len && !Error ; Len-- ) {

    // Comando di cancellazione StatusRegister
    *((WORD far *)EE_BLOCK_ADDR1) = 0x50;

    // Comando di Inizio Programmazione
    *((WORD far *)Addr) = 0x40;

    // Dato da Scrivere (16bit)
    *((WORD far *)Addr) = *Pnt++;
    Addr += 2;

    // Controllo Status
    while ( !fmr00 ) ;

    // Controllo Cancellazione
    if ( fmr06 )
      Error = 1;

  }

  //--- Software Commands  END

  // Disabilito ReWrite Mode ( Torno a ReadArrayMode )
  asm ( "nop" );
  fmr01 = 0;
  asm ( "nop" );

  return Error;
*/
} // END ProgEePromBlock()










// *** Calcola e aggiorna il CheckSum della struttura TabProg
UC DoCheck ( UC *PntData )
{
  WORD Chk;
  WORD *Pnt;
  UC StrDim = sizeof( struct StrTabProg ) - 2;  // nel calcolo del CheckSum non va incluso il CheckSum stesso

  Pnt = (WORD *)PntData;
  PntData += 2;

  Chk = 0;
  for ( ; StrDim ; StrDim-- )
    Chk += *PntData++;

  if ( Chk != *Pnt ) {
    *Pnt = Chk;                           // se il chechsum e' no-ok lo sovrascrive
      return 0;                           // chechsum no-ok se lettura, non importa se scrittura
  } else
      return 1;                           // checksum ok se lettura, non importa se scrittura

} // END DoCheck()










// *** Procedura di scrittura in EEPROM:
//     ritorna 0 -> ok , ritorna 1 -> no ok
//_________________________________________
UC WriteEeprom ( struct StrTabProg *PntData )
{
   UC Ret;


   DoCheck( (UC*)PntData );             // calcola il checksum di PntData e lo memorizza in PntData

   asm ("\tFCLR I");                    // disable interrupt

   if ( DelEePromBlock() ) {            // cancella la EEPROM
    asm ("\tFSET I");                   // enable interrupt
    return 0;                           // esce in errore
   }

   Ret = ProgEePromBlock ( 0x00, (UC*)PntData, sizeof( struct StrTabProg ) + 1 );  // copia PntData in EEPROM

   asm ("\tFSET I");                    // enable interrupt

   return !Ret;                         // esce ok/ko

} // END WriteEeprom()










// *** Procedura di lettura da EEPROM:
//     ritorna 0 -> chechsum no-ok , ritorna 1 -> checksum ok
//___________________________________________________________
UC ReadEeprom ( struct StrTabProg *PntData )
{
   CopyFnt( (UC*)PntData, (char far *)EE_BLOCK_ADDR1, sizeof( struct StrTabProg ) );

   return DoCheck( (UC*)PntData );      // calcola il checksum di PntData e lo memorizza in PntData

} // END ReadEeprom()


