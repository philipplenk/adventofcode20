#!/bin/bash

id=${PWD##*/}
id=$(echo $id | awk '{print $1 + 0}' )

fetch_input.sh $id
copy_from.sh base

nohup geany -i Makefile input 02.cpp 01.cpp &
