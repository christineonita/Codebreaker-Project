// Christine Onita ECE303 Lab/Project 2 Winter 2021


int led1 = 11; // Timer 1 - led1 is red
int led2 = 28; // Timer 5 - led2 is yellow
int led3 = 3; // Timer 3 - led3 is green
int led4 = 7; // Timer 4 - led4 is blue

int numberoftries = 5;
int guess = 0;
String guess_string, userInput;

void setup() {
  Serial.begin(9600); // baud rate
  
  pinMode(led1, OUTPUT); //initialize pin 11 as output  
  pinMode(led2, OUTPUT); //initialize pin 28 as output
  pinMode(led3, OUTPUT); //initialize pin 3 as output
  pinMode(led4, OUTPUT); //initialize pin 7 as output
  digitalWrite(led1, LOW); //turn the red LED off 
  digitalWrite(led2, LOW); //turn the yellow LED off
  digitalWrite(led3, LOW); //turn the green LED off
  digitalWrite(led4, LOW); //turn the blue LED off

  randomSeed(analogRead(0));
  guess=random(9999);  // generating an initial combination (the number you are trying to guess between 0000 and 9999)
  guess_string = String(guess);
  
  noInterrupts(); //disable interrupts
  
  TCCR1A = 0; //Initializes all bits in TCCR1A to 0
  TCCR1B = 0; //Initializes all bits in TCCR1B to 0
  TCCR5A = 0; //Initializes all bits in TCCR5A to 0
  TCCR5B = 0; //Initializes all bits in TCCR5B to 0
  TCCR3A = 0; //Initializes all bits in TCCR3A to 0
  TCCR3B = 0; //Initializes all bits in TCCR3B to 0
  TCCR4A = 0; //Initializes all bits in TCCR4A to 0
  TCCR4B = 0; //Initializes all bits in TCCR4B to 0
  TIMSK1 = 0; 
  TIMSK5 = 0; 
  TIMSK3 = 0; 
  TIMSK4 = 0; 
  TCNT1 = 0; // Initializes counter TCNT1 to 0
  TCNT5 = 0; // Initializes counter TCNT5 to 0
  TCNT3 = 0; // Initializes counter TCNT3 to 0
  TCNT4 = 0; // Initializes counter TCNT4 to 0
  OCR1A = 15624;
  OCR5A = 15624;
  OCR3A = 15624;
  OCR4A = 15624;
  TCCR1B |= (1<<WGM12); // Waveform generation mode 4
  TCCR5B |= (1<<WGM52); // Waveform generation mode 4
  TCCR3B |= (1<<WGM32); // Waveform generation mode 4
  TCCR4B |= (1<<WGM42); // Waveform generation mode 4
  TCCR1B |= (1<<CS12) | (0<<CS11) | (1<<CS10); //pin 11 prescaler set to 1024
  TCCR5B |= (1<<CS52) | (0<<CS51) | (1<<CS50); //pin 28 prescaler set to 1024
  TCCR3B |= (1<<CS32) | (0<<CS31) | (1<<CS30); //pin 3 prescaler set to 1024
  TCCR4B |= (1<<CS42) | (0<<CS41) | (1<<CS40); //pin 7 prescaler set to 1024
  TIMSK1 |= (1<<OCIE1A); // enables pin 11
  TIMSK5 |= (1<<OCIE5A); // enables pin 28
  TIMSK3 |= (1<<OCIE3A); // enables pin 3
  TIMSK4 |= (1<<OCIE4A); // enables pin 7
  
  interrupts(); //re-enable interrupts
  
}

void loop() {
    // put your main code here, to run repeatedly:
  while (guess_string.length() < 4) {
    guess_string = "0" + guess_string;
  }
  if (Serial.available() > 0) {  //this checks the number of characters in the serial port
    Serial.println(guess_string);
    userInput = Serial.readString(); 
    if (numberoftries > 0) {  
      if (userInput[0] == guess_string[0]) {  // Compare first digit of user input and number to be guessed
        digitalWrite(led1, LOW);  //turn LED off if first digit of userinput matches first digit of number to be guessed
        TIMSK1 &= (0<<OCIE1A);  
      }
      else {
        OCR1A = OCR1A/3;  // Increasing frequency of blink if first digit of userinput does not match first digit of number to be guessed
      }
      if (String(userInput)[1] == String(guess)[1]) { // Compare second digit of user input and number to be guessed
        digitalWrite(led2, LOW); //turn LED off if second digit of userinput matches second digit of number to be guessed
        TIMSK5 &= (0<<OCIE5A);
      }
      else {
        OCR5A = OCR5A/3; // Increasing frequency of blink if second digit of userinput does not match second digit of number to be guessed
      }
      if (String(userInput)[2] == String(guess)[2]) {  // Compare third digit of user input and number to be guessed
        digitalWrite(led3, LOW); //turn LED off if third digit of userinput matches third digit of number to be guessed
        TIMSK3 &= (0<<OCIE3A);
      }
      else {
        OCR3A = OCR3A/3; // Increasing frequency of blink if third digit of userinput does not match third digit of number to be guessed
      }
      if (String(userInput)[3] == String(guess)[3]) {  // Compare fourth digit of user input and number to be guessed
        digitalWrite(led4, LOW); //turn LED off if fourth digit of userinput matches fourth digit of number to be guessed
        TIMSK4 &= (0<<OCIE4A);
      }
      else {
        OCR4A = OCR4A/3; // Increasing frequency of blink if fourth digit of userinput does not match fourth digit of number to be guessed
      }
      numberoftries -= 1; //reduce number of tries by one after each time user inputs a guess
    }
    else {  
      if (TIMSK1 == (1<<OCIE1A)) 
      {
        digitalWrite(led1, HIGH);
        TIMSK1 &= (0 << OCIE1A); 
      }
      if (TIMSK5 == (1<<OCIE5A)) 
      {
        digitalWrite(led2, HIGH);
        TIMSK5 &= (0 << OCIE5A); 
      }
      if (TIMSK3 == (1<<OCIE3A))
      {
        digitalWrite(led3, HIGH);
        TIMSK3 &= (0 << OCIE3A);
      }
      if  (TIMSK4 == (1<<OCIE4A)) 
      {
        digitalWrite(led4, HIGH);
        TIMSK4 &= (0 << OCIE4A);
      }
    }
  }
}


ISR(TIMER1_COMPA_vect) { 
  digitalWrite(led1, !digitalRead(led1)); //flips output value
}
ISR(TIMER5_COMPA_vect) {  
  digitalWrite(led2, !digitalRead(led2)); //flips output value
}
ISR(TIMER3_COMPA_vect) {  
  digitalWrite(led3, !digitalRead(led3)); //flips output value
}
ISR(TIMER4_COMPA_vect) {  
  digitalWrite(led4, !digitalRead(led4)); //flips output value
}
