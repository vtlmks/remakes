#!/bin/bash

for dir in remake_*/; do
    if [ -d "$dir" ]; then
        cd "$dir" || continue

        if [ -x "./build.sh" ]; then
            # Run build.sh
            ./build.sh
        else
            echo "build.sh not found or not executable in $dir"
        fi

        cd ..
    fi
done

