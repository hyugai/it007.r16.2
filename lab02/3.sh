#!/bin/bash

while true; do
    read -rp "Enter n < ${MAX}: " n

    if [[ ! "$n" =~ ^0*[0-9]$ ]]; then
        echo "Invalid n!"
    else
        break
    fi
done

sum=0
for ((i = 1; i <= n; i++)); do
    ((sum += i))
done

echo "1 +...+ n = ${sum}"
