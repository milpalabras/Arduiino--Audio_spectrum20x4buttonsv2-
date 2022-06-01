//utilizar la libreria fix_fft.h de:https://github.com/imagest108/arduino/tree/master/libraries/FixFFT


#include <LiquidCrystal.h>
#include <fix_fft.h>

#define DEBUG 0
#define L_IN 1 // Audio input A0 Arduino
#define R_IN 0 // Audio input A1 Arduino

//mario
//************************************************************************************************************************
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951

#define melodyPin 6
//Mario main theme melody
int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo
int tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};

//**************************************************************************************************************************



// var y const de spectro
//****************************************************************************************************************************
const int Yres = 24;
const int gain = 6;
float peaks[64];
char im[64], data[64],Analogs;
char Rim[64], Rdata[64];
char data_avgs[64];
int debugLoop;
int i;
int load;
//**************************************************************************************************************************



//var y const de selector pagina
//*****************************************************************************************************************************
//Counter to change positions of pages
int page_counter=1 ;       //para moverse entre paginas
//-------Pins-----//
int up = 16;               //Up button
int down = 7;           //Down button
int brup = 8;               //brightness Up button
int brdown = 17;           //brightnessDown button
int led = 10;            //Led brightness (pwm output)
int contrast = 9;         //pwm output contrast
//---------Storage debounce function-----//
boolean current_up = LOW;          
boolean last_up=LOW;            
boolean last_down = LOW;
boolean current_down = LOW;
boolean current_brup = LOW;          
boolean last_brup=LOW;            
boolean last_brdown = LOW;
boolean current_brdown = LOW;
boolean current_ctup = LOW;          
boolean last_ctup=LOW;            
boolean last_ctdown = LOW;
boolean current_ctdown = LOW;
int led_lum =4;
int ct_gan =5;
//****************************************************************************************************************************

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Custom CHARACTERS Spectro
byte v0[8] = {B00000,B00000,B00000,B00000,B00000,B00000,B00000,B00000};
byte v1[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B11111};
byte v2[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B11111};
byte v3[8] = {B00000, B00000, B00000, B00000, B00000, B11111, B11111, B11111};
byte v4[8] = {B00000, B00000, B00000, B00000, B11111, B11111, B11111, B11111};
byte v5[8] = {B00000, B00000, B00000, B11111, B11111, B11111, B11111, B11111};
byte v6[8] = {B00000, B00000, B11111, B11111, B11111, B11111, B11111, B11111};
byte v7[8] = {B00000, B11111, B11111, B11111, B11111, B11111, B11111, B11111};
byte v8[8] = {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111};




void setup() {
pinMode(led,OUTPUT);
pinMode(brdown, INPUT);
pinMode(up, INPUT);
pinMode(6, OUTPUT);//buzzer//mario theme
analogWrite(led,75);

//setup lcd spectro
 Serial.begin(9600); 
  if (DEBUG) {
   // hardware serial
    Serial.print("Debug ON");
    Serial.println("");
  }

  lcd.begin(20, 4);
  lcd.clear();
  lcd.createChar(0, v0);
  lcd.createChar(1, v1);
  lcd.createChar(2, v2);
  lcd.createChar(3, v3);
  lcd.createChar(4, v4);
  lcd.createChar(5, v5);
  lcd.createChar(6, v6);
  lcd.createChar(7, v7);
  lcd.createChar(8, v8);

  for (i=0;i<80;i++)
  {
    for (load = 0; load < i / 5; load++)
    {
      lcd.setCursor(load, 1);
      lcd.write(5);
    }
    if (load < 1)
    {
      lcd.setCursor(0, 1);
      lcd.write(5);
    }

    lcd.setCursor(load + 1, 1);
    lcd.write((i - i / 5 * 5) + 1);
    for (load = load + 2; load < 20; load++)
    {
      lcd.setCursor(load, 1);
      lcd.write(9);
    }
    lcd.setCursor(0, 0);
    lcd.print("Cargando Spectro");   
    delay(50);
  }
  lcd.clear();
 sing();

}
 //---- De-bouncing function for all buttons----//
boolean debounce(boolean last, int pin)
{
boolean current = digitalRead(pin);
if (last != current)
{
delay(5);
current = digitalRead(pin);
}
return current;
}
//*******************************************************************************************************************************
//Leer señal R-L
//*******************************************************************************************************************************
void loop() {

  for (int i = 0; i < 64; i++) {    // 64 bins = 32 bins 
    data[i]  = ((((analogRead(L_IN)+analogRead(R_IN))) / 4 ) - 64);  // input analog
    im[i]  = 0;   // imaginary component
 

  }

  fix_fft(data, im, 6, 0);   // 
 

 
  for (int i = 0; i < 16; i++) {
    data[i] = sqrt(data[i]  *  data[i] +  im[i] *  im[i]);
  

  
    data_avgs[i] = data[i];
 

    // Remap values 
    data_avgs[i] = constrain(data_avgs[i], 0, 24 );     
    data_avgs[i] = map(data_avgs[i], 0, 24 , 0, (Yres*gain));  
  }
 
 
 // Two16_LCD();

current_up = debounce(last_up, up);         //Debounce  Up button
current_down = debounce(last_down, down);   //Debounce  Down button
current_brup = debounce(last_brup, brup);         //Debounce  Up button
current_brdown = debounce(last_brdown, brdown);   //Debounce  Down button




//Page Up
    if (last_up== LOW && current_up == HIGH){  
      lcd.clear();                     
      if(page_counter <3){             
      page_counter= page_counter +1;   
      
      }
      else{
      page_counter= 3;  
      }
  }
  
    last_up = current_up;

//Page Down
    if (last_down== LOW && current_down == HIGH){ 
      lcd.clear();                     
      if(page_counter >1){             
      page_counter= page_counter -1;   
      
      }
      else{
      page_counter= 1;  
      }
  }
    
    last_down = current_down;

//------- Switch function -// 
  switch (page_counter) {
   
    case 1:{     // home page 1
      for (int x = 1; x < 16; x++) {  //hum effect
    if (data_avgs[x] > peaks[x])  peaks[x] = data_avgs[x];
     if (peaks[x]>=1 && peaks[x]<=8){
      lcd.setCursor(x+1, 3);
      lcd.write(peaks[x]);
      }
      else {
       
        lcd.print(" ");
    } 
    }

  for (int x = 1; x < 16; x++) {  
    if (data_avgs[x] > peaks[x])  peaks[x] = data_avgs[x];
    if (peaks[x]> 8 && peaks[x]<16) {
      lcd.setCursor(x+1,3);
      lcd.write(8);
      lcd.setCursor(x+1, 2);
      lcd.write(peaks[x]);         
    }
     else {
       lcd.print(" ");
    } 
    }
    
    for (int x = 1; x < 16; x++) {  
    if (data_avgs[x] > peaks[x])  peaks[x] = data_avgs[x];
    if (peaks[x]>16 && peaks[x]<24) {
      lcd.setCursor(x+1,3);
      lcd.write(8);
      lcd.setCursor(x+1,2);
      lcd.write(8);
      lcd.setCursor(x+1,1);
      lcd.write(peaks[x]-16);
     } 
      else {
       lcd.print(" ");
    } 
      
          
    }   
    }
    break;

    case 2: { //Design page 2 
      for (int x = 1; x < 16; x++) {  // hum effect
    if (data_avgs[x] > peaks[x])  peaks[x] = data_avgs[x];
     if (peaks[x]>=1 && peaks[x]<=8){
      lcd.setCursor(x, 2);
      lcd.write(8);
      }
      else {
       
        lcd.print(" ");
    } 
    }

  for (int x = 1; x < 16; x++) {  
    if (data_avgs[x] > peaks[x])  peaks[x] = data_avgs[x];
    if (peaks[x]> 8 && peaks[x]<16) {
      lcd.setCursor(x,2);
      lcd.write(8);
      lcd.setCursor(x, 1);
      lcd.write(8);         
    }
     else {
       lcd.print(" ");
    } 
    }
    
    for (int x = 1; x < 16; x++) {  
    if (data_avgs[x] > peaks[x])  peaks[x] = data_avgs[x];
    if (peaks[x]>16 && peaks[x]<24) {
      lcd.setCursor(x,2);
      lcd.write(8);
      lcd.setCursor(x,1);
      lcd.write(8);
      lcd.setCursor(x,0);
      lcd.write(8);
      lcd.setCursor(x,3);
      lcd.write(8);
     } 
      else {
       lcd.print(" ");
    } 
      
          
    }
    }
    break;

    case 3: {   //Design  page 3 
     //Static objects 
     lcd.setCursor(2,0);
     lcd.print("Led Brightness");
     lcd.setCursor(7,1);
     if(led_lum <10){    
     lcd.setCursor(7,1);
     lcd.print("0");
     }
     lcd.print(led_lum);
     
     lcd.setCursor(15,1);
       
    
    
     
         
     //Control led_lum variable
     if (last_brup== LOW && current_brup == HIGH){  //Up bright +
     if(led_lum < 10){                         
     led_lum ++ ;
     }
     else{
     led_lum = 0;                              
     }
     }
     last_brup=current_brup;
     
     
     if(last_brdown== LOW && current_brdown == HIGH){  //Down bright -
     if(led_lum >0){                             
     led_lum --;                                   
     }
     else{
     led_lum = 0;                                 
     }
     }
     last_brdown=current_brdown;
     
     

     //PWM Control (pwm output 0 to 255 divided by 10 ,pin 11)
     if(led_lum==0){
     analogWrite(led,0);
     }
     if(led_lum==1){
     analogWrite(led,25);
     }
     if(led_lum==2){
     analogWrite(led,50);
     }
     if(led_lum==3){
     analogWrite(led,75);
     }
     if(led_lum==4){
     analogWrite(led,100);
     }
     if(led_lum==5){
     analogWrite(led,125);
     }
     if(led_lum==6){
     analogWrite(led,150);
     }
     if(led_lum==7){
     analogWrite(led,175);
     }
     if(led_lum==8){
     analogWrite(led,200);
     }
     if(led_lum==9){
     analogWrite(led,225);
     }
     if(led_lum==10){
     analogWrite(led,255);
     } 
        
 
    }
    break;

  
    
  }//switch end
   decay(1); 
}//loop end
 
 // decay(1);


//*********************************************************************************************************************************
//graficar
//*********************************************************************************************************************************

void Two16_LCD() {
 // lcd.setCursor(0, 1);
//  lcd.print("L"); // Channel ID replaces bin #0 due to hum & noise
 // lcd.setCursor(0,3);
//  lcd.print("R"); // ditto

 
   
  for (int x = 1; x < 16; x++) {  
    if (data_avgs[x] > peaks[x])  peaks[x] = data_avgs[x];
     if (peaks[x]>=1 && peaks[x]<=8){
      lcd.setCursor(x+1, 3);
      lcd.write(peaks[x]);
      }
      else {
       
        lcd.print(" ");
    } 
    }

  for (int x = 1; x < 16; x++) {  
    if (data_avgs[x] > peaks[x])  peaks[x] = data_avgs[x];
    if (peaks[x]> 8 && peaks[x]<16) {
      lcd.setCursor(x+1,3);
      lcd.write(8);
      lcd.setCursor(x+1, 2);
      lcd.write(peaks[x]);         
    }
     else {
       lcd.print(" ");
    } 
    }
    
    for (int x = 1; x < 16; x++) {  
    if (data_avgs[x] > peaks[x])  peaks[x] = data_avgs[x];
    if (peaks[x]>16 && peaks[x]<24) {
      lcd.setCursor(x+1,3);
      lcd.write(8);
      lcd.setCursor(x+1,2);
      lcd.write(8);
      lcd.setCursor(x+1,1);
      lcd.write(peaks[x]-16);
     } 
      else {
       lcd.print(" ");
    } 
      
          
    }    
    
    
   


  debugLoop++;
  if (DEBUG && (debugLoop > 99)) {
    Serial.print( "Free RAM = " );
    Serial.println( freeRam(), DEC);
    Serial.println( millis(), DEC);
    debugLoop = 0;
  }
}

void sing() {
  

    Serial.println(" 'Mario Theme'");
    int size = sizeof(melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      
      int noteDuration = 1000 / tempo[thisNote];

      buzz(melodyPin, melody[thisNote], noteDuration);

      
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      buzz(melodyPin, 0, noteDuration);

    }
  }


void buzz(int targetPin, long frequency, long length) {
  digitalWrite(13, HIGH);
  long delayValue = 1000000 / frequency / 2; 
  long numCycles = frequency * length / 1000; 
  for (long i = 0; i < numCycles; i++) { 
    digitalWrite(targetPin, HIGH); 
    delayMicroseconds(delayValue); 
    digitalWrite(targetPin, LOW); 
    delayMicroseconds(delayValue);
  }
  

}


int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}


void decay(int decayrate) {
  int DecayTest = 1;
  // reduce the values of the last peaks by 1
  if (DecayTest == decayrate) {
    for (int x = 0; x < 32; x++) {
      peaks[x] = peaks[x] - 1;  // subtract 1 from each column peaks
      DecayTest = 0;
    }
  }

  DecayTest++;
}
