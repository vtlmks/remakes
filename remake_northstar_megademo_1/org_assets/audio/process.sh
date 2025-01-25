#!/bin/bash

dest_base="../../data"

for file in *.raw *.mod; do
	base_name="${file}"                          # Full filename
	prefix="${base_name%%_*}"                    # Extract px prefix (e.g., p01, p02)
	stripped_name="${base_name#*_}"              # Remove px_ prefix
	output_dir="${dest_base}/${prefix}"
	cp "$file" "${output_dir}/${stripped_name}"
done

