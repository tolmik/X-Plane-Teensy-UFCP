/* X-plane UFCP code
 * 
 * Written as proof of concept by Mike
 *
 * You may freely distribute or modify this code for non-commercial open-source applications while mentioning the sources 
 */
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
 FlightSimCommand Weapon_Arm;
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
 FlightSimInteger Radio_XPNDR;
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
 byte CommandValidation = 0;

///////////////////////////////////////////////////////////////////////////////
// We set up the things we will need for the program
void setup()  {
  Serial.begin(9600);
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
  Weapon_Arm = XPlaneRef("sim/weapons/master_arm_on");
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
  Radio_XPNDR = XPlaneRef("sim/cockpit/radios/transponder_code");
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
  //Serial.println(Radar_range);
  char key1 = keypad.getKey();
  if (key1 != NO_KEY){   
    switch(byte(key1)){
      case 35: //RCL
        NumericalCommand = String("");
        break;
      case 42: //ENTR
        CommandValidation = ParseCommand(NumericalCommand);
        NumericalCommand = String("");
        break;
      case 48 ... 57: //0-9
        NumericalCommand = String(NumericalCommand + key1);      
        break;  
      case 65: //ALT
        AP_ALT.once();
        break;
      case 66: //AG
        Weapon_Arm.once();
        Weapon_AG = 1;        
        break;  
      case 67: //Com2
        Audio_Com2_Monitor.once();
        break;
      case 70: //FLCH
        AP_FLCH.once();
        break;  
      case 71: //GS
        AP_GS.once();
        break;
      case 72: //HDG
        AP_HDG.once();  
      case 73: //IAS  
        AP_IAS.once();
        break;
      case 76: //LOC
        AP_LOC.once();
        break;
      case 77: //AA
        Weapon_Arm.once();
        Weapon_AA = 1;      
      case 78: //NAV2
        AP_NAV2.once();
        break;
      case 80: //FD
        if(Autopilot != 1){
          Autopilot = 1;
        } else {
          Autopilot = 2;
        }
        break;
      case 82: //R 
        NumericalCommand = String(NumericalCommand + key1);      
        break;          
      case 83: //SPD
        AP_SPD.once();
        break;
      case 84: //TEST
        AP_TEST.once();
        break;  
      case 86: //VVI
        AP_VVI.once();
        break;
      case 87: //WLW
        AP_WLV.once();
        break;
      case 89: //SYNC
        AP_SYNC.once();
        break;
      case 98: //BC
        AP_BC.once();
        break;
      case 99: //Com1  
        Audio_Com1_Monitor.once();
        break;
      case 103: //GPS  
        AP_GPS.once();
        break;
      case 110: //NAV1
        AP_NAV1.once();
        break;  
    }
//    R
    FlightSim.update();    
    while(key1 != NO_KEY){
      delay(50);
      key1 = keypad.getKey();
    }    
  }
}

byte ParseCommand(String Command){
  byte CommandValidation = 1;
  if(Command.startsWith("R")) { //Map zoom change
    if(Command.length() == 2){
      Radar_range = Command.substring(1).toInt();
    } else if(Command.length() == 5){
      Radio_XPNDR = Command.substring(1).toInt();
    } else {
      CommandValidation = 0;
    }
  } else {
    switch(Command.toInt()){
      case 6:
        Barometer = 29.92;
        break;
      case 8:
        Autopilot_terrain.once();
        break;  
      case 11:
        Radio_Nav1_Flip.once();
        break;
      case 12:
        Radio_Nav2_Flip.once();
        break;
      case 21:
        Audio_Com1_Flip.once();
        break;
      case 22:
        Audio_Com1_Flip.once();
        break;
      case 31:
        Radio_Adf1_Flip.once();
        break;
      case 32:
        Radio_Adf2_Flip.once();
        break;
      case 50 ... 999:
        Autopilot_DH = Command.substring(1).toInt();
        break;      
      case 4001 ... 4360:
        Autopilot_Heading = Command.substring(1).toInt();
        break;
      case 5000 ... 5600:
        Autopilot_Speed = Command.substring(1).toInt();
        break;
      case 7000 ... 7999:
        Autopilot_Altitude = ( Command.substring(1).toInt() * 100 );
        break;
      case 9001 ... 9099:
        Autopilot_VVI = -100 * Command.substring(2).toInt();
        break;
      case 9100 ... 9199:
        Autopilot_VVI = 100 * Command.substring(2).toInt();
        break;      
      case 31000 ... 31999:
        Radio_Adf1 = Command.substring(2).toInt();
        break;
      case 32000 ... 32999:
        Radio_Adf2 = Command.substring(2).toInt();
        break;
      case 62500 ... 63099:
        Barometer = float(Command.substring(1).toInt()) / 100;            
        break;
      case 1110000 ... 1113999:
        Radio_Nav1 = Command.substring(2).toInt();
        break;
      case 1210000 ... 1213999:  
        Command = Command.substring(2);
        Radio_Nav2 = Command.toInt();
        break;
      case 2110000 ... 2113999: 
        Audio_Com1 = Command.substring(2).toInt();
        break;
      case 2210000 ... 2213999: 
        Audio_Com2 = Command.substring(2).toInt();
        break;
      default:
        CommandValidation = 0;
    }
  }
  return CommandValidation;
}

