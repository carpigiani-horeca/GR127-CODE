//-------------------------------------------------------------
//-------------------------------------------------------------
//
//
// Gestione Procedure di Inizializzazione
//
//
//-------------------------------------------------------------
//-------------------------------------------------------------
#include "Sfr_r825.h"
#include "defsLow.h"
#include "protLow.h"
#include "Interrupt.h"





//------------------------------
//          Inizializzazione del micro
void SystemInit ( void )
{
  char Cnt;

  prc0 = 1;                     /* Protect off : System control resistor protect cancellation */
  cm11 = 0;                     /* Xin-Xout On-chip feed back resistor enabled */
  cm13 = 1;                     /* Xin-Xout pin */
  cm15 = 1;                     /* Xin-Xout drive capacity select bit : HIGH */
  asm("NOP");                   /* Waiting for stable of oscillation */
  asm("NOP");                   /* Waiting for stability */

  cm05 = 0;                     /* Main clock : oscillation */
  asm("NOP");                   /* Waiting for stability */
  asm("NOP");                   /* Waiting for stability */

  cm16 = 0;                     /* Main clock = No division mode */
  cm17 = 0;                     /* Main clock = No division mode */
  for ( Cnt = 1 ; Cnt < 100 ; Cnt++ ) ;         /* Waiting for stability */

  cm06 = 0;                     /* Main clock division : CM16 and CM17 enabled */
  cm07 = 0;                     /* System clock is selected */
  asm("NOP");                   /* Waiting for stability */
  asm("NOP");                   /* Waiting for stability */
  ocd2 = 0;                     /* Main clock change */
  for ( Cnt = 1 ; Cnt < 100 ; Cnt++ ) ;         /* Waiting for stability */

  cm12 = 0;                     /* Xcin-Xcout On-chip feed back resistor enabled */
  cm04 = 0;                     /* Xcin-Xcout oscillation function is selected */
  asm("NOP");                   /* Waiting for stability */
  asm("NOP");                   /* Waiting for stability */
  prc0 = 0;                     /* Protect on : System control resister protect */
}









//-----------------------------
//      Inizializzazione dei Port / Periferiche
void PeripheralInit ( void )
{
  //--- Settaggio dei Port
  prc2 = 1;
  pd0 = 0x02;           // I### ##O#
  p0 = 0x00;            // D### ##0#
  prc2 = 0;

  pd1 = 0x00;           // #### ####
  p1 = 0x00;            // #### ####

  pd2 = 0xFF;           // OOOO OOOO
  p2 = 0x30;            // 0011 0000

  pd3 = 0x8A;           // O### O#O#
  p3 = 0x00;            // 0### 0#0#

  pd4 = 0x18;           // ###O OI##
  p4 = 0x10;            // ###1 0D##

  pd6 = 0x3F;           // ##OO OOOO
  p6 = 0x20;            // ##10 0000

  // Pull-up
  pur0 = 0x00;
  pur1 = 0x00;

  // Port mode resister
  pmr = 0x90;           // CLK1,TXD1,RXD1 + Seriale I2C


  //--- Timer
  // Timer Ra
  traioc = 0x00;        // ##00 #000 No I/O Function
  tracr = 0x00;         // ##00 #0#0 Timer Stopped
  tramr = 0x10;         // 0001 #000 Fi/8, TimerMode
  traic = 0x06;         // #### 0110 Priorita 6
#ifdef      INCLUDE_PWM
  // Timer RB (PWM)
  trbcr = 0x00;         // #### #0#0 Timer Stopped
  trbocr = 0x00;        // #### #000 OneShot OFF
  trbioc = 0x00;        // #### 0000 TBOut Enabled, Pwm:OnOff-OnOff-...
  trbmr = 0x15;         // 0#01 1#01 Fi/8
  trbpre = 0;           // F = Fi/8 /100
  trbsc = 1-1;          // On = 1/100
  trbpr = 99-1;         // Off = 99/100
#endif
  // Timer RD
  trdstr = 0x0C;        // 0000 1100 Timer Run Continuo
  trdmr = 0x00;         // 0000 0000 Standard mode
  trdpmr = 0x00;        // 0000 0000 No Pwm
  trdfcr = 0x80;        // 10## ##00 NoExt Clock, Timer Mode
  trdoer1 = 0xFF;       // 1111 1111 No Output Mode
  trdoer2 = 0x00;       // 0### ####
  // Timer RD-0 (1mSec)
  trdcr0 = 0x23;        // 001# #011 Cleared by tdrgra0, Fi/8
  trdiora0 = 0x08;      // #000 1000 No Output Change
  trdiorc0 = 0x88;      // 1000 1000 No Output Change
  trdier0 = 0x01;       // 0000 0001 Interrupt on IMIEA
  trd0 = 0;
  trdgra0 = 1000;       // Fi/8/1000
  trd0ic = 0x02;        // Interrupt Priority = 2
  // Timer RD-1 (100mSec)
  trdcr1 = 0x24;        // 001# #100 Cleared by tdrgra1, Fi/32
  trdiora1 = 0x08;      // #000 1000 No Output Change
  trdiorc1 = 0x88;      // 1000 1000 No Output Change
  trdier1 = 0x01;       // 0000 0001 Interrupt on IMIEA
  trd1 = 0;
  trdgra1 = 25000;      // Fi/32/25000
  trd1ic = 0x01;        // Interrupt Priority = 1
  // Timer RE (4KHz)
#ifndef      BUZZER_AUTO_OSCILLANTE
  trecr1 = 0x10;        // Timer Reset
  trecr1 = 0x04;        // #### #1## Output Enable
  trecr2 = 0x20;        // ##1# #### Compare Mode
  trecsr = 0x61;        // #11# #001 Compare Output, No 4bit divider, F = Fi / 8 ( /2 )
  tremin = 62;          // F/62 -> Periodo = 1/(F/125)
#endif

  // Int0 ( ZeroCrossing )
  //inten = 0x03;         // #### ##11 Entrambi i Fronti, Abilitato
  //int0ic = 0x07;        // ###0 0111 Priorita 7 (Max)
  inten = 0x01;         // #### ##11 1 Fronte, Abilitato
  int0ic = 0x17;        // ###1 0111 Salita, Priorita 7 (Max)

  // Analog Converter
  adcon0 = 0x80;        // 1000 0ccc AN0 is selected , Single mdoe , Port P0 group is selected , Soft trigger , A/D conversion stop , fAD/2
  //adcon1 = 0x08;        // ##00 1### 10bit mode , cks1=0 setting, NO Vref
  adcon1 = 0x28;        // ##10 1### 10bit mode , cks1=0 setting, Vref ON
  adcon2 = 0x00;        // #### ###0 No sample and hold function is selected


  // Timer Start
  trdstr = 0x0F;        // 0000 1111 Timer Run Continuo + TRD0 + TRD1 Start
/*
  //--- Timer
  // Timer RB
  trbcr = 0x00;           // Timer mode , count stop
  trbocr = 0x00;          // Timer RB one-shot is not used
  trbioc = 0x00;          // Timer mode
  trbmr = 0x00;           // Timer mode is selected , f1 , provides count source
  trbpre = 125-1;         // Timer RB prescaler register : set to 125-1 (20MHz x f1 x 125 = 6.25usec)
  trbpr = 40-1;           // Timer RB Primary Register : set to 40-1 (6.25usec x 40 = 250usec)
  trbic = 0x02;           // Timer RB interrupt : Priority level-2 is selected , Interrupt request bit = clear


  // Timer Start
  tstart_trbcr = 1;       // Timer RB conut start bit = start
  trdstr = 0x0F;          // 0000 1111 Timer Run Continuo + TRD0 + TRD1 Start
*/
}


