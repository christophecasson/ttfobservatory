#include <Servo.h>

#define ledPin        3
#define servoPin      9

int brightness = 0;
Servo servo;

int servopos = 0;

char inputString[20];
int iSIndex = 0;
bool stringComplete = false;


enum devices
{
  FLAT_MAN_L = 10,
  FLAT_MAN_XL = 15,
  FLAT_MAN = 19,
  FLIP_FLAT = 99
};

enum motorStatuses
{
  STOPPED = 0,
  RUNNING = 1
};

enum motorDirections
{
  OPENING = 0,
  CLOSING = 1
};

enum lightStatuses
{
  OFF = 0,
  ON = 1
};

enum coverStatuses
{
  UNKNOWN = 0,
  CLOSED = 1,
  OPEN = 2
};


int deviceId = FLIP_FLAT;
int motorStatus = STOPPED;
int motorDirection = OPENING;
int lightStatus = OFF;
int coverStatus = UNKNOWN;

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 0);

  servo.attach(servoPin);
  servopos = 0;
  servo.write(servopos);
  delay(2000);
}

void loop() 
{
  while (Serial.available())
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString[iSIndex] = inChar;
    iSIndex++;
   
    // if the incoming character is a \n, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n' || inChar == '\r' )
    {
      stringComplete = true;
    }

    //handle overflow
    if( iSIndex >= 19 )
    {
      // clear the string:
      memset(inputString, 0, 20);
      iSIndex = 0;
      stringComplete = false;
    }
    
  }
  
  handleSerial();
  handleMotor();
}


void handleSerial()
{
  char* cmd;
  char* data;
  char temp[10];

  if (stringComplete)
  {
    cmd = inputString + 1;
    data = inputString + 2;
    
    switch( *cmd )
    {
    /*
    Ping device
      Request: >P000\n
      Return : *Pii000\n
        id = deviceId
    */
      case 'P':
      sprintf(temp, "*P%d000\n", deviceId);
      Serial.print(temp);
      break;

      /*
    Open shutter
      Request: >O000\n
      Return : *Oii000\n
        id = deviceId
      This command is only supported on the Flip-Flat!
    */
      case 'O':
      sprintf(temp, "*O%d000\n", deviceId);
      Serial.print(temp);
      SetShutter(OPEN);
      break;


      /*
    Close shutter
      Request: >C000\n
      Return : *Cii000\n
        id = deviceId
      This command is only supported on the Flip-Flat!
    */
      case 'C':
      sprintf(temp, "*C%d000\n", deviceId);
      Serial.print(temp);
      SetShutter(CLOSED);
      break;

    /*
    Turn light on
      Request: >L000\n
      Return : *Lii000\n
        id = deviceId
    */
      case 'L':
      sprintf(temp, "*L%d000\n", deviceId);
      Serial.print(temp);
      lightStatus = ON;
      analogWrite(ledPin, brightness);
      break;

    /*
    Turn light off
      Request: >D000\n
      Return : *Dii000\n
        id = deviceId
    */
      case 'D':
      sprintf(temp, "*D%d000\n", deviceId);
      Serial.print(temp);
      lightStatus = OFF;
      analogWrite(ledPin, 0);
      break;

    /*
    Set brightness
      Request: >Bxxx\n
        xxx = brightness value from 000-255
      Return : *Biiyyy\n
        id = deviceId
        yyy = value that brightness was set from 000-255
    */
      case 'B':
      brightness = atoi(data);    
      if( lightStatus == ON ) 
        analogWrite(ledPin, brightness);   
      sprintf( temp, "*B%d%03d\n", deviceId, brightness );
      Serial.print(temp);
        break;

    /*
    Get brightness
      Request: >J000\n
      Return : *Jiiyyy\n
        id = deviceId
        yyy = current brightness value from 000-255
    */
      case 'J':
        sprintf( temp, "*J%d%03d\n", deviceId, brightness);
        Serial.print(temp);
        break;
      
    /*
    Get device status:
      Request: >S000\n
      Return : *SidMLC\n
        id = deviceId
        M  = motor status( 0 stopped, 1 running)
        L  = light status( 0 off, 1 on)
        C  = Cover Status( 0 moving, 1 closed, 2 open)
    */
      case 'S': 
        sprintf( temp, "*S%d%d%d%d\n",deviceId, motorStatus, lightStatus, coverStatus);
        Serial.print(temp);
        break;

    /*
    Get firmware version
      Request: >V000\n
      Return : *Vii001\n
        id = deviceId
    */
      case 'V': // get firmware version
      sprintf(temp, "*V%d001\n", deviceId);
      Serial.print(temp);
      break;
    }    

    // clear the string:
    memset(inputString, 0, 20);
    iSIndex = 0;
    stringComplete = false;
  }
}


void handleMotor()
{  
  if( servo.read() == 180 )
  {
    coverStatus = OPEN;
  }
  else if( servo.read() == 0 )
  {
    coverStatus = CLOSED;
  }
  else
  {
    coverStatus = UNKNOWN;
  }
 
  if( motorStatus == RUNNING)
  {
    servo.attach(servoPin);
    if( motorDirection == OPENING )
    {
      servopos++;
      servo.write(servopos);
      delay(20);
      if( coverStatus == OPEN )
      {
        motorStatus = STOPPED;
        delay(1000);
      }
    }
    if( motorDirection == CLOSING )
    {
      servopos--;
      servo.write(servopos);
      delay(20);
      if( coverStatus == CLOSED )
      {
        motorStatus = STOPPED;
        delay(1000);
      }
    }
  }
  if( motorStatus == STOPPED )
  {
    servo.detach();
  }
}

void SetShutter(int val)
{
  if( val == OPEN )
  {
    coverStatus = UNKNOWN;
    motorDirection = OPENING;
    motorStatus = RUNNING;
  }
  else if( val == CLOSED )
  {
    coverStatus = UNKNOWN;
    motorDirection = CLOSING;
    motorStatus = RUNNING;
  }  
}
