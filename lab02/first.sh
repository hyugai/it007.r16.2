#!/bin/sh
for file in *
do
    if [ -f "$file" ] && grep -l 'main()' "$file"
    then
        more "$file"
    fi
done
exit 0
