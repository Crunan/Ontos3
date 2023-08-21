# Crash Avoidance System

The Crash Avoidance System's Engineer Mode provides advanced settings and controls for configuring the system's collision detection capabilities. Designed to safeguard the Plasma Head and Substrate from potential collisions caused by incorrect Operator inputs, the Crash Avoidance System adds an additional layer of protection to the ONTOS _CLEAN_.

To access and utilize the Crash Avoidance System in [Engineer Mode](../accessing-engineer-mode.md), follow these steps:

{% hint style="success" %}
1. **Enable Crash Avoidance System**: Navigate to the Engineer Mode section of the application. In this mode, you'll have access to the advanced settings and controls, including the Crash Avoidance System. Activate the Crash Avoidance System option to enable the system's sensors.
2. [**Enter Stage Recipe Info**](../manual-recipe-setpoints.md)**:** The substrate information can be entered by the Operator under Gap, Thickness.
3. [**Run Plasma**](../running-plasma.md): Run plasma per Starting Plasma section.&#x20;
4. **Collision Test**: With the Crash Avoidance System active, the system will perform a collision test to ensure that the substrate can pass safely under the plasma head. This involves elevating the stage chuck to its maximum Z-axis height and then moving it towards the plasma head.
5. **Operator Input Validation**: The Collision Test assesses the correctness of Operator inputs for Gap and Thickness settings. If these inputs have been correctly configured, the substrate on the chuck will successfully pass under the plasma head without any issue.
6. **Detection of Miscalculation**: If there is a miscalculation in the Gap and Thickness settings, the Crash Avoidance System's laser sensors will detect the potential collision before it occurs. In response, the system will promptly halt all stage movement to prevent any contact between the substrate and the plasma head.
7. **Error Indication**: When the Crash Avoidance System prevents a collision, the system will indicate this with an error code labeled "LASER TRIPPED." Additionally, the Indicator box for the Laser Sensor will illuminate in red, clearly indicating that the Laser has detected a potential collision.
8. [**Acknowledge Error**](../../../troubleshooting/troubleshooting-the-ontos-clean-system.md): In the Error Code section, click the "Acknowledge" button to acknowledge the prevented collision. This action will return the stage to the Load position.
9. **Corrective Action**: Evaluate and determine the accurate substrate gap and thickness values, and input these values into the system. Perform the Collision Test again to verify that the new values ensure safe passage of the substrate beneath the plasma head.
{% endhint %}

The Crash Avoidance System – Engineer Mode empowers you with comprehensive control over collision detection settings, ensuring the utmost safety and operational precision of the ONTOS _CLEAN_.&#x20;

### &#x20;<a href="#_toc84500395" id="_toc84500395"></a>
