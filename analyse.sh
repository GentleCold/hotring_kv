#!/bin/bash

# generate data
echo "===Run Hotring==="

./bin/server &
sleep 1
./bin/random_load hotring
kill $(pgrep -f "./bin/server")

echo ""
echo "===Run Hash==="
./bin/hash_server &
sleep 1
./bin/random_load hash
kill $(pgrep -f "./bin/hash_server")

# draw pic
cd utils/
python draw_single.py hotring
python draw_compare.py hotring hash
