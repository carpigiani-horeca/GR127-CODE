/************************************************************************
 *                                                                      *
 *                    Funzioni della macchina a stati                   *
 *                                                                      *
 ************************************************************************/




#include <string.h>

#include "defs.h"
#include "prot.h"
#include "191barpsp.h"





//UC dddddurata;



/* ********************* Strutture Dati ***************************** */




// Struttura dati per la lettura dell'orario dal REAL-TIME-CLOCK
struct TimerData Ora;

// Struttura dati per l'allarme dal REAL-TIME-CLOCK
struct TimerAlarm AllarmeOra;


// Struttura dati relativa alla tabella di programmazione da salvare in EEPROM
struct StrTabProg TabProg;

// Struttura dati da salvare in caso di Black-Out
struct StrBlackOutData  BlackOutData;

// Struttura dati da salvare per il Salvataggio Eventi su Flash Esterna
struct StrEvento Evento;

// Struttura dati della pagina 0 della Flash Esterna
struct StrPagina0 Pagina0;



//-------------------------------------------------------------------------------------------------


// array di puntatori ai campi della struttura dati TabProg ( serve per scorrere le struttura )
int *const PtrTabProg[ DimTabProg ] = {
		&TabProg.EVC,
		&TabProg.DIF,
		&TabProg.DEL,
		&TabProg.CYC,
		&TabProg.EPT,
		&TabProg.ORA,
		&TabProg.MIN,
		&TabProg.SEC,
		&TabProg.WEK,
		&TabProg.DAY,
		&TabProg.MES,
		&TabProg.YAR,
		&TabProg.TEV,
		&TabProg.TEC,
		&TabProg.TGV,
		&TabProg.TE1,
		&TabProg.TVM,
		&TabProg.TCM,
		&TabProg.TGM,
		&TabProg.T1M,
		&TabProg.TM1,
		&TabProg.TM2,
		&TabProg.TM3,
		&TabProg.TM4,
		&TabProg.PTC,
		&TabProg.PTV,
		&TabProg.DEF,
		&TabProg.HOT,
		&TabProg.CON,
		&TabProg.IIC,
		&TabProg.PST,
		&TabProg.STO,
		&TabProg.GAS,
		&TabProg.ICE,
		&TabProg.SPS,
		&TabProg.WDY,
		&TabProg.HOB,
		&TabProg.TIR,
		&TabProg.FAR,
		&TabProg.TES,
		&TabProg.RT,
		&TabProg.PR,
		&TabProg.A22,
		&TabProg.FIL,
		&TabProg.KBB,
		&TabProg.HMP,
		&TabProg.LMP,
		&TabProg.MMP,
		&TabProg.SMP,
		&TabProg.HAG,
		&TabProg.LAG,
		&TabProg.MAG,
		&TabProg.SAG,
		&TabProg.LIV,
		&TabProg.ILI,
};


UC IsInTabProg [ DimTabProg ] = {
		YES,  // ---      hot
		YES,  // DIF      t01
		YES,  // DEL      t02
		0,  // CYC      t03
		YES,  // EPT      t04
		0,  // ORA      t05
		0,  // MIN      t06
		0,  // SEC      t07
		0,  // WEK      t08
		0,  // DAY      t09
		0,  // MES      t10
		0,  // YAR      t11
		YES,  // TEV      t12
		YES,  // TEC      t13
		0,  // TGV      t14
		0,  // TE1      t15
		0,  // TVM      --1
		0,  // TCM      --2
		0,  // TGM      --3
		0,  // T1M      --4
		0,  // MOD      --5
		0,  // TM2      t16
		0,  // TM3      t17
		0,  // TM4      t18
		0,  // PTC      t19
		0,  // PTV      t20
		0,  // DEF      t21
		YES,  // HOT      t22
		0,  // CON      t23
		0,  // IIC      t24
		0,  // PST      t25
		YES,  // STO      t26
		0,  // GAS      t27
		0,  // ICE      t28
		0,  // SPS      t29
		YES,  // WDY      t30
		YES,  // HOB      t31
		0,  // TIR      t32
		0,  // FAR      t33    38
		0,  // TES      t34    39
		0,  // RT       t35    40
		0,  // PR       t36    41
		0,  // A22      t37    42
		YES,  // FIL      t38    43
		YES,  // KBB      t39    44
		YES,  // MV_P     t40
		0,  // MV_Off   t41
		0,  // MMP      t42
		0,  // SMP      t43
		0,  // HAG      t44
		0,  // LAG      t45
		0,  // MAG      t46
		0,  // SAG      t47
		0,  // LIV      t48
		0,  // ILI      t49
};


// struttura dati di default MOD.1
const struct StrTabProg TabProg_Mod1 = {
		0, // chechsum
		// ----------------------------------------------------------------------------------
		120, // HOT
		10, // DIF
		35, // DEL
		20, // CYC
		135, // 135, // EPT
		1, // ORA
		0, // MIN
		0, // SEC
		1, // WEK
		1, // DAY
		1, // MES
		5, // YAR
		-4, // TEV                        // valore AD relativo a 5 gradi centigradi
		3, // TEC                        // valore AD relativo a 5 gradi centigradi
		551, // TGV                        // valore AD relativo a 5 gradi centigradi
		571, // TE1                        // valore AD relativo a 5 gradi centigradi
		414, // TVM                        // m*100 della retta
		410, // TCM                        // m*100 della retta
		380, // TGM                        // m*100 della retta
		378, // T1M                        // m*100 della retta
		1, // MOD                        // modello macchina
		3, // TM2                        // regolazione manuale del parametro m
		0, // TM3                        // versione Colore
		0, // TM4                        // definizione macchina pastorizzante o non pastorizzante
		110, // PTC
		1, // PTV
		3, // DEF
		120, // BELT
#ifdef NO_DIARY
		42, // CON
#else
		0, // CON
#endif
		3, // 7, // IIC
		60, // PST
		1, // 1, // STO
		1, // GAS  (da intendersi 1.0)
		1, // ICE
		5, // SPS
		15, // WDY
		70, // HOB
		500, // 500, // TIR  ( 500 -> 5 mSec )
		140, // FAR
		120, // TES
		1, // RT
		40, // PR
		1, // A22
		180, // FIL
		14, // KBB
		0, // MV_P
		0, // MV_Off
		0, // MMP
		0, // SMP
		0, // HAG
		0, // LAG
		0, // MAG
		0, // SAG
		50, // LIV
		10, // ILI
		// ----------------------------------------------------------------------------------
		5, // PZTEV                        // punto zero temperatura
		5, // PZTEC                        // punto zero temperatura
		5, // PZTGV                        // punto zero temperatura
		5, // PZTE1                        // punto zero temperatura
};






const int MinParametro[ DimTabProg ] = {   // valori MIN dei parametri
		70,   // HOT
		1,   // DIF
		1,   // DEL
		0,   // CYC
		1,   // EPT
		0,   // ORA
		0,   // MIN
		0,   // SEC
		1,   // WEK
		1,   // DAY
		1,   // MES
		0,   // YAR
		-20,   // TEV                         // temperatura min di taratura TEV a 5 gradi
		-20,   // TEC                         // temperatura min di taratura TEC a 5 gradi
		-20,   // TGV                         // temperatura min di taratura TGV a 5 gradi
		-20,   // TE1                         // temperatura min di taratura TE1 a 5 gradi
		50,   // TVM                         // temperatura min di taratura TVM a 65 gradi
		50,   // TCM                         // temperatura min di taratura TCM a 65 gradi
		-30,   // TGM                         // temperatura min di taratura TGM a -20 gradi
		-30,   // T1M                         // temperatura min di taratura T1M a -20 gradi
		-99,   // TM1                         // min variazione manuale di m della retta
		1,   // TM2                         // min variazione manuale di m della retta
		0,   // TM3                         // min variazione manuale di m della retta
		0,   // TM4                         // min variazione manuale di m della retta
		010,   // PTC
		0,   // PTV
		1,   // DEF
		10,   // BELT
#ifdef NO_DIARY
		1, // CON
#else
		0, // CON
#endif
		1,   // IIC
		10,   // PST
		1,   // STO
		0,   // GAS  (da intendersi 0.0)
		1,   // ICE
		0,   // SPS
		0,   // WDY
		20,   // HOB
		400,   // TIR
		60,   // FAR
		0,   // TES
		0,   // RT
		1,   // PR
		0,   // A22
		1,   // FIL
		4,   // KBB
		0,   // MV_P
		-20,   // MV_Off
		-20,   // MMP
		-20,   // SMP
		0,   // HAG
		0,   // LAG
		0,   // MAG
		0,   // SAG
		35,   // LIV
		0,    // ILI
};


const int MaxParametro[ DimTabProg ] = {   // valori MAX dei parametri
		120,   // HOT
		25,   // DIF
		300,   // DEL
		80,   // CYC
		250,   // EPT
		23,   // ORA
		59,   // MIN
		59,   // SEC
		7,   // WEK
		31,   // DAY
		12,   // MES
		99,   // YAR
		20,   // TEV                         // temperatura max di taratura TEV a 5 gradi
		20,   // TEC                         // temperatura max di taratura TEC a 5 gradi
		90,   // TGV                         // temperatura max di taratura TGV a 5 gradi
		90,   // TE1                         // temperatura max di taratura TE1 a 5 gradi
		70,   // TVM                         // temperatura max di taratura TEV a 65 gradi
		70,   // TCM                         // temperatura max di taratura TEC a 65 gradi
		0,   // TGM                         // temperatura max di taratura TGV a -20 gradi
		0,   // T1M                         // temperatura max di taratura TE1 a -20 gradi
		99,   // TM1                         // max variazione manuale di m della retta
		5,   // TM2                         // max variazione manuale di m della retta
		0,   // TM3                         // max variazione manuale di m della retta
		0,   // TM4                         // max variazione manuale di m della retta
		300,   // PTC
		1,   // PTV
		120,   // DEF
		120,   // BELT
#ifdef NO_DIARY
		42, // CON
#else
		3, // CON
#endif
		30,   // IIC
		120,   // PST
		7,   // STO
		1,   // GAS  (da intendersi 2.0)
		20,   // ICE
		30,   // SPS                         // 24 � fuori range e serve per non attivazione
		600,   // WDY
		100,   // HOB
		600,   // TIR
		600,   // FAR
		300,   // TES
		1,   // RT
		80,   // PR
		1,   // A22
		180,   // FIL
		15,   // KBB
		1,   // MV_P
		20,   // MV_Off
		20,   // MMP
		20,   // SMP
		65,   // HAG
		535,   // LAG
		59,   // MAG
		59,   // SAG
		120,   // LIV
		20,   // ILI
};





const UC *const Parametro[ DimTabProg ] = {
		"hot", // "HOT",
		"t01", // "dif",
		"t02", // "del",
		"t03", // "CyC",
		"t04", // "ept",
		"t05", // "OrA",
		"t06", // "min",
		"t07", // "SeC",
		"t08", // "wek",
		"t09", // "day",
		"t10", // "mes",
		"t11", // "yar",
		"t12", // "tev",
		"t13", // "teC",
		"t14", // "tgv",
		"t15", // "te1",
		"--1", // "tvm",
		"--2", // "tCm",
		"--3", // "tgm",
		"--4", // "t1m",
		"--5", // "tm1",       // lo uso sono in lettura per visualizzare il modello di machina all'accensione, � il *PtrTabProg[20]
		"t16", // "tm2",       // FREE
		"t17", // "tm3",       // lo uso per differenziare le versioni colore
		"t18", // "tm4",       // lo uso per differenziare le pastorizzanti dalle non pastorizzanti
		"t19", // "PtC",
		"t20", // "ptv",
		"t21", // "def",
		"t22", // "EVC",
		"t23", // "con",
		"t24", // "IIC",
		"t25", // "pst",
		"t26", // "stO",
		"t27", // "gas",
		"t28", // "ICE",
		"t29", // "sps",
		"t30", // "wdy",
		"t31", // "hob",
		"t32", // "tir",
		"t33", // "far",
		"t34", // "tes",
		"t35", // "rt ",
		"t36", // "pr ",
		"t37", // "a22",
		"t38", // "fil",
		"t39", // "kbb",
		"t40", // "hmp",
		"t41", // "lmp",
		"t42", // "mmp",
		"t43", // "smp",
		"t44", // "hag",
		"t45", // "lag",
		"t46", // "mag",
		"t47", // "sag",
		"t48", // "liv",
		"t49", // "ili",
};




//-------------------------------------------------------------------------------------------------











/********************************** Variabili per le uscite logiche ****************************/

UC  M_C;
UC  M_C_Cabinet;
// UC  M_V;
UC  M_V_Cabinet;
UC  Variabile_Appoggio;
UC  Variabile_Appoggio_MV;
UC  HysteresisHOT;






/*********************************** Funzioni per il Black-Out **********************************/


// Aggiorna la struttura BlackOutData prima del salvataggio su EEPROM
void SaveDataBlackOut ( UC State )
{
	if ( State == 1 || State == 2 || State == 4  /* LED_CONSERVAZIONE */ )   // impostazione del campo BlackOut
		BlackOutData.BlackOut = TRUE;
	else
		BlackOutData.BlackOut = FALSE;

	BlackOutData.State = State;                     // impostazione del campo State
	BlackOutData.Pasto_End = Pasto_End;             // salvataggio di Pasto_End
	BlackOutData.TappaPasto = TappaPasto;           // salvataggio di TappaPasto


	// Legge dal Real-Time-Clock
	TimerRdData( &Ora );

	BlackOutData.Ora.Second = Ora.Second;           // impostazione della sottostruttura Ora
	BlackOutData.Ora.Minute = Ora.Minute;
	BlackOutData.Ora.Hour = Ora.Hour;
	BlackOutData.Ora.DayWeek = Ora.DayWeek;
	BlackOutData.Ora.DayMonth = Ora.DayMonth;
	BlackOutData.Ora.Month = Ora.Month;
	BlackOutData.Ora.Year = Ora.Year;

} // END SaveDataBlackOut()





// Calcola la durata del black-out in minuti
BYTE DurataBlackOut ( struct TimerData *AfterBlackOut, struct TimerData *BeforeBlackOut )
{
	UC Days_in_Month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};  // anno non bisestile
	//                      Gen,Feb,Mar,Apr,Mag,Giu,Lug,Ago,Set,Ott,Nov,Dic

	WORD MinutiBlackOut;

	if ( !(BeforeBlackOut->Year % 4) && ( (BeforeBlackOut->Year % 100) || !(BeforeBlackOut->Year % 400) ) ) {
		Days_in_Month[1] = 29;              // anno bisestile
	}


	if ( AfterBlackOut->Year > BeforeBlackOut->Year ) {    // cambia l'anno
		if ( (12 - BeforeBlackOut->Month + AfterBlackOut->Month) > 1 )   // pi� di un mese
			return 201;
		else                                                             // meno di un mese
			if ( (31 - BeforeBlackOut->DayMonth + AfterBlackOut->DayMonth) > 1 )  // pi� di un giorno
				return 202;
			else {                                                                // meno di un giorno
				MinutiBlackOut = (((24 - BeforeBlackOut->Hour - 1) * 60) + 60 - BeforeBlackOut->Minute) +
						((AfterBlackOut->Hour * 60) + AfterBlackOut->Minute );
				if ( MinutiBlackOut > 120 )
					return 203;
				else
					return MinutiBlackOut;
			}
	} else {                                               // stesso anno
		if ( AfterBlackOut->Month - BeforeBlackOut->Month > 1 )       // pi� di un mese
			return 204;
		else
			if ( AfterBlackOut->Month - BeforeBlackOut->Month == 1 ) {    // un solo mese
				if ( (Days_in_Month[(BeforeBlackOut->Month)-1] - BeforeBlackOut->DayMonth + AfterBlackOut->DayMonth) > 1 )
					return 205;
				else
					if ( (Days_in_Month[(BeforeBlackOut->Month)-1] - BeforeBlackOut->DayMonth + AfterBlackOut->DayMonth) == 1 )
						MinutiBlackOut = (((24 - BeforeBlackOut->Hour - 1) * 60) + 60 - BeforeBlackOut->Minute) +
						((AfterBlackOut->Hour * 60) + AfterBlackOut->Minute );
					else
						if ( (Days_in_Month[(BeforeBlackOut->Month)-1] - BeforeBlackOut->DayMonth + AfterBlackOut->DayMonth) == 0 )
							MinutiBlackOut = ( 60 - BeforeBlackOut->Minute + AfterBlackOut->Minute +
									(AfterBlackOut->Hour - BeforeBlackOut->Hour - 1)*60 );
			} else
				if ( AfterBlackOut->Month - BeforeBlackOut->Month == 0 ) {    // stesso mese
					if ( AfterBlackOut->DayMonth - BeforeBlackOut->DayMonth > 1 )   // pi� di un giorno
						return 206;
					else
						if ( AfterBlackOut->DayMonth - BeforeBlackOut->DayMonth == 1 )
							MinutiBlackOut = (((24 - BeforeBlackOut->Hour - 1) * 60) + 60 - BeforeBlackOut->Minute) +
							((AfterBlackOut->Hour * 60) + AfterBlackOut->Minute );
						else
							if ( AfterBlackOut->DayMonth - BeforeBlackOut->DayMonth == 0 )
								MinutiBlackOut = ( 60 - BeforeBlackOut->Minute + AfterBlackOut->Minute +
										(AfterBlackOut->Hour - BeforeBlackOut->Hour - 1)*60 );
				}
		if ( MinutiBlackOut > 120 )
			return 207;
		else
			return MinutiBlackOut;
	}
} // END DurataBlackOut()

UC MinutiBlackOut;



// Estrae i dati dalla struttura BlackOutData e calcola il flag OK_Tempo
void ReadDataBlackOut ( void )
{
	// WORD MinutiBlackOut;

	SequentialReadEEPROM( 0x0000, (UC*)&BlackOutData, sizeof( struct StrBlackOutData ) );

	BlackOut = BlackOutData.BlackOut;     // impostazione della variabile BlackOut

	OldState = BlackOutData.State;        // impostazione della variabile OldState

	NumeroConi = BlackOutData.NumConi;    // impostazione delle variabili NumCOni
	OldNumeroConi = NumeroConi;

	ConiOggi = BlackOutData.ConiOggi;     // impostazione delle variabili ConiOggi

	WASH_Days = BlackOutData.WASH_Days;   // impostazione della variabile WASH_Days

	Coni_LIV = BlackOutData.Coni_LIV;     // impostazione della variabile Coni_LIV

	Pasto_End = BlackOutData.Pasto_End;

	TappaPasto = BlackOutData.TappaPasto;


	MP_Ore = BlackOutData.MP_Ore;         // impostazione della variabile MP_Ore
	Old_MP_Ore = MP_Ore;
	MA_Ore = BlackOutData.MA_Ore;         // impostazione della variabile MA_Ore
	Old_MA_Ore = MA_Ore;

	// Legge dal Real-Time-Clock
	TimerRdData( &Ora );

	MinutiBlackOut = DurataBlackOut( &Ora, &BlackOutData.Ora );
	//dddddurata = MinutiBlackOut;
	if ( ( T_E_V >= 50 ) && ( T_E_V <= 68 ) ) {
		if ( MinutiBlackOut >= 30 ) {
			OK_Tempo = FALSE;
		} else
			OK_Tempo = TRUE;
	}
	if ( ( T_E_V >= 15 ) && ( T_E_V <= 49 ) ) {
		if ( MinutiBlackOut >= 10 ) {
			OK_Tempo = FALSE;
		} else
			OK_Tempo = TRUE;
	}
	if ( ( T_E_V >= 10 ) && ( T_E_V <= 14 ) ) {
		if ( MinutiBlackOut >= 20 ) {
			OK_Tempo = FALSE;
		} else
			OK_Tempo = TRUE;
	}
	if ( T_E_V <= 9 ) {
		if ( MinutiBlackOut >= 2*60 ) {
			OK_Tempo = FALSE;
		} else
			OK_Tempo = TRUE;
	}

} // END ReadDataBlackOut()




// Salva la struttura BlackOut in EEPROM
void SalvaStatoMacchina ( UC State )
{
	SaveDataBlackOut( State );      // riempimento della struttura dati per black-out
	PageWriteEEPROM( 0x0000, (UC*)&BlackOutData, sizeof( struct StrBlackOutData ) );
}
// n.b.: la struttura StrBlackOutData viene salvata all'indirizzo 0 della EEPROM,
//       nella prima meta' della pag.0 ( spazio disponibile: 32 byte )




//-------------------------------------------------------------------------------------------------






/* *********** Funzioni per salvataggio eventi su Flash-Card o su EEPROM ************************* */



//BYTE BufferFLASH[256];                         // buffer di appoggio per la lettura/scrittura della Flash-Card


/*-----------------16/01/2008 16.33-----------------
 * irene gen08
 * --------------------------------------------------*/
// *** Procedura di lettura Pagina0 su Flash-Card
/*BYTE ReadPage0FlashCard ( void )
{
  ReadPageDF( 0x0000, (UC*)&Pagina0, sizeof( struct StrPagina0 ) );
  WaitBusyDF( 10 );
  return CheckSum( (UC*)&Pagina0, sizeof( struct StrPagina0 ) );
} */
// n.b.: la struttura Pagina0 su Flash-Card e' all'indirizzo 00(dec) = 00(hex)





// *** Procedura di lettura Pagina0 su EEPROM
BYTE ReadPage0EEPROM ( void )
{
	SequentialReadEEPROM( 0x0020, (UC*)&Pagina0, sizeof( struct StrPagina0 ) );
	return CheckSum( (UC*)&Pagina0, sizeof( struct StrPagina0 ) );
}
// n.b.: la struttura Pagina0 su EEPROM e' all'indirizzo 24(dec) = 18(hex)






// *** Procedura di scrittura Pagina0 su Flash-Card
void WritePage0FlashCard ( void )
{   // irene gen 08
	/*CheckSum( (UC*)&Pagina0, sizeof( struct StrPagina0 ) );             // calcola il check-sum della pagina
  memset( BufferFLASH, 0xFF, sizeof( BufferFLASH ) );                 // inizializza tutto il buffer con 0xFF
  WritePageDF( 0x0000, BufferFLASH, sizeof( BufferFLASH ) );          // iniazializza la Pagina-0 con 0xFF
  WaitBusyDF( 10 );
  WritePageDF( 0x0000, (UC*)&Pagina0, sizeof( struct StrPagina0 ) );  // copia in Pagina0 la struttura relativa
  WaitBusyDF( 10 );  */
}
// n.b.: la struttura Pagina0 su FLASH-CARD e' all'indirizzo 00(dec) = 00(hex)





// *** Procedura di scrittura Pagina0 su EEPROM
void WritePage0EEPROM ( void )
{
	CheckSum( (UC*)&Pagina0, sizeof( struct StrPagina0 ) );                 // calcola il check-sum della pagina
	PageWriteEEPROM( 0x0020, (UC*)&Pagina0, sizeof( struct StrPagina0 ) );  // copia in Pagina0 la struttura relativa
}
// n.b.: la struttura Pagina0 su EEPROM e' all'indirizzo 24(dec) = 18(hex)





// *** Procedura di reset Pagina0 su EEPROM
void ResetPage0EEPROM ( void )
{
	Pagina0.CheckSum = 0;
	Pagina0.TipoScheda = 0xBB;
	Pagina0.NumEvento = 0;
	PageWriteEEPROM( 0x0020, (UC*)&Pagina0, sizeof( struct StrPagina0 ) );  // copia in Pagina0 la struttura relativa
}
// n.b.: la struttura Pagina0 su EEPROM e' all'indirizzo 24(dec) = 18(hex)






// *** Procedura di lettura Pagina0 da usare all'avvio della macchina
void LeggiPagina0 ( void )
{
	/* if ( CheckFlashCard() ) {           // se c'e' la Flash esterna...
    E2UpdOn;                          // alimenta la Flash Esterna ( rimane sempre accesa )
    Wait10mSec( 10 );                 // attesa alimentazione stabile
    if ( !ReadPage0FlashCard() )      // se checksum della Pagina-0 e' NO-OK allora la memoria e' vergine o c'e' un errore
      Pagina0.NumEvento = 0;          // e quindi riparte a scrivere dall'inizio
  } else */{                            // alrimenti usa la EEPROM...
	  if ( !ReadPage0EEPROM() ) {     // se checksum della Pagina-0 e' NO-OK allora la memoria e' vergine o c'e' un errore
		  Pagina0.NumEvento = 0;        // e quindi riparte a scrivere dall'inizio
	  }
  }
}






UC TappaPasto;

UC Allarme1,Allarme2;


void TrovaAllarme ( void )
{
	Allarme1 = 0;
	Allarme2 = 0;

	if ( Str_Allarme1 != Null ) {
		if ( strcmp( Str_Allarme1, "A01" ) == 0 )
			Allarme1 = 1;
		if ( strcmp( Str_Allarme1, "A02" ) == 0 )
			Allarme1 = 2;
		if ( strcmp( Str_Allarme1, "A03" ) == 0 )
			Allarme1 = 3;
		if ( strcmp( Str_Allarme1, "A04" ) == 0 )
			Allarme1 = 4;
		if ( strcmp( Str_Allarme1, "A05" ) == 0 )
			Allarme1 = 5;
		if ( strcmp( Str_Allarme1, "A06" ) == 0 )
			Allarme1 = 6;
		if ( strcmp( Str_Allarme1, "A11" ) == 0 )
			Allarme1 = 11;
		if ( strcmp( Str_Allarme1, "A18" ) == 0 )
			Allarme1 = 18;
		if ( strcmp( Str_Allarme1, "A20" ) == 0 )
			Allarme1 = 20;
		if ( strcmp( Str_Allarme1, "OPN" ) == 0 )
			Allarme1 = 51;
		if ( strcmp( Str_Allarme1, "  x" ) == 0 )
			Allarme1 = 52;
	}
	if ( Str_Allarme2 != Null ) {
		if ( strcmp( Str_Allarme2, "A07" ) == 0 )
			Allarme2 = 7;
		if ( strcmp( Str_Allarme2, "A12" ) == 0 )
			Allarme2 = 12;
		if ( strcmp( Str_Allarme2, "A15" ) == 0 )
			Allarme2 = 15;
		if ( strcmp( Str_Allarme2, "A22" ) == 0 )
			Allarme2 = 22;
		if ( strcmp( Str_Allarme2, "A24" ) == 0 )
			Allarme2 = 24;
		if ( strcmp( Str_Allarme2, "A30" ) == 0 )
			Allarme2 = 30;
	}


}



// *** Procedura di scrittura eventi su Flash-Card o EEPROM
void SalvaEvento ( UC en_tappa, UC en_al_1, UC en_al_2 )
{
	WORD NumPage;
	WORD PageOffset;
	UC Stato;

	if ( State == 6 ) {
		Stato = 0;
	} else
		Stato = State;

	// Legge dal Real-Time-Clock
	TimerRdData( &Ora );

	// Cerca l'allarme attivo
	TrovaAllarme();

	// Riempimento della struttura con i valori aggiornati
	Evento.Stato = Stato;
	Evento.Allarme1 = ( Allarme1 * en_al_1 );
	Evento.Allarme2 = ( Allarme2 * en_al_2 );
	Evento.TappaPasto = ( TappaPasto * en_tappa );
	Evento.TEV = ( SC )T_E_V;
	Evento.TEC = ( SC )T_E_C;
	//Evento.TGV = ( SC )T_G_V;
	Evento.TE1 = ( SC )T_E_1;
	Evento.Ora.Second = Ora.Second;
	Evento.Ora.Minute = Ora.Minute;
	Evento.Ora.Hour = Ora.Hour;
	Evento.Ora.DayWeek = Ora.DayWeek;
	Evento.Ora.DayMonth = Ora.DayMonth;
	Evento.Ora.Month = Ora.Month;
	Evento.Ora.Year = Ora.Year;
	//Evento.MMV = M_M_V;
	Evento.MV = M_V;
	//Evento.MP = M_P;
	Evento.MA = M_A;
	Evento.MC = M_C;
	//Evento.EVRC = E_V_R_C;
	//Evento.EVRV = E_V_R_V;
	Evento.EVFC = E_V_F_C;
	Evento.EVFV = E_V_F_V;
	Evento.dummy0 = 0;
	Evento.dummy1 = 0;
	Evento.dummy2 = 0;
	Evento.dummy3 = 0;
	Evento.dummy4 = 0;
	Evento.dummy5 = 0;
	Evento.dummy6 = 0;
	Evento.free0 = 0xFF;
	Evento.free1 = 0xFF;
	Evento.free2 = 0xFF;
	Evento.free3 = 0xFF;
	Evento.free4 = 0xFF;
	Evento.free5 = 0xFF;
	Evento.free6 = 0xFF;
	Evento.NumConi = NumeroConi;
	Evento.MP_Ore = MP_Ore;
	Evento.MA_Ore = MA_Ore;


	// Calcolo del chksum della struttura e lo sovrascrive
	CheckSum( (UC*)&Evento, sizeof( struct StrEvento ) );

	{

		// ( n.ro Evento / eventi x pagina ) = n.ro Pagina
		// max n.ro di pagine = 512 - pag0 = 511
		NumPage = (( Pagina0.NumEvento / EVENTI_X_PAGINA_EEPROM ) % 511 ) + 1;       // la Pagina-0 viene saltata
		PageOffset = ( Pagina0.NumEvento % EVENTI_X_PAGINA_EEPROM ) * DIM_STREVENTO;

		PageWriteEEPROM( (NumPage*64)+PageOffset, (UC*)&Evento, sizeof( struct StrEvento ) );

		Pagina0.TipoScheda = 0xBB;
		Pagina0.NumEvento ++;
		WritePage0EEPROM();                 // aggiorna la pagina-0 con anche il checksum

	}

} // END SalvaEvento()




/*********************************** Funzioni per l'accensione **********************************/



UC AutosetupFlag;



// *** AutoSetup ( carica tabella di default e imposta il REAL-TIME-CLOCK )
void AutoSetup ( void )
{
	UC modello = 1;

	Cnt1Sec = 5;

	StringVal(" UP");
	Wait100mSec( 20 );

	Play_Buzzer ( HIGH, 20 );

	/* while ( Cnt1Sec )
  {
  	StringVal(" UP");
	DoIdle();
  }*/

	NewKey = 0;
	Cnt1Sec = 0;    // tolgo le visualizzazioni e scelte modello iniziali !!!!!!!!!!!!

	while ( Cnt1Sec )
	{
		DigitValProg(modello);
		DoIdle();
		if ( NewKey && ( IS_KEY( KEY_P2 ) ) )
		{
			NewKey = 0;
			modello = (modello + 1);
			if ( modello == 13) modello =  1;
			Cnt1Sec = 5;
		}
	}


	switch ( modello ) {
	case 1  : memcpy( &TabProg, &TabProg_Mod1,  sizeof( TabProg ) );    break;
	/* case 2  : memcpy( &TabProg, &TabProg_Mod2,  sizeof( TabProg ) );    break;
          case 3  : memcpy( &TabProg, &TabProg_Mod3,  sizeof( TabProg ) );    break;
          case 4  : memcpy( &TabProg, &TabProg_Mod4,  sizeof( TabProg ) );    break;
          case 5  : memcpy( &TabProg, &TabProg_Mod5,  sizeof( TabProg ) );    break;
          case 6  : memcpy( &TabProg, &TabProg_Mod6,  sizeof( TabProg ) );    break;
          case 7  : memcpy( &TabProg, &TabProg_Mod7,  sizeof( TabProg ) );    break;
          case 8  : memcpy( &TabProg, &TabProg_Mod8,  sizeof( TabProg ) );    break;
          case 9  : memcpy( &TabProg, &TabProg_Mod9,  sizeof( TabProg ) );    break;
          case 10 : memcpy( &TabProg, &TabProg_Mod10, sizeof( TabProg ) );    break;
          case 11 : memcpy( &TabProg, &TabProg_Mod11, sizeof( TabProg ) );    break;
          case 12 : memcpy( &TabProg, &TabProg_Mod12, sizeof( TabProg ) );    break;*/
	}


	WriteEeprom( &TabProg );                                    // salva la struttura TabProg in FLASH ( RAM -> FLASH )


	/*  // reinizializzazione orologio
  Ora.Second = TabProg.SEC;
  Ora.Minute = TabProg.MIN;
  Ora.Hour = TabProg.ORA;
  Ora.DayWeek = TabProg.WEK;
  Ora.DayMonth = TabProg.DAY;
  Ora.Month = TabProg.MES;
  Ora.Year = TabProg.YAR;
  TimerWrData( &Ora );                                        // salva l'orario di default sul REAL-TIME-CLOCK
	 */

	// reinizializzazione flags
	// Coni_LIV = 0;                   // reinizializzazione Coni_LIV
	// WASH_Days = TabProg.WDY;        // reinizializzazione WASH_Days
	// Pasto_End = TRUE;               // reinizializzazione Pasto_End
	// SalvaPastoEnd();


	// ResetPage0EEPROM();                                         // reinizializza la pagina-0 dell'EEPROM

	AutosetupFlag = TRUE;                                       // ricorda che e' stato eseguito l'autosetup

} // END AutoSetup()





// *** Azzeramento Coni
void AzzeraConi ( void )
{
	SequentialReadEEPROM( 0x0000, (UC*)&BlackOutData, sizeof( struct StrBlackOutData ) );
	BlackOutData.NumConi = 0;
	PageWriteEEPROM( 0x0000, (UC*)&BlackOutData, sizeof( struct StrBlackOutData ) );

} // END AzzeraConi()




// *** Azzeramento conteggio ore dei motori
void AzzeraMotori ( void )
{
	SequentialReadEEPROM( 0x0000, (UC*)&BlackOutData, sizeof( struct StrBlackOutData ) );
	// if ( !M_I_R ) {
	BlackOutData.MP_Ore = 0;
	MP_Ore = 0;
	// } else {
	//		BlackOutData.MA_Ore = 0;
	//		MA_Ore = 0;
	//	}
	PageWriteEEPROM( 0x0000, (UC*)&BlackOutData, sizeof( struct StrBlackOutData ) );
} // END AzzeraMotori()





// *** Azzeramento-Contaconi / Autosetup
void Setup ( void )
{
	// Pulisce il display
	StringVal( "   " );

	// Beep che segnala l'accensione
	Play_Buzzer ( HIGH, 200 );

	// Attesa per rilevare la pressione di tasto con antirimbalzo tramite DoIdle()
	Wait10mSec( 20 );

	//__________________________________________________________________________________
	// *************** Gestione dei tasti (pulsanti): P1, P2, P1+P2 ********************

	if ( NewKey ) {                     // Se e' stato premuto un tasto...

		//____________________________________________________________________
		// ********************** Gestione del tasto P1 **********************

		if ( IS_KEY( KEY_P1 ) ) {       // Se il tasto premuto e' P1...
			Play_Buzzer ( HIGH, 20 );
			NewKey = 0;
			while ( IS_KEY( KEY_P1 ) ) {  // Controllo del tempo di pressione del P1
				DoIdle();
				if ( PressCnt >= 500 )  break;
			}
			if ( IS_KEY( KEY_P1 ) ) {     // Se la pressione del P1 e' prolungata...

				// *** Azioni eseguite con P1 premuto per un tempo lungo
				AzzeraConi() ;            // azzeramento del conteggio dei coni erogati
				// AzzeraMotori();           // azzeramento del conteggio delle ore dei motori

			} else {                      // Se la pressione del P1 e' breve...

				// *** Azioni eseguite con P1 premuto per un tempo breve
				;

			}
		} // END gestore tasto P1 ********************************************

		//____________________________________________________________________
		// ********************** Gestione del tasto P2 **********************

		if ( IS_KEY( KEY_P2 ) ) {       // Se il tasto premuto e' P2...
			Play_Buzzer ( HIGH, 20 );
			NewKey = 0;
			while ( IS_KEY( KEY_P2 ) ) {  // Controllo del tempo di pressione del P2
				DoIdle();
				if ( PressCnt >= 500 )   break;
			}
			if ( IS_KEY( KEY_P2 ) ) {     // Se la pressione del P2 e' prolungata...

				// *** Azioni eseguite con P2 premuto per un tempo lungo
				AutoSetup();              // caricamento dei valori di default della tabella di programmazione

			} else {                      // Se la pressione del P2 e' breve...

				// *** Azioni eseguite con P2 premuto per un tempo breve
				;

			}
		} // END gestore tasto P2 ********************************************

	} // END gestione della pressione di un tasto ****************************


} // END Setup()









// *** Il display all'accensione
void TestDisplay ( void )
{
	UC unita, decine, centinaia, migliaia, decimigliaia;
	UC NumConi;

	// Accende tutto
	AllDigitLedOn();
	Wait100mSec( 10 );

	// Attiva il  lampeggio
	LampAllDigitLedOn();
	Wait100mSec( 10 );

	// Spegne tutto
	AllDigitLedOff();

	// Disattiva il lampeggio
	LampAllDigitLedOff();
	Wait100mSec( 10 );

	// Visualizza versione
	StringVal("V06");        //versione_sw
	DOT_3 = 1;
	Wait100mSec( 20 );
	StringVal("   ");
	DOT_3 = 0;
	Wait100mSec( 10 );
	StringVal("XXX");
	Wait100mSec( 20 );
	StringVal("xxx");
	Wait100mSec( 20 );
	//	  StringVal("Mod");
	//	  Wait100mSec( 15 );
	//	  DigitVal(*PtrTabProg[20]);
	//	  Wait100mSec( 15 );

} // END TestDisplay()




//-------------------------------------------------------------------------------------------------









/********************************* Funzioni per la programmazione ****************************/



// Legge la struttura TabProg dalla memoria EEPROM-virtuale
void CaricaTabellaProgrammazione ( void )
{

	if ( ReadEeprom( &TabProg ) == 0 ) {       // se checksum NO ok...

		StringVal("err");
		Wait100mSec( 10 );
		StringVal("   ");
		Wait100mSec( 10 );

		AutoSetup();                                // caricamento dei valori di default della tabella di programmazione
		// AzzeraConi() ;                              // azzeramento del conteggio dei coni erogati

		StringVal("---");
		Wait100mSec( 10 );
		StringVal("   ");
		Wait100mSec( 10 );

	}

	//  Impostazione DIGITAL-POTENTIOMETER
	DataPot = TabProg.EPT;                     //  scrittura del parametro di taratura nel Digital-Pot
	WriteDigitalPot0( DataPot );

} // END CaricaTabellaProgrammazione()








UC alterna;  // variabile usata in ProgrammazioneUtente() ed in ProgrammazioneTecnico()


// *** Programmazione utente
void ProgrammazioneUtente ( void )
{

	UC i = 27;
	UC modificato = 0;                        // indica se viene modificata la struttura TabProg

	while ( ( Cnt1Sec ) && ( /* Versione_Colore ||  ( !M_I_R ) */ TRUE ) ) {                       // per 10 Sec rimane in programmazione utente

		if ( Cnt10mSec == 0 ) {               // ogni Sec visualizza alternativamente nome e valore del parametro
			if ( alterna ) {                    // visualizza il nome del paramero
				if ( i == 27 )
					StringVal("hot");
				//		    else StringVal( Parametro[i] );
				else if ( i == 3 )
					StringVal("X  ");
				else if ( i == 28 )
					StringVal("  x");
				else if ( i == 32 )
					StringVal(" X ");

				alterna = !alterna;
				Cnt10mSec = 100;
			} else {                            // visualizza il valore del parametro
				if ( i == 32 )  {          // passo SPS
					if ( *PtrTabProg[i] == 0 )
						StringVal( " no" );    // scrive "no" invece di "24"
					else
						StringVal( "YES" );
				} else
					DigitValProg( *PtrTabProg[i] );
				alterna = !alterna;
				Cnt10mSec = 100;
			}
		}

		DoIdle();

		//___________________________________________________________________________________
		// ************************** Gestione dei tasti (pulsanti): P1, P2, P1+P2 **********
		if ( NewKey ) {                         // Se e' stato premuto un tasto...

			//____________________________________________________________________
			// ********************** Gestione del tasto P2 **********************

			if ( IS_KEY( KEY_P2 ) ) {             // Se il tasto premuto e' P2...
				Play_Buzzer ( HIGH, 20 );
				NewKey = 0;
				while ( IS_KEY( KEY_P2 ) ) {        // Controllo del tempo di pressione del P2
					DoIdle();
					if ( PressCnt >= 100 )   break;
				}
				if ( IS_KEY( KEY_P2 ) ) {           // Se la pressione del P2 e' prolungata...

					// *** Azioni eseguite con P2 premuto per un tempo lungo

					Speed_100mSec = 30;
					// incrementa il parametro HOT in modo continuo ogni 250 mSec inizialmente e ogni 100 mSec dopo 3 Sec
					while ( IS_KEY( KEY_P2 ) ) {
						DoIdle();
						DigitValProg( *PtrTabProg[i] );
						(*PtrTabProg[i]) ++;
						if ( *PtrTabProg[i] > MaxParametro[i] )
							*PtrTabProg[i] = MinParametro[i];
						if ( Speed_100mSec )
							Wait10mSec( 25 );
						else
							Wait10mSec( 10 );
						Cnt1Sec = 10;
						modificato = 1;
					}

				} else {                            // Se la pressione del P2 e' breve...

					// *** Azioni eseguite con P2 premuto per un tempo breve

					// incrementa il parametro HOT di 1
					alterna = 0;
					(*PtrTabProg[i]) ++;
					if ( *PtrTabProg[i] > MaxParametro[i] )
						*PtrTabProg[i] = MinParametro[i];
					Cnt1Sec = 10;
					modificato = 1;

				}

			} // END gestore tasto P2 ********************************************

			//____________________________________________________________________
			// ********************** Gestione del tasto P1 **********************

			if ( IS_KEY( KEY_P1 ) ) {             // Se il tasto premuto e' P1...
				NewKey = 0;
				while ( IS_KEY( KEY_P1 ) ) {        // Controllo del tempo di pressione del P1
					DoIdle();
					if ( PressCnt >= 100 )   break;
				}
				if ( IS_KEY( KEY_P1 ) ) {           // Se la pressione del P1 e' prolungata...

					// *** Azioni eseguite con P1 premuto per un tempo lungo
					;

				} else {                            // Se la pressione del P1 e' breve...

					// *** Azioni eseguite con P1 premuto per un tempo breve

					alterna = 1;
					if ( i == 27 ) i = 28; // 3;
					// else if ( i == 3 ) i = 28;
					else if ( i == 28 ) i = 32;
					else if ( i == 32 ) i = 27;
					Cnt10mSec = 0;
					Cnt1Sec = 10;
					// break;              // esce dalla programmazione utente

				}

			} // END gestore tasto P1 ********************************************


		} // END gestione della pressione di un tasto ***************************************
		//___________________________________________________________________________________

	} // end while

	TastoOffBreve = FALSE;
	if ( modificato ) {

		AllDigitLedOff();
		WaitmSec( 50 );
		WriteEeprom( &TabProg );             // salva la struttura TabProg in EEPROM solo se modificata
	}

} // END ProgrammazioneUtente()





#define i_offset  11                    // numero del passo TEV - 1


// #ifdef  NON_PASTORIZZANTE
#define PROBE_PN_7  0               // NTC-1
#define PROBE_PN_8  1               // NTC-2
#define PROBE_PN_9  2               // NTC-3
#define PROBE_PN_10 3               // NTC-4
#define PROBE_PN_11 4               // NTC-3
#define PROBE_PN_12 5               // NTC-4
// #else
#define PROBE_PN_1  6               // PTC-1
#define PROBE_PN_2  7               // PTC-2
#define PROBE_PN_3  8               // PTC-3
#define PROBE_PN_4  9               // PTC-4
#define PROBE_PN_5 10               // PTC-5
#define PROBE_PN_6 11               // PTC-6
// #endif




// Procedura di taratura PTC/NTC
UC SetTemp_PNTC ( UC i )
{

	int Temp;

	switch ( i ) {

	//----------------------------------------------
	/* case i_offset+1 :                           // *** TEV
      Temp = T_E_V + 1;
      if ( Temp > MaxParametro[i] )
        Temp = MinParametro[i];
      TabProg.TEV = ProbeValue[PROBE_PN_1];      // q = AD
      TabProg.PZTEV = Temp;             // temperatura alla quale viene impostato q
      break; */

	case i_offset+2 :                           // *** TEC
	Temp = T_E_C + 1;
	if ( Temp > MaxParametro[i] )
		Temp = MinParametro[i];
	TabProg.TEC = ProbeValue[PROBE_PN_2];      // q = AD
	TabProg.PZTEC = Temp;             // temperatura alla quale viene impostato q
	break;

	case i_offset+3 :                           // *** TGV
	Temp = /*T_G_V*/T_E_C + 1;
	if ( Temp > MaxParametro[i] )
		Temp = MinParametro[i];
	TabProg.TGV = ProbeValue[PROBE_PN_3];      // q = AD
	TabProg.PZTGV = Temp;             // temperatura alla quale viene impostato q
	break;

	case i_offset+4 :                           // *** TE1
	Temp = T_E_1 + 1;
	if ( Temp > MaxParametro[i] )
		Temp = MinParametro[i];
	TabProg.TE1 = ProbeValue[PROBE_PN_4];      // q = AD
	TabProg.PZTE1 = Temp;             // temperatura alla quale viene impostato q
	break;

	//----------------------------------------------
	case i_offset+5 :                           // *** TVM
	Temp = T_E_V + 1;                 // temperature positive
	if ( Temp > MaxParametro[i] )
		Temp = MinParametro[i];
	if ( ProbeValue[PROBE_PN_1] > TabProg.TEV )
		TabProg.TVM = (long)(ProbeValue[PROBE_PN_1] - TabProg.TEV) * 100 / (Temp - TabProg.PZTEV);  // calcolo di m
	TabProg.TM1 = TabProg.TVM - TabProg_Mod1.TVM;
	break;

	case i_offset+6 :                           // *** TCM
	Temp = T_E_C + 1;                 // temperature positive
	if ( Temp > MaxParametro[i] )
		Temp = MinParametro[i];
	if ( ProbeValue[PROBE_PN_2] > TabProg.TEC )
		TabProg.TCM = (long)(ProbeValue[PROBE_PN_2] - TabProg.TEC) * 100 / (Temp - TabProg.PZTEC);  // calcolo di m
	TabProg.TM2 = TabProg.TCM - TabProg_Mod1.TCM;
	break;

	case i_offset+7 :                           // *** TGM
	Temp = /*T_G_V*/T_E_C + 1;                 // temperature negative
	if ( Temp > MaxParametro[i] )
		Temp = MinParametro[i];
	if ( ProbeValue[PROBE_PN_3] < TabProg.TGV )
		TabProg.TGM = (long)(TabProg.TGV - ProbeValue[PROBE_PN_3]) * 100 / (TabProg.PZTGV - Temp);  // calcolo di m
	TabProg.TM3 = TabProg.TGM - TabProg_Mod1.TGM;
	break;

	case i_offset+8 :                           // *** T1M
	Temp = T_E_1 + 1;                 // temperature negative
	if ( Temp > MaxParametro[i] )
		Temp = MinParametro[i];
	if ( ProbeValue[PROBE_PN_4] < TabProg.TE1 )
		TabProg.T1M = (long)(TabProg.TE1 - ProbeValue[PROBE_PN_4]) * 100 / (TabProg.PZTE1 - Temp);  // calcolo di m
	TabProg.TM4 = TabProg.T1M - TabProg_Mod1.T1M;
	break;

	//----------------------------------------------

	//----------------------------------------------
	default:
		return 0;
	}
	return 1;

}






int Appoggio_Ore;



// *** Programmazione Tecnico
void ProgrammazioneTecnico ( void )
{

	UC i = 1;                             // indice del parametro selezionato
	UC modificato_tabella = 0;            // indica se viene modificata la struttura TabProg
	UC modificato_orario = 0;             // indica se viene modificato l'orario
	UC salva_orario_E2 = 0;
	char const giorni_settimana[7][4] = {"MON","TUE","WED","THU","FRI","SAT","SUN"};
	UC giorno;

	// ciclo interno di programmazione tecnico
	while ( ( Cnt1Sec ) && ( /* Versione_Colore || ( !M_I_R ) */ TRUE ) ) { // Cnt1Sec � per uscire dalla prog. tecnico dopo 10 sec di inattivit�

		if ( i == 4 ) {               // i=5,6,7,8,9,10,11 => parametri dell'orario
			//______________________________________________________________________________
			// Legge l'ora dal REAL-TIME-CLOCK tramite I2C e imposta la struttura TabProg
			TimerRdData( &Ora );
			TabProg.SEC = Ora.Second;
			TabProg.MIN = Ora.Minute;
			TabProg.ORA = Ora.Hour;
			TabProg.WEK = Ora.DayWeek;
			TabProg.DAY = Ora.DayMonth;
			TabProg.MES = Ora.Month;
			TabProg.YAR = Ora.Year;
		}

		// visualizzazioni dei valori dei parametri della tabella di programmazione
		if ( Cnt10mSec == 0 ) {           // Cnt10mSec � per alternare il nome del parametro col suo valore
			if ( alterna ) {                // *** visualizzazione del nome del parametro ***
				StringVal( Parametro[i] );
				Led_2_AllarmeSpento;           // spengo il puntino dei decimali
				alterna = !alterna;
				Cnt10mSec = 100;
			} else {                        // *** visualizzazione del valore del parametro ***
				if ( i == i_offset+1 )
					DigitValProg( Cel_Far_Delta(TabProg.TEV) );// DigitValProg( Cel_Far(T_E_V) );
				else  if ( i == i_offset+2 )
					DigitValProg( Cel_Far_Delta(TabProg.TEC) );// DigitValProg( Cel_Far(T_E_C) );
				else  if ( i == i_offset+3 )
					DigitValProg( Cel_Far(/*T_G_V*/T_E_C) );
				else  if (i == i_offset+4 )
					DigitValProg( Cel_Far(T_E_1) );
				else
					if ( i == i_offset+5 )
						DigitValProg( Cel_Far(T_E_V) );
					else  if ( i == i_offset+6 )
						DigitValProg( Cel_Far(T_E_C) );
					else  if ( i == i_offset+7 )
						DigitValProg( Cel_Far(/*T_G_V*/T_E_C) );
					else  if (i == i_offset+8 )
						DigitValProg( Cel_Far(T_E_1) );
					else

						if ( /*i == 32*/ FALSE ) { //Led_2_AllarmeFisso;
							DigitValProg( *PtrTabProg[i] );
						}
						else

							if ( i == 34 )  {          // passo SPS
								if ( *PtrTabProg[i] == 24 )
									StringVal( " no" );    // scrive "no" invece di "24"
								else
									DigitValProg( *PtrTabProg[i] );
							}
							else if ( ( /*( i == 35 ) ||*/ ( ( ( i >= 40 ) && ( i <= 40 ) ) || ( ( i >= 42 ) && ( i <= 42 ) ) ) )
									|| ( i == 22 ) || ( i == 23 ) || ( i == 25 ) || ( i == 32 ) || ( i == 45 ) )
							{
								if ( *PtrTabProg[i] == 0 )
									StringVal( " no" );    // scrive "no" invece di "0"
								else
									StringVal( "YES" );    // scrive "yes" invece di "1"
							}
							else if ( i == 46 ) { Appoggio_Ore=(MA_Ore%1000); DigitValProg(Appoggio_Ore) ; }

							else if ( /*( i == 24 )||( i == 25 )||*/( i == 26 )||( i == 31 )||( i == 33 ) ) {
								DigitValProg( Cel_Far(*PtrTabProg[i]) );
							}
							else if ( ( i == 27 )||( i == 29 )/*||(( i >= 45 )||( i <= 48 ))*/ ) {
								DigitValProg( Cel_Far_Delta(*PtrTabProg[i]) );
							}
							else DigitValProg( *PtrTabProg[i] );
				alterna = !alterna;
				Cnt10mSec = 100;
			}
		}
		DoIdle();

		//___________________________________________________________________________________
		// ************************** Gestione dei tasti (pulsanti): P1, P2, P1+P2 **********
		if ( NewKey ) {                         // Se e' stato premuto un tasto...

			//________________________________________________________________________
			// ********************** Gestione del tasto P1 **************************

			if ( IS_KEY( KEY_P1 ) ) {             // Se il tasto premuto e' P1...
				Play_Buzzer ( HIGH, 20 );
				NewKey = 0;
				while ( IS_KEY( KEY_P1 ) ) {        // Controllo del tempo di pressione del P1
					DoIdle();
					if ( PressCnt >= 100 )   break;
				}
				if ( IS_KEY( KEY_P1 ) ) {           // Se la pressione del P1 e' prolungata...

					// *** Azioni eseguite con P1 premuto per un tempo lungo
					;

				} else {                            // Se la pressione del P1 e' breve...

					// *** Azioni eseguite con P1 premuto per un tempo breve

					// passa al parametro successivo
					alterna = 1;
					i++;
					if ( i > DimTabProg - 1 )  i = 1;
					Cnt10mSec = 0;        // per eliminare il ritardo della visualizzazione rispetto alla pressione del tasto

					while ( ! IsInTabProg [i] )
					{
						i++;
						if ( i > DimTabProg - 1 )
							i = 1;
					}

					Cnt1Sec = 30;

				}

			} // END gestore tasto P1 ************************************************

			//________________________________________________________________________
			// ********************** Gestione del tasto P2 **************************

			if ( IS_KEY( KEY_P2 ) ) {             // Se il tasto premuto e' P2...
				Play_Buzzer ( HIGH, 20 );
				NewKey = 0;
				while ( IS_KEY( KEY_P2 ) ) {        // Controllo del tempo di pressione del P2
					DoIdle();
					if ( PressCnt >= 100 )   break;
				}
				if ( IS_KEY( KEY_P2 ) ) {           // Se la pressione del P2 e' prolungata...

					// *** Azioni eseguite con P2 premuto per un tempo lungo

					Speed_100mSec = 30;
					// incrementa il parametro selezionato in modo continuo
					while ( IS_KEY( KEY_P2 ) ) {
						DoIdle();
						if ( i == i_offset+1 )
							DigitValProg( Cel_Far_Delta(TabProg.TEV) );// DigitValProg( Cel_Far(T_E_V) );
						else  if ( i == i_offset+2 )
							DigitValProg( Cel_Far_Delta(TabProg.TEC) );// DigitValProg( Cel_Far(T_E_C) );
						else  if ( i == i_offset+3 )
							DigitValProg( Cel_Far(/*T_G_V*/T_E_C) );
						else  if (i == i_offset+4 )
							DigitValProg( Cel_Far(T_E_1) );
						else
							if ( i == i_offset+5 )
								DigitValProg( Cel_Far(T_E_V) );
							else  if ( i == i_offset+6 )
								DigitValProg( Cel_Far(T_E_C) );
							else  if ( i == i_offset+7 )
								DigitValProg( Cel_Far(/*T_G_V*/T_E_C) );
							else  if (i == i_offset+8 )
								DigitValProg( Cel_Far(T_E_1) );
							else if ( ( i == 24 )||/*( i == 25 )||*/( i == 26 )||( i == 31 )||( i == 33 ) ) {
								DigitValProg( Cel_Far(*PtrTabProg[i]) );
							}
							else if ( ( i == 27 )||( i == 29 )/*||(( i >= 45 )||( i <= 48 ))*/ ) {
								DigitValProg( Cel_Far_Delta(*PtrTabProg[i]) );
							}
							else
								DigitValProg( *PtrTabProg[i] );   ////////////////////////
						if ( /*!SetTemp_PNTC(i)*/ TRUE ) {
							(*PtrTabProg[i]) ++;
							if ( *PtrTabProg[i] > MaxParametro[i] )
								*PtrTabProg[i] = MinParametro[i];
						}
						if ( Speed_100mSec )
							Wait10mSec( 25 );
						else
							Wait10mSec( 10 );
						Cnt1Sec = 30;
						modificato_tabella = 1;
					}

				} else {                            // Se la pressione del P2 e' breve...

					// *** Azioni eseguite con P2 premuto per un tempo breve

					// incrementa di 1 il parametro selezionato
					alterna = 0;
					if ( ( /*!SetTemp_PNTC(i)*/ TRUE ) && ( i <= 45)) {      //48==>45
						(*PtrTabProg[i]) ++;
						if ( *PtrTabProg[i] > MaxParametro[i] )
							*PtrTabProg[i] = MinParametro[i];
					}
					Cnt1Sec = 30;
					modificato_tabella = 1;

				}

			} // END gestore tasto P2 ************************************************

			// _____ Gestione del tasto P3 ________________________________________

		} // END gestione della pressione di un tasto ***************************************
		//___________________________________________________________________________________

		if ( ( i >= 5 ) && ( i <= 12 ) ) {

			NOP;
			//________________________________
			// Salvataggio sul REAL-TIME-CLOCK

			// se viene modificato l'orario viene salvato su REAL-TIME-CLOCK
			// if ( TabProg.SEC != Ora.Second )   { modificato_orario = TRUE; salva_orario_E2 = TRUE; } // non serve !!!
			if ( TabProg.MIN != Ora.Minute )   { modificato_orario = TRUE; salva_orario_E2 = TRUE; }
			if ( TabProg.ORA != Ora.Hour )     { modificato_orario = TRUE; salva_orario_E2 = TRUE; }
			if ( TabProg.WEK != Ora.DayWeek )  { modificato_orario = TRUE; salva_orario_E2 = TRUE; }
			if ( TabProg.DAY != Ora.DayMonth ) { modificato_orario = TRUE; salva_orario_E2 = TRUE; }
			if ( TabProg.MES != Ora.Month )    { modificato_orario = TRUE; salva_orario_E2 = TRUE; }
			if ( TabProg.YAR != Ora.Year )     { modificato_orario = TRUE; salva_orario_E2 = TRUE; }
			if ( modificato_orario ) {
				modificato_orario = FALSE;
				Ora.Second = TabProg.SEC;
				Ora.Minute = TabProg.MIN;
				Ora.Hour = TabProg.ORA;
				Ora.DayWeek = TabProg.WEK;
				Ora.DayMonth = TabProg.DAY;
				Ora.Month = TabProg.MES;
				Ora.Year = TabProg.YAR;
			}
		}

	} // end while


	//______________________
	// Salvataggio in FLASH

	TastoOffBreve = FALSE;
	if ( modificato_tabella ) {            // se e' stata modificata la struttura TabProg...
		AllDigitLedOff();
		WaitmSec( 50 );
		WriteEeprom( &TabProg );             // salva la struttura TabProg in FLASH
	}
	if ( salva_orario_E2 ) {                // se e' stato modificato l'orario...
		salva_orario_E2 = FALSE;
		TimerWrData( &Ora );                    // salva l'orario sul REAL-TIME-CLOCK
		CountDownFlag = FALSE;                  // deve essere reimpostato il countdown del REAL-TIME-CLOCK
		SalvaStatoMacchina(State);
	}




	//_______________________________________
	// Impostazione del DIGITAL-POTENTIOMETER
	DataPot = TabProg.EPT;                 // scrittura del parametro di taratura nel Digital-Pot
	WriteDigitalPot0( DataPot );

	AbilitaControlloOrologio = TRUE;

} // END ProgrammazioneTecnico()












// *** Programmazione utente + programmazione tecnico
void Programmazione ( void )
{
	alterna = 1;    // variabile per alternare il nome del parametro con il suo valore
	// Cnt10mSec = 0;  // contatore per la durata delle visualizzazioni del parametro e del valore, la commento
	// senn� non riesco a visualizzare "LoW" in STOP; faccio questa inizializzazione prima
	// delle chiamate ProgrammazioneTecnico() e ProgrammazioneUtente();
	Cnt1Sec = 30;   // contatore per l'uscita automatica dalla programmazione

	DoIdle();

	//___________________________________________________________________________________
	// ************************** Gestione dei tasti (pulsanti): P1, P2, P1+P2 **********
	if ( NewKey ) {                         // Se e' stato premuto un tasto...

		//_______________________________________________________________________________
		// ********************** Gestione dei tasti P1+P2 ******************************
		if ( IS_KEY( KEY_P1P2 ) ) {         // Se il tasto premuto e' P1+P2...
			Play_Buzzer ( HIGH, 20 );
			NewKey = 0;
			while ( IS_KEY( KEY_P1P2 ) ) {    // Controllo del tempo di pressione del P1+P2
				DoIdle();
				if ( PressCnt >= 300 )         // 300 * 10 mSec = 3 Sec
					StringVal("Hot");
				if ( PressCnt >= 1000 )        // 300 * 10 mSec = 5 Sec
					break;
			}
			if ( IS_KEY( KEY_P1P2 ) ) {       // Se la pressione del P1+P2 e' prolungata...
				// *** Azioni eseguite con P1+P2 premuti per un tempo lungo
				Cnt10mSec = 0;
				AbilitaControlloOrologio = FALSE;
				ProgrammazioneTecnico();
			} else {                          // Se la pressione del P1+P2 e' breve...
				// *** Azioni eseguite con P1+P2 premuti per un tempo breve
				Cnt10mSec = 0;
				ProgrammazioneUtente();
			}
		} // END gestore tasti P1+P2 ****************************************************

		//_________________________________________________________________________________
		// ********************** Gestione del tasto P1 ***********************************
		if ( IS_KEY( KEY_P1 ) ) {             // Se il tasto premuto e' P1...
			NewKey = 0;
		} // END gestore tasto P1 *********************************************************

		//_________________________________________________________________________________
		// ********************** Gestione del tasto P2 ***********************************
		if ( IS_KEY( KEY_P2 ) ) {             // Se il tasto premuto e' P2...
			NewKey = 0;
		} // END gestore tasto P2 *********************************************************

	} // END gestione della pressione di un tasto ***************************************
	//___________________________________________________________________________________

} // END Programmazione()





//-------------------------------------------------------------------------------------------------










/* ************************* Salvataggio / Visualizzazione dei Coni *********************** */



int NumeroConi;                              // numero dei coni erogati
int OldNumeroConi;                           // memorizza il numero di coni salvati l'ultima volta
int ConiOggi;                                // numero dei coni erogati oggi





// *** Salvataggio in EEPROM della variabile "NumeroConi"
void SalvaConi ( void )
{

	if ( ( NumeroConi >= OldNumeroConi + MAXCONI ) || ( !ConiOggi && ( BlackOutData.ConiOggi > 0 ) ) ) {  // memorizza ogni MAXCONI coni erogati
		BlackOutData.NumConi = NumeroConi;
		BlackOutData.ConiOggi = ConiOggi;
		PageWriteEEPROM( 0x0000, (UC*)&BlackOutData, sizeof( struct StrBlackOutData ) );
		OldNumeroConi = NumeroConi;
	}

} // END SalvaConi()





// *** Salvataggio in EEPROM della variabile "Coni_LIV"
void SalvaConiLiv ( void )
{
	BlackOutData.Coni_LIV = Coni_LIV;
	PageWriteEEPROM( 0x0000, (UC*)&BlackOutData, sizeof( struct StrBlackOutData ) );

} // END SalvaConiLiv()




// *** Salvataggio in EEPROM della variabile "WASH_Days"
void SalvaWashDays ( void )
{
	BlackOutData.WASH_Days = WASH_Days;
	PageWriteEEPROM( 0x0000, (UC*)&BlackOutData, sizeof( struct StrBlackOutData ) );

} // END SalvaWashDays()




// *** Salvataggio in EEPROM della variabile "Pasto_End"
void SalvaPastoEnd ( void )
{
	BlackOutData.Pasto_End = Pasto_End;
	PageWriteEEPROM( 0x0000, (UC*)&BlackOutData, sizeof( struct StrBlackOutData ) );

} // END SalvaPastoEnd()







// *** Visualizzazione dei coni erogati ( migliaia e centinaia )
void VisualizzaConi ( void )
{
	UC unita, decine, centinaia, migliaia, decimigliaia;
	int NumConi;

	NumConi = NumeroConi;

	unita = NumConi % 10;         // unita'
	NumConi /= 10;
	decine = NumConi % 10;        // decine
	NumConi /= 10;
	centinaia = NumConi % 10;     // centinaia
	NumConi /= 10;
	migliaia = NumConi % 10;      // migliaia
	NumConi /= 10;
	decimigliaia = NumConi % 10;  // decine di migliaia

	Digit[0] = migliaia;
	Digit[1] = decimigliaia;
	Digit[2] = 62;
	Wait10mSec( 100 );
	Digit[0] = unita;
	Digit[1] = decine;
	Digit[2] = centinaia;
	Wait10mSec( 100 );
	StringVal("   ");
	Wait10mSec( 100 );

} // END VisualizzaConi()





//-------------------------------------------------------------------------------------------------







/* ************************* Salvataggio delle ore dei motori MP e MA *********************** */

int MP_Ore;
int MA_Ore;
int Old_MP_Ore;
int Old_MA_Ore;



// *** Salvataggio in EEPROM delle variabili "MP_Ore", "MA_Ore"
void SalvaOreMotori ( void )
{
	UC OkSalva = 0;

	if ( MP_Ore >= (Old_MP_Ore + MAX_ORE_MOTORI) ) {  // memorizza ogni MAX_ORE_MOTORI ore
		BlackOutData.MP_Ore = MP_Ore;
		Old_MP_Ore = MP_Ore;
		OkSalva = 1;
	}

	if ( MA_Ore >= (Old_MA_Ore + MAX_ORE_MOTORI) ) {  // memorizza ogni MAX_ORE_MOTORI ore
		BlackOutData.MA_Ore = MA_Ore;
		Old_MA_Ore = MA_Ore;
		OkSalva = 1;
	}

	if ( OkSalva ) {
		PageWriteEEPROM( 0x0000, (UC*)&BlackOutData, sizeof( struct StrBlackOutData ) );
	}

} // END SalvaOreMotori()


//-------------------------------------------------------------------------------------------------








/* ***************************** Digital Potentiomenter ************************************ */



// *** Salvataggio in EEPROM del valore di autotaratura del Digital-Pot
void SalvaTaraturaPot ( void )
{

	TabProg.EPT = DataPot;                   // modifica il campo 'EPT' della struttura TabProg dall'EEPROM
	AllDigitLedOff();
	WaitmSec( 50 );
	WriteEeprom( &TabProg );                 // salva la struttura TabProg in EEPROM

} // END SalvaTaraturaPot()





//-------------------------------------------------------------------------------------------------





/* ***************************** Time-Keeper Alarm ************************************ */


// *** Procedura di impostazione allarme 24 ore
void SetAlarm24h ( void )
{
	// Lettura dell'ora dal REAL-TIME-CLOCK
	TimerRdData( &Ora );

	// Impostazione dell'allarme a 24 ore
	AllarmeOra.Minute = ( Ora.Minute + 0 ) % 60;
	AllarmeOra.Hour = ( Ora.Hour + 0 ) % 24;
	AllarmeOra.DayWeek = 1 << ((Ora.DayWeek+1) % 7);        // conversione: W1 W2 W4 -> AW0,AW1,AW2,AW3,AW4,AW5,AW6

	// Scrittura dell'allarme sul REAL-TIME-CLOCK tramite I2C
	I2cWriteByte( 0x64, 0xE0, 0x00 );             // AALE = 0 -> disattivazione alarm_A sul REAL-TIME-CLOCK
	WaitmSec( 20 );
	TimerWrAlarm_A( &AllarmeOra );                // riconfigura il REAL-TIME-CLOCK per il timer a 24 ore

} // END SetAlarm24h()



// *** Procedura di attivazione allarme 24 ore
void StartAlarm24h ( void )
{
	I2cWriteByte( 0x64, 0xE0, 0x80 );             // AALE = 1 -> attivazione alarm_A sul REAL-TIME-CLOCK
	WaitmSec( 20 );
	I2cWriteByte( 0x64, 0xF0, 0x20 );             // AAFG = 0 -> reset flag alarm_A sul REAL-TIME-CLOCK
	WaitmSec( 20 );

} // END StartAlarm24h()



// *** Procedura di disattivazione allarme 24 ore
void StopAlarm24h ( void )
{
	I2cWriteByte( 0x64, 0xE0, 0x00 );             // AALE = 0 -> disattivazione alarm_A sul REAL-TIME-CLOCK
	WaitmSec( 20 );
	I2cWriteByte( 0x64, 0xF0, 0x20 );             // AAFG = 0 -> reset flag alarm_A sul REAL-TIME-CLOCK
	WaitmSec( 20 );

} // END StoAlarm24h()



// *** Procedura di controllo allarme 24 ore
BYTE CheckAlarm24h ( void )
{
	return ( I2cReadByte( 0x64, 0xF0 ) & 0x02 );

} // END CheckAlarm24h()






/* ********************************* Funzioni per le uscite ********************************* */



void All_Out_Off ( void )
{

}











/* ****************************** Stati della Macchina ************************************* */



// Variabili usate nel main()
//_________________________________________________________________________________________________
UC State;
UC BlackoutState;                                 // variabile di stato dello SWITCH
UC OldState;                              // stato salvato in caso di Black-Out


// Variabili usate in Selezione()
//_________________________________________________________________________________________________
UC StatoSelect;                           // variabile di stato dello SWITCH
//--------------------------------------------------------------------------
WORD  CntSelect;                          // contatore dello SWITCH



// Variabili usate in Stop()
//_________________________________________________________________________________________________
UC  StatoStop0;                           // variabile di stato dello SWITCH-0
UC  StatoStop1;                           // variabile di stato dello SWITCH-1
//UC  StatoStop2;                           // variabile di stato dello SWITCH-2
//UC  StatoStop3;                           // variabile di stato dello SWITCH-3
//----------------------------------------------------------------------------
WORD CntStop0;                            // contatore dello SWITCH-0
/*WORD CntStop1;                            // contatore dello SWITCH-1
WORD CntStop2;                            // contatore dello SWITCH-2
WORD CntStop3;   */                         // contatore dello SWITCH-3



// Variabili usate in Produzione()
//_________________________________________________________________________________________________
UC StatoPrd0;                             // variabile di stato dello SWITCH-0
UC StatoPrd1;                             // variabile di stato dello SWITCH-1
UC StatoPrd2;                             // variabile di stato dello SWITCH-2
UC StatoPrd3;                             // variabile di stato dello SWITCH-3
UC StatoPrd4;                             // variabile di stato dello SWITCH-4
UC StatoPrd5;                             // variabile di stato dello SWITCH-5
UC StatoPrd6;                             // variabile di stato dello SWITCH-6
UC StatoPrd10;                            // variabile di stato dello SWITCH-10
UC StatoPrd7;                             // variabile di stato dello SWITCH-7
/*UC StatoPrd8;                             // variabile di stato dello SWITCH-8
UC StatoPrd9;                             // variabile di stato dello SWITCH-9
UC StatoPrd13;                            // variabile di stato dello SWITCH-13
UC StatoPrd14;                            // variabile di stato dello SWITCH-14  */
//----------------------------------------------------------------------------
WORD CntPrd0;                             // contatore dello SWITCH-0
WORD CntPrd1;                             // contatore dello SWITCH-1
WORD CntPrd1a;                            // contatore dello SWITCH-1
WORD CntPrd2;                             // contatore dello SWITCH-2
WORD CntPrd3;                             // contatore dello SWITCH-3
WORD CntPrd4;                             // contatore dello SWITCH-4
WORD CntPrd5;                             // contatore dello SWITCH-5
WORD CntPrd5_Pre_SBR;                             // contatore dello SWITCH-5
WORD CntPrd5_SBR;                             // contatore dello SWITCH-5
WORD CntPrd5_SBR_Delay;
WORD CntPrd6;                             // contatore dello SWITCH-6
WORD CntPrd7;                             // contatore dello SWITCH-7
WORD CntPrd7_S_LIV;                             // contatore dello SWITCH-7
//WORD CntPrd8;                             // contatore dello SWITCH-8
//WORD CntPrd9;                             // contatore dello SWITCH-9
WORD CntPrd10;                            // contatore dello SWITCH-10
WORD CntPrdLAMP;
WORD TimerT1;                             // contatore globale in Produzione()
WORD TimerT2;                             // contatore globale in Produzione()
WORD TimerT3;                             // contatore globale in Produzione()
WORD TimerT4;                             // contatore globale in Produzione()
WORD TimerBeepAutotaratura;               // contatore globale in Produzione()
WORD Timer_Totale;
//----------------------------------------------------------------------------
UC  AttivatoTimerT1;                      // flag di attivazione del contatore TimerT1
UC  AttivatoTimerT2;                      // flag di attivazione del contatore TimerT2
UC  AttivatoTimerT3;                      // flag di attivazione del contatore TimerT3
UC  AttivatoTimerBeepAutotaratura;        // flag di attivazione del contatore TimerBeepAutotaratura
//----------------------------------------------------------------------------
UC DataPot;                               // valore da scrivere sul Digital-Pot
UC Delta_Timer;
UC Cicli_Fine_Freddo_Vasca;
UC Aggiorna_Cicli;
UC Interrupt_Cyl;
UC Attesa_MA;

// Variabili usate in Conservazione() ( Stoccaggio )
//_________________________________________________________________________________________________
UC  StatoSto0;                            // variabile di stato dello SWITCH-0
UC  StatoSto1;                            // variabile di stato dello SWITCH-1
UC  StatoSto2;                            // variabile di stato dello SWITCH-2
UC  StatoSto3;                            // variabile di stato dello SWITCH-3
UC  StatoSto4;                            // variabile di stato dello SWITCH-4
UC  StatoSto5;                            // variabile di stato dello SWITCH-5

//----------------------------------------------------------------------------
WORD CntSto0;                             // contatore dello SWITCH-0
WORD CntSto1;                             // contatore dello SWITCH-1
WORD CntSto2;                             // contatore dello SWITCH-2
WORD CntSto2_SBR;                             // contatore dello SWITCH-2
WORD CntSto2_HOT;
WORD CntSto2_MA_OFF_Delay;                             // contatore dello SWITCH-2
WORD CntSto3;                             // contatore dello SWITCH-2
WORD CntSto2a;                            // contatore dello SWITCH-2
WORD CntSto4;                             // contatore dello SWITCH-4
WORD CntSto5;                            // contatore dello SWITCH-5
UC   Abilita_CntSto2_MA_OFF_Delay;


// Variabili usate in Pulizia()
//_________________________________________________________________________________________________
UC StatoCle0;                             // variabile di stato dello SWITCH-0
UC StatoCle1;                             // variabile di stato dello SWITCH-1
UC StatoCle2;                             // variabile di stato dello SWITCH-2
UC StatoCle3;                             // variabile di stato dello SWITCH-3
//----------------------------------------------------------------------------
WORD CntCle0;                             // contatore dello SWITCH-0
WORD CntCle1;                             // contatore dello SWITCH-1
WORD CntCle2;                             // contatore dello SWITCH-2
WORD CntCle2_2;                             // contatore dello SWITCH-2
WORD CntCle3;                             // contatore dello SWITCH-4
//----------------------------------------------------------------------------
UC AttivatoCntCle2;                       // flag di attivazione del contatore dello SWITCH-2
UC AttivatoCntCle2_2;                       // flag di attivazione del contatore dello SWITCH-2
//UC AttivatoCntCle4;                       // flag di attivazione del contatore dello SWITCH-4

WORD Acc_Apprendimento;
UC   Acc_EN;
int  Delta_Set_MP;
UC   Start_MA_ON;






// Variabili usate in Monitoring()
//_________________________________________________________________________________________________
UC StatoMon0;                               // variabile di stato dello SWITCH-0
UC StatoMon1;                               // variabile di stato dello SWITCH-1
UC StatoMon1a;                               // variabile di stato dello SWITCH-1a
/*UC StatoMon1b;                              // variabile di stato dello SWITCH-1b
UC StatoMon1c; */                             // variabile di stato dello SWITCH-1c
UC StatoMon2;                               // variabile di stato dello SWITCH-2
//UC StatoMon2b;                              // variabile di stato dello SWITCH-2b
UC StatoMon3;                               // variabile di stato dello SWITCH-3
UC StatoMon4;                               // variabile di stato dello SWITCH-4
UC StatoMon5;                               // variabile di stato dello SWITCH-5
// UC StatoMon6;                               // variabile di stato dello SWITCH-6
UC StatoMon7;                               // variabile di stato dello SWITCH-7
UC StatoMon8;                               // variabile di stato dello SWITCH-8
UC StatoMon8b;                              // variabile di stato dello SWITCH-8b
//UC StatoMon9;                               // variabile di stato dello SWITCH-9
UC StatoMon10;                              // variabile di stato dello SWITCH-10
UC StatoMon11;                              // variabile di stato dello SWITCH-11
//UC StatoMon12;                              // variabile di stato dello SWITCH-12
//UC StatoMon13;                              // variabile di stato dello SWITCH-13
UC StatoMon14;                              // variabile di stato dello SWITCH-14
UC StatoMon15;                              // variabile di stato dello SWITCH-15
UC StatoMon16;                              // variabile di stato dello SWITCH-16
/*UC StatoMon17;                              // variabile di stato dello SWITCH-17
UC StatoMon18;                              // variabile di stato dello SWITCH-18
UC StatoMon19;                              // variabile di stato dello SWITCH-19
UC StatoMon20;                              // variabile di stato dello SWITCH-20
UC StatoMon21;                              // variabile di stato dello SWITCH-21
UC StatoMon21b;                              // variabile di stato dello SWITCH-21
UC StatoMon22;                              // variabile di stato dello SWITCH-22
UC StatoMon23;                              // variabile di stato dello SWITCH-23
UC StatoMon24;                              // variabile di stato dello SWITCH-24
UC StatoMon25;                              // variabile di stato dello SWITCH-25
UC StatoMon26;                              // variabile di stato dello SWITCH-26
UC StatoMon27;                              // variabile di stato dello SWITCH-27
UC StatoMon28;                              // variabile di stato dello SWITCH-28
UC StatoMon29;*/                              // variabile di stato dello SWITCH-29
UC StatoMon30;                              // variabile di stato dello SWITCH-30
UC StatoMon31;                              // variabile di stato dello SWITCH-30
UC StatoMon32;                              // variabile di stato dello SWITCH-32
UC StatoMon33;                              // variabile di stato dello SWITCH-33
//UC StatoMon36;                              // variabile di stato dello SWITCH-36
UC StatoMon37;                              // variabile di stato dello SWITCH-37
//UC StatoMon38;                              // variabile di stato dello SWITCH-38
//UC StatoMon41;                              // variabile di stato dello SWITCH-41

//-------------------------------------------------------------------------------
//WORD  CntMon0;                               // contatore dello SWITCH-0
WORD  CntMon1;                               // contatore dello SWITCH-1
//WORD  CntMon1c;                               // contatore dello SWITCH-1c
WORD  CntMon2;                               // contatore dello SWITCH-2
//WORD  CntMon3;                               // contatore dello SWITCH-3
//WORD  CntMon4;                               // contatore dello SWITCH-4
WORD  CntMon5;                               // contatore dello SWITCH-5
/*WORD  CntMon6; */                              // contatore dello SWITCH-6
//WORD  CntMon7;                               // contatore dello SWITCH-7
WORD  CntMon8;                               // contatore dello SWITCH-8
//WORD  CntMon9;                               // contatore dello SWITCH-9
//WORD  CntMon10;                              // contatore dello SWITCH-10
WORD  CntMon11;                              // contatore dello SWITCH-11
/*WORD  CntMon12;                              // contatore dello SWITCH-12
WORD  CntMon13;                              // contatore dello SWITCH-13
WORD  CntMon14;                              // contatore dello SWITCH-14
WORD  CntMon15;                              // contatore dello SWITCH-15
WORD  CntMon16;                              // contatore dello SWITCH-16
WORD  CntMon17;                              // contatore dello SWITCH-17
WORD  CntMon18;                              // contatore dello SWITCH-18
WORD  CntMon19;                              // contatore dello SWITCH-19
WORD  CntMon20;                              // contatore dello SWITCH-20
WORD  CntMon21;                              // contatore dello SWITCH-21
WORD  CntMon23;                              // contatore dello SWITCH-23
WORD  CntMon24;                              // contatore dello SWITCH-24
WORD  CntMon25;                              // contatore dello SWITCH-25
WORD  CntMon27;                              // contatore dello SWITCH-27
WORD  CntMon28; */                             // contatore dello SWITCH-28
WORD  CntMon30;                              // contatore dello SWITCH-30
WORD  CntMon31;                              // contatore dello SWITCH-30
WORD  CntMon32;                              // contatore dello SWITCH-33
WORD  CntMon33;                              // contatore dello SWITCH-33
WORD  CntMon33_2;                              // contatore dello SWITCH-33
//WORD  CntMon38;                              // contatore dello SWITCH-38
WORD  CntMonFiltroIMS;                  // contatore dell' intermittenza del buzzer
WORD  Cnt_EVF_Cabinet;
WORD  Cnt_MC_Delay;                  // contatore dell' intermittenza del buzzer
WORD  Cnt_EV_Su_MC_OFF;                  // contatore dell' intermittenza del buzzer
WORD  CntMon_MV;
/*WORD  Timer_IMS;                             // contatore dello SWITCH-19
WORD  Timer_Der_TEV; */                        // contatore dello SWITCH-19
UC    CntMonSec_Ore_MP;                      // contatore dello SWITCH-38
UC    CntMonSec_Ore_MA;                      // contatore dello SWITCH-38
UC    Old_second;                            // controllo secondi per conteggio ore motori
UC    Incremento_Ora;
WORD  DelayMC;                           // Contatore per accensione MV prima di MC
//------------------------------------------------------------------------------
/*UC  Attivato_CntMon17;                      // flag di attivazione del contatore dello SWITCH-17
UC  Attivato_CntMon18;                      // flag di attivazione del contatore dello SWITCH-18
UC  Attivato_CntMon23;                      // flag di attivazione del contatore dello SWITCH-23
UC  Attivato_Timer_IMS;   */                  // flag di attivazione del contatore dello SWITCH-21

// Variabili della 191 Bar Pastorizzante
//_________________________________________________________________________________________________
//UC  FCV;                                  // Fine Conservazione Vasca
//UC  FCC;                                  // Fine Conservazione Cilindro
//UC  FRV;                                  // Fine Riscaldamento Vasca
//UC  FRC;                                  // Fine Riscaldamento Cilindro
UC  FFV;                                  // Fine Freddo Vasca
UC  FFC;                                  // Fine Freddo Cilindro
//-------------------------------------------------------------------------------
//UC  Ripristino_TGV;
//UC  Ripristino_TGV_non_PSP;
//SC  Sonda_freddo_vasca;
UC  Pasto_End;
//UC  Forza_Pasto;
//UC  Derivata_TEV;
// UC  Old_TEV;
// UC  Pasto_Night;
//UC  Temp_Out_Pausa_Ok;
//UC  Pausa;
//UC  Risc;
UC  BlackOut;
UC  OK_Tempo;
//UC  Salta_Pausa;
//UC  Display_End;
//UC  Pasto_con_Liv_Scoperto;
UC  MIR_Enable = TRUE;
//UC  Fine24h;
UC  CountDownFlag;                       // � il Countdown_24h
//UC  AbilitaTransizione_Pasto_Cons;
UC  PWM_Values[5][2] = {{12,8},    {12,7}    ,{12,6},{12,5},{12,0}};
UC  LED_PASTORIZZAZIONE;
UC  LED_STOP;
UC  LED_PRODUZIONE;
UC  LED_CONSERVAZIONE;
UC  LED_ESTRAZIONE;
UC  LED_PULIZIA;
UC  LED_GUSTO_1;
UC  LED_GUSTO_2;
UC  LED_GUSTO_3;
UC  Flag_SC_1;
UC  Flag_SC_2;
UC  Flag_SC_3;
UC  BEEPER;
UC  E_V_S_3;
// UC  M_I_R;
//UC  Versione_Colore;
UC  HOT_Max;
UC  Ciclo_High;

UC  Set_HOT;
UC  Prod_Per_Cons;
UC  Prod_shake;
UC  Isteresi_HOT;
WORD  Timer_Prod;
UC  Soglia_Timer;
UC  Delta_Incremento_HOT;
UC  Modificato_Set_Hot;
UC  Mostra_HOT;
UC  Step_HOT;
// Variabili degli Allarmi in Monitoring
//_________________________________________________________________________________________________
UC  Spegni_Macchina;
UC  M_C_Enable;
UC  BUZZER_EXT;
// UC  End_Timer;
UC  AL;
UC  AL2;
//UC  AL3;
//UC  AL_TE1;
UC  AL_T3_Prod;
UC  Allarme_IMS;
UC  I_M_S_Filtrato;
// UC  OK_Tempo;
UC  Coni_LIV;
UC  ConiLivFlag;
UC  WashDaysFlag;
//UC  PastoEndFlag;
UC  WASH;
UC  WASH_Today;
UC  WASH_Days;
UC  Alt_HOB;
UC  Divisore_Timer;
UC  Scaduto_Timer_MP;
UC  Flag_TEC;
UC  PR_Counter;
//UC  Caldo_TGV;
// UC  M_V;
UC  M_V_H;
UC  MV_Enable;
//UC  alterna_LoW;       // per visualizzare la scritta LoW lampeggiante del livello scoperto
//UC	start_secondi;
//#define   AL_No_Prod   ( !(Pasto_End) || (Coni_LIV==TabProg.CON) || (WASH_Today==TRUE) /*|| LivelloScoperto*/ )
UC E_V_F_C;
UC E_V_F_V_delay;
UC E_V_F_V;

//UC SalvaStatoMon;


// Variabili dei tasti
//_________________________________________________________________________________________________
UC TastoSelezioneBreve;
UC TastoSelezioneLungo;
UC TastoSelezioneExtraLungo;
UC TastoStopBreve;
UC TastoStopLungo;
UC TastoOffBreve;
UC TastoOffLungo;

UC TastoSelezioneBreveMon;
UC TastoSelezioneLungoMon;
UC TastoSelezioneExtraLungoMon;
UC TastoStopBreveMon;
UC TastoStopLungoMon;

UC DisabilitaTastoStop;



// Variabili dei livelli
//_________________________________________________________________________________________________
//UC  LivelloCoperto;
//UC  LivelloScoperto;


// Variabili usate per i motori
//_________________________________________________________________________________________________
UC MP_Minuti;
UC MA_Minuti;


UC HOT ( void )
{
	UC Value;

	if ( (StatoPrd3 >= S1_IN) && (StatoPrd1 >= END) )
	{
		Value = (UC) ( ( ((UINT)(((long)In010Value*120)/573)) * TabProg.DEF ) /100 );
	} else {
		if ( Ciclo_High )
		{
			Value = (UC) ( ( ((UINT)(((long)In010Value*120)/573)) * TabProg.PTC ) /100 );
		}
		else  Value = (UC) ( ( ((UINT)(((long)In010Value*120)/573)) *  /*TabProg.PTV*/ 80 ) /100 );
	}
	if ( Value >= 120)
	{
		Value = 120;
	}

	return Value;
}






//_________________________________________________________________________________________________
//_____Funzione Monitoring_________________________________________________________________________
UC Monitoring ( void )
{
	DoIdle();
	if ( NewKey)
	{
		// _____ Gestione del tasto P1 ________________________________________
		if ( IS_KEY( KEY_P1 ) ) {               // Se il tasto premuto e' P1...
			Play_Buzzer ( HIGH, 20 );
			NewKey = 0;
			while ( IS_KEY( KEY_P1 ) ) {      // Controllo del tempo di pressione del P2
				DoIdle();
				if ( PressCnt >= 1000 ) {
					TastoStopLungo = TRUE;   // TASTO-STOP premuto per un tempo lungo
					break; }
			}
			// Azioni eseguite con P1 premuto per un tempo breve

			if ( IS_KEY( KEY_P1 ) ) { // Se la pressione del P1 e' prolungata...
				// Azioni eseguite con P1 premuto per un tempo lungo
				TastoStopLungo = TRUE;   // TASTO-STOP premuto per un tempo lungo
			} else { // Se la pressione del P1 e' breve...
				TastoStopBreveMon = TRUE;                // TASTO-STOP-MON premuto per un tempo breve
				if ( ( State != 0 ) && ( State != 6 ) )
				{
					TastoStopBreve = TRUE;                   // TASTO-STOP premuto per un tempo breve
				}
			}
		} // END gestione tasto P1

	}
	if ( ( ( Str_Allarme1 != Null ) || ( Str_Allarme2 != Null ) ) && ( AL == 0 ) ) {    // per evitare interferenze

		// ____________________Gestione tasti______________________________________
		if ( NewKey ) {                       // Se e' stato premuto un tasto...

			// ____Gestione del tasto P2___________________________________________
			if ( IS_KEY( KEY_P2 ) ) {           // Se il tasto premuto e' P2...
				Play_Buzzer ( HIGH, 20 );
				NewKey = 0;
				while ( IS_KEY( KEY_P2 ) ) {      // Controllo del tempo di pressione del P2
					DoIdle();
					if ( PressCnt >= 300 )
						break;
				}
				if ( IS_KEY( KEY_P2 ) ) {         // Se la pressione del P2 e' prolungata...
					// Azioni eseguite con P2 premuto per un tempo lungo 5"
					TastoSelezioneLungoMon = TRUE;
				} else { // Se la pressione del P2 e' breve...
					// Azioni eseguite con P2 premuto per un tempo breve
					TastoSelezioneBreveMon = TRUE;
				}
			}

		}
		//___________________Fine Gestione Tasti_____________________________________
	}
	// Gestione dei led PROD, CONS, PUL e
	LED_4 = LED_STOP;       // STOP
	LED_3 = LED_PULIZIA;   // PUL
	LED_1 = LED_PRODUZIONE;     // PROD
	LED_2 = LED_CONSERVAZIONE; // CONS


	if(State==2){
		E_V_F_C_reale = E_V_F_C;
		E_V_F_V_reale = E_V_F_V;
		M_C_reale     =E_V_F_C||E_V_F_V;
	}else{
		E_V_F_C_reale = (FFC==FALSE) && E_V_F_C;
		E_V_F_V_reale = (FFV==FALSE)&&(FFC==TRUE)&&E_V_F_V;
		M_C_reale     =M_C;
	}
	//(State!=2&&M_C) || (state==2&&(FFV==FALSE || FFC==FALSE));

	switch ( StatoMon0 ) {                // *** SWITCH-0 ***
	//_______________________________________________________
	case S0_IN :
		StatoMon1 =  S0_IN;
		StatoMon2 =  S0_IN;
		StatoMon3 =  S0_IN;
		StatoMon4 =  S0_IN;
		StatoMon10=  S0_IN;
		StatoMon0 =  S0;
		StatoMon32 =  S0_IN;
		break;
	case S0 :
		if ( TRUE ) {
			StatoMon0 = S1_IN;
		}
		break;
	case S1_IN :
		LED_CONSERVAZIONE = OFF;
		AL = 0;
		Str_Allarme1 = Null;
		Str_Allarme2 = Null;
		Str_Allarme3 = Null;
		I_M_S_Filtrato = FALSE;
		AL2 = 0;
		StatoMon0 = S1;
		break;
	case S1:
		StatoMon0 = END;
		StatoMon1 =  S1_IN;
		StatoMon2 =  S1_IN;
		StatoMon3 =  S1_IN;
		StatoMon4 =  S1_IN;
		StatoMon7 =  S1_IN;
		StatoMon10=  S1_IN;
		StatoMon32=  S1_IN;
		break;
	}
	DoIdle();

	switch ( StatoMon2 ) {                // *** SWITCH-1 ***
	//_______________________________________________________
	case S0_IN :
		StatoMon2 = S0;
		break;
		//-------------------------------------------------------
	case S0 :
		if ( TRUE ) {
			StatoMon2 = S0;
		}
		break;
		//_______________________________________________________
	case S1_IN :
		NOP;
		StatoMon2 = S1;
		break;
		//-------------------------------------------------------
	case S1 :
		if ( ( TabProg.RT == YES ) && ( AL == 0 ) && ( ( M_A == ON ) && ( H_O_T >= 127 ) ) ) {
			StatoMon2 = S1a_IN;
		}
		break;
		//_______________________________________________________
	case S1a_IN :
		CntMon2 = 30 * 10;            //contatore a 30 secondi
		StatoMon2 = S1a;
		break;
		//-------------------------------------------------------
	case S1a :
		if ( ( !CntMon2 ) && ( H_O_T >=127 ) ) {
			StatoMon2 = S2_IN;
		} else if ( ( M_A == OFF ) || ( H_O_T < 120 ) )
			StatoMon2 = S1_IN;
		break;
		//_______________________________________________________
	case S2_IN :
		Str_Allarme1 = "A03";
		AL++;
		LedAllarmeLampeggiante;
		CntMon2 = 3000;
		StatoMon2 = S2;
		break;
		//-------------------------------------------------------
	case S2 :
		if ( ( !CntMon2 ) || ( TabProg.RT == NO ) ) {
			StatoMon2 = S3_IN;
		}
		break;
		//_______________________________________________________
	case S3_IN :
		AL--;
		LedAllarmeFisso;
		StatoMon2 = S3;
		break;
		//-------------------------------------------------------
	case S3 :
		if ( ( strcmp( Str_Allarme1, "A03" ) != 0 ) || ( ( TabProg.RT == YES ) && ( ( M_A == ON ) && ( H_O_T >= 127 ) ) ) ) {
			StatoMon2 = S1_IN;
		} else {
			if ( ( ( TastoSelezioneBreveMon == TRUE ) /*|| Allarme_IMS*/ ) && ( strcmp( Str_Allarme1, "A03" ) == 0 ) ) {
				TastoSelezioneBreveMon = FALSE;
				StatoMon2 = S4_IN;
			}
		}
		break;
		//_______________________________________________________
	case S4_IN :
		Str_Allarme1 = Null;
		LedAllarmeSpento;
		StatoMon2 = S4;
		break;
		//-------------------------------------------------------
	case S4 :
		if ( TRUE ) {
			StatoMon2 = S1_IN;
		}
		break;
		//_______________________________________________________
	} // END SWITCH-2
	DoIdle();

	switch ( StatoMon1 ) {                // *** SWITCH-1 ***
	//_______________________________________________________
	case S0_IN :
		StatoMon1 = S0;
		break;
		//-------------------------------------------------------
	case S0 :
		if ( TRUE ) {
			StatoMon1 = S0;
		}
		break;
		//_______________________________________________________
	case S1_IN :
		NOP;
		StatoMon1 = S1;
		break;
		//-------------------------------------------------------
	case S1 :
		if ( ( TabProg.RT == YES ) && ( AL == 0 ) && ( ( M_A == ON ) && ( H_O_T < 10 ) ) ) {
			StatoMon1 = S1a_IN;
		}
		break;
		//_______________________________________________________
	case S1a_IN :
		CntMon1 = 5;
		StatoMon1 = S1a;
		break;
		//-------------------------------------------------------
	case S1a :
		if ( ( !CntMon1 ) && ( H_O_T < 10 ) ) {
			StatoMon1 = S2_IN;
		} else if ( ( M_A == OFF ) || ( H_O_T > 15 ) )
			StatoMon1 = S1_IN;
		break;
		//_______________________________________________________
	case S2_IN :
		Str_Allarme1 = "A01";
		AL++;
		LedAllarmeLampeggiante;
		CntMon1 = 3000;
		StatoMon1 = S2;
		break;
		//-------------------------------------------------------
	case S2 :
		if ( ( !CntMon1 ) || ( TabProg.RT == NO ) ) {
			StatoMon1 = S3_IN;
		}
		break;
		//_______________________________________________________
	case S3_IN :
		AL--;
		LedAllarmeFisso;
		StatoMon1 = S3;
		break;
		//-------------------------------------------------------
	case S3 :
		if ( ( strcmp( Str_Allarme1, "A01" ) != 0 ) || ( ( TabProg.RT == YES ) && ( ( M_A == ON ) && ( H_O_T < 10 ) ) ) ) {
			StatoMon1 = S1_IN;
		} else {
			if ( ( ( TastoSelezioneBreveMon == TRUE ) /*|| Allarme_IMS*/ ) && ( strcmp( Str_Allarme1, "A01" ) == 0 ) ) {
				TastoSelezioneBreveMon = FALSE;
				StatoMon1 = S4_IN;
			}
		}
		break;
		//_______________________________________________________
	case S4_IN :
		Str_Allarme1 = Null;
		LedAllarmeSpento;
		StatoMon1 = S4;
		break;
		//-------------------------------------------------------
	case S4 :
		if ( TRUE ) {
			StatoMon1 = S1_IN;
		}
		break;
		//_______________________________________________________
	} // END SWITCH-1

	DoIdle();


	switch ( StatoMon3 ) {                // *** SWITCH-3 ***
	//_______________________________________________________
	case S0_IN :
		StatoMon3 = S0;
		break;
		//-------------------------------------------------------
	case S0 :
		if ( TRUE ) {
			StatoMon3 = S0;
		}
		break;
		//_______________________________________________________
	case S1_IN :
		if ( ( Str_Allarme1 == Null ) && ( Str_Allarme2 == Null ) )
		{
			LedAllarmeSpento;
		}
		StatoMon3 = S1;
		break;
		//-------------------------------------------------------
	case S1 :
		if ( ( T_E_V_CortoCircuito || T_E_V_CircuitoAperto ) && ( AL == 0 )  ) {
			StatoMon3 = S2_IN;
		}
		break;
		//_______________________________________________________
	case S2_IN :
		Str_Allarme1 = "A05";
		AL++;
		LedAllarmeLampeggiante;
		// SalvaEvento(0,1,0);                      // **** Salva un evento per i LOG ****
		StatoMon3 = S2;
		break;
		//-------------------------------------------------------
	case S2 :
		if ( !( T_E_V_CortoCircuito || T_E_V_CircuitoAperto ) ) {
			StatoMon3 = S3_IN;
		}
		break;
		//_______________________________________________________
	case S3_IN :
		AL--;
		LedAllarmeFisso;
		StatoMon3 = S3;
		break;
		//-------------------------------------------------------
	case S3 :
		if ( ( TastoSelezioneBreveMon == TRUE ) && ( strcmp( Str_Allarme1, "A05" ) == 0 ) ) {
			TastoSelezioneBreveMon = FALSE;
			StatoMon3 = S4_IN;
		}
		break;
		//_______________________________________________________
	case S4_IN :
		Str_Allarme1 = Null;
		LedAllarmeSpento;
		StatoMon3 = S4;
		break;
		//-------------------------------------------------------
	case S4 :
		if ( TRUE ) {
			StatoMon3 = S1_IN;
		}
		break;
		//_______________________________________________________
	} // END SWITCH-3


	DoIdle();

	switch ( StatoMon4 ) {                // *** SWITCH-4 ***
	//_______________________________________________________
	case S0_IN :
		StatoMon4 = S0;
		break;
		//-------------------------------------------------------
	case S0 :
		if ( TRUE ) {
			StatoMon4 = S0;
		}
		break;
		//_______________________________________________________
	case S1_IN :
		if ( ( Str_Allarme1 == Null ) && ( Str_Allarme2 == Null ) )
		{
			LedAllarmeSpento;
		}
		StatoMon4 = S1;
		break;
		//-------------------------------------------------------
	case S1 :
		if ( (T_E_C_CortoCircuito||T_E_C_CircuitoAperto) && (AL==0) ) {
			StatoMon4 = S2_IN;
		}
		break;
		//_______________________________________________________
	case S2_IN :
		if ( AL == 0 ) {
			Str_Allarme1 = "A06";
			LedAllarmeLampeggiante;
		}
		StatoMon4 = S2;
		break;
		//-------------------------------------------------------
	case S2 :
		if ( ( State == 2 ) || ( State == 4 ) ) {  // se lo stato attivo e' CONSERVAZIONE o PASTORIZZAZIONE
			Flag_TEC = TRUE;
		}
		if ( !(T_E_C_CortoCircuito||T_E_C_CircuitoAperto) ) {
			StatoMon4 = S3_IN;
		} else
			if ( Str_Allarme1 == Null ) {
				StatoMon4 = S2_IN;
			}
			else if ( (TastoSelezioneBreveMon==TRUE) && (strcmp( Str_Allarme1, "A06" )==0) ) {
				TastoSelezioneBreveMon = FALSE;
				StatoMon4 = S5_IN;
			}
		break;
		//_______________________________________________________
	case S3_IN :
		if ( strcmp( Str_Allarme1, "A06" )==0 ) {
			LedAllarmeFisso;
		}
		if ( Flag_TEC == TRUE ) {
			Flag_TEC = FALSE;
		}
		StatoMon4 = S3;
		break;
		//-------------------------------------------------------
	case S3 :
		if ( (strcmp( Str_Allarme1, "A06" )!= 0)||(T_E_C_CortoCircuito||T_E_C_CircuitoAperto) ) {
			StatoMon4 = S1_IN;
		} else {
			if ( ( TastoSelezioneBreveMon == TRUE ) && (strcmp( Str_Allarme1, "A06" )==0) ) {
				TastoSelezioneBreveMon = FALSE;
				StatoMon4 = S4_IN;
			}
		}
		break;
		//_______________________________________________________
	case S4_IN :
		Str_Allarme1 = Null;
		LedAllarmeSpento;
		StatoMon4 = S4;
		break;
		//-------------------------------------------------------
	case S4 :
		if ( TRUE ) {
			StatoMon4 = S1_IN;
		}
		break;
		//_______________________________________________________
	case S5_IN :
		if ( strcmp( Str_Allarme1, "A06" ) == 0 ) {
			Str_Allarme1 = Null;
		}
		StatoMon4 = S5;
		break;
		//-------------------------------------------------------
	case S5 :
		if (!(T_E_C_CortoCircuito||T_E_C_CircuitoAperto))
		{
			Flag_TEC = FALSE;
		} else if ( ( State == 2 ) || ( State == 4 ) )
		{
			Flag_TEC = TRUE;
		}
		if ( ( !(T_E_C_CortoCircuito||T_E_C_CircuitoAperto) ) && ( Str_Allarme1 == Null ) && ( Str_Allarme2 == Null ) )
		{
			LedAllarmeFisso;
		}
		if ( (State==1)||(State==2)||(State==3)||(State==4) ) {
			StatoMon4 = S6_IN;
		}
		break;
		//_______________________________________________________
	case S6_IN :
		NOP;
		StatoMon4 = S6;
		break;
		//-------------------------------------------------------
	case S6 :
		if (!(T_E_C_CortoCircuito||T_E_C_CircuitoAperto))
		{
			Flag_TEC = FALSE;
		} else if ( ( State == 2 ) || ( State == 4 ) )
		{
			Flag_TEC = TRUE;
		}
		if ( ( !(T_E_C_CortoCircuito||T_E_C_CircuitoAperto) ) && ( Str_Allarme1 == Null ) && ( Str_Allarme2 == Null ) )
		{
			LedAllarmeFisso;
		}
		if ( !((State==1)||(State==2)||(State==3)||(State==4)) ) {
			StatoMon4 = S1_IN;
		}
		break;
		//_______________________________________________________

	} // END SWITCH-4
	DoIdle();

	switch ( StatoMon7 ) {                // *** SWITCH-7 ***
	//_______________________________________________________
	case S0_IN :
		StatoMon7 = S0;
		break;
		//-------------------------------------------------------
	case S0 :
		if ( TRUE ) {
			StatoMon7 = S0;
		}
		break;
		//_______________________________________________________
	case S1_IN :
		NOP;
		StatoMon7 = S1;
		break;
		//-------------------------------------------------------
	case S1 :
		if ( I_M_S == ON ) {
			StatoMon7 = S2_IN;
		}
		break;
		//_______________________________________________________
	case S1a_IN :
		CntMonFiltroIMS = 10;
		StatoMon7 = S1a;
		break;
		//-------------------------------------------------------
	case S1a :
		if ( ( CntMonFiltroIMS == 0 ) && ( I_M_S == ON ) ) {
			StatoMon7 = S2_IN;
		} else if ( I_M_S == OFF ) {
			StatoMon7 = S1_IN;
		}
		break;
		//_______________________________________________________
	case S2_IN :
		I_M_S_Filtrato = TRUE;
		AL++;
		if ( Str_Allarme1 == Null ) {
			Str_Allarme1 = "IMS";
		}
		StatoMon7 = S2;

		break;
		//-------------------------------------------------------
	case S2 :

		StatoMon7 = S2a_IN;
		break;
		//_______________________________________________________
	case S2a_IN :
		if ( Str_Allarme1 == Null ) {
			Str_Allarme1 = "IMS";
		}
		StatoMon7 = S2a;
		break;
		//-------------------------------------------------------
	case S2a :

		if ( Str_Allarme1 == Null ) {
			StatoMon7 = S2a_IN;
		} else {
			if ( I_M_S == OFF ) {
				StatoMon7 = S3_IN;
			}
		}

		break;
		//_______________________________________________________
	case S3_IN :
		I_M_S_Filtrato = FALSE;
		AL--;
		StatoMon7 = S3;
		break;
		//-------------------------------------------------------
	case S3 :
		if ( (strcmp(Str_Allarme1,"IMS")==0) && ( TastoSelezioneBreveMon == TRUE ) ) {
			TastoSelezioneBreveMon = FALSE;
			Str_Allarme1 = Null;
			StatoMon7 = S1_IN;
		}
		break;
		//_______________________________________________________
	} // END SWITCH-7
	DoIdle();

	switch ( StatoMon10 ) {                // *** SWITCH-10 ***
	//_______________________________________________________
	case S0_IN :
		StatoMon10 = S0;
		break;
		//-------------------------------------------------------
	case S0 :
		if ( TRUE ) {
			StatoMon10 = S0;
		}
		break;
		//_______________________________________________________
	case S1_IN :
		//      OK_Tempo = TRUE;
		StatoMon10 = S1;
		break;
		//-------------------------------------------------------
	case S1 :
		if ( BlackOut == TRUE ) {
			StatoMon10 = S2_IN;
		}
		break;
		//_______________________________________________________
	case S2_IN :
		if ( T_E_V >= TabProg.KBB ) {
			Str_Allarme2 = "A16";
		} else {
			Str_Allarme2 = "A15";
		}

		// SalvaEvento(0,0,1);
		StatoMon10 = S2;
		break;
		//-------------------------------------------------------
	case S2 :
		if ( OK_Tempo == FALSE ) {
			StatoMon10 = S3_IN;
		} else {
			if ( OK_Tempo == TRUE ) {
				StatoMon10 = S4_IN;
			}
		}
		break;
		//_______________________________________________________
	case S3_IN :
		LedAllarmeFisso;
		StatoMon10 = S3;
		break;
		//-------------------------------------------------------
	case S3 :
		StatoMon10 = S4_IN;
		break;
		//_______________________________________________________
	case S4_IN :
		BlackOut = FALSE;
		StatoMon10 = S4;
		break;
		//-------------------------------------------------------
	case S4 :
		if ( ( strcmp(Str_Allarme2,"A15")!=0 ) && ( strcmp(Str_Allarme2,"A16")!=0 ) )
		{
			StatoMon10 = S1_IN;
		} else
			if ( ( TastoSelezioneBreveMon == TRUE ) && ( ( strcmp(Str_Allarme2,"A15" )==0 ) || ( strcmp(Str_Allarme2,"A16" )==0 ) ) ) {
				TastoSelezioneBreveMon = FALSE;
				StatoMon10 = S5_IN;
			}
		break;
		//_______________________________________________________
	case S5_IN :
		Str_Allarme2 = Null;
		LedAllarmeSpento;
		// OK_Tempo = TRUE;
		StatoMon10 = S5;
		break;
		//-------------------------------------------------------
	case S5 :
		if ( TRUE ) {
			StatoMon10 = S1_IN;
		}
		break;
		//_______________________________________________________
	} // END SWITCH-10
	DoIdle();

	switch ( StatoMon32 ) {                // *** SWITCH-32 ***
	//_______________________________________________________
	case S0_IN :
		StatoMon32 = S0;
		break;
		//-------------------------------------------------------
	case S0 :
		if ( TRUE ) {
			StatoMon32 = S0;
		}
		break;
		//_______________________________________________________
	case S1_IN :
		NOP;
		StatoMon32 = S1;
		break;
		//-------------------------------------------------------
	case S1 :
		if (!MIR_Enable && ( TabProg.PTV == YES ) && Prod_shake==TRUE ) {
			StatoMon32 = S2_IN;
		}
		break;
		//_______________________________________________________
	case S2_IN :
		Str_Allarme2 = "x  ";
		CntMon32 = 10;
		LedAllarmeLampeggiante;
		StatoMon32 = S2;
		break;
		//-------------------------------------------------------
	case S2 :
		if ( !CntMon32 ) {
			Play_Buzzer ( HIGH, 20 );
			CntMon32 = 10;
		}
		if ( ( (TastoSelezioneBreveMon == TRUE) && MIR_Enable ) || (strcmp(Str_Allarme2,"x  ")!=0) || ( TabProg.PTV == NO ) ||Prod_shake==FALSE) {
			TastoSelezioneBreveMon = FALSE;
			StatoMon32 = S3_IN;
		}
		break;
		//_______________________________________________________
	case S3_IN :
		BUZZER_EXT = OFF;
		if ( strcmp(Str_Allarme2,"x  ")==0 ) {
			Str_Allarme2 = Null;
		}
		Scaduto_Timer_MP = FALSE;
		LedAllarmeSpento;
		StatoMon32 = S3;
		break;
		//-------------------------------------------------------
	case S3 :
		if ( TRUE ) {
			StatoMon32 = S1_IN;
		}
		break;
		//_______________________________________________________
	} // END SWITCH-32
	DoIdle();

	//_________________________________________________
	// Aggiornamento ogni 100 mSec dei contatori attivi
	if ( Mon_100mSec ) {
		Mon_100mSec = 0;
		// *** azioni eseguite ogni 100 mSec ***
		if ( CntMon1 )   CntMon1 --;                      // contatore dello SWITCH-1
		if(CntMon2)CntMon2--;                             // contatore dello SWITCH-2
		if ( CntMonFiltroIMS ) CntMonFiltroIMS--;         // timer per filtrare il segnale dell'IMS
		if(CntMon32)CntMon32--;

		TimerRdData( &Ora );
	}

}
//_____END Monitoring()____________________________________________________________________________
//_________________________________________________________________________________________________


//_________________________________________________________________________________________________
//_____Stato 0 -> STOP( Programmazione )___________________________________________________________
UC Stop ( void )
{

	DoIdle();

	switch ( StatoStop0 ) {                 // *** SWITCH-0 ***
	//_______________________________________________________
	case S0_IN :
		StatoStop1 = S0_IN;
		StatoStop0 = S0;
		break;
		//-------------------------------------------------------
	case S0 :
		if ( TRUE ) {
			StatoStop0 = S1_IN;
		}
		break;
	case S1_IN:
		LED_STOP = ON;                    // accende il LED di STOP
		LED_PRODUZIONE = OFF;
		LED_CONSERVAZIONE = OFF;
		LED_PULIZIA = OFF;
		FFC=TRUE;
		E_V_F_C=OFF;
		FFV=TRUE;
		E_V_F_V=OFF;
		//M_A=OFF;
		M_C=OFF;
		if ( (AL == 0) && ( Str_Allarme1 == Null ) && ( Str_Allarme2 == Null )) {
			StringVal( "StP");                // scrive "Stp" sul display
		}
		SalvaStatoMacchina( State );        // **** salvataggio preventivo al black-out ****
		StatoStop0 = S1;
		break;
	case S1:
		CntStop0 = TabProg.DEL * 10;      // per contare DEL" ( DEL * 10 * 100mSec)
		StatoStop0 = END;
		StatoStop1 = S1_IN;
		break;
	}
	DoIdle();

	switch ( StatoStop1 ) {                 // *** SWITCH-1 ***
	//_______________________________________________________
	case S0_IN :
		StatoStop1 = S0;
		break;
		//-------------------------------------------------------
	case S0 :
		if ( TRUE ) {
			StatoStop1 = S0;
		}
		break;
		//_______________________________________________________
	case S1_IN :
		if ( ( Str_Allarme1 == Null ) && ( Str_Allarme2 == Null ) ) {
			if ( !S_LIV ) {
				StringVal( "StP"); // DigitValProg( /*Input0_10V*/ (UINT)In010Value );
			}
			else {
				StringVal("X  ");
			}
		} else {
			DigitVal( Cel_Far(T_E_V) );    // SERVE PER VISUALIZZARE GLI ALLARMI !!!
		}
		if(!CntStop0)M_A = OFF;
		StatoStop1 = S1;
		break;
		//-------------------------------------------------------
	case S1 :
		if ( TRUE ) {
			StatoStop1 = S2_IN;
		}
		break;
		//_______________________________________________________
	case S2_IN :
		Programmazione();                   // PROGRAMMAZIONE
		Led_2_AllarmeSpento;         // spengo il puntino dei decimali
		StatoStop1 = S2;
		break;
		//-------------------------------------------------------
	case S2 :
		if ( TRUE ) {
			StatoStop1 = S1_IN;
		}
		break;
		//_______________________________________________________
	} // END SWITCH-1

	DoIdle();

	//_________________________________________________
	// Aggiornamento ogni 100 mSec dei contatori attivi
	if ( Stop_100mSec ) {
		Stop_100mSec = 0;
		// *** azioni eseguite ogni 100 mSec ***
		if ( CntStop0 )  CntStop0 --;                     // contatore dello SWITCH-0
	}
	return 6;                                            // va in SELECT

}
//_____END Stop()__________________________________________________________________________________
//_________________________________________________________________________________________________






UC Beep_Timer;



//_________________________________________________________________________________________________
//_____Stato 1 -> PRODUZIONE_______________________________________________________________________
UC Produzione ( void )
{

	DoIdle();
	if ( ( ( Str_Allarme1 == Null ) && ( Str_Allarme2 == Null ) ) ) {  // per evitare interferenze

		// ____________________Gestione tasti________________________________________
		if ( NewKey ) {                       // Se e' stato premuto un tasto...

			// ____Gestione del tasto P2_____________________________________________
			if ( ( IS_KEY( KEY_P2 ) ) /*&& ( StatoPrd5 <= S1 )*/ ) {           // Se il tasto premuto e' P2...
				Play_Buzzer ( HIGH, 20 );
				NewKey = 0;
				while ( IS_KEY( KEY_P2 ) ) {      // Controllo del tempo di pressione del P2
					DoIdle();
					if ( PressCnt >= 1500 )
						break;
				}
				if ( IS_KEY( KEY_P2 ) ) {         // Se la pressione del P2 e' prolungata...
					// Azioni eseguite con P2 premuto per un tempo lungo
					TastoSelezioneLungo = TRUE;
				} else { // Se la pressione del P2 e' breve...
					// Azioni eseguite con P2 premuto per un tempo breve
					TastoSelezioneBreve = TRUE;
				}
			}

			// _____ Gestione del tasto P1 __________________________________________
			if ( IS_KEY( KEY_P1 ) ) {               // Se il tasto premuto e' P1...
				Play_Buzzer ( HIGH, 20 );
				NewKey = 0;
				// Azioni eseguite con P1 premuto per un tempo breve
				if ( !DisabilitaTastoStop ) {
					TastoStopBreve = TRUE;                // TASTO-STOP premuto per un tempo breve
				}
			} // END gestione tasto P1

		}
		//___________________END Gestione Tasti______________________________________
	}

	//______________________________________________________________________________
	// Condizioni di uscita da PRODUZIONE

	if ( TastoStopBreve == TRUE ) {
		TastoStopBreve = FALSE;
		TastoSelezioneBreve = FALSE;
		TastoSelezioneLungo = FALSE;
		LED_PRODUZIONE = OFF;
		LED_CONSERVAZIONE = OFF;
		LED_STOP = ON;
		StatoStop0 = S0_IN;
		return 0;                             // esce e va in STOP forzatamente
	} else if ( AL > 0 ) {
		TastoSelezioneBreve = FALSE;
		TastoSelezioneLungo = FALSE;
		LED_PRODUZIONE = OFF;
		StringVal( "   " );
		Wait10mSec( 40 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StatoStop0 = S0_IN;
		return 0;                             // esce e va in STOP forzatamente
	}
	//______________________________________________________________________________
	switch ( StatoPrd0 ) {                  // *** SWITCH-0 ***
	//_______________________________________________________
	case S0_IN :
		StatoPrd1=S0_IN;
		StatoPrd2=S0_IN;
		StatoPrd3=S0_IN;
		StatoPrd4=S0_IN;
		StatoPrd5=S0_IN;
		StatoPrd6=S0_IN;
		StatoPrd10=S0_IN;
		MIR_Enable= TRUE;
		FFC=TRUE;
		E_V_F_C=OFF;
		FFV=TRUE;
		E_V_F_V=OFF;
		M_C =OFF;
		M_A=OFF;
		CntPrd0 = 20;
		StatoPrd0=S0;
		break;
	case S0:
		if(!CntPrd0){
			StatoPrd0=S1_IN;
		}
		break;
	case S1_IN:
		M_C =ON;
		M_A=ON;
		CntPrd0 = 10;
		StatoPrd0 = S1;
		break;
	case S1:
		if(!CntPrd0){
			StatoPrd0 = S2_IN;
		}
		break;
	case S2_IN:
		StatoPrd1=S1_IN;
		StatoPrd2=S1_IN;
		StatoPrd3=S1_IN;
		StatoPrd4=S1_IN;
		StatoPrd5=S1_IN;
		StatoPrd6=S1_IN;
		StatoPrd10=S1_IN;
		LED_STOP = OFF;
		LED_PRODUZIONE = ON;
		LED_PULIZIA = OFF;
		SalvaStatoMacchina( State );        // **** salvataggio preventivo al black-out ****
		StatoPrd0 = S2;
		break;
	case S2:
		StatoPrd0 = END;
		break;
	}

	DoIdle();
	switch ( StatoPrd1 ) {                  // *** SWITCH-1 ***
	//_______________________________________________________
	case S0_IN :
		StatoPrd1 = S0;
		break;
		//-------------------------------------------------------
	case S0 :
		if ( TRUE ) {
			StatoPrd1 = S0;
		}
		break;
	case S1_IN :
		M_A=ON;
		StatoPrd1 = S1;
		break;
	}
	DoIdle();
	switch ( StatoPrd2 ) {                  // *** SWITCH-2 ***
	//_______________________________________________________
	case S0_IN :
		StatoPrd2 = S0;
		break;
		//-------------------------------------------------------
	case S0 :
		if ( TRUE ) {
			StatoPrd2 = S0;
		}
		break;
	case S1_IN :
		StatoPrd2 = S1;
		break;
	case S1 :
		if(H_O_T < Set_HOT-HysteresisHOT && Prod_shake==FALSE){
			StatoPrd2 = S2_IN;
		}
		break;
	case S2_IN :
		FFC=FALSE;
		E_V_F_C=ON;
		StatoPrd2 = S2;
		break;
	case S2:
		if(H_O_T >= Set_HOT){
			StatoPrd2 = S3_IN;
		}
		break;
	case S3_IN :
		FFC=TRUE;
		E_V_F_C=OFF;
		StatoPrd2 = S3;
		break;
	case S3 :
		StatoPrd2 = S1_IN;
		break;
	}
	DoIdle();
	switch ( StatoPrd3 ) {                  // *** SWITCH-3 ***
	//_______________________________________________________
	case S0_IN :
		StatoPrd3 = S0;
		break;
		//-------------------------------------------------------
	case S0 :
		if ( TRUE ) {
			StatoPrd3 = S0;
		}
		break;
	case S1_IN :
		StatoPrd3 = S1;
		break;
	case S1 :
		if(T_E_V >= ( TabProg.STO + IstSTO )){
			StatoPrd3 = S2_IN;
		}
		break;
	case S2_IN :
		FFV=FALSE;
		E_V_F_V=ON;
		StatoPrd3 = S2;
		break;
	case S2:
		if(T_E_V <  TabProg.STO){
			StatoPrd3 = S3_IN;
		}
		break;
	case S3_IN :
		FFV=TRUE;
		E_V_F_V=OFF;
		StatoPrd3 = S3;
		break;
	case S3 :
		StatoPrd3 = S1_IN;
		break;
	}
	DoIdle();
	switch ( StatoPrd4 ) {                  // *** SWITCH-4 ***
	//_______________________________________________________
	case S0_IN :
		StatoPrd4 = S0;
		break;
		//-------------------------------------------------------
	case S0 :
		if ( TRUE ) {
			StatoPrd4 = S0;
		}
		break;
		//_______________________________________________________
	case S1_IN :
		StatoPrd4 = S1;
		break;
		//-------------------------------------------------------
	case S1 :
		if(!S_LIV)DigitVal( Cel_Far(T_E_C) );
		if ( TastoSelezioneBreve == TRUE ) {
			TastoSelezioneBreve = FALSE;
			StatoPrd4 = S2_IN;
		}
		break;
		//_______________________________________________________
	case S2_IN :
		StatoPrd4 = S2;
		CntPrd4 = 5 * 10;                  // per contare 5" (5 * 10 * 100mSec)
		break;
		//-------------------------------------------------------
	case S2 :
		if(!S_LIV)DigitVal( Cel_Far(T_E_V) );
		if ( TastoSelezioneBreve == TRUE ) {
			TastoSelezioneBreve = FALSE;
			StatoPrd4 = S3_IN;
		}else if(!CntPrd4){
			StatoPrd4 = S1_IN;
		}
		break;
	case S3_IN :
		StatoPrd4 = S3;
		CntPrd4 = 5 * 10;                  // per contare 5" (5 * 10 * 100mSec)
		break;
		//-------------------------------------------------------
	case S3 :
		if(!S_LIV)DisplayHot( H_O_T );
		if ( TastoSelezioneBreve == TRUE) {
			TastoSelezioneBreve = FALSE;
			StatoPrd4 = S1_IN;
		}else if(!CntPrd4){
			StatoPrd4 = S1_IN;
		}
		break;
	/*case S4_IN :
		StatoPrd4 = S4;
		CntPrd4 = 5 * 10;                  // per contare 5" (5 * 10 * 100mSec)
		break;
		//-------------------------------------------------------
	case S4 :
		if(Prod_shake==TRUE)StringVal( "Sha" );
		//else if(Prod_shake==FALSE)StringVal( "Slu" );
		if ( TastoSelezioneBreve == TRUE) {
			TastoSelezioneBreve = FALSE;
			StatoPrd4 = S1_IN;
		}else if(!CntPrd4){
			StatoPrd4 = S1_IN;
		}
		break;*/
		//_______________________________________________________
		//_______________________________________________________

	} // END SWITCH-4

	DoIdle();
	switch ( StatoPrd5 ) {                  // *** SWITCH-5 ***
	//_______________________________________________________
	case S0_IN :
		StatoPrd5 = S0;
		break;
		//-------------------------------------------------------
	case S0 :
		if ( TRUE ) {
			StatoPrd5 = S0;
		}
		break;
	case S1_IN :
		StatoPrd5 = S1;
		break;
	case S1:
		if(S_LIV){
			StatoPrd5 = S2_IN;
		}
		break;
	case S2_IN:
		CntPrd5=50;
		StatoPrd5 = S2;
		break;
	case S2:
		if(S_LIV &&!CntPrd5 ){
			StatoPrd5 = S3_IN;
		}else if(!S_LIV){
			StatoPrd5 = S1;
		}
		break;
	case S3_IN:
		if (TRUE ) {
			if ( CntPrd5 >= 15 ) {
				StringVal("X  ");
			} else if ( ( CntPrd5 < 15 ) && ( CntPrd5 > 0 ) ) {
				StringVal("   ");
			} else {
				Play_Buzzer ( HIGH, 100 );
				CntPrd5 = 30;
			}
		}
		if (  !S_LIV ) {
			TastoSelezioneBreve = FALSE;
			StringVal("RSF");
			Wait100mSec( 10 );
			StatoPrd5 = S1_IN;
		}
		break;
	case S3:

		break;
	}// END SWITCH-5
	DoIdle();
	switch ( StatoPrd6 ) {                  // *** SWITCH-6 ***
	//_______________________________________________________
	case S0_IN :
		StatoPrd6 = S0;
		break;
		//-------------------------------------------------------
	case S0 :
		if ( TRUE ) {
			StatoPrd6 = S0;
		}
		break;
	case S1_IN :
		StatoPrd6 = S1;
		break;
	case S1:
		if(Prod_shake == TRUE){
			StatoPrd6 = S2_IN;
		}
		break;
	case  S2_IN:
		M_A = ON;
		CntPrd6=20;
		StatoPrd6 = S2;
		break;
	case S2:
		if(!CntPrd6){
			StatoPrd6 = S3_IN;
		}
		break;
	case S3_IN:
		CntPrd6=15*60*10;
		StatoPrd6 = S3;
		break;
	case S3:
		if(H_O_T < Set_HOT-HysteresisHOT ){
			StatoPrd6 = S4_IN;
		}else if(!CntPrd6){
			StatoPrd6 = S6_IN;
		}
		break;
	case S4_IN :
		FFC=FALSE;
		E_V_F_C=ON;
		StatoPrd6 = S4;
		break;
	case S4:
		if(H_O_T >= Set_HOT){
			StatoPrd6 = S5_IN;
		}
		break;
	case S5_IN :
		FFC=TRUE;
		E_V_F_C=OFF;
		StatoPrd6 = S5;
		break;
	case S5 :
		StatoPrd6 = S3;
		break;
	case S6_IN:
		M_A=ON;
		CntPrd6=20;
		StatoPrd6=S6;
		break;
	case S6:
		if(!CntPrd6){
			StatoPrd6=S7_IN;
		}
		break;
	case S7_IN:
		CntPrd6=200;
		StatoPrd6=S7;
		break;
	case S7:
		if(H_O_T < Set_HOT-HysteresisHOT ){
			StatoPrd6 = S8_IN;
		}else if(!CntPrd6 && (M_I_R==OFF||MIR_Enable==OFF)){
			StatoPrd6 = S9_IN;
		}
		break;
	case S8_IN :
		FFC=FALSE;
		E_V_F_C=ON;
		StatoPrd6 = S8;
		break;
	case S8:
		if(H_O_T >= Set_HOT){
			StatoPrd6 = S9_IN;
		}
		break;
	case S9_IN :
		FFC=TRUE;
		E_V_F_C=OFF;
		CntPrd6=10*60*10; // 10 min *10
		StatoPrd6 = S9;
		StatoPrd7 =S1_IN; // RITARDO MA
		break;
	case S9 :
		if(!CntPrd6|| (M_I_R==ON && MIR_Enable==TRUE)){
			StatoPrd6 = S6_IN;
			StatoPrd7 =S0_IN;
		}
		break;

	} // *** END SWITCH-6 ***
	DoIdle();

	switch ( StatoPrd7 ) {                  // *** SWITCH-7 ***
	//_______________________________________________________
	case S0_IN :
		StatoPrd7 = S0;
		break;
		//-------------------------------------------------------
	case S0 :
		if ( TRUE ) {
			StatoPrd7 = S0;
		}
		break;
	case S1_IN :
		M_A=ON;
		CntPrd7= TabProg.DEL * 10;      // per contare DEL" ( DEL * 10 * 100mSec)
		StatoPrd7 = S1;
		break;
	case S1:
		if(!CntPrd7){
			StatoPrd7 = S2_IN;
		}
		break;
	case S2_IN:
		M_A=OFF;
		StatoPrd7 = S2;
		break;
	case S2:
		break;
	case S3_IN:
		break;
	case S3:
		break;

	}// *** END SWITCH-7 ***
	DoIdle();
	switch ( StatoPrd10 ) {                  // *** SWITCH-10 ***
	//_______________________________________________________
	case S0_IN :
		StatoPrd10 = S0;
		break;
		//-------------------------------------------------------
	case S0 :
		if ( TRUE ) {
			StatoPrd10 = S0;
		}
		break;
		//_______________________________________________________
	case S1_IN :
		NOP;
		StatoPrd10 = S1;
		break;
		//-------------------------------------------------------
	case S1 :
		if ( M_I_R == ON && Prod_shake== TRUE) {
			StatoPrd10 = S2_IN;
		}
		break;
		//_______________________________________________________
	case S2_IN :
		CntPrd10 = /*15*/ 30 * 10;               //  per contare 15" ( 15 *10 * 100mSec )
		StatoPrd10 = S2;
		break;
		//-------------------------------------------------------
	case S2 :
		if ( CntPrd10 == 0 ) {
			StatoPrd10 = S3_IN;
		} else {
			if ( M_I_R == OFF ) {
				StatoPrd10 = S1_IN;
			}
		}
		break;
		//_______________________________________________________
	case S3_IN :
		MIR_Enable = FALSE;
		StatoPrd10 = S3;
		break;
		//-------------------------------------------------------
	case S3 :
		if ( M_I_R == OFF ) {
			StatoPrd10 = S4_IN;
		}
		break;
		//_______________________________________________________
	case S4_IN :
		MIR_Enable = TRUE;
		StatoPrd10 = S4;
		break;
		//-------------------------------------------------------
	case S4 :
		if ( TRUE ) {
			StatoPrd10 = S1_IN;
		}
		break;
		//_______________________________________________________
	} // END SWITCH-10
	DoIdle();
	// Aggiornamento ogni 10 mSec dei contatori attivi
	if ( Prd_10mSec ) {
		Prd_10mSec = 0;
		//______________________________________
	}
	// Aggiornamento ogni 100 mSec dei contatori attivi
	if ( Prd_100mSec ) {
		Prd_100mSec = 0;
		if(CntPrd0)CntPrd0--;
		if(CntPrd1)CntPrd1--;
		if(CntPrd2)CntPrd2--;
		if(CntPrd3)CntPrd3--;
		if(CntPrd4)CntPrd4--;
		if(CntPrd5)CntPrd5--;
		if(CntPrd6)CntPrd6--;
		if(CntPrd7)CntPrd7--;
		if(CntPrd10)CntPrd10--;
	}
	// Aggiornamento ogni 1 Sec dei contatori attivi
	if ( Prd_1Sec ) {
		Prd_1Sec = 0;
	}
	if(Prod_shake==FALSE){
		return 1;                             // rimane in PRODUZIONE SLUSH
	}else{
		return 4;                             // rimane in PRODUZIONE SHAKE
	}
	return 1;                             // rimane in PRODUZIONE SLUSH

}
//_____END Produzione()____________________________________________________________________________
//_________________________________________________________________________________________________










//_________________________________________________________________________________________________
//_____Stato 2 -> CONSERVAZIONE (STORAGE)__________________________________________________________
UC Conservazione ( void )
{
	DoIdle();


	if ( ( Str_Allarme1 == Null ) && ( Str_Allarme2 == Null ) ) {  // per evitare interferenze

		// ____________________Gestione tasti_______________________________________
		if ( NewKey ) {                       // Se e' stato premuto un tasto...

			// _____ Gestione del tasto P1 _________________________________________
			if ( IS_KEY( KEY_P1 ) ) {               // Se il tasto premuto e' P1...
				NewKey = 0;
				// Azioni eseguite con P1 premuto per un tempo breve
				TastoStopBreve = TRUE;                // TASTO-STOP premuto per un tempo breve
			} // END gestione tasto P1

			// ____Gestione del tasto P2____________________________________________
			if ( IS_KEY( KEY_P2 ) ) {           // Se il tasto premuto e' P2...
				NewKey = 0;
				while ( IS_KEY( KEY_P2 ) ) {      // Controllo del tempo di pressione del P2
					DoIdle();
					if ( PressCnt >= 300 )
						break;
				}
				if ( IS_KEY( KEY_P2 ) ) {         // Se la pressione del P2 e' prolungata...
					// Azioni eseguite con P2 premuto per un tempo lungo 5"
					TastoSelezioneLungo = TRUE;
				} else { // Se la pressione del P2 e' breve...
					// Azioni eseguite con P2 premuto per un tempo breve
					TastoSelezioneBreve = TRUE;
				}
			}

		}
		//___________________END Gestione Tasti_____________________________________
	}
	//______________________________________________________________________________
	// Condizioni di uscita da CONSERVAZIONE

	if ( TastoStopBreve == TRUE ) {
		TastoStopBreve = FALSE;
		TastoSelezioneBreve = FALSE;
		TastoSelezioneLungo = FALSE;
		LED_CONSERVAZIONE = OFF;
		StringVal( "   " );
		Wait10mSec( 40 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StatoStop0 = S0_IN;
		return 0;                             // va in STOP forzatamente
	} else
		if ( ( AL > 0 ) || ( Flag_TEC == TRUE ) ) {
			TastoSelezioneBreve = FALSE;
			TastoSelezioneLungo = FALSE;
			LED_CONSERVAZIONE = OFF;
			StringVal( "   " );
			Wait10mSec( 40 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
			StatoStop0 = S0_IN;
			return 0;                             // va in STOP forzatamente
		}  //______________________________________________________________________________

	//______________________________________________________________________________
	switch ( StatoSto0 ) {                  // *** SWITCH-0 ***
	//_______________________________________________________
	case S0_IN :
		StatoSto1=S0_IN;
		StatoSto2=S0_IN;
		StatoSto3=S0_IN;
		StatoSto4=S0_IN;
		StatoSto5=S0_IN;
		M_C =OFF;
		M_A=OFF;
		FFC=TRUE;
		E_V_F_C=OFF;
		FFV=TRUE;
		E_V_F_V=OFF;
		CntSto0 = 20;
		StatoSto0=S0;
		break;
	case S0:
		if(!CntSto0){
			StatoSto0=S1_IN;
		}
		break;
	case S1_IN:
		M_C =ON;
		M_A=ON;
		LED_STOP = OFF;
		LED_CONSERVAZIONE = ON;
		LED_PULIZIA = OFF;
		LED_PRODUZIONE = OFF;
		SalvaStatoMacchina( State );        // **** salvataggio preventivo al black-out ****
		CntSto0 = 10;
		StatoSto0 = S1;
		break;
	case S1:
		if(!CntSto0){
			StatoSto0 = S2_IN;
		}
		break;
	case S2_IN:
		StatoSto1=S1_IN;
		StatoSto2=S1_IN;
		StatoSto3=S1_IN;
		StatoSto4=S1_IN;
		StatoSto5=S1_IN;
		StatoSto0 = S2;
		break;
	case S2:
		StatoSto0 = END;
		break;
	}

	DoIdle();
	switch ( StatoSto1 ) {                  // *** SWITCH-1 ***
	//_______________________________________________________
	case S0_IN :
		StatoSto1 = S0;
		break;
		//-------------------------------------------------------
	case S0 :
		if ( TRUE ) {
			StatoSto1 = S0;
		}
		break;
	case S1_IN :
		M_A=ON;
		StatoSto1 = S1;
		break;
	}
	DoIdle();
	switch ( StatoSto2 ) {                  // *** SWITCH-2 ***
	//_______________________________________________________
	case S0_IN :
		StatoSto2 = S0;
		break;
		//-------------------------------------------------------
	case S0 :
		if ( TRUE ) {
			StatoSto2 = S0;
		}
		break;
	case S1_IN :
		StatoSto2 = S1;
		break;
	case S1 :
		if(T_E_C >= ( TabProg.STO + IstSTO ) && H_O_T < Set_HOT){
			StatoSto2 = S2_IN;
		}
		break;
	case S2_IN :
		FFC=FALSE;
		E_V_F_C=ON;
		StatoSto2 = S2;
		break;
	case S2:
		if(H_O_T >= Set_HOT ||T_E_C <  TabProg.STO){
			StatoSto2 = S3_IN;
		}
		break;
	case S3_IN :
		FFC=TRUE;
		E_V_F_C=OFF;
		StatoSto2 = S3;
		break;
	case S3 :
		StatoSto2 = S1_IN;
		break;
	}
	DoIdle();
	switch ( StatoSto3 ) {                  // *** SWITCH-3 ***
	//_______________________________________________________
	case S0_IN :
		StatoSto3 = S0;
		break;
		//-------------------------------------------------------
	case S0 :
		if ( TRUE ) {
			StatoSto3 = S0;
		}
		break;
	case S1_IN :
		StatoSto3 = S1;
		break;
	case S1 :
		if(T_E_V >= ( TabProg.STO + IstSTO )){
			StatoSto3 = S2_IN;
		}
		break;
	case S2_IN :
		FFV=FALSE;
		E_V_F_V=ON;
		StatoSto3 = S2;
		break;
	case S2:
		if(T_E_V <  TabProg.STO){
			StatoSto3 = S3_IN;
		}
		break;
	case S3_IN :
		FFV=TRUE;
		E_V_F_V=OFF;
		StatoSto3 = S3;
		break;
	case S3 :
		StatoSto3 = S1_IN;
		break;
	}
	DoIdle();
	switch ( StatoSto4 ) {                  // *** SWITCH-4 ***
	//_______________________________________________________
	case S0_IN :
		StatoSto4 = S0;
		break;
		//-------------------------------------------------------
	case S0 :
		if ( TRUE ) {
			StatoSto4 = S0;
		}
		break;
		//_______________________________________________________
	case S1_IN :
		StatoSto4 = S1;
		break;
		//-------------------------------------------------------
	case S1 :
		if(!S_LIV)DigitVal( Cel_Far(T_E_C) );
		if ( TastoSelezioneBreve == TRUE ) {
			TastoSelezioneBreve = FALSE;
			StatoSto4 = S2_IN;
		}
		break;
		//_______________________________________________________
	case S2_IN :
		StatoSto4 = S2;
		CntSto4 = 5 * 10;                  // per contare 5" (5 * 10 * 100mSec)
		break;
		//-------------------------------------------------------
	case S2 :
		if(!S_LIV)DigitVal( Cel_Far(T_E_V) );
		if ( TastoSelezioneBreve == TRUE) {
			TastoSelezioneBreve = FALSE;
			StatoSto4 = S3_IN;
		}else if(!CntSto4){
			StatoSto4 = S1_IN;
		}
		break;
	case S3_IN :
		StatoSto4 = S3;
		CntSto4 = 5 * 10;                  // per contare 5" (5 * 10 * 100mSec)
		break;
		//-------------------------------------------------------
	case S3 :
		if(!S_LIV)DisplayHot( H_O_T );;
		if ( TastoSelezioneBreve == TRUE) {
			TastoSelezioneBreve = FALSE;
			StatoSto4 = S1_IN;
		}else if(!CntSto4){
			StatoSto4 = S1_IN;
		}
		break;
		//_______________________________________________________
		//_______________________________________________________

	} // END SWITCH-4
	DoIdle();
	switch ( StatoSto5 ) {                  // *** SWITCH-5 ***
	//_______________________________________________________
	case S0_IN :
		StatoSto5 = S0;
		break;
		//-------------------------------------------------------
	case S0 :
		if ( TRUE ) {
			StatoSto5 = S0;
		}
		break;
	case S1_IN :
		StatoSto5 = S1;
		break;
	case S1:
		if(S_LIV){
			StatoSto5 = S2_IN;
		}
		break;
	case S2_IN:
		CntSto5=50;
		StatoSto5 = S2;
		break;
	case S2:
		if(S_LIV &&!CntSto5 ){
			StatoSto5 = S3_IN;
		}else if(!S_LIV){
			StatoSto5 = S1;
		}
		break;
	case S3_IN:
		if (TRUE ) {
			if ( CntSto5 >= 15 ) {
				StringVal("X  ");
			} else if ( ( CntSto5 < 15 ) && ( CntSto5 > 0 ) ) {
				StringVal("   ");
			} else {
				Play_Buzzer ( HIGH, 100 );
				CntSto5 = 30;
			}
		}
		if (  !S_LIV ) {
			TastoSelezioneBreve = FALSE;
			StringVal("RSF");
			Wait100mSec( 10 );
			StatoSto5 = S1_IN;
		}
		break;
	case S3:

		break;
	}// END SWITCH-5
	DoIdle();
	// Aggiornamento ogni 100 mSec dei contatori attivi
	if ( Sto_100mSec ) {
		Sto_100mSec = 0;
		if(CntSto4)CntSto4--;
		if(CntSto5)CntSto5--;
		if(CntSto0)CntSto0--;

	}

	return 2;                               // rimane in CONSERVAZIONE

}
//_____END Conservazione()_________________________________________________________________________
//_________________________________________________________________________________________________










//_________________________________________________________________________________________________
//_____Stato 3 -> PULIZIA__________________________________________________________________________
UC Pulizia ( void )
{

	DoIdle();


	if ( ( ( Str_Allarme1 == Null ) && ( Str_Allarme2 == Null ) ) || ( AL > 0 ) ) {  // per evitare interferenze

		// ____________________Gestione tasti_______________________________________
		if ( NewKey ) {                         // Se e' stato premuto un tasto...

			// ____Gestione del tasto P1____
			if ( IS_KEY( KEY_P1 ) ) {           // Se il tasto premuto e' P1...
				Play_Buzzer ( HIGH, 20 );
				NewKey = 0;
				// Azioni eseguite con P1 premuto per un tempo breve
				TastoStopBreve = TRUE;            // TASTO-STOP premuto per un tempo breve
			}

			// ____Gestione del tasto P2____
			if ( IS_KEY( KEY_P2 ) ) {           // Se il tasto premuto e' P2...
				Play_Buzzer ( HIGH, 20 );
				NewKey = 0;
				while ( IS_KEY( KEY_P2 ) ) {      // Controllo del tempo di pressione del P2
					DoIdle();
					if ( PressCnt >= 500 )
						break;
				}
				if ( IS_KEY( KEY_P2 ) ) {         // Se la pressione del P2 e' prolungata...
					// Azioni eseguite con P2 premuto per un tempo lungo 3"
					TastoSelezioneLungo = TRUE;   // TASTO-SELEZIONE premuto per un tempo breve
				} else { // Se la pressione del P2 e' breve...
					// Azioni eseguite con P2 premuto per un tempo breve
					TastoSelezioneBreve = TRUE;   // TASTO-SELEZIONE premuto per un tempo lungo
				}
			}

		}
		//______________________Fine Gestione Tasti_________________________________
	}
	if ( TastoStopBreve == TRUE ) {
		TastoStopBreve = FALSE;
		TastoSelezioneBreve = FALSE;
		TastoSelezioneLungo = FALSE;
		LED_PULIZIA = OFF;
		StringVal( "   " );
		Wait10mSec( 40 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
		StatoStop0 = S0_IN;
		return 0;                             // va in STOP forzatamente
	} else
		if ( ( AL > 0 )  ) {
			TastoSelezioneBreve = FALSE;
			TastoSelezioneLungo = FALSE;
			LED_PULIZIA = OFF;
			StringVal( "   " );
			Wait10mSec( 40 );  // !!! evita che vada in stop quando manca tensione causa i disturbi dei motori
			StatoStop0 = S0_IN;
			return 0;                             // va in STOP forzatamente
		}  //______________________________________________________________________________

	switch ( StatoCle0 ) {                  // *** SWITCH-0 ***
	//_______________________________________________________
	case S0_IN :
		StatoCle0 = S0;
		break;
		//-------------------------------------------------------
	case S0 :
		if ( TRUE ) {
			StatoCle0 = S1_IN;
		}
		break;
		//_______________________________________________________
	case S1_IN :
		M_C =OFF;
		M_A=OFF;
		FFC=TRUE;
		E_V_F_C=OFF;
		FFV=TRUE;
		E_V_F_V=OFF;
		LED_STOP = OFF;
		LED_PRODUZIONE = OFF;
		LED_CONSERVAZIONE = OFF;
		LED_PULIZIA = ON;  // accende il LED di PULIZIA
		SalvaStatoMacchina( State );        // **** salvataggio preventivo al black-out ****
		CntCle0=20;
		StatoCle0 = S1;
		break;
		//-------------------------------------------------------
	case S1 :
		if ( !CntCle0 ) {
			StatoCle0 = S2_IN;
		}
		break;
	case S2_IN:
		M_A =ON;
		StatoCle0 = S2;
		break;
	case S2:
		StatoCle0=END;
		break;
		//_______________________________________________________
	}

	DoIdle();
	// Aggiornamento ogni 100 mSec dei contatori attivi
	if ( Cle_100mSec ) {
		Cle_100mSec = 0;
		if(CntCle0)CntCle0--;
	}
	return 3;                                                 // rimane in PULIZIA

}
//_____END Pulizia()_______________________________________________________________________________
//_________________________________________________________________________________________________


BYTE CodeRS232;



//_________________________________________________________________________________________________
//_____Stato 6 -> SELECT___________________________________________________________________________
UC Select ( void )
{

	UC OutState;              // e' la variabile di ritorno della Select()
	WORD TimePress;           // per contare il tempo di pressione dei tasti

	DoIdle();

	if ( TabProg.HOT < 100 ){
		HysteresisHOT = 4;
	} else if ( TabProg.HOT >=101 && TabProg.HOT < 104 ){
		HysteresisHOT = 5;
	} else if ( TabProg.HOT >=104 && TabProg.HOT < 108 ){
		HysteresisHOT = 6;
	} else if ( TabProg.HOT >=108 && TabProg.HOT < 112 ){
		HysteresisHOT = 7;
	} else if ( TabProg.HOT >=112 && TabProg.HOT < 116 ){
		HysteresisHOT = 8;
	} else if ( TabProg.HOT >=116 && TabProg.HOT < 120 ){
		HysteresisHOT = 9;
	} else if ( TabProg.HOT >= 120 ){
		HysteresisHOT = 11;
	}



	if ( ( ( Str_Allarme1 == Null ) || ( strcmp(Str_Allarme1,"A05")==0 ) ) &&
			( ( Str_Allarme2 == Null ) || ( strcmp(Str_Allarme2,"Low")==0 ) || ( strcmp(Str_Allarme2,"   ")==0 )
					|| ( strcmp(Str_Allarme2,"A07")==0 ) ) )
	{  // per evitare interferenze

		// ________________________ Gestione tasti ______________________________________
		if ( NewKey ) {                             // se e' stato premuto un tasto...


			// _____ Gestione del tasto P1 ________________________________________
			if ( IS_KEY( KEY_P1 ) ) {               // Se il tasto premuto e' P1...
				Play_Buzzer ( HIGH, 20 );
				NewKey = 0;
				while ( IS_KEY( KEY_P1 ) ) {      // Controllo del tempo di pressione del P2
					DoIdle();
					if ( PressCnt >= 1000 ) {
						TastoStopLungo = TRUE;   // TASTO-STOP premuto per un tempo lungo
						break; }
				}
				// Azioni eseguite con P1 premuto per un tempo breve

				if ( IS_KEY( KEY_P1 ) ) { // Se la pressione del P1 e' prolungata...
					// Azioni eseguite con P1 premuto per un tempo lungo
					TastoStopLungo = TRUE;   // TASTO-STOP premuto per un tempo lungo
				} else
					TastoStopBreve = TRUE;                   // TASTO-STOP premuto per un tempo breve

			} // END gestione tasto P1



			// ____Gestione del tasto P2_________________________________________________
			if ( IS_KEY( KEY_P2 ) ) {               // Se il tasto premuto e' P2...
				Play_Buzzer ( HIGH, 20 );
				NewKey = 0;
				while ( IS_KEY( KEY_P2 ) ) {          // Controllo del tempo di pressione del P2
					DoIdle();
					TimePress = PressCnt;
					if ( PressCnt >= 1000 )              // pressione di 10" ( usata in STOP )
						break;
				}
				if ( TimePress >= 1000 ) {             // pressione di 10" ( usata in STOP )
					// Azioni eseguite con P2 premuto per un tempo extra-lungo
					TastoSelezioneExtraLungo = TRUE;  // TASTO-SELEZIONE premuto per un tempo extra-lungo
				} else {
					if ( TimePress >= 500 ) {         // pressione di 5" ( usata in STOP )
						// Azioni eseguite con P2 premuto per un tempo lungo
						TastoSelezioneLungo = TRUE;   // TASTO-SELEZIONE premuto per un tempo lungo
					} else {
						// Azioni eseguite con P2 premuto per un tempo breve
						TastoSelezioneBreve = TRUE;   // TASTO-SELEZIONE premuto per un tempo corto
					}
				}
			} // END gestione tasto P2


		}
		// ________________________ Fine Gestione Tasti _____________________________________
	}



	CodeRS232 = ControllaRS232( True );
	if ( (CodeRS232 == RQ_TX_LOG) ||
			(CodeRS232 == RQ_TX_TABPROG) ||
			(CodeRS232 == RQ_RX_TABPROG) ) {
		StatoSelect = S5_IN;
	}
	DoIdle();
	switch ( StatoSelect ) {
	//_____________________________________________________________________
	case S0_IN :                              // seleziona STOP
		LED_STOP = ON;            // accende il LED di STOP
		All_Out_Off();
		LED_PRODUZIONE = OFF;
		LED_CONSERVAZIONE = OFF;
		LED_PULIZIA = OFF;
		StatoSelect = S0;
		OutState = 6;                           // rimane in SELECT
		break;
		//---------------------------------------------------------------------
	case S0 :
		if ( (TastoSelezioneBreve == TRUE) || BlackoutState ) {
			TastoSelezioneBreve = FALSE;
			StatoSelect = S1_IN;                  // per andare in SELECT-SLUSH
			if(BlackoutState==1){
				StatoSelect = S1_IN;   // per andare in SELECT-PRODUZIONE
				BlackoutState=0;
			}else if(BlackoutState==2){
				StatoSelect = S2_IN;   // per andare in SELECT-CONSERVAZIONE
				BlackoutState=0;
			}else if(BlackoutState==3){
				StatoSelect = S3_IN;   // per andare in SELECT-PULIZIA
				BlackoutState=0;
			}else if(BlackoutState==4){
				StatoSelect = S4_IN;   // per andare in SELECT-SHAKE
				BlackoutState=0;
			}
			OutState = 6;                         // rimane in SELECT
		} else {
			StatoSelect = S0_IN;                // per tornare in SELECT-STOP
			OutState = 0;                       // va in STOP
		}
		break;
		//_____________________________________________________________________
	case S1_IN :                              // seleziona PRODUZIONE
		LED_STOP = OFF;
		LED_PRODUZIONE = ON;
		LED_CONSERVAZIONE = OFF;
		LED_PULIZIA = OFF;
		CntSelect = 20;
		StringVal( "Slu" );
		StatoSelect = S1;
		OutState = 6;                           // rimane in SELECT
		break;
		//---------------------------------------------------------------------
	case S1 :
		if ( (TastoSelezioneBreve == TRUE)) {
			TastoSelezioneBreve = FALSE;
			StatoSelect = S2_IN;              // per andare in SELECT-CONSARVAZIONE
			OutState = 6;                     // rimane in SELECT
		} else if ( CntSelect == 0 ) {           // dopo 2"...
			StatoSelect = S0_IN;            // per tornare in SELECT-STOP
			StatoPrd0 = S0_IN;              // iniz. stato per lo SWITCH-0 di PRODUZIONE
			Set_HOT = TabProg.HOT;
			Prod_Per_Cons = FALSE;
			Prod_shake=FALSE;
			OutState = 1;                   // va in PRODUZIONE
		} else if (TastoStopBreve == TRUE){
			TastoStopBreve = FALSE;
			StatoSelect = S0_IN;
			OutState = 0;     // va in STOP
		}else{
			OutState = 6;                 // rimane in SELECT
		}
		break;

		//_____________________________________________________________________
	case S2_IN :                            // seleziona CONSERVAZIONE (STOCCAGGIO)
		StringVal( "Sto" );
		LED_STOP = OFF;
		LED_PRODUZIONE = OFF;
		LED_CONSERVAZIONE = ON;
		LED_PULIZIA = OFF;
		CntSelect = 20;                       // per contare 2" (20 * 100mSec)
		StatoSelect = S2;
		OutState = 6;                         // rimane in SELECT
		break;
		//---------------------------------------------------------------------
	case S2 :
		if ( ( TastoStopBreve == TRUE ) /* || ( AL > 0 ) */ ) {
			TastoStopBreve = FALSE;
			StatoSelect = S3_IN;                  // per andare in SELECT-CLEAN
			OutState = 6;                         // rimane in SELECT
		} else
			if ( ( TastoSelezioneBreve == TRUE )) {
				TastoSelezioneBreve = FALSE;
				StatoSelect = S3_IN;              // per andare in SELECT-PULIZIA
				OutState = 6;                     // rimane in SELECT
			} else
				if ( CntSelect == 0 ) {         // dopo 2"...
					StatoSelect = S0_IN;          // per tornare in SELECT-STOP
					StatoSto0 = S0_IN;              // iniz. stato per lo SWITCH-0 di CONSERVAZIONE
					Set_HOT = TabProg.HOT;
					OutState = 2;                 // va in CONSERVAZIONE
				} else
					OutState = 6;               // rimane in SELECT
		break;

		//_____________________________________________________________________
	case S3_IN :                          // seleziona PULIZIA
		StringVal( "ClE" );
		LED_STOP = OFF;
		LED_PRODUZIONE = OFF;
		LED_CONSERVAZIONE = OFF;
		LED_PULIZIA = ON;
		CntSelect = 20;                     // per contare 2" (20 * 100mSec)
		StatoSelect = S3;
		OutState = 6;                         // rimane in SELECT
		break;
		//---------------------------------------------------------------------
	case S3 :
		if ( ( TastoSelezioneBreve == TRUE )) {
			TastoSelezioneBreve = FALSE;
			StatoSelect = S4_IN;              // per andare in SELECT-PRODUZIONE SHAKE
			OutState = 6;                     // rimane in SELECT
		}else if ( CntSelect == 0 ) {         // dopo 2"...
			StatoSelect = S0_IN;          // per tornare in SELECT-STOP
			StatoCle0 = S0_IN;            // iniz. stato per lo SWITCH-0 di PULIZIA
			OutState = 3;                 // va in PULIZIA
		} else if (TastoStopBreve == TRUE) {
			TastoStopBreve = FALSE;
			StatoSelect = S0_IN;
			OutState = 0; //VA IN STOP
		}else{
			OutState = 6;               // rimane in SELECT
		}
		break;
		//_____________________________________________________________________
	case S4_IN :                          // seleziona PULIZIA
		LED_STOP = OFF;
		LED_PRODUZIONE = ON;
		LED_CONSERVAZIONE = OFF;
		LED_PULIZIA = OFF;
		CntSelect = 20;
		StringVal( "Sha" );
		StatoSelect = S4;
		OutState = 6;                           // rimane in SELECT
		break;
		//---------------------------------------------------------------------
	case S4 :
		if ( (TastoSelezioneBreve == TRUE)) {
			TastoSelezioneBreve = FALSE;
			StatoSelect = S1_IN;              // per andare in SELECT-PRODUZIONE SLUSH
			OutState = 6;                     // rimane in SELECT
		} else if ( CntSelect == 0 ) {           // dopo 2"...
			StatoSelect = S0_IN;            // per tornare in SELECT-STOP
			StatoPrd0 = S0_IN;              // iniz. stato per lo SWITCH-0 di PRODUZIONE
			Set_HOT = TabProg.HOT;
			Prod_Per_Cons = FALSE;
			Prod_shake=TRUE;
			OutState = 4;                   // va in PRODUZIONE SHAKE
		} else if (TastoStopBreve == TRUE){
			TastoStopBreve = FALSE;
			StatoSelect = S0_IN;
			OutState = 0;     // va in STOP
		}else{
			OutState = 6;                 // rimane in SELECT
		}
		break;
	}
	//__________________________________________________________________________________
	// Aggiornamento ogni 100 mSec dei contatori attivi
	if ( Select_100mSec ) {
		Select_100mSec = 0;
		//______________________________________
		// *** azioni eseguite ogni 100 mSec ***
		if ( CntSelect )  CntSelect --;     // contatore dello SWITCH-0
	}

	return OutState;                        // ritorna lo stato selezionato

}
//_____END Select()________________________________________________________________________________
//_________________________________________________________________________________________________









//_________________________________________________________________________________________________
//_____Stato 5 -> InviaSeriale()___________________________________________________________________
UC InviaSeriale ( void )
{
	if ( CodeRS232 == RQ_TX_LOG ) {
		InviaLog();
	}
	if ( CodeRS232 == RQ_TX_TABPROG ) {
		InviaTabProg();
	}
	if ( CodeRS232 == RQ_RX_TABPROG ) {
		RiceviTabProg();
	}

	return 6;

}
//_____END InviaSeriale()__________________________________________________________________________
//_________________________________________________________________________________________________





extern UC TxBufLength;


// *** Invia gli eventi al programma su PC
UC InviaLog ( void )
{
	UC CntEvento;
	UC NumPage;
	UC PageOffset;
	UC FLASHCARD_OK = FALSE;
	UC EEPROM_OK = FALSE;

	UC TxBuf[10];                         // TxBuf LOCALE
	// le modifiche a TxBuf in InviaLog utilizzano la versione locale e NON la TxBuf globale

	TxBufLength = 0;
	if ( !ReadPage0EEPROM() )         // se checksum della Pagina-0 e' no-OK esce
		return 0;

	//-----------------------------------------------
	AppendToBuffer ( "a-", 2, true );
	//-----------------------------------------------
	//-----------------------------------------------
	HexToAscii( TxBuf, ( WORD ) Pagina0.TipoScheda, 2);
	AppendToBuffer ( TxBuf, 2, true );
	//-----------------------------------------------
	BinToAscii( TxBuf, ( WORD ) Pagina0.NumEvento, 2);
	AppendToBuffer ( TxBuf, 2, true );
	//-----------------------------------------------
	TxBuf[0] = 10;
	TxBuf[1] = 13;
	AppendToBuffer ( TxBuf, 2, true );
	//-----------------------------------------------
	SendBuffer(1);

	if ( Pagina0.NumEvento )            // se ci sono eventi...
		for ( CntEvento = 0; CntEvento < Pagina0.NumEvento; CntEvento ++ ) {
			{
				NumPage = ( CntEvento / EVENTI_X_PAGINA_EEPROM ) + 1;      // la Pagina-0 viene saltata
				PageOffset = ( CntEvento % EVENTI_X_PAGINA_EEPROM ) * DIM_STREVENTO;
				SequentialReadEEPROM( (NumPage*64)+PageOffset, (UC*)&Evento, sizeof( struct StrEvento ) );
			}

			//-----------------------------------------------
			BinToAscii( TxBuf, ( WORD ) Evento.Stato, 1);
			AppendToBuffer ( TxBuf, 1, true );
			//-----------------------------------------------
			BinToAscii( TxBuf, ( WORD ) Evento.Allarme1, 2);
			AppendToBuffer ( TxBuf, 2, true );
			//-----------------------------------------------
			BinToAscii( TxBuf, ( WORD ) Evento.Allarme2, 2);
			AppendToBuffer ( TxBuf, 2, true );
			//-----------------------------------------------
			BinToAscii( TxBuf, ( WORD ) Evento.TappaPasto, 1);
			AppendToBuffer ( TxBuf, 1, true );
			//-----------------------------------------------
			if ( Evento.TEV < 0 ) {
				TxBuf[0] = '-';
				BinToAscii( TxBuf+1, ( WORD ) (-Evento.TEV), 2);
				AppendToBuffer ( TxBuf, 1+2, true );
			} else {
				BinToAscii( TxBuf, ( WORD ) Evento.TEV, 2);
				AppendToBuffer ( TxBuf, 2, true );
			}
			//-----------------------------------------------
			if ( Evento.TEC < 0 ) {
				TxBuf[0] = '-';
				BinToAscii( TxBuf+1, ( WORD ) (-Evento.TEC), 2);
				AppendToBuffer ( TxBuf, 2+1, true );
			} else {
				BinToAscii( TxBuf, ( WORD ) Evento.TEC, 2);
				AppendToBuffer ( TxBuf, 2, true );
			}
			//-----------------------------------------------
			if ( Evento.TGV < 0 ) {
				TxBuf[0] = '-';
				BinToAscii( TxBuf+1, ( WORD ) (-Evento.TGV), 2);
				AppendToBuffer ( TxBuf, 2+1, true );
			} else {
				BinToAscii( TxBuf, ( WORD ) Evento.TGV, 2);
				AppendToBuffer ( TxBuf, 2, true );
			}
			//-----------------------------------------------
			if ( Evento.TE1 < 0 ) {
				TxBuf[0] = '-';
				BinToAscii( TxBuf+1, ( WORD ) (-Evento.TE1), 2);
				AppendToBuffer ( TxBuf, 2+1, true );
			} else {
				BinToAscii( TxBuf, ( WORD ) Evento.TE1, 2);
				AppendToBuffer ( TxBuf, 2, true );
			}
			//-----------------------------------------------
			BinToAscii( TxBuf, ( WORD ) Evento.Ora.Hour, 2);
			AppendToBuffer ( TxBuf, 2, true );
			//-----------------------------------------------
			BinToAscii( TxBuf, ( WORD ) Evento.Ora.Minute, 2);
			AppendToBuffer ( TxBuf, 2, true );
			//-----------------------------------------------
			SendBuffer(1);
			//-----------------------------------------------
			BinToAscii( TxBuf, ( WORD ) Evento.Ora.Second, 2);
			AppendToBuffer ( TxBuf, 2, true );
			//-----------------------------------------------
			BinToAscii( TxBuf, ( WORD ) Evento.Ora.DayWeek, 2);
			AppendToBuffer ( TxBuf, 2, true );
			//-----------------------------------------------
			BinToAscii( TxBuf, ( WORD ) Evento.Ora.DayMonth, 2);
			AppendToBuffer ( TxBuf, 2, true );
			//-----------------------------------------------
			BinToAscii( TxBuf, ( WORD ) Evento.Ora.Month, 2);
			AppendToBuffer ( TxBuf, 2, true );
			//-----------------------------------------------
			BinToAscii( TxBuf, ( WORD ) Evento.Ora.Year, 2);
			AppendToBuffer ( TxBuf, 2, true );
			//-----------------------------------------------
			BinToAscii( TxBuf, ( WORD ) Evento.MMV, 1);
			AppendToBuffer ( TxBuf, 1, true );
			//-----------------------------------------------
			BinToAscii( TxBuf, ( WORD ) Evento.MV, 1);
			AppendToBuffer ( TxBuf, 1, true );
			//-----------------------------------------------
			BinToAscii( TxBuf, ( WORD ) Evento.MP, 1);
			AppendToBuffer ( TxBuf, 1, true );
			//-----------------------------------------------
			BinToAscii( TxBuf, ( WORD ) Evento.MA, 1);
			AppendToBuffer ( TxBuf, 1, true );
			//-----------------------------------------------
			BinToAscii( TxBuf, ( WORD ) Evento.MC, 1);
			AppendToBuffer ( TxBuf, 1, true );
			//-----------------------------------------------
			BinToAscii( TxBuf, ( WORD ) Evento.EVRC, 1);
			AppendToBuffer ( TxBuf, 1, true );
			//-----------------------------------------------
			BinToAscii( TxBuf, ( WORD ) Evento.EVRV, 1);
			AppendToBuffer ( TxBuf, 1, true );
			//-----------------------------------------------
			BinToAscii( TxBuf, ( WORD ) Evento.EVFC, 1);
			AppendToBuffer ( TxBuf, 1, true );
			//-----------------------------------------------
			BinToAscii( TxBuf, ( WORD ) Evento.EVFV, 1);
			AppendToBuffer ( TxBuf, 1, true );
			//-----------------------------------------------
			BinToAscii( TxBuf, ( WORD ) Evento.MP_Ore, 4);  // max 9999
			AppendToBuffer ( TxBuf, 4, true );
			//-----------------------------------------------
			BinToAscii( TxBuf, ( WORD ) Evento.MA_Ore, 4);  // max 9999
			AppendToBuffer ( TxBuf, 4, true );
			//-----------------------------------------------
			BinToAscii( TxBuf, ( WORD ) Evento.NumConi, 5);  // max 99999
			AppendToBuffer ( TxBuf, 5, true );
			//-----------------------------------------------
			TxBuf[0] = 10;
			TxBuf[1] = 13;
			AppendToBuffer ( TxBuf, 2, true );
			//-----------------------------------------------
			SendBuffer(1);
			//-----------------------------------------------
		}
	TxBuf[0] = 10;
	TxBuf[1] = 13;
	AppendToBuffer ( TxBuf, 2, false );
	SendBuffer(1);

	return 1;

} // END InviaLog()






// *** Invia la Tabella di Programmazione
UC InviaTabProg ( void )
{
	UC *Pnt = (UC*)&TabProg;

	RxData.Address = 0;
	MakeMsg( TX_TABPROG, Pnt, 32 );
	SendMsg( 32 );
	WaitTxEnd();
	Pnt += 32;

	RxData.Address = 32;
	MakeMsg( TX_TABPROG, Pnt, 32 );
	SendMsg( 32 );
	WaitTxEnd();
	Pnt += 32;

	RxData.Address = 64;
	MakeMsg( TX_TABPROG, Pnt, 32 );
	SendMsg( 32 );
	WaitTxEnd();
	Pnt += 32;

	RxData.Address = 96;
	MakeMsg( TX_TABPROG, Pnt, 22 );
	SendMsg( 22 );
	WaitTxEnd();

} // END InviaTabProg()





// *** Riceve la Tabella di Programmazione
UC RiceviTabProg ( void )
{
} // END RiceviTabProg()
