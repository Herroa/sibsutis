#!/bin/sh
set -e

CXXFLAGS="-Wall -I ./src/ -Wno-unused -Wno-deprecated -Wno-write-strings -Wno-free-nonheap-object"

bison -d -v -y -b cool --debug -p cool_yy -o cool-bison-parser.cc cool.bison

flex -d -o cool-flex-lexer.cc ./cool.flex

g++ $CXXFLAGS -c parser-phase.cc -o parser-phase.o
g++ $CXXFLAGS -c utilities.cc -o utilities.o
g++ $CXXFLAGS -c stringtab.cc -o stringtab.o
g++ $CXXFLAGS -c cool-tree.cc -o cool-tree.o
g++ $CXXFLAGS -c cool-flex-lexer.cc -o cool-flex-lexer.o
g++ $CXXFLAGS -c cool-bison-parser.cc -o cool-bison-parser.o
g++ $CXXFLAGS -c semant.cc -o semant.o

g++ $CXXFLAGS parser-phase.o semant.o utilities.o stringtab.o cool-tree.o cool-flex-lexer.o cool-bison-parser.o -o parser
