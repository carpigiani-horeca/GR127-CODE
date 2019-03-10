//-------------------------------------------------------------
//-------------------------------------------------------------
//
//
// Gestione Schedino esterno 3 Digit + 3 Tasti + 4 Led
//
//
//-------------------------------------------------------------
//-------------------------------------------------------------
#include "Sfr_r825.h"
#include "defsLow.h"
#include "protLow.h"
#include "Interrupt.h"




struct BitStruct Digit_1;       // Digit_1 -> cifra unita'
struct BitStruct Digit_2;       // Digit_2 -> cifra decine
struct BitStruct Digit_3;       // Digit_3 -> cifra centinaia

struct BitStruct Led;           // Led     -> DL1, DL2, Dl3, DPl1



const UCHAR reverse_bit[] = {
  0x00,0x80,0x40,0xC0,0x20,0xA0,0x60,0xE0,0x10,0x90,0x50,0xD0,0x30,0xB0,0x70,0xF0,
  0x08,0x88,0x48,0xC8,0x28,0xA8,0x68,0xE8,0x18,0x98,0x58,0xD8,0x38,0xB8,0x78,0xF8,
  0x04,0x84,0x44,0xC4,0x24,0xA4,0x64,0xE4,0x14,0x94,0x54,0xD4,0x34,0xB4,0x74,0xF4,
  0x0C,0x8C,0x4C,0xCC,0x2C,0xAC,0x6C,0xEC,0x1C,0x9C,0x5C,0xDC,0x3C,0xBC,0x7C,0xFC,
  0x02,0x82,0x42,0xC2,0x22,0xA2,0x62,0xE2,0x12,0x92,0x52,0xD2,0x32,0xB2,0x72,0xF2,
  0x0A,0x8A,0x4A,0xCA,0x2A,0xAA,0x6A,0xEA,0x1A,0x9A,0x5A,0xDA,0x3A,0xBA,0x7A,0xFA,
  0x06,0x86,0x46,0xC6,0x26,0xA6,0x66,0xE6,0x16,0x96,0x56,0xD6,0x36,0xB6,0x76,0xF6,
  0x0E,0x8E,0x4E,0xCE,0x2E,0xAE,0x6E,0xEE,0x1E,0x9E,0x5E,0xDE,0x3E,0xBE,0x7E,0xFE,
  0x01,0x81,0x41,0xC1,0x21,0xA1,0x61,0xE1,0x11,0x91,0x51,0xD1,0x31,0xB1,0x71,0xF1,
  0x09,0x89,0x49,0xC9,0x29,0xA9,0x69,0xE9,0x19,0x99,0x59,0xD9,0x39,0xB9,0x79,0xF9,
  0x05,0x85,0x45,0xC5,0x25,0xA5,0x65,0xE5,0x15,0x95,0x55,0xD5,0x35,0xB5,0x75,0xF5,
  0x0D,0x8D,0x4D,0xCD,0x2D,0xAD,0x6D,0xED,0x1D,0x9D,0x5D,0xDD,0x3D,0xBD,0x7D,0xFD,
  0x03,0x83,0x43,0xC3,0x23,0xA3,0x63,0xE3,0x13,0x93,0x53,0xD3,0x33,0xB3,0x73,0xF3,
  0x0B,0x8B,0x4B,0xCB,0x2B,0xAB,0x6B,0xEB,0x1B,0x9B,0x5B,0xDB,0x3B,0xBB,0x7B,0xFB,
  0x07,0x87,0x47,0xC7,0x27,0xA7,0x67,0xE7,0x17,0x97,0x57,0xD7,0x37,0xB7,0x77,0xF7,
  0x0F,0x8F,0x4F,0xCF,0x2F,0xAF,0x6F,0xEF,0x1F,0x9F,0x5F,0xDF,0x3F,0xBF,0x7F,0xFF,
};



//_______________________________________________________________________________
//  Traduzione da elementi luminosi del DIGIT a numero esadecimale
//  G F A B E D C H
//  x x x x x x x x   ->    0xXX
//
//             A_
//           F| |B
//            G_
//           E| |C
//            D_ .H
//
//________________________________________________________________________________

#define     DP_MASK_3       0x40                  /* Maschera per il DecimalPoint */
#define     DP_MASK_1_2     0x02                  /* Maschera per il DecimalPoint */

// *** Mappatura dei numeri su Lcd
const UCHAR Digit_Table_1_2[64] = {
//                  DHEC GBFA    		FABG    CEHD
    0xED, // "0" -> 1011 0111     		1110 	1101
    0x28, // "1" -> 0001 0100     		0010 	1000
    0xB5, // "2" -> 1010 1101     		1011 	0101
    0xB9, // "3" -> 1001 1101     		1011 	1001
    0x78, // "4" -> 0001 1110     		0111 	1000
    0xD9, // "5" -> 1001 1011     		1101 	1001
    0xDD, // "6" -> 1011 1011     		1101 	1101
    0xA8, // "7" -> 0001 0101     		1010 	1000
    0xFD, // "8" -> 1011 1111     		1111 	1101
    0xF9, // "9" -> 1001 1111     		1111 	1001

    // MAIUSCOLE    DHEC GBFA                   AFBG    CEHD
    0xFC,//0x3F, // "A" -> 0011 1111            1111    1100
    0xFD,//0xBF, // "B" -> 1011 1111            1111    1101
    0xC5,//0xA3, // "C" -> 1010 0011            1100    0101
    0x3D,//0xBC, // "d" -> 1011 1100            0011    1101
    0xD5, //0xAB, // "E" -> 1010 1011           1101    0101
    0xD4, //0x2B, // "F" -> 0010 1011           1101    0100
    0xCD, //0xB3, // "G" -> 1011 0011           1100    1101
    0x7C, //0x3E, // "H" -> 0011 1110           0111    1100
    0x28, //0x14, // "I" -> 0001 0100           0010    1000
    0x29, //0x94, // "J" -> 1001 0100           0010    1001
    0x38, //0x1C, // "K" -> 0001 1100           0011    1000
    0x45, //0xA2, // "L" -> 1010 0010           0100    0101
    0xEC, //0x37, // "M" -> 0011 0111           1110    1100
    0x1C, //0x38, // "n" -> 0011 1000           0001    1100
    0xED, //0xB7, // "O" -> 1011 0111           1110    1101
    0xF4, //0x2F, // "p" -> 0010 1111           1111    0100
    0xF8, //0x1F, // "q" -> 0001 1111           1111    1000
    0x14, //0x28, // "r" -> 0010 1000           0001    0100
    0xD9, //0x9B, // "S" -> 1001 1011           1101    1001
    0x55, //0xAA, // "t" -> 1010 1010           0101    0101
    0x0D, //0xB0, // "U" -> 1011 0000           0000    1101
    0x6D, //0xB6, // "V" -> 1011 0110           0110    1101
    0x7D, //0xBE, // "W" -> 1011 1110           0111    1101
//    0x00, //0x00, // "X" ->
    0x80, //0x00, // "X" -> trattino alto (opposto di underscore) ->     1000    0000
    0x79, //0x9E, // "y" -> 1001 1110           0111    1001
    0xB5, //0xAD, // "Z" -> 1010 1101           1011    0101

    // minuscole
    0xFC, //0x3F, // "A" -> 0011 1111           1111    1100
    0x5D, //0xBA, // "b" -> 1011 1010           0101    1101
    0x15, //0xA8, // "c" -> 1010 1000           0001    0101
    0x3D, //0xBC, // "d" -> 1011 1100           0011    1101
    0xD5, //0xAB, // "E" -> 1010 1011           1101    0101
    0xD4, //0x2B, // "F" -> 0010 1011           1101    0100
    0xCD, //0xB3, // "G" -> 1011 0011           1100    1101
    0x7C, //0x3E, // "H" -> 0011 1110           0111    1100
    0x28, //0x14, // "I" -> 0001 0100           0010    1000
    0x29, //0x94, // "J" -> 1001 0100           0010    1001
    0x38, //0x1C, // "K" -> 0001 1100           0011    1000
    0x45, //0xA2, // "L" -> 1010 0010           0100    0101
    0xEC, //0x37, // "M" -> 0011 0111           1110    1100
    0x1C, //0x38, // "n" -> 0011 1000           0001    1100
    0x1D, //0xB8, // "o" -> 1011 1000           0001    1101
    0xF4, //0x2F, // "p" -> 0010 1111           1111    0100
    0xF8, //0x1F, // "q" -> 0001 1111           1111    1000
    0x14, //0x28, // "r" -> 0010 1000           0001    0100
    0xD9, //0x9B, // "S" -> 1001 1011           1101    1001
    0x55, //0xAA, // "t" -> 1010 1010           0101    0101
    0x0D, //0xB0, // "u" -> 1011 0000           0000    1101
    0x6D, //0xB6, // "V" -> 1011 0110           0110    1101
    0x7D, //0xBE, // "W" -> 1011 1110           0111    1101
//    0x00, //0x00, // "X" ->
    0x01, //0x00, // "x" -> trattino basso (underscore) ->     0000    0001
    0x79, //0x9E, // "y" -> 1001 1110           0111    1001
    0xB5, //0xAD, // "Z" -> 1010 1101           1011    0101

    0x00, //0x00, // " " -> 0000 0000           0000    0000
    0x10, //0x08  // "-" -> 0000 1000           0001    0000
};


// *** Mappatura dei numeri su Lcd
const UCHAR Digit_Table_3[64] = {
//                  		DHEC GBFA
    0xB7, // "0" -> 1011 0111
    0x14, // "1" -> 0001 0100
    0xAD, // "2" -> 1010 1101
    0x9D, // "3" -> 1001 1101
    0x1E, // "4" -> 0001 1110
    0x9B, // "5" -> 1001 1011
    0xBB, // "6" -> 1011 1011
    0x15, // "7" -> 0001 0101
    0xBF, // "8" -> 1011 1111
    0x9F, // "9" -> 1001 1111

    // MAIUSCOLE
    0x3F, // "A" -> 0011 1111
    0xBF, // "B" -> 1011 1111
    0xA3, // "C" -> 1010 0011
    0xBC, // "d" -> 1011 1100
    0xAB, // "E" -> 1010 1011
    0x2B, // "F" -> 0010 1011
    0xB3, // "G" -> 1011 0011
    0x3E, // "H" -> 0011 1110
    0x14, // "I" -> 0001 0100
    0x94, // "J" -> 1001 0100
    0x1C, // "K" -> 0001 1100
    0xA2, // "L" -> 1010 0010
    0x37, // "M" -> 0011 0111
    0x38, // "n" -> 0011 1000
    0xB7, // "O" -> 1011 0111
    0x2F, // "p" -> 0010 1111
    0x1F, // "q" -> 0001 1111
    0x28, // "r" -> 0010 1000
    0x9B, // "S" -> 1001 1011
    0xAA, // "t" -> 1010 1010
    0xB0, // "U" -> 1011 0000
    0xB6, // "V" -> 1011 0110
    0xBE, // "W" -> 1011 1110
//    0x00, // "X" ->
    0x01, //0x00, // "X" -> trattino alto (opposto di underscore) ->     1000    0000
    0x9E, // "y" -> 1001 1110
    0xAD, // "Z" -> 1010 1101

    // minuscole
    0x3F, // "A" -> 0011 1111
    0xBA, // "b" -> 1011 1010
    0xA8, // "c" -> 1010 1000
    0xBC, // "d" -> 1011 1100
    0xAB, // "E" -> 1010 1011
    0x2B, // "F" -> 0010 1011
    0xB3, // "G" -> 1011 0011
    0x3E, // "H" -> 0011 1110
    0x14, // "I" -> 0001 0100
    0x94, // "J" -> 1001 0100
    0x1C, // "K" -> 0001 1100
    0xA2, // "L" -> 1010 0010
    0x37, // "M" -> 0011 0111
    0x38, // "n" -> 0011 1000
    0xB8, // "o" -> 1011 1000
    0x2F, // "p" -> 0010 1111
    0x1F, // "q" -> 0001 1111
    0x28, // "r" -> 0010 1000
    0x9B, // "S" -> 1001 1011
    0xAA, // "t" -> 1010 1010
    0xB0, // "u" -> 1011 0000
    0xB6, // "V" -> 1011 0110
    0xBE, // "W" -> 1011 1110
//    0x00, // "X" ->
    0x80, //0x00, // "x" -> trattino basso (underscore) ->     1000    0000
    0x9E, // "y" -> 1001 1110
    0xAD, // "Z" -> 1010 1101

    0x00, // " " -> 0000 0000
    0x08  // "-" -> 0000 1000
};









//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------            Led / Digit




//-------------------------------
//      Invia un Byte (595) e ne legge un altro (166)
UCHAR IntOut ( UCHAR Out )
{
  UCHAR CntB;
  UCHAR TempI;


  InOutClock = 0;
  TempI = 0x00;
  for ( CntB = 0 ; CntB < 8 ; CntB++ )
  {
    // Prepara il bit in Uscita
    if ( Out & 0x80 )
      OutData = 1;
    else
      OutData = 0;

    Out <<= 1;
    TempI <<= 1;

    // Lo manda allo ShiftRegister
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    InOutClock = 1;
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    InOutClock = 0;

    // Memorizza il bit in Entrata
    if ( InData )
      TempI |= 0x01;

    // Rilascia il Load per gli Ingressi
    InLoad = 1;
  }

  return TempI;
}









extern UCHAR Input;

// *** Procedura di refresh dei DIGIT, LED
//          Chiamata ogni 10mSec
void SetDigitLed ( void )
{
  // Prepara i Latch
  InLoad = 0;
  OutLatch = 0;

  Input = IntOut ( ~(ToByte(Digit_1)) );

  IntOut ( ~(ToByte(Digit_2)) );

  IntOut ( reverse_bit[~(ToByte(Digit_3))] );

  IntOut ( ~(ToByte(Led)) );

#ifdef      INCLUDE_EXTKEY

  if ( (ToByte(Led)) & 0x10 ) LED1 = 1; else LED1 = 0;
  if ( (ToByte(Led)) & 0x20 ) LED2 = 1; else LED2 = 0;

#endif

  // Latch delle USCITE
  OutLatch = 1;
}












UCHAR LampOn;
UCHAR Digit[3];
struct BitStruct LampEnable;
struct BitStruct Dot;
struct BitStruct SetLed;
struct BitStruct LampEnableLed;


// *** Procedura di caricamento dei valori sui DIGIT, accensione dei LED e gestione del lampeggio
//      ( chiamata nella DoIdle() ogni 10 mSec )
void SendToDigit ( void )
{
  UCHAR TempDigit_1, TempDigit_2, TempDigit_3, LedTemp;


  // Led
  LedTemp = ToByte(SetLed);


  // Digit 1 (unita')
  TempDigit_1 = Digit_Table_1_2[ Digit[0] ];
  if ( Dot.Bit0 )
    TempDigit_1 |= DP_MASK_1_2;

  // Digit 2 (decine)
  TempDigit_2 = Digit_Table_1_2[ Digit[1] ];
  if ( Dot.Bit1 )
    TempDigit_2 |= DP_MASK_1_2;

  // Digit 3 (centinaia)
  TempDigit_3 = Digit_Table_3[ Digit[2] ];
  if ( Dot.Bit2 )
    TempDigit_3 |= DP_MASK_3;


  if ( LampEnable.Bit0 && !LampOn )             // lampeggio DIGIT 1
    TempDigit_1 &= DP_MASK_1_2;

  if ( LampEnable.Bit1 && !LampOn )             // lampeggio DIGIT 2
    TempDigit_2 &= DP_MASK_1_2;

  if ( LampEnable.Bit2 && !LampOn )             // lampeggio DIGIT 3
    TempDigit_3 &= DP_MASK_3;


  if ( LampEnable.Bit3 && !LampOn )             // lampeggio DOT 1
    TempDigit_1 &= ~DP_MASK_1_2;

  if ( LampEnable.Bit4 && !LampOn )             // lampeggio DOT 2
    TempDigit_2 &= ~DP_MASK_1_2;

  if ( LampEnable.Bit5 && !LampOn )             // lampeggio DOT 3
    TempDigit_3 &= ~DP_MASK_3;


  if ( LampEnableLed.Bit0 && !LampOn )           // lampeggio LED4 (Pl1)
    LedTemp &= 0xFE;

  if ( LampEnableLed.Bit1 && !LampOn )           // lampeggio LED3
    LedTemp &= 0xFD;

  if ( LampEnableLed.Bit2 && !LampOn )           // lampeggio LED2
    LedTemp &= 0xFB;

  if ( LampEnableLed.Bit3 && !LampOn )           // lampeggio LED1
    LedTemp &= 0xF7;

#ifdef      INCLUDE_EXTKEY

  if ( LampEnableLed.Bit4 && !LampOn )           // lampeggio LED_CN13_1
    LedTemp &= 0xEF;

  if ( LampEnableLed.Bit5 && !LampOn )           // lampeggio LED_CN13_2
    LedTemp &= 0xDF;

#endif


  ToByte(Digit_1) = TempDigit_1;                  // assegnamento e conversione a byte per il DIGIT 1

  ToByte(Digit_2) = TempDigit_2;                  // assegnamento e conversione a byte per il DIGIT 2

  ToByte(Digit_3) = TempDigit_3;                  // assegnamento e conversione a byte per il DIGIT 3

  ToByte(Led) = LedTemp;                          // assegnamento e conversione a byte per i LED
}














//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------            Key





// Stato ingressi RealTime
UCHAR Input;




int PressCnt;                               // conta il tempo di pressione di un tasto

UCHAR NoKey;
UCHAR OldKey;
UCHAR KeyCnt;
volatile UCHAR KeyState;
UCHAR NewKey;


// *** Procedura che rileva la pressione di un tasto
//     ( lettura dei tasti con meccanismo di antirimbalzo )
void RimbKey ( void )
{
  UCHAR Key;


  // Controllo Stato dei Tasti
  Key = Input;
  Key = ~Key;
#ifndef     INCLUDE_EXTKEY
  // Ci sono solo 3 Tasti -> Maschero gli Altri
  Key &= 0x38;
#else
  // Ci sono 5 Tasti -> Maschero gli altri Bit
  Key &= 0xF8;
#endif

  // Antirimbalzo
  if ( Key == OldKey ) {
  	PressCnt++;
    if ( KeyCnt )
      KeyCnt--;
    else {
      KeyState = OldKey;
      if ( KeyState == KEY_NONE ) {
        NoKey = 1;
        PressCnt = 0;
      } else {
          if ( NoKey )
            NewKey = 1;
          NoKey = 0;
      }
    }
  } else {
      OldKey = Key;
      KeyCnt = 10-1;                        // n.ro di letture per antirimbalzo
  }

}




	/*  while ( 1 )
	  {
	    Play_Buzzer ( HIGH, 30 );
	    //StringVal( "IrE"); // DigitValProg(  (UINT)In010Value );
    	//Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
	    StringVal( "111"); // DigitValProg(  (UINT)In010Value );
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
	    StringVal( "222"); // DigitValProg(  (UINT)In010Value );
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
	    StringVal( "333"); // DigitValProg(  (UINT)In010Value );
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
	    StringVal( "444"); // DigitValProg(  (UINT)In010Value );
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
	    StringVal( "555"); // DigitValProg(  (UINT)In010Value );
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
	    StringVal( "666"); // DigitValProg(  (UINT)In010Value );
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
	    StringVal( "777"); // DigitValProg(  (UINT)In010Value );
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
	    StringVal( "888"); // DigitValProg(  (UINT)In010Value );
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
	    StringVal( "999"); // DigitValProg(  (UINT)In010Value );
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
	    StringVal( "000"); // DigitValProg(  (UINT)In010Value );
		// MAIU
	 	StringVal("AAA");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("BBB");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("CCC");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("ddd");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("EEE");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("FFF");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("GGG");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("HHH");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("III");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("JJJ");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("KKK");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("LLL");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("MMM");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("nnn");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("OOO");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("ppp");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("qqq");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("rrr");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("SSS");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("ttt");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("UUU");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("VVV");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("WWW");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("XXX");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("yyy");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("ZZZ");

		// min
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("AAA");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("bbb");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("ccc");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("ddd");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("EEE");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("FFF");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("GGG");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("HHH");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("III");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("JJJ");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("KKK");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("LLL");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("MMM");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("nnn");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("ooo");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("ppp");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("qqq");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("rrr");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("SSS");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("ttt");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("uuu");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("VVV");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("WWW");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("XXX");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("yyy");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("ZZZ");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori

		StringVal("   ");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StringVal("---");
    	Wait10mSec( 500 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori

	  }  */
