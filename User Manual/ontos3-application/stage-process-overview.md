# Stage Process Overview

Example of positive (+2mm) and negative (-2mm) overlap)

As shown in Figure 66: Scanning Overlap Setting, sometimes the effective width is larger than the actual aperture (typical of surface activation processes and organic removal processes); and sometimes the effective width is smaller than the actual aperture (typical of reducing-chemistry processes).  The overlap parameter is a correction factor that is applied in the computer calculations of how far the stage shifts in X before starting a neighboring scan.

The overlap parameter can be programmed to either a positive or negative number, depending on whether it is suitable for the neighboring scans to “overlap” or “underlap”.  For example:

*
  * If reducing chemistry is used to remove oxide, the effective aperture width is smaller than the actual aperture width, so you would want the neighboring scans to be closer together (overlapped) to be sure that the whole area is uniformly treated.
  * Conversely, a negative overlap spreads out the neighboring scans.

The actual “overlap” number to use is determined by running tests to optimize lateral uniformity of the substrate in the X dimension after Ontos treatment, usually with an ellipsometer.

*
