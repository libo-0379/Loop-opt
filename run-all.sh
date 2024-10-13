#!/bin/bash


yhbatch -N 1 -p thcp1 run-O0.sh & \
yhbatch -N 1 -p thcp1 run-O1.sh & \
yhbatch -N 1 -p thcp1 run-O2.sh & \
yhbatch -N 1 -p thcp1 run-O3.sh & \
yhbatch -N 1 -p thcp1 run-jh-O3.sh & \
yhbatch -N 1 -p thcp1 run-jh-hb-O3.sh & \
yhbatch -N 1 -p thcp1 run-jh-hb-tc-O1.sh & \
yhbatch -N 1 -p thcp1 run-jh-hb-tc-O2.sh & \
yhbatch -N 1 -p thcp1 run-jh-hb-tc-O3.sh