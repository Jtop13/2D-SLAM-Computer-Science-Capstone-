#include "Aria.h"


/*double getRange(ArRobot * robot, ArLaser* laser){
	  double range;
	  double* angle;
	  int * list = new int[2];

	  for (int i=0; i<20; i++){
		  range = robot->getLaserMap();

		  printf("Range: %f, Angle: %f\n", range, angle);
		  ArUtil::sleep(5000);
	  }


	 return range;
}*/

/*void get_range(ArRobot *robot, ArLaser laser) {
    double data;
    double angle0 = 10000;
    data = laser -> currentReadingPolar(-135.0, 135.0, &angle0) - robot->getRobotRadius();
    
    printf("Nearest obstacle at angle %.2f degrees and distance %.2f meters.\n", data, angle0);
    
}*/


int main(int argc, char** argv)
{
  Aria::init();

  ArArgumentParser parser(&argc, argv);
  parser.loadDefaultArguments();
  ArRobot robot;
  ArRobotConnector robotConnector(&parser, &robot);

  // Try connecting to the robot.
  if (!robotConnector.connectRobot())
  {
    // Error!
    ArLog::log(ArLog::Terse, "Error, could not connect to robot.\n");
    Aria::exit(1);
  }

  // Try connecting to the laser.
  ArLaserConnector laserConnector(&parser, &robot, &robotConnector);
  if (!laserConnector.connectLasers())
  {
    ArLog::log(ArLog::Terse, "Error, could not connect to laser.\n");
    Aria::exit(2);
  }

  // Run the robot in its own thread.
  robot.runAsync(true);

  // Get the laser.
  ArLaser* laser = robot.findLaser(1);

  // Set the range device to use the first laser.
  robot.lock();
  robot.addRangeDevice(laser);
  robot.unlock();
  
  //get_range(&robot, &laser);
  double angle = 10000;
  double distance = 1000;
  //ArRobot::checkRangeDevicesCurrentPolar(-135.0, 135.0, &angle0);
  robot.checkRangeDevicesCurrentPolar(-135.0, 135.0, &angle);
  
  
  printf("Nearest obstacle at angle %.2f degrees and distance %.2f meters.\n", angle, distance);
  ArUtil::sleep(1000); // sleep for 100 milliseconds
  
  
  
  
  
  double angle = 0.0;
  double range = 0.0;

  

  printf("Angle: %.2f, Range: %.2f\n", angle, range);


  
  
  ArUtil::sleep(5000);

  /*
  // Choose the angles at which to get range readings from the laser.
  int angles[] = {0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240, 255, 270, 285, 300, 315, 330, 345};
  int numAngles = sizeof(angles)/sizeof(angles[0]);

  // Get range readings from the laser at the chosen angles.
  std::list<ArPoseWithTime*> * readings = laser->getRangeDeviceReadings();
  int distance_list[numAngles];
  int i = 0;
  for(int angle : angles){
    distance_list[i] = readings->front()->findDistanceTo(ArPose(angle,0,0));
    readings->pop_front();
    i++;
  }

  // Print out the range readings.
  for(int distance : distance_list){
    printf("%d\n", distance);
  }
	*/
  // Disconnect the laser.
  //laserConnector.disconnect();

  // Disconnect the robot.
  robot.disconnect();

  // Terminate Aria.
  Aria::exit(0);
}
