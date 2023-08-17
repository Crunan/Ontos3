# Running Scanning Cycle

In Engineer Mode, executing a scanning cycle involves precise parameter configurations to achieve optimal results. Follow these steps to ensure successful scanning operations:

{% hint style="success" %}
1. **THICKNESS Setting**: Specify the thickness of the component, which, together with the process gap, determines the safe vertical position of the substrate chuck during X/Y scanning movement. This positioning is crucial for operating the X/Y scanning process effectively and safely.
2. **GAP Configuration**: Define the process gap, the space between the plasma head and the component during X/Y scanning. The process gap varies based on gas chemistry and ensures proper local confinement for maintaining gas flow while preventing ambient air from entering the process zone.
3. **OVERLAP Adjustment**: Set the scanning overlap parameter, influencing the aperture width calculation for stage indexing between neighboring scans. The effective aperture width varies with plasma parameters. Positive and negative overlap values determine whether neighboring scans "overlap" or "underlap" for uniform treatment. For example:
   * Positive Overlap: Effective width is smaller than the actual aperture. Neighboring scans are closer for uniform treatment, typical of reducing-chemistry processes.
   * Negative Overlap: Effective width is larger than the aperture. Neighboring scans are spread out, useful for processes like surface activation and organic removal.
4. **SPEED Configuration**: Set the scanning speed, expressed in millimeters per second. Adjust the speed based on process requirements; lower speeds are suitable for thick oxide reduction, while higher speeds are ideal for thin organic layer removal.
5. **CYCLES Adjustment**: If desired, configure the number of scan cycles within a single recipe. For certain applications, performing repeated, less aggressive scans can be beneficial. After each cycle, the scanning sequence repeats, starting from the beginning of the next cycle.
6. **Begin Plasma**: [Start Plasma.](running-plasma.md) If Auto-Scan is enabled, the stage should begin the Run Scan automatically.
7. **Run Scan**: Click the \[RUN SCAN] button when ready to initiate the scan sequence.  &#x20;
{% endhint %}

