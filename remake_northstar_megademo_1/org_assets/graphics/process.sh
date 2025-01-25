#!/bin/bash

bmp2ugg_exe="$(git rev-parse --show-toplevel)/bin/bmp2ugg"

for file in *.bmp; do
	base_name="${file%.bmp}"
	prefix="${base_name%%_*}" # This grabs everything before the first '_'
	stripped_name="${base_name#*_}" # This grabs everything after the first '_'
	output_dir="../../data/${prefix}"
	"$bmp2ugg_exe" -i "$file" -o "${output_dir}/${stripped_name}.ugg"
done
