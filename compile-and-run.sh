#!/usr/bin/bash

set -o pipefail

source="$1"
output="$2"

#echo "source = $source"
#echo "output = $output"

gcc $(pkg-config --cflags gtk4) $source -o $output $(pkg-config --libs gtk4)


if [ $? -ne 0 ]; then
  echo "failed to compile"
  exit 1
fi

./$output
exit 0
