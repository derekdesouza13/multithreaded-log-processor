#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

mutex mtx;
int error_count = 0;
int info_count = 0;

// Function to process a chunk of lines
void process_chunk(vector<string>& lines, int start, int end) {
    int local_error = 0;
    int local_info = 0;

    for (int i = start; i < end; i++) {
        if (lines[i].find("ERROR") != string::npos)
            local_error++;
        else if (lines[i].find("INFO") != string::npos)
            local_info++;
    }

    lock_guard<mutex> lock(mtx);
    error_count += local_error;
    info_count += local_info;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Usage: ./log_processor <file> <num_threads>\n";
        return 1;
    }

    string filename = argv[1];
    int num_threads = stoi(argv[2]);

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file\n";
        return 1;
    }

    vector<string> lines;
    string line;

    while (getline(file, line)) {
        lines.push_back(line);
    }

    int total_lines = lines.size();
    int chunk_size = total_lines / num_threads;

    vector<thread> threads;

    auto start_time = chrono::high_resolution_clock::now();

    for (int i = 0; i < num_threads; i++) {
        int start = i * chunk_size;
        int end = (i == num_threads - 1) ? total_lines : start + chunk_size;

        threads.emplace_back(process_chunk, ref(lines), start, end);
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end_time = chrono::high_resolution_clock::now();

    cout << "ERROR count: " << error_count << endl;
    cout << "INFO count: " << info_count << endl;

    cout << "Execution time: "
         << chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count()
         << " ms\n";

    return 0;
}