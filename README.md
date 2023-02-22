# NASA GLEE 3214
 
https://www.youtube.com/watch?v=zvgQ6exOklg

https://github.com/GLEE2023/GLEE2023#Library-Descriptions

https://drive.google.com/drive/u/0/folders/10Ula5tRrOFotDJTbjy7J_5Lvk9pGOtq1

Data Points:
- Seismic Activity: Magnetometer & Accelerometer
- Lunar Regolith Capacitance: Capacitive Sensor
- Temperature: Thermopile & Temperature Sensor
- Light Intensity (Experimental, Alternative: Voltage): Solar Panel
- Radio Frequency Data Packets (Recheck): Radio Frequency Transceiver System

Algorithms:
- Gaussian Outlier Detection
- Albedo (Potentially Implicit)
- Energy Balance Equation
- Fourier's Law Of Heat Conduction
- I2C Bus Master & Slave Check
- Wein's Displacement (Emissivity Corrections)
- Maxwell's Equation For Permitivity (Experimental)
- General Matrix & Array Averaging Algorithms
- More To Add (Spike Detection (May Be Integrated With Gaussian Outlier Detection))
- Algorithms That May Be Used: Derivative-Based Spike Detection Method, Local Outlier Factor, Isolation Forest, Minimum Covariance Determinant, One-Class SVM and/or Local Correlation Integral - As opposed to Gaussian or in parallel

Mission (Brief, Not Exact Wording, Just For Understanding Dataflow, Not RQ Wording, CS Wording):
- Correlation Of Seismic Activity, Temperature, Solar Energy & Magnetic Field Changes With Capacitance
- To Think About: Accelerometer Values & Polynomial Regression (May require regularization) / Mass Classification Relationships

Notes:
- Mid-Latitude for max sunlight
- Capacitive sensor In-Contact with regolith constantly
- Few feet away / immediate sensor ranges for thermopile (and possibly IR)
- 100 m magnetic field generalization (Is of the LunaSat)
- 1 Earth Week mission duration
- May have real-time data
- No external computing or sensors
- No program uploads post-launch

General Pointers:
- Specific Sampling Approach - No need to make excessive assumptions
- Light, IR, Temperature, Albedo, Wein's Displacement and potentially Solar Flares favored with ML algorithms OR Lunar Regolith Sampling
- KEEP IT SIMPLE - PROGRAMMATIC PROBLEM

Previous Possible Missions:
- Seismic Activity- Accelerometer, Magnetometer
- Lunar Surface Characterization (Regolith & Composition)- Capacitive Sensor, Magnetometer, Temperature Sensor, Thermopile, Solar Panel (Fourier's Law of Heat Conduction)
- Temperature Variations- Thermopile, Temperature Sensor
- Albedo (Light Intensity Profiling)- Solar Panel, Thermopile, Temperature Sensor (Energy Balance Equations & Emissivity)
