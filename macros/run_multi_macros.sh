#!/bin/bash

cd multi_macros

shopt -s nullglob
for file in ./*
do
    ../../build/startSIM -m "$file"
done
shopt -u nullglob #revert nullglob back to it's normal default state

