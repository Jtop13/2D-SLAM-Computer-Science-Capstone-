PIP notes from Jack Topper (2023)


The following is my 2D-SLAM capstone project using robot PIP:

The following is my 2D-SLAM project using robot PIP.

In order to compile the c++ code with python3.5m run make file in this folder with:

make -f mypyMakefile


The python file, simpleLines.py, including is just an example of what I used.
In order for the file to be functional and actually ran you
need to place that file on pips computer

The python file draws the map and the c++ code calls on
the python file. 





TIPS:

Using the Gyroscope:
	robot.getX() is Y (mm) as in moving forward (+) and backward (-).
	robot.getY() is X (mm) as in moving left (+) and right(-).

	In my code I make these changes and multiply robot.getY() by (-1) since left 	is usually negative on a grid format.

	
	robot.getTh() returns the angle (degrees) that PIP is facing and that 	ranges 	from:(-180, 180)

	Don’t forget to convert to radians Cmaths sin and cos is not in degrees

Movement:


See movement functions I created to make PIP move forward and turn. There are no movement functions that can take in a distance rather just a velocity set function.

	 
See comments I made on liveScan.cpp to understand more problems I have addressed. Any further questions you can reach out to me


