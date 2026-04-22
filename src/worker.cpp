#include "../include/worker.h"
#include <mutex>
#include <string>

extern std::mutex mtx;
extern int error_count;
extern int info_count;

void process_chunk(std::vector<std::string>& lines, int start, int end) {
    int local_error = 0;
    int local_info = 0;

    for (int i = start; i < end; i++) {
        if (lines[i].find("ERROR") != std::string::npos)
            local_error++;
        else if (lines[i].find("INFO") != std::string::npos)
            local_info++;
    }

    std::lock_guard<std::mutex> lock(mtx);
    error_count += local_error;
    info_count += local_info;
}