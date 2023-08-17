# Running Plasma

To initiate plasma operation, follow these steps:

{% hint style="success" %}
1. **Ensure Recipe is Loaded:**
   * Ensure that a recipe has been loaded as outlined in [**Load a Recipe**](load-a-recipe.md).
2. **Initialize Stage (If Required):**
   * If the stage has not been initialized during machine startup, perform initialization as described in [Initializing the Stage](initializing-the-stage.md).
3. **Set Recipe Values:**
   * Verify that all [recipe setpoint values](../engineer-mode/manual-recipe-setpoints.md) for gases, RF Power, and Matchbox (MB) Tuning are their expected recipe values.
4. **Start Plasma:**
   * Click the **\[START PLASMA]** button.
5. **Tuning and Ignition:**
   * The Matchbox will begin tuning until the RF Power Reflected reaches a minimum value, observed in the RF Reflected dial.
   * Plasma ignition will commence as the RF Power Reflected approaches zero watts after the MB tuner adjustment.
6. **Monitor Plasma Head Temperature:**
   * Keep an eye on the Plasma head temperature displayed on the Temperature Dial.
7. **Stop Plasma:**
   * To halt plasma operation at any point, click on the **\[TURN PLASMA OFF]** button.
{% endhint %}
