# HexGrid
Procedurally generated hexagon grid to be used in turn-based strategy games

This was one of the first projects I created to learn how cpp integrates with Unreal Engine and its Blueprint system. The project creates a procedurally 
generated hexagon grid that can be adjusted for the amount of columns and rows you want or the size of the individual grids. The "smart" grid can detect
when the ground is too steep and will not draw a hexagon at that location. A* search algorithm also implemented for efficient character movement on the grid.
This system is NOT YET replicated to work in a multiplayer setting.
