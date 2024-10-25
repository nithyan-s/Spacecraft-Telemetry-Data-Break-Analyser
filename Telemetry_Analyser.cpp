// Program to report data breaks in the telemetry of a spacecraft

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>

using namespace std;

// Report generator
void generate_report(const string& input_file) {
    // Open the input file
    ifstream in(input_file);
    if (!in) {
        cerr << "Unable to open the input file." << endl;
        exit(1);
    }

    ofstream out("report.txt", ios::app);
    if (!out) {
        cerr << "Unable to create output file." << endl;
        exit(1);
    }
    ofstream plot("plot.txt",ios::app);
    if(!plot){
        cerr<<"Unable to create plotter file."<<endl;
        exit(1);
    }
    // Read the file and formulate the report
    string line;
    // Skip the header
    getline(in, line);
    
    vector<vector<string>> times;
    int telemetry_data;

    // Parse each line of telemetry data
    while (getline(in, line)) {
        stringstream ss(line);
        string timeString;
        
        // Read time and telemetry data
        ss >> timeString >> telemetry_data;

        vector<string> time(3);
        stringstream ts(timeString);

        // Read each part of the time
        string component;
        getline(ts, component, ':');
        time[0] = component; // Hours

        getline(ts, component, ':');
        time[1] = component; // Minutes

        getline(ts, component);
        time[2] = component; // Seconds

        // Check if there's a break in telemetry data continuity
        if (!times.empty()) {
            string prevHR = times.back()[0];
            string prevMN = times.back()[1];
            string prevSEC = times.back()[2];

            // Example check: if there's a break of more than 1 second
            if (stoi(time[0]) == stoi(prevHR) && stoi(time[1]) == stoi(prevMN) && stoi(time[2]) != stoi(prevSEC) + 1) {
                out << "Data break detected at time: " << prevHR << ":" << prevMN << ":" << prevSEC
                    << " to " << time[0] << ":" << time[1] << ":" << time[2] << endl;
            }
        }

        // Store the time entry
        times.push_back(time);

        // add the data to plotter file
        plot<<time[0]<<":"<<time[1]<<":"<<time[2]<<"\t"<<telemetry_data<<endl;
    }

    // close the files
    in.close();
    out.close();
    plot.close();

    cout << "Report Generated....Terminating Program...." << endl;
}

// Driver Function
int main() {
    string date;
    cout << "Enter the Date of Initiation (DD-MMM-YY) : ";
    getline(cin, date);

    string input_file = date + ".txt";
    // Call the report generating function
    generate_report(input_file);

    return 0;
}
