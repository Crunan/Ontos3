# Plasma Issues

{% hint style="info" %}
In **EVERY** recipe, MFC#1 must have a flow setting to open the manifold gate valve. A setting of 0.00 for MFC#1 will cause the manifold to remain closed, and gas pressure from the other flows will only build up behind the valve, eventually leading to a non-flow condition and an error message.
{% endhint %}

**Issue #1: Plasma Fails to Activate**

If the plasma fails to activate (The Plasma On indicator light is not violet), a pop-up window will provide information about potential issues. Common problems include:

{% hint style="warning" %}
* **Gas Supply Lines**: Ensure that the gas supply lines are turned on.
* **MFC#1 Flow Setting**: Verify that the flow of MFC#1 (Helium) is **not** set to 0.00. A value of 0.00 will keep the manifold closed.
* **RF Power**: Check if RF Power is set to 0.00.
* **Active Recipe**: Confirm that an active recipe has been loaded.
{% endhint %}

To address this issue:

{% hint style="success" %}
1. Manually enter 10 SLPM for Helium and 80 watts for RF Power.
2. Run plasma according to section 07.1.5 - "Run Plasma – Engineer Mode".
{% endhint %}

**Issue #2: Plasma Running, but No Plasma Activation**

If the plasma is **RUNNING**, but the current recipe does not light the plasma (The Plasma On indicator light is not violet), it could be due to gas values being outside the operating range of the plasma head.

To troubleshoot this issue:

{% hint style="success" %}
1. [Manually enter](../software-application/engineer-mode/manual-recipe-setpoints.md) **10 SLPM** for Helium.
2. [Manually enter](../software-application/engineer-mode/manual-recipe-setpoints.md) **0.1 SLPM** for Nitrogen. _(Maximum depends on Helium flow but generally under 0.25 SLPM.)_
3. **Optional: If the system is equipped with the Ozone option**, [manually enter ](../software-application/engineer-mode/manual-recipe-setpoints.md)**0.1 SLPM** for Oxygen. _(Maximum depends on Helium flow but generally under 0.25 SLPM.)_
{% endhint %}

{% hint style="danger" %}
Do not run oxygen and hydrogen together in a single process, as this can cause an explosion and may damage the plasma head. Do not run oxygen unless the Optional O2 gas kit has been installed (prevents oxygen and hydrogen from running at the same time).
{% endhint %}

After adjusting the gas settings:

1. [Run plasma](../software-application/engineer-mode/running-plasma.md).

**Issue #3: Plasma Stops During a Cycle**

If the plasma is **RUNNING**, but goes out during a cycle, an error message will appear (most typically, the gas supply has been interrupted by an empty gas cylinder). To troubleshoot this issue, follow these steps:

{% hint style="success" %}
1. Check Gas Flow Compliance:
   * Verify if gas flow compliance is out of specification for an extended period.
   * Potential Causes:
     * Empty gas cylinder.
     * Noise spikes in the environment.
     * Compliance limit set too tight for the gas flow setting (e.g., if trying to control flow to less than 10% of the max range of the MFC).&#x20;
     * **Note**: Contact service before changing compliance settings.
2. Check Source Temperature:
   * Check the temperature of the source, displayed in the process window.
   * If the temperature is near **60°C**, the system may shut down to prevent plasma head overheating.
   * Ensure that cooling water is circulating normally.
3. Verify System Interlocks:
   * A system interlock may be opening, causing the plasma to stop.
   * Check all safety systems to ensure they are functioning properly.
{% endhint %}

After performing the above steps:

1. [Repeat the plasma run](../software-application/engineer-mode/running-plasma.md).
