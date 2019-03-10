/****************************************************************************************
 *                                                                                      *
 *              Gestione REAL-TIME-CLOCK ( Time-Keeper ) su bus I2C                     *
 *                                                                                      *
 ****************************************************************************************/







#include "string.h"
#include "defs.h"
#include "prot.h"







#define      Wait   WaitmSec            // ridefinizione per ovviare ad un'incongruenza di nomi








// Dichiarazione di funzioni
UC BcdToBin( UC Val );
UC BinToBcd( UC Val );







// *** Inizializza il REAL-TIME-CLOCK
void ExtTimerInit ( void )
{
  // Inizializzazione I2C
  I2cInit();

  // Inizializzazione Control-Register-2 del REAL-TIME-CLOCK
  I2cWriteByte( 0x64, 0xF0, 0x20 );     // imposta l'orologio a 24h

} // END ExtTimerInit











// **************** Scrittura sul REAL-TIME-CLOCK *******************************************




// *** Procedura per scrivere 1 struttura di tipo TimerData sul REAL-TIME-CLOCK
void TimerWrData ( struct TimerData *Pnt )
{
  Wait ( 20 );

  // Ptn1: per memorizzare la struttura TimerData completa

  I2cWriteByte( 0x64, 0x00, BinToBcd( Pnt->Second ) );
  Wait ( 20 );

  I2cWriteByte( 0x64, 0x10, BinToBcd( Pnt->Minute ) );
  Wait ( 20 );

  I2cWriteByte( 0x64, 0x20, BinToBcd( Pnt->Hour ) );
  Wait ( 20 );

  I2cWriteByte( 0x64, 0x30, Pnt->DayWeek );
  Wait ( 20 );

  I2cWriteByte( 0x64, 0x40, BinToBcd ( Pnt->DayMonth ) );
  Wait ( 20 );

  I2cWriteByte( 0x64, 0x50, BinToBcd ( Pnt->Month ) );
  Wait ( 20 );

  I2cWriteByte( 0x64, 0x60, BinToBcd ( Pnt->Year ) );
  Wait ( 20 );


  // Memorizza le Modifiche all'Orario effettuate Manualmente
  SettaOrario ( &Campione, Pnt );

} // END TimerWrData




// *** Procedura per scrivere 1 struttura di tipo TimerData sul REAL-TIME-CLOCK
void TimerWrAlarm_A ( struct TimerAlarm *Pnt )
{
  Wait ( 20 );

  // Ptn: per memorizzare Alarm_A

  I2cWriteByte( 0x64, 0x80, BinToBcd ( Pnt->Minute ) );
  Wait ( 20 );

  I2cWriteByte( 0x64, 0x90, BinToBcd ( Pnt->Hour ) );
  Wait ( 20 );

  I2cWriteByte( 0x64, 0xA0, ( Pnt->DayWeek ) );
  Wait ( 20 );

} // END TimerWrAlarm_A










// **************** Lettura sul REAL-TIME-CLOCK *******************************************




// *** Procedura per leggere 1 struttura di tipo TimerData dal REAL-TIME-CLOCK
void TimerRdData ( struct TimerData *Pnt )
{
  UC Ret;

  // Ptn1: per leggere la struttura TimerData completa

  Ret = I2cReadByte ( 0x64, 0x00 );
  Pnt->Second = BcdToBin ( Ret );

  Ret = I2cReadByte ( 0x64, 0x10 );
  Pnt->Minute = BcdToBin ( Ret );

  Ret = I2cReadByte ( 0x64, 0x20 );
  Pnt->Hour = BcdToBin ( Ret );

  Ret = I2cReadByte ( 0x64, 0x30 );
  Pnt->DayWeek = Ret;

  Ret = I2cReadByte ( 0x64, 0x40 );
  Pnt->DayMonth = BcdToBin ( Ret );

  Ret = I2cReadByte ( 0x64, 0x50 );
  Pnt->Month = BcdToBin ( Ret );

  Ret = I2cReadByte ( 0x64, 0x60 );
  Pnt->Year = BcdToBin ( Ret );

} // END TimerRdData1






// *** Procedura per leggere 1 struttura di tipo TimerData dal REAL-TIME-CLOCK
void TimerRdAlarm_A ( struct TimerAlarm *Pnt )
{
  UC Ret;

  // Ptn: per leggere Alarm_A

  Ret = I2cReadByte ( 0x64, 0x80 );
  Pnt->Minute = BcdToBin ( Ret );

  Ret = I2cReadByte ( 0x64, 0x90 );
  Pnt->Hour = BcdToBin ( Ret );

  Ret = I2cReadByte ( 0x64, 0xA0 );
  Pnt->DayWeek = ( Ret );

} // END TimerRdAlarm_A
















// ******************** Conversione / controllo del formato dei dati ********************************





// *** Converte da Bcd a Binario
UC BcdToBin ( UC Val )
{
  UC Ret;

  Ret = Val >> 4;
  return Ret * 10 + (Val & 0x0F);

} // END BcdToBin





// *** Converte da Binario a Bcd
UC BinToBcd ( UC Val )
{
  UC Ret;

  Ret = Val / 10;
  Val -= Ret * 10;
  Ret = (Ret << 4) + Val;

  return Ret;
} // END BinToBcd







const WORD LenMesi[12] = {
  31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};


// *** Calcola la Durata del Mese in Giorni
UC DurataMese ( UC Mese, WORD Anno )
{
  UC Val;


  if ( Mese > 12 ) Mese = 1;
  if ( !Mese ) Mese = 1;
  Val = LenMesi[Mese-1];

  if ( Mese == 2 ) {

    // Febbraio
    if ( !(Anno & 0x03) ) {
      Val = 29;
      if ( !(Anno % 100) ) {
        Val = 28;
        if ( !(Anno % 400) )
          Val = 29;
      }
    }

  }

  return Val;
}




// Calcola il numero di minuti trascorsi
long GetMinuti ( struct TimerData *Pnt )
{
  long Tot;

  Tot = Pnt->Year;
  Tot = Tot * 12 + Pnt->Month;
  Tot = Tot * 31 + Pnt->DayMonth;
  Tot = Tot * 24 + Pnt->Hour;
  Tot = Tot * 60 + Pnt->Minute;

  return Tot;

} // END GetMinuti








//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//                    Funzioni Aggiuntive di Controllo Orario
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------







#define     MASK_MIN      0x01
#define     MASK_ORA      0x02
#define     MASK_DAY      0x04
#define     MASK_MESE     0x08
#define     MASK_ANNO     0x10
#define     MASK_ERRDATA  0x80


// Struttura utilizzata per il controllo dell'Orario
struct TimerData Campione;

// Indica quando viene sistemato l'orologio per un problema
UC ProblemiOrologio;

// Maschera dell'ultimo problema
UC UltimoProblema;

// Indica se abilitare il controllo sull'ora
UC AbilitaControlloOrologio = 1;



// *** Procedura per scrivere 1 struttura di tipo TimerData sul REAL-TIME-CLOCK
//       Utilizza una Maschera per Decidere che cosa salvare
void TimerRefresh ( struct TimerData *Pnt, UC Mask )
{
  /* DEBUG (da Cancellare)
  SetBufLcd32( "Ora SCRITTA     ................" );
  BinToAscii ( BufLcd+14, Mask, 2 );

  BinToAscii ( BufLcd+16, Pnt->DayMonth, 2 );
  BinToAscii ( BufLcd+18, Pnt->Month, 2 );
  BinToAscii ( BufLcd+20, Pnt->Year, 2 );

  BinToAscii ( BufLcd+24, Pnt->Hour, 2 );
  BinToAscii ( BufLcd+27, Pnt->Minute, 2 );
  BinToAscii ( BufLcd+30, Pnt->Second, 2 );
  Wait100mSec ( 30 );
  */

  if ( Mask & MASK_MIN ) {
    I2cWriteByte ( 0x64, 0x10, BinToBcd ( Pnt->Minute ) );
    Wait ( 20 );
  }

  if ( Mask & MASK_ORA ) {
    I2cWriteByte ( 0x64, 0x20, BinToBcd ( Pnt->Hour ) );
    Wait ( 20 );
  }

  if ( Mask & MASK_DAY ) {
    I2cWriteByte ( 0x64, 0x40, BcdToBin ( Pnt->DayMonth ) );
    Wait ( 20 );
  }

  if ( Mask & MASK_MESE ) {
    I2cWriteByte ( 0x64, 0x50, BcdToBin ( Pnt->Month ) );
    Wait ( 20 );
  }

  if ( Mask & MASK_ANNO ) {
    I2cWriteByte ( 0x64, 0x60, BcdToBin ( Pnt->Year ) );
    Wait ( 20 );
  }

} // END TimerWrData


struct TimerData Copia;


// *** Controlla se i due orari sono compatibili ( differenza permessa <= 1 Minuto )
//       Deve essere chiamato almeno 2 volte al minuto ( 1 Sec )
UC ControlloOrario ( struct TimerData *Ora )
{
  UC Mask, Giusto;


  if ( !AbilitaControlloOrologio )
    return 1;


  Giusto = 1;

  // Controllo Corettezza Dati
  CopiaOrario ( &Copia, Ora );
  Mask = ControllaOrario ( &Copia );
  if ( Mask ) {
    Giusto = 0;                         // Orario NON Valido

  /* DEBUG (da Cancellare)
  SetBufLcd32( "Ora REALE       ................" );
  BinToAscii ( BufLcd+16, Ora->DayMonth, 2 );
  BinToAscii ( BufLcd+18, Ora->Month, 2 );
  BinToAscii ( BufLcd+20, Ora->Year, 2 );

  BinToAscii ( BufLcd+24, Ora->Hour, 2 );
  BinToAscii ( BufLcd+27, Ora->Minute, 2 );
  BinToAscii ( BufLcd+30, Ora->Second, 2 );

  BinToAscii ( BufLcd+14, DurataMese ( Ora->Month, Ora->Year ), 2 );
  Wait100mSec ( 30 );


  SetBufLcd32( "Ora CORRETTA    ................" );
  BinToAscii ( BufLcd+16, Copia.DayMonth, 2 );
  BinToAscii ( BufLcd+18, Copia.Month, 2 );
  BinToAscii ( BufLcd+20, Copia.Year, 2 );

  BinToAscii ( BufLcd+24, Copia.Hour, 2 );
  BinToAscii ( BufLcd+27, Copia.Minute, 2 );
  BinToAscii ( BufLcd+30, Copia.Second, 2 );
  Wait100mSec ( 30 );
  */

  }

  // Controllo Passaggio del Tempo ( < 1 Min )
  if ( Giusto ) {

    if ( !ControllaDifferenze ( Ora, &Campione ) ) {
      // Tempo diverso
      CopiaOrario ( &Copia, &Campione );
      //   Incrementa di 1 Minuto
      Incrementa1Minuto ( &Copia );
      // Ripete il Controllo
      if ( !ControllaDifferenze ( Ora, &Copia ) )
        Giusto = 0;
    }

  }

  if ( !Giusto ) {

    // Orario Errato
    ProblemiOrologio++;

    // Controlla le Differenze
    Mask |= ControllaDifferenze ( Ora, &Campione );
    UltimoProblema = Mask;

    // Sistema l'Ora
    CopiaOrario ( Ora, &Campione );

    // Le mette nel timeKeeper
    TimerRefresh ( Ora, Mask );

    return 0;

  } else {

    // Orario Corretto
    //  Rinfresca il Campione
    CopiaOrario ( &Campione, Ora );

    return 1;
  }
}


// *** Restituisce una maschera che indica le differenze di orario
UC ControllaDifferenze ( struct TimerData *Ora, struct TimerData *Campione )
{
  UC Mask;


  Mask = 0x00;
  if ( Ora->Minute != Campione->Minute )
    Mask |= MASK_MIN;
  if ( Ora->Hour != Campione->Hour )
    Mask |= MASK_ORA;
  if ( Ora->DayMonth != Campione->DayMonth )
    Mask |= MASK_DAY;
  if ( Ora->Month != Campione->Month )
    Mask |= MASK_MESE;
  if ( Ora->Year != Campione->Year )
    Mask |= MASK_ANNO;

  return Mask;
}



// *** Controlla che l'Orario sia Corretto
//       Restituisce una maschera che indica i casi di Orario Errato
UC ControllaOrario ( struct TimerData *Ora )
{
  UC Mask;
  UC Giorni;


  Mask = 0x00;
  Giorni = DurataMese ( Ora->Month, Ora->Year );
  if ( Ora->Minute >= 60 ) {
    Mask |= MASK_MIN;
    Ora->Minute = 0;
  }
  if ( Ora->Hour >= 24 ) {
    Mask |= MASK_ORA;
    Ora->Hour = 0;
  }
  if ( Ora->DayMonth > Giorni ) {
    Mask |= MASK_DAY;
    Ora->DayMonth = 1;
  }
  if ( Ora->Month > 12 ) {
    Mask |= MASK_MESE;
    Ora->Month = 1;
  }

  if ( Mask )
    Mask |= MASK_ERRDATA;

  return Mask;
}






// Incrementa la Data/Ora di 1 Minuto
void Incrementa1Minuto ( struct TimerData *Ora )
{
  UC Giorni;


  Giorni = DurataMese ( Ora->Month, Ora->Year );

  Ora->Minute++;
  if ( Ora->Minute >= 60 ) {
    Ora->Minute = 0;

    Ora->Hour++;
    if ( Ora->Hour >= 24 ) {
      Ora->Hour = 0;

      Ora->DayMonth++;
      if ( Ora->DayMonth >= Giorni ) {
        Ora->DayMonth = 0;

        Ora->Month++;
        if ( Ora->Month >= 12 ) {
          Ora->Month = 0;

          Ora->Year++;
        }
      }
    }
  }
}




// *** Copia un orario su di un altro
void CopiaOrario ( struct TimerData *PntTo, struct TimerData *PntFrom )
{
  memcpy ( PntTo, PntFrom, sizeof ( struct TimerData ) );
}


// *** Copia un orario su di un altro
//       Controlla che i Dati copiati abbiano un Senso
void SettaOrario ( struct TimerData *PntTo, struct TimerData *PntFrom )
{
  // Controllo Dati
  ControllaOrario ( PntFrom );

  // Copia
  memcpy ( PntTo, PntFrom, sizeof ( struct TimerData ) );
}

