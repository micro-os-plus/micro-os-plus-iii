#! /bin/bash

cd "$(dirname "$0")"

if [ -d "$HOME/Applications/Visual Studio Code.app" ]
then
  exec "$HOME/Applications/Visual Studio Code.app/Contents/MacOS/Electron" .
elif [ -d "/Applications/Visual Studio Code.app" ]
then
  exec "/Applications/Visual Studio Code.app/Contents/MacOS/Electron" .
else
  echo "Could not find Visual Studio Code.app in Applications"
fi
