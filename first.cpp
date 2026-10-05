#include <iostream>
using namespace std;

int main() {
    float raw_count;
    float scaled_pressure;

    cout << "Enter Analog Raw Count: ";
    cin >> raw_count;

    // Interlock 1: Wire Break Check (Agar count 0 se kam hai)
    if (raw_count < 0) {
        cout << "FAULT: 4-20mA Sensor Wire Break Detected!" << endl;
    } 
    // Interlock 2: Over-range Check
    else if (raw_count > 27648) {
        cout << "FAULT: Sensor Over-Range / High Current!" << endl;
    } 
    // Normal Scaling + High Pressure Trip Logic
    else {
        scaled_pressure = (raw_count / 27648.0) * 10.0;
        cout << "Live Pressure: " << scaled_pressure << " Bar" << endl;

        // Pump Trip Setpoint at 8.0 Bar
        if (scaled_pressure >= 8.0) {
            cout << "ALARM: High Pressure! Pump TRIP Command Active [1]" << endl;
        } else {
            cout << "STATUS: System Normal. Pump RUNNING [1]" << endl;
        }
    }

    return 0;
}