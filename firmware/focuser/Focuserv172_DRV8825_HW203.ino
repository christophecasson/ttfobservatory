// myfocuserPro DRV8825-HW203
// HW203 PCB BOARD REVISION ONLY
// Arduino Nano CH340G ATMEGA328P 1024 EEPROM
// ONLY FOR USE DRV8825 AND EASYDRIVER Driver Board - supports 1.5A at 12VDC and microstepping
// BIPOLAR STEPPER MOTORS ONLY

// This firmware file supports all versions

// IF USING MOONLITE ASCOM DRIVER
// MAKE SURE THAT POWER ON RESET CIRCUIT IS ENABLED
// ELSE MOONLITE WILL REPORT A TIMEOUT ERROR
// TO REPROGRAM THE CONTROLLER DISABLE THE POWER ON RESET CIRCUIT

// (c) Copyright Robert Brown 2014-2018. All Rights Reserved.
// YOU MAY NOT SELL CONTROLLERS OR PCB'S BASED ON THIS PROJECT
// for commercial gain without express written permission granted from the author.
// Schematics, Code, Firmware, Ideas, Applications, Layout are protected by International Copyright Law.

// Permission is NOT granted to any person to redistribute, market, manufacture or sell for commercial gain the myFocuser
// products, ideas, PCB's, circuits, builds, variations and units described or discussed herein or on this site.
// Permission is granted for personal and Academic/Educational use only.

// THIS MEANS THAT YOU CANNOT RE-DESIGN AND MAKE AVAILABLE KITS OR PCB BASED ON THIS PROJECT AND
// OFFER THOSE KITS FOR SALE TO OTHERS. THAT IS A BREACH OF COPYRIGHT.

// PCB to use
// DRV8825HW203-M-MT-F-BT    https://aisler.net/p/DWEURMAC

// ----------------------------------------------------------------------------------------------------------
// HARDWARE MAPPINGS

// BlueTooth HC-05 Module wired as follows - Do not use with Rotary Encoder
// EN/CMD           // do not connect
// RX to middle of 2K and 1K resistor - other end of 2K resistor to D11 and other end of 1K resistor to ground
// TX to D10
// STATE            // do not connect

// Mapping for PushButtons
// A0

// Mapping for LCD DISPLAY
// VCC    5V
// GND    GND
// SDA    A4
// SCL    A5

// ----------------------------------------------------------------------------------------------------------
// DEFINABLE FEATURES
// Caution: Do not enable a feature if you have not added the associated hardware circuits to support that feature
// With this version you can enable or disable specific add-ons and build the controller with the options you want
// using a single firmware file
// By default, this is a MINIMAL Controller firmware

// For a MINIMAL controller, comment out - TEMPERATUREPROBE, PUSHBUTTONS, DISPLAY
// For a MT controller, comment out - PUSHBUTTONS, DISPLAY
// For a FULL controller, uncomment TEMPERATUREPROBE, PUSHBUTTONS, LCDDISPLAY, BUZZER, INOUTLEDS
// To enable Bluetooth on any controller, uncomment BLUETOOTH

// To enable bluetooth, uncomment the next line
//#define BLUETOOTH 1

// To enable temperature probe, uncomment next line
//#define TEMPERATUREPROBE 1

// To enable the Push Buttons for manual focusing, uncomment the next line
//#define PUSHBUTTONS 1

// To enable the LCD DISPLAY uncomment the next line (LCD1602, I2C)
//#define LCDDISPLAY 1

// To enable the buzzer, uncomment the next line
//#define BUZZER 1

// To enable the IN-OUT LEDS, uncomment the next line
//#define INOUTLEDS 1

// do not change
//#define DEBUG

// ----------------------------------------------------------------------------------------------------------
// FIRMWARE CHANGE LOG

// v1.72
// Change firmware to single file
// Changes to processCommand()
// Changes to clockwise() and anticlockwise()
// Changes to serial events code incl bluetooth
// Implement queue for received serial commands
// Add defines for BUZZER and INOUT LEDS
// Wrap include of OneWire into #define as only use for I2C
// Consolidate check code routines
// Consolidate serial response routines
// myXXXX libraries must be placed in myDocuments/Arduino/libraries
// Complier Define Options now used to generate different firmware versions
// Enable defines for BLUETOOTH, TEMPERATUREPROBE, PUSHBUTTONS, LCDDISPLAY, INOUTLEDS, BUZZER
// Reduce memory footprint

// v1.71 18112017
// Fix major bug in nearinghomepos

// v1.70 31102017
// Fix errors in timer overflows

// v1.69 29072017
// Changes for compatibility with IDE v1.6.8
// Changes to Dallas Temperature Code and EEPROM and HalfStepper files (where included)
// Minor fixes

// v1.68_DRV8825_HW203_F 02112016
// Fix errors related to motorspeed setting and motor delays

// v1.67_DRV8825_HW203_F 28092016
// Fix for small values of stepsize between 0-1

// v1.66_DRV8825_HW203_F 11092016
// Fix to work with APT 3.13
// Fix incorrect logic in handling of GT temperature command for moonlite

// v1.65_DRV8825_HW203_F 15082016
// Fix to PBCode (again)

// v1.64_DRV8825_HW203_F 11082016
// Fix to PBCode as v163 fix upset motor speed settings

// v1.63_DRV8825_HW203_F 04082016
// Small number of fixes related to LED's and PB code

// v1.62_DRV8825_HW203_F 26062016
// Fix error when disable/enable display

// v1.61_DRV8825_HW203_F 07062016
// Fix error when requesting MaxIncrement

// v1.60_DRV8825_HW203_F 14052016
// Tweaks to improve response
// Remove updatelcd when moving
// Add lcdupdateonmove, set and get
// Add LCD update when moving
// Remove tempwaitdelay - wait delay when requesting temperature
// Focuser remembers temperature mode (C/F) setting
// Focuser now remembers temperature resolution setting correctly (was resetting to 0.5)
// Add get and set temperature resolution (9-12)
// Add get/set Motorspeed change and threshold setting
// Removed numwrites
// Removed :XY to free up space
// Support reset of focuser defaults from Windows App
// Save current focuser settings to EEPROM support for Windows App/
// Get motorspeed
// Set motorspeedchange value (threshold) when moving - switches to slowspeed when nearing destination
// Return motorspeedchange value for which stepper slows down at end of its move
// Enable/Disable motorspeedchange when moving
// Get status of motorspeedchange enabled/disabled
// Add Set/Get Step size
// Add Stepsize enabled/disabled
// Fix for setting LCD page display time
//
// For use with Windows App v1750 or greater
// For use with ASCOM driver v1600 or greater

// v1.65_xx FULL VERSIONS ONLY 15082016
// Fix to PBCode (again)
// Change to GF Get Firmware code

// v1.64_xx FULL VERSIONS ONLY 11082016
// Fix to PBCode as v163 fix upset motor speed settings

// v1.63_xxxx All VERSIONS 04082016
// Small number of fixes related to LED's and PB code

// v1.62_xxxx ALL VERSIONS 26062016
// Fix error when disable/enable display

// v1.61_xxxx ALL VERSIONS 07062016
// Fix error when requesting MaxIncrement

// v1.60_xxxx ALL VERSIONS 18052016
// Tweaks to improve response
// Remove updatelcd when moving
// Add lcdupdateonmove, set and get
// Add LCD update when moving
// Remove tempwaitdelay - wait delay when requesting temperature
// Focuser remembers temperature mode (C/F) setting
// Focuser now remembers temperature resolution setting correctly (was resetting to 0.5)
// Add get and set temperature resolution (9-12)
// Add get/set Motorspeed change and threshold setting
// Removed numwrites
// Removed :XY to free up space
// Support reset of focuser defaults from Windows App
// Save current focuser settings to EEPROM support for Windows App/
// Get motorspeed
// Set motorspeedchange value (threshold) when moving - switches to slowspeed when nearing destination
// Return motorspeedchange value for which stepper slows down at end of its move
// Enable/Disable motorspeedchange when moving
// Get status of motorspeedchange enabled/disabled
// Add Set/Get Step size
// Add Stepsize enabled/disabled
// Fix for setting LCD page display time
// For use with Windows App v1750 or greater
// For use with ASCOM driver v1600 or greater
//
// v1.58_DRV8825_HW203_F 29042016
// Further refinement of temperature code

// v1.57_DRV8825_HW203_F 17042016
// Major changes to temperature code
// Added commands to reset Arduino

// v1.56_DRV8825_HW203_F 12042016
// Changes to support higher serial port baud rates

// v1.55_DRV8825_HW203_F 01042016
// Changes for new LCD library and Arduino IDE 1.6.8

// v1.54_DRV8825_HW203_F 24022016
// Fixed bug in temperature display on LCD (was not showing Fahrenheit values)

// v1.53_DRV8825_HW203_F 26012016
// Added extra LCD page to display temperature and maxsteps
// Delay between LCD pages is user configurable and remembered between sessions
// This new feature only works with the latest application software and ASCOM drivers
// fixed bug in setting temperrorwaitdelay "SE" - was being set to 20,000 instead of 2,000
// Added GX and SX to handle updatedisplayintervalNotMoving

// v1.52_DRV8825_HW203_F 14122015
// updating of LCD etc now via timers to ensure regular update and response to commands (Full versions)
// minor bug fixes and removal of unneccessary code (All versions)
// motorspeed values verified/changed in some versions

// v1.51_DRV8825_HW203_F 15112015
// major bug fixes

// v1.50_DRV8825_HW203_F 13112015
// Restored dummy send on startup to communicate with moonlite focuser

// v1.49_DRV8825_HW203_F 09112015
// changes to DS18B20 precision and gettemp routines, fixes to IN/OUT/Buzzer LEDS
// added commands for display in celsius or fahrenheit (:SC# and :SF#)
// changed lcd display to handle C/F as well as increased step mode 64/128

// v1.48_DRV8825_HW203_F 24102015
// MaxSteps limit now 65000
// get MaxIncrement returns MaxIncrement
//
// v1.47_DRV8825_HW203_F 07092015
// Release for HW203 board

// v1.47_DRV8825_F 03092015
// Initiate a temperature conversion on connect()

// v1.46_DRV8825_F 31082015
// Made the wait between tempcalls a saved variable in EEPROM. now it can be reconfigured via Serial interface
// Added serial commands :SExx# and :GE# to send and get the tempwaitdelay time in milliseconds
//
// v1.45_DRV8825_F 30082015
// Fixed issue in :GZ (line of code was left out by mistake)
// Changed time between temp calls to 5s from 10s
//
// v1.44_DRV8825_F 14072015
// Major changes in GZ/GT calls, return last temp if focuser is moving, also if new temp call is
// received less than 10s from the last one - prevents system interruoting moves and causing focuser
// halt as temp conversion takes 750-1000ms
// Issue with FocusMax (now fixed), reported by Tomas Wikander (thanks)

// v1.43_DRV8825_F 20062015
// Push buttons got broken from v1.34, now fixed, reported by Michael Glibstrup (thanks)

// v1.42_DRV8825_F 10062015
// Added protocol commands to disable/enable updating of lcd when motor is moving
// :SU0#     Set updatefposlcd() off, lcd does not update when motor moving, enables faster motor speed
// :SU1#     Set updatefposlcd() on, lcd updates when motor moving

// v1.41_DRV8825_F 28052015
// Minor fixes
//
// v1.40_DRV8825_F 25052015
// On a :FQ (Halt), will write position to EEPROM after delay (in case user turns off focuser later on without any further moves)
// On powerup, both LED IN/OUT now light up with Buzzer On to indicate boot up cycle
//
// v139_DRV8825_F 08052015
// Added serial commands :DS0# :DS1# and :DG# to enable and disable display
//
// v1.38_DRV8825_F 27032015
// added lcd display update of current position and target position whilst moving updatefposlcd()
// added constant value LCDUPDATERATE for handling update (flicker) of LCD when not moving
// changed command :XY# to dump additional parameters
// changed motorSpeed values to get better differences between slow, med and fast
// changes to processcmd() SN, SP, PH, FQ, FG
//
// v1.35_DRV8825_F 22032015
// Halfsteps, Reverse Direction,  and CoilPwr now saved in EEPROM to allow for manual operation
// Changes required to startup and also processcmd (to ensure eeprom write)
// Changes to displaylcd() re stepmode
//
// v1.34_DRV8825_F 16032015
// Test changes for INDI moonlite focuser
// Added gotonewposition variable, used with SN followed by FG (Moonlite)
// Problem is that SN is causing focuser to immediately move, its not waiting for FG
// now FG sets this flag to true, and when move is complete it is reset to false
// the moving condition in the main loop now also uses this flag as a condition to move
// Added GC for moonlite
// Changed maxFocuserLimit to 60000 for be compatible with moonlite
//
// v1.33_DRV8825_F 20022015
// changed Maxsteps and FocuserPosition to type long - On ,net and c# integers are 32 bit so support
// values greater than 32767. Changes maxFocuserLimit to 64000
//
// v1.32_DRV8825_F 05022015
// add stepdelay variable for moonlite protocol support
//
// v1.31_DRV8825_F 03022015
// changes to processcmd(), refined :POXX# to do temperature offset calibration for Moonlite protocol
// moved :GB to top of command processing to respond quicker
// added float tempoffsetval
//
// v1.30_DRV8825_F 30012014
// Changes for Coil Power, using /ENABLE to turn off power after a move
// Requires /ENABLE of DRV8825 or EASYDRIVER to be wired to D8
// Changed temperature precision of probe
//
// v1.29_DRV8825_F 03012014
// code change to ensure currentaddr does not end up with invalid value if not initialized
//
// v1.28_DRV8825_F 30122014
// Minor changes to motorSpeed and stepontime values so that 1/4 and 1/8 stepping works better
// added protocol to set motorSpeed via serial command
// fixed bug in ReverseDirection
//
// v1.27_DRV8825_F 18122014
// Minor bug fix to analogWrite() statements
//
// v1.26_DRV8825_F 13122014
// In GF, Get Firmware version, return program name and version number (ASCOM uses GV)
//
// v1.25_DRV8825_F 10122014
// Modified driver code to write values to EEPROM on SP and SM commands
// At present code only updates 30s after a move. Using the Set() commands for
// position and maxStep does not initiate a move so these would not be saved
// unless there was a move() command that followed before shutdown/power-off
// Doubtful whether user would ever actually alter these parameters like this
// scenario describes, however included just in case
//
// v1.24_DRV8825_F 01122014
// Copied from DRV8825 Minimal
// supports push buttons, LCD and temperature probe
// To work with ONE common Windows APP v1.25 and above
//
// based on some portions of original code by Dave Wells, and orly.andico@gmail.com (moonlite focuser example)
// heavily modified by RBB to work with myFocuser
// ----------------------------------------------------------------------------------------------------------
// FIRMWARE START
#include <Arduino.h>
#include <myQueue.h>                //  By Steven de Salas
#ifdef TEMPERATUREPROBE
#include <OneWire.h>                // needed for DS18B20 temperature probe, see https://github.com/PaulStoffregen/OneWire
#include <myDallasTemperature.h>    // needed for DS18B20 temperature probe, see https://github.com/milesburton/Arduino-Temperature-Control-Library
#endif
#ifdef LCDDISPLAY
#include <Wire.h>                   // needed for I2C, installed when installing the Arduino IDE
#include <LCD.h>
#include <LiquidCrystal_I2C.h>      // needed for LCD, see https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
#endif
#include <myEEPROM.h>               // needed for EEPROM
#include <myeepromanything.h>       // needed for EEPROM
#ifdef BLUETOOTH
#include <SoftwareSerial.h>         // needed for bt adapter - this library is already included when you install the Arduino IDE
#endif

// these are stored in EEPROM - all variables in a structure
struct config_t {
  int validdata;                 // if this is 99 then data is valid
  long fposition;                // last focuser position
  long maxstep;                  // max steps
  int stepmode;                  // stepping mode
  // indicates stepmode, full, half, 1/4, 1/8. 1/16. 1/32 [1.2.4.8.16.32]
  int ds18b20resolution;         // resolution of DS18B20 temperature probe
  long updatedisplayintervalNotMoving;  // refresh rate of display - time each page is displayed for
  double stepsize;               // the step size in microns, ie 7.2
  bool ReverseDirection;      // reverse direction
  bool coilPwr;               // coil pwr
  bool tempmode;              // temperature display mode, Celcius=1, Fahrenheit=0
  bool stepsizeenabled;       // if true, controller returns step size
  bool lcdupdateonmove;       // update position on lcd when moving
} myfocuser;

#define PBswitchesPin     A0
#define bledIN            A1
#define gledOUT           A2
#define Buzzer            A3
#define ch1temp           2
#define myDir             12
#define myStep            11
#define myEnable          5
#define myM2              8         // microstepping lines
#define myM1              7         // M0/M1/M2 sets stepping mode 000=F, 100=1/2, 010=1/4, 110=1/8, 001=1/16, 101=1/16, 110=1/16 etc
#define myM0              6         // steps per revolution = 200, 400, 800, 1600, 6400
#define EEPROMSIZE        1024      // ATMEGA328P 1024 EEPROM
#define TIMEINTERVAL      10000L
#define OUTPUTENABLED     1
#define OUTPUTDISABLED    0
#define MAXSTEPLIMIT      65000
#define STARTMAXPOS       10000
#define STARTPOS          5000
#define MAXINC            1000
#define PULSETIME         5             // expressed in microseconds
#define MOTORSPEEDSLOW0   80000
#define MOTORSPEEDSLOW2   40000
#define MOTORSPEEDSLOW4   4000
#define MOTORSPEEDSLOW8   2000
#define MOTORSPEEDSLOW16  1000
#define MOTORSPEEDSLOW32  1000
#define MOTORSPEEDSLOW64  1000
#define MOTORSPEEDMED0    22000
#define MOTORSPEEDMED2    3250
#define MOTORSPEEDMED4    1500
#define MOTORSPEEDMED8    800
#define MOTORSPEEDMED16   400
#define MOTORSPEEDMED32   400
#define MOTORSPEEDMED64   400
#define MOTORSPEEDFAST0   3000
#define MOTORSPEEDFAST2   1500
#define MOTORSPEEDFAST4   600
#define MOTORSPEEDFAST8   400
#define MOTORSPEEDFAST16  200
#define MOTORSPEEDFAST32  200
#define MOTORSPEEDFAST64  200
#define SLOW              0
#define MED               1
#define FAST              2
#define MAXCOMMAND        15
#define TEMP_PRECISION    10
#define TMPREFRESHRATE    5000
#define MTRTHRESHHOLD     200

const String programName = "myFP-DRV8825HW203";
const String programVersion = "172";
int currentaddr;            // will be address in eeprom of the data stored
bool writenow;              // should we update values in eeprom
long previousMillis;        // used as a delay whenever the EEPROM settings need to be updated
long myinterval;            // interval in milliseconds to wait after a move before writing settings to EEPROM, 10s
int stepdelay;              // used by moonlite
int tempcomp;               // used by moonlite
int tempoffsetval;          // used by moonlite
int boardstate;
int stepontime;             // stepontime - time in microseconds that coil power is ON for one step, board requires 2us pulse
int motorSpeedDelay;        // the delay between steps
int motorSpeed;             // the motorspeed setting 0=slow, 1=medium, 2=fast, default=slow on startup only
int savedmotorSpeed;        // used to save original speed if slowing down when nearing target position
long currentPosition;       // current position
long targetPosition;        // target position
long maxFocuserLimit;       // arbitary focuser limit
long maxSteps;              // maximum position of focuser
long maxIncrement;          // maximum number of steps permitted in one move
long minimumPosition;       // minimum position to avoid focuser damage
bool gotonewposition;       // used by moonlite after an SN command followed by a FG
bool isMoving;              // is the motor currently moving
long pos;
int TSWTHRESHOLD;           // position at which stepper slows down at it approaches home position
int motorspeedchange;
#ifdef PUSHBUTTONS
int PBVal;                  // Push button value
#endif

#define SerialPortSpeed 9600
Queue<String> queue(10);    // receive serial queue of commands
char line[MAXCOMMAND];
int eoc;                    // end of command
int idx;                    // index into command string
#ifdef BLUETOOTH
char btline[MAXCOMMAND];
int bteoc;
int btidx;
#define BTPortSpeed 9600
#define btRX    11          // define Bluetooth Adapter
#define btTX    10
SoftwareSerial btSerial( btTX, btRX);
#endif

bool displayenabled = true;         // used to enable and disable the display
#ifdef LCDDISPLAY
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
long olddisplaytimestampNotMoving;  // timestamp of last display update
int LCD1602Screen;                  // used to decide which LCD screen to display
int updatecount;                    // loop variable used in updating lcd when moving
int lcdupdatestepcount;             // the number of steps moved which triggers an lcd update when moving
#endif

int tprobe1;                        // indicate if there is a probe attached to that channel
double ch1tempval;                  // temperature value for probe
double lasttempval;                 // holds previous temperature value - used if ismoving and if temp request < 10s apart
#ifdef TEMPERATUREPROBE
OneWire oneWirech1(ch1temp);        // setup temperature probe 1
DallasTemperature sensor1(&oneWirech1);
DeviceAddress tpAddress;            // used to send precision setting to specific sensor
double starttempval;
long lasttempconversion;            // holds time of last conversion
long temprefreshrate;               // refresh rate between temperature conversions unless an update is requested via serial command
#endif

void updatemotorSpeedDelay()
{
  switch ( motorSpeed )
  {
    case 0: // slow
      switch (myfocuser.stepmode)
      {
        case 1: // full steps
          motorSpeedDelay = (unsigned int) MOTORSPEEDSLOW0; break;
        case 2: // half steps
          motorSpeedDelay = MOTORSPEEDSLOW2; break;
        case 4: // 1/4 steps
          motorSpeedDelay = MOTORSPEEDSLOW4; break;
        case 8: // 1/8 steps
          motorSpeedDelay = MOTORSPEEDSLOW8; break;
        case 16: // 1/16 steps
          motorSpeedDelay = MOTORSPEEDSLOW16; break;
        case 32: // 1/32 steps
          motorSpeedDelay = MOTORSPEEDSLOW32; break;
        case 64: // 1/64 steps
          motorSpeedDelay = MOTORSPEEDSLOW64; break;
        default:
          motorSpeedDelay = MOTORSPEEDSLOW2; break;
      }
      break;
    case 1: // medium
      switch (myfocuser.stepmode)
      {
        case 1: // full steps
          motorSpeedDelay = MOTORSPEEDMED0; break;
        case 2: // half steps
          motorSpeedDelay = MOTORSPEEDMED2; break;
        case 4: // 1/4 steps
          motorSpeedDelay = MOTORSPEEDMED4; break;
        case 8: // 1/8 steps
          motorSpeedDelay = MOTORSPEEDMED8; break;
        case 16: // 1/16 steps
          motorSpeedDelay = MOTORSPEEDMED16; break;
        case 32: // 1/32 steps
          motorSpeedDelay = MOTORSPEEDMED32; break;
        case 64: // 1/64 steps
          motorSpeedDelay = MOTORSPEEDMED64; break;
        default:
          motorSpeedDelay = MOTORSPEEDMED2; break;
      }
      break;
    case 2: // fast
      switch (myfocuser.stepmode)
      {
        case 1: // full steps
          motorSpeedDelay = MOTORSPEEDFAST0; break;
        case 2: // half steps
          motorSpeedDelay = MOTORSPEEDFAST2; break;
        case 4: // 1/4 steps
          motorSpeedDelay = MOTORSPEEDFAST4; break;
        case 8: // 1/8 steps
          motorSpeedDelay = MOTORSPEEDFAST8; break;
        case 16: // 1/16 steps
          motorSpeedDelay = MOTORSPEEDFAST16; break;
        case 32: // 1/32 steps
          motorSpeedDelay = MOTORSPEEDFAST32; break;
        case 64: // 1/64 steps
          motorSpeedDelay = MOTORSPEEDFAST64; break;
        default:
          motorSpeedDelay = MOTORSPEEDFAST2; break;
      }
      break;
  }
#ifdef DEBUG
  Serial.print("motorSpeed = "); Serial.println(motorSpeed);
  Serial.print("stepmode = "); Serial.println(myfocuser.stepmode);
  Serial.print("motorSpeedDelay = "); Serial.println(motorSpeedDelay);
#endif
}

void software_Reboot()
{
  // jump to the start of the program
  asm volatile ( "jmp 0");
}

#ifdef PUSHBUTTONS
// read the push button switches and return state of switches
// 1 = SW1 ON AND SW2 OFF, 2 = SW2 ON AND SW1 OFF, 3 = SW1 ON and SW2 ON, 0 = OFF
int readpbswitches(int pinNum)
{
  //  sw1 (681) 650-720, sw2 (338) 310-380, sw1 and sw2 (509) 460-530
  int val = 0;                              // variable to store the read value

  val = analogRead(pinNum);                 // read the input pin
  if ( (val >= 650) && (val <= 720) )
  {
    return 1;                    // toggle sw1 ON and SW2 OFF
  }
  else if ( (val >= 460) && (val <= 530) )
  {
    return 3;                    // toggle sw1 and sw2 ON
  }
  else if ( (val >= 310) && (val <= 380) )
  {
    return 2;                    // toggle sw2 ON and SW1 OFF
  }
  else return 0;                 // switches are OFF
}
#endif

// disable the stepper motor outputs - coil power off
void disableoutput()
{
  digitalWrite(myEnable, HIGH);
  // control via ENABLE pin, but this turns off indexer inside DRV8825
  // which means the stepper motor will loose power and move position to the nearest full step
  // not an issue if using full steps but major issue if using microstepping as will cause change
  // in focus position
}

// enable the stepper motor outputs - coil power on
void enableoutput()
{
  digitalWrite(myEnable, LOW);
}

// Move stepper anticlockwise
void anticlockwise()
{
#ifdef INOUTLEDS
  (!myfocuser.ReverseDirection) ? digitalWrite( gledOUT, 1) : digitalWrite( bledIN, 1);
#endif
  (!myfocuser.ReverseDirection) ? digitalWrite(myDir, LOW ) : digitalWrite(myDir, HIGH );
  digitalWrite(myStep, 1 );
  delayMicroseconds(stepontime);
  digitalWrite(myStep, 0 );
#ifdef INOUTLEDS
  (!myfocuser.ReverseDirection) ? digitalWrite( gledOUT, 0 ) : digitalWrite( bledIN, 0);
#endif
}

// Move stepper clockwise
void clockwise()
{
#ifdef INOUTLEDS
  (!myfocuser.ReverseDirection) ? digitalWrite( bledIN, 1) : digitalWrite( gledOUT, 1);
#endif
  (!myfocuser.ReverseDirection) ? digitalWrite(myDir, HIGH ) : digitalWrite(myDir, LOW );
  digitalWrite(myStep, 1 );
  delayMicroseconds(stepontime);
  digitalWrite(myStep, 0 );
#ifdef INOUTLEDS
  (!myfocuser.ReverseDirection) ? digitalWrite( bledIN, 0 ) : digitalWrite( gledOUT, 0);
#endif
}

// set the microstepping mode
void setstepmode()
{
  switch ( myfocuser.stepmode )
  {
    case 1:      // full step
      digitalWrite(myM0, 0);
      digitalWrite(myM1, 0);
      digitalWrite(myM2, 0);
      break;
    case 2:      // half step
      digitalWrite(myM0, 1);
      digitalWrite(myM1, 0);
      digitalWrite(myM2, 0);
      break;
    case 4:
      digitalWrite(myM0, 0);
      digitalWrite(myM1, 1);
      digitalWrite(myM2, 0);
      break;
    case 8:
      digitalWrite(myM0, 1);
      digitalWrite(myM1, 1);
      digitalWrite(myM2, 0);
      break;
    case 16:
      digitalWrite(myM0, 0);
      digitalWrite(myM1, 0);
      digitalWrite(myM2, 1);
      break;
    case 32:
      digitalWrite(myM0, 1);
      digitalWrite(myM1, 0);
      digitalWrite(myM2, 1);
      break;
    default:      // half step
      digitalWrite(myM0, 1);
      digitalWrite(myM1, 0);
      digitalWrite(myM2, 0);
      myfocuser.stepmode = 2;
      break;
  }
}

void ResetFocuserDefaults()
{
  myfocuser.validdata = 99;
  myfocuser.fposition = 25000L;
  myfocuser.maxstep = 50000L;
  myfocuser.stepmode = 8;
  myfocuser.ReverseDirection = false;
  myfocuser.coilPwr = false;
  myfocuser.tempmode = true; // celsius
  myfocuser.updatedisplayintervalNotMoving = 2500L;
  myfocuser.ds18b20resolution = TEMP_PRECISION;
  myfocuser.stepsizeenabled = false;
  myfocuser.stepsize = 10;
  myfocuser.lcdupdateonmove = false;
  // now write the data to EEPROM
  EEPROM_writeAnything(currentaddr, myfocuser);    // update values in EEPROM
}

void sendresponsestr(String str)
{
  if (Serial)
  {
    Serial.print(str);
  }
#ifdef BLUETOOTH
  btSerial.print(str);
#endif
}

void sendresponsenl()
{
  if (Serial)
  {
    Serial.println();
  }
#ifdef BLUETOOTH
  btSerial.println();
#endif
}

// rangecheck position for <= 0
void checktargetzero()
{
  if ( targetPosition < 0 )
  {
    targetPosition = 0;
  }
}

// rangecheck position for > maxsteps
void checktargetmax()
{
  if ( targetPosition > myfocuser.maxstep )
  {
    targetPosition = myfocuser.maxstep;
  }
}

// Serial Commands
void processCommand()
{
  int len;
  long pos;
  String replystr = "";
  char mycmd[MAXCOMMAND];
  char param[MAXCOMMAND];

  memset(mycmd, 0, MAXCOMMAND);
  memset(param, 0, MAXCOMMAND);
  replystr = queue.pop();
  len = replystr.length();
  if ( len == 2 )
  {
    String tmp = replystr.substring(0, 3);
    tmp.toCharArray(mycmd, 3);
#ifdef DEBUG
    Serial.print("tmp cmd = "); Serial.println(tmp);
#endif
  }
  else if ( len > 2 )
  {
    String tmp = replystr.substring(0, 3);
    tmp.toCharArray(mycmd, 3);
#ifdef DEBUG
    Serial.print("tmp cmd = "); Serial.println(tmp);
#endif
    tmp = replystr.substring(2, len + 1);
    tmp.toCharArray(param, tmp.length() + 1);
#ifdef DEBUG
    Serial.print("tmp param = "); Serial.println(tmp);
#endif
  }
  else return;

#ifdef DEBUG
  Serial.print("replystr = "); Serial.println(replystr);
  Serial.print("len = "); Serial.println(len);
  Serial.print("mycmd = "); Serial.println(mycmd);
  Serial.print("param = "); Serial.println(param);
#endif

  // :GP# get the current focuser position
  if (!strcasecmp( mycmd, "GP"))
  {
    char tempString[6];
    sprintf(tempString, "%04X", (unsigned int) currentPosition);
    replystr = String(tempString) + "#";
    sendresponsestr(replystr);
  }

  // :GI# motor is moving - 1 if moving, 0 otherwise
  else if (!strcasecmp( mycmd, "GI"))
  {
    if (isMoving )
      replystr = "01#";
    else
      replystr = "00#";
    sendresponsestr(replystr);
  }

  // :GT# get the current temperature - moonlite compatible
  else if (!strcasecmp( mycmd, "GT"))
  {
    char tempString[6];
    ch1tempval = 20.0;
#ifdef TEMPERATUREPROBE
    if ( isMoving == false )   // if focuser is not moving
    {
      // if there is a temperature probe
      if ( tprobe1 == true )
      {
        gettemp();
      }
      else
      {
        // there is not a probe so just return 20
        ch1tempval = 20.0;
      }
    }
    else
    {
      // focuser is moving, send back last temp value
    }
#endif
    int tpval = (ch1tempval * 2);
    sprintf(tempString, "%04X", (int) tpval);
    replystr = String(tempString) + "#";
    sendresponsestr(replystr);
  }

  // :GZ# get the current temperature
  else if (!strcasecmp( mycmd, "GZ"))
  {
    ch1tempval = 20.0;
#ifdef TEMPERATUREPROBE
    if ( isMoving == false )
    {
      // is focuser moving
      if ( tprobe1 == true )
      {
        // not moving so do a temperature conversion
        gettemp();
      }
      else
      {
        // there is not a probe so just return 20
        ch1tempval = 20.0;
      }
    }
    else
    {
      // focuser is moving, send back last temp value
    }
#endif
    char tempstr[8];
    dtostrf(ch1tempval, 4, 3, tempstr);
    replystr = String(tempstr) + "#";
    sendresponsestr(replystr);
  }

  // :GV# firmware value Moonlite
  else if (!strcasecmp( mycmd, "GV"))
  {
    sendresponsestr("10#");
  }

  // :GF# firmware value
  // myfocuser command
  else if (!strcasecmp( mycmd, "GF"))
  {
    sendresponsestr(programName);
    sendresponsenl();
    sendresponsestr(programVersion + "#");
  }

  // :GM# get the MaxSteps
  // myfocuser command
  else if (!strcasecmp( mycmd, "GM"))
  {
    char tempString[6];
    sprintf(tempString, "%04X", (unsigned int) maxSteps);
    replystr = String(tempString) + "#";
    sendresponsestr(replystr);
  }

  // :GY# get the maxIncrement - set to MaxSteps
  // myfocuser command
  else if (!strcasecmp( mycmd, "GY"))
  {
    char tempString[6];
    sprintf(tempString, "%04X", (unsigned int) maxIncrement);
    replystr = String(tempString) + "#";
    sendresponsestr(replystr);
  }

  // whether half-step is enabled or not, moonlite always return "00"
  else if (!strcasecmp(mycmd, "GH"))
  {
    if ( myfocuser.stepmode == 2 )
      replystr = "FF#";
    else
      replystr = "00#";
    sendresponsestr(replystr);
  }

  // get stepmode
  // myfocuser command
  else if (!strcasecmp(mycmd, "GS"))
  {
    char tempString[6];
    sprintf(tempString, "%02X", myfocuser.stepmode);
    replystr = String(tempString) + "#";
    sendresponsestr(replystr);
  }

  // :GO# get the coilPwr setting
  // myfocuser command
  else if (!strcasecmp( mycmd, "GO"))
  {
    if ( myfocuser.coilPwr )
      replystr = "01#";
    else
      replystr = "00#";
    sendresponsestr(replystr);
  }

  // :GR# get the Reverse Direction setting
  // myfocuser command
  else if (!strcasecmp( mycmd, "GR"))
  {
    if ( myfocuser.ReverseDirection )
      replystr = "01#";
    else
      replystr = "00#";
    sendresponsestr(replystr);
  }

  // :MR# get Motor Speed
  // myfocuser command
  else if (!strcasecmp( mycmd, "MR"))
  {
    replystr = String(motorSpeed) + "#";
    sendresponsestr(replystr);
  }

  // :MU# Get the MotorSpeed Threshold
  // myfocuser command
  else if (!strcasecmp( mycmd, "MU"))
  {
    replystr = String(TSWTHRESHOLD) + "#";
    sendresponsestr(replystr);
  }

  // :MW#         xxx#      Get if motorspeedchange enabled/disabled
  else if (!strcasecmp( mycmd, "MW"))
  {
    replystr = String(motorspeedchange) + "#";
    sendresponsestr(replystr);
  }

  // :GB# LED backlight value, always return "00" - moonlite
  // not implemented in INDI driver
  else if (!strcasecmp( mycmd, "GB"))
  {
    sendresponsestr("00#");
  }

  // :GN# get the new motor position (target)
  // not implemented in INDI driver
  else if (!strcasecmp( mycmd, "GN"))
  {
    char tempString[6];
    sprintf(tempString, "%04X", (unsigned int) targetPosition);
    replystr = String(tempString) + "#";
    sendresponsestr(replystr);
  }

  // :GD# get the current motor step delay, only values of 02, 04, 08, 10, 20
  // not used so just return 02
  else if (!strcasecmp( mycmd, "GD"))
  {
    char tempString[6];
    sprintf(tempString, "%02X", stepdelay);
    replystr = String(tempString) + "#";
    sendresponsestr(replystr);
  }

  // :GC# get temperature co-efficient XX
  else if (!strcasecmp( mycmd, "GC"))
  {
    // do nothing, ignore
    char tempString[6];
    sprintf(tempString, "%02X", tempcomp);
    replystr = String(tempString) + "#";
    sendresponsestr(replystr);
  }

  // :DG# get display state on or off
  else if (!strcasecmp( mycmd, "DG"))
  {
    if ( displayenabled == true )
      replystr = "01#";
    else
      replystr = "00#";
    sendresponsestr(replystr);
  }

  // :GXxxxxx#          get the time that an LCD screen is displayed for (in milliseconds, eg 2500 = 2.5seconds
  else if ( !strcasecmp( mycmd, "GX"))
  {
    char tempString[12];
    sprintf(tempString, "%04X", (unsigned int) myfocuser.updatedisplayintervalNotMoving);
    replystr = String(tempString) + "#";
    sendresponsestr(replystr);
  }

  // :PG    Get temperature precision (9-12)
  else if ( !strcasecmp( mycmd, "PG"))
  {
    int resolution = TEMP_PRECISION;
#ifdef TEMPERATUREPROBE
    if ( tprobe1 == 1 )
    {
      myfocuser.ds18b20resolution = sensor1.getResolution(tpAddress);
      resolution = myfocuser.ds18b20resolution;
      writenow = true;             // updating of EEPROM ON
      previousMillis = millis();   // start 30s time interval
    }
#endif
    replystr = String(resolution) + "#";
    sendresponsestr(replystr);
  }

  // :PN# xx#     get update of position on lcd when moving (00=disable, 01=enable)
  else if ( !strcasecmp( mycmd, "PN"))
  {
    if ( myfocuser.lcdupdateonmove == false )
      replystr = "00#";
    else
      replystr = "01#";
    sendresponsestr(replystr);
  }

  // :PQ#    None      Get if stepsize is enabled in controller (true or false, 0/1)
  else if ( !strcasecmp( mycmd, "PQ"))
  {
    if (myfocuser.stepsizeenabled == true)
      replystr = "1#";
    else
      replystr = "0#";
    sendresponsestr(replystr);
  }

  // :PR#    xxxxx#    Get step size in microns (if enabled by controller)
  else if ( !strcasecmp( mycmd, "PR"))
  {
    replystr = String(myfocuser.stepsize) + "#";
    sendresponsestr(replystr);
  }

  // :FM#    x#      Get Display temp mode (Celsius=0, Fahrenheit=1)
  else if ( !strcasecmp( mycmd, "FM"))
  {
    if (myfocuser.tempmode == true)  // when true display is in celsius
      replystr = "0#";
    else
      replystr = "1#";
    sendresponsestr(replystr);
  }

  // :XY# troubleshooting only - print currentaddr value, use in serial monitor mode is best
  else if (!strcasecmp( mycmd, "XY"))
  {
    sendresponsestr("-#");
  }

  // :FG# initiate a move to the target position
  else if (!strcasecmp( mycmd, "FG"))
  {
    gotonewposition = true;
    isMoving = true;
  }

  // :FQ# stop a move - HALT
  else if (!strcasecmp( mycmd, "FQ"))
  {
    gotonewposition = false;
    isMoving = false;
    targetPosition = currentPosition;
    writenow = true;             // updating of EEPROM ON
    previousMillis = millis();   // start time interval
  }

  // :SNxxxx# set new target position SNXXXX - this is a move command
  // but must be followed by a FG command to start the move
  else if (!strcasecmp( mycmd, "SN"))
  {
    pos = hexstr2long(param);
    targetPosition = pos;
    checktargetmax();
    checktargetzero();
    gotonewposition = false;
    isMoving = false;
  }

  // :PH# home the motor, hard-coded, ignore parameters
  // not implemented in INDI driver
  else if (!strcasecmp( mycmd, "PH"))
  {
    gotonewposition = true;
    isMoving = true;
    targetPosition = 0;
  }

  // :SPxxxx# set current position to received position - no move SPXXXX
  // in INDI driver, only used to set to 0 SP0000 in reset()
  else if (!strcasecmp( mycmd, "SP"))
  {
    pos = hexstr2long(param);
    targetPosition = pos;
    checktargetmax();
    checktargetzero();
    currentPosition = targetPosition;
    writenow = true;             // updating of EEPROM ON
    previousMillis = millis();   // start time interval
    gotonewposition = false;
    isMoving = false;
  }

  // set fullstep mode
  else if (!strcasecmp(mycmd, "SF"))
  {
    myfocuser.stepmode = 1;
    setstepmode();
    updatemotorSpeedDelay();
    writenow = true;             // updating of EEPROM ON
    previousMillis = millis();   // start time interval
  }

  // set halfstep mode
  else if (!strcasecmp(mycmd, "SH"))
  {
    myfocuser.stepmode = 2;
    setstepmode();
    updatemotorSpeedDelay();
    writenow = true;             // updating of EEPROM ON
    previousMillis = millis();   // start time interval
  }

  // set stepmode
  // myfocuser command
  else if (!strcasecmp(mycmd, "SS"))
  {
    pos = hexstr2long(param);
    myfocuser.stepmode = pos;
    setstepmode();
    updatemotorSpeedDelay();
    writenow = true;             // updating of EEPROM ON
    previousMillis = millis();   // start time interval
  }

  // :SOxxxx# set the coilPwr setting
  // myfocuser command
  else if (!strcasecmp( mycmd, "SO"))
  {
    int pos = decstr2int(param);
    if ( pos == 0 )
      myfocuser.coilPwr = false;
    else
      myfocuser.coilPwr = true;
    writenow = true;             // updating of EEPROM ON
    previousMillis = millis();   // start time interval
  }

  // :SRxx# set the Reverse Direction setting
  // myfocuser command
  else if (!strcasecmp( mycmd, "SR"))
  {
    int pos = decstr2int(param);
    if ( pos == 0 )
      myfocuser.ReverseDirection = false;
    else
      myfocuser.ReverseDirection = true;
    writenow = true;             // updating of EEPROM ON
    previousMillis = millis();   // start time interval
  }

  // :DMx# set displaystate C or F
  else if ( !strcasecmp( mycmd, "DM"))
  {
    // int pos = decstr2int(param);
    if ( decstr2int(param) > 0 )
    {
      myfocuser.tempmode = false;
      writenow = true;             // updating of EEPROM ON
      previousMillis = millis();   // start time interval
    }
    else
    {
      myfocuser.tempmode = true;
      writenow = true;             // updating of EEPROM ON
      previousMillis = millis();   // start time interval
    }
  }

  // set motorSpeed - time delay between pulses, acceptable values are 00, 01 and 02 which
  // correspond to a slow, med, high
  // myfocuser command
  else if (!strcasecmp(mycmd, "MS"))
  {
    int pos = decstr2int(param);
    if ( pos == 0 )
      motorSpeed = SLOW;                    // slow
    else if ( pos == 1 )
      motorSpeed = MED;                     // medium
    else if (pos == 2 )
      motorSpeed = FAST;                    // fast
    else
      motorSpeed = SLOW;
    savedmotorSpeed = motorSpeed;        // remember the speed setting
    updatemotorSpeedDelay();
  }

  // :MTxxx# set the MotorSpeed Threshold
  // myfocuser command
  else if (!strcasecmp( mycmd, "MT"))
  {
    int pos = decstr2int(param);
    if ( pos < 50 )
    {
      pos = 50;
    }
    else if ( pos > 200 )
    {
      pos = 200;
    }
    TSWTHRESHOLD = pos;
  }

  // :MVx#       None         Set Enable/Disable motorspeed change when moving
  else if (!strcasecmp( mycmd, "MV"))
  {
    int pos = decstr2int(param);
    motorspeedchange = pos;
  }

  // :MX#          None        Save settings to EEPROM
  else if (!strcasecmp( mycmd, "MX"))
  {
    // copy current settings and write the data to EEPROM
    myfocuser.validdata = 99;
    myfocuser.fposition = currentPosition;
    myfocuser.maxstep = maxSteps;
    EEPROM_writeAnything(currentaddr, myfocuser);    // update values in EEPROM
    writenow = false;
  }

  // :SDxx# set step delay, only acceptable values are 02, 04, 08, 10, 20 which
  // correspond to a stepping delay of 250, 125, 63, 32 and 16 steps
  // per second respectively. Moonlite only
  else if (!strcasecmp( mycmd, "SD"))
  {
    pos = hexstr2long(param);
    stepdelay = (int) pos;
  }

  // :SCxx# set temperature co-efficient XX
  else if (!strcasecmp( mycmd, "SC"))
  {
    // do nothing, ignore
    pos = hexstr2long(param);
    tempcomp = (int) pos;
  }

  // + activate temperature compensation focusing
  else if (!strcasecmp( mycmd, "+"))
  {
    // ignore
  }

  // - disable temperature compensation focusing
  else if (!strcasecmp( mycmd, "-"))
  {
    // ignore
  }

  // :PO# temperature calibration offset POXX in 0.5 degree increments (hex)
  else if (!strcasecmp( mycmd, "PO"))
  {
    // Moonlite only
    // this adds/subtracts an offset from the temperature reading in 1/2 degree C steps
    // FA -3, FB -2.5, FC -2, FD -1.5, FE -1, FF -.5, 00 0, 01 0.5, 02 1.0, 03 1.5, 04 2.0, 05 2.5, 06 3.0
    tempoffsetval = 0.0;
    // param is a char []
    String parm1 = String(param);
    if ( parm1 == "FA" )
      tempoffsetval = -3.0;
    else if ( parm1 == "FB")
      tempoffsetval = -2.5;
    else if ( parm1 == "FC")
      tempoffsetval = -2.0;
    else if ( parm1 == "FD")
      tempoffsetval = -1.5;
    else if ( parm1 == "FE")
      tempoffsetval = -1.0;
    else if ( parm1 == "FF")
      tempoffsetval = -0.5;
    else if ( parm1 == "00")
      tempoffsetval = 0.0;
    else if ( parm1 == "01")
      tempoffsetval = 0.5;
    else if ( parm1 == "02")
      tempoffsetval = 1.0;
    else if ( parm1 == "03")
      tempoffsetval = 1.5;
    else if ( parm1 == "04")
      tempoffsetval = 2.0;
    else if ( parm1 == "05")
      tempoffsetval = 2.5;
    else if ( parm1 == "06")
      tempoffsetval = 3.0;
  }

  // :SMxxx# set new maxSteps position SMXXXX
  // myfocuser command
  else if (!strcasecmp( mycmd, "SM"))
  {
    pos = hexstr2long(param);
    if ( pos > maxFocuserLimit )
      pos = maxFocuserLimit;
    // avoid setting maxSteps below current position
    if ( pos < currentPosition )
      pos = currentPosition;
    // for NEMA17 at 400 steps this would be 5 full rotations of focuser knob
    // for 28BYG-28 this would be less than 1/2 a revolution of focuser knob
    maxSteps = pos;
    // check maxIncement in case its larger
    if ( maxIncrement > maxSteps )
      maxIncrement = maxSteps;
    // signal that the focuser position has changed and should be saved to eeprom
    writenow = true;             // updating of EEPROM ON
    previousMillis = millis();   // start time interval
  }

  // :SYxxxx# set new maxIncrement SYXXXX
  // myfocuser command
  else if (!strcasecmp( mycmd, "SY"))
  {
    pos = hexstr2long(param);
    // ignore
    maxIncrement = maxSteps;
  }

  // :DSx# disable or enable the display setting
  else if (!strcasecmp( mycmd, "DS"))
  {
#ifdef LCDDISPLAY
    if ( decstr2int(param) == 0 )
    {
      // disable the screen by setting clearing the screen and then the text color to black
      // clear the screen
      lcd.noDisplay();
      lcd.noBacklight();
      displayenabled = false;
    }
    else
    {
      // set the text color to white
      lcd.display();
      lcd.backlight();
      displayenabled = true;
    }
#endif
  }

  // :SXxxxx# None    Set updatedisplayNotMoving (length of time an LCD page is displayed for in milliseconds
  else if ( !strcasecmp( mycmd, "SX"))
  {
    pos = hexstr2long(param);
    // bounds check to 2000-4000 2s-4s
    if ( pos < 2000L )
      pos = 2000L;
    if ( pos > 4000L )
      pos = 4000L;
    myfocuser.updatedisplayintervalNotMoving = pos;
    writenow = true;             // updating of EEPROM ON
    previousMillis = millis();   // start time interval
  }

  // :TA#  Reboot Arduino
  else if ( !strcasecmp( mycmd, "TA"))
  {
    software_Reboot();
  }

  // :PS    Set temperature precision (9-12 = 0.5, 0.25, 0.125, 0.0625)
  else if ( !strcasecmp( mycmd, "PS"))
  {
#ifdef TEMPERATUREPROBE
    int precision = decstr2int(param);
    if ( tprobe1 == 1 )
    {
      sensor1.setResolution( tpAddress, precision );
      myfocuser.ds18b20resolution = precision;        // save setting in EEPROM
      writenow = true;             // updating of EEPROM ON
      previousMillis = millis();   // start time interval
    }
    else
    {
      // no probe, set to default
      myfocuser.ds18b20resolution = TEMP_PRECISION;
    }
#endif
  }

  // :PMxx#    None    set update of position on lcd when moving (00=disable, 01=enable)
  else if ( !strcasecmp( mycmd, "PM"))
  {
    if ( decstr2int(param) == 0 )
      myfocuser.lcdupdateonmove = false;
    else
      myfocuser.lcdupdateonmove = true;
    writenow = true;             // updating of EEPROM ON
    previousMillis = millis();   // start time interval
  }

  // :PZxx#   None      Set the return of user specified stepsize to be OFF - default (0) or ON (1)
  else if ( !strcasecmp( mycmd, "PZ"))
  {
    int pos = decstr2int(param);
    if ( pos == 1 )
    {
      myfocuser.stepsizeenabled = true;
    }
    else   // any other value please disable
    {
      myfocuser.stepsizeenabled = false;
    }
    writenow = true;             // updating of EEPROM ON
    previousMillis = millis();   // start time interval
  }

  // :PPxxxx#  None     Set the step size value - double type, eg 2.1
  else if ( !strcasecmp( mycmd, "PP"))
  {
    // convert param to float
    String str = param;
    str = str + "";      // add end of string terminator
    double tempstepsize = (double) str.toFloat();
    if ( tempstepsize < 0 )
    {
      tempstepsize = 0;
    }
    myfocuser.stepsize = tempstepsize;
    writenow = true;             // updating of EEPROM ON
    previousMillis = millis();   // start time interval
  }

  // :XZ# troubleshooting only - reset focuser defaults
  // myfocuser command
  else if (!strcasecmp( mycmd, "XZ"))
  {
    currentaddr = 0;
    ResetFocuserDefaults();
    // Set focuser defaults.
    currentPosition = myfocuser.fposition;
    targetPosition = myfocuser.fposition;
    maxSteps = myfocuser.maxstep;
  }
}

// convert hex string to long int
long hexstr2long(char *line)
{
  long ret = 0;
  ret = strtol(line, NULL, 16);
  return (ret);
}

// convert string to int
int decstr2int(char *line)
{
  int ret = 0;
  String Str(line);
  ret = Str.toInt();
  return ret;
}

#ifdef LCDDISPLAY
// displaylcd first screen
void displaylcdpage1()
{
  lcd.clear();
  lcd.print("C=");
  lcd.print(currentPosition);
  lcd.setCursor( 0, 1);
  lcd.print("T=");
  lcd.print(targetPosition);
  lcd.setCursor( 8, 0 );
  lcd.print("PW=");
  if ( myfocuser.coilPwr )
    lcd.print("ON");
  else
    lcd.print("OF");
  lcd.setCursor( 15, 0);
  // temp compensation will go here

  lcd.setCursor( 8, 1 );
  lcd.print("RD=");
  if ( myfocuser.ReverseDirection )
    lcd.print("ON");
  else
    lcd.print("OF");
  lcd.setCursor( 14, 1 );
  if ( myfocuser.stepmode == 1 )
    lcd.print("F");
  else if ( myfocuser.stepmode == 2 )
    lcd.print("H");
  else if ( myfocuser.stepmode == 4 )
    lcd.print("4");
  else if ( myfocuser.stepmode == 8 )
    lcd.print("8");
  else if ( myfocuser.stepmode == 16 )
    lcd.print("16");
  else if ( myfocuser.stepmode == 32 )
    lcd.print("32");
}

// displaylcd second screen
void displaylcdpage2()
{
  char tempString[8];
  // display values on LCD1602
  lcd.clear();
  lcd.print("Temp   =");
  if ( tprobe1 == false )
  {
    lcd.print("--");
    if (myfocuser.tempmode == true )
    {
      lcd.print("c");
    }
    else
    {
      lcd.print("f");
    }
  }
  else
  {
    if (myfocuser.tempmode == true )
    {
      /* 4 is mininum width, 3 is precision; float value is copied onto str_temp*/
      dtostrf(ch1tempval, 4, 3, tempString);
      lcd.print(tempString);
      lcd.print("c");
    }
    else
    {
      float tempvalf;
      tempvalf = (ch1tempval * 1.8) + 32;
      dtostrf(tempvalf, 4, 3, tempString);
      lcd.print(tempString);
      lcd.print("f");
    }
  }
  lcd.setCursor( 0, 1);
  lcd.print("MaxStep=");
  lcd.print(myfocuser.maxstep);
}
#endif

#ifdef TEMPERATUREPROBE
// find the address of the DS18B20 sensor probe
void findds18b20address()
{
  // look for probes
  // Search the wire for address
  if (sensor1.getAddress(tpAddress, 0))
  {
    tprobe1 = 1;  // there is a probe1
  }
  // device address is now saved in tpAddress
}

void gettemp()
{
  sensor1.requestTemperatures();
  delay(700 / (1 << (12 - myfocuser.ds18b20resolution))); // should enough time to wait
  // get channel 1 temperature, always in celsius
  double result = sensor1.getTempCByIndex(0);
  // sometimes sensor returns -127, not sure why, so check and use last reading if this is the case
  // range of sensor is -55c to +125c
  // limit to values of -40c to +80c (-40f to 176f)
  int tempval = (int) result;
  if ( tempval < -40 )
    ch1tempval = lasttempval;
  else if ( tempval > 80 )
    ch1tempval = lasttempval;
  else
  {
    ch1tempval = result;
    lasttempval = ch1tempval;         // remember last reading
  }
}
#endif

// Setup
void setup()
{
  int datasize;      // will hold size of the struct myfocuser - 6 bytes
  int nlocations;    // number of storage locations available in EEPROM
  bool found;        // did we find any stored values?

  Serial.begin(SerialPortSpeed);
#ifdef BLUETOOTH
  btSerial.begin(BTPortSpeed);            // start bt adapter
  clearbtPort();
#endif

#ifdef BUZZER
  pinMode(Buzzer, OUTPUT);
  digitalWrite( Buzzer, 1);
#endif
#ifdef INOUTLEDS
  pinMode( bledIN, OUTPUT);
  pinMode( gledOUT, OUTPUT);
  digitalWrite( bledIN, 1 );
  digitalWrite( gledOUT, 1 );
#endif
  previousMillis = 0L;
  myinterval = TIMEINTERVAL;
  stepdelay = 0;
  tempcomp = 0;
  tempoffsetval = 0;
  eoc = 0;
  idx = 0;
  isMoving = false;
  gotonewposition = false;
  memset(line, 0, MAXCOMMAND);
#ifdef PUSHBUTTONS
  PBVal = 0;
#endif
  targetPosition = currentPosition = STARTPOS;
  maxFocuserLimit = MAXSTEPLIMIT;
  maxSteps = STARTMAXPOS;
  maxIncrement = MAXINC;
  minimumPosition = 0L;
  gotonewposition = false;
  displayenabled = false;
#ifdef LCDDISPLAY
  displayenabled = true;
  olddisplaytimestampNotMoving = 0L;
  LCD1602Screen = 1;
  updatecount = 0;
  lcdupdatestepcount = 15;
#endif
#ifdef TEMPERATUREPROBE
  lasttempconversion = 0L;
  temprefreshrate = TMPREFRESHRATE;
#endif

  currentaddr = 0;
  found = false;
  writenow = false;
  datasize = sizeof( myfocuser );
  nlocations = EEPROMSIZE / datasize;

  for (int lp1 = 0; lp1 < nlocations; lp1++ )
  {
    int addr = lp1 * datasize;
    EEPROM_readAnything( addr, myfocuser );
    // check to see if the data is valid
    if ( myfocuser.validdata == 99 )
    {
      // data was erased so write some default values
      currentaddr = addr;
      found = true;
    }
  }
  if ( found == true )
  {
    EEPROM_readAnything( currentaddr, myfocuser );
    myfocuser.validdata = 0;
    EEPROM_writeAnything(currentaddr, myfocuser);
    currentaddr += datasize;
    if ( currentaddr >= (nlocations * datasize) )
      currentaddr = 0;
    myfocuser.validdata = 99;
    EEPROM_writeAnything(currentaddr, myfocuser);
  }
  else
  {
    ResetFocuserDefaults();
  }

  currentPosition = myfocuser.fposition;  // Set focuser defaults from saved values in EEPROM.
  targetPosition = myfocuser.fposition;
  maxSteps = myfocuser.maxstep;

  ch1tempval  = 20.0;
  lasttempval = 20.0;
  tprobe1 = 0;
#ifdef TEMPERATUREPROBE
  sensor1.begin();
  sensor1.getDeviceCount();
  findds18b20address();
  if ( tprobe1 == 1 )
  {
    sensor1.setResolution( tpAddress, myfocuser.ds18b20resolution );
    gettemp();
  }
#endif
#ifdef LCDDISPLAY
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.print(programName);
  lcd.setCursor(0, 1);
  lcd.print(programVersion);
  olddisplaytimestampNotMoving = millis();
  updatecount = 0;
#endif

//sleep and reset pin
  pinMode(  9, OUTPUT );
  pinMode(  10, OUTPUT );
  digitalWrite( 9, 1 );
  digitalWrite( 10, 1 );

  pinMode(  myDir, OUTPUT );
  pinMode(  myStep, OUTPUT );
  pinMode(  myM0, OUTPUT );
  pinMode(  myM1, OUTPUT );
  pinMode(  myM2, OUTPUT );
  digitalWrite( myDir, 0 );
  digitalWrite( myStep, 0 );
  boardstate = OUTPUTENABLED;
  pinMode( myEnable, OUTPUT );
  enableoutput();
  stepontime = PULSETIME;
  TSWTHRESHOLD = MTRTHRESHHOLD;
  motorspeedchange = 0;
  setstepmode();
  motorSpeed = FAST;
  savedmotorSpeed = FAST;
  updatemotorSpeedDelay();

  if ( myfocuser.updatedisplayintervalNotMoving < 2000 )
    myfocuser.updatedisplayintervalNotMoving = 2000;
  if ( myfocuser.updatedisplayintervalNotMoving > 4000 )
    myfocuser.updatedisplayintervalNotMoving = 4000;

  writenow = false;

#ifdef PUSHBUTTONS
  pinMode( PBswitchesPin, INPUT_PULLUP );
  PBVal = readpbswitches(PBswitchesPin);
#endif

#ifdef INOUTLEDS
  digitalWrite( bledIN, 0 );
  digitalWrite( gledOUT, 0 );
#endif
#ifdef BUZZER
  digitalWrite( Buzzer, 0);
#endif
}

// Main Loop
void loop()
{
#ifdef BLUETOOTH
  btSerialEvent();                          // check for command from bt adapter
#endif

  if ( queue.count() >= 1 )                 // check for serial command
  {
    processCommand();
  }

#ifdef PUSHBUTTONS
  PBVal = readpbswitches(PBswitchesPin);
  if ( PBVal != 0)
  {
    // if its a genuine button press
    delay(20);
    PBVal = readpbswitches(PBswitchesPin);
    // now check the pbval using a switch for 1 2 and 3
    switch ( PBVal )
    {
      case 0:   // both switches are off
        break;  // do nothing
      case 1:                        // toggle sw1 is ON and 2 is off
        // move IN
        targetPosition = targetPosition - 1;
        if ( targetPosition < 0 ) targetPosition = 0;
        gotonewposition = true;
        break;
      case 2:                        // toggle sw2 is ON and SW1 is OFF
        // move OUT
        targetPosition = targetPosition + 1;
        // this line required because only target commands received are checked
        if ( targetPosition > myfocuser.maxstep )
        {
          targetPosition = myfocuser.maxstep;
        }
        gotonewposition = true;
        break;
      case 3:                        // toggle sw1 and sw2 are ON
#ifdef BUZZER
        digitalWrite(Buzzer, 1);    // turn on buzzer
#endif
        while ( readpbswitches(PBswitchesPin) == 3 )  // wait for pb to be released
          ;
        currentPosition = 0;
        targetPosition = 0;
        isMoving = false;
#ifdef BUZZER
        digitalWrite(Buzzer, 0);     // turn off buzzer
#endif
        break;
      default:
        // do nothing
        break;
    } // end of switch
  } // end of pb test
#endif

  // Move the position by a single step if target <> current position
  if ((targetPosition != currentPosition) && (gotonewposition == true))
  {
    // Slow down if approaching home position
    if ( motorspeedchange == 1 )
    {
      // Slow down if approaching home position
      long nearinghomepos = currentPosition - targetPosition;
      nearinghomepos = abs(nearinghomepos);
      if ( nearinghomepos < TSWTHRESHOLD )
      {
#ifdef DEBUG
        Serial.println("Speed change to Slow because nearing position");
#endif
        motorSpeed = SLOW;                    // slow
        updatemotorSpeedDelay();              // set the correct delay
      }
    }

    if ( boardstate == OUTPUTDISABLED )       // if board is not enabled, we need to enable it else motor will not step
    {
      boardstate = OUTPUTENABLED;
      enableoutput();              // have to enable driver board
    }
    isMoving = true;              // focuser is moving
    writenow = true;              // updating of EEPROM ON
    //previousMillis = millis();    // keep updating previousMillis whilst focuser is moving

    // Going Anticlockwise to lower position
    if (targetPosition < currentPosition)
    {
      enableoutput();
      anticlockwise();
      currentPosition--;
    }

    // Going Clockwise to higher position
    if (targetPosition > currentPosition)
    {
      // do not need to check if > maximumPosition as its done when a target command is receieved
      enableoutput();
      clockwise();
      currentPosition++;
    }

#ifdef LCDDISPLAY
    // check if lcd needs updating during move
    if ( myfocuser.lcdupdateonmove == 1 )
    {
      updatecount++;
      if ( updatecount > lcdupdatestepcount )
      {
        // every 15 times
        lcd.clear();
        lcd.print("C=");
        lcd.print(currentPosition);
        lcd.setCursor(0, 1);
        lcd.print("T=");
        lcd.print(targetPosition);
        updatecount = 0;
      }
    }
#endif
    delayMicroseconds(motorSpeedDelay);     // microseconds, 1uS = 1000mS
  }
  else
  {
    // focuser is NOT moving now, move is completed
    isMoving = false;
    gotonewposition = false;

    // reset motorSpeed
    motorSpeed = savedmotorSpeed;
    updatemotorSpeedDelay();

#ifdef LCDDISPLAY
    // see if the display needs updating
    long newdisplaytimestampNotMoving = millis();
    if ( ((newdisplaytimestampNotMoving - olddisplaytimestampNotMoving) > myfocuser.updatedisplayintervalNotMoving) || (newdisplaytimestampNotMoving < olddisplaytimestampNotMoving))
    {
      olddisplaytimestampNotMoving = newdisplaytimestampNotMoving;    // update the timestamp
      if ( LCD1602Screen == 1 )
      {
        displaylcdpage1();                           // update ALL the display values - takes about 2s
        LCD1602Screen = 2;
      }
      else if ( LCD1602Screen == 2)
      {
        displaylcdpage2();                          // 2nd screen, display temperature
        LCD1602Screen = 1;
      }
    }
#endif
#ifdef TEMPERATUREPROBE
    // if there is a temperature probe
    if ( tprobe1 == true)
    {
      long tempnow = millis();
      // see if the temperature needs updating - done automatically every 5s
      if ( ((tempnow - lasttempconversion) > temprefreshrate) || (tempnow < lasttempconversion))
      {
        gettemp();
        lasttempconversion = tempnow;
      } // end of if( tempnow .... )
    } // end of if( tprobe1 == 1 )
#endif

    // is it time to update EEPROM settings?
    if ( writenow == true )
    {
      // decide if we have waited 10s after the last move, if so, update the EEPROM
      long currentMillis = millis();
      if ( ((currentMillis - previousMillis) > myinterval) || (currentMillis < previousMillis) )
      {
        previousMillis = currentMillis;
        // copy current settings and write the data to EEPROM
        myfocuser.validdata = 99;
        myfocuser.fposition = currentPosition;
        myfocuser.maxstep = maxSteps;
        EEPROM_writeAnything(currentaddr, myfocuser);    // update values in EEPROM
        writenow = false;
      }
    }
    
    clearOutput(); // release the stepper coils to save power
    
  }  // end of else
}

// disable the stepper motor outputs
void clearOutput()
{
  // check to see what is selected, keep or release
  if ( !myfocuser.coilPwr )
  {
    disableoutput();
  }
}

void clearSerialPort()
{
  while ( Serial.available() )
    Serial.read();
}

#ifdef BLUETOOTH
void clearbtPort()
{
  while (btSerial.available())
  {
    btSerial.read();
  }
}

void btSerialEvent()
{
  while (btSerial.available() && (bteoc == 0) )
  {
    char btinChar = (char) btSerial.read();
    if ((btinChar != '#') && (btinChar != ':'))
    {
      btline[btidx++] = btinChar;
      if (btidx >= MAXCOMMAND)
      {
        btidx = MAXCOMMAND - 1;
      }
    }
    else
    {
      if (btinChar == '#')
      {
        bteoc = 1;
        btidx = 0;
        queue.push(String(btline));
        bteoc = 0;
        memset( btline, 0, MAXCOMMAND);
      }
    }
  }
}
#endif

// SerialEvent occurs whenever new data comes in the serial RX.
void serialEvent()
{
  // : starts the command, # ends the command, do not store these in the command buffer
  // read the command until the terminating # character
  while (Serial.available() && !eoc)
  {
    char inChar = Serial.read();
    if (inChar != '#' && inChar != ':')
    {
      line[idx++] = inChar;
      if (idx >= MAXCOMMAND)
      {
        idx = MAXCOMMAND - 1;
      }
    }
    else
    {
      if (inChar == '#')
      {
        eoc = 1;
        idx = 0;
        queue.push(String(line));
        eoc = 0;
        memset( line, 0, MAXCOMMAND);
      }
    }
  }
}

