# Assignment 2
## Overview

In this assignment, we implemented a command shell in c++ modeled after bash.
You input a series of commands with connectors "&&" "||" and ";" and it will compile based on the properties of those connectors

## Issues
There are some issues."" acts as an escape character in my implementation the command:
      
      ls -la "&&" echo hello 
      
  will return a vector error.
  Another command:
      
      "ls -la" 
       
  will work fine. 
     Note. Both of these commands in the bash system would not work.
     Also having to connectors next to one other will essentially break the code.
     example:
     
     ls && || echo hello
  
 will make anything after ls a scrambled mess.
     
 ## Overall
  Overall I believe my implementation of Rshell was rather successful. We managed to parse the strings by using a string stream and then split the various connectors from their strings, then finally we created the final cmd* vector that allowed us to execute all the commands. Future iterations of this project can lead to difficulties as we only have a connector and execute class.

------------------------------------------------------------------------------------------------------------------------------------------
# assignment 3

## Overview
We changed the implementation from a vector to a tree because of questions in modularity. We now implemented "[]", "()" and "test" along with the usual "&&" "||" and ";"  operators. These also have unique properties.

## Issues
Other than the previous issues, we have had no new errors in our code from testing. 

## Percentage Distrubution
* Jason Mendoza
  * Task 1: Github Setup (10%) 
  * Task 2: The Precedence Operators (30%)
  * Task 6: Construct json
  * __Total: 50%__
