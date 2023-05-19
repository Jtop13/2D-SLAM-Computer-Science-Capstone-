#include "Aria.h"

void get_range(ArRobot *robot, ArRangeDevice *laser) {
    double data;
    double angle0 = 10000;
    data = laser -> currentReadingPolar(-135.0, 135.0, &angle0) - robot->getRobotRadius();
    
    printf("Nearest obstacle at angle %.2f degrees and distance %.2f meters.\n", data, angle0);
    
}

int main(int argc, char **argv) {
	Aria::init();
	ArArgumentParser parser(&argc, argv);
	parser.loadDefaultArguments();
	ArRobot robot;
    ArRangeDevice laser;
    
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
    
    ArKeyHandler keyHandler;
    Aria::setKeyHandler(&keyHandler);
    robot.attachKeyHandler(&keyHandler);
    robot.addRangeDevice(&sonar);
    robot.runAsync(true);
    
    robot.lock();
    robot.addRangeDevice(&laser);
    robot.unlock();
    

    while (true) {
        get_range(&robot, &laser);
        //printf("Nearest obstacle at angle %.2f degrees and distance %.2f meters.\n", angle, distance);
        ArUtil::sleep(1000); // sleep for 100 milliseconds
    }

    robot.lock();
    robot.stop();
    robot.unlock();
    
    Aria::exit(0);
    return 0;
}
