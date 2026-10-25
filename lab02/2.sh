#!/bin/bash

MSSV="25520692"

while true; do
    read -rp "Enter MSSV: " user_input

    if [[ "$user_input" == "$MSSV" ]]; then
        printf "Correct MSSV!\nHello, Le Gia Huy.\n"
        break
    else
        echo "Wrong MSSV!"
    fi
done
