/*

 */

//#include <SQSocketRemote.h>

int rxPin = 13;
int txPin = 3;
int pttPin = 2;

//const int samples = 100;

int leadStartTime = 5000;
const int bufferSize = 100;
unsigned long lengths[bufferSize];
unsigned int len = 0;
int lastReadVal = 0;
unsigned long timeDifference = 0;
unsigned long flopStartTime = 0;

//unsigned long symbol1 = 9160;
//unsigned long symbol2 = 344;
//unsigned long symbol3 = 870;
//unsigned long symbol4 = 844;
//unsigned long symbol5 = 264;
//unsigned long symbol6 = 868;
//unsigned long symbol7 = 208;

//SQSocketRemote socketRemote(txPin, pttPin);

void setup()
{
  Serial.begin(115200);	// Debugging only
  Serial.println("setup");

  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  pinMode(pttPin, OUTPUT);
  
  digitalWrite(pttPin, LOW);
  digitalWrite(txPin, LOW);
  
  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  //TCNT1 = 34286;            // preload timer 65536-16MHz/256/2Hz
  TCNT1 = 32767;
  //TCCR1B |= (1 << CS12);    // 256 prescaler 
  TCCR1B |= (1 << CS11);    // 64 prescaler 
  TCCR1B |= (1 << CS10);    // 64 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
}

ISR(TIMER1_OVF_vect)        // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  //TCNT1 = 32767;            // preload timer

//  noInterrupts();
//  TCNT1 = socketRemote.timerDidOverflow();
//  interrupts();
}


void waitForStartBits()
{ 
  unsigned long lastOne = micros();

  while(micros() - lastOne < leadStartTime)
  {
    int val = digitalRead(rxPin);
    if (val == 1)
    {
      // Still getting 1's or noise, reset counter
      lastOne = micros();
    }
  }
}

void readBitLengths()
{
  len = 0;
  lastReadVal = 0;
  timeDifference = 0;
  flopStartTime = micros();
  while (len < bufferSize && timeDifference < 9000)
  {
    int val = digitalRead(rxPin);

    if (val != lastReadVal) // The bit flipped
    {
      lengths[len] = timeDifference;
      len++;
      lastReadVal = val;
      flopStartTime = micros(); // Start timeing the next bit
    }

    timeDifference = micros() - flopStartTime;
  }

  if (len > 10)
  {
    //Serial.print(" Discovered: ");
    Serial.print(len);
    Serial.print(" - ");
    printBitForVal(lengths[0]);
    for (int i = 1; i < len && i < bufferSize; i++)
    {
      Serial.print(",");
      printBitForVal(lengths[i]);
    }
    Serial.println("};");
  }
}

void printBitForVal(unsigned long val)
{
//  Serial.print(val);
//  Serial.print(":");
  
  if (val < 400)
  {
    // digital 0
    Serial.print("0");
  }
  else if (val > 2000)
  {
    // A stop or start bit
    Serial.print("S");
  }
  else if (val > 800)
  {
    // A digital 1
    Serial.print("1");
  }
}

void loop()
{
  Serial.println("Waiting for data");
  waitForStartBits();
  readBitLengths();
  
  

  /*Serial.println("All Off");
  socketRemote.turnAllDevicesOff();
  delay(1000);
  Serial.println("1 On"); 
  socketRemote.turnDevice1On();
  delay(1000); 
  Serial.println("2 On");   
  socketRemote.turnDevice2On();
  delay(1000);
  Serial.println("3 On");
  socketRemote.turnDevice3On();
  delay(1000);
  Serial.println("4 On");   
  socketRemote.turnDevice4On();
  delay(1000);*/
}










