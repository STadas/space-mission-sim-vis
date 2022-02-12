#!/bin/bash

find . \( -iname "*.hpp" -o -iname "*.cpp" \) -exec clang-format -i {} \;
