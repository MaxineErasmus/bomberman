#!/bin/bash
echo >&2 "Checking is Brew is installed";
command -v brew >/dev/null 2>&1 || { echo >&2 "Installing Homebrew Now"; \
source brewinstall.sh; \
source ~/.zshrc;
}
echo >&2 "Brew is installed";
echo >&2 "Checking is CMAKE is installed";
command -v cmake --version >/dev/null 2>&1 || { echo >&2 "Installing CMAKE Now"; \
command brew install cmake 2>&1;
}

echo >&2 "CMAKE is installed";

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

if [ -d $DIR/Resources ];
then
   echo "File Resources already exist."
   command cd build && cmake .. && make -j5
else
   echo "File Resources does not exist cloning\n"
   command git clone https://github.com/lancechant/Bomber-resources Resources && mkdir build && cd build && cmake .. && make -j5 
fi
    


command install_name_tool -change /usr/local/lib/libirrklang.dylib @executable_path/_deps/irrklang/bin/macosx-gcc/libirrklang.dylib Bomberman

echo "done ;) \n"
command ./Bomberman