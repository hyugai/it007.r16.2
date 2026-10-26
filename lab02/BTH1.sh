#!/bin/bash

MSSV="25520692"

read -rp "Enter name: " name

while true; do
    read -rp "Enter MSSV: " mssv

    if [[ "$mssv" == "$MSSV" ]]; then
        printf 'Correct MSSV!\nHello, %s.\n' "$name"
        break
    else
        echo "Wrong MSSV!"
    fi
done
