#!/bin/bash
idf.py -p $1 -b 460800 flash
idf.py -p $1 -b 115200 monitor
