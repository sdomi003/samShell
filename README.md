# Assignment 4

We now support redirection and piping.
For a detailed view, visit "shell documentation" in the docs folder.

KNOWN ERRORS:

One error is that (echo a && echo b) > file.txt will only echo b into file.txt
This is because it is set up to apply the redirection to the right child of the top node. 
So echo a && (echo a && echo b) > file.txt WORKS but
(echo a && echo b) > file.txt DOES NOT work.

FIX:

This is easily fixed by adding a decorator which will have 1 child, and it will redirect the entire child.
I will update that soon.

## Percentage Distrubution
We want to distribute percentages 50/50.
