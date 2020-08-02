//arduino nano firmware for observatory powerboard
//4 onboard outputs (2 12V and 2 regulated by LM2596 modules) controlled by 4 keyswitchs
//2 external relay outputs for use with 220V relay board


//Use MD_KeySwitch 1.4.2 from arduino library tool
//https://github.com/MajicDesigns/MD_KeySwitch
#include <MD_KeySwitch.h>


#define pin_inputvoltage  A0
#define INV_COEF          19.6

#define pin_button_1  2
#define pin_button_2  3
#define pin_button_3  4
#define pin_button_4  5

#define pin_out_1     12
#define pin_out_2     11
#define pin_out_3     10
#define pin_out_4     9
#define pin_out_5     8
#define pin_out_6     7


//blinking status led
#define pin_statusled 13
int ledState = LOW;
unsigned long ledblink_previousMillis = 0;
long ledblink_interval = 1000;           // interval at which to blink (milliseconds)


//automatic status sending
unsigned long status_previousMillis = 0;
long status_interval = 250;           // interval at which to blink (milliseconds)


int inputvoltage = 0;

MD_KeySwitch *key[4];
bool state_out[6];


String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
bool guiconnected = false;    // handle automatic status sending for GUI

bool board_status = false;

void setup() {
  pinMode(pin_statusled, OUTPUT);

  pinMode(pin_button_1, INPUT);
  pinMode(pin_button_2, INPUT);
  pinMode(pin_button_3, INPUT);
  pinMode(pin_button_4, INPUT);

  pinMode(pin_out_1, OUTPUT);
  pinMode(pin_out_2, OUTPUT);
  pinMode(pin_out_3, OUTPUT);
  pinMode(pin_out_4, OUTPUT);
  pinMode(pin_out_5, OUTPUT);
  pinMode(pin_out_6, OUTPUT);

  digitalWrite(pin_out_1, HIGH);
  digitalWrite(pin_out_2, HIGH);
  digitalWrite(pin_out_3, HIGH);
  digitalWrite(pin_out_4, HIGH);
  digitalWrite(pin_out_5, HIGH);
  digitalWrite(pin_out_6, HIGH);

  key[0] = new MD_KeySwitch(pin_button_1, LOW);
  key[1] = new MD_KeySwitch(pin_button_2, LOW);
  key[2] = new MD_KeySwitch(pin_button_3, LOW);
  key[3] = new MD_KeySwitch(pin_button_4, LOW);

  state_out[0] = false;
  state_out[1] = false;
  state_out[2] = false;
  state_out[3] = false;
  state_out[4] = false;
  state_out[5] = false;

  for(int i=0; i<4; i++)
  {
    key[i]->begin();
    key[i]->enableDoublePress(false);
    key[i]->enableLongPress(true);
    key[i]->enableRepeat(false);
    key[i]->enableRepeatResult(false);
  }

  Serial.begin(9600);

  // reserve 200 bytes for the inputString:
  inputString.reserve(200);


  Serial.println("Power Board v1.0 start...");
  Serial.print("#");
}

void loop() {
  ledblink();

  inputvoltage = analogRead(pin_inputvoltage) * INV_COEF;
  if(inputvoltage < 8000)
  {
    state_out[0] = false;
    state_out[1] = false;
    state_out[2] = false;
    state_out[3] = false;
    state_out[4] = false;
    state_out[5] = false;
    ledblink_interval = 250;
    board_status = false;
  }
  else if(inputvoltage > 13500)
  {
    state_out[0] = false;
    state_out[1] = false;
    state_out[2] = false;
    state_out[3] = false;
    state_out[4] = false;
    state_out[5] = false;
    ledblink_interval = 100;
    board_status = false;
  }
  else
  {
    ledblink_interval = 1000;
    board_status = true;
  }

  state_out[0] ? digitalWrite(pin_out_1, LOW) : digitalWrite(pin_out_1, HIGH);
  state_out[1] ? digitalWrite(pin_out_2, LOW) : digitalWrite(pin_out_2, HIGH);
  state_out[2] ? digitalWrite(pin_out_3, LOW) : digitalWrite(pin_out_3, HIGH);
  state_out[3] ? digitalWrite(pin_out_4, LOW) : digitalWrite(pin_out_4, HIGH);
  state_out[4] ? digitalWrite(pin_out_5, LOW) : digitalWrite(pin_out_5, HIGH);
  state_out[5] ? digitalWrite(pin_out_6, LOW) : digitalWrite(pin_out_6, HIGH);


  if (guiconnected)
  {
    statussend();
  }


  for(int i=0; i<4; i++)
  {
    switch(key[i]->read())
    {
      case MD_KeySwitch::KS_NULL:                                       break;
      case MD_KeySwitch::KS_PRESS:      Serial.print("K"); Serial.print(i);                               break;
      case MD_KeySwitch::KS_DPRESS:                                     break;
      case MD_KeySwitch::KS_LONGPRESS:  state_out[i] = !state_out[i];   break;
      case MD_KeySwitch::KS_RPTPRESS:                                   break;
      default:                                                          break;
    }
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

    else if(inputString == "ALL ON\r")
    {
      for(int i=0;i<6;i++)
      {
        state_out[i] = true;
      }
      Serial.println("[OK] ALL ON");
      Serial.print("#");
    }

    else if(inputString == "ALL OFF\r")
    {
      for(int i=0;i<6;i++)
      {
        state_out[i] = false;
      }
      Serial.println("[OK] ALL OFF");
      Serial.print("#");
    }

    else if(inputString == "ALL\r")
    {
      Serial.print("[OK] STATE ");
      for(int i=0;i<6;i++)
      {
        Serial.print(i+1);
        Serial.print(":");
        state_out[i] ? Serial.print("ON") : Serial.print("OFF");
        Serial.print(" ");
      }
      Serial.println("");
      Serial.print("#");
    }

    else if(inputString == "1 ON\r")
    {
        state_out[0] = true;
        Serial.println("[OK] 1 ON");
        Serial.print("#");
    }
    else if(inputString == "1 OFF\r")
    {
        state_out[0] = false;
        Serial.println("[OK] 1 OFF");
        Serial.print("#");
    }
    else if(inputString == "1\r")
    {
      Serial.print("[OK] STATE 1:");
      state_out[0] ? Serial.println("ON") : Serial.println("OFF");
      Serial.print("#");
    }

    else if(inputString == "2 ON\r")
    {
        state_out[1] = true;
        Serial.println("[OK] 2 ON");
        Serial.print("#");
    }
    else if(inputString == "2 OFF\r")
    {
        state_out[1] = false;
        Serial.println("[OK] 2 OFF");
        Serial.print("#");
    }
    else if(inputString == "2\r")
    {
      Serial.print("[OK] STATE 2:");
      state_out[1] ? Serial.println("ON") : Serial.println("OFF");
      Serial.print("#");
    }

    else if(inputString == "3 ON\r")
    {
        state_out[2] = true;
        Serial.println("[OK] 3 ON");
        Serial.print("#");
    }
    else if(inputString == "3 OFF\r")
    {
        state_out[2] = false;
        Serial.println("[OK] 3 OFF");
        Serial.print("#");
    }
    else if(inputString == "3\r")
    {
      Serial.print("[OK] STATE 3:");
      state_out[2] ? Serial.println("ON") : Serial.println("OFF");
      Serial.print("#");
    }

    else if(inputString == "4 ON\r")
    {
        state_out[3] = true;
        Serial.println("[OK] 4 ON");
        Serial.print("#");
    }
    else if(inputString == "4 OFF\r")
    {
        state_out[3] = false;
        Serial.println("[OK] 4 OFF");
        Serial.print("#");
    }
    else if(inputString == "4\r")
    {
      Serial.print("[OK] STATE 4:");
      state_out[3] ? Serial.println("ON") : Serial.println("OFF");
      Serial.print("#");
    }

    else if(inputString == "5 ON\r")
    {
        state_out[4] = true;
        Serial.println("[OK] 5 ON");
        Serial.print("#");
    }
    else if(inputString == "5 OFF\r")
    {
        state_out[4] = false;
        Serial.println("[OK] 5 OFF");
        Serial.print("#");
    }
    else if(inputString == "5\r")
    {
      Serial.print("[OK] STATE 5:");
      state_out[4] ? Serial.println("ON") : Serial.println("OFF");
      Serial.print("#");
    }

    else if(inputString == "6 ON\r")
    {
        state_out[5] = true;
        Serial.println("[OK] 6 ON");
        Serial.print("#");
    }
    else if(inputString == "6 OFF\r")
    {
        state_out[5] = false;
        Serial.println("[OK] 6 OFF");
        Serial.print("#");
    }
    else if(inputString == "6\r")
    {
      Serial.print("[OK] STATE 6:");
      state_out[5] ? Serial.println("ON") : Serial.println("OFF");
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


}

void PrintHelp()
{
  Serial.println("Available commands:");
  Serial.println("HELP/help/? \tprint commands");
  Serial.println("V/v/IN \t\tget input voltage status");
  Serial.println("ALL ON \t\tturn on all outputs");
  Serial.println("ALL OFF \tturn off all outputs");
  Serial.println("ALL \t\tquery state of all outputs");
  Serial.println("X ON \t\tturn on  output X [1-6]");
  Serial.println("X OFF \t\tturn off  output X [1-6]");
  Serial.println("X \t\tquery state of output X [1-6]");
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
    state_out[0] ? Serial.print("1") : Serial.print("0");
    Serial.print("-");
    state_out[1] ? Serial.print("1") : Serial.print("0");
    Serial.print("-");
    state_out[2] ? Serial.print("1") : Serial.print("0");
    Serial.print("-");
    state_out[3] ? Serial.print("1") : Serial.print("0");
    Serial.print("-");
    state_out[4] ? Serial.print("1") : Serial.print("0");
    Serial.print("-");
    state_out[5] ? Serial.print("1") : Serial.print("0");
    Serial.print("-");
    Serial.print("$");
  }
}

void printVoltageStatus()
{
  if(inputvoltage < 8000)
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
