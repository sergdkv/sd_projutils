#!/bin/bash

./filterTXTByProj.exe  1 2 filterTXTByProj.pro filterTXTByProj.inp filterTXTByProj.out
cp filterTXTByProj.inp filterTXTByProj.inp2
./filterTXTByProj.exe  1 2 filterTXTByProj.pro filterTXTByProj.inp2 filterTXTByProj.inp2

