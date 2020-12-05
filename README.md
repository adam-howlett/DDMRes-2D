# DDMRes-2D

This project contains the release & source code for the  implementation of a Discrete-Dual Minimal Residual Finite Element Method, as written & implemented by me in service of my dissertation project, details of which can be found on
[the dissertation project page on my website](adam-howlett.github.io/dissertation.html)
All code was written by me, unless otherwise acknowledged.

Source code for the project can be found in /src
Project Files for Codeblocks can be found in /ide

Pictures generated for the purposes of the dissertation can be found in /img


# Running The Program
The program file, "DDMRES-2d.exe" can be found in /release 

The program will automatically plot the solution to matlab for reference if it is installed on your machine. If this is not the case, a solution file will still be generated, & error values generated agianst the exact solution.

The final function in the list (e^x(cos(pi\*x)) is a border term only, and can be translated through the mesh with an arbitrary angle determined by the slider. In this case, streamlines are apparent along this direction.

Orienting the plot to look down these streamlines will generate a straight line, as in /img/streamlineGeneralAngle

The two different refinement schemes afford different benefits. The red refinement scheme is good 'general purpose' and can be applied to any mesh. The vertical line refinement scheme may only be applied when the field is "Straight up", but is more efficient.

Mesh Size can be controlled with the bottom slider, and effectively increases the resolution and accuracy of the result. since we have an O(h) convergence, each doubling of the system size should halve the absolute error.
