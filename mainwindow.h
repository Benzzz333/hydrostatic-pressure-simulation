#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
class QLabel;
class QComboBox;
class QPushButton;
class QSlider;
class QDoubleSpinBox;
class QLCDNumber;
class QProgressBar;
class QTextEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onModeChanged();
    void onFluidChanged();
    void onStructureChanged();
    void onSafetyChanged(int value);
    void onRateChanged(double value);
    void startSimulation();
    void resetSimulation();
    void updateSimulation();

private:
    // Simulation variables
    double depth;
    double pressure;
    double maxPressure;
    bool isRunning;
    bool hasFailed;

    // Constants
    const double atmPressure = 101325.0;
    const double gravity = 9.81;
    double fluidDensity;

    // UI Components
    QComboBox *modeBox;
    QComboBox *fluidBox;
    QComboBox *structureBox;
    QSlider *safetySlider;
    QLabel *safetyLabel;
    QDoubleSpinBox *rateBox;

    QPushButton *startButton;
    QPushButton *resetButton;

    QLCDNumber *depthDisplay;
    QLCDNumber *pressureDisplay;
    QLabel *statusLabel;
    QProgressBar *safetyBar;

    QTextEdit *pseudoCode;
    QTextEdit *explanation;
    QLabel *depthLabel;

    QTimer *timer;

    void setupUI();
    void calculatePressure();
    void calculateMaxPressure();
    void updateStatus();
    void updateUI();
};

#endif // MAINWINDOW_H
