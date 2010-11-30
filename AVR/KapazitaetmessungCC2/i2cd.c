
//********************************
//I2C Low Level Funktionen
//********************************

void i2cInit(void)
{
 //ca. 10kHz bei 8Mhz Takt
 TWBR=100;
 TWSR|=(0<<TWPS0)|(0<<TWPS1);
 TWCR=(1<<TWEA)|(1<<TWEN);
}

U8 i2cCheck(U8 Adr)
//0=Device antwortet nicht
{
 U16 tout=65535;
 U8 krax;
 TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
 while ((!(TWCR & (1<<TWINT))) && (tout>0)){tout--;}
 if (!tout) return 0; 
 tout=65535;
 TWDR=Adr;
 TWCR=(1<<TWINT)|(1<<TWEN);
 while ((!(TWCR & (1<<TWINT))) && (tout>0)){tout--;}
 krax=TWSR;
 TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
 if (!tout) return 0;
 if ((krax&0xF8)==0x20)return 0;
 return (krax & 0xF8);
}

void i2cSend(U8 Adr,U8 Lae,U8* Data)
{
 //Start Condition
 TWCR=(TWCR & 0X0F)|(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
 while(!(TWCR&(1<<TWINT)));
 TWDR=Adr & 0XFE;
 TWCR=(TWCR & 0X0F)|(1<<TWINT)|(1<<TWEN);
 while(!(TWCR&(1<<TWINT)));
 while (Lae)
 {
  TWDR=*Data++;
  TWCR=(TWCR & 0X0F)|(1<<TWINT)|(1<<TWEN);
  while(!(TWCR&(1<<TWINT)));
  Lae--;
 }
 TWCR=(TWCR & 0X0F)|(1<<TWINT)|(1<<TWSTO)|(1<<TWEA);
 while(!(TWCR&(1<<TWSTO)));
}

void i2cReceive(U8 Adr,U8 Lae,U8* Data)
{
 //Start Condition
 TWCR=(TWCR & 0X0F)|(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
 while(!(TWCR&(1<<TWINT)));
 TWDR=Adr | 0X01;
 TWCR=(TWCR & 0X0F)|(1<<TWINT)|(1<<TWEN);
 while(!(TWCR&(1<<TWINT)));
 
 while (Lae>1)
 {
  TWCR=(TWCR & 0X0F)|(1<<TWINT)|(1<<TWEN)|(1<<TWEA);
  while(!(TWCR&(1<<TWINT)));
  *Data++=TWDR;
  Lae--;
 }
 TWCR=(TWCR & 0X0F)|(1<<TWINT)|(1<<TWEN);
 while(!(TWCR&(1<<TWINT)));
 *Data=TWDR;
 TWCR=(TWCR & 0X0F)|(1<<TWINT)|(1<<TWSTO)|(1<<TWEA);
 while(!(TWCR&(1<<TWSTO)));
}

//*****************************************
//Display Ansteuerung Low Level
//*****************************************

#define nippmsk 241 
#define comset 0
#define datset 2  
#define comrd 4 

U8 Dled = 1;
#define ledoff 1
#define ledon  0

//Nibble ausgeben
void whNipp(U8 Adr,U8 Mode,U8 dat)
{
 U8 krax1;
 krax1=(dat & nippmsk) | Mode | Dled;
 i2cSend(Adr,1,&krax1);
 krax1|=(1<<3);
 i2cSend(Adr,1,&krax1);
 krax1&=~(1<<3);
 i2cSend(Adr,1,&krax1);
}

//Systembyte lesen
U8 rdsyB(U8 Adr)
{
 U8 krax1;
 U8 krax2;
 krax1=240 | comrd | Dled;
 i2cSend(Adr,1,&krax1);
 krax1|=(1<<3);
 i2cSend(Adr,1,&krax1);
 i2cReceive(Adr,1,&krax2);
 krax1&=~(1<<3);
 i2cSend(Adr,1,&krax1);
 krax1|=(1<<3);
 i2cSend(Adr,1,&krax1);
 krax1&=~(1<<3);
 i2cSend(Adr,1,&krax1);
 return krax2;
}

//Steuerbyte Senden
void cntrB(U8 Adr,U8 Dat)
{
 U8 krax1;
 krax1=Dat & 240;
 whNipp(Adr,comset,krax1);
 krax1=(Dat << 4) & 240;
 whNipp(Adr,comset,krax1);
}
//Steuerbytes
#define dshr   	0b00011100			//Display um 1 nach rechts schieben
#define dshl   	0b00011000			//Display um 1 nach links schieben
#define curon	0b00001110			//Cursor Ein
#define curoff	0b00001100			//Cursor Aus
#define curblk	0b00001111			//Cursor Blinkt	




//Datenbyte Senden
void dataB(U8 Adr,U8 Dat)
{
 U8 krax1;
 krax1=Dat & 240;
 whNipp(Adr,datset,krax1);
 krax1=(Dat << 4) & 240;
 whNipp(Adr,datset,krax1);
}

//Display idle Abfragen
void wBusy(U8 Adr)
{
 while(rdsyB(Adr)&128);
}


//***********************************
// Display Funktionen
//***********************************


//Zeichen Ausgeben
void Ddisp(U8 Adr,U8 Zei)
{
 wBusy(Adr);
 dataB(Adr,Zei);
}

//Display loeschen
void DClear(U8 Adr)
{
 wBusy(Adr);
 cntrB(Adr,1);
}

//Cursor Position setzen
//erste 	zeile 0-63 		(0-15 sichtbar)
//zweite 	zeile 64-127 	(64-79 sichtbar)
void Dpos(U8 Adr,U8 Pos)
{
 U8 krax1;
 krax1=Pos | 128;
 cntrB(Adr,krax1);
}


//Warten
void DWait(U16 Wart)
{
 U16 krax1;
 U16 krax2;
 for (krax1=0;krax1<Wart;krax1++)
 {
  krax2=krax1/2;
 }
}

//Display Initialisiern
void Dinit(U8 Adr)
{
 U8 krax1;
 krax1=0;
 i2cSend(Adr,1,&krax1);
 DWait(65000);
 whNipp(Adr,comset,0b00110001);
 DWait(65000);
 whNipp(Adr,comset,0b00110000);
 DWait(65000);
 whNipp(Adr,comset,0b00110001);
 DWait(65000);
 whNipp(Adr,comset,0b00100000);
 wBusy(Adr);
 cntrB(Adr,0b00101000);
 cntrB(Adr,0b00000000);
 wBusy(Adr);
 cntrB(Adr,0b00000001);
 wBusy(Adr);
 cntrB(Adr,0b00001111);
 wBusy(Adr);
}


//*********************************************
// High Level Ausgabefunktionen
//*********************************************

void DRamStr (U8 Adr,U8* str)
{
	while (*str) {
		Ddisp (Adr,*str++);
	}
}

void DBcd2(U8 Adr,U8 Value)
{
 U8 DisplayStr[3];
 DisplayStr[2] = '\0';
 DisplayStr[1]=(Value & 0X0F)+'0';
 DisplayStr[0]=(Value>>4)+'0';
 DRamStr(Adr,&DisplayStr[0]);
}

void DHexByte(U8 Adr,U8 Value)
{
 U8 DisplayStr[3];
 U8 hlp;
 DisplayStr[2] = '\0';
 hlp=(Value & 0X0F);
 if (hlp>9)
 {
  DisplayStr[1]=hlp-10+'A';
 }else{
  DisplayStr[1]=hlp+'0';
 }
 hlp=(Value>>4)&(0X0F);
 if (hlp>9)
 {
  DisplayStr[0]=hlp-10+'A';
 }else{
  DisplayStr[0]=hlp+'0';
 }
 DRamStr(Adr,&DisplayStr[0]);
}

void DByte(U8 Adr,U8 Value)
{
    U8 DisplayStr[3];
    U8 Loop = 3;
    DisplayStr[Loop] = '\0';
    while (Loop)
    {
        --Loop;
        DisplayStr[Loop] = (Value % 10) + '0';
        Value /= 10;
    }
    DRamStr(Adr,&DisplayStr[0]);
}

void DWord(U8 Adr,U16 Value,U8 kpos)
{
    U8 DisplayStr[7];
    U8 Loop = 5;
	if (kpos!=0) Loop=6;
    DisplayStr[Loop] = '\0';
    while (Loop)
    {
        --Loop;
		if ((kpos==0) || (5-Loop)!=kpos) 
		{
        DisplayStr[Loop] = (Value % 10) + '0';
        Value /= 10;
		} else {
		 DisplayStr[Loop]='.';
		}
        
    }
    DRamStr(Adr,&DisplayStr[0]);
}

void DLong(U8 Adr,U32 Value,U8 anz,U8 kpos)
{
    U8 DisplayStr[15];
    U8 Loop;
	Loop=anz;
	if (kpos!=0) Loop++;
    DisplayStr[Loop] = '\0';
    while (Loop)
    {
        --Loop;
		if ((kpos==0) || (anz-Loop)!=kpos) 
		{
        DisplayStr[Loop] = (Value % 10) + '0';
        Value /= 10;
		} else {
		 DisplayStr[Loop]='.';
		}
    }
    DRamStr(Adr,&DisplayStr[0]);
}




void DInt(U8 Adr,I16 Value)
{
    U8 DisplayStr[7];
    U8 Loop = 6;
    DisplayStr[6] = '\0';
	if (Value < 0)
	{
	 Value=Value*(-1);
	 DisplayStr[0]='-';
	}else{
	 DisplayStr[0]=' ';
	} 
    while (Loop>1)
    {
        --Loop;
        DisplayStr[Loop] = (Value % 10) + '0';
        Value /= 10;
    }
    DRamStr(Adr,&DisplayStr[0]);
}

