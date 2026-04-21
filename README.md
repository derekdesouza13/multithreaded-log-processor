# Multi-Threaded Log Processing Engine (C++)

## Overview
This project implements a high-performance log processing system using multithreading.

## Features
- Multi-threading using std::thread
- Mutex synchronization
- CLI support
- Performance benchmarking

## How to Run
```bash
g++ src/main.cpp -o log_processor
./log_processor logs/sample.log 4

Performance
Single thread: ~X ms
Multi-thread: ~Y ms

Tech
C++
STL
Multithreading