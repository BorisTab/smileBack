#!/bin/zsh

exec "../progLang/cmake-build-debug/progLang ../program.smile" &
sleep 5
exec "./cmake-build-debug/smileBack ../../progLang/cmake-build-debug/program.ast asmExecute/program.asm" &
echo "Done"