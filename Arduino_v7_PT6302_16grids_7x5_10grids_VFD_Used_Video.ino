/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>
#include <avr/io.h>
#include <String.h>
#include <avr/interrupt.h>

/*End of auto generated code by Atmel studio */


//Beginning of Auto generated function prototypes by Atmel Studio
void pt6302_init(void );
void send_char(unsigned char a);
void send_data(unsigned char a);
void test_VFD(void );
void clear_VFD(void );
void pt6302_ser_out(unsigned char a);

//End of Auto generated function prototypes by Atmel Studio

#define VFD_in 7
#define VFD_clk 8
#define VFD_stb 9

#define ARRAYELEMENTS(x) (sizeof(x) / sizeof(x[0])
int myarray[] = {1,2,3,4,5,6};

#define BUFFERSIZE 123
int buffer[BUFFERSIZE];

#define BIN(x) \
( ((0x##x##L & 0x00000001L) ? 0x01 : 0) \
| ((0x##x##L & 0x00000010L) ? 0x02 : 0) \
| ((0x##x##L & 0x00000100L) ? 0x04 : 0) \
| ((0x##x##L & 0x00001000L) ? 0x08 : 0) \
| ((0x##x##L & 0x00010000L) ? 0x10 : 0) \
| ((0x##x##L & 0x00100000L) ? 0x20 : 0) \
| ((0x##x##L & 0x01000000L) ? 0x40 : 0) \
| ((0x##x##L & 0x10000000L) ? 0x80 : 0))


//**************************************************************//

void pt6302_init(void)
{
unsigned char i;

  delay(300); //power_up delay

  // set GPO pins to low
  
  pt6302_ser_out(BIN(01000000)); //(BIN(01000000)));//  (0b01000000)
  //pt6302_ser_out(BIN(00000010)); //(BIN(01000000)));//  (0b01000000)
 delay(5);
  
  // Configure VFD display (number of grids)
  
  pt6302_ser_out(BIN(01100000)); //(BIN(01100110))); //14 grids  (0b01100110)
 delay(5);
  

  // set DIMM/PWM to value
  
  pt6302_ser_out(BIN(01010000) | 7);//((BIN(01010000)) | 7); //0 min - 7 max  )(0b01010000)
delay(5);
  
  
  // switch off extra "ADRAM"
  
  pt6302_ser_out(BIN(00110000));//(BIN(00110000)); //(0b00110000)
  for(i=0;i<16;i++)
  { 
  pt6302_ser_out(0x00); 
  }
 delay(5);
  
// test mode: light all
    
  pt6302_ser_out(BIN(01110011));//((BIN(01110011))); //on    (0b01110011)
    delay(5);
  
// normal mode

   pt6302_ser_out(BIN(01110000));//((BIN(01110000))); //test off-normal mode on  (0b01110000)
   delay(5);
}

void send_char(unsigned char a)
{

 // Note: The strobe change need happne when the clock is up!
  
 unsigned char transmit = 7; //define our transmit pin
 unsigned char data = 170; //value to transmit, binary 10101010
 unsigned char mask = 1; //our bitmask
  
  data=a;
  digitalWrite(VFD_clk, HIGH);
  delayMicroseconds(50);
  digitalWrite(VFD_stb, HIGH);
  delayMicroseconds(50);
  for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
    digitalWrite(VFD_clk, LOW);
    delayMicroseconds(50);
    if (data & mask){ // if bitwise AND resolves to true
      digitalWrite(VFD_in, HIGH);
    }
    else{ //if bitwise and resolves to false
      digitalWrite(VFD_in, LOW);
    }
    delayMicroseconds(50);
    digitalWrite(VFD_clk, HIGH);
    delayMicroseconds(50); 
  }
  digitalWrite(VFD_stb, LOW);
  delayMicroseconds(50);
}

void send_data(unsigned char a)
{
  unsigned char transmit = 7; //define our transmit pin
  unsigned char data = 170; //value to transmit, binary 10101010
  unsigned char mask = 1; //our bitmask
  
  data=a;
  //This don't send the strobe signal, to be used in burst data send
   for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
     digitalWrite(VFD_clk, LOW);
     delayMicroseconds(50);
     if (data & mask){ // if bitwise AND resolves to true
        digitalWrite(VFD_in, HIGH);
     }
     else{ //if bitwise and resolves to false
       digitalWrite(VFD_in, LOW);
     }
     delayMicroseconds(50);
    digitalWrite(VFD_clk, HIGH);
    delayMicroseconds(50);
    //digitalWrite(VFD_clk, LOW);
    //delayMicroseconds(50);
   }
   //digitalWrite(VFD_clk, HIGH);
}

void test_VFD(void)
{
  /* 
  Here do a test for all segments of 6 grids
  each grid is controlled by a group of 3 bytes
  by these reason I'm send a burst of 3 bytes of
  data. The cycle for do a increment of 3 bytes on 
  the variable "i" on each test cycle of FOR.
  */
  
  // to test 6 grids is 6*3=18, the 8 grid result in 8*3=24.
for (int i =0; i< 36; i=i+3){
  send_char(0b01000000); // cmd 2
  digitalWrite(VFD_stb, HIGH);
 delay(5);
  send_data((0b11000000) | i); //cmd 3 Each grid use 3 bytes here define the 1º, 2º, 3º until max address

  send_data(0b11111111); // data
  send_data(0b11111111); // data
  //send_data(0b11111111); // data

  digitalWrite(VFD_stb, LOW);
 delay(5);

  send_char(0b00001000); // cmd1 Here I define the 12 grids and 16 Segments
  //send_char(0b00010000); // cmd1 Here I define the 12 grids and 16 Segments
 delay(5);
  send_char((0b10001000) | 7); //cmd 4
  //send_char((0b00010001) | 7); //cmd 4
 delay(5);
}
}

void pt6302_cls(void)
{
unsigned char i;
  // cls DCRAM
  digitalWrite(VFD_stb, HIGH); // Remember: This signal is inverted by a transistor on the board MEO...
  send_data(BIN(00010000));//((BIN(00010000)));//(0b00010000);
  for(i=0;i<16;i++)
  {
  send_data(' '); // Send space to clear the tube. Zone of DCRAM...
  }
 delay(5); 
 digitalWrite(VFD_stb, LOW);
  //
  // cls Data Write
  digitalWrite(VFD_stb, HIGH);
  send_data(BIN(00110000));//((BIN(00110000)));//(0b00110000);
  for(i=0;i<16;i++) 
  { 
  send_data('a');
  }
 delay(5);
 digitalWrite(VFD_stb, LOW);
}


void pt6302_ser_out(unsigned char a)
{

 // Note: The strobe change need happne when the clock is up!
  
 unsigned char transmit = 7; //define our transmit pin
 unsigned char data = 170; //value to transmit, binary 10101010
 unsigned char mask = 1; //our bitmask
  
  data=a;
  digitalWrite(VFD_clk, HIGH);
  delayMicroseconds(50);
  digitalWrite(VFD_stb, HIGH);
 delay(5);
  for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
    digitalWrite(VFD_clk, LOW);
   delay(5);
    if (data & mask){ // if bitwise AND resolves to true
      digitalWrite(VFD_in, HIGH);
    }
    else{ //if bitwise and resolves to false
      digitalWrite(VFD_in, LOW);
    }
   delay(5);
    digitalWrite(VFD_clk, HIGH);
   delay(5); 
  }
  digitalWrite(VFD_clk, HIGH);
  delayMicroseconds(10); 
  digitalWrite(VFD_stb, LOW);
  delay(5);
}


void pt6302_print(unsigned char address, unsigned char *text)
{
unsigned char c;
 digitalWrite(VFD_stb, HIGH); // Remember: This signal is inverted by a transistor on the board MEO...
  send_data(BIN(00010000) | (address & 0x0F)); //((BIN(00010000)) + (address & 0x0F) );//)(0b00010000
  while ( c = *text++ )
    {
    send_data(c & 0x7F); // 0x7F to 7bits ASCII code
    }
    digitalWrite(VFD_stb, LOW);
  delayMicroseconds(250);
}

void send_bits(unsigned char a){
  unsigned char mask = 1; //our bitmask
  unsigned char data;
  data=a;
  digitalWrite(VFD_clk, HIGH);
  delayMicroseconds(50);
  for (mask = 0b00000001; mask>0; mask <<= 1) { //iterate through bit mask
    digitalWrite(VFD_clk, LOW);
   delayMicroseconds(50);
    if (data & mask){ // if bitwise AND resolves to true
      digitalWrite(VFD_in, HIGH);
    }
    else{ //if bitwise and resolves to false
      digitalWrite(VFD_in, LOW);
    }
   delay(5);
    digitalWrite(VFD_clk, HIGH);
   delay(5); 
  }
 // digitalWrite(VFD_clk, HIGH);
 // delayMicroseconds(10); 
}

void check_run(unsigned int b)
{
int n ,g;
for (n=0;n<b;n++)
{
digitalWrite(13, LOW);
  for(g=0;g<5;g++)
  {
  //delayMicroseconds(100);
  delay(4000);
  }
  digitalWrite(13, HIGH);
  for(g=0;g<5;g++)
  {
  //delayMicroseconds(100);
  delay(4000);
  }
}
}

void strrevert2(char *string)
{

    for (int i = 0; string[i]; i++)
        for (int j = i - 1; j >= 0; j--)
        {
            char c = string[j + 1];
            string[j + 1] = string[j];
            string[j] = c;
        }

}

void strrevert1(char *string)
{
  // Inverter the contents of pointer of string
  // and let it reverted until the next call of
  // function! exp: char letter[16] = "jogos.....tarde";
  // To do a declaration of pointer:  char* ptr=letter;
  // don't forget the null char "\0", this is a 1 more char
  // presente on the string.
  // To avoid str rest reverted, do declaration inside hte while
  // or keep it on new pointer str.
    
    int len = 0;
    while (string[len])
        len++;

    int down = 0;
    int up = len - 1;

    while (up > down)
    {
        char c = string[down];
        string[down++] = string[up];
        string[up--] = c;
    }
}


//*************************************************//
void setup() {
  pinMode(VFD_clk, OUTPUT);
  pinMode(VFD_in, OUTPUT);
  pinMode(VFD_stb, OUTPUT);
  pinMode(13, OUTPUT);


  cli();           // disable all interrupts
  digitalWrite(13, LOW);
  delay(1000);
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(1000);
  digitalWrite(13, HIGH);
  delay(100);
  
check_run(1);
pt6302_init();

delayMicroseconds(5000);
check_run(2);

pt6302_cls();
delayMicroseconds(5000);
check_run(3);

//pt6302_print(0x00,"User chars");
check_run(4);
}

void loop() {
int i=0;
int lng=0;
char buffer1= '1 2 3 4 5 6 7 8';
char buffer2= 'a b c d e f g h';
  lng = sizeof(buffer1);
char letter[16] = "jogos.....tarde";
char rLetter[16] = "................";
char* pointer =letter;
char* ptr = buffer1;
char* point = buffer1;

//const char *names[] = {"aa", "bb", "cc"};
const char names[15] = {1,2,3,4,5,6};
const char nome[] = "jose luis";
const char invNome[]="         ";
char* myStrings[][6] = {"This is string 1", "This is string 2", "This is string 3",
                        "This is string 4", "This is string 5", "This is string 6"
                       };
char hexchar[] = "0123456789ABCDEF";
char* ps = hexchar;
//while(*ps != '\0') {
  // Revert your str here
  // ps++;
//}
//ps=s; 
 while(1){

unsigned char c;
unsigned char r=0;
  while ( c = *ptr++ ) {
    rLetter[r]=(c & 0x7F); // 0x7F to 7bits ASCII code
    r++;
    }
    
strrevert1(ps);  // I'm use a function to revert str... have a strrevert2 also!

    pt6302_print(0, ps);

    for (int v=0; v<40; v++){
     delay(20000);
     digitalWrite(13,!digitalRead(13));  //Only to debug and delay... blink of led pin 13
    }
    
    pt6302_cls();

  delay(6000);
  char info1[]= "Test a VFD tube!";
  char* ptrInfo1=info1;
  strrevert1(ptrInfo1);
  pt6302_print(0,ptrInfo1);  // On "C" code is used the ' as delimiter of string...
  for (int v=0; v<20; v++){
     delay(10000);
     digitalWrite(13,!digitalRead(13));
  }

  pt6302_cls();
  
  char info2[]= "Based on PT6302!";
  char* ptrInfo2=info2;
  strrevert1(ptrInfo2);
  pt6302_print(0,ptrInfo2);
  for (int v=0; v<20; v++){
     delay(10000);
     digitalWrite(13,!digitalRead(13));
  }

  pt6302_cls();
  
 //pt6302_ser_out(BIN(01110000));  // All on is: (BIN(01110011))
 }
}

