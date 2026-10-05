#include <iostream>
using namespace std;

int main() {
    float raw_count;
    float pressure_bar;
    bool main_pump = true;      // Main Pump ON [1]
    bool standby_pump = false;  // Standby Pump OFF [0]

    cout << "=== INDUSTRIAL PUMP HOUSE EDGE CONTROLLER ===" << endl;

    // PLC Scan Cycle Simulator (5 Scans ke liye loop)
    for (int scan = 1; scan <= 5; scan++) {
        cout << "\n--- [PLC SCAN CYCLE #" << scan << "] ---" << endl;
        cout << "Enter PT-101 Raw Count (0 - 27648): ";
        cin >> raw_count;

        // 1. Wire Break Interlock
        if (raw_count < 0) {
            cout << "[FAULT] Sensor Wire Break! Both Pumps TRIPPED!" << endl;
            main_pump = false;
            standby_pump = false;
            break; // Emergency Stop: Loop se bahar nikal jao
        }

        // 2. Scaling Calculation (0-10 Bar)
        pressure_bar = (raw_count / 27648.0) * 10.0;
        cout << "Live Header Pressure: " << pressure_bar << " Bar" << endl;

        // 3. Auto Lead/Lag & High Pressure Logic
        if (pressure_bar < 3.0) {
            // Low Pressure: Dono Pump Chala Do (Booster Mode)
            main_pump = true;
            standby_pump = true;
            cout << "[LOW PRESS] Booster Active! Main Pump: ON | Standby Pump: ON" << endl;
        } 
        else if (pressure_bar >= 8.5) {
            // High Pressure Trip
            main_pump = false;
            standby_pump = false;
            cout << "[HIGH PRESS ALARM] Overpressure! Main Pump: TRIP | Standby Pump: TRIP" << endl;
        } 
        else {
            // Normal Operation
            main_pump = true;
            standby_pump = false;
            cout << "[NORMAL] Main Pump: ON | Standby Pump: OFF" << endl;
        }
    }

    cout << "\n=== Edge Controller Scan Completed ===" << endl;
    return 0;
}