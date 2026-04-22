#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <string>
#include <unordered_map>
#include <algorithm>

#include "../include/parser.h"
#include "../include/worker.h"

using namespace std;

// Global variables
mutex mtx;
int error_count = 0;
int info_count = 0;
unordered_map<string, int> device_errors;

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cout << "Usage: ./log_processor <file> <threads> <top_k>\n";
        return 1;
    }

    string filename = argv[1];
    int num_threads = stoi(argv[2]);
    int top_k = stoi(argv[3]);

    vector<string> lines = read_file(filename);

    int total = lines.size();
    int chunk = total / num_threads;

    vector<thread> threads;

    for (int i = 0; i < num_threads; i++) {
        int start = i * chunk;
        int end = (i == num_threads - 1) ? total : start + chunk;

        threads.emplace_back(process_chunk, ref(lines), start, end);
    }

    for (auto& t : threads) t.join();

    cout << "ERROR: " << error_count << endl;
    cout << "INFO: " << info_count << endl;

    // 🔥 Convert map → vector for sorting
    vector<pair<string, int>> vec(device_errors.begin(), device_errors.end());

    sort(vec.begin(), vec.end(), [](auto &a, auto &b) {
        return a.second > b.second;
    });

    cout << "\nTop " << top_k << " failing devices:\n";

    for (int i = 0; i < min(top_k, (int)vec.size()); i++) {
        cout << "Device " << vec[i].first << " → " << vec[i].second << " errors\n";
    }

    return 0;
}