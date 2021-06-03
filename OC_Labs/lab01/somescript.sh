#!/bin/bash
mkdir -p 9091/Prokofyev
cd 9091/Prokofyev
date > Max.txt
date --date="next mon" > filedate.txt
cat Max.txt filedate.txt > result.txt
cat result.txt
