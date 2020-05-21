# MazeSolver
 Blind maze solver and match manager
 
 ## Rules
 - Each maze is defined in a .maze file which includes its dimensions, maximum allowed steps, start and end points and the maze itself
 - The maze can be of any size or shape, and does not have to be closed - stepping beyond an open edge trasports the solver to the    beggining/end of the row/column respectively.
 - The solver is blind - it does not have any starting information about the maze including its dimensions. Each turn the solver can move up/down/left/right or leave a bookmark in its current position without moving. Any of these actions counts as one step
 - After each move the solver makes, the match manager reports to the solver whether a wall/bookmark was hit

 ## Features
  - Multithreaded match manager which dynamically loads solvers (which are shared objects), test them against each other and output a results table to the screen after all solvers finished:
  
  ![](https://raw.githubusercontent.com/ron828/MazeSolver/master/Screenshots/match-results.png)
  
  - A standard interface for adding a new solver (see `AbstractAlgorithm.h`)
  - Includes a graphical program to visually show the path taken by a solver (see `Showpath`):
  
  ![](https://raw.githubusercontent.com/ron828/MazeSolver/master/Screenshots/showpath.gif)
  
  - The included solver ranked #1 in the Advanced Topics in Programming course, TAU 2019
