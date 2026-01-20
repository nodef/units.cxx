#!/usr/bin/env bash
# Fetch the latest version of the library
fetch() {
if [ -f "units.h" ]; then return; fi
URL="https://github.com/nholthaus/units/raw/refs/heads/master/include/units.h"
FILE="units.h"

# Download the release
if [ ! -f "$FILE" ]; then
  echo "Downloading $FILE from $URL ..."
  curl -L "$URL" -o "$FILE"
  echo ""
fi
}


# Test the project
test() {
echo "Running 01-simple ..."
clang++ -std=c++20 -I. -o 01.exe examples/01-simple.cxx   && ./01.exe && echo -e "\n"
echo "Running 02-pipeline ..."
clang++ -std=c++20 -I. -o 02.exe examples/02-pipeline.cxx && ./02.exe && echo -e "\n"
}


# Main script
if [[ "$1" == "test" ]]; then test
elif [[ "$1" == "fetch" ]]; then fetch
else echo "Usage: $0 {fetch|test}"; fi
