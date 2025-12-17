**Hydrostatic Pressure Simulation**

HydroPress is an interactive engineering-oriented simulation designed to model the relationship between **hydrostatic pressure and depth** in real time. Developed using **C++ and the Qt Framework**, the application employs a discrete-time simulation loop to approximate continuous pressure changes during descent.
The simulator focuses on **algorithmic behavior, control flow, and system state transitions**, making it suitable for an **Algorithm and Programming** academic project.

## Key Features

### Physics-Based Pressure Model
Models hydrostatic pressure using the fundamental equation:

P = Patm + ρgh

where pressure increases linearly with depth based on fluid density and gravitational acceleration.

### Real-Time Simulation Loop
The simulation updates depth, pressure, and system status at fixed time steps, enabling continuous real-time feedback during descent.

### Structural Status System
Evaluates the condition of a conceptual structure using three discrete states:
- **SAFE**
- **CRITICAL**
- **FAILURE**

State transitions are determined by comparing the current pressure to an internally calculated maximum tolerable pressure.

### Dual Operation Modes
- **Preset Mode**: Predefined conceptual structures with locked parameters
- **Custom Mode**: User-defined abstract structures using limited adjustable parameters

Once the simulation begins, all parameters are locked to preserve consistency.

### Interactive Dashboard
Displays live numerical feedback including:
- Depth
- Pressure
- Simulation status
- Failure summary (displayed only after failure)

Status indicators are color-coded for clarity.

## System Design Highlights
- Object-oriented class structure (Environment, Structure, Simulation Controller)
- Qt signal-slot mechanism for real-time UI updates
- Event-based failure detection and locking
- Algorithm-focused implementation without advanced physics modeling

## Scope and Limitations
This simulation is **conceptual and educational** in nature. It intentionally excludes:
- Computational Fluid Dynamics (CFD)
- Material science and structural mechanics
- Real-world engineering validation

The system is designed solely to demonstrate **programming logic and algorithmic design**.

## Technologies Used
- **C++**
- **Qt Framework (Widgets)**
- Discrete-time simulation
- Object-Oriented Programming (OOP)

## Academic Context
Developed as part of an undergraduate **Algorithm and Programming** course project.
