# HexGrid
Procedurally generated hexagon grid to be used in turn-based strategy games

This was one of the first projects I created to learn how c++ integrates with Unreal Engine and its Blueprint system. The original project, done with only blueprints, creates a procedurally generated hexagon grid that can be adjusted for the amount of columns and rows you want or the size of the individual grids. The "smart" grid can detect when the ground is too steep and will not draw a hexagon at that location making it dynamic for any terrain a level designer would want. A* search algorithm also implemented for efficient character movement on the grid. The c++ files in this repository are a WIP to implement the same blueprint system in only cpp as practice.


Source/HexGrid/GridSystem/Grid.cpp is the primary file to look at in this repository

Follow the link to see a short clip of the system in action.
https://imgur.com/a/QUdmr6M
