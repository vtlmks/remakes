#!/bin/bash

for dir in */; do
	pushd "$dir" > /dev/null
	./process.sh
	popd > /dev/null
done
