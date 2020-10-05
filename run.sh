#!/usr/bin/bash

gcc arbitrary_ca.c

rule_str=$(./a.out)

convert iteration_*.pbm "$rule_str".gif

rm iteration_*.pbm
