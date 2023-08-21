# Running Plasma

In Engineer Mode, you have comprehensive control over the parameters required for running the plasma process. To initiate and manage the plasma operation effectively, follow these steps:

{% hint style="success" %}
1. [**Gas Flow Configuration**](../../installation/gasses.md): Gas flow is a crucial parameter programmed in Standard Liters per Minute (SLPM). Gas #1 serves as the carrier gas. It is mandatory to set the gas #1 flow to a value equal to or higher than 10 SLPM, regardless of the chemistry used in the process. This ensures the optimal carrier gas flow rate. Other gases like Helium/Hydrogen, Argon/Hydrogen, Nitrogen, or Oxygen cannot be used independently without Gas #1.
2. **RF Power Setting**: The Radio-Frequency (RF) Power is set in Watts. Adjust the RF Power to the desired level for the plasma process.
3. **Matchbox Tuner Configuration**: The impedance of the Plasma Head varies based on the gas mixture used. An impedance matching network, known as the Matchbox Tuner (MB Tuner), is essential to minimize reflected power and optimize plasma activation time. Engineers can preset the MB Tuner to ensure efficient plasma operation.
4. [**Verify Auto-Start Scan**](settings/auto-start-scan.md): Determine whether the stage scan should automatically initiate when Plasma stabilizes, otherwise disable this setting.
5. **Starting Plasma**: Click the \[START PLASMA] button to initiate the plasma. If the AUTO TUNE option is enabled, the Matchbox Tuner will automatically adjust until the RF Power Reflected reaches a minimum level. This adjustment can be monitored on the RF Reflected dial.
6. **Plasma Ignition**: After the MB Tuner has been properly adjusted and the RF Power Reflected reaches a minimum, plasma ignition will commence as the RF Power Reflected approaches zero watts.
7. **Temperature Monitoring**: Keep track of the Plasma Head temperature using the Temperature Dial to ensure that it remains within the desired operational range.
8. **Stopping Plasma**: When you wish to halt the plasma operation, click on the \[TURN PLASMA OFF] button. This will deactivate the plasma, ending the process.
{% endhint %}

Running Plasma in Engineer Mode allows you to fine-tune and manage critical parameters to ensure optimal plasma performance and reliable processing results. This mode provides you with the control needed to create recipes and achieve precise for efficient plasma operations.
