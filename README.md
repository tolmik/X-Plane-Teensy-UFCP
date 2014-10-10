X-Plane-Teensy-UFCP
===================

Arduino source code, PCB layouts and full documentation on a Teensy++ based Autopilot panel for fighters in X-Plane 10.30
The code is designed to be able to run on both X-Plane 9.70 and on X-Plane 10.30 simulators.
Unfortunately, the detarefs are not compatible in the two versions, so You will have to 
program the microcontroller for your specific version of the simulator in order to get the code working.

Current missing features:
-Character LCD support
-Command evaulation fro the numerical keypad inputs
-Weapon mode selector.
-VNAV support


Changelog

2014.10.10 Documentation update:
-Added a new User's Manual that details the planned Character LCD expansion, and all the currently implemented features.
-Renamed the current Android source code file to ufcp_ver9.ino to distinguish it from the planned X-Plane 10 version, 
    as the current code only works in X-Plane 9.70

2014.10.10 ver 0.3.1:
-Started work on the command strings, Nav and Com radios can now be manipulated with the panel.
-Added a Sprint Layout 5.0 Keypad PCB layout, and it's Gerber export.

2014.10.07. ver 0.3:
-Basic Buttons are working, but the Command string is not parsed, so you can not use the numerical keypad.
-Documentation is added. Documentation features detailed description for future updates, 
    such as the syntax for the commands entered via the numerical keypad.
