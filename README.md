Multilateration
===============

Multilateration with 4 ref nodes

Input:  13 doubles: x,y coordinates of 4 reference nodes, fixed height of reference nodes, 4 distance estimates (one measured from each reference node)
	                     
Can be extended for more than 4 reference nodes

Executes the Multilateration equations

Return: Cartesian coordinate (x,y,z) for the unknown location.



compile with e.g. gcc:
gcc -g -o multilateration -ansi Multilateration.c -lm

then run with ./multilateration x1, y1, x2, y2, x3, y3, x4, y4, h, d1, d2, d3, d4
