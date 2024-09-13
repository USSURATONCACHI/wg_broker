#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <out_filename> <xml_interface>"
    exit 1
fi


FILENAME=$1
XML_FILE=$2

gdbus-codegen --generate-c-code=$FILENAME --interface-prefix=ussur.wg. $XML_FILE
mkdir -p src/gen
mkdir -p include/wg_broker/gen
mv $FILENAME.c src/gen/$FILENAME.c
mv $FILENAME.h include/wg_broker/gen/$FILENAME.h