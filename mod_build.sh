#!/bin/bash

sudo rmmod char_driver

make clean

make

cd build

make

sudo insmod char_driver.ko

sudo chmod 777 /dev/a6
