//arduino nano firmware for observatory roof controller


//Use MD_KeySwitch 1.4.2 from arduino library tool
//https://github.com/MajicDesigns/MD_KeySwitch
#include <MD_KeySwitch.h>


#define pin_inputvoltage  A0
#define INV_COEF          19.6

#define pin_button_open  8
#define pin_button_close  9
#define pin_button_abort  10
#define pin_sensor_closed  11
#define pin_sensor_opened 12

#define pin_relay_enable     2
#define pin_relay_direction     3



#define STATE_IDLE    0
#define STATE_ABORT   1
#define STATE_CLOSED  2
#define STATE_OPENED  3
#define STATE_CLOSING 4
#define STATE_OPENING 5

int state = STATE_IDLE;

//blinking status led
#define pin_statusled 13
int ledState = LOW;
unsigned long ledblink_previousMillis = 0;
long ledblink_interval = 1000;           // interval at which to blink (milliseconds)


//automatic status sending
unsigned long status_previousMillis = 0;
long status_interval = 250;           // interval at which to blink (milliseconds)


int inputvoltage = 0;

MD_KeySwitch *key_Open;
MD_KeySwitch *key_Close;
MD_KeySwitch *key_Abort;
bool state_out[6];


String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
bool guiconnected = false;    // handle automatic status sending for GUI 

bool board_status = false;

void setup() {
  pinMode(pin_statusled, OUTPUT);
  
  pinMode(pin_button_open, INPUT);
  pinMode(pin_button_close, INPUT);
  pinMode(pin_button_abort, INPUT);
  pinMode(pin_sensor_closed, INPUT);
  pinMode(pin_sensor_opened, INPUT);

  pinMode(pin_relay_enable, OUTPUT);
  pinMode(pin_relay_direction, OUTPUT);


  digitalWrite(pin_relay_enable, LOW);
  digitalWrite(pin_relay_direction, LOW);

  key_Open = new MD_KeySwitch(pin_button_open, LOW);
  key_Close = new MD_KeySwitch(pin_button_close, LOW);
  key_Abort = new MD_KeySwitch(pin_button_abort, LOW);

  key_Open->begin();
  key_Open->enableDoublePress(false);
  key_Open->enableLongPress(true);
  key_Open->enableRepeat(false);
  key_Open->enableRepeatResult(false);

  key_Close->begin();
  key_Close->enableDoublePress(false);
  key_Close->enableLongPress(true);
  key_Close->enableRepeat(false);
  key_Close->enableRepeatResult(false);

  key_Abort->begin();
  key_Abort->enableDoublePress(true);
  key_Abort->enableLongPress(true);
  key_Abort->enableRepeat(true);
  key_Abort->enableRepeatResult(true);
  

  Serial.begin(9600);
  
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);


  Serial.println("Roof controller Board v1.0 start...");
  Serial.print("#");
}

void loop() {
  ledblink();
  
  //inputvoltage = analogRead(pin_inputvoltage) * INV_COEF;
  inputvoltage = 12000;
  if(inputvoltage < 10000)
  {
    state = STATE_IDLE;
    ledblink_interval = 250;
    board_status = false;
  }
  else if(inputvoltage > 13500)
  {
    state = STATE_IDLE;
    ledblink_interval = 100;
    board_status = false;
  }
  else
  {
    ledblink_interval = 1000;
    board_status = true;
  }

  if (guiconnected)
  {
    statussend();
  }

  switch(key_Open->read())
  {
    case MD_KeySwitch::KS_NULL:                                       break;
    case MD_KeySwitch::KS_PRESS:                                      break;
    case MD_KeySwitch::KS_DPRESS:                                     break;
    case MD_KeySwitch::KS_LONGPRESS:  state = STATE_OPENING;          break;
    case MD_KeySwitch::KS_RPTPRESS:                                   break;
    default:                                                          break;
  }

  switch(key_Close->read())
  {
    case MD_KeySwitch::KS_NULL:                                       break;
    case MD_KeySwitch::KS_PRESS:                                      break;
    case MD_KeySwitch::KS_DPRESS:                                     break;
    case MD_KeySwitch::KS_LONGPRESS:  state = STATE_CLOSING;          break;
    case MD_KeySwitch::KS_RPTPRESS:                                   break;
    default:                                                          break;
  }

  switch(key_Abort->read())
  {
    case MD_KeySwitch::KS_NULL:                                       break;
    case MD_KeySwitch::KS_PRESS:      Abort();                        break;
    case MD_KeySwitch::KS_DPRESS:     Abort();                        break;
    case MD_KeySwitch::KS_LONGPRESS:  Abort();                        break;
    case MD_KeySwitch::KS_RPTPRESS:   Abort();                        break;
    default:                                                          break;
  }




  if (stringComplete)
  {
    if(inputString == "?\r")
    {
      PrintHelp();
    }
    else if(inputString == "help\r")
    {
      PrintHelp();
    }
    else if(inputString == "Help\r")
    {
      PrintHelp();
    }
    else if(inputString == "HELP\r")
    {
      PrintHelp();
    }
    else if(inputString == "h\r")
    {
      PrintHelp();
    }
    else if(inputString == "H\r")
    {
      PrintHelp();
    }

    else if(inputString == "V\r")
    {
      printVoltageStatus();
    }
    else if(inputString == "v\r")
    {
      printVoltageStatus();
    }
    else if(inputString == "in\r")
    {
      printVoltageStatus();
    }
    else if(inputString == "IN\r")
    {
      printVoltageStatus();
    }
    else if(inputString == "ABORT\r")
    {
        SetOutputRelay_OFF();
        if(state == STATE_OPENING || state == STATE_CLOSING)
        {
          state = STATE_ABORT;
          Serial.println("ABORT - roof is stopped in unknown position");
        }
        else
        {
          Serial.println("Roof is not moving: Nothing to abort!");  
        }
        Serial.print("#");
    }
    else if(inputString == "OPEN\r")
    {
        state = STATE_OPENING;
        Serial.println("Opening roof");
        Serial.print("#");
    }
    else if(inputString == "CLOSE\r")
    {
        state = STATE_CLOSING;
        Serial.println("Closing roof");
        Serial.print("#");
    }
    else if(inputString == "STATE\r")
    {
      Serial.print("Roof state: ");
      printState();
      Serial.println("");
      Serial.print("#");
    }

    
    else
    {
      Serial.println("[ERROR] UNKNOWN COMMAND!");
      PrintHelp();
    }
    
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  }

  StateMachine();
}



void StateMachine()
{
  //Serial.print("=====");
  //printState();
  //Serial.println("=====");
  switch(state)
  {
    case STATE_IDLE:
    {
      SetOutputRelay_OFF();
      if(isClosed())
      {
        state = STATE_CLOSED;
      }
      if(isOpened())
      {
        state = STATE_OPENED;
      }
      break;
    }

    case STATE_ABORT:
    {
      SetOutputRelay_OFF();
      break;
    }

    case STATE_CLOSED:
    {
      //delay(2000);
      SetOutputRelay_OFF();
      if(isClosed() == false)
      {
        state = STATE_IDLE;
      }   
      break;
    }

    case STATE_OPENED:
    {
      //delay(2000);
      SetOutputRelay_OFF();
      if(isOpened() == false)
      {
        state = STATE_IDLE;
      }   
      break;
    }

    case STATE_CLOSING:
    {
      SetOutputRelay_CLOSE();
      if(isClosed())
      {
        state = STATE_CLOSED;
      }
      break;
    }

    case STATE_OPENING:
    {
      SetOutputRelay_OPEN();
      if(isOpened())
      {
        state = STATE_OPENED;
      }
      break;
    }
      
  }
}


bool isClosed()
{
  if(digitalRead(pin_sensor_closed) == LOW)
  {
    return true; 
  }
  else
  {
    return false;
  }
}

bool isOpened()
{
  if(digitalRead(pin_sensor_opened) == LOW)
  {
    return true; 
  }
  else
  {
    return false;
  }
}


void Abort()
{
  SetOutputRelay_OFF();
  if(state == STATE_OPENING || state == STATE_CLOSING)
  {
    state = STATE_ABORT;
  }
}

void SetOutputRelay_OFF()
{
  digitalWrite(pin_relay_enable, LOW);
  digitalWrite(pin_relay_direction, LOW);
}

void SetOutputRelay_OPEN()
{
  digitalWrite(pin_relay_direction, HIGH);
  delay(10);
  digitalWrite(pin_relay_enable, HIGH);
}

void SetOutputRelay_CLOSE()
{
  digitalWrite(pin_relay_direction, LOW);
  delay(10);
  digitalWrite(pin_relay_enable, HIGH);
}




void PrintHelp()
{
  Serial.println("Available commands:");
  Serial.println("HELP/help/? \tprint commands");
  Serial.println("V/v/IN \t\tget input voltage status");
  Serial.println("OPEN \t\tOpen roof");
  Serial.println("CLOSE \tClose roof");
  Serial.println("ABORT \tAbort motion - leave roof at current position");
  Serial.println("STATE \treturn current state of roof");
  Serial.println("");
  Serial.print("#");
}


/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent()
{
  while (Serial.available())
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
   
    if(inputString == "@")
    {
      guiconnected = true;
      inputString = "";
    }
    else if(inputString == "#")
    {
      guiconnected = false;
      inputString = "";
    }
    else
    {
      Serial.print(inChar);
      // if the incoming character is a newline, set a flag so the main loop can
      // do something about it:
      if (inChar == '\r')
      {
        stringComplete = true;
        Serial.println("");
      }
    }

  }
}






void ledblink()
{
  unsigned long currentMillis = millis();

  if (currentMillis - ledblink_previousMillis >= ledblink_interval)
  {
    ledblink_previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(pin_statusled, ledState);
  }
}

void statussend()
{
  unsigned long currentMillis = millis();

  if (currentMillis - status_previousMillis >= status_interval)
  {
    status_previousMillis = currentMillis;

    Serial.print("@");
    Serial.print("-");
    board_status ? Serial.print("O") : Serial.print("E"); //Serial.print("E");
    Serial.print("-");
    Serial.print(inputvoltage);
    Serial.print("-");
    printState();
    Serial.print("-");
    Serial.print("$");
  }
}

void printState()
{
  switch(state)
    {
      case STATE_IDLE: Serial.print("IDLE"); break;
      case STATE_ABORT: Serial.print("ABORT"); break;
      case STATE_CLOSED: Serial.print("CLOSED"); break;
      case STATE_OPENED: Serial.print("OPENED"); break;
      case STATE_CLOSING: Serial.print("CLOSING"); break;
      case STATE_OPENING: Serial.print("OPENING"); break;
    }
}


void printVoltageStatus()
{
  if(inputvoltage < 10000)
      {
        Serial.print("[ERROR] IN Voltage too low (");
      }
      else if(inputvoltage > 13500)
      {
        Serial.print("[ERROR] IN Voltage too high (");
      }
      else
      {
        Serial.print("[OK] IN Voltage OK (");
      } 
      Serial.print(inputvoltage);
      Serial.println(" mV)");
      Serial.print("#");
}


