//Digital Eingang an PortB lesen
U8 getB (U8 pn) 
{
 return (PINB & (1<<pn))?0:1;// 1=auf ground
}
//Digitalausgang an PortB schreiben
void setB (U8 val,U8 pn)
{
 if (val){PORTB|=(1<<pn);}else{PORTB&=~(1<<pn);}
}
//Digitaleingang an PortC lesen
U8 getC (U8 pn) 
{
 return (PINC & (1<<pn))?0:1;// 1=auf ground
}
//Digitalausgang an PortC schreiben
void setC (U8 val,U8 pn)
{
 if (val){PORTC|=(1<<pn);}else{PORTC&=~(1<<pn);}
}
//Digitaleingang an PortD lesen
U8 getD (U8 pn) 
{
 return (PIND & (1<<pn))?0:1;// 1=auf ground
}

//Digitalausgang an PortD schreiben
void setD (U8 val,U8 pn)
{
 if (val){PORTD|=(1<<pn);}else{PORTD&=~(1<<pn);}
}


