# Stage Recipe Setpoints



* HICKNESS: The component thickness, together with the process gap described hereafter are used by the system to calculate the vertical position of the substrate chuck in order to operate safely the X/Y scanning movement (Figure 65).
* GAP: The process gap is the space left between the plasma head and the component during the X/Y scanning movement. It varies with gas chemistry. It should not be too large in order to ensure a proper local confinement enabling the gas flow to keep the ambient air to penetrate the process zone.
* OVERLAP: The scanning overlap is a parameter that modifies the aperture width parameter when calculating the indexing of the stage between neighboring scans (whenever the substrate is too wide to be covered in one scan). The effective aperture width, that is, the zone of plasma effectiveness on the substrate, varies with plasma parameters.
* SPEED: The scanning speed, programmed in millimeter(s) per second varies with the process requirements. For instance, if a thick oxide needs to be reduced, the speed will be low while it can be significantly increased if only a thin layer of organics is to be eliminated.
* CYCLES: For some applications, it can be beneficial to execute the treatment with a repeated less aggressive chemistry; in that case, it is possible to perform several scan cycles within a same recipe. A first scanning sequence is performed and if the number of cycles is greater than one, then at the end of a cycle, the scanning is performed again from the beginning of the next cycle.

