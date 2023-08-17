# Crash Avoidance System

The Crash Avoidance System's Engineer Mode provides advanced settings and controls for configuring the system's collision detection capabilities. Designed to safeguard the Plasma Head and Substrate from potential collisions caused by incorrect Operator inputs, the Crash Avoidance System adds an additional layer of protection to the ONTOS Clean tool.

To access and utilize the Crash Avoidance System in Engineer Mode, follow these steps:

{% hint style="success" %}
1. **Enable Crash Avoidance System**: Navigate to the Engineer Mode section of the application. In this mode, you'll have access to the advanced settings and controls, including the Crash Avoidance System. Activate the Crash Avoidance System option to enable the system's sensors.
2. **Collision Test**: With the Crash Avoidance System active, the system will perform a collision test to ensure that the substrate can pass safely under the plasma head. This involves elevating the stage chuck to its maximum Z-axis height and then moving it towards the plasma head.
3. **Operator Input Validation**: The Collision Test assesses the correctness of Operator inputs for Gap and Thickness settings. If these inputs have been correctly configured, the substrate on the chuck will successfully pass under the plasma head without any issue.
4. **Detection of Miscalculation**: If there is a miscalculation in the Gap and Thickness settings, the Crash Avoidance System's laser sensors will detect the potential collision before it occurs. In response, the system will promptly halt all stage movement to prevent any contact between the substrate and the plasma head.
5. **Error Indication**: When the Crash Avoidance System prevents a collision, the system will indicate this with an error code labeled "LASER TRIPPED." Additionally, the Indicator box for the Laser Sensor will illuminate in red, clearly indicating that the Laser has detected a potential collision.
6. **Acknowledge Error**: In the Error Code section, click the "Acknowledge" button to acknowledge the prevented collision. This action will return the stage to the Load position.
7. **Corrective Action**: Evaluate and determine the accurate substrate gap and thickness values, and input these values into the system. Perform the Collision Test again to verify that the new values ensure safe passage of the substrate beneath the plasma head.
{% endhint %}

The Crash Avoidance System – Engineer Mode empowers you with comprehensive control over collision detection settings, ensuring the utmost safety and operational precision of the ONTOS Clean system. For detailed instructions on utilizing and configuring the Crash Avoidance System in Engineer Mode, refer to section 7.1.9 ("Crash Avoidance System – Engineer Mode").

### &#x20;<a href="#_toc84500395" id="_toc84500395"></a>
