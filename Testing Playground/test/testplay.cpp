
#include "Aria.h"

void move(ArRobot * robot){
	  robot->lock();
	  robot->setRotVel(0);
	  robot->setVel(200);
	  robot->unlock();
	  ArUtil::sleep(2000);
	  robot->lock();
	  robot->stop();
	  robot->unlock();

}




void turn(ArRobot * robot, double angle){
	  printf("turn %f.2\n",angle);
	  robot->lock();  // turn left
	  robot->setVel(0);
	  robot->setRotVel(angle/2); // turn 45 degrees/sec for 2 sec
	  robot->unlock();
	  ArUtil::sleep(2000);
	  robot->lock();
	  robot->setRotVel(0);
	  robot->unlock();
}



double getRange(ArRobot * robot, ArSonarDevice* sonar){
	  double range;

	  for (int i=0; i<20; i++){

		  range = sonar->currentReadingPolar(-70, 70) - robot->getRobotRadius();
		  //printf("Jack My Range: %f", range);


		  ArUtil::sleep(100);
	  }
	 return range;
}

class MyCB{
  ArRobot* myrobot;
  ArFunctorC<MyCB> myhomeCB;


public:
  bool done = false;

  MyCB(ArRobot* robot) :
    myrobot(robot),
    myhomeCB(this, &MyCB::goHome)

  {
  }

  void addKeyHandlers(ArRobot *robot)
  {
    ArKeyHandler *keyHandler = Aria::getKeyHandler();
    if(keyHandler == NULL)
    {
      keyHandler = new ArKeyHandler();
      Aria::setKeyHandler(keyHandler);
      robot->attachKeyHandler(keyHandler);
    }

    keyHandler->addKeyHandler('h', &myhomeCB);
  }

  void goHome()
  {
    printf("goHome\n");
    done = true;

  }
};



int main(int argc, char **argv)
{
  Aria::init();
  ArArgumentParser parser(&argc, argv);
  parser.loadDefaultArguments();
  ArRobot robot;
  ArSonarDevice sonar;
  ArRobotConnector robotConnector(&parser, &robot);
  //Jack  
  ArLaserConnector laserConnector(&parser, &robot, &robotConnector);
  ArFunctorC<MyCB> myUpCB;
  int moves[100];
  int numMoves = 0;
  for (int i=0; i<100; i++){
	  moves[i] = 0;
  }


  // Connect to the robot, get some initial data from it such as type and name,
  // and then load parameter files for this robot.
  if(!robotConnector.connectRobot())
  {
    ArLog::log(ArLog::Terse, "test: Could not connect to the robot.");
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
    Aria::exit(1);
  }

  ArLog::log(ArLog::Normal, "test: Connected to robot.");

  ArKeyHandler keyHandler;
  Aria::setKeyHandler(&keyHandler);
  robot.attachKeyHandler(&keyHandler);
  robot.addRangeDevice(&sonar);
  robot.runAsync(true);


  //robot.addAction(&recover, 100);
  //  robot.addAction(&go, 50);


    // Enable the motors, disable amigobot sounds
    robot.enableMotors();
    robot.enableSonar();

    MyCB cb(&robot);
    cb.addKeyHandlers(&robot);

  // motion commands here
    while(!cb.done){

    while(getRange(&robot,&sonar)>600 && !cb.done){
		moves[numMoves]++;
        move(&robot);

    }

    if (!cb.done){
      numMoves++;
      printf("wall turn\n");

      //Jack before we turn let's get each sonar reading a couple of times
      //printf("My radius: %f", )

      turn(&robot,-90); // turn right
    }
    }

  // go home

   for (int i=0; i<=numMoves; i++){
	   printf("moves %d: %d\n",i,moves[i]);
   }

   // turn around
   turn(&robot,90);
   turn(&robot,90);
   for (int i=numMoves; i>=0; i--){
	   for (int j=0; j<moves[i]; j++){
		   move(&robot);
	   }
	   if (i>0)
	     turn(&robot,90);
   }

   // turn around
   turn(&robot,90);
   turn(&robot,90);


  robot.lock();
  robot.stop();
  robot.unlock();


  // Robot disconnected or time elapsed, shut down

  Aria::exit(0);
  return 0;
}

