#!/bin/bash

while true; do
    read -erp "Enter target file: " -i "test.txt" target_file

    if [[ ! -f "$target_file" ]]; then
        echo "Invalid file path! (${target_file})"
    else
        break
    fi
done

read -rp "Enter string: " pattern

if grep -Fq -- "$pattern" "$target_file"; then
    echo "Pattern FOUND!"
else
    echo "Pattern NOT found."
fi
