#!/bin/bash

mkdir -p .git/hooks
cp scripts/post-checkout .git/hooks/post-checkout
chmod +x .git/hooks/post-checkout
echo "Git hooks installed."

