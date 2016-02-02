# Monitor

This project is an implementation of distributed "monitor" (mutual exclusion + condition variable) based on Ricart–Agrawala algorithm. It provides typical operations like lock/unlock/wait/signalOne/signalAll. You can create multiple mutexes and associated condition variables. With each mutex you can associate data structure that will act as a shared object and will be provided to process that currently has access to cirital section. Implementation uses OpenMPI library for communication.

## Usage

You can compile this project using Makefile:

    make clean && make

The base directory contains example solution of producer-consumer problem (see algorithms.hpp & main.cpp) that uses main library located in "src" subdirectory. You can run that example implementation using:

    make run

It should create 10 processes and produce output that shows competition for resources.

## License

See LICENSE file.
