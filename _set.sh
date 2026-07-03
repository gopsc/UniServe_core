#!/bin/bash
cd "$(dirname "$0")" || exit
make clean && make
