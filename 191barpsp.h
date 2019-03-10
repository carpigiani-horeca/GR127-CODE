/******************************************************************************************
 *                                                                                        *
 *               Definizioni relative alla macchina 191 Bar Pastorizzante                 *
 *                                                                                        *
 ******************************************************************************************/








// => commentare la #DEFINE per il funzionamento pastorizzante (con PTC);
// => togliere il commento per il funzionamento NON-pastorizzante (con NTC)
//_________________________________________________________________________
#define   NON_PASTORIZZANTE   1
// #define   NO_DIARY            1        // per abilitare NO_DIARY togliere il commento











// ***************************  Macchina a stati  *****************************************


#define   ON      1
#define   OFF     0

#define   TRUE    1
#define   FALSE   0
#define   true    1
#define   false   0

#define   YES   1
#define   NO    0




//________________________________________________________________________
// nomi dei CASE : S#_IN ( azioni all'ingresso ), S# ( azioni continuate )
//--------------------
#define   S0_IN     0
#define   S0        1
//--------------------
#define   S1_IN     10
#define   S1        11
#define   S1a_IN    12
#define   S1a       13
#define   S1b_IN    14
#define   S1b       15
//--------------------
#define   S2_IN     20
#define   S2        21
#define   S2a_IN    22
#define   S2a       23
//--------------------
#define   S3_IN     30
#define   S3        31
//--------------------
#define   S3a_IN     32
#define   S3a        33
//--------------------
#define   S4_IN     40
#define   S4        41
//--------------------
#define   S5_IN     50
#define   S5        51
#define   S5a_IN    52
#define   S5a       53
#define   S5b_IN    54
#define   S5b       55
//--------------------
#define   S6_IN     60
#define   S6        61
#define   S6b_IN    62
#define   S6b       63
//--------------------
#define   S7_IN     70
#define   S7        71
#define   S7b_IN    72
#define   S7b       73
//--------------------
#define   S8_IN     80
#define   S8        81
//--------------------
#define   S9_IN     90
#define   S9        91
//--------------------
#define   S10_IN    100
#define   S10       101
#define   S10b_IN   102
#define   S10b      103
//--------------------
#define   S11_IN    110
#define   S11       111
#define   S11a_IN   112
#define   S11a      113
//--------------------
#define   S12_IN    120
#define   S12       121
//--------------------
#define   S13_IN    130
#define   S13       131
//--------------------
#define   S14_IN    140
#define   S14       141
//--------------------
#define   S15_IN    150
#define   S15       151
//--------------------
#define   S16_IN    160
#define   S16       161
//--------------------
#define   S17_IN    170
#define   S17       171
//--------------------
#define   S18_IN    180
#define   S18       181
//--------------------
#define   S19_IN    190
#define   S19       191
//--------------------
#define   S20_IN    200
#define   S20       201
//--------------------
#define   S21_IN    210
#define   S21       211
//--------------------
#define   S22_IN    220
#define   S22       221
//--------------------
#define   S23_IN    230
#define   S23       231
//--------------------
#define   S24_IN    240
#define   S24       241
#define   S24a_IN   242
#define   S24a      243
//--------------------
#define   END       255
//--------------------



//______________________________
// Variabili relative agli stati
extern UC State;                        // stato della macchina attuale
extern UC BlackoutState;                 // stato della macchina attuale dopo BlackOut
extern UC OldState;                     // stato della macchine salvato in caso di Black-out

//___________________
// Funzione AUTOSETUP
extern UC AutosetupFlag;                // flag di avvenuto autosetup

//___________
// Stato STOP
extern UC StatoStop0;                   // stato per lo SWITCH-0 di STOP
extern UC StatoStop1;                   // stato per lo SWITCH-1 di STOP


//_________________
// Stato PRODUZIONE
#define   IstSTO              2         // isteresi di TabProg.STO
#define   FOTODELAY          10         // tempo minimo di permanenza sotto la fotocellula per contare un cono
#define   DelayHOT           40         // tempo di attesa tra la nuova impostazione del D-Pot e la lettura dell'HOT

//____________________
// Stato CONSERVAZIONE

//______________
// Stato PULIZIA

//______________________
// Stato PASTORIZZAZIONE
#define   IstPTV              2         // isteresi di TabProg.PTV
#define   IstPTC              2         // isteresi di TabProg.PTC
#define   IstHOB              5         // isteresi di TabProg.HOB


//_____________
// Stato SELECT
extern UC StatoSelect;                 // variabile di stato dello SWITCH SELECT


//_____________
// Monitoring
extern UC CountDownFlag;                // flag di abilitazione lettura Alarm_A
extern UC WASH_Days;
extern UC Pasto_End;
extern UC BlackOut;                     // flag che avverte se c'e' stato un black-out
extern int MP_Ore;                      // per contare le ore di funzionamento del motore MP
extern int Old_MP_Ore;
extern int MA_Ore;                      // per contare le ore di funzionamento del motore MA
extern int Old_MA_Ore;
extern UC MP_Minuti;
extern UC MA_Minuti;
extern UC Variabile_Appoggio;
extern UC Variabile_Appoggio_MV;
extern UC E_V_F_C;
extern UC E_V_F_V_delay;
extern UC E_V_F_V;
extern UC M_C;
// extern UC M_V;
extern UC M_V_Cabinet;

#define MAX_ORE_MOTORI  1               // n.ro di ore contate prima di salvare

// #define VERSIONE_TEST

#ifdef  VERSIONE_TEST
#define ORE_MP_TUBE     1
#define CONI_FIL        5
#else
#define ORE_MP_TUBE     30
#define CONI_FIL        TabProg.CYC // 40
#endif

//____________________________
// *** Variabili dei tasti ***
extern UC TastoSelezioneBreve;
extern UC TastoSelezioneLungo;
extern UC TastoSelezioneExtraLungo;
extern UC TastoStopBreve;
extern UC TastoStopLungo;
extern UC TastoOffBreve;
extern UC TastoOffLungo;

extern UC TastoSelezioneBreveMon;
extern UC TastoSelezioneLungoMon;
extern UC TastoSelezioneExtraLungoMon;
extern UC TastoStopBreveMon;
extern UC TastoStopLungoMon;

extern UC DisabilitaTastoStop;


// *** Variabili per la 191 ***
extern UC  LED_PASTORIZZAZIONE;
extern UC  LED_STOP;
extern UC  LED_PRODUZIONE;
extern UC  LED_CONSERVAZIONE;
extern UC  LED_ESTRAZIONE;
extern UC  LED_PULIZIA;
extern UC  LED_GUSTO_1;
extern UC  LED_GUSTO_2;
extern UC  LED_GUSTO_3;
extern UC  Flag_SC_1;
extern UC  Flag_SC_2;
extern UC  Flag_SC_3;
extern UC  BEEPER;
extern UC  E_V_S_3;
// extern UC  M_I_R;
//extern UC  Versione_Colore;


//______________________________________________________________________________________________
// *** Variabili per gli allarmi ***
extern UC  AL;
extern UC  AL2;
//extern UC  AL3;
// extern UC  AL_TE1;
extern UC  AL_T3_Prod;
extern UC  I_M_S_Filtrato;
extern UC  OK_Tempo;
extern UC  Coni_LIV;
extern UC  ConiLivFlag;
extern UC  WashDaysFlag;
//extern UC  PastoEndFlag;
extern UC  PastoEnd;
extern UC  WASH;
extern UC  WASH_Today;
extern UC  WASH_Days;
extern UC  Alt_HOB;
extern UC  Step_HOT;
//extern UC  Flag_TEC;
extern UC  PR_Counter;
//extern UC  Caldo_TGV;
// extern UC  M_V;
extern UC  M_V_H;
extern UC  MV_Enable;
extern UC  TappaPasto;

extern int ConiOggi;                                // numero dei coni erogati oggi
extern UC  Cicli_Fine_Freddo_Vasca;

//______________________________________________________________________________________________
// *** Variabili per il PWM_Ventola ***
extern UC PWM_MV_Periodo;
extern UC PWM_MV_Tempo_OFF;



//______________________________________________________________________________________________
// *** Stringhe per gli allarmi ***
#define   Str_Allarme1     c1            // Stringa di allarme 1
#define   Str_Allarme2     c2            // Stringa di allarme 2
#define   Str_Allarme3     c3            // Stringa di allarme 3


//______________________________________________________________________________________________
// *** LAmpeggio LED allarme ***
#define   LedAllarmeLampeggiante    DOT_1 = 1; LAMP_DOT_1 = 1
#define   LedAllarmeFisso           DOT_1 = 1; LAMP_DOT_1 = 0
#define   LedAllarmeSpento          DOT_1 = 0; LAMP_DOT_1 = 0

#define   Led_2_AllarmeLampeggiante    DOT_2 = 1; LAMP_DOT_2 = 1
#define   Led_2_AllarmeFisso           DOT_2 = 1; LAMP_DOT_2 = 0
#define   Led_2_AllarmeSpento          DOT_2 = 0; LAMP_DOT_2 = 0

#define   Led_3_AllarmeLampeggiante    DOT_3 = 1; LAMP_DOT_3 = 1
#define   Led_3_AllarmeFisso           DOT_3 = 1; LAMP_DOT_3 = 0
#define   Led_3_AllarmeSpento          DOT_3 = 0; LAMP_DOT_3 = 0

#define   LedAllarmeLampeggiante_2    if ( Str_Allarme1 == Null ) { DOT_1 = 1; LAMP_DOT_1 = 1; }
#define   LedAllarmeFisso_2           if ( Str_Allarme1 == Null ) { DOT_1 = 1; LAMP_DOT_1 = 0; }
#define   LedAllarmeSpento_2          if ( Str_Allarme1 == Null ) { DOT_1 = 0; LAMP_DOT_1 = 0; }

//______________________________________________________________________________________________
// *** Livelli ***
#define   Rref    10*1000                   // valore in ohm:  10 Kohm
#define   SogliaLivello                OHMtoLEVEL( TabProg.LIV )
#define   SogliaIsteresiLivello        OHMtoLEVEL( TabProg.LIV + TabProg.ILI )
//#define   SondaLivelloCoperta      ((SondaLivelloCoperta_1)&&(SondaLivelloCoperta_2))    // la sonda livello e' nel liquido
//#define   SondaLivelloScoperta     ((SondaLivelloScoperta_1)||(SondaLivelloScoperta_2))  // la sonda livello e' fuori dal liquido
//#define   SondaLivelloCoperta_1    (S_L <= SogliaLivello)           // la sonda livello e' nel liquido
//#define   SondaLivelloScoperta_1   (S_L >= SogliaIsteresiLivello)   // la sonda livello e' fuori dal liquido
//#define   SondaLivelloCoperta_2    (S_L_2 <= SogliaLivello)           // la sonda livello e' nel liquido
//#define   SondaLivelloScoperta_2   (S_L_2 >= SogliaIsteresiLivello)   // la sonda livello e' fuori dal liquido
//#define   Licolab                  (P_R_Japan <= SogliaLivello)           // la sonda livello e' nel liquido
//#define   P_R_A                    (P_R_Japan >= SogliaIsteresiLivello)   // la sonda livello e' fuori dal liquido


//______________________________________________________________________________________________
// *** HOT ***
#define   HOT_min     10



//______________________________________________________________________________________________
// *** Sonde in corto circuito e in circuito aperto ***
//-----------------------------------------------------
#ifdef NON_PASTORIZZANTE
//________________________________________________________
#define   T_E_V_CortoCircuito     NtcValue >= NTC_CCC
#define   T_E_V_CircuitoAperto    NtcValue <= NTC_OOO
//--------------------------------------------------------
/*#define   T_E_C_CortoCircuito     ProbeValue[1] >= NTC_CCC
#define   T_E_C_CircuitoAperto    ProbeValue[1] <= NTC_OOO*/
#define   T_E_C_CortoCircuito     PtcValue <= PTC_CCC
#define   T_E_C_CircuitoAperto    PtcValue >= PTC_OOO
//--------------------------------------------------------
//#define   T_G_V_CortoCircuito     ProbeValue[2] >= NTC_CCC
//#define   T_G_V_CircuitoAperto    ProbeValue[2] <= NTC_OOO
//--------------------------------------------------------
//#define   T_E_1_CortoCircuito     ProbeValue[3] >= NTC_CCC
//#define   T_E_1_CircuitoAperto    ProbeValue[3] <= NTC_OOO
//--------------------------------------------------------
//#define   T_E_2_CortoCircuito     ProbeValue[4] >= NTC_CCC
//#define   T_E_2_CircuitoAperto    ProbeValue[4] <= NTC_OOO
//________________________________________________________
//#define   T_0_6_CortoCircuito     ProbeValue[5] >= NTC_CCC
//#define   T_0_6_CircuitoAperto    ProbeValue[5] <= NTC_OOO
//________________________________________________________
#else
//________________________________________________________
//#define   T_E_V_CortoCircuito     ProbeValue[6] <= PTC_CCC
//#define   T_E_V_CircuitoAperto    ProbeValue[6] >= PTC_OOO
//--------------------------------------------------------
//#define   T_E_C_CortoCircuito     ProbeValue[7] <= PTC_CCC
//#define   T_E_C_CircuitoAperto    ProbeValue[7] >= PTC_OOO
//--------------------------------------------------------
//#define   T_G_V_CortoCircuito     ProbeValue[8] <= PTC_CCC
//#define   T_G_V_CircuitoAperto    ProbeValue[8] >= PTC_OOO
//--------------------------------------------------------
//#define   T_E_1_CortoCircuito     ProbeValue[9] <= PTC_CCC
//#define   T_E_1_CircuitoAperto    ProbeValue[9] >= PTC_OOO
//--------------------------------------------------------
//#define   T_0_5_CortoCircuito     ProbeValue[10] <= PTC_CCC
//#define   T_0_5_CircuitoAperto    ProbeValue[10] >= PTC_OOO
//________________________________________________________
//#define   T_0_6_CortoCircuito     ProbeValue[11] <= PTC_CCC
//#define   T_0_6_CircuitoAperto    ProbeValue[11] >= PTC_OOO
//________________________________________________________
#endif





// ________________________________________________________________________________________
// ************************** MRS2 ********************************************************

#define     M_V      			Variabile_Appoggio_MV // OUT_TK1
#define     M_V_Cab    			OUT_TK1
#define     M_P      			OUT_TK2
#define     T_T_A               OUT_K1
#define     T_T_C        		OUT_K2
#define     E_V_F_C_reale        		OUT_K3
#define     E_V_F_V_reale      	OUT_K4



// #define     E_V_F_V_delay      		E_V_F_Cabinet   //  TK11      // ElettroValvola Raffreddamento Vasca
#define     E_V_S_1      		Variabile_Appoggio // TK7
#define     M_V_reale    		Variabile_Appoggio   // Motore Ventilatore
#define     R_R_C 		 		Variabile_Appoggio   // TK3                  // Motore Ventilatore veloce
//#define     M_C_A        		  TK5                  // Motore Ventilatore veloce
//#define     M_P          		  TK4                  // Motore Pompa
//#define     M_M_V        		  TK1                  // Motore Miscelatore Vasca
//#define     E_V_S_2             TK13
//#define     BUZZER_EXT          TK7
//#define     E_V_R_C             TK8                  // ElettroValvola Riscaldamento Cilindro
//#define     E_V_R_V               TK9                  // ElettroValvola Riscaldamento Vasca
//#define     BEEPER       		  TK12                 // Beeper

// ________________________________________________________________________________________
// ************************** MRS3 ********************************************************

#define     R_T_C       IN24_5                // Rele Termico Motore Compressore
#define     P_T_M_A     FALSE  // IN24_4                // Pressostato
#define     P_R         (!INP_BK2)  // IN24_4                // Pressostato
// #define     P_T_M_A     (!IN_LOGIC_1)                // Protettore Termico Motore Agitatore
#define     I_M_S       (!INP_BK1) // (!IN_LOGIC_2)                // Protettore Termico Motore Agitatore
// #define     AL_INV      (!IN_LOGIC_3)                // Protettore Termico Motore Agitatore
#define     DOOR       !IN_LOGIC_3                // Protettore Termico Motore Agitatore

// N.B. gli IN_24 necessitano di negazioni perche' sono normanmente alimentati dato che le
//      elettrovalvole sono normalmente chiuse



// ________________________________________________________________________________________
// ************************** MRS4 ********************************************************

#ifdef NON_PASTORIZZANTE
//________________________________________________________________
#define     T_E_V     NTC_1(TabProg.TEV)   // Temperatura Miscela in Vasca
#define     T_E_C     (PTC_1(/*TabProg.TEC*/559,/*TabProg.TCM*/410,/*TabProg.PZTEC*/5) + TabProg.TEC)  // Termostato Cilindro

extern UC T_E_1;
extern UC T_E_2;

//#define     T_E_C     NTC_2(TabProg.LMP)   // Termostato Cilindro
//#define     T_G_V     NTC_3(TabProg.MMP)   // Termostato Vasca
//#define     T_E_1     NTC_4(TabProg.SMP)   // Termostato Evaporatore Cilindro 1
//#define     T_E_2     NTC_5(TabProg.SMP)   // Termostato Evaporatore Cilindro 2, stessa taratura di TE1
//________________________________________________________________
#else
//________________________________________________________________
//#define     T_E_V     PTC_1(TabProg.TEV,TabProg.TVM,TabProg.PZTEV)   // Temperatura Miscela in Vasca
//#define     T_E_C     PTC_2(TabProg.TEC,TabProg.TCM,TabProg.PZTEC)   // Termostato Cilindro
//#define     T_G_V     PTC_3(TabProg.TGV,TabProg.TGM,TabProg.PZTGV)   // Termostato Vasca per TGV < 0
//#define     T_E_1     PTC_4(TabProg.TE1,TabProg.T1M,TabProg.PZTE1)   // Termostato Evaporatore Cilindro 1
//________________________________________________________________
#endif


#define     S_F                 (!IN_LOGIC_1)       // Sensore Fotoelettrico ( negazione + pull-up )
#define     S_LIV               (IN_LOGIC_2)       // Sensore Fotoelettrico ( negazione + pull-up )
#define     AUTOTARATURAPOT     Variabile_Appoggio       // abilitazione autotaratura HOT

// ________________________________________________________________________________________
// ************************** MRS5 ********************************************************

//#define     S_L         LEVEL_1                 // Sonda Livello
//#define     S_L_2       LEVEL_2                 // Sonda Livello
#define     T_R_A       TA_1                    // Trasformantore Amperometrico 1
//#define     P_R_Japan   LEVEL_4               // Pressostato aria versione Japan


// ________________________________________________________________________________________
// ************************** CN3 *********************************************************

// #define     I_M_S      (!INP_BK1)  // ENABLE_V_OUT          // Interruttore Magnetico Sicurezza
                                             // 1 => aperto, attivo, portello aperto
                                             // 0 => chiuso, disattivo, portello chiuso

// ________________________________________________________________________________________
// ************************** CN8 *********************************************************





// ________________________________________________________________________________________
// *********************** Ridefinizioni **************************************************

#define     M_A         		 T_T_A                    // Motore Agitatore
#define     M_C_reale    		 T_T_C                    // Motore Compressore
#define     M_I_R    		     ( !S_F && !Prod_Per_Cons )                      // Micro Rubinetto
//#define     C_F                  T_0_6_CortoCircuito      // Micro Sciroppi
#define     H_O_T       		 TAtoHOT( TA_1 )          // HOT
#define     H_O_T_RT    		 TAtoHOT( TA1_RealTime )  // HOT Real-Time
//#define     Delta_HOT_TE1_TE2    ( ( ( ( T_E_1 - T_E_2 >= 5 ) || ( T_E_2 - T_E_1 >= 5 ) ) && (LEVEL_3 <= SogliaLivello) ) ? 10:0 )
