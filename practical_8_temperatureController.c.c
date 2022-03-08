
#include<reg51.h> /*Instructing Preprocessor to add header file reg51.h to use the features of 8051 C Programming*/
#define MYDATA P3 /*Defining Port 3 as "MYDATA" Function:To input the digital data from ADC*/
#define ldata P1  /*Defining Port 1 as data pins for lcd as ldata,Function:To output data to LCD*/
#define FL3i 35   /*Defining set of constants for the temperature limits*/
#define FL2ii 35  /*This approach makes changing the temperature limits very easy*/
#define FL2i 30   /*This prevents from going deeper into the code.*/
#define FL1ii 30
#define FL1i 25
#define NLii 25
#define NLi 20
#define HL1i 20
#define HL1ii 10
#define HL2i 10
#define HL2ii 00



sbit rd=P2^5;   /*Configuring P2.5 with identifier rd,Function:To send read command to ADC*/
sbit wr=P2^4;   /*Configuring P2.4 with identifier wr,Function:To send write command to ADC*/
sbit INTR=P2^3;   /*Configuring P2.3 with identifier INTR,Function:To detect start and end of conversion by ADC*/
sbit rs=P2^0;   /*Configuring P2.0 to give a value to RS register of LCD*/
sbit rw=P2^1;   /*Configuring P2.1 to give a value to RW register of LCD*/
sbit en=P2^2;   /*Configuring P2.2 to give a value to EN register of LCD*/
sbit MTR=P2^6;   /*Configuring P2.6 to give a Pulse width modulated signal to Motor Control Circuitry*/
sbit HTR1=P0^0;   /*Configuring P0.0 as a output line for led which is used as prototype model for heater*/
sbit HTR2=P0^1;   /*Configuring P0.0 as a output line for led which is used as prototype model for heater*/
sbit busy=P1^7;   /*Configuring P1.7,8th bit of ldata or P1 with identifier busy,Function:To know whether */

/*Following are set of functions required by the main routine.
  It is to be noted that function protoype are not used.
  Instead functions are directly implemented along with their definitions.
  While doing so proper function ordering should be made otherwise compiler 
  will generate error.For example:Most of the functions in program calls msDelay() function 
  ,and if msDealy() function is kept below the one calling it,compiler will generate error.*/

void msDelay(unsigned int value){ /*Provides delay in Miliseconds equal to the argument provided.Note that the choice of loop parameter 1275 is 
                                   completely determined by the inernal design of Compiler and may vary from Compiler to Compiler.*/
unsigned int x,y;
for(x=0;x<value;x++)
    for(y=0;y<1275;y++);  /*; is kept because second for loop is written as a single line statement*/
}


void lcdReady(){  /*Checks if LCD controller is busy or not and waits till not busy if it is busy*/
busy=1;
rs=0;
rw=1;
while(busy==1)
{
en=0;
en=1;
}
return;
}

void lcdCmd(unsigned char value){ /*Gives command to LCD*/
lcdReady();/*Calls to check for busy flag*/
ldata=value;
rs=0;
rw=0;/*To appreciate why these values are enforced,one needs to have basic understanding*/
en=1;/*of LCD controller internal operations for read,write etc.*/
en=0;
return;
}


void lcdInit(){ /*Initializes LCD.Whenever initialization is necessary this function is called.*/
lcdCmd(0x38);
lcdCmd(0x0c);
lcdCmd(0x01);
lcdCmd(0x80);
return;
}

void lcdData(char value){ /*To give data to LCD controller for display.*/
ldata=value;
rs=1;
rw=0;
en=1;
en=0;
return;
}

void display(char d1,char d2){
    lcdData(d2);
 msDelay(30);
 msDelay(30);
 lcdData(d1);
 msDelay(30);
 msDelay(' ');
 msDelay(30);
 lcdData('C');
}



void convert(char value){/*Converts data from binary to ASCII code.*/
    char y,d1,d2,d3;
 y=value/10;
 d1=value%10;
 d2=y%10;
 d3=y/10;
 d1=d1|0x30;
 d2=d2|0x30;
 d3=d3|0x30;
 display(d1,d2);/*d3 will be needed only if temperature exceeds 100 degree celcius.
                         If needed it can be added in this call and in function definition
                         as well as body of display().Here it is not included so as to 
                         eliminate redundant digit in the display.*/
 }

 void update(char value){/*Updates the data in lcd if data is changed and is within the range.*/
    char y,d1,d2,d3;     /*Other method would also apply to update data*/
 y=value/10;      /*But I thought this to be the easy and elegant approach.*/
 d1=value%10;
 d2=y%10;
 d3=y/10;
 d1=d1|0x30;
 d2=d2|0x30;
 d3=d3|0x30;
 lcdData(d2);
 msDelay(30);
 msDelay(30);
 lcdData(d1);
 msDelay(30);
 lcdCmd(0xc0);
 }



char adcRead(){/*Reads data from ADC and returns a value in binary format.*/
char value;
wr=0;/*Gives LO-HI pulse to ADC to Start the conversion process.*/
wr=1;
while(INTR==1);/*Waits till data has been converted by ADC.*/
rd=0;/*Gives LO-HI pulse to ADC to read the data converted by ADC*/
value=MYDATA;/*Receiving the converted data into the port 3 of uC*/ 
rd=1;/*End of LO-HI transition.*/
return value;
}


void motorcontrol(){
         unsigned char i;
  unsigned char value;
  unsigned char x[6]="TEMPR:";/*This Section consists Set of Strings defined for purpose of display in LCD*/
  unsigned char x1[5]="FANON";/*TEMPR: means Temperature,HTRON means Heater On*/
  unsigned char x2[5]="HTRON";/*HTRON meansHeater on.*/
  unsigned char y[12]="SPEED:LEVEL";
  unsigned char z[11]="HEAT:LEVEL";
  unsigned char u1[5]="NORM.";/*NORM. means Normal.*/
  unsigned char u2[11]="FAN,HTR OFF";/*FAN,HTR OFF means FAN and Heater both off.*/
  
while(1){  /*Infinite loop is made because of the absence of Operating System.
            Because there is no operating system to return to*/
 value=adcRead(); /*read data from adc*/
    if(value<20){ /*Heater on logic ,confirms to turn on the heater*/   
     lcdCmd(0x01);   /*clear display*/
  for(i=0;i<6;i++) /*display string TEMPR: */
     {
     msDelay(50);
     lcdData(x[i]);
     }
  convert(value); /*convert data and display*/
  lcdCmd(0x8b);      
  for(i=0;i<5;i++){ /*display HTRON message*/
  msDelay(30);
  lcdData(x2[i]);
  }
  
     while(value>HL1ii && value<HL1i){ /*level 1 heat for heater*/
       MTR=1;  
    HTR2=1;
    HTR1=0;
    msDelay(1);
    z[10]='1';
    lcdCmd(0xc0);
    for(i=0;i<11;i++){
     msDelay(30);
     lcdData(z[i]);
     }
    bkl5:
    value=adcRead();
    if(value>HL1ii && value<HL1i){
     lcdCmd(0x86);
     update(value);
    goto bkl5;
    }
    else
    break;
    } /*While closed*/
  
    while(value>=HL2ii && value<=HL2i){ /*level 2 heat for heater*/
      MTR=1;
   HTR1=0;
   msDelay(30);
   HTR2=0;
   msDelay(1);
   lcdCmd(0xc0);
   z[10]='2';
   for(i=0;i<11;i++){
     msDelay(30);
     lcdData(z[i]);
     }
   bkl6:
   value=adcRead();
   if(value>=HL2ii && value<=HL2i){
     lcdCmd(0x86);
     update(value);
     goto bkl6;
    }
   else
   break;
   } /*While closed*/
    

   } /*Heater on logic closed*/
   
   
   while(value>=NLi&&value<NLii){ /*Normal state logic*/
     HTR1=1;
  HTR2=1;
  MTR=1;
  lcdCmd(0x01); /*clear display*/
  for(i=0;i<6;i++){ /*display TEMPR:*/
     msDelay(50);
     lcdData(x[i]);
      }
  convert(value); /*convert data and display*/
  for(i=0;i<5;i++){
  msDelay(30);
  lcdData(u1[i]);
  }
     lcdCmd(0xc0);
  for(i=0;i<11;i++){
  msDelay(30);
  lcdData(u2[i]);
  }
  bklbkl:
  value=adcRead();
  if(value>=NLi&&value<NLii){
   lcdCmd(0x86);
   update(value);
   goto bklbkl;
   }
     else
  break;
   }
    
    
 
 if(value>=25) /*fan on logic,first confirms the situation to turn on fan*/
 {
  HTR1=1;
  HTR2=1;
  lcdCmd(0x01); /*clear display*/
  for(i=0;i<6;i++) /*display TEMPR:*/
     {
     msDelay(50);
     lcdData(x[i]);
     }
  convert(value); /*convert data and display*/
  lcdCmd(0x8b);
  for(i=0;i<5;i++)
  {
  msDelay(30);
  lcdData(x1[i]);
  }



  
     while(value>=FL1i && value<FL1ii){ /*level 1 speed for fan*/
   lcdCmd(0xc0);
   y[11]='1';
   for(i=0;i<12;i++){
     msDelay(30);
     lcdData(y[i]);
    }
   bkl1:
   MTR=0;
      msDelay(50);
      MTR=1;
      msDelay(50);
   value=adcRead();
   if(value>=FL1i && value<FL1ii){   
   lcdCmd(0x86);
   update(value);
   goto bkl1;
   }
   else
   break;
    
   } /*level 1 speed logic closed*/
  


     while(value>=FL2i && value<FL2ii){ /*level 2 speed for fan*/
   y[11]='2';
   lcdCmd(0xc0);
   for(i=0;i<12;i++){
     msDelay(30);
     lcdData(y[i]);
    }
   bkl2:
   MTR=0;
      msDelay(75);
      MTR=1;
      msDelay(25);
      value=adcRead();
   if(value>=FL2i && value<FL2ii){
     lcdCmd(0x86);
     update(value);
     goto bkl2;
    }
   else
   break;
   } /*level 2 speed logic closed*/


    
  while(value>=FL3i){ /*level 3 speed for fan*/
   y[11]='3';
   lcdCmd(0xc0);
   for(i=0;i<12;i++){
     msDelay(30);
     lcdData(y[i]);
    }
   bkl3:
   MTR=0;
   value=adcRead();
   if(value>=FL3i){
     lcdCmd(0x86);
     update(value);
     goto bkl3;
    }
   else
   break;
   } /*level 3 speed logic closed*/


    
     } /*if statement closed i.e fan on logic closed*/
   }  
}  

  void main(){ /*Main function starts.Execution begins from here.*/
   P0=0xff;  /*Setting all pins of P0 to 1s to make it as output port in negative logic*/
   MYDATA=0xff; /*Setting all pins of P3 to 1s to make it as input port in positive logic*/
   INTR=1;/*Active low signal therefore initialized as high.*/
   rd=1;/*Active low signal therefore initialized as high.*/
   wr=1;/*Active low signal therefore initialized as high.*/
   MTR=1;/*Active low is required to drive optocoupler,so initialized as high.*/
   HTR1=0;/*To give a blink effect during a startup to show they are functioning and*/
   HTR2=0;/*leds are used as prototype for heater and are connected in active low configuration.*/
   msDelay(50);
   HTR1=1;/*After 50ms of delay Leds are turned off by high output.*/
   HTR2=1;
   lcdInit();/*Initializes LCD with appropriate Display Setting*/
   motorcontrol();
   }