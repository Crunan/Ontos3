# ⚛ Ontos3 User Manual

####

#### RMonitor the Plasma head temperature from the _Temperature Dial_.

#### To stop the plasma, click on the \[_**TURN PLASMA OFF**_] button. <a href="#_ref53153085" id="_ref53153085"></a>



Figure 39: RUNNING Plasma

### Initializing the Stage – Operator Mode <a href="#_ref71274312" id="_ref71274312"></a>

#### On machine startup the stage must first go through an initialization. Click the \[_**INITIALIZE STAGE**_] button.

#### Once initialization is complete, several stage controls will become available to the operator.



Figure 40: Stage Controls

Note: _Start Scan_ is enabled only when plasma is running.

### LIFT PINS <a href="#_toc54339777" id="_toc54339777"></a>

The lift pins are used for loading and unloading a substrate onto the stage chuck.

#### The lift pins button is enabled by default after the stage is initialized.

#### Clicking the \[_**LIFT PINS**_] button while the button is enabled (green light is lit) will bring a loaded substrate onto the chuck.

#### Whenever the LIFT PINS is enabled, the chuck vacuum is automatically turned off, regardless of whether the manual vacuum button has been enabled.

### VACUUM <a href="#_toc54339778" id="_toc54339778"></a>

The chuck has a built-in vacuum which ensures the substrate from shifting during processing.

#### The Vacuum is turned off by default.

#### Clicking the \[_**VACUUM**_] button will turn the vacuum on for the chuck.

Note: _VACUUM_ will not turn on when the lift pins are enabled (green light is lit)

### LOAD POSITION <a href="#_toc54339779" id="_toc54339779"></a>

#### Click the \[_**LOAD POSITION**_] button to bring the stage into position for loading and unloading substrates.

#### Sliding the handle will open the windows as shown in **Figure 41**: Windows Open/Closed



**OPEN**

**CLOSED**

Figure 41: Windows Open/Closed

### START SCAN <a href="#_toc54339780" id="_toc54339780"></a>

#### The \[_**START SCAN**_] button is disabled by default.

#### Load a recipe per section 6.1.1 - Load a Recipe, Operator mode and ensure the recipe and stage values are correct for the recipe.

#### Start plasma per section 6.1.5 – Running Plasma

#### The \[START SCAN] button will now be enabled as seen in **Figure 42** below, click the \[START SCAN] button to begin the scan.



Figure 42: Start Scan Enabled

### HEATER – Preheat Plasma Head <a href="#_toc54339781" id="_toc54339781"></a>

The OntosTT is installed with a heater. When enabled, this will heat the plasma head for a more consistent plasma ignition.

#### To turn on/off the heater, click the _Settings_ button as shown in **Figure 37**.

#### Click the _Preheat Plasma Head_ option as seen in Figure 43.



Figure 43: Preheat Plasma Head

#### Click the _CONFIRM_ button.

#### The screen will now indicate that the heater is ON via the HEAT indicator lighting green as seen in **Figure 44**.

Note:_The Preheat Plasma Head option will preheat the plasma head to 35°C_

Figure 44: Heater On

#### Once the Preheat Plasma Head setting is set, an Engineer level of access can save the selection to a recipe per section 7.1.30 -Save a New Recipe – Engineer Mode and 7.1.33 - Overwrite an existing Recipe configuration – Engineer Mode.

### Acknowledge an Error <a href="#_toc84500387" id="_toc84500387"></a>

#### When an error occurs, the error codes section will display a code as shown in **Figure 45** below.



Figure 45: Error Codes

#### Plasma will automatically be disabled during an error code (except for DOORS OPEN)

#### For a list of error codes, see section 8 - **Troubleshooting**

#### Click the \[_**ACKNOWLEDGE**_] button to acknowledge the error.

Note: _The red light next to the_ \[_**START PLASMA**_] _button will be lit during an error code. This light will not change after acknowledgement of an error code. Either starting plasma again or resetting the system will clear this red light._

### Closing the OntosTT program <a href="#_toc54339782" id="_toc54339782"></a>

#### Ensure plasma has been turned off per step 6.1.22.

#### To close the application window, click _Recipe▼Exit_.



Figure 46: Exit Application

### Log File <a href="#_ref53416128" id="_ref53416128"></a>

The log file is a functionality of the OntosTT application whereby various tasks performed by the operator and status updates from the tool are logged within the system. Accessing these logs and details on what is tracked is outlined below.

#### The log file items are tracked via a text file located in the following directory: C:\OTT\_PLUS\Logs

#### The log files are time stamped as can be seen in the figure below.

Figure 47: Log File Time Stamps

#### The log file will track the following events.

| **Initiating Event for Log Entry**                   | **Logged Value**                                                                                         |
| ---------------------------------------------------- | -------------------------------------------------------------------------------------------------------- |
| Open a Recipe                                        | Recipe File name                                                                                         |
| Save                                                 | Recipe File name                                                                                         |
| Save As                                              | Recipe File name                                                                                         |
| Simulate Plasma On                                   | "Plasma Simulation Mode ON"                                                                              |
| Engineer Mode selected                               | "Engineer Mode enabled"                                                                                  |
| Operator Mode selected                               | "Operator Mode enabled"                                                                                  |
| Comms Error                                          | "Comms error: " + the last command sent                                                                  |
| Vac On/Off                                           | "Vac On" "Vac Off"                                                                                       |
| Laser Sensor Tripped                                 | “Laser Sensor Tripped”                                                                                   |
| Laser Sensor Activated                               | “Laser Sensor Active”                                                                                    |
| Laser Sensor deactivated                             | “Laser Sensor Deactivated”                                                                               |
| Initialize Stage                                     | "Initializing Axes"                                                                                      |
| Initialized Staged                                   | "Stage Initialized"                                                                                      |
| Preheat Plasma Head                                  | “Preheat Plasma Recipe ON / OFF”                                                                         |
| Auto Scan (Plasma Off feature)                       | "Plasma turned off (Auto-Off is active)"                                                                 |
| Batch ID Toggled On/Off                              | "Batch ID logging toggled ON / OFF"                                                                      |
| Batch ID button                                      | “Logging Batch ID #:”                                                                                    |
| Load Position + Move Speed                           | "Homing Start" + Speed of the axis                                                                       |
| Load Position Z                                      | "Homing Z"                                                                                               |
| Load Position Z Parked                               | "Homing Z Parked"                                                                                        |
| Load Position XY                                     | "Homing X & Y" + Speed and location of the X and Y axis                                                  |
| X & Y Loaded                                         | "X & Y Homed"                                                                                            |
| Z Loaded                                             | "Z Homed"                                                                                                |
| Load Stopped                                         | "Operator Abort Homing"                                                                                  |
| Joystick Started                                     | "TwoSpotSM Start Up"                                                                                     |
| Joystick First Spot                                  | "TwoSpotSM Got First"                                                                                    |
| Joystick Second Spot                                 | "TwoSpotSM Getting Second"                                                                               |
| Joystick Complete                                    | "TwoSpotSM Got Second - done"                                                                            |
| Scanning Started                                     | "-------------Scan Start-Up--------------"                                                               |
| State Machine Start Up                               | "Main State Machine Start Up"                                                                            |
| State Machine Shut Down                              | "Main State Machine Shut Down"                                                                           |
| Get MFCs                                             | "Number of MFCs: "                                                                                       |
| Get Axes Status                                      | "Bad Response = "" + Last Command from the 3-Axes PCB                                                    |
| Status Change                                        | If there is a change in the status of anything within the Control PCB then it is logged in the Log File. |
| Status Bits changed                                  | "Status Bits Change from " + (StatusBits)                                                                |
| Status Bits changed: No N2 purge detected            | "NO PURGE N2"                                                                                            |
| Status Bits changed: No connection or USB            | "NO HEARTBEAT"                                                                                           |
| Status Bits changed: Low Threshold for MFC1          | "MFC\_1 Low Flow"                                                                                        |
| Status Bits changed: Low Threshold for MFC2          | "MFC\_2 Low Flow"                                                                                        |
| Status Bits changed: Low Threshold MFC3              | "MFC\_3 Low Flow"                                                                                        |
| Status Bits changed: Low Threshold MFC4              | "MFC\_4 Low Flow"                                                                                        |
| Status Bits changed: No Flow for Helium              | "BAD HELIUM"                                                                                             |
| Status Bits changed: ESTOP ACTIVATED                 | "ESTOP ACTIVE"                                                                                           |
| Status Bits changed: MFC are no longer communicating | "MFC Comms Down"                                                                                         |
| Status Bits changed: Power from RF too low           | "Power Fwd Low"                                                                                          |
| Status Bits changed: Head Temperature too high       | "Head Too Hot"                                                                                           |

Table 2: Event Log Table

Note: For more details on how to resolve the error codes found in Table 2: Event Log Table see **section 8**.

#### The log file can be stored onto a USB or other network drive by simply using the Windows _Cut/Copy_ as shown below in **Figure 48**.

Figure 48: Log to USB or Network Cut/Copy

#### In the **Figure 49** below, the Log file is pasted onto the desktop, but any location such as USB or other desired network drive can be selected.

Figure 49: Log Paste Location

### Batch ID Logging - <a href="#_toc84500390" id="_toc84500390"></a>

#### Batch ID Logging is enabled by selection from the settings window as show in **Figure 50: Batch ID Logging**.



Figure 50: Batch ID Logging

#### Once enabled, a new button _Batch ID_ is available on the OntosTT dashboard as shown in **Figure 51**.

Figure 51: Batch ID button

#### Click the _Batch ID_ button to enter a batch/lot number.

_Note: The maximum characters allowed in the batch ID field is 45 characters._

#### Once the batch/lot number has been entered, click OK.



Figure 52: Enter a Batch ID

#### The Batch ID will be displayed in the Batch ID field.



Figure 53: Batch ID field display

#### The Batch ID that was entered will also be stored in the Log file as described in 6.1.55

#### **Batch ID Logging** -

The log files are stored in the directory - C:\OTT\_PLUS\Logs

Log files are organized by date.

Figure : Log File Directory

With a log file open in the notepad application, use of the _Search function_ (Ctl + F) allows for easy tracking of a given Batch ID. Enter the batch ID in the search box and all relevant data will be available for the given batch.

Figure 55: Log File - Batch ID #

### OntosTT Application – Engineer Mode <a href="#_ref82778588" id="_ref82778588"></a>

The OntosTT application contains all the information and controls needed to run and monitor the plasma head. Below is a description of the features displayed in the application window followed by the procedural steps for running the control software. The use of the following symbol: _▼_ indicates a button click followed by a dropdown selection. Always ensure a communication port is established per 0 before performing any of the other steps outlined in chapter 6.1.55.

Figure 56: OntosTT Engineer Interface Controls

1. Engineers can adjust the setpoint by clicking the associated _Enter_ button for a given MFC.
2. \[_**AUTO TUNE**_] button. Turns automatic tuning on/off.
3. The manual buttons for adjusting the Matchbox tuning. Click on the arrows to decrease and increase the Matchbox tuning % respectively.
4. USB Communication connection. The dropdown allows selection of an available port. Clicking the \[_**CONNECT**_] button will connect to the selected port. Use this to enable connection between PC and control rack.
5. Engineers have access to additional stage controls such as \[_**SET TWO SPOT**_] and \[_**SET DIAMETER**_].

### Connecting to a Communication Port <a href="#_toc84500392" id="_toc84500392"></a>

Figure 57: Com Port Hardware

#### The OntosTT software remembers previously connected comm ports for future use and automatically connects. If for any reason the connection is lost, follow the steps below. <a href="#_ref67315375" id="_ref67315375"></a>

#### In the OntosTT application window, click the dropdown menu next to _Com Port_. Select an open port from the list.



Figure 58: Com Port Selection

#### Click on the red \[_**CONNECT**_] button to connect.



Figure 59: COMMS button

### Auto Start Scan – Engineer Mode <a href="#_ref56074054" id="_ref56074054"></a>

#### The _Auto Start Scan_ option will automatically start the stage scan once the _Start Plasma_ button is clicked as well as turn plasma off at the end of the scan.

Note: _The AUTO START SCAN option can only be selected by an Engineer level access._



Figure 60: Auto Start Scan

Note: _Auto Start Scan_ _will begin once plasma is ignited, which is indicated by the Plasma Indicator lit violet_

#### The _Auto Start Scan_ option will also turn plasma off automatically at the end of a scan.

#### Once the _Auto Start Scan_ setting is set, an Engineer level of access can save the selection to a recipe per section 7.1.30 -Save a New Recipe – Engineer Mode and 7.1.33 - Overwrite an existing Recipe configuration – Engineer Mode..

### Crash Avoidance System – Engineer Mode <a href="#_ref76120128" id="_ref76120128"></a>

Note: _The Crash Avoidance_ _System option can only be selected by an Engineer level access._

#### The _Crash Avoidance_ _System_ option will make active the Crash Avoidance System sensors. The Crash Avoidance System is intended to protect both the Plasma Head and the Substrate from collision in the case where the Operator inputs have been incorrectly assessed.



Figure 61: Crash Avoidance System

#### With the Crash Avoidance System active, the system will attempt a Collision Test. The stage chuck will elevate to the max height in the Z-Axis, then it will begin to approach the plasma head. If the Stage Inputs for _Gap_ and _Thickness_ have been correctly set, the substrate on the chuck will pass under the plasma head. If there has been a miscalculation, the Laser on the Crash Avoidance System will detect an imminent collision and stop all stage movement.

#### When a crash has been prevented, the tool will indicate this by an error code which states “LASER TRIPPED” as well as the Indicator box for the Laser Sense will be lit Red as seen in **Figure** **62**.

Figure 62: Laser Tripped

#### Click the Acknowledge button in the Error Code section to return the stage to the Load position.

#### Correctly determine the substrate gap and thickness and enter the new values.

#### Repeat the Collision test to ensure the entered values will allow the substrate to pass underneath the plasma head.

Note: The Laser Sense indicator box will light up green, () to indicate the Crash Avoidance System is active.

### Running Plasma – Engineer Mode <a href="#_toc84500395" id="_toc84500395"></a>

To run the plasma, the following parameters are required:

*
  * GAS FLOW: Gas flow is programmed in Standard Liter(s) per Minute (SLPM). The gas #1 is the carrier gas. Carrier gas is Helium and it is mandatory to gas #1 to a value equal or higher than 10 SLPM whatever chemistry is used for a given process. As a consequence, Helium/Hydrogen, Nitrogen, or Oxygen cannot be used alone (without Gas #1).
  * RF POWER: The Radio-Frequency Power is set in Watts.
  * MB TUNER: The Plasma Head impedance varies with the gas mixture; therefore, an impedance matching network is required to minimize the reflected power. The Matchbox Tuner (MB Tuner) can accomplish this and can be preset by the engineer to minimize the plasma activation time.

#### Click the \[_**START PLASMA**_] button to turn on plasma.

Note: _Engineer mode does not restrict the_ \[_**START PLASMA**_] _button. This means you can run plasma without loading a recipe as well as having no recipe setpoints._

Figure 63: Plasma Stopped

#### If AUTO TUNE is enabled, the Matchbox will begin to tune until the RF Power Reflected is at minimum. This can be observed in the _RF Reflected dial_.

#### Once the MB tuner has adjusted, Plasma ignition will start as the RF Power Reflected approaches zero watts.



Figure 64: RUNNING Plasma

Note: The _Plasma On_ indicator light will turn violet when plasma is activated.

#### Monitor the Plasma head temperature from the _Temperature Dial_.

#### To stop the plasma, click on the \[_**TURN PLASMA OFF**_] button.

### Running Scanning Cycle – Engineer Mode <a href="#_toc84500396" id="_toc84500396"></a>

To run a scanning cycle, the following parameters are required:

*
  * THICKNESS: The component thickness, together with the process gap described hereafter are used by the system to calculate the vertical position of the substrate chuck in order to operate safely the X/Y scanning movement (Figure 65).
  * GAP: The process gap is the space left between the plasma head and the component during the X/Y scanning movement. It varies with gas chemistry. It should not be too large in order to ensure a proper local confinement enabling the gas flow to keep the ambient air to penetrate the process zone.

**Plasma Head**

**Substrate Chuck**

**THICKNESS**

**GAP**

Figure 65: Thickness and Gap Setting

*
  * OVERLAP: The scanning overlap is a parameter that modifies the aperture width parameter when calculating the indexing of the stage between neighboring scans (whenever the substrate is too wide to be covered in one scan). The effective aperture width, that is, the zone of plasma effectiveness on the substrate, varies with plasma parameters.

(Plasma head aperture)

**PH**

**PH + 2**

(Plasma head aperture)

**PH**

**OVL**

(Plasma head aperture)

**PH**

**PH - 2**

(Plasma head aperture)

**PH**

**OVL**

Figure 66: Scanning Overlap Setting

(Example of positive (+2mm) and negative (-2mm) overlap)

As shown in Figure 66: Scanning Overlap Setting, sometimes the effective width is larger than the actual aperture (typical of surface activation processes and organic removal processes); and sometimes the effective width is smaller than the actual aperture (typical of reducing-chemistry processes).  The overlap parameter is a correction factor that is applied in the computer calculations of how far the stage shifts in X before starting a neighboring scan.

The overlap parameter can be programmed to either a positive or negative number, depending on whether it is suitable for the neighboring scans to “overlap” or “underlap”.  For example:

*
  * If reducing chemistry is used to remove oxide, the effective aperture width is smaller than the actual aperture width, so you would want the neighboring scans to be closer together (overlapped) to be sure that the whole area is uniformly treated.
  * Conversely, a negative overlap spreads out the neighboring scans.

The actual “overlap” number to use is determined by running tests to optimize lateral uniformity of the substrate in the X dimension after Ontos treatment, usually with an ellipsometer.

*
  * SPEED: The scanning speed, programmed in millimeter(s) per second varies with the process requirements. For instance, if a thick oxide needs to be reduced, the speed will be low while it can be significantly increased if only a thin layer of organics is to be eliminated.
  * CYCLES: For some applications, it can be beneficial to execute the treatment with a repeated less aggressive chemistry; in that case, it is possible to perform several scan cycles within a same recipe. A first scanning sequence is performed and if the number of cycles is greater than one, then at the end of a cycle, the scanning is performed again from the beginning of the next cycle.

Below is the illustration of a scanning cycle requiring scanning two lanes in order to cover the wafer surface with the installed plasma head aperture and with a positive overlap.

The chuck is displaced automatically below the plasma head to scanning cycle starting point \[a], next the chuck moves in Y to scan line lane #1 \[b], then the chuck moves along the X axis to change from lane #1 to lane #2 and along the Y-axis to position the component below the plasma head at the beginning on lane #2 \[c], finally the chuck moves along Y axis again to scan lane #2 \[d]. If a second cycle is requested, then the chuck return to step \[a].

| <p>Chuck X/Y move to</p><p>scanning-cycle start point</p>                                                                               | Chuck Y-Scan Lane #1                                                                                                                    | <p>Chuck X-Move to lane #2</p><p>Then chuck Y-move</p><p>to lane #2 start point</p>                                                     | Chuck Y-Scan Lane #2                                                                                                                    |
| --------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------- |
| <ul><li><p></p><ul><li><p></p><ul><li><p></p><ul><li><p></p><ol><li>Beginning lane #1</li></ol></li></ul></li></ul></li></ul></li></ul> | <ul><li><p></p><ul><li><p></p><ul><li><p></p><ul><li><p></p><ol><li>Finishing lane #1</li></ol></li></ul></li></ul></li></ul></li></ul> | <ul><li><p></p><ul><li><p></p><ul><li><p></p><ul><li><p></p><ol><li>Beginning lane #2</li></ol></li></ul></li></ul></li></ul></li></ul> | <ul><li><p></p><ul><li><p></p><ul><li><p></p><ul><li><p></p><ol><li>Finishing lane #2</li></ol></li></ul></li></ul></li></ul></li></ul> |

#### SET TWO-SPOT <a href="#_ref82778140" id="_ref82778140"></a>

This sequence sets the coordinates of the Scanning function for a rectangular substrate. It allows to define a scanning area for the Plasma that is not necessarily the entire substrate surface, but can be a localized area, eventually off center (see Figure 67 and Figure 68 below for examples)

| <p><strong>1st point</strong></p><p><strong>2nd point</strong></p> | <p><strong>1st point</strong></p><p><strong>2nd point</strong></p> |
| ------------------------------------------------------------------ | ------------------------------------------------------------------ |
| Figure 67: Entire substrate centered                               | Figure 68: Partial Scan (off-center)                               |

**Ensure the stage has been initialized.**

**Connect the Joystick**

**Plug the supplied cable into the Joystick and the other end of the HDMI cable into the OntosTT back panel.**

**Click the \[**_**SET TWO-SPOT**_**] on the screen.**

**The Joystick LED will turn on (Figure 71). The laser attached to the plasma head will also turn on (Figure 72).**

|                           |                                      | 7782a1364db945bb98b62bd623c73d36 |
| ------------------------- | ------------------------------------ | -------------------------------- |
| Figure 69: Joystick Cable | Figure 70: Back panel Joystick Cable | Figure 71: Joystick LED          |



Figure 72: TWO SPOT Laser

**WARNING: The laser (Class 2, < 1mW) can reflect off surfaces on the OntosTT, do not stare directly into beam of the laser source.**



**First Point Setting**

#### The stage status bar indicates the current and the next step status. Using the joystick, navigate the stage to the first point (**Error! Reference source not found.**, below). The first point is one of the extreme edges of the substate (usually one end of a diagonal)



Figure 73: Joystick Status Bars

When the stage is in the desired location, hold the Joystick button to activate the memorization of the first point.



Figure 74: Joystick button location

Once the next step status reads “Release Joystick Button”, release the button.



Figure 75: Release Joystick Button

**Second point setting**

The next step in the Status Information section will read “Spot Second Point”.

Figure 76: Spot Second Point

Navigate the stage to the second point using the Joystick.

When the stage is in the desired location, hold the Joystick button until the joystick status bar reads “Release Joystick Button”, indicating that the position has been memorized.

The location of the selected points will be displayed in the Joystick section shown in **Figure 77**.



Figure 77: Joystick Locations

#### SET DIAMETER – Engineer Mode

**Ensure the stage has been initialized.**

**Click the \[**_**SET DIAMETER**_**] button.**

**Select one of the size options in the popup window**



Figure 78: Set Diameter

#### Click the \[_**START SCAN**_] button to activate the scanning cycle.

Note: _Engineer mode does not restrict the_ \[_**START SCAN**_] _button. This means you can run the Scanning cycle without loading a recipe as well as having no recipe setpoints._

Figure 79: Stage Stopped

### Manual Recipe Control – Engineer Mode <a href="#_ref56262227" id="_ref56262227"></a>

#### At any point, including while running plasma, an Engineer may want to adjust the _Recipe_ value. To do this, click on an \[_**ENTER**_] button to adjust a setpoint.



Figure 80: Setpoint _ENTER_ Button

#### A pop-up window will appear, enter a new value for the given gas, RF Power or MB tuner value for the selected parameter of the active recipe.

Note: The format of the value entered must comply with the format shown in the corresponding window. (see **Figure** **81**.)



Figure 81: Recipe Value Entry Box

#### Ensure the value entered is within range of the format shown in the window and click _OK_. (X or Y values can be a value of 0)

### Save a New Recipe – Engineer Mode <a href="#_ref71287225" id="_ref71287225"></a>

#### To save the current recipe settings, click _Recipe_▼_Save As_.



Figure 82: Save a New Recipe

#### The _Enter Recipe Name_ window will appear, enter a name for the Recipe file and click _OK_.

Figure 83: Enter Recipe Name Box

### Overwrite an existing Recipe configuration – Engineer Mode <a href="#_ref71287243" id="_ref71287243"></a>

#### To overwrite an existing Recipe, the recipe must first be loaded per step **6.1.1 - Load a Recipe.**

Note: Ensure that whichever recipe file configuration is loaded is intended to be overwritten.

#### Once all manual entry for a recipe has been performed per step **7.1.26 - Manual Recipe Control**, the recipe will be ready to overwrite the existing recipe configuration.

#### Click Recipe ▼ Save.



Figure 84: Overwrite Configuration

#### The new recipe configuration will now be saved over the pre-existing configuration save file.

### Build Cascade Recipe – Engineer Mode <a href="#_hlk82681534" id="_hlk82681534"></a>

Cascade Recipes are a way to have multiple recipes run automatically in sequence. The recipes are added using the Cascade Menu. Once the cascade recipe is created and opened, the recipes will behave as normal during the run. Once the recipe has completed, the next recipe will automatically begin.

#### Click on Build Cascade Recipe.

Figure 85: Build Cascade Recipe

#### The Cascade Recipe List menu will display. There are four buttons associated with building a cascade recipe.

Figure 86: Cascade Recipe Menu

#### Click _ADD_

#### The “Add a Recipe to the Cascade Recipe” menu will appear, select a recipe to add to the Cascade Recipe



Figure 87: Adding a Recipe to Cascade Recipe

#### Click _OPEN_.

#### The Cascade Recipe List will now include the added recipe (Figure 88).

#### To remove a recipe, select the recipe in the list and click _REMOVE_ (Figure 89).

|                                                |                                                   |
| ---------------------------------------------- | ------------------------------------------------- |
| Figure 88: Added Recipe to Cascade Recipe List | Figure 89: Remove Recipe from Cascade Recipe List |

#### When ready to save the cascade recipe, click _SAVE_ (Figure 90)

#### When finished with managing the cascade recipe, click _OK_ to close the menu.

|                                         |                                   |
| --------------------------------------- | --------------------------------- |
| Figure 90: Save the Cascade Recipe List | Figure 91: OK Cascade Recipe List |

### N2 PURGE – Engineer Mode <a href="#_toc84500401" id="_toc84500401"></a>

#### The plasma head is equipped with a nitrogen purge consisting of a nitrogen curtain, parallel to the downstream plasma aperture and distant from it of 10mm. It is used to cool down the substrate before it becomes exposed to the atmosphere.

#### By default, the N2 Purge button is off. Click the \[_**N2 PURGE**_] button to turn on N2 substrate purge during scanning of a recipe. The green light we be lit on the button.

#### During scanning, N2 purge will be turned on and off automatically when N2 Purge is enabled.

#### The N2 Purge value (enabled/disabled) will be saved to a recipe when using Save or Save As.

Note: The _PURGE ON_ indicator will light cyan when nitrogen is flowing during a scan.

Figure 92: N2 Purge

### Servicing – Maintenance <a href="#_toc84500402" id="_toc84500402"></a>

Maintenance that goes beyond the steps listed below require the Maintenance Training Course. The Maintenance Training Course is a package provide by Ontos Equipment Systems to train personnel on how to properly service and perform preventative maintenance of the OntosTT system

### Wipe down critical surfaces: <a href="#_toc79478588" id="_toc79478588"></a>

Note: During the wipe down procedure, avoid contact with guiding rails surfaces. Lubrication from these surfaces should not be removed.

#### Use clean dry air (CDA) or N2 to gently blow off the chuck surface. <a href="#_ref76549992" id="_ref76549992"></a>

#### Using a clean room wipe (e.g., Texwipe™ TX 1008B or similar), dampen the wipe with Isopropyl Alcohol (not soaked or dripping). <a href="#_ref76543553" id="_ref76543553"></a>

Figure 93: Clean room wipes

#### Wipe down all interior surfaces of the enclosure, repeat step 8.1.3 for each step below:

**Inside the access door and the sill at the bottom of the door opening.**

**The polished mirror bottom panel underneath the stage.**

Note: With the OntosTT powered off, the stage motors are de-energized and the stage can be freely moved around for cleaning.

**The top surface of the Stage where the sample/substrate is placed.**

**The sliding doors at the front of the tool.**

**The touchscreen monitor (if applicable).**

**Repeat step 8.1.2.**

### Changing a Gas Bottle <a href="#_toc84500404" id="_toc84500404"></a>

### Turn off the valve located near the machine. <a href="#_ref79581630" id="_ref79581630"></a>

#### On the bottle to be changed out, ensure the gas valve is closed.

#### Using a adjustable wrench, remove the regulator assembly.

#### Remove the empty tank and replace with new bottle.

#### Using the adjustable wrench, attach the regulator assembly until secure.

#### Open the tank valve.

#### Check for leaks at the regulator-to-bottle connection.

#### Ensure the bottle pressure is set correctly.

#### Open the valve to the machine from step 8.1.6.

#### Proceed to step 8.1.16 Purge Gas Lines.

### Purge Gas Lines <a href="#_ref79582241" id="_ref79582241"></a>

#### From the OntosTT software perform the following steps:

#### Select _Engineer_ mode from the _Service_ menu bar.

#### Select _Auto Tune_ to disable _Auto Tune_.

#### Set RF power to 0.

#### Under _Settings_, ensure _Auto Scan_ is set to OFF.

#### Purge the gas line associated with the replaced bottle per the following steps:

Note: Make sure gas #1 does not drop below 10 SLPM.

**To perform this, first ensure the gas regulator for the replaced gas bottle is set to 20 PSI. (1.4 bar)**

**On the OntosTT software, set the replaced gas to the maximum allowed value.**

Note: For gas #1 & #2 max is 20 SLPM. For gas #3 & #4 max is 0.3 SLPM.

**Select the **_**Start Plasma**_** button**_**.**_

**Wait at least 1 second per meter of line length of the delivery gas line**_**.**_

### Troubleshooting <a href="#_toc84500407" id="_toc84500407"></a>

While running, if any process error should occur, the _Error Codes_ section of the interface will contain an error code. Also, during an error, the red light next to the \[_**START PLASMA**_] button will be lit. This light will not change after acknowledgement of an error code_._ Either starting plasma again or resetting the system will clear this red light. See **Table 3** for a list of all error codes with descriptions for the error.

| **Error Message** | **Description**                                                                                                              |
| ----------------- | ---------------------------------------------------------------------------------------------------------------------------- |
| No PURGE N2       | Gas flow from Valve 5 - Nitrogen is not detected.                                                                            |
| No Heartbeat      | The communication from the controller to the PC has been interrupted.                                                        |
| MFC\_1 Low Flow   | Gas flow from MFC #1 is not detected, or the combined Gas flow of MFC 1 & MFC 2 does need meet the minimum flow requirement. |
| MFC\_2 Low Flow   | Gas flow from MFC #2 is not detected.                                                                                        |
| MFC\_3 Low Flow   | Gas flow from MFC #3 is not detected.                                                                                        |
| MFC\_4 Low Flow   | Gas flow from MFC #4 is not detected.                                                                                        |
| BAD HELIUM        | Helium flow has gone below 5 SLPM.                                                                                           |
| ESTOP ACTIVE      | The Emergency Stop has been activated. The System must be restarted.                                                         |
| MFC Comms Down    | The communications from the MFC to the controller has been interrupted.                                                      |
| Power Fwd Low     | RF power is running to low.                                                                                                  |
| Head Too Hot      | Plasma head temperature over 60c, snap switch has activated ESTOP                                                            |
| Laser Tripped     | Collisions Sensor System was active and the system stopped the stage from colliding with the Plasma Head.                    |

Table 3: Error Codes & Descriptions

### Plasma issues <a href="#_toc84500408" id="_toc84500408"></a>

Note: In EVERY recipe, MFC#1 must have some flow setting to open the manifold gate valve. A setting of 0.00 for MFC#1 will cause the manifold to remain closed and gas pressure from the other flows will only build up behind the valve, eventually leading to a non-flow condition and an error message

**Issue #1: If plasma fails to activate, (The **_**Plasma On**_** indicator light is not violet) a pop-up window will display information regarding what may have gone wrong. Common issues are the following: the gas supply lines are not turned on, the flow of MFC#1 (Helium) is set to 0.00, RF Power is set to 0.00, No active recipe has been loaded.**

**Manually enter 10 SLPM for Helium and 80 watts for RF Power. Run plasma per section 07.1.5 -Run Plasma – Engineer mode.**

**If this does not activate the plasma, call or email service at** [**service@set-na.com**](mailto:service@set-na.com)**.**

**Issue #2: If plasma is RUNNING, but the current recipe does not light the plasma (The **_**Plasma On**_** indicator light is not violet) then it could be that the gas values are outside the operating range of the plasma head.**

**Manually enter 10 SLPM for Helium per section 7 - Engineer mode.**

**Manually enter 0.1 SLPM for Nitrogen per section per 7 - Engineer mode (Maximum depends on Helium flow but is generally under 0.25 SLPM**

**Only if the system is equipped with the Ozone option, manually enter 0.1 SLPM for Oxygen per section 7 - Engineer mode (maximum depends on Helium flow but is generally under 0.25 SLPM.)**

**Do not run oxygen and hydrogen together in a single process as this can cause an explosion and may damage the plasma head. Do not run oxygen unless the Optional O2 gas kit has been installed (prevents oxygen and hydrogen from running at same time).**



**Run plasma per section 07.1.5 -Run Plasma – Engineer mode**

**If this does not activate the plasma, call or email service at** [**service@set-na.com**](mailto:service@set-na.com)**.**

**Issue #3: If plasma is RUNNNING, but then goes out during a cycle, an error message will appear (most typically, the gas supply has been interrupted by an empty gas cylinder). check the following:**

**Gas flow compliance may be out of spec for too long. This can be caused by:**

**Empty gas cylinder.**

**Noise spikes in the environment**

**Compliance limit set too tight for the gas flow setting (i.e., if trying to control flow to less than 10% of the max range of the MFC). Call or email service before changing compliance settings.**

**Check the temperature of the source, which is displayed in process window. If it is near 60C, the system may be shutting down to prevent plasma head overheating. Check that cooling water is circulating normally.**

**A system interlock may be opening.**

**check all safety systems and then repeat section 7.1.5 - Running Plasma – Engineer Mode**

**If this does not resolve the issue, call or email service at** [**service@set-na.com**](mailto:service@set-na.com)**.**

### Gas Flow Issues <a href="#_toc54339793" id="_toc54339793"></a>

**Issue #1: If gas flow is near zero when it is supposed to be flowing, the gas supply is probably interrupted due to:**

**Gas valves not turned on.**

**Gas cylinder empty.**

**No flow setting in MFC#1. (There must be at least 5 SLPM of flow for MFC#1 to activate the manifold outlet valve.**

**If this does not resolve the issue, call or email service at** [**service@set-na.com**](mailto:service@set-na.com)**.**

**Issue #2: Safety interlock issues**

Note: Gas flow will automatically be shut down if safety interlocks are open.

**Ensure all safety interlocks are closed.**

**If this does not resolve the issue, call or email service at** [**service@set-na.com**](mailto:service@set-na.com)**.**

**Issue #3: If gas flow still does not control properly**

**An MFC may could faulty.**

**There is a problem with the MFC controller**

**A fuse may be blown in the Main Control Module**

**For any of the above issues, call or email service at** [**service@set-na.com**](mailto:service@set-na.com)**.**

### Fuse Replacement <a href="#_toc84500410" id="_toc84500410"></a>



**Fuse replacement should be accomplished by OES-trained maintenance personnel. There is always a reason why fuses blow. Replacing a fuse with an incorrect value or type may cause serious damage to the system. Call or email service at** [service@set-na.com](mailto:service@set-na.com)

### Cooling System Issues <a href="#_toc54339795" id="_toc54339795"></a>

#### Plasma head temperature should normally stay below 45°C, even when running continuously. If you notice the head temperature going above this, or if you get an error message due to reaching the maximum temperature (60°C), a cooling system issue is indicated.

#### Please call or email service at [service@set-na.com](mailto:service@set-na.com).

Note: You can continue to run the system intermittently, watching closely that the head temperature does not exceed 40°C; however, the cooling system should be checked and repaired as soon as possible. Please call Maintenance.

### Safety Interlock System <a href="#_toc54339796" id="_toc54339796"></a>

Refer to block diagram, Figure 94.

Figure 94: Safety Interlock Diagram

#### Base System (without the optional Oxygen Kit) <a href="#_toc54339797" id="_toc54339797"></a>

For base systems without the optional Oxygen kit there is “one” interlock which must be satisfied for the process to run. The red EPO button must be released per step 2.1.5.

Note: The EPO button occasionally gets accidentally bumped and disconnects and it may not be apparent that it has been activated.

Refer to the diagram in Figure 95 to see the Standard Interlock Configuration.

Figure 95: Standard Interlock Configuration

#### System with the optional Oxygen Kit <a href="#_toc54339798" id="_toc54339798"></a>

For systems with the optional Oxygen kit: there are “two” interlocks which must be satisfied for process to run.

The red EMO (Emergency-Stop) button must be released per step 2.1.5.

Note: The EMO button occasionally gets accidentally bumped and disconnects and it may not be apparent that it has been activated.

The Ozone sensor display must be green or yellow at the bottom of the scale, this is the safe zone.

If excessive Ozone is being sensed in the vicinity of the OntosTT, the display will be red, this is unsafe and a ESTOP will be triggered. This may be an indication of insufficient exhaust. Ozone must be cleared from the area until the Ozone scale drops back into the yellow or green causing the green LED on the front panel to come on again.

### Attachment 1 - Heat Transfer Fluid – MSDS (Dynalene EG) <a href="#_toc84500413" id="_toc84500413"></a>

### Product and Company Identification <a href="#_toc54339800" id="_toc54339800"></a>

**Product identifiers**

Product Name: Dynalene EG (includes all concentrations/dyes)

Producer: Dynalene, Inc.

Product Number: Not available.

CAS-No.: Not available.

**Identified uses of the product and uses advised against**

Identified Uses: Heat transfer fluid.

**Details of the chemical supplier**

Company: Dynalene, Inc.

5250 West Coplay Road, Whitehall, PA 18052, USA

Telephone: +1 610-262-9686

Fax: +1 610-262-7437

**Emergency telephone number**

Within the U.S.: +1 800-424-9300 (CHEMTREC)

Outside the U.S.: +1 703-527-3887 (CHEMTREC)

### Hazards Identification <a href="#_toc84500415" id="_toc84500415"></a>

**Classification of the substance or mixture**

GHS Classification in accordance with 29 CFR 1910 (OSHA HCS)

Acute toxicity, Oral (Category 4), H302

**GHS Label elements, including precautionary statements**

Pictogram

Signal word: Warning

Hazard statement(s)

H302 Harmful if swallowed.

Precautionary statement(s)

P264 Wash skin thoroughly after handling.

P270 Do not eat, drink, or smoke when using this product.

P301+P312 IF SWALLOWED: Call a POISON CENTER or doctor/physician if you feel unwell.

P330 Rinse mouth.

P501 Dispose of contents/container to an approved waste disposal plant.

**Hazards not otherwise classified (HNOC) or not covered by GHS**

None.

### Composition/Information on Ingredients <a href="#_toc54339802" id="_toc54339802"></a>

**Product mixture**

Synonyms: Mixture.

Molecular Wt: Not available.

CAS-No.: Not available.

| Ingredients                       | Classification     | CAS-No.  | Concentration |
| --------------------------------- | ------------------ | -------- | ------------- |
| Ethylene glycol                   | Acute Tox. 4; H302 | 107-21-1 | 15-100%       |
| Inhibitor solution (trade secret) | Not hazardous.     | n/a      | <12%          |

### First Aid Measures <a href="#_toc54339803" id="_toc54339803"></a>

**Description of first aid measures**

**Skin exposure**

Wash off with soap and water. Consult a physician.

**Eye exposure**

Rinse thoroughly with plenty of water for at least 15 minutes and consult a physician.

**Inhalation**

If breathed in, move person into fresh air. If not breathing, give artificial respiration. Consult a physician.

**Ingestion**

Never give anything by mouth to an unconscious person. Rinse mouth with water and consult a physician.

**Most important symptoms and effects, both acute and delayed**

The most important known symptoms and effects are described in the labelling (see section 10.1.2) and in section 10.1.11.

**Indication of any immediate medical attention and special treatment needed**

No data available.

### Fire Fighting Measures <a href="#_toc54339804" id="_toc54339804"></a>

**Suitable (and unsuitable) extinguishing media**

Suitable: Water spray, carbon dioxide, foam, dry chemical, any ABC class.

**Specific hazards arising from the chemical**

When involved in a fire, this material may decompose and produce irritating vapors and toxic gases (e.g., carbon oxides).

**Advice for firefighters**

Incipient fire responders should wear eye protection. Structural fire fighters must wear Self-Contained Breathing Apparatus and full protective equipment. Move fire-exposed containers if it can be done without risk to firefighters. If possible, prevent run-off water from entering storm drains, bodies of water, or other environmental areas.

### Accidental Release Measures <a href="#_toc54339805" id="_toc54339805"></a>

**Personal precautions, protective equipment, and emergency procedures**

Proper protective equipment should be used. In case of an uncontrolled release, clear the affected area, protect people, and respond with trained personnel. Avoid breathing vapors. Ensure adequate ventilation.

**Environmental precautions**

Do not let product enter drains.

**Methods and materials for containment and cleaning up**

Small spill: Cover with absorbent material (floor absorbent, vermiculite, etc.). Soak up spill and place material into a drum.

Large spill: Wear protective equipment. Stop spill at source, dike the area surrounding the spill to prevent further exposure. Prevent material from entering sewer system. If necessary, absorbents such as vermiculite, clay floor absorbent may be used on spill and shoveled into drums.

**References to other sections**

For disposal see section 10.1.13.

### Handling and Storage <a href="#_toc54339806" id="_toc54339806"></a>

**General hygiene considerations**

Handle in accordance with good industrial hygiene and safety practice. Wash hands before breaks and at the end of the day.

**Precautions for safe handling**

Use in a well-ventilated location. Open drums and other containers of this product slowly, on a stable surface. Drums and other containers of this product should be properly labeled. Keep containers tightly closed.

**Conditions for safe storage, including any incompatibilities**

Move drums of this product carefully, with the appropriate drum-handling equipment. Store drums and other containers in cool, dry locations, away from direct sunlight, or sources of intense heat. Storage areas should be made of fire-resistant materials. Keep containers away from incompatible chemicals.

### Exposure Controls/Personal Protection <a href="#_toc54339807" id="_toc54339807"></a>

**Control and exposure limits recommended by the chemical manufacturer**

USA OSHA, Table Z-1, Limits for Air Contaminants – 1910.1000: 50 ppm, 125 mg/m3 (ethylene glycol, C value)

USA ACGIH, Threshold Limit Values (TLV): 100 mg/m3 (ethylene glycol, C value)

Eye and upper respiratory tract irritation, not classifiable as a human carcinogen.

**Appropriate engineering controls**

Use with adequate ventilation to minimize exposure to mists or sprays of this product. Prudent practice is to ensure eyewash/safety shower stations are available near areas where this product is used. Monitoring of oxygen level is recommended.

**Individual protection measures, such as personal protective equipment**

All personnel handling the product should use a personal protective equipment level D.

**Respiratory protection**

None needed for normal circumstances of use. If respiratory protection is needed, use only protection authorized in 29 CFR 1910.134, or applicable State regulations. Use supplied air respiration protection if oxygen levels are below 19.5% or are unknown.

**Eye protection**

Wear safety glasses with side shields.

**Hand protection**

Wear butyl rubber, natural rubber, neoprene, Nitrile rubber, or other suitable gloves for routine industrial use.

**Body protection**

Wear impervious clothing.

### Physical and Chemical Properties <a href="#_toc54339808" id="_toc54339808"></a>

**Information on basic physical and chemical properties**

1. Appearance Clear, liquid.
2. Odor Odorless.
3. Odor threshold No data available.
4. pH 7.0 - 11.0
5. Melting/freezing point -16.7°C (2°F), for >99% concentration.
6. Boiling point >100°C (>212°F)
7. Flash point None for concentrations <80%.
8. Evaporation rate 1
9. Flammability (solid, gas) No data available.
10. Upper/lower flammability or explosive limits Upper (UEL): 15.3% (V)

Lower (LEL): 3.2% (V)

1. Vapor pressure 0.08 mmHg at 25°C (77°F)
2. Vapor density 2.14 (Air = 1.0)
3. Relative density 1.0 – 1.2 g/cm3 at 25°C (77°F)
4. Water solubility Soluble.
5. Partition coefficient: n- octanol/water logP = -1.36
6. Auto-ignition temp No data available.
7. Decomposition temp No data available.
8. Viscosity >1.0 cP at 25°C (77°F)

### Stability and Reactivity <a href="#_toc54339809" id="_toc54339809"></a>

**Reactivity**

No data available.

**Chemical stability**

Stable under ordinary conditions of use and storage.

**Possibility of hazardous reactions**

Stable under ordinary conditions of use and storage.

**Conditions to avoid**

Contact with incompatible chemicals and exposure to extremely high temperatures.

**Incompatible materials**

Strong oxidizers, strong acids, acid chlorides, acid anhydrides, chloroformates, or strong reducing agents.

**Hazardous decomposition products**

Mainly carbon dioxide and carbon monoxide.

### Toxicological Information <a href="#_toc84500424" id="_toc84500424"></a>

**Information on toxicological effects**

For ethylene glycol

LD50 Oral – rat: 4,700 mg/kg

LD50 Dermal – rabbit: 10,626 mg/kg

**Skin corrosion/irritation**

No data available.

**Serious eye damage/eye irritation**

Eyes – rabbit. Result: mild eye irritation, 24h.

**Irritancy of product**

This product may cause irritation to contaminated tissues.

**Reproductive toxicity**

This product is not reported to produce mutagenic, embryotoxic, teratogenic, or reproductive effects in humans.

**Suspected cancer agent**

No component of this product present at levels greater than or equal to 0.1% is identified as a carcinogen or potential carcinogen by ACGIH, NTP, or OSHA.

**Medical conditions aggravated by exposure**

It is anticipated that mainly skin, eye, and respiratory disorders may be aggravated after over-exposure.

**Additional information**

RTECS: KW2975000

When ingested early symptoms mimic alcohol inebriation and are followed by nausea, vomiting, abdominal pain, weakness, muscle tenderness, respiratory failure, convulsions, cardiovascular collapse, pulmonary edema, hypocalcemic tetany, and severe metabolic acidosis. Without treatment, death may occur in 8 to 24 hours. Victims who survive the initial toxicity period usually develop renal failure along with brain and liver damage., Exposure to and/or consumption of alcohol may increase toxic effects.

Central nervous system - Irregularities - Based on Human Evidence

### Ecological Information <a href="#_toc54339811" id="_toc54339811"></a>

**Ecotoxicity (aquatic and terrestrial)**

This product may be harmful to aquatic life if large quantities are released into bodies of water.

Ethylene glycol

Toxicity to fish: NOEC – Pimephales promelas (fathead minnow) – 39,140 mg/L, 96h.

NOEC – Pimephales promelas (fathead minnow) – 32,000 mg/L, 7d.

LC50 – Oncorhynchus mykiss (rainbow trout) – 18,500 mg/L, 96h.

LC50 – Leuciscus idus (golden orfe) - >10,000 mg/L, 48h.

Toxicity to invertebrates: NOEC – Daphnia – 24,000 mg/L, 48h.

EC50 – Daphnia magna (water flea) – 74,000 mg/L, 24h.

LC50 – Daphnia magna (water flea) – 41,000 mg/L, 48h.

**Persistence and degradability**

Ratio BOD/ThBOD: 0.78%

**Bioaccumulation potential**

Bioaccumulation, other fish – 61d, 50 mg/L

Bioconcentration factor (BCF): 0.60

**Mobility in soil**

No data available.

**Other adverse effects**

None.

### Disposal Considerations <a href="#_toc84500426" id="_toc84500426"></a>

**Waste treatment methods**

Waste disposal must be in accordance with appropriate Federal, State, and local regulations. This product, if unaltered by use, may be disposed of by treatment at a permitted facility or as advised by your local hazardous waste regulatory authority.

### Transport Information <a href="#_toc54339813" id="_toc54339813"></a>

**IMDG :** Not dangerous goods.

**IMDG :** Not dangerous goods.

**IATA :** Not dangerous goods.

#### Regulatory Information

**SARA Reporting Requirements**

SARA 302 Components: No chemicals in this material are subject to the reporting requirements of SARA Title III, Section 302.

SARA 313 Components: Ethylene glycol, CAS-No.: 107-21-1; Revision date: 2007-07-01.

SARA 311/312 Hazards: Acute health hazard, chronic health hazard.

### Other Information <a href="#_toc54339814" id="_toc54339814"></a>

**Revision Date**

August 19th, 2014

This SDS was prepared by Dynalene, Inc.

The information contained herein is based on data considered accurate. However, no warranty is expressed or implied regarding the accuracy of these data or the results to be obtained from the use thereof. Dynalene Heat Transfer Fluids assumes no responsibility for injury to the vendee or third persons proximately caused by the material if reasonable safety procedures are not adhered to as stipulated in the data sheet. Additionally, Dynalene Heat Transfer Fluids assumes no responsibility for injury to vendee or third persons proximately caused by abnormal use of the material even if reasonable safety procedures are followed. Furthermore, vendee assumes the risk in his use of the material.
