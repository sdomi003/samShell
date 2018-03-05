#!/bin/bash

echo testing test implementation

test makefile && echo exists

test -e makefile && echo exists

test asdfasdf || echo Does not exist

test -d src && echo exists

echo Testing Brackets

[makefile] && echo exists 

[-e makefile] && echo exists

[-f makefile] && echo exists

[-d makefile] && echo exists

[asdfasdf] || echo Does not exist

