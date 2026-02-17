#!/usr/bin/env sh
set -eu

make clean && make
sudo make clean install
