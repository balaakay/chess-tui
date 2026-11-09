#!/usr/bin/env bash

set -o pipefail

output="chess.out"

gcc src/*.c -o "$output"

if [ $? -ne 0 ]; then
  echo "failed to compile"
  exit 1
fi

./"$output"
exit 0
