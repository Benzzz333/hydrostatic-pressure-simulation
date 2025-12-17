# HydroPress: Real-Time Hydrostatic Pressure Simulator

HydroPress is an interactive engineering simulation designed to model hydrostatic pressure variation with depth in real time. Developed using **C++** and the **Qt Framework**, the application employs a discrete-time simulation loop to approximate continuous pressure changes during descent.

### Key Features
* **Physics Model:** Computes hydrostatic pressure using the formula $P = P_{atm} + \rho gh$.
* **Real-Time Simulation:** Updates depth, pressure, and system status at fixed time steps.
* **Status Evaluation:** Classifies structural condition into **SAFE**, **CRITICAL**, and **FAILURE** states.
* **Interactive Dashboard:** Displays live depth, pressure, and status indicators.
* **User Controls:** Provides preset and limited custom configurations with locked parameters during simulation.
