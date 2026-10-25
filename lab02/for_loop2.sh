#!/bin/sh
for file in f*.sh
do
    [ -f "$file" ] || continue
    lpr "$file"
done
exit 0
