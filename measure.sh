#!/bin/bash

get_size() {
    size src/picovm.o | sed -n 2p | cut -f1
}

out=$'|                       | Code size     |
|-----------------------|:-------------:|
'

make -C ./src clean > /dev/null

cp ./src/config.h /tmp/measure-config.h.bak
cp ./src/config_example.h ./src/config.h
make -C ./src measure > /dev/null
out+="| Full features         | $(get_size) bytes |
"

make -C ./src clean > /dev/null
sed -i -e 's/#define INCLUDE_FLOATING_POINT_INSTRUCTIONS//g' ./src/config.h 
make -C ./src measure > /dev/null

out+="| - floating point      | $(get_size) bytes |
"

make -C ./src clean > /dev/null
sed -i -e 's/USE_SINGLE_DATA_TYPE 0/USE_SINGLE_DATA_TYPE 32/g' ./src/config.h 
make -C ./src measure > /dev/null

out+="| - Single integer type | $(get_size) bytes |
"

make -C ./src clean > /dev/null
sed -i -e 's/#define INCLUDE_BITWISE_INSTRUCTIONS//g' ./src/config.h 
make -C ./src measure > /dev/null

out+="| - Bitwise operations  | $(get_size) bytes |
"

make -C ./src clean > /dev/null
sed -i -e 's/#define INCLUDE_OPTIONAL_INSTRUCTIONS//g' ./src/config.h 
make -C ./src measure > /dev/null

mv /tmp/measure-config.h.bak ./src/config.h

out+="| - Advanced operations | $(get_size) bytes |"

f=`perl -p0e 's/\|.*\|/__table__/se' README.md`

echo "${f/__table__/$out}" > README.md
