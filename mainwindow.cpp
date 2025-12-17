#include "MainWindow.h"
#include <QtWidgets>
#include <QTimer>
#include <cmath>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // Initialize
    depth = 0.0;
    pressure = atmPressure;
    maxPressure = 450000.0;
    isRunning = false;
    hasFailed = false;
    fluidDensity = 1000.0;

    setupUI();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateSimulation);

    setWindowTitle("Pressure Simulator");
    resize(1000, 600);
}

void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    // === LEFT: CONTROLS ===
    QGroupBox *controlBox = new QGroupBox("CONTROLS");
    QVBoxLayout *controlLayout = new QVBoxLayout;

    // Mode
    controlLayout->addWidget(new QLabel("Mode:"));
    modeBox = new QComboBox;
    modeBox->addItems({"Preset", "Custom"});
    connect(modeBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onModeChanged);
    controlLayout->addWidget(modeBox);

    // Fluid
    controlLayout->addWidget(new QLabel("Fluid:"));
    fluidBox = new QComboBox;
    fluidBox->addItems({"Fresh Water", "Sea Water", "Heavy Fluid"});
    connect(fluidBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onFluidChanged);
    controlLayout->addWidget(fluidBox);

    // Structure
    controlLayout->addWidget(new QLabel("Structure:"));
    structureBox = new QComboBox;
    structureBox->addItems({"Simple", "Reinforced", "Vessel", "Human Diver"});
    connect(structureBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onStructureChanged);
    controlLayout->addWidget(structureBox);

    // Safety Factor (Custom only)
    controlLayout->addWidget(new QLabel("Safety Factor:"));
    safetySlider = new QSlider(Qt::Horizontal);
    safetySlider->setRange(10, 30);
    safetySlider->setValue(20);
    safetyLabel = new QLabel("2.0");
    connect(safetySlider, &QSlider::valueChanged, this, &MainWindow::onSafetyChanged);
    controlLayout->addWidget(safetySlider);
    controlLayout->addWidget(safetyLabel);

    // Descent Rate - FIXED: Better range
    controlLayout->addWidget(new QLabel("Descent Rate (m/s):"));
    rateBox = new QDoubleSpinBox;
    rateBox->setRange(0.5, 50.0);        // CHANGED: from 0.1-5.0 to 0.5-50.0
    rateBox->setValue(2.0);              // CHANGED: Default from 1.0 to 2.0
    rateBox->setSingleStep(0.5);         // CHANGED: Better step size
    rateBox->setSuffix(" m/s");

    // Add tooltip for user guidance
    rateBox->setToolTip("0.5 = Slow (Realistic)\n"
                        "2.0 = Normal\n"
                        "10.0 = Fast\n"
                        "50.0 = Very Fast (For Testing)");

    connect(rateBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onRateChanged);
    controlLayout->addWidget(rateBox);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    startButton = new QPushButton("START");
    resetButton = new QPushButton("RESET");
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startSimulation);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetSimulation);
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(resetButton);
    controlLayout->addLayout(buttonLayout);

    controlLayout->addStretch();
    controlBox->setLayout(controlLayout);
    mainLayout->addWidget(controlBox, 1);

    // === CENTER: SIMULATION ===
    QGroupBox *simBox = new QGroupBox("SIMULATION");
    QVBoxLayout *simLayout = new QVBoxLayout;

    // Displays
    QGridLayout *grid = new QGridLayout;
    grid->addWidget(new QLabel("Depth:"), 0, 0);
    depthDisplay = new QLCDNumber(6);
    depthDisplay->display(depth);
    grid->addWidget(depthDisplay, 0, 1);

    grid->addWidget(new QLabel("Pressure:"), 1, 0);
    pressureDisplay = new QLCDNumber(10);
    pressureDisplay->display(pressure);
    grid->addWidget(pressureDisplay, 1, 1);
    simLayout->addLayout(grid);

    // Status
    statusLabel = new QLabel("SAFE");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("background: green; color: white; padding: 10px;");
    simLayout->addWidget(statusLabel);

    // Safety Bar
    simLayout->addWidget(new QLabel("Safety Margin:"));
    safetyBar = new QProgressBar;
    safetyBar->setRange(0, 100);
    safetyBar->setValue(100);
    safetyBar->setFormat("%p%");
    simLayout->addWidget(safetyBar);

    // Simple Visual
    depthLabel = new QLabel("Depth: 0.0 m");
    depthLabel->setAlignment(Qt::AlignCenter);
    depthLabel->setStyleSheet("border: 2px solid blue; background: lightblue; padding: 20px;");
    simLayout->addWidget(depthLabel);

    simLayout->addStretch();
    simBox->setLayout(simLayout);
    mainLayout->addWidget(simBox, 2);

    // === RIGHT: INFO ===
    QGroupBox *infoBox = new QGroupBox("INFORMATION");
    QVBoxLayout *infoLayout = new QVBoxLayout;

    // Pseudo Code
    pseudoCode = new QTextEdit;
    pseudoCode->setPlainText(
        "PSEUDO-CODE:\n"
        "1. Start: depth=0, pressure=101325\n"
        "2. Loop: depth = depth + rate*0.1\n"
        "3. Calculate: pressure = 101325 + density*9.81*depth\n"
        "4. Check: if pressure > maxPressure -> FAIL\n"
        "5. Update display\n"
        "6. Repeat until fail\n"
        );
    pseudoCode->setReadOnly(true);
    infoLayout->addWidget(pseudoCode);

    // Explanation - UPDATED with new rate info
    explanation = new QTextEdit;
    explanation->setPlainText(
        "PARAMETER EFFECTS:\n"
        "- Fluid: Changes density (kg/m³)\n"
        "  Fresh=1000, Sea=1025, Heavy=1200\n"
        "- Structure: Sets max pressure\n"
        "  Simple=500kPa, Reinforced=1MPa\n"
        "  Vessel=2MPa, Human=300kPa\n"
        "- Safety Factor: 1.0-3.0\n"
        "  Higher = stronger structure\n"
        "- Descent Rate: Speed of simulation\n"
        "  0.5-2.0 = Realistic speed\n"
        "  5.0-10.0 = Fast simulation\n"
        "  20.0-50.0 = Very fast (testing)\n"
        );
    explanation->setReadOnly(true);
    infoLayout->addWidget(explanation);

    infoLayout->addStretch();
    infoBox->setLayout(infoLayout);
    mainLayout->addWidget(infoBox, 1);

    setCentralWidget(central);
}

// === EVENT HANDLERS ===
void MainWindow::onModeChanged()
{
    bool isCustom = (modeBox->currentIndex() == 1);
    safetySlider->setEnabled(isCustom);
    calculateMaxPressure();
}

void MainWindow::onFluidChanged()
{
    int idx = fluidBox->currentIndex();
    if(idx == 0) fluidDensity = 1000.0;
    else if(idx == 1) fluidDensity = 1025.0;
    else fluidDensity = 1200.0;
    calculatePressure();
    updateUI();
}

void MainWindow::onStructureChanged()
{
    calculateMaxPressure();
}

void MainWindow::onSafetyChanged(int value)
{
    double factor = value / 10.0;
    safetyLabel->setText(QString::number(factor, 'f', 1));
    calculateMaxPressure();
}

void MainWindow::onRateChanged(double value)
{
    // Optional: You can add visual feedback for rate changes
    // For now, just store the value
}

// === SIMULATION CONTROL ===
void MainWindow::startSimulation()
{
    if(hasFailed) return;

    isRunning = true;
    startButton->setEnabled(false);

    // Adjust timer interval based on rate for smoother display
    // Faster rates need faster updates to look smooth
    double rate = rateBox->value();
    int interval;

    if(rate <= 2.0) interval = 100;        // Normal speed
    else if(rate <= 10.0) interval = 50;   // Faster
    else interval = 20;                    // Very fast

    timer->start(interval);
}

void MainWindow::resetSimulation()
{
    timer->stop();
    depth = 0.0;
    pressure = atmPressure;
    hasFailed = false;
    isRunning = false;
    startButton->setEnabled(true);
    updateUI();
}

void MainWindow::updateSimulation()
{
    if(hasFailed) {
        timer->stop();
        return;
    }

    // Increase depth - CHANGED: More consistent scaling
    double timeStep = timer->interval() / 1000.0; // Convert ms to seconds
    depth += rateBox->value() * timeStep;

    // Calculate physics
    calculatePressure();
    updateStatus();
    updateUI();

    // Check failure
    if(pressure >= maxPressure) {
        hasFailed = true;
        timer->stop();
        statusLabel->setText("FAILED!");
        statusLabel->setStyleSheet("background: red; color: white; padding: 10px;");
    }
}

// === CALCULATIONS ===
void MainWindow::calculatePressure()
{
    pressure = atmPressure + (fluidDensity * gravity * depth);
}

void MainWindow::calculateMaxPressure()
{
    double base;
    int idx = structureBox->currentIndex();

    if(idx == 0) base = 500000.0;      // Simple
    else if(idx == 1) base = 1000000.0; // Reinforced
    else if(idx == 2) base = 2000000.0; // Vessel
    else base = 300000.0;              // Human

    if(modeBox->currentIndex() == 0) { // Preset
        maxPressure = base * 1.5;
    } else { // Custom
        double factor = safetySlider->value() / 10.0;
        maxPressure = base * factor;
    }
}

void MainWindow::updateStatus()
{
    // Simple status logic
    double critical = 0.8 * maxPressure;

    if(pressure < critical) {
        statusLabel->setText("SAFE");
        statusLabel->setStyleSheet("background: green; color: white; padding: 10px;");
    } else if(pressure < maxPressure) {
        statusLabel->setText("CRITICAL");
        statusLabel->setStyleSheet("background: yellow; color: black; padding: 10px;");
    } else {
        statusLabel->setText("FAILED");
        statusLabel->setStyleSheet("background: red; color: white; padding: 10px;");
    }
}

void MainWindow::updateUI()
{
    // Update displays
    depthDisplay->display(depth);
    pressureDisplay->display(pressure);

    // Update the depth label in blue box
    depthLabel->setText(QString("Depth: %1 m").arg(depth, 0, 'f', 1));

    // Update safety bar
    double safety = 100.0 * (1.0 - (pressure - atmPressure) / (maxPressure - atmPressure));
    safety = qMax(0.0, qMin(100.0, safety));
    safetyBar->setValue((int)safety);

    // Update bar color
    QString color;
    if(safety > 70) color = "green";
    else if(safety > 30) color = "yellow";
    else color = "red";

    safetyBar->setStyleSheet(QString("QProgressBar::chunk { background-color: %1; }").arg(color));
}
