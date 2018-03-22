echo testing append to file
echo a >> yo.txt

echo testing truncate 
echo b > yo.txt

echo testing inpute redirect
cat < yo.txt

echo testing single pipe
echo I wAnt tHiS SenTencE iN LoWER cASe | tr A-Z a-z

echo testing redirection and pipes together
cat < hi.txt | tr A-Z a-z | tee newOutFile | tr a-z A-Z > newOutFile2

echo testing pipe after parenthesis

(echo a && echo b) | tr A-Z a-z
