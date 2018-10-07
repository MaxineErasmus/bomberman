#!/bin/bash

command cd build && make -j5

command install_name_tool -change /usr/local/lib/libirrklang.dylib @executable_path/_deps/irrklang/bin/macosx-gcc/libirrklang.dylib Bomberman

echo "done ;) \n"
command ./Bomberman