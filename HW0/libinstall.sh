#!/bin/zsh

which -s brew
if [[ $? != 0 ]] ; then
    # Install Homebrew
    curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh
else
    brew update
fi

brew install glew
brew install freeglut
brew install mesa-glu
