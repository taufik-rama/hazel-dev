#!/bin/bash

# Must be ran on current file directory
find ../hazel-dev/include -iname *.h -o -iname *.hpp | xargs clang-format -i
find ../hazel-dev/src -iname *.c -o -iname *.cpp | xargs clang-format -i
find ../sandbox/include -iname *.h -o -iname *.hpp | xargs clang-format -i
find ../sandbox/src -iname *.c -o -iname *.cpp | xargs clang-format -i
