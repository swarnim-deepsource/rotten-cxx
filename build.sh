#!/bin/bash

clang++ -c -w src/integer-division.cpp -o /tmp/n.o
clang++ -c -w src/assignment-in-if-condition.cpp -std=c++17 -o /tmp/n.o
clang++ -c -w src/bool-pointer-implicit-conversion.cpp -std=c++11 -o /tmp/n.o
clang++ -c -w src/copy-constructor-init.cpp -o /tmp/n.o
clang++ -c -w src/dangling-handle.cpp -std=c++11 -o /tmp/n.o
clang++ -c -w src/app/no_issues.cpp -o /tmp/n.o
