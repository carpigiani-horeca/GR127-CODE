//-------------------------------------------------------------
//-------------------------------------------------------------
//
//
//          Dichiarazioni Costanti / Variabili / Strutture
//
//
//-------------------------------------------------------------
//-------------------------------------------------------------




typedef unsigned int UINT;                                      // Equivalente a 'WORD'
typedef unsigned char UCHAR;                                    // Equivalente a 'UC'


struct BitStruct {                                              // Equivalente a 'ByteBit'
  UCHAR Bit0 : 1;
  UCHAR Bit1 : 1;
  UCHAR Bit2 : 1;
  UCHAR Bit3 : 1;
  UCHAR Bit4 : 1;
  UCHAR Bit5 : 1;
  UCHAR Bit6 : 1;
  UCHAR Bit7 : 1;
};

#define         ToByte          *(unsigned char*)&              // Equivalente a 'Byte'









//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------            Direttive di Compilazione

// Abilita la Gestione degli ingressi collegati al Connettore CN13
//#define         COLLEGA_CN13            1

// Decide di NON chiamare la PArte ad Alto Livello
//   Utilizzata per testare la parte a BassoLivello
//#define     DISABLE_HILEV       1



// Decide se Abilitare le funzioni per il collaudo della Parte LowLevel
//#define     COLLAUDO_MACROTEK       1


// Indica che il Buzzer NON ha bisogno di una frequenza per farlo suonare
#define     BUZZER_AUTO_OSCILLANTE      1


// Indica se includere la Gestione della Parte Opzionale
#define     INCLUDE_OPTIONAL            1
#ifdef      INCLUDE_OPTIONAL

// Solo in Caso di Inclusione Parte Opzionale

// Include la Gestione del Display 2x16
#define     INCLUDE_DISPLAY             1

// Abilita la Gestione degli ingressi collegati al Connettore CN13
//#define     INCLUDE_EXTKEY              1

// Include la Gestione dell'uscita PWM
#define     INCLUDE_PWM                 1

#endif


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------            Pin del Micro

// Connettore CN4
#define     OutData             p0_1
#define     OutLatch            p4_3
#define     InOutClock          p6_2

// Ingressi
#define     InLoad              p3_7
#define     InData              p0_7

// Uscite
#define     K1OUT               p2_0
#define     K2OUT               p2_1
#define     K3OUT               p2_2
#define     K4OUT               p2_3
#define     TK1OUT              p2_4
#define     TK2OUT              p2_5

// EPot
#define     SI                  p6_3
#define     SCK                 p3_3
#define     nCS                 p4_4


#ifdef      BUZZER_AUTO_OSCILLANTE

// Buzzer
#define     BuzzerOut           p6_0
#define     BUZZER_EXT_Reale    BuzzerOut

#endif


//  Display LCD
#define         LcdPort         p1
#define         LcdPortDir      pd1
#define         LcdRead         p6_5 = 1
#define         LcdWrite        p6_5 = 0
#define         LcdComandi      p2_6 = 0
#define         LcdDati         p2_6 = 1
#define         EnaLcd          p6_4 = 1
#define         DisLcd          p6_4 = 0


// Led su Connettore CN13
#define         LED1            p6_1
#define         LED2            p2_7


// Uscita PWM
#define         PWM_OUT         p3_1










//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------            Timer / Contatori



extern volatile UCHAR Delay1;
extern volatile UCHAR Delay10;
extern volatile UCHAR Delay100;








//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------            Display LCD 2x16


#ifdef          INCLUDE_DISPLAY

extern struct BitStruct LcdState;
extern struct BitStruct LcdStates;

extern UCHAR Bufedi[34];              /* Buffer del display */
extern UCHAR TempLcdBuf[17];          /* Buffer Temporaneo per le scritte su Display ( es. Numeri ) */

// Per indirizzare le Linee del Display
#define                 LcdRiga1            &Bufedi[0]
#define                 LcdRiga2            &Bufedi[17]

// Riempe il buffer del display con degli spazi
#define       ClrBuf()    memset(Bufedi,' ',32)


/** posiziona il blink del cursore per le due righe **/
#define       PosBlk(x)   SetCursor(x);\
                            Cursor(1)
#define       PosBlkH(x)  SetCursor(x);\
                            HCursor(1)
#define       BlkOff      Cursor(0)
#define       Home        SetCursor(0)



extern struct BitStruct LcdState;
  #define	Blink_Char	LcdState.Bit0
  #define	Cursor_On   LcdState.Bit1
  #define	Chars_On    LcdState.Bit2

extern struct BitStruct LcdStates;
  #define   ReqInit     LcdStates.Bit0      /* richiedo un reset dell'LCD */
  #define   NoMsg       LcdStates.Bit1      /* sospende i messaggi in INTERRUPT */
  #define   IsInInit    LcdStates.Bit2      // dice se sto facendo l'inizializzazione del display

#endif





//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------            Buzzer

extern struct BitStruct BuzzStatus;

#define F_Buzzer         BuzzStatus.Bit0
#define Buzzer_Enab      BuzzStatus.Bit1

extern UCHAR C_Buzzer;








//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------            Ingressi

// Stato mediato degli Ingressi
extern struct BitStruct ExtInput;


// Pos Ingressi ( deve seguire ExtInput )
#define         CH_INP_I1       0
#define         CH_INP_I2       1
#define         CH_INP_I3       2
#define         CH_INP_BK1      3
#define         CH_INP_BK2      4


// Maschere per accedere allo Stato degli Ingressi
#define         INP_I1          ExtInput.Bit0
#define         INP_I2          ExtInput.Bit1
#define         INP_I3          ExtInput.Bit2
#define         INP_BK1         ExtInput.Bit3
#define         INP_BK2         ExtInput.Bit4









//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------            Uscite


//--- Struttura per le Uscite
extern struct BitStruct OutPut;


// Maschere per accedere alle Uscite
#define         OUT_K1          OutPut.Bit0
#define         OUT_K2          OutPut.Bit1
#define         OUT_K3          OutPut.Bit2
#define         OUT_K4          OutPut.Bit3
#define         OUT_TK1         OutPut.Bit4
#define         OUT_TK2         OutPut.Bit5









//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------            Schedino Led / Tasti / Digit

extern struct BitStruct Digit_1;       // Digit_1 -> cifra unita'
extern struct BitStruct Digit_2;       // Digit_2 -> cifra decine
extern struct BitStruct Digit_3;       // Digit_3 -> cifra centinaia
extern struct BitStruct Led;           // Led     -> DL1, DL2, Dl3, DPl1


extern UCHAR LampOn;
extern UCHAR Digit[3];
extern struct BitStruct LampEnable;
extern struct BitStruct Dot;
extern struct BitStruct SetLed;
extern struct BitStruct LampEnableLed;


#define        LED_4                        SetLed.Bit3     //
#define        LED_3                        SetLed.Bit2     //
#define        LED_2                        SetLed.Bit1     //
#define        LED_1                        SetLed.Bit0     //

#define        DOT_1                        Dot.Bit0
#define        DOT_2                        Dot.Bit1
#define        DOT_3                        Dot.Bit2

#define        LAMP_DIGIT_1                 LampEnable.Bit0
#define        LAMP_DIGIT_2                 LampEnable.Bit1
#define        LAMP_DIGIT_3                 LampEnable.Bit2

#define        LAMP_DOT_1                   LampEnable.Bit3
#define        LAMP_DOT_2                   LampEnable.Bit4
#define        LAMP_DOT_3                   LampEnable.Bit5

#ifdef      INCLUDE_EXTKEY

#define        LED_EXT_1                    SetLed.Bit4
#define        LED_EXT_2                    SetLed.Bit5

#endif




 extern volatile UCHAR KeyState;
 extern UCHAR NewKey;
 extern UCHAR NoKey;
 extern int PressCnt;                       // conta il tempo di pressione di un tasto



#ifdef      INCLUDE_EXTKEY

 // Tasti su CN13
 #define        MASK_EXT_1              0x08
 #define        MASK_EXT_2              0x10
 #define        MASK_EXT_3              0x20
 #define        MASK_EXT_4              0x40
 #define        MASK_EXT_5              0x80

#else

 // Tasti su CN4
 #define        MASK_P1                 0x08
 #define        MASK_P2                 0x10
 #define        MASK_P3                 0x20

#endif


// Stato di KeyState
#ifdef      INCLUDE_EXTKEY

 // Tasti su CN13
 #define        KEY_NONE                (0x00)
 #define        KEY_P1                  (MASK_EXT_1)
 #define        KEY_P2                  (MASK_EXT_2)
 #define        KEY_P3                  (MASK_EXT_3)
 #define        KEY_P4                  (MASK_EXT_4)
 #define        KEY_P5                  (MASK_EXT_5)
 #define        KEY_P1P2                (MASK_EXT_1 | MASK_EXT_2)

#else

 // Tasti su CN4
 #define        KEY_NONE                (0x00)
 #define        KEY_P1                  (MASK_P1)
 #define        KEY_P2                  (MASK_P2)
 #define        KEY_P3                  (MASK_P3)
 #define        KEY_P1P2                (MASK_P1 | MASK_P2)

#endif



 #define        KEY_PRESSED(Key)        ( KeyState & Key )
 #define        IS_KEY(Key)             ( KeyState == Key )

// Costanti di pressione tasti (tenendo conto che la RimbKey() e' chiamata ogni 10 mSec)
#define         LONGPRES1               100              // LONGPRES1*10 = n.ro Sec
#define         LONGPRES2               100              // LONGPRES2*10 = n.ro Sec
#define         LONGPRES3               200              // LONGPRES3*10 = n.ro Sec











//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------            Conversione A/D


#define     StartAD             adst      // inizio conversione A/D

extern UINT ValAD[8];

// Corrispondenza dei Canali A/D
#define         AD_CH_NTC           0
#define         AD_CH_PTC           1
#define         AD_CH_TA            2
#define         AD_CH_010           3

// Pin Utilizzati dai Canali A/D
#define         AD_PIN_TA           5
#define         AD_PIN_NTC          2
#define         AD_PIN_PTC          1
#define         AD_PIN_010          7

// Numero Medie per il Secondo Livello
#define         AD_AVERAGE          8


// Valore finale Mediato
extern UINT AdValue[4];

// Valori Utilizzabili dall'Esterno
#define         NtcValue        AdValue[AD_CH_NTC]
#define         PtcValue        AdValue[AD_CH_PTC]
#define         TaValue         AdValue[AD_CH_TA]
#define         In010Value      AdValue[AD_CH_010]

// 0.28 Volt ad ogni 1Volt in Ingresso ( 57.3 in unita A/D )
// 2.8 Volt per 10 Volt ( 10000 mVolt ) in Ingresso ( 573 in unita A/D )
#define         Input0_10V      (UINT)(((long)In010Value*10000)/573)
//#define         H_O_T           (UINT)(((long)In010Value*120)/573)
#define           H_O_T_P         ((UINT)In010Value) // HOT()














//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------            I2c per TimeKeeper


#define     INIT_I2C_LINE   pd3_4 = 0; pd3_5 = 0; p3_4 = 0; p3_5 = 0 /* Input */

#define     I2C_SDA         p3_4                            /* P34->SDA */
#define     I2C_SDA_OUT     pd3_4 = 1; p3_4 = 0
#define     I2C_SDA_IN      pd3_4 = 0

#define     I2C_SCL         p3_5                            /* P35->SCL */
#define     I2C_SCL_OUT     pd3_5 = 1; p3_5 = 0
#define     I2C_SCL_IN      pd3_5 = 0
