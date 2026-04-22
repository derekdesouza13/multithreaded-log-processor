#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

#include "../include/parser.h"
#include "../include/worker.h"
#include <unordered_map>

unordered_map<string, int> device_errors;
using namespace std;

mutex mtx;
int error_count = 0;
int info_count = 0;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Usage: ./log_processor <file> <threads>\n";
        return 1;
    }

    string filename = argv[1];
    int num_threads = stoi(argv[2]);

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

    return 0;
}