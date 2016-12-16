#include <IRremote.h>

#define COLORS_NUMBER 16

#define CLR_RED          0
#define CLR_ORANGE_1     1
#define CLR_ORANGE_2     2
#define CLR_ORANGE_3     3
#define CLR_YELLOW       4
#define CLR_GREEN        5
#define CLR_LIGHTBLUE_1  6
#define CLR_LIGHTBLUE_2  7
#define CLR_LIGHTBLUE_3  8
#define CLR_CYAN         9
#define CLR_BLUE        10
#define CLR_PURPLE_1    11
#define CLR_PURPLE_2    12
#define CLR_PURPLE_3    13
#define CLR_MAGENTA     14
#define CLR_WHITE       15

#define MODE_MANUAL 1
#define MODE_EFFECT 2

#define PROGRAM_NONE        1
#define PROGRAM_WHITEBLINK  2
#define PROGRAM_FLASH       3
#define PROGRAM_STROBE      4
#define PROGRAM_FADE        5
#define PROGRAM_SMOOTH      6


#define Rpin   5
#define Gpin   6
#define Bpin   9

#define RECVpin  11

/**
 *  Variabili globali ricevitore IR
 */

IRrecv irrecv(RECVpin);
decode_results results;

/**
 * Mappa dei colori indicizzati
 */
byte R[]={255,255,255,255,255,    0,   0,   0,   0,   0,    0,  12,  48, 192, 255, 255};
byte G[]={0,   12, 48,192,255,  255, 255, 255, 255, 255,    0,   0,   0,   0,   0, 255};
byte B[]={0,    0,  0,  0,  0,    0,  12,  48, 192, 255,  255, 255, 255, 255, 255, 255};
byte I=255;


byte mode           = MODE_MANUAL;
byte program        = PROGRAM_NONE;
byte lastColorIndex = CLR_WHITE;

unsigned long clr=0;
unsigned long lastTime=0;


bool blinkLedsOn     = false;

int strobe_direction = 1;

byte fadeR = 255;
byte fadeG = 85;
byte fadeB = 170;

int fadeRdirection = 1;
int fadeGdirection = 1;
int fadeBdirection = 1;

void setColor(int clrIndex,int intensity)
{
 if(clrIndex>=0)
  {
    lastColorIndex = clrIndex;
    analogWrite(Rpin,R[clrIndex]*intensity/255);
    analogWrite(Gpin,G[clrIndex]*intensity/255);
    analogWrite(Bpin,B[clrIndex]*intensity/255);
  }
  else
  {
    analogWrite(Rpin,0);
    analogWrite(Gpin,0);
    analogWrite(Bpin,0);
  }
}


void setRGB(unsigned long rgb, int intensity )
{
  analogWrite(Rpin,((rgb>>16) & 255)*intensity/255);
  analogWrite(Gpin,((rgb>>8)  & 255)*intensity/255);
  analogWrite(Bpin,(rgb & 255)*intensity/255);  
}


void setup() {
  pinMode(Rpin, OUTPUT);
  pinMode(Gpin, OUTPUT);
  pinMode(Bpin, OUTPUT);

  analogWrite(Rpin,0);
  analogWrite(Gpin,0);
  analogWrite(Bpin,0);

  irrecv.enableIRIn();   // Attivazione degli IR
}



// the loop function runs over and over again forever
void loop() 
{
  if (irrecv.decode(&results)) 
  {
      switch(results.value)
      {
        case 0x807f916e:
          setColor(CLR_RED,I);
          mode = MODE_MANUAL;
          break;
        case 0x807FB14E:
          setColor(CLR_ORANGE_1,I);
          mode = MODE_MANUAL;
          break;
        case 0x807FA956:
          mode = MODE_MANUAL;
          setColor(CLR_ORANGE_2,I);
          break;
        case 0x807F9966:
          mode = MODE_MANUAL;
          setColor(CLR_ORANGE_3,I);
          break;
        case 0x807F8976:
          mode = MODE_MANUAL;
          setColor(CLR_YELLOW,I);
          break;
        
        case 0x807f11ee:
          setColor(CLR_GREEN,I);
          mode = MODE_MANUAL;
          break;
        case 0x807F31CE:
          setColor(CLR_LIGHTBLUE_1,I);
          mode = MODE_MANUAL;
          break;
        case 0x807F29D6:
          setColor(CLR_LIGHTBLUE_2,I);
          mode = MODE_MANUAL;
          break;
        case 0x807F19E6:
          setColor(CLR_LIGHTBLUE_3,I);
          mode = MODE_MANUAL;
          break;
        case 0x807F09F6:
          setColor(CLR_CYAN,I);
          mode = MODE_MANUAL;
          break;

        case 0x807f51ae:
          setColor(CLR_BLUE,I);
          mode = MODE_MANUAL;
          break;
        case 0x807F718E:
          setColor(CLR_PURPLE_1,I);
          mode = MODE_MANUAL;
          break;
        case 0x807F6996:
          setColor(CLR_PURPLE_2,I);
          mode = MODE_MANUAL;
          break;
        case 0x807F59A6:
          setColor(CLR_PURPLE_3,I);
          mode = MODE_MANUAL;
          break;
        case 0x807F49B6:
          setColor(CLR_MAGENTA,I);
          mode = MODE_MANUAL;
          break;

        case 0x807fd12e:
          setColor(CLR_WHITE,I);
          mode = MODE_MANUAL;
          break;

        case 0x807fe11e: // On
          setColor(lastColorIndex,I);
          mode = MODE_MANUAL;
          break;
        case 0x807f619e: // Off
          setColor(lastColorIndex,0);
          mode = MODE_MANUAL;
          break;
          
        case 0x807fa15e: // Up
          if(I<=235)
            I+=20;
          else
            I = 255;
            
          if(mode==MODE_MANUAL)
            setColor(lastColorIndex,I);
          break;
          
        case 0x807f21de: // Down
          if(I>20)
            I-=20;
          else
            I = 0;
            
          if(mode==MODE_MANUAL)
            setColor(lastColorIndex,I);
            
          break;
          
          
        case 0x807ff10e: // Flash  indexed colors sequence
          mode = MODE_EFFECT;
          program = PROGRAM_FLASH;
          I = 255;
          break;
        case 0x807fe916: // Strobe  indexed color fade in /out
          mode = MODE_EFFECT;
          program = PROGRAM_STROBE;
          I = 254;
          break;
        case 0x807fd926: // Fade  rainbow
          mode = MODE_EFFECT;
          program = PROGRAM_FADE;
          fadeR = 254;
          fadeG = 85;
          fadeB = 170;
          fadeRdirection = 1;
          fadeGdirection = 1;
          fadeBdirection = 1;
          I = 255;
          break;
        case 0x807fc936: // Smooth rainbow
          mode = MODE_EFFECT;
          program = PROGRAM_SMOOTH;
          fadeR = 0;
          fadeG = 0;
          fadeB = 0;
          fadeRdirection = 1;
          fadeGdirection = 0;
          fadeBdirection = 0;
          I = 255;
          break;
      }
      
      irrecv.resume(); // Receive the next value
   }
   
   if(mode == MODE_EFFECT)
   {
     switch(program)
     {
       case PROGRAM_WHITEBLINK:
         if(millis()-lastTime>1000)
         {
           if(blinkLedsOn==true)
             setColor(lastColorIndex,I);
           else
             setColor(lastColorIndex,0);
           
           lastTime=millis();
           blinkLedsOn = !blinkLedsOn;
         }
         break;
       case PROGRAM_FLASH:
         if(millis()-lastTime>1000)
         {
           lastColorIndex++;
           if(lastColorIndex>=COLORS_NUMBER)
             lastColorIndex=0;
           setColor(lastColorIndex,I);
           
           lastTime=millis();
         }
         break;
       case PROGRAM_STROBE:
         if(millis()-lastTime>10)
         {
           I+=strobe_direction;
           if(I>=255)
           {
              strobe_direction = -strobe_direction;
           }
           if(I<=10)
           {
              strobe_direction = -strobe_direction;
           }
             
           setColor(lastColorIndex,I);
           lastTime=millis();
         }
       
         break;
       case PROGRAM_FADE:
         if(millis()-lastTime>10)
         {
           unsigned long newRGB;
           
           fadeR += fadeRdirection;
           fadeG += fadeGdirection;
           fadeB += fadeBdirection;

           if(fadeR>=255 || fadeR <=1)
             fadeRdirection = -fadeRdirection;
           if(fadeG>=255 || fadeG <=1)
             fadeGdirection = -fadeGdirection;
           if(fadeB>=255 || fadeB <=1)
             fadeBdirection = -fadeBdirection;


             
           newRGB = ((unsigned long)fadeR)*0x00010000 + ((unsigned long)fadeG)*0x00000100 + ((unsigned long)fadeB);

           setRGB(newRGB,I);
           lastTime=millis();
         }
       
        break; 
      case PROGRAM_SMOOTH:
         if(millis()-lastTime>50)
         {
           unsigned long newRGB;

           fadeR += fadeRdirection;
           fadeG += fadeGdirection;
           fadeB += fadeBdirection;
           
           if(fadeR>=255 && fadeB>=255)
           {
             fadeRdirection = 0;
             fadeGdirection = 0;
             fadeBdirection = -1;
           }
           
           if(fadeR>=255 && fadeB<=0)
           {
             fadeRdirection = 0;
             fadeGdirection = 1;
             fadeBdirection = 0;
           }
           
           if(fadeG>=255 && fadeR>=255)
           {
              fadeRdirection = -1;
              fadeGdirection = 0;
              fadeBdirection = 0;
           } 
           
           if(fadeG>=255 && fadeR<=0)
           {
              fadeRdirection = 0;
              fadeGdirection = 0;
              fadeBdirection = 1;
           }
           
           if(fadeB>=255 && fadeG>=255)
           {
              fadeRdirection = 0;
              fadeGdirection = -1;
              fadeBdirection = 0;
           }
           
           if(fadeB>=255 && fadeG<=0 && fadeR<=0)
           {
              fadeRdirection = 1;
              fadeGdirection = 0;
              fadeBdirection = 0;
           }

           newRGB = ((unsigned long)fadeR)*0x00010000 + ((unsigned long)fadeG)*0x00000100 + ((unsigned long)fadeB);

           setRGB(newRGB,I);
           lastTime=millis();
         }
       
        break;         
     }
   }
   
   
}
