#!/bin/zsh
smileBack="/home/boristab/CLionProjects/smileBack"
progLang="/home/boristab/CLionProjects/progLang"

$progLang/cmake-build-debug/progLang $progLang/$1.smile $progLang/cmake-build-debug/$1.ast &&
$smileBack/cmake-build-debug/smileBack $progLang/cmake-build-debug/$1.ast $smileBack/cmake-build-debug/asmExecute/$1.asm &&
fasm $smileBack/cmake-build-debug/asmExecute/$1.asm &&
echo "Done"