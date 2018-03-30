# samShell

I created this shell which supports:
```
Logic operators:            && || ; 
Parenthesis:                ()
Input/Output Redirection:   > >> < 
Piping:                     | 
Tests:                      using both "test" and []
Comments:                   anything after '#' is ignored
```
I used C/C++ and basic linux system calls such as:
```
pipe(), dup() and dup2(), fork(), open() and close(), and execvp()
```

An inportant key of my implementation is that I used [Composite Pattern](https://en.wikipedia.org/wiki/Composite_pattern).

## Getting Started and Installing

Git clone this repository, move to its directory, then make.

Simply move to the bin (which will be created with make) and run samShell.

You will be prompted with a "$" for your input.

To exit, simply type in "exit" or "quit" anywhere in a composite of commands or as a command alone.

## Author
Samuel Dominguez

Univerity of California, Riverside

Computer Science and Engineering

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Documentation
I have created extensive documentation on how this program creates a tree of commands and executes them. Visit the Documents directory for more.
