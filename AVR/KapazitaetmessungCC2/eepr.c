
#ifdef __AVR_ATmega168__
void writeEE(U16 adr,U8 dat)
{
 while (EECR & (1<<EEPE));
 EEAR=adr;
 EEDR=dat;
 EECR|=(1<<EEMPE);
 EECR|=(1<<EEPE);
}

U8 readEE(U16 adr)
{
 while (EECR & (1<<EEPE));
 EEAR=adr;
 EECR|=(1<<EERE);
 return EEDR;
}
#endif


#ifdef __AVR_ATmega8__
void writeEE(U16 adr,U8 dat)
{
 while (EECR & (1<<EEWE));
 EEAR=adr;
 EEDR=dat;
 EECR|=(1<<EEMWE);
 EECR|=(1<<EEWE);
}

U8 readEE(U16 adr)
{
 while (EECR & (1<<EEWE));
 EEAR=adr;
 EECR|=(1<<EERE);
 return EEDR;
}
#endif



