/*********************************************************************************
 *                                                                               *
 *                              Gestione bus I²C                                 *
 *                                                                               *
 *********************************************************************************/




#include "defs.h"
#include "prot.h"



UC I2c_ReadByte ( UC FAddr, UC Addr );





#define NoOp  no_operation()

#define		I2C_WRITE	0
#define		I2C_READ	1





volatile UC I2cStatus;






/*
 Utilizzo :

   I2cInit();

 - Write data => Adr(00) = 0x55

   I2c_Start ( 0x00, I2C_WRITE );
   I2c_Write ( 0x55 );
   I2c_Stop();

 - Read data => Ret = Adr(00)

   I2c_Start ( 0x00, I2C_WRITE );
   I2c_RepStart ( 0x00, I2C_READ );
   Ret = I2c_Read();
   I2c_Stop();
*/





/*-----------------15/01/02 09.09--------------------
   No Operation
 * --------------------------------------------------*/
void no_operation ( void )
{

}





/*-----------------15/01/02 09.09-------------------
   Wait : half i2c clock period delay (normal: 5.0us / fast: 1.3us)
--------------------------------------------------*/
void I2cDelayH ( void )
{

  NOP;  NOP;  NOP;  NOP;

  //  NoOp; NoOp;
  //  NoOp; NoOp;
  //  NoOp; NoOp;
  //  NoOp; NoOp;
  //  NoOp; NoOp;
  //  NoOp; NoOp;
  //  NoOp; NoOp;
  //  NoOp; NoOp;
  //  NoOp; NoOp;
  //  NoOp; NoOp;
}





/*-----------------15/01/02 09.09-------------------
   Wait : quarter i2c clock period delay (normal: 2.5us / fast: 0.6us)
--------------------------------------------------*/
void I2cDelayQ ( void )
{

  NOP;  NOP;

  // NoOp; NoOp;
  // NoOp; NoOp;
  // NoOp;
  // NoOp; NoOp;
  // NoOp; NoOp;
  // NoOp;
}





/*-----------------16/01/02 13.38-------------------
   Inizializza l'I2C
--------------------------------------------------*/
void I2cInit ( void )
{
  INIT_I2C_LINE;

  I2C_SDA_IN;                  // SDA -> Input -> Hi
  I2C_SCL_IN;                  // SCL -> HI

  I2cStatus = 0;
}





/*-----------------16/01/02 13.38-------------------
   Deinizializza l'I2C
--------------------------------------------------*/
void I2cDeInit ( void )
{
  I2C_SDA_IN;                  // SDA -> Input -> Hi
  I2C_SCL_IN;                  // SCL -> HI
}





/*-----------------15/01/02 10.45-------------------
   Spedire uno start Intermedio
   Return : Risposta dello Slave
--------------------------------------------------*/
/*
UC I2c_RepStart ( UC Addr, UC R_W )
{
  // Stop Condition
  I2C_SCL_OUT;              // SCL -> OutPut Low
  I2C_SDA_OUT;	  			// SDA -> OutPut Low

  I2cDelayH();

  I2C_SCL_IN;               // SCL -> Input -> Hi

  I2cDelayQ();

  I2C_SDA_IN; 				// SDA -> Input -> Hi

  I2cDelayH();

  // Start Condition
  I2C_SDA_OUT;				// SDA -> OutPut Low
  I2cDelayQ();

  // Address
  if ( R_W == I2C_WRITE )
	  Addr &= 0xFE;
  else
	  Addr |= 0x01;
  return I2c_Write ( Addr );
}
*/

UC I2c_RepStart ( UC Addr, UC R_W )
{
  // NULL Condition
  I2C_SCL_OUT;              // SCL -> OutPut Low

  I2cDelayQ();

  I2C_SDA_IN; 							// SDA -> Input -> Hi

  I2cDelayH();

  I2C_SCL_IN;               // SCL -> Input -> Hi

  I2cDelayH();

  // ReStart Condition
  I2C_SDA_OUT;							// SDA -> OutPut Low
  I2cDelayQ();

  // Address
  if ( R_W == I2C_WRITE )
	  Addr &= 0xFE;
  else
	  Addr |= 0x01;
  return I2c_Write ( Addr );
}





/*-----------------15/01/02 10.58-------------------
   Spedisce uno Start
   Return : Risposta dello Slave (0 -> OK)
--------------------------------------------------*/
UC I2c_Start ( UC Addr, UC R_W )
{
  // Start condition
  I2C_SCL_IN;               // SCL -> Input -> Hi
  I2C_SDA_IN;				// SDA -> Input -> Hi

  I2C_SDA_OUT;				// SDA -> OutPut Low
  I2cDelayQ();

  // Address
  if ( R_W == I2C_WRITE )
	Addr &= 0xFE;
  else
	Addr |= 0x01;
  return I2c_Write ( Addr );
}





/*-----------------16/01/02 13.34-------------------
   Stop trasmissione
--------------------------------------------------*/
void I2c_Stop ( void )
{
  I2C_SCL_OUT;                  // SCL -> OutPut Low
  I2C_SDA_OUT;	  				// SDA -> OutPut Low

  I2cDelayH();

  I2C_SCL_IN;                  // SCL -> Input -> Hi

  I2cDelayQ();

  I2C_SDA_IN; 					// SDA -> Input -> Hi

  I2cDelayH();
}





volatile UC CntCardTOut;

/*-----------------23/01/02 10.29-------------------
   Controlla se l'E2 e` Pronta
   Wait = 1 -> Aspetta che sia pronta
--------------------------------------------------*/
UC WaitBusy ( UC FAddr, UC Wait )
{
  UC Out;

  CntCardTOut = 10;
  do {

	I2cStatus &= ~NOACKNOW;

  I2c_ReadByte ( FAddr, 0x00 );

	if ( !(I2cStatus & NOACKNOW) )
	  Wait = 0;

	if ( !CntCardTOut )
	  Wait = 0;

  } while ( Wait );

  return CntCardTOut;
}





/*-----------------15/01/02 11.57-------------------
   Scrive un dato ( Dati o Indirizzi )
   Return : Risposta dello Slave ( 0 -> OK )
--------------------------------------------------*/
UC I2c_Write ( UC Data )
{
  UC Ret, Tmp;


  // ** Scrittura
  Tmp = Data;
  Data <<= 1;

  // Manda in uscita
  I2C_SCL_OUT;              // SCL -> OutPut Low
  NoOp;
  NoOp;
  if ( Tmp & 0x80 ) {
	I2C_SDA_IN;				// SDA -> Input -> Hi
  } else {
	I2C_SDA_OUT;			// SDA -> OutPut Low
  }

  // Ritardo Clock
  I2cDelayH();
  I2C_SCL_IN;               // SCL -> Input -> Hi
  I2cDelayH();

  Data |= 0x01;

  do {

	// Prepara il Dato
    Tmp = Data;
    Data <<= 1;

	// Uscita
    if ( !Data )
	  break;

	// Manda in uscita
    I2C_SCL_OUT;            // SCL -> OutPut Low
    NoOp;
    NoOp;
    if ( Tmp & 0x80 ) {
	  I2C_SDA_IN;			// SDA -> Input -> Hi
    } else {
	  I2C_SDA_OUT;			// SDA -> OutPut Low
	}

	// Ritardo Clock
    I2cDelayH();
    I2C_SCL_IN;            // SCL -> Input -> Hi
    I2cDelayH();

  } while ( Data );


  // ** Lettura Acknowledge
  I2C_SCL_OUT;              // SCL -> OutPut Low
  I2C_SDA_IN;	   			// SDA -> Input -> Hi
  I2cDelayH();
  I2C_SCL_IN;              // SCL -> Input -> Hi
  I2cDelayH();

  Ret = 0;					// Controllo Slave
  if ( I2C_SDA ) {
	Ret = 1;
	I2cStatus |= NOACKNOW;
  }

  I2cDelayH();

  return Ret;
}





/*-----------------15/01/02 16.54-------------------
   Legge un dato
--------------------------------------------------*/
UC I2c_Read ( UC Last )
{
  UC Data, Tmp;

  // ** Lettura del dato
  Data = 1;
  do {

    I2C_SCL_OUT;                // SCL -> OutPut Low
	I2cDelayH();

    I2C_SCL_IN;                // SCL -> Input -> Hi
	I2cDelayH();

	Tmp = Data;
	Data <<= 1;
	if ( I2C_SDA )
	  Data |= 0x01;

  } while ( !(Tmp & 0x80) );


  // ** Send Acknowledge
  I2C_SCL_OUT;        // SCL -> OutPut Low
  if ( Last ) {
	I2C_SDA_IN; 				// SDA -> Input -> Hi
  } else {
	I2C_SDA_OUT;				// SDA -> OutPut Low
  }

  I2cDelayH();

  I2C_SCL_IN;                  // SCL -> Input -> Hi
  I2cDelayH();


  return Data;
}





/*-----------------16/01/02 14.04-------------------
   Scrive un Byte ad un Indirizzo
	 FAddr = Indirizzo fisico dello Slave
   Addr = Inidizzo interno su cui scrivere
	 Dato = Dato da Scrivere
--------------------------------------------------*/
void I2c_WriteByte ( UC FAddr, UC Addr, UC Dato )
{
  I2cStatus &= ~NOACKNOW;

	// Scrive l'indirizzo dello slave
  I2c_Start ( FAddr, I2C_WRITE );

	// Scrive l'indirizzo su cui scrivere
  if ( !(I2cStatus & NOACKNOW) )        // Solo se NON ci sono stati Errori
    I2c_Write ( Addr );

	// Scrive il dato a quell'indirizzo
  if ( !(I2cStatus & NOACKNOW) )        // Solo se NON ci sono stati Errori
    I2c_Write ( Dato );

  // Stop
  I2c_Stop();
}





/*-----------------16/01/02 14.04-------------------
   Legge un Byte ad un Indirizzo
	 FAddr = Indirizzo fisico dello Slave
   Addr = Inidizzo interno su cui leggere
	 Return : Dato Letto
--------------------------------------------------*/
UC I2c_ReadByte ( UC FAddr, UC Addr )
{
  UC Dato;

  I2cStatus &= ~NOACKNOW;

  // Scrive l'indirizzo dello slave
  I2c_Start ( FAddr, I2C_WRITE );

	// Scrive l'indirizzo su cui leggere
  if ( !(I2cStatus & NOACKNOW) )        // Solo se NON ci sono stati Errori
    I2c_Write ( Addr );

  // Legge a quell'indirizzo
  Dato = 0xFF;
  if ( !(I2cStatus & NOACKNOW) ) {        // Solo se NON ci sono stati Errori
    I2c_RepStart ( FAddr, I2C_READ );
    if ( !(I2cStatus & NOACKNOW) )        // Solo se NON ci sono stati Errori
      Dato = I2c_Read ( 1 );
  }

  // Stop
  I2c_Stop();

  return Dato;
}





#define       READ_BACK_TIME        1




/*-----------------16/01/02 14.04-------------------
   Scrive un Byte ad un Indirizzo
	 FAddr = Indirizzo fisico dello Slave
   Addr = Inidizzo interno su cui scrivere
	 Dato = Dato da Scrivere
--------------------------------------------------*/
void I2cWriteByte ( UC FAddr, UC Addr, UC Dato )
{
  UC Retry;
  UC Ret;


  for ( Retry = 0 ; Retry < 10 ; Retry++ ) {

    // Attesa ( non la prima volta )
    if ( Retry ) WaitmSec ( 1 );

    // Scrive il Dato
    I2c_WriteByte ( FAddr, Addr, Dato );

    if ( !(I2cStatus & NOACKNOW) )          // Solo se NON ci sono stati Errori
    {
      // Controllo fine scrittura
      WaitBusy ( FAddr, 1 );

#ifdef  READ_BACK_TIME
      WaitmSec ( 2 );
      if ( !(I2cStatus & NOACKNOW) )
      {
        // Rilettura di controllo
        Ret = I2c_ReadByte ( FAddr, Addr );
        // Controllo
        if ( !(I2cStatus & NOACKNOW) )        // Solo se NON ci sono stati Errori
          if ( Ret == Dato )
            break;
      }
#else
      break;
#endif
    }
  }
}



/*-----------------16/01/02 14.04-------------------
   Legge un Byte ad un Indirizzo
	 FAddr = Indirizzo fisico dello Slave
   Addr = Inidizzo interno su cui leggere
	 Return : Dato Letto
--------------------------------------------------*/
UC I2cReadByte ( UC FAddr, UC Addr )
{
  UC Retry;
  UC Ret, Ret2;


  for ( Retry = 0 ; Retry < 10 ; Retry++ ) {

    // Attesa ( non la prima volta )
    if ( Retry ) WaitmSec ( 1 );

    // Lettura del Dato
    Ret = I2c_ReadByte ( FAddr, Addr );

    if ( !(I2cStatus & NOACKNOW) )          // Solo se NON ci sono stati Errori
    {
#ifdef  READ_BACK_TIME
      // Rilettura per conferma
      Ret2 = I2c_ReadByte ( FAddr, Addr );

      // Controllo
      if ( !(I2cStatus & NOACKNOW) )        // Solo se NON ci sono stati Errori
        if ( Ret == Ret2 )
          break;
#else
      break;
#endif
    }

  }

  return Ret;
}
