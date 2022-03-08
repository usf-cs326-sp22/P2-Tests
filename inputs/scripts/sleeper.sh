#!/usr/bin/env bash
 
exec &> /dev/null

# Ignores any other arguments than $1 (unlike the sleep command)
sleep ${1}

