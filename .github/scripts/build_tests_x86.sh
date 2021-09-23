#!/bin/bash
set -e

cmake -B build.x86
cmake --build build.x86
