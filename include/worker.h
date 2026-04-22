#ifndef WORKER_H
#define WORKER_H

#include <vector>
#include <string>

void process_chunk(std::vector<std::string>& lines, int start, int end);

#endif