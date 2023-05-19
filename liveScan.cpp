/*
Adept MobileRobots Robotics Interface for Applications (ARIA)
Copyright (C) 2004-2005 ActivMedia Robotics LLC
Copyright (C) 2006-2010 MobileRobots Inc.
Copyright (C) 2011-2015 Adept Technology, Inc.
Copyright (C) 2016 Omron Adept Technologies, Inc.

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with this program; if not, write to the Free Software
     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

If you wish to redistribute ARIA under different terms, contact
Adept MobileRobots for information about a commercial version of ARIA at
robots@mobilerobots.com or
Adept MobileRobots, 10 Columbia Drive, Amherst, NH 03031; +1-603-881-7960
*/


#include "Aria.h"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include <python3.5m/Python.h>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <string>


/** @example lasers.cpp example showing how to connect to laser rangefinders attached to the robot and retrieve data from them.  What kinds of laser or lasers are connected to the robot, and their parameters, are loaded from the robot's parameter file in the params directory. First the default for this type of mobile robot is loaded (e.g. p3dx-sh.p for a Pioneer 3 DX), followed by a parameter file specific to this individual robot (based on its 'name'), if such a parameter file exists.  Laser parameters may also be given as command line options, use the -help option to list them. This program will only connect to any lasers that have LaserAutoConnect true in the parameter file, or if the -connectLaser or -cl command-line options are given. You may need to edit the parameter file for your robot to set LaserAutoConnect to true.

   TODO: show how to force laser connect even if autoconnect is false.
 *
 * This program will work either with the MobileSim simulator or on a real
 * robot's onboard computer.  (Or use -remoteHost to connect to a wireless
 * ethernet-serial bridge.)
 */

FILE* intializePlot(){
	FILE *gnupipe = NULL;
	gnupipe = popen("gnuplot --persist", "w");
	fprintf(gnupipe, "set title \"Demo\"\nplot 'laser_map_data.tmp'\n");
	return gnupipe;
}
char *GnuCommands[] = {"set title \"Demo\"", "plot 'laser_map_data.tmp'"};

void putDataInFile(std::vector<double> X, std::vector<double> Y){
	FILE *fp = NULL;
	fp = fopen("laser_map_data.tmp", "a");


	for (int i = 0; i< X.size() ; i++){
			//ArLog::log(ArLog::Normal, "%f\n", X[i]);
			fprintf(fp, "%f %f\n", X[i], Y[i]);
			/*if (X[i] < 0 && Y[i] < 0){
				fprintf(fp, "-%d -%d\n", abs(int(X[i])), abs(int(Y[i])));
			}
			else if (X[i] < 0){
				fprintf(fp, "-%d %d\n", abs(int(X[i])), int(Y[i]));
			}
			else if (Y[i] < 0){
				fprintf(fp, "%d -%d\n", int(X[i]), abs(int(Y[i])));
			}else{
			fprintf(fp, "%d %d\n", int(X[i]), int(Y[i]));
			}*/
	}

	fclose(fp);
}

void turn(ArRobot* robot, double angle) { //give a negative angle to turn right and a positive angle to turn left
	ArLog::log(ArLog::Normal,"turn %f.2\n", angle);
    robot->lock();
    robot->setVel(0);
    robot->unlock();

    double startTh = robot->getTh();
    double targetTh = startTh + angle;

    //convert to -180 to 180
    if (targetTh > 180){
    	targetTh -= 360;
    }
    if (targetTh < -180){
       	targetTh += 360;
    }


    ArLog::log(ArLog::Normal,"Targert %f Start %f\n", targetTh,startTh);


    while (fabs(targetTh - robot->getTh()) > 1) {
    	//ArLog::log(ArLog::Normal,"I'm stuck");
    	double error = targetTh - robot->getTh();
        double rotVel = error / 2.0;
        //ArLog::log(ArLog::Normal,"Speed %f.2\n", rotVel);
        robot->lock();

        robot->setRotVel(rotVel);
        robot->unlock();

        ArUtil::sleep(10);
    }

    robot->lock();
    robot->setRotVel(0);
    robot->unlock();
	ArUtil::sleep(200);
}

void move(ArRobot * robot, double distance){
	printf("Moving %f.2\n", distance);
	robot->lock();
    robot->setRotVel(0);
    robot->setVel(200); // set forward velocity
    robot->unlock();
    double initialPoseX = robot->getX(); // store initial x position
    double initialPoseY = robot->getY(); // store initial y position
    double distanceMoved = 0.0;

    double startTh = robot->getTh();

    while(distanceMoved < distance){ // loop until the robot has moved the desired distance
        ArUtil::sleep(100); // wait for 0.1 second
        double currentPoseX = robot->getX(); // get current x position
        double currentPoseY = robot->getY(); // get current y position
        distanceMoved = sqrt(pow(currentPoseX - initialPoseX, 2) + pow(currentPoseY - initialPoseY, 2)); // calculate distance moved using Pythagorean theorem
    }

    printf("End %f Start %f\n", robot->getTh(), robot->getTh());

    robot->lock();
    robot->stop(); // stop the robot once it has moved the desired distance
    robot->unlock();

    ArUtil::sleep(1000);
}



int main(int argc, char **argv)
{
  Aria::init();
  ArArgumentParser parser(&argc, argv);
  parser.loadDefaultArguments();
  ArRobot robot;
  ArRobotConnector robotConnector(&parser, &robot);
  ArLaserConnector laserConnector(&parser, &robot, &robotConnector);



  // Connect to the robot, get some initial data from it such as type and name,
  // and then load parameter files for this robot.
  if(!robotConnector.connectRobot())
  {
    ArLog::log(ArLog::Terse, "lasersExample: Could not connect to the robot.");
    if(parser.checkHelpAndWarnUnparsed())
    {
        // -help not given
        Aria::logOptions();
        Aria::exit(1);
    }
  }



  if (!Aria::parseArgs() || !parser.checkHelpAndWarnUnparsed())
  {
    Aria::logOptions();
    Aria::exit(2);
    return 2;
  }

  ArLog::log(ArLog::Normal, "lasersExample: Connected to robot.");

  // Start the robot processing cycle running in the background.
  // True parameter means that if the connection is lost, then the
  // run loop ends.
  robot.runAsync(true);

  // Enable the motors, disable amigobot sounds
  robot.enableMotors();
  robot.enableSonar();


  // Connect to laser(s) defined in parameter files, if LaseAutoConnect is true
  // for the laser.
  // (Some flags are available as arguments to connectLasers() to control error behavior and to control which lasers are put in the list of lasers stored by ArRobot. See docs for details.)
  ArLog::log(ArLog::Normal, "lasersExample: Connecting to any lasers enabled for connect...");
  if(!laserConnector.connectLasers())
  {
    ArLog::log(ArLog::Terse, "lasersExample: Could not connect to configured lasers. Exiting.");
    Aria::exit(3);
    return 3;
  }
  ArLog::log(ArLog::Normal, "lasersExample: Connected to all lasers.\n");


  //test movement functions
  //move(&robot, 100);
  //turn(&robot, -70);



  // Log parameters related to the lasers
  ArLog::log(ArLog::Normal, "\nlasersExample: Robot parameters include information for %d lasers:", robot.getNumLasers());
  const ArRobotParams *params = robot.getRobotParams();
  for(size_t i = 1; i <= robot.getNumLasers(); ++i)
  {
    if(!robot.findLaser(i))
      continue;
    ArLog::log(ArLog::Normal, "\tlaser #%d: pose=(x:%d, y:%d, z:%d, th:%0.2f), powerOutput=%s",
      i,
      params->getLaserX(i), params->getLaserY(i), params->getLaserZ(i), params->getLaserTh(i),
      params->getLaserPowerOutput(i)
    );
  }
  ArLog::log(ArLog::Normal, "lasersExample: ArLaserConnector also using the following parameters (if values given, otherwise defaults for this laser type will be used):");
  laserConnector.logLaserData();
  ArLog::log(ArLog::Normal, "");

  // Allow some time to read laser data
  ArUtil::sleep(500);

  //clear file
	FILE *clear_fp = NULL;
	clear_fp = fopen("laser_map_data.tmp", "w");
	fclose(clear_fp);

	FILE *gnupipe = NULL;

	float convert = 3.14159/180;

	std::vector<double> X;
	std::vector<double> Y;
	bool initialPlot = true;
	bool initialBox = true;

	int scan_angle = 360;
	int n_steps = 5;
	int mostUsed[]= {0,0,0,0,0,0,0,0,0};
	double offset = -scan_angle/2;
	double angle_steps = scan_angle/n_steps;
    double inRadians = M_PI/180;
    bool roomScan=false, leftScan=false, rightScan=false, lagLeft=false, lagRight=false;
    double depthRoom=0;


	  //Implment Smart Movement
	  double left, frontDist, right, closeLeft, closeRight;

	std::vector<int> anglesToSearch;
	//create list of angles to increment
    for(int i = 0; i<241; ++i){
  	  double value = -120 + i;
  	  anglesToSearch.push_back(value);
    }


    // Initialize the Python interpreter
    Py_Initialize();

    // Add the current directory to the Python module search path
    PyObject* sysPath = PySys_GetObject((char*)"path");
    PyObject* currentDir = PyUnicode_FromString(".");
    PyList_Append(sysPath, currentDir);
    Py_DECREF(currentDir);

    // Import the Python script that performs the machine learning tasks
    PyObject* module = PyImport_ImportModule("simpleLines");
    if (module == NULL) {
        PyErr_Print();
        std::cerr << "Failed to import Python module." << std::endl;
        ArLog::log(ArLog::Terse, "lasersExample: Could not connect to configured lasers. Exiting.");
        Aria::exit(3);
        return 3;
    }

    // Get the function that performs the machine learning tasks
    PyObject* function = PyObject_GetAttrString(module, "simpleLines");
    if (function == NULL) {
        PyErr_Print();
        std::cerr << "Failed to get Python function." << std::endl;
        ArLog::log(ArLog::Terse, "lasersExample: Could not connect to configured lasers. Exiting.");
        Aria::exit(3);
        return 3;
    }


  // Print out some data from each connected laser.
  //for (int i=0; i< n_steps; i++){
    while(robot.isConnected()){

	//pipLocation[0]=robot.getX()/100;
	//pipLocation[1]=robot.getY()/100;
    int numLasers = 0;

 	  // Get a pointer to ArRobot's list of connected lasers. We will lock the robot while using it to prevent changes by tasks in the robot's background task thread or any other threads. Each laser has an index. You can also store the laser's index or name (laser->getName()) and use that to get a reference (pointer) to the laser object using ArRobot::findLaser().
	  robot.lock();
	  std::map<int, ArLaser*> *lasers = robot.getLaserMap();

    ArLog::log(ArLog::Normal, "lasersExample: ArRobot provided a set of %d ArLaser objects.", lasers->size());

	  for(std::map<int, ArLaser*>::const_iterator i = lasers->begin(); i != lasers->end(); ++i)
	  {
      int laserIndex = (*i).first;
      ArLaser* laser = (*i).second;
      if(!laser)
        continue;
      ++numLasers;

      laser->lockDevice();

      // The current readings are a set of obstacle readings (with X,Y positions as well as other attributes) that are the most recent set from teh laser.
      std::list<ArPoseWithTime*> *currentReadings = laser->getCurrentBuffer(); // see ArRangeDevice interface doc

      // The raw readings are just range or other data supplied by the sensor. It may also include some device-specific extra values associated with each reading as well. (e.g. Reflectance for LMS200)
      const std::list<ArSensorReading*> *rawReadings = laser->getRawReadings();

      //printf("%f, %f", laser->getStartDegrees(), laser->getEndDegrees());

      ArLog::log(ArLog::Normal,"%f, %f", laser->getStartDegrees(), laser->getEndDegrees());
      ArLog::log(ArLog::Normal, "X: %f Y: %f theta: %f\n", robot.getY(),-1*robot.getX(), robot.getTh());

	  for (size_t i = 0; i < 240; ++i)
	  {
		  // There is a utility to find the closest reading wthin a range of degrees around the laser, here we use this laser's full field of view (start to end)
		  // If there are no valid closest readings within the given range, dist will be greater than laser->getMaxRange().
		  double angle = 0;
		  //double dist = laser->currentReadingPolar(laser->getStartDegrees(), laser->getEndDegrees(), &angle);
		  double dist = laser->currentReadingPolar(anglesToSearch[i], anglesToSearch[i+1], &angle);



		if (2000.00 > dist){
																			  
			X.push_back(-1*(sin((robot.getTh()+(angle))*inRadians)*(dist/100.0))+(-1)*robot.getY()/100);
			Y.push_back((cos((robot.getTh()+(angle))*inRadians)*(dist/100.0))+robot.getX()/100);
			
		}


		 // Get distance to obstacles in front, left, and right directions
		 frontDist = laser->currentReadingPolar(-5, 5);
		 left = laser->currentReadingPolar(95, 100);
		 right = laser->currentReadingPolar(-100, -95);
		 closeLeft = laser->currentReadingPolar(25, 35);
		 closeRight = laser->currentReadingPolar(-35, -25);
	  }
	  laser->unlockDevice();
	  break;
    }
	//ArLog::log(ArLog::Normal, "X offset: %f Y offset: %f\n", -1*sin(robot.getTh()*(inRadians))*1.2, cos(robot.getTh()*(inRadians))*1.2);
	//ArLog::log(ArLog::Normal, "Cos degrees: %f radians: %f\n", cos(90), cos(90*inRadians));
	putDataInFile(X,Y);
	//ArUtil::sleep(4000);
	//clear data out
	X.clear();
	Y.clear();

	if (initialPlot){
		gnupipe = intializePlot();
		fflush(gnupipe);

		initialPlot = false;
	}else{
		fprintf(gnupipe, "replot\nrefresh\n");
		fflush(gnupipe);
	}

	robot.unlock();


	// Create a Python object that represents the data file
	PyObject* data_file = PyUnicode_FromString("laser_map_data.tmp");

	// Call the Python function with the data file as an argument
	PyObject* args = PyTuple_New(1);
	PyTuple_SetItem(args, 0, data_file);
	ArLog::log(ArLog::Normal, "I get hit");
	PyObject* result = PyObject_CallObject(function, args);
	ArLog::log(ArLog::Normal, "I finish");
	if (result == NULL) {
		PyErr_Print();
		std::cerr << "Failed to call Python function." << std::endl;
		return 1;
	}

	//Py_DECREF(result);
	//Py_DECREF(data_file);  // Decrement the reference count for the data_file object
	//Py_DECREF(args);

	ArUtil::sleep(200);

	// Constants for the obstacle avoidance algorithm
	  const double minSafeDistance = 1000; // Minimum safe distance to obstacles in mm
	  const double wallFollowDistance = 600; // Ideal distance to follow walls in mm
	  const int turnAngle = 45; // Turn angle in degrees

	  ArLog::log(ArLog::Normal, "front: %f left: %f right: %f, minSafe %f, wallFollow %f",frontDist,left,right,minSafeDistance,wallFollowDistance);

		 // If there's an obstacle in front of the robot
		 if (frontDist < minSafeDistance)
		 {
			 if (roomScan){
				 ArUtil::sleep(200);
				 turn(&robot, 180);
				 move(&robot, depthRoom);
				 depthRoom = 0;
				 if(leftScan){
					 turn(&robot, 90);
					 leftScan = false;
					 lagLeft = true;
				 }else if(rightScan){
					 turn(&robot, -90);
					 rightScan = false;
					 lagRight = true;
				 }
				 roomScan = false;
			 }else{
				 turn(&robot,180);
			 }

		 }
		 else if (left > 2000 && leftScan != true && rightScan != true && lagLeft != true ){
			 ArUtil::sleep(200);
			 turn(&robot, 90);
			 roomScan = true;
			 leftScan = true;
			 depthRoom = 0;
		 }else if (right > 2000 && rightScan != true && leftScan != true && lagRight != true ){
			 ArUtil::sleep(200);
			 turn(&robot, -90);
			 roomScan = true;
			 rightScan = true;
			 depthRoom = 0;
		 }else if (closeLeft<500){
			 ArUtil::sleep(200);
			 turn(&robot, -10);
		 }
		 else if (closeRight<500){
			 ArUtil::sleep(200);
			 turn(&robot, 10);
		 }
		 else{
			 ArUtil::sleep(200);
			 move(&robot, 600);
			 depthRoom += 600;
			 lagLeft = false;
			 lagRight = false;

		 }


    if(numLasers == 0)
      ArLog::log(ArLog::Normal, "lasersExample: No lasers.");
    else
      ArLog::log(ArLog::Normal, "");

    // Unlock robot and sleep for next loop.
    //robot.unlock();
    //ArUtil::sleep(200);
  }

  ArLog::log(ArLog::Normal, "lasersExample: exiting.");
  Aria::exit(0);
  return 0;
}
