#!/bin/bash

fullstamp=$(date +'%m-%d-%Y_%H-%M-%S')

echo Linux/Unix > build_logs/linux/build_log_${fullstamp}.txt
echo ---- >> build_logs/linux/build_log_${fullstamp}.txt
echo "" >> build_logs/linux/build_log_${fullstamp}.txt
echo BUILD debug_linux32 >> build_logs/linux/build_log_${fullstamp}.txt
scons platform=linux target=template_debug arch=x86_32 >> build_logs/linux/build_log_${fullstamp}.txt
echo "" >> build_logs/linux/build_log_${fullstamp}.txt
echo BUILD release_linux32 >> build_logs/linux/build_log_${fullstamp}.txt
scons platform=linux target=template_release arch=x86_32 >> build_logs/linux/build_log_${fullstamp}.txt
echo "" >> build_logs/linux/build_log_${fullstamp}.txt
echo BUILD debug_linux64 >> build_logs/linux/build_log_${fullstamp}.txt
scons platform=linux target=template_debug arch=x86_64 >> build_logs/linux/build_log_${fullstamp}.txt
echo "" >> build_logs/linux/build_log_${fullstamp}.txt
echo BUILD release_linux64 >> build_logs/linux/build_log_${fullstamp}.txt
scons platform=linux target=template_release arch=x86_64 >> build_logs/linux/build_log_${fullstamp}.txt
echo "" >> build_logs/linux/build_log_${fullstamp}.txt

echo COMPLETE >> build_logs/linux/build_log_${fullstamp}.txt