/* X-plane UFCP code
 * 
 * Written as proof of concept by Mike
 *
 * You may freely distribute or modify this code for non-commercial open-source applications while mentioning the sources 
 */
#include <LiquidCrystalFast.h>
#include <Keypad.h>

///////////////////////////////////////////////////////////////////////////////
// Hardware setup
//

///////////////////
// Input: Keypad
//
 const byte ROWS = 5; // We have 5 rows of keys
 const byte COLS = 7; // And each row contains 7 buttons 
 char keys[ROWS][COLS] = {
   {'1','2','3','H','T','B','M'},
   {'4','5','6','A','Y','n','P'},
   {'7','8','9','V','F','N','R'},
   {'#','0','*','S','I','g','b'},
   {' ','W','L','G','c','C',' '},
 };
 byte rowPins[ROWS] = {27,0,1,2,3};
 byte colPins[COLS] = {17,15,14,13,12,11,9};
 Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
 
///////////////////
// Output hardware
//
enum LCD_PINS {
  RS = 39,
  RW,
  EN, // these are automatically numbered from 39 to 45
  D4, // thanks to the magic of enum
  D5,
  D6,
  D7 // = 45
};
LiquidCrystalFast lcd(RS, RW, EN, D4, D5, D6, D7);

///////////////////////////////////////////////////////////////////////////////
// X-Plane objects we will be needing
//
 FlightSimCommand AP_On;
 FlightSimCommand FD_On;
 FlightSimCommand AP_WLV;
 FlightSimCommand AP_HDG;
 FlightSimCommand AP_ALT;
 FlightSimCommand AP_VVI;
 FlightSimCommand AP_SPD;
 FlightSimCommand AP_LOC;
 FlightSimCommand AP_GS;
 FlightSimCommand AP_SYNC;
 FlightSimCommand AP_FLCH;
 FlightSimCommand AP_IAS;
 FlightSimCommand AP_TEST;
 FlightSimCommand AP_NAV1;
 FlightSimCommand AP_NAV2;
 FlightSimCommand AP_GPS;
 FlightSimCommand AP_BC;
 FlightSimInteger Weapon_Guns;
 FlightSimInteger Weapon_AG;
 FlightSimInteger Weapon_AA;
 FlightSimInteger Audio_Com1;
 FlightSimCommand Audio_Com1_Monitor;
 FlightSimCommand Audio_Com1_Flip;
 FlightSimInteger Audio_Com2;
 FlightSimCommand Audio_Com2_Monitor;
 FlightSimCommand Audio_Com2_Flip;
 FlightSimInteger Radio_Nav1;
 FlightSimCommand Radio_Nav1_Flip;
 FlightSimInteger Radio_Nav2;
 FlightSimCommand Radio_Nav2_Flip;
 FlightSimInteger Radio_Adf1;
 FlightSimCommand Radio_Adf1_Flip;
 FlightSimInteger Radio_Adf2;
 FlightSimCommand Radio_Adf2_Flip;
 FlightSimInteger Autopilot;
 FlightSimFloat Autopilot_Heading;
 FlightSimFloat Autopilot_Altitude;
 FlightSimFloat Autopilot_VVI;
 FlightSimFloat Autopilot_Speed;
 FlightSimInteger Autopilot_isMach;
 FlightSimFloat Autopilot_DH;
 FlightSimCommand Autopilot_terrain;
 FlightSimInteger Autopilot_State;
 FlightSimInteger Radar_range;
 FlightSimFloat Bus_Voltage_1A; // Tuboflan Generator A
 FlightSimFloat Bus_Voltage_1B; // Tuboflan Generator B
 FlightSimFloat Bus_Voltage_2; // APU
 FlightSimFloat Bus_Voltage_3; // RAMJet
 FlightSimFloat Bus_Voltage_4; // RAT
 FlightSimFloat Barometer;

///////////////////////////////////////////////////////////////////////////////
// Program Variables
//
 String NumericalCommand = "";
 byte FlightSimEnabled = 1;

///////////////////////////////////////////////////////////////////////////////
// We set up the things we will need for the program
void setup()  {
  // by keeping crap out of here the setup is nice and modular.
  // also, by using functions everything is neatly packed together in a convenient place.
  pinMode (RS, OUTPUT);
  pinMode (RW, OUTPUT);
  pinMode (EN, OUTPUT);
  pinMode (D4, OUTPUT);
  pinMode (D5, OUTPUT);
  pinMode (D6, OUTPUT);
  pinMode (D7, OUTPUT);
  lcd.begin (20, 4);
  pinMode (LED_BUILTIN, OUTPUT);
  AP_On = XPlaneRef("sim/autopilot/servos_on");
  FD_On = XPlaneRef("sim/autopilot/fdir_on");
  AP_WLV = XPlaneRef("sim/autopilot/wing_leveler");
  AP_HDG = XPlaneRef("sim/autopilot/heading");
  AP_ALT = XPlaneRef("sim/autopilot/altitude_hold");
  AP_VVI = XPlaneRef("sim/autopilot/vertical_speed_pre_sel");
  AP_SPD = XPlaneRef("sim/autopilot/autothrottle_toggle");
  AP_LOC = XPlaneRef("sim/autopilot/NAV");
  AP_GS = XPlaneRef("sim/autopilot/glide_slope");
  AP_SYNC = XPlaneRef("sim/autopilot/pitch_sync");
  AP_FLCH = XPlaneRef("sim/autopilot/level_change");
  AP_IAS = XPlaneRef("sim/autopilot/airspeed_sync");
  AP_TEST = XPlaneRef("sim/autopilot/test_auto_annunciators");
  AP_NAV1 = XPlaneRef("sim/autopilot/hsi_select_nav_1");
  AP_NAV2 = XPlaneRef("sim/autopilot/hsi_select_nav_2");
  AP_GPS = XPlaneRef("sim/autopilot/hsi_select_gps");
  AP_BC = XPlaneRef("sim/autopilot/back_course");
  Weapon_Guns = XPlaneRef("sim/cockpit/weapons/guns_armed");
  Weapon_AG = XPlaneRef("sim/cockpit/weapons/bombs_armed");
  Weapon_AA = XPlaneRef("sim/cockpit/weapons/missiles_armed");
  Audio_Com1 = XPlaneRef("sim/cockpit/radios/com1_freq_hz");
  Audio_Com1_Monitor = XPlaneRef("sim/audio_panel/select_audio_com1");
  Audio_Com1_Flip = XPlaneRef("sim/radios/com1_standy_flip");
  Audio_Com2 = XPlaneRef("sim/cockpit/radios/com2_freq_hz");
  Audio_Com2_Monitor = XPlaneRef("sim/audio_panel/select_audio_com2");
  Audio_Com2_Flip = XPlaneRef("sim/radios/com2_standy_flip");
  Radio_Nav1 = XPlaneRef("sim/cockpit/radios/nav1_freq_hz");
  Radio_Nav1_Flip = XPlaneRef("sim/radios/nav1_standy_flip");
  Radio_Nav2 = XPlaneRef("sim/cockpit/radios/nav2_freq_hz");
  Radio_Nav2_Flip = XPlaneRef("sim/radios/nav2_standy_flip");
  Radio_Adf1 = XPlaneRef("sim/cockpit/radios/adf1_freq_hz");
  Radio_Adf1_Flip = XPlaneRef("sim/radios/adf1_standy_flip");
  Radio_Adf2 = XPlaneRef("sim/cockpit/radios/adf2_freq_hz");
  Radio_Adf2_Flip = XPlaneRef("sim/radios/adf2_standy_flip");
  Autopilot = XPlaneRef("sim/cockpit/autopilot/autopilot_mode");
  Autopilot_Heading = XPlaneRef("sim/cockpit/autopilot/heading_mag");
  Autopilot_Altitude = XPlaneRef("sim/cockpit/autopilot/altitude");
  Autopilot_VVI = XPlaneRef("sim/cockpit/autopilot/vertical_velocity");
  Autopilot_Speed = XPlaneRef("sim/cockpit/autopilot/airspeed");
  Autopilot_isMach = XPlaneRef("sim/cockpit/autopilot/airspeed_is_mach");
  Autopilot_DH = XPlaneRef("sim/cockpit/misc/radio_altimeter_minimum");
  Autopilot_terrain = XPlaneRef("sim/autopilot/terrain_following");
  Autopilot_State = XPlaneRef("sim/cockpit/autopilot/autopilot_state");
  Bus_Voltage_1A = XPlaneRef("sim/cockpit2/electrical/bus_volts[0]"); // Tuboflan Generator A
  Bus_Voltage_1B = XPlaneRef("sim/cockpit2/electrical/bus_volts[1]"); // Tuboflan Generator B
  Bus_Voltage_2 = XPlaneRef("sim/cockpit2/electrical/bus_volts[2]"); // APU
  Bus_Voltage_3 = XPlaneRef("sim/cockpit2/electrical/bus_volts[3]"); // RAMJet
  Bus_Voltage_4 = XPlaneRef("sim/cockpit2/electrical/bus_volts[4]"); // RAT
  Radar_range = XPlaneRef("sim/cockpit/avidyne/map_range_sel");
  Barometer = XPlaneRef("sim/cockpit/misc/barometer_setting");
}

///////////////////////////////////////////////////////////////////////////////
// And then we loop. This is the main function that runs
void loop(){
  ///////////////////////////////////////////////////////////////////////////////
  // The first step in the loop() should be an update for the DataRefs
  if(FlightSim.isEnabled() != FlightSimEnabled){
    FlightSimEnabled = FlightSim.isEnabled();
  }
  FlightSim.update();
  
  char key1 = keypad.getKey();
  if (key1 != NO_KEY){
    if (key1 != ' ') {
      if(key1 == 'W'){
        AP_WLV = 1;
        AP_WLV = 0;
      } else if(key1 == 'L'){
        AP_LOC = 1;
        AP_LOC = 0;
      } else if(key1 == 'G'){
        AP_GS = 1;
        AP_GS = 0;
      } else if(key1 == 'c'){
        Audio_Com1_Monitor = 1;
        Audio_Com1_Monitor = 0;
      } else if(key1 == 'C'){
        Audio_Com2_Monitor = 1;
        Audio_Com2_Monitor = 0;
      } else if(key1 == 'S'){
        AP_SPD = 1;
        AP_SPD = 0;
      } else if(key1 == 'I'){
        AP_IAS = 1;
        AP_IAS = 0;
      } else if(key1 == 'g'){
        AP_GPS = 1;
        AP_GPS = 0;
      } else if(key1 == 'b'){
        AP_BC = 1;
        AP_BC = 0;
      } else if(key1 == 'V'){
        AP_VVI = 1;
        AP_VVI = 0;
      } else if(key1 == 'F'){
        AP_FLCH = 1;
        AP_FLCH = 0;
      } else if(key1 == 'N'){
        AP_NAV2 = 1;
        AP_NAV2 = 0;
      } else if(key1 == 'A'){
        AP_ALT = 1;
        AP_ALT = 0;
      } else if(key1 == 'Y'){
        AP_SYNC = 1;
        AP_SYNC = 0;
      } else if(key1 == 'n'){
        AP_NAV1 = 1;
        AP_NAV1 = 0;
      } else if(key1 == 'P'){
        if(Autopilot != 1){
          Autopilot = 1;
        } else {
          Autopilot = 2;
        }
      } else if(key1 == 'H'){
        AP_HDG = 1;
        AP_HDG = 0;
      } else if(key1 == 'T'){
        AP_TEST = 1;
        AP_TEST = 0;
      } else if(key1 == 'B'){
         Weapon_Guns = 0;
         Weapon_AA = 0;
         Weapon_AG = 1;
      } else if(key1 == 'M'){        
         Weapon_Guns = 0;
         Weapon_AG = 0;
         Weapon_AA = 1;
      } else {
        if(key1 == '#'){
          NumericalCommand = String("");
        } else if(key1 == '*'){
          byte CommandValidation = ParseCommand(NumericalCommand);
          NumericalCommand = String("");
        } else {
          NumericalCommand = String(NumericalCommand + key1);
        }
        //UpdateLCD_Command(NumericalCommand, CommandValidation);
      }      
    }
    FlightSim.update();    
    while(key1 != NO_KEY){
      delay(50);
      key1 = keypad.getKey();
    }
  }
}

byte ParseCommand(String Command){
  byte CommandValidation = 1;
  if(Command.startsWith("11")) {
    if(Command == "11"){
      Radio_Nav1_Flip.once();
    } else if(Command.length() == 7){
      Command = Command.substring(2);
      Radio_Nav1 = Command.toInt();
    } else {
      CommandValidation = 0;
    }
  } else if(Command.startsWith("12")) {
    if(Command == "12"){
      Radio_Nav2_Flip.once();
    } else if(Command.length() == 7){
      Command = Command.substring(2);
      Radio_Nav2 = Command.toInt();
    } else {
      CommandValidation = 0;
    }
  } else if(Command.startsWith("21")) {
    if(Command == "21"){
      Audio_Com1_Flip.once();
    } else if(Command.length() == 7){
      Command = Command.substring(2);
      Audio_Com1 = Command.toInt();
    } else {
      CommandValidation = 0;
    }
  } else if(Command.startsWith("22")) {
    if(Command == "22"){
      Audio_Com1_Flip.once();
    } else if(Command.length() == 7){
      Command = Command.substring(2);
      Audio_Com2 = Command.toInt();
    } else {
      CommandValidation = 0;
    }
  } else if(Command.startsWith("31")) {
    if(Command == "31"){
      Radio_Adf1_Flip.once();
    } else if(Command.length() == 5){
      Command = Command.substring(2);
      Radio_Adf1 = Command.toInt();
    } else {
      CommandValidation = 0;
    }
  } else if(Command.startsWith("32")) {
    if(Command == "32"){
      Radio_Adf2_Flip.once();
    } else if(Command.length() == 5){
      Command = Command.substring(2);
      Radio_Adf2 = Command.toInt();
    } else {
      CommandValidation = 0;
    }
  } else if(Command.startsWith("4")) {
    if(Command.length() == 4){
      Command = Command.substring(1);
      Autopilot_Heading = Command.toInt();
    } else {
      CommandValidation = 0;
    }
  } else if(Command.startsWith("5")) {
    if(Command.length() == 4){
      Command = Command.substring(1);
      Autopilot_Speed = Command.toInt();
    } else {
      CommandValidation = 0;
    }
  } else if(Command.startsWith("6")) {
    if(Command == "6"){
      Command = Command.substring(1);
      Barometer = 29.92;      
    } else if(Command.length() == 5){
      Command = Command.substring(1);
      Barometer = Command.toInt() / 100;            
    } else {
      CommandValidation = 0;
    }
  } else if(Command.startsWith("7")) {
    if(Command.length() == 4){
      Command = Command.substring(1);
      Autopilot_Altitude = ( Command.toInt() * 100 );
    } else {
      CommandValidation = 0;
    }
  } else if(Command == "8") {
    Autopilot_terrain.once();
  } else if(Command.startsWith("9")) {
    if(Command.length() == 4){
      if(Command.startsWith("90")){
        Command = Command.substring(2);
        Autopilot_VVI = -100 * Command.toInt();
      } else if(Command.startsWith("91")) {
        Command = Command.substring(2);
        Autopilot_VVI = 100 * Command.toInt();
      } else {
        CommandValidation = 0;
      }
    } else {
      CommandValidation = 0;
    }
  } else if(Command.startsWith("0")) {
    if(Command.length() == 4){
      Command = Command.substring(1);
      Autopilot_DH = Command.toInt();
    } else {
      CommandValidation = 0;
    }
  } else if(Command.startsWith("R")) { //Map zoom change
    if(Command.length() == 2){
      Command = Command.substring(1);
      Radar_range = Command.toInt();
    } else {
      CommandValidation = 0;
    }
  } else {
    CommandValidation = 0;
  }
  return CommandValidation;
}

void SetupLCD_Screen(){
  lcd.setCursor(0, 0);
  lcd.print("CMD> _");
  lcd.setCursor(0, 2);
  lcd.print("HDG");
  lcd.setCursor(0, 3);
  lcd.print("SPD");
  lcd.setCursor(16, 2);
  lcd.print("ALT");
  lcd.setCursor(16, 3);
  lcd.print("VVI");  
}

void UpdateLCD_Command(String Command, byte Validation){
  if(Validation == 0){
  }else if(Command.startsWith("1")) {
  } else if(Command.startsWith("2")) {
  } else if(Command.startsWith("3")) {
  } else if(Command.startsWith("4")) {
  } else if(Command.startsWith("5")) {
  } else if(Command.startsWith("6")) {
  } else if(Command.startsWith("7")) {
  } else if(Command.startsWith("8")) {
  } else if(Command.startsWith("9")) {
  } else if(Command.startsWith("0")) {
  } else {
    
  }
}

