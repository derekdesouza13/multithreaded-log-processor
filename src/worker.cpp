#include "../include/worker.h"

#include <vector>
#include <string>
#include <unordered_map>
#include <mutex>

using namespace std;

// Access global variables from main.cpp
extern mutex mtx;
extern int error_count;
extern int info_count;
extern unordered_map<string, int> device_errors;

void process_chunk(vector<string>& lines, int start, int end) {
    int local_error = 0;
    int local_info = 0;

    unordered_map<string, int> local_device_errors;

    for (int i = start; i < end; i++) {
        string line = lines[i];

        if (line.find("ERROR") != string::npos) {
            local_error++;

            // Extract device ID
            size_t pos = line.find("Device ");
            if (pos != string::npos) {
                string id = line.substr(pos + 7, 3);
                local_device_errors[id]++;
            }
        }
        else if (line.find("INFO") != string::npos) {
            local_info++;
        }
    }

    // Critical section
    lock_guard<mutex> lock(mtx);

    error_count += local_error;
    info_count += local_info;

    for (auto& pair : local_device_errors) {
        device_errors[pair.first] += pair.second;
    }
}