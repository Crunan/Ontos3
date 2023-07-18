# Log Commands

#### The log files are time stamped as can be seen in the figure below.

Figure 47: Log File Time Stamps

#### The log file will track the following events.

<table data-header-hidden><thead><tr><th width="546"></th><th></th></tr></thead><tbody><tr><td><strong>Initiating Event for Log Entry</strong></td><td><strong>Logged Value</strong></td></tr><tr><td>Open a Recipe</td><td>Recipe File name</td></tr><tr><td>Save</td><td>Recipe File name</td></tr><tr><td>Save As</td><td>Recipe File name</td></tr><tr><td>Simulate Plasma On</td><td>"Plasma Simulation Mode ON"</td></tr><tr><td>Engineer Mode selected</td><td>"Engineer Mode enabled"</td></tr><tr><td>Operator Mode selected</td><td>"Operator Mode enabled"</td></tr><tr><td>Comms Error</td><td>"Comms error: " + the last command sent</td></tr><tr><td>Vac On/Off</td><td>"Vac On" "Vac Off"</td></tr><tr><td>Laser Sensor Tripped</td><td>“Laser Sensor Tripped”</td></tr><tr><td>Laser Sensor Activated</td><td>“Laser Sensor Active”</td></tr><tr><td>Laser Sensor deactivated</td><td>“Laser Sensor Deactivated”</td></tr><tr><td>Initialize Stage  </td><td>"Initializing Axes"</td></tr><tr><td>Initialized Staged</td><td>"Stage Initialized"</td></tr><tr><td>Preheat Plasma Head</td><td>“Preheat Plasma Recipe ON / OFF”</td></tr><tr><td>Auto Scan (Plasma Off feature)</td><td>"Plasma turned off (Auto-Off is active)"</td></tr><tr><td>Batch ID Toggled On/Off</td><td>"Batch ID logging toggled ON / OFF"</td></tr><tr><td>Batch ID button</td><td>“Logging Batch ID #:”</td></tr><tr><td>Load Position + Move Speed</td><td>"Homing Start" + Speed of the axis</td></tr><tr><td>Load Position Z</td><td>"Homing Z"</td></tr><tr><td>Load Position Z Parked</td><td>"Homing Z Parked"</td></tr><tr><td>Load Position XY</td><td>"Homing X &#x26; Y" + Speed and location of the X and Y axis</td></tr><tr><td>X &#x26; Y Loaded</td><td>"X &#x26; Y Homed"</td></tr><tr><td>Z Loaded</td><td>"Z Homed"</td></tr><tr><td>Load Stopped</td><td>"Operator Abort Homing"</td></tr><tr><td>Joystick Started</td><td>"TwoSpotSM Start Up"</td></tr><tr><td>Joystick First Spot</td><td>"TwoSpotSM Got First"</td></tr><tr><td>Joystick Second Spot</td><td>"TwoSpotSM Getting Second"</td></tr><tr><td>Joystick Complete</td><td>"TwoSpotSM Got Second - done"</td></tr><tr><td>Scanning Started</td><td>"-------------Scan Start-Up--------------"</td></tr><tr><td>State Machine Start Up</td><td>"Main State Machine Start Up"</td></tr><tr><td>State Machine Shut Down</td><td>"Main State Machine Shut Down"</td></tr><tr><td>Get MFCs</td><td>"Number of MFCs: "</td></tr><tr><td>Get Axes Status</td><td>"Bad Response = "" + Last Command from the 3-Axes PCB</td></tr><tr><td>Status Change</td><td>If there is a change in the status of anything within the Control PCB then it is logged in the Log File.</td></tr><tr><td>Status Bits changed</td><td>"Status Bits Change from " + (StatusBits)</td></tr><tr><td>Status Bits changed: No N2 purge detected</td><td>"NO PURGE N2"</td></tr><tr><td>Status Bits changed: No connection or USB</td><td>"NO HEARTBEAT"</td></tr><tr><td>Status Bits changed: Low Threshold for MFC1</td><td>"MFC_1 Low Flow"</td></tr><tr><td>Status Bits changed: Low Threshold for MFC2</td><td>"MFC_2 Low Flow"</td></tr><tr><td>Status Bits changed: Low Threshold MFC3</td><td>"MFC_3 Low Flow"</td></tr><tr><td>Status Bits changed: Low Threshold MFC4</td><td>"MFC_4 Low Flow"</td></tr><tr><td>Status Bits changed: No Flow for Helium</td><td>"BAD HELIUM"</td></tr><tr><td>Status Bits changed: ESTOP ACTIVATED</td><td>"ESTOP ACTIVE"</td></tr><tr><td>Status Bits changed: MFC are no longer communicating</td><td>"MFC Comms Down"</td></tr><tr><td>Status Bits changed: Power from RF too low</td><td>"Power Fwd Low"</td></tr><tr><td>Status Bits changed: Head Temperature too high</td><td>"Head Too Hot"</td></tr></tbody></table>

Table 2: Event Log Table

Note: For more details on how to resolve the error codes found in Table 2: Event Log Table see **section 8**.

#### The log file can be stored onto a USB or other network drive by simply using the Windows _Cut/Copy_ as shown below in **Figure 48**.

Figure 48: Log to USB or Network Cut/Copy

#### In the **Figure 49** below, the Log file is pasted onto the desktop, but any location such as USB or other desired network drive can be selected.

Figure 49: Log Paste Location

### &#x20;<a href="#_toc84500390" id="_toc84500390"></a>
