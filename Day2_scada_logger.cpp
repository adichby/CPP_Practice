#include <iostream>
#include <fstream> // Excel/CSV File banane ke liye library
#include <string>
using namespace std;

// === PLC FC1: Standard Analog Scaling Function (Reusable) ===
float scaleAnalog(float raw_count, float min_eng, float max_eng) {
    return min_eng + (raw_count / 27648.0) * (max_eng - min_eng);
}

int main() {
    float pt_raw, ft_raw;
    float pressure_bar, flow_m3h;
    string alarm_status;

    // 1. Excel (CSV) File Open/Create Karna
    ofstream logFile("Pump_SCADA_Log.csv");

    // Excel ke Column Headers likhna
    logFile << "Scan_No,PT101_Bar,FT101_m3h,System_Status\n";

    cout << "=== DAY 2: SCADA DATA LOGGER & FC BLOCK ===" << endl;

    // 3 Scan Cycles ka Loop
    for (int scan = 1; scan <= 3; scan++) {
        cout << "\n--- [SCAN CYCLE #" << scan << "] ---" << endl;
        
        cout << "Enter PT-101 Pressure Raw Count (0-27648): ";
        cin >> pt_raw;
        cout << "Enter FT-101 Flow Raw Count (0-27648): ";
        cin >> ft_raw;

        // Humare banaye hue FC Block (Function) ko call karna!
        // PT-101: 0 to 16 Bar | FT-101: 0 to 500 m3/hr
        pressure_bar = scaleAnalog(pt_raw, 0.0, 16.0);
        flow_m3h     = scaleAnalog(ft_raw, 0.0, 500.0);

        // Interlock Logic
        if (pt_raw < 0 || ft_raw < 0) {
            alarm_status = "FAULT_WIRE_BREAK";
        } else if (pressure_bar > 12.0) {
            alarm_status = "HIGH_PRESS_TRIP";
        } else if (pressure_bar > 2.0 && flow_m3h < 10.0) {
            alarm_status = "DRY_RUN_ALARM";
        } else {
            alarm_status = "NORMAL_RUNNING";
        }

        // Terminal (HMI) par dikhana
        cout << "-> PT-101: " << pressure_bar << " Bar | FT-101: " << flow_m3h << " m3/h" << endl;
        cout << "-> STATUS: " << alarm_status << endl;

        // 2. Excel (CSV) File ke andar row save karna
        logFile << scan << "," << pressure_bar << "," << flow_m3h << "," << alarm_status << "\n";
    }

    logFile.close(); // File save karke band karna
    cout << "\n[SUCCESS] Data logged into 'Pump_SCADA_Log.csv'!" << endl;

    return 0;
}