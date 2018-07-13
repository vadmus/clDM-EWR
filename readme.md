#clDM-EWR

Digital Micrograph plugin for acquisition and exit wave reconstruction of focal series using the FTSR method.

###What do you need

1. First of all you need Digital Micrograph ver_2.x x64
2. Digital Micrograph SDK 2.x (available by request on Gatan offisial site)
2. OpenCL SDK (for example, intel openCL SDK 1.7)
3. clAmdFft
4. clFFT

###Features

1. Uses OpenCL to provide acceleration via multicore CPU or AMD/nVidia GPU.
2. Optimised for GPUs to provide best performance. (Even better for GPUs that can store entire series in RAM at once).
3. Can take focal series via adjusting focus or voltage(if available).
4. Capable of full reconstructions in under 2s on high performance GPUs. (cf. several minutes on CPU, (30 minutes for other software.)
5. Very customisable (accuracy or speed).
6. Image alignment can be performed via phase correlation, or Mutual Information.
7. Includes functionality to calibrate focal increment via automated diffractogram matching procedure.

###Installation

To install this plugin copy clDM-EWR.dll and clFFT.dll to plugin folder (*)

* for windows 7 you can find it here:
C:\Program Files\Gatan\Plugins
or here
C:\ProgramData\Gatan\Plugins
