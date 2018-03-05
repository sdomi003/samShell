#!/bin/bash

echo testing precedence

(echo A && echo B) || (echo C && echo D)

(echo K; echo O) && (echo P || echo T)

(echo A || echo B) && (echo C || echo D)

(false || echo 1 && echo 3) && (echo 6 || echo 7)

echo ending test
