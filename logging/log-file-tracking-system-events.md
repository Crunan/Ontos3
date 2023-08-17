# Log File - Tracking System Events



The Log File feature in the OntosTT application serves as a comprehensive record of various tasks performed by operators and updates from the tool itself. This tracking system provides valuable insights into the operation of the ONTOS Clean system. The key details about this Log File functionality are presented below:

* **Log File Location**:
  * The log file is stored as a text file in the directory: C:\OTT\_PLUS\Logs.
* **Time Stamping**:
  * Log entries are time-stamped for accurate chronological tracking.
*   **Tracked Events**: The log file captures a range of events that are described below:&#x20;



    | Initiating Event                                     | Description                                                                                              |
    | ---------------------------------------------------- | -------------------------------------------------------------------------------------------------------- |
    | Open a Recipe                                        | Recipe File name                                                                                         |
    | Save                                                 | Recipe File name                                                                                         |
    | Save As                                              | Recipe File name                                                                                         |
    | Simulate Plasma On                                   | <mark style="color:red;">"Plasma Simulation Mode ON"</mark>                                              |
    | Engineer Mode selected                               | <mark style="color:red;">"Engineer Mode enabled"</mark>                                                  |
    | Operator Mode selected                               | <mark style="color:red;">"Operator Mode enabled"</mark>                                                  |
    | Comms Error                                          | <mark style="color:red;">"Comms error: "</mark> + the last command sent                                  |
    | Vac On/Off                                           | <mark style="color:red;">"Vac On" "Vac Off"</mark>                                                       |
    | Laser Sensor Tripped                                 | <mark style="color:red;">“Laser Sensor Tripped”</mark>                                                   |
    | Laser Sensor Activated                               | <mark style="color:red;">“Laser Sensor Active”</mark>                                                    |
    | Laser Sensor deactivated                             | <mark style="color:red;">“Laser Sensor Deactivated”</mark>                                               |
    | Initialize Stage                                     | <mark style="color:red;">"Initializing Axes"</mark>                                                      |
    | Initialized Staged                                   | <mark style="color:red;">"Stage Initialized"</mark>                                                      |
    | Auto Scan (Plasma Off feature)                       | <mark style="color:red;">"Plasma turned off (Auto-Off is active)"</mark>                                 |
    | Batch ID Toggled On/Off                              | <mark style="color:red;">"Batch ID logging toggled ON / OFF"</mark>                                      |
    | Batch ID button                                      | <mark style="color:red;">“Logging Batch ID #:”</mark>                                                    |
    | Load Position + Move Speed                           | <mark style="color:red;">"Homing Start"</mark> + Speed of the axis                                       |
    | Load Position Z                                      | <mark style="color:red;">"Homing Z"</mark>                                                               |
    | Load Position Z Parked                               | <mark style="color:red;">"Homing Z Parked"</mark>                                                        |
    | Load Position XY                                     | <mark style="color:red;">"Homing X & Y"</mark> + Speed and location of the X and Y axis                  |
    | X & Y Loaded                                         | <mark style="color:red;">"X & Y Homed"</mark>                                                            |
    | Z Loaded                                             | <mark style="color:red;">"Z Homed"</mark>                                                                |
    | Load Stopped                                         | <mark style="color:red;">"Operator Abort Homing"</mark>                                                  |
    | Joystick Started                                     | <mark style="color:red;">"TwoSpotSM Start Up"</mark>                                                     |
    | Joystick First Spot                                  | <mark style="color:red;">"TwoSpotSM Got First"</mark>                                                    |
    | Joystick Second Spot                                 | <mark style="color:red;">"TwoSpotSM Getting Second"</mark>                                               |
    | Joystick Complete                                    | <mark style="color:red;">"TwoSpotSM Got Second - done"</mark>                                            |
    | Scanning Started                                     | <mark style="color:red;">"-------------Scan Start-Up--------------"</mark>                               |
    | State Machine Start Up                               | <mark style="color:red;">"Main State Machine Start Up"</mark>                                            |
    | State Machine Shut Down                              | <mark style="color:red;">"Main State Machine Shut Down"</mark>                                           |
    | Get MFCs                                             | <mark style="color:red;">"Number of MFCs: "</mark>                                                       |
    | Get Axes Status                                      | <mark style="color:red;">"Bad Response = ""</mark> + Last Command from the 3-Axes PCB                    |
    | Status Change                                        | If there is a change in the status of anything within the Control PCB then it is logged in the Log File. |
    | Status Bits changed                                  | <mark style="color:red;">"Status Bits Change from "</mark> + (StatusBits)                                |
    | Status Bits changed: No N2 purge detected            | <mark style="color:red;">"NO PURGE N2"</mark>                                                            |
    | Status Bits changed: No connection or USB            | <mark style="color:red;">"NO HEARTBEAT"</mark>                                                           |
    | Status Bits changed: Low Threshold for MFC1          | <mark style="color:red;">"MFC\_1 Low Flow"</mark>                                                        |
    | Status Bits changed: Low Threshold for MFC2          | <mark style="color:red;">"MFC\_2 Low Flow"</mark>                                                        |
    | Status Bits changed: Low Threshold MFC3              | <mark style="color:red;">"MFC\_3 Low Flow"</mark>                                                        |
    | Status Bits changed: Low Threshold MFC4              | <mark style="color:red;">"MFC\_4 Low Flow"</mark>                                                        |
    | Status Bits changed: No Flow for Helium              | <mark style="color:red;">"BAD HELIUM"</mark>                                                             |
    | Status Bits changed: ESTOP ACTIVATED                 | <mark style="color:red;">"ESTOP ACTIVE"</mark>                                                           |
    | Status Bits changed: MFC are no longer communicating | <mark style="color:red;">"MFC Comms Down"</mark>                                                         |
    | Status Bits changed: Power from RF too low           | <mark style="color:red;">"Power Fwd Low"</mark>                                                          |
    | Status Bits changed: Head Temperature too high       | <mark style="color:red;">"Head Too Hot"</mark>                                                           |
* **Log File Management**:
  * The log file can be easily copied or moved to external storage such as a USB drive or a network location. Utilize Windows Cut/Copy functions for this purpose.
  * The destination for storing the log file can be chosen as needed.

By maintaining a detailed log of various operations, events, and system status changes, the Log File functionality empowers operators and engineers with a comprehensive record for analysis, troubleshooting, and process optimization within the ONTOS Clean system.

\
