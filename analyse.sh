#!/bin/bash

# generate data
echo "===Run Hotring==="
./bin/server &
sleep 1
./bin/random_load hotring

echo ""
echo "===Run Hash==="
./bin/hash_server &
sleep 1
./bin/random_load hash

# draw pic
cd utils/
python draw_single.py hotring
python draw_compare.py hotring hash
