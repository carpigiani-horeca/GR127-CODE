/**************************************************************
 *                                                            *
 *               definizioni e strutture dati                 *
 *                                                            *
 **************************************************************/
#include "Sfr_r825.h"
#include "defsLow.h"
#include "protLow.h"
#include "Interrupt.h"










// => commentare la #DEFINE per il funzionamento normale;
// => togliere il commento per debuggare con KD30
//____________________________________________________
// #define DEBUG_KD30   1










// *** definizione di No-OPeration del microcontrollore
#define		NOP		asm ( "nop" )



// *** definizioni di byte, word, doubleword
typedef   unsigned char       UC;
typedef   unsigned char       BYTE;
typedef   unsigned int        WORD;
typedef   signed   char       SC;
typedef   unsigned long int   DWORD;



// *** definizioni di conversione a byte, word, doubleword
#define   Byte *(unsigned char*)&
#define   Word *(unsigned int*)&
#define   Long *(unsigned long*)&



// *** definizione di un byte gestibile a singoli bit
struct ByteBit {
  UC Bit0 : 1;
  UC Bit1 : 1;
  UC Bit2 : 1;
  UC Bit3 : 1;
  UC Bit4 : 1;
  UC Bit5 : 1;
  UC Bit6 : 1;
  UC Bit7 : 1;
};










/***********************************************************************
 *                                                                     *
 *      definizioni e strutture dati relative a porzioni di hardware   *
 *                                                                     *
 ***********************************************************************/









// *********************************** Timers *******************************************

extern volatile UC Is100mSec;           // per DoIdle()
extern volatile UC Is10mSec;            // per DoIdle()
extern volatile UC Is1mSec;             // per DoIdle()
extern volatile UC Is1Sec;              // per DoIdle()
extern volatile UC Is1Min;              // per DoIdle()

extern volatile UC Main_100mSec;        // per Main()
extern volatile UC Main_10mSec;         // per Main()
extern volatile UC Main_1mSec;          // per Main()
extern volatile UC Main_1Sec;           // per Main()
extern volatile UC Main_1Min;           // per Main()

extern volatile UC Prd_10mSec;          // per Produzione()
extern volatile UC Mon_100mSec;         // per Montoring()
extern volatile UC Stop_100mSec;        // per Stop()
extern volatile UC Prd_100mSec;         // per Produzione()
extern volatile UC Sto_100mSec;         // per Conservazione()
extern volatile UC Cle_100mSec;         // per Pulizia()
extern volatile UC Pas_100mSec;         // per Pastorizzazione()
extern volatile UC Select_100mSec;      // per Selezione()
extern volatile UC Prd_1Sec;            // per Produzione()
extern volatile UC Stop_1Min;           // per Stop()
extern volatile UC Mon_1Min;            // per Monitoring()
extern volatile UC Pas_1Min;            // per Pastorizzazione()

extern volatile UC Speed_100mSec;       // per ProgrammazioneUtente() e ProgrammazioneTecnico()

extern volatile UC Cnt100mSec;
extern volatile UC Cnt10mSec;  // � usato anche per la visualizzazione di "LoW" lampeggiante quando il livello � scoperto
extern volatile UC Cnt1mSec;
extern volatile UC Cnt1Sec;
extern volatile UC Cnt1Min;
extern volatile UC CntForSec;
extern volatile UC CntForMin;


extern volatile UC CntForDelay;


extern volatile UC Polling;


// ********************************* Buzzer **********************************************

#define		BEEPLEN			10
#define		WAITBEEP		20
#define   HIGH        4000
#define   LOW         3500
#define		VERYLOW			3000



#define Hi 1
#define HI 1
#define RF_TX 1
#define RF_RX 0
#define TRUE 1
#define True 1
#define False 0
#define FALSE 0
#define Lo 0
#define LO 0



#define Flag_100msA      Status1.b2
#define Flag_100msB      Status1.b3


extern near struct bit_def Status1, Status2;





// ***************************** Display Digit e Led ************************************
extern  const char *c1;
extern  const char *c2;
extern  const char *c3;

extern        int   OldVal;

#define         Null                                (const char *)0





 #define        LAMP_LED_STOP                       LAMP_LED_4
 #define        LAMP_LED_PULIZIA                    LAMP_LED_1
 #define        LAMP_LED_PRODUZIONE                 LAMP_LED_2
 #define        LAMP_LED_ESTRAZIONE                 LAMP_LED_3
 #define		LAMP_LED_CONSERVAZIONE              Variabile_Appoggio

 #define        LAMP_LED_1                      	LampEnableLed.Bit0
 #define        LAMP_LED_2                    		LampEnableLed.Bit1
 #define        LAMP_LED_3                 			LampEnableLed.Bit2
 #define        LAMP_LED_4              			LampEnableLed.Bit3




// **************************************** Tasti ***************************************







// *************************************** Fotocellula *****************************************

#define   MAXCONI   5                   // numero di coni contati prima di salvarli
// #define   Set_NumeroConi   CONI_FIL // 40                   // numero di coni contati prima di salvarli
#define	  Num_Modelli  11;              // numero modelli per la tabella di programmazione di default all'AUTOSETUP
extern    int   NumeroConi;             // conta il numero dei coni erogati
extern    int   OldNumeroConi;

#define   TIMEOUT_PRODUZIONE ( 15 / Divisore_Timer )



// ********************************** Sonde ****************************************************


//_______________________________________________________________________________________________________
#define     TA_1                          TaValue                                     // TA-1
//-------------------------------------------------------------------------------------------------------
#define     NTC_1(offset)                 TempNTC(NtcValue,offset)                    // NTC-1
//-------------------------------------------------------------------------------------------------------
#define     PTC_1(offset,slope,pzt)       TempPTC(PtcValue,offset,slope,pzt)          // PTC-3
//-------------------------------------------------------------------------------------------------------




//_______________________________________________________________________________________________________

#define     NTC_OOO                40       // sarebbe 0
#define     NTC_CCC               950       // sarebbe 1023

#define     PTC_CCC               360       // corrispondente a -45 gradi
#define     PTC_OOO               960       // corrispondente a 110 gradi

//__________________________________________________________________________





extern  WORD ProbeValue[12];






// ******************************* Potenziometro Digitale **************************************

#define   WDP0    0x11                  // Write Data Pot 0:  XX01XX01
#define   WDP1    0x12                  // Write Data Pot 1   XX01XX10
#define   WDP2    0x13                  // Write Data Pot 0/1   XX01XX11

extern    UC      DataPot;              // Dato da scrivere nel Digital-Pot





// ***************************************** IN-LOGIC ******************************************

#define  IN_LOGIC_1   INP_I1            // => ingresso logico con pull-up => logica negata
#define  IN_LOGIC_2   INP_I2            // => ingresso logico con pull-up => logica negata
#define  IN_LOGIC_3   INP_I3            // => ingresso logico con pull-up => logica negata




// ***************************************** IN-24Vac *******************************************

extern volatile  UC   IN24_1;           // p2_5; ingresso logico con pull-up => logica negata
extern volatile  UC   IN24_4;           // p2_2; ingresso logico con pull-up => logica negata
extern volatile  UC   IN24_5;           // p5_6; ingresso logico con pull-up => logica negata






// ***************************************** OUT-TRIAC *****************************************

#define   TK2     OUT_TK1
#define   TK3     OUT_TK2
#define   TK14    OUT_K2
#define   TK10    OUT_K3
#define   TK11    OUT_K4
#define   TK6     OUT_K1



// ************************************ Altri Input ********************************************

extern UC ENABLE_V_OUT;          // portello macchina




// ************************************* Seriale ************************************************



#define     MAXLEN      36              // max n.ro di byte da mandare via seriale

struct StrRxData {
  UC Start;
  UC Len;
  UC Code;
  WORD Address;
  UC Data[MAXLEN];
  WORD CheckSum;
};
extern struct StrRxData RxData;

#define    STARTBYTE    0x3C

extern volatile struct ByteBit UartStat;
#define     MsgTxEnd      UartStat.Bit0
#define     MsgRxEnd      UartStat.Bit1
#define     RxDato        UartStat.Bit2

extern UC SerByte;
extern UC TxBuf[MAXLEN + 8];
extern UC ByteToSend;
extern UC ByteSended;

extern UC TurnoSariale;                 // per la funzione InviaSeriale()

#define   RQ_TX_LOG        0xA3         // ReQuest Tx Log

#define   RQ_TX_TABPROG    0xB3         // ReQuest Tx TABella di PROgrammaszione
#define   TX_TABPROG       0xB7         // Tx TABella di PROgrammaszione

#define   RQ_RX_TABPROG    0xC3         // ReQuest Rx TABella di PROgrammaszione
#define   RX_TABPROG       0xC7         // Rx TABella di PROgrammaszione



// ***************************** REAL-TIME-CLOCK su BUS-I�C ****************************************

extern volatile UC I2cStatus;

#define     NOACKNOW    0x02







// **************************** Strutture dati per l'orologio *************************************



// Definizione della struttura dati per l'orario da REAL-TIME-CLOCK ***** 7 BYTE ******
struct TimerData {
  UC Second;                            // Second counter
  UC Minute;                            // Minute counter
  UC Hour;                              // Hour counter
  UC DayWeek;                           // Day of Week counter
  UC DayMonth;                          // Day of Month counter
  UC Month;                             // Month
  UC Year;                              // Year
};

extern struct TimerData Ora;            // struttura dati per l'ora del REAL-TIME-CLOCK





// Definizione dela struttura dati per l'allarme da REAL-TIME-CLOCK ***** 3 BYTE *****
struct TimerAlarm {
  UC Minute;                            // Minute counter
  UC Hour;                              // Hour counter
  UC DayWeek;                           // Day of Week counter
};

extern struct TimerAlarm AllarmeOra;    // struttura dati per l'allarme del REAL-TIME-CLOCK




// ****************** Strutture dati per il controllo del corretto funzionamento dell'orologio ****************************



// Struttura utilizzata per il controllo dell'Orario
extern struct TimerData Campione;

// Indica quando viene sistemato l'orologio per un problema
extern UC ProblemiOrologio;

// Maschera dell'ultimo problema ( indica i campi sbagliati dell'orario )
extern UC UltimoProblema;

// Indica se abilitare il controllo sull'ora
extern UC AbilitaControlloOrologio;








// ************************ Struttura dati della tabella di programmazione ************************



#define DimTabProg   55              // numero dei campi della struttura StrTabProg da visualizzare (da HOT a ILI)

// Def della struttura dati dei parametri da salvare nel BLOCCO-A
//            (vedi tabella di programmazione nel manuale Carpigiani)
struct StrTabProg {
    WORD CheckSum;
    int EVC;                            // consistenza gelato
    int DIF;
    int DEL;
    int CYC;
    int EPT;                            // Trimmer HOT -> Digital-Pot
    int ORA;
    int MIN;
    int SEC;
    int WEK;
    int DAY;
    int MES;
    int YAR;
    int TEV;                            // TEV q (taratura automatica via temperatura)
    int TEC;                            // TEC q (taratura automatica via temperatura)
    int TGV;                            // TGV q (taratura automatica via temperatura)
    int TE1;                            // TE1 q (taratura automatica via temperatura)
    int TVM;                            // TeV m (taratura automatica via temperatura)
    int TCM;                            // TeC m (taratura automatica via temperatura)
    int TGM;                            // TGv m (taratura automatica via temperatura)
    int T1M;                            // Te1 m (taratura automatica via temperatura)
    int TM1;                            // Tev m (taratura manuale via parametro)
    int TM2;                            // Tec m (taratura manuale via parametro)
    int TM3;                            // Tgv m (taratura manuale via parametro)
    int TM4;                            // definizione macchina pastorizzante o non pastorizzante
    int PTC;
    int PTV;
    int DEF;
    int HOT;
    int CON;
    int IIC;
    int PST;
    int STO;
    int GAS;
    int ICE;
    int SPS;
    int WDY;
    int HOB;
    int TIR;
    int FAR;
    int TES;
    int RT;
    int PR;
    int A22;
    int FIL;
    int KBB;
    int HMP;
    int LMP;
    int MMP;
    int SMP;
    int HAG;
    int LAG;
    int MAG;
    int SAG;
    int LIV;                            // LIVello
    int ILI;                            // Isteresi LIvello
    // ----------------------------------------------------------------------------------
    int PZTEV;                          // Punto-Zero-TEV ( q a temparatura = 5 gradi )
    int PZTEC;                          // Punto-Zero-TEC ( q a temparatura = 5 gradi )
    int PZTGV;                          // Punto-Zero-TGV ( q a temparatura = 5 gradi )
    int PZTE1;                          // Punto-Zero-TE1 ( q a temparatura = 5 gradi )
};


// Variabili e flag per monitorare il corretto funzionamento dell'EEPROM
extern UC ProblemiWrEeProm;   // Flag Utilizzabile come segnale di Problemi con l'EeProm (Write)
extern UC ProblemiRdEeProm;   // Flag Utilizzabile come segnale di Problemi con l'EeProm (Read)
extern UC TentativiEeProm;    // Numero MAX di tentativi mai provati

// Struttura dati da salvare in EEPROM (tabella di programmazione)
extern struct StrTabProg TabProg;

// Struttura dati di default MOD.1
extern const struct StrTabProg TabProg_Mod1;

// Struttura dati di default MOD.2
extern const struct StrTabProg TabProg_Mod2;

// Struttura dati di default MOD.3
extern const struct StrTabProg TabProg_Mod3;

// Struttura dati di default MOD.4
extern const struct StrTabProg TabProg_Mod4;

// Struttura dati di default MOD.5
extern const struct StrTabProg TabProg_Mod5;

// Struttura dati di default MOD.6
extern const struct StrTabProg TabProg_Mod6;

// Struttura dati di default MOD.7
extern const struct StrTabProg TabProg_Mod7;

// Struttura dati di default MOD.8
extern const struct StrTabProg TabProg_Mod8;

// Struttura dati di default MOD.9
extern const struct StrTabProg TabProg_Mod9;

// Struttura dati di default MOD.10
extern const struct StrTabProg TabProg_Mod10;

// Struttura dati di default MOD.11
extern const struct StrTabProg TabProg_Mod11;

// Struttura dati di default MOD.12
extern const struct StrTabProg TabProg_Mod12;

// n.b.: viene salvata sul BLOCCO-A sul MICROCONTROLLORE









// ******************************** Struttura dati per il Black-Out ********************************


// Struttura dati da salvare per il Black-Out  ***** 23 byte *****
struct StrBlackOutData {
  //---------------------------------------------- (11 byte)
  WORD CheckSum;                        // 2 byte
  UC BlackOut;                          // 1 byte
  UC State;                             // 1 byte
  struct TimerData Ora;                 // 7 byte
  //---------------------------------------------- (12 byte)
  int NumConi;                          // 2 byte
  int ConiOggi;                         // 2 byte
  UC  Coni_LIV;                         // 1 byte
  UC  WASH_Days;                        // 1 byte
  UC  Pasto_End;                        // 1 byte
  UC  TappaPasto;                       // 1 byte
  int MP_Ore;                           // 2 byte
  int MA_Ore;                           // 2 byte
};
// n.b.: la struttura StrBlackOutData viene salvata all'indirizzo 0x00 della EEPROM,
//       cio� nella prima meta' della pag.0 ( spazio disponibile: 24 byte )

extern struct StrBlackOutData BlackOutData;

extern UC MinutiBlackOut;






// **************************** Struttura dati per gli eventi *************************************


#define   DIM_STREVENTO      32         //  Dimensione della struttura StrEvento

// memoria Flash con pagine da 256 byte => 8 strutture da 32 byte per pagina (ne rimangono non usati 8)
#define   EVENTI_X_PAGINA_FLASH   8     //  numero di eventi per pagina su Flash-Card

// memoria EEPROM con pagine da 64 byte => 2 strutture da 32 byte per pagina
#define   EVENTI_X_PAGINA_EEPROM  2     // numero di eventi per pagina su EEPROM


// Definizione della struttura per il salvataggio degli eventi  ****** 32 byte ******
struct StrEvento {
  WORD CheckSum;                        // 2 byte
  UC Stato;                             // 1 byte
  UC Allarme1;                          // 1 byte
  UC Allarme2;                          // 1 byte
  UC TappaPasto;                        // 1 byte
  SC TEV;                               // 1 byte
  SC TEC;                               // 1 byte
  SC TGV;                               // 1 byte
  SC TE1;                               // 1 byte
  struct TimerData Ora;                 // 7 byte
  UC MMV    :1;                         // 1 bit
  UC MV     :1;                         // 1 bit
  UC MP     :1;                         // 1 bit
  UC MA     :1;                         // 1 bit
  UC MC     :1;                         // 1 bit
  UC EVRC   :1;                         // 1 bit
  UC EVRV   :1;                         // 1 bit
  UC EVFC   :1;                         // 1 bit
  UC EVFV   :1;                         // 1 bit
  UC dummy0 :1;                         // 1 bit
  UC dummy1 :1;                         // 1 bit
  UC dummy2 :1;                         // 1 bit
  UC dummy3 :1;                         // 1 bit
  UC dummy4 :1;                         // 1 bit
  UC dummy5 :1;                         // 1 bit
  UC dummy6 :1;                         // 1 bit
  //---------------------------------------------- (19 byte)
  UC free0;                             // 1 byte
  UC free1;                             // 1 byte
  UC free2;                             // 1 byte
  UC free3;                             // 1 byte
  UC free4;                             // 1 byte
  UC free5;                             // 1 byte
  UC free6;                             // 1 byte
  int NumConi;                          // 2 byte
  int MP_Ore;                           // 2 byte
  int MA_Ore;                           // 2 byte
  //---------------------------------------------- (32 byte)
};
// n.b.: le strutture StrEvento su EEPROM partono dall'indirizzo 32(dec) = 20(hex)


extern struct StrEvento Evento;         // struttura dati per salvare l'evento




// Definizione della struttura dati da salvare nella pagina 0 della FLASH su Card esterna
struct StrPagina0 {
  WORD CheckSum;                        // 2 byte
  UC TipoScheda;                        // 1 byte
  WORD NumEvento;                       // 2 byte
};
// n.b.: la struttura StrPagina0 su EEPROM e' all'indirizzo 24(dec) = 18(hex)
// n.b.: la struttura StrPagina0 su FLASH-CARD e' all'indirizzo 00(dec) = 00(hex)


extern struct StrPagina0 Pagina0;       // struttura dati da salvare nella pagina 0

// Variabili usate in Produzione
extern UC  Ciclo_High;
