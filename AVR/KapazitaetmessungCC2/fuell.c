
// hardcoded frequency. 
#define F_CPU 12000000UL		
#ifndef fOSC 
	#define fOSC F_CPU
#endif
#ifndef BAUDRATE
	#define BAUDRATE  9600	
#endif



#include "MYLIBS\itypes.h"
#include "avr/io.h"
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#include "MYLIBS\ports.c"
#include "MYLIBS\i2cd.c"
#include "MYLIBS\eepr.c"


//Globale Konstanten
#define DADR 126			//I2C Displayadresse
//EEprom Adressen
#define HMIN 0				//minimale Hoehe Steigrohr
#define HMAX 2				//maximale Hoehe Steigrohr
#define CMIN 4				//Kapazitaet bei Minimum
#define CMAX 6			    //Kapazitaet bei Maximum

//Globale Variable
U16 volatile t0over=0;
U32 pulse=0;
U8 toggl=0;
U32 Cfix = 0;


//String aus Flash lesen und am Display ausgeben
//Dazu brauchts die "PGMSPACE.h"
void OutStr_DP (U8 Adr,PGM_P ptr)
{
	while (pgm_read_byte(ptr)) {
		Ddisp(Adr,pgm_read_byte(ptr));
		ptr++;
	}
}



//Kapazitaet Messen
void messcap(void)
{
//Frequenzmessung vorbereiten
 TCCR1B=0b00011000;			//Zeittakt anhalten
 TCCR0=0;					//Frequenzzaehler anhalten
 TCNT0=0; t0over=0;			//Frequenzzaehler loeschen
 TCNT1=0;					//Zeitzaehler loeschen
 
 //Start Frequenzmessung
 TCCR0=0b00000110;			//Impulszaehler Starten
 TCCR1B=0b00011100;			//Zeitbasis starten
 
 
 //Warten bis Zeitbasis abgelaufen
 while (!(TIFR&(1<<ICF1)));
 TCCR0=0;					//Frequenzzaehler anhalten
 TIFR|=(1<<ICF1);			//Ueberlaufbit loeschen
 pulse=((U32)t0over<<8)+(U32)TCNT0;  //Zusammenbau Zaehlerwert
 
 //Berechnung Kapazitaet
 #define ZWEIPI 39.44 	//=(2*Pi hoch 2)
 #define L 10E-3 	  	//Referenzinduktivitaet 10mH
 #define C0 20.0	  	//parasitaere Systemkapazitaet
 float C = 0.00;
 float F = 0.0;

 F=(float)pulse*(float)pulse;		//Frequenz quadrieren
 C=1/(ZWEIPI*L*F);					//Kapatitaet berechnen
 C=C*1E12-C0;						//auf pF skalieren und offset abz.
 Cfix=(U32)(C*10);					//auf LongInt wandel + eine Kommast.
}

//Kalibriermodus Tiefststand
void kaliblow(void)
{
 _delay_ms(200);
 DClear(DADR);
 OutStr_DP (DADR,PSTR("SET CMIN:     pF"));
 while(getD(PIN7));
 _delay_ms(200);			//Nach loslassen der Taste 1.Messung
 //Kapaziteat Tiefstand
 messcap();
 messcap();
 messcap();
 Dpos(DADR,9);				//Kapazitaet am Display anzeigen
 DLong(DADR,Cfix,4,1);
 cli();
 writeEE(CMIN,(U8)Cfix);	//Gemessene Kapazitaet is EEprom
 writeEE(CMIN+1,(U8)(Cfix>>8));
 sei();
 while(!getD(PIN7));
 DClear(DADR);
 OutStr_DP (DADR,PSTR("Fuellstand:000mm"));
 Dpos(DADR,64);
 OutStr_DP (DADR,PSTR("      Hz      pF"));
}
//Kalibriermodus Hoechsstand
void kalibhigh(void)
{
 _delay_ms(200);
 DClear(DADR);
 OutStr_DP (DADR,PSTR("SET CMAX:     pF"));
 while(getD(PIN6));
 _delay_ms(200);			//Nach loslassen der Taste 1.Messung
 //Kapaziteat Hoechststand
 messcap();
 messcap();
 messcap();
 Dpos(DADR,9);				//Kapazitaet am Display anzeigen
 DLong(DADR,Cfix,4,1);
 cli();
 writeEE(CMAX,(U8)Cfix);	//Gemessene Kapazitaet is EEprom
 writeEE(CMAX+1,(U8)(Cfix>>8));
 sei();
 while(!getD(PIN6));
 DClear(DADR);
 OutStr_DP (DADR,PSTR("Fuellstand:000mm"));
 Dpos(DADR,64);
 OutStr_DP (DADR,PSTR("      Hz      pF"));
}

//Timer 0 Interrupt Service
ISR(TIMER0_OVF_vect) 
{
 t0over++;
}




//*********************************************************************
int main(void)//*******************************************************
//*********************************************************************
{

 //I2C Display Initialisieren
 i2cInit();
 Dinit(DADR);
 cntrB(DADR,curoff);
 Dled=ledon;

 //Port B.0 als Ausgang
 DDRB|=(1<<PIN0);
 setB(1,PIN0);
 //Port D.4 als Eingang mit Pullup
 //Frequenzzaehleingang Timer 0
 DDRD&=~(1<<PIN4);
 PORTD|=(1<<PIN4);
 //Port D.7 als Eingang mit Pullup
 DDRD&=~(1<<PIN7);
 PORTD|=(1<<PIN7);
 //Port D.6 als Eingang mit Pullup
 DDRD&=~(1<<PIN6);
 PORTD|=(1<<PIN6);
 
 //Timer 0 als externer Impulszaehler
 //mit Interrupt bei Overflow
 TCCR0=0b00000110;
 TIMSK|=(1<<TOIE0);
 
 //Timer 1 als Zeitbasis im CTC Mode
 //Laufzeit 1 Sekunde
 //12Mhz/256/46875=1
 TCCR1A=0;
 TCCR1B=0b00011100;
 ICR1=46838;//mit Frequenzzaehler getrimmt - 46875 wäre theoretisch;
            
 
 
 
 
 //Interrupts freigeben
 sei();
 
 //Fixe Texte am Display ausgeben
 DClear(DADR);
 OutStr_DP (DADR,PSTR("Fuellstand:000mm"));
 Dpos(DADR,64);
 OutStr_DP (DADR,PSTR("      Hz      pF"));
 
 //////////////////////////////////////////////////////////////////////////////////////
 LOOP:;////////////////////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////////////////
 
 //Abfrage Taste an PortD.7 (Kalibriermodus Tiefststand)
 if (getD(PIN7))
 {
  kaliblow();
 } 
 //Abfrage Taste an PortD.6 (Kalibriermodus Hoechststand)
 if (getD(PIN6))
 {
  kalibhigh();
 } 
 
 //Kapazität messen
 messcap();
 
 //Messzyklus mit LED anzeigen
 toggl=!toggl;
 setB(toggl,PIN0);
 
 Dpos(DADR,64);							//Frequenz am Display anzeigen
 DLong(DADR,pulse,6,0);
 Dpos(DADR,73);							//Kapazitaet am Display anzeigen
 DLong(DADR,Cfix,4,1);
 
 //Berechnung Elektrolythoehe
 float K=0;
 float D=0;
 float Y=0;
 U32 Yfix=0;
 
 U16 iHMAX;
 U16 iHMIN;
 U16 iCMAX;
 U16 iCMIN;
 
									//Grenzdaten aus EEprom lesen
 cli();								//Interrupts aus wegen EEPROM
 iHMIN=(U16)(readEE(HMIN+1)<<8)+(U16)readEE(HMIN);
 iHMAX=(U16)(readEE(HMAX+1)<<8)+(U16)readEE(HMAX);
 iCMIN=(U16)(readEE(CMIN+1)<<8)+(U16)readEE(CMIN);
 iCMAX=(U16)(readEE(CMAX+1)<<8)+(U16)readEE(CMAX);
 sei();
 
 if (Cfix<iCMIN) {Cfix=iCMIN;}
 
 K=(float)iHMAX/((float)iCMAX-(float)iCMIN);
 D=-K*(float)iCMIN;
 Y=(float)Cfix*K+D;
 Yfix=(U32)Y;
 
 Dpos(DADR,11);						//am Display anzeigen
 DLong(DADR,Yfix,3,0);
 
 
 ///////////////////////////////////////////////////////////////////////////////////////
 goto LOOP;/////////////////////////////////////////////////////////////////////////////
 ///////////////////////////////////////////////////////////////////////////////////////
 
 
//***********************************************************************
}//**********************************************************************
//***********************************************************************

