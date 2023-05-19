
#include "Aria.h"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>


//#include "dos.h"

using namespace std;

/*void move(ArRobot * robot){
	  robot->lock();
	  robot->setRotVel(0);
	  robot->setVel(300);
	  robot->unlock();
	  ArUtil::sleep(3000);
	  robot->lock();
	  robot->stop();
	  robot->unlock();

}*/

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



/*void turn(ArRobot * robot, double angle){
	  printf("turn %f.2\n",angle);
	  double sleepPip;
	  double velPip;
	  if angle <= 91){
		  velPip = angle/3;
	  }else if(angle <=181){
		  velPip = angle/6;
	  }else if (angle <= 271){
		  velPip = angle / 9;
	  }else {
		  velPip = angle/12;
	  }
	  sleepPip = velPip *100;
	  
	  
	  robot->lock();  // turn left
	  robot->setVel(0);
	  robot->setRotVel(velPip); // turn 45 degrees/sec for 2 sec
	  robot->unlock();
	  ArUtil::sleep(sleepPip);
	  robot->lock();
	  robot->setRotVel(0);
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
}*/

void turn(ArRobot* robot, double angle) { //give a negative angle to turn right and a positive angle to turn left
    printf("turn %f.2\n", angle);
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
    
    
    printf("Targert %f Start %f\n", targetTh,startTh);
    
    
    while (fabs(targetTh - robot->getTh()) > 1) {
        double error = targetTh - robot->getTh();
        double rotVel = error / 2.0;
        //printf("Error: %f\n", error);
        
        //printf("Targert %f Current %f\n", targetTh, robot->getTh());
        
        //set max velocity so pip doesn't spin in circles 
        /*if(rotVel > 40){
        	rotVel = 40;
        }else if (rotVel < -40){
        	rotVel = -40;
        } else if (3 < error && error < 6){
        	rotVel = 2;
        }else if (-3 > error && error > -6){
        	rotVel = -2;
        }*/
        
        /*if (fabs(error) < 0.2){
        	robot->lock();
        	robot->setRotVel(0);
        	robot->unlock();
        	break;
        }*/
        
        robot->lock();
        
        
        //add check for error threshold

        
        robot->setRotVel(rotVel);
        robot->unlock();
        //}
        
        ArUtil::sleep(10);
    }

    robot->lock();
    robot->setRotVel(0);
    robot->unlock();
}

int*  getRange(ArRobot * robot, ArSonarDevice* sonar){
	  double left;
	  double right;
	  double left_10;
	  double right_10;
	  double left_50;
	  double right_50;
	  double left_30;
	  double right_30;
	  
	  double angle0 = 500;
	  double angle1 = 500;
	  double angle2 = 500;
	  double angle3 = 500;
	  double angle4 = 500;
	  double angle5 = 500;
	  double angle6 = 500;
	  double angle7 = 500;
	  
	  double test1;
	  double test2;
	  double test3;
	  double test4;
	  
	  double angle11 = 500;
	  double angle22 = 500;
	  double angle33= 500;
	  double angle44= 500;
	  
	  int* distance_list = new int[16];

	  for (int i=0; i<30; i++){
		  //range = sonar->currentReadingPolar(-10, 10) - robot->getRobotRadius();
		  //printf("#%i: %f\n", i, range);
		  left = sonar->currentReadingPolar(80, 91, &angle0) - robot->getRobotRadius();
		  left_50 = sonar->currentReadingPolar(45, 55, &angle1) - robot->getRobotRadius();
		  left_30 = sonar->currentReadingPolar(20,40, &angle2) - robot->getRobotRadius();
		  left_10 = sonar->currentReadingPolar(5, 15, &angle3) - robot->getRobotRadius();
		  right_10 = sonar->currentReadingPolar(-15, -5, &angle4) - robot->getRobotRadius();
		  right_30 = sonar->currentReadingPolar(-40, -20, &angle5) - robot->getRobotRadius();		  
		  right_50 = sonar->currentReadingPolar(-55, -45, &angle6) - robot->getRobotRadius();
		  right = sonar->currentReadingPolar(-91, -80, &angle7) - robot->getRobotRadius();
		  
		  //test1 = sonar->currentReadingPolar(45, 90, &angle11) - robot->getRobotRadius();
		  //test2 = sonar->currentReadingPolar(0, 45, &angle22) - robot->getRobotRadius();
		  //test3 = sonar->currentReadingPolar(-45, -0, &angle33) - robot->getRobotRadius();
		  //test4 = sonar->currentReadingPolar(-90, -45, &angle44) - robot->getRobotRadius();
		  
		  ArUtil::sleep(200);
	  }
	
	 //printf("Angles: %f ,%f ,%f ,%f ,%f ,%f ,%f %f \n", angle0, angle1, angle2, angle3, angle4, angle5, angle6, angle7);
	 //printf("%f ,%f ,%f ,%f\n", angle11, angle22, angle33, angle44);
	 distance_list[0] = left;
	 distance_list[1] = left_50;
	 distance_list[2] = left_30;
	 distance_list[3] = left_10;
	 distance_list[4] = right_10;
	 distance_list[5] = right_30;
	 distance_list[6] = right_50; 
	 distance_list[7] = right; 
	 
	 
	 distance_list[8] = angle0;
	 distance_list[9] = angle1;
	 distance_list[10] = angle2;
	 distance_list[11] = angle3;
	 distance_list[12] = angle4;
	 distance_list[13] = angle5;
	 distance_list[14] = angle6;
	 distance_list[15] = angle7;
	
	 //convert angles from -180:180 to 0:360 for cos and sin math functions 
	 for (int i=0; i<8; i++){
		 if (distance_list[8+i]<0){
			 distance_list[8+i] += 360;
		 }
	 }
			 
	 return distance_list;
}


double simpleGetRange(ArRobot * robot, ArSonarDevice* sonar){
	  double range;

	  for (int i=0; i<20; i++){

		  range = sonar->currentReadingPolar(-45, 45) - robot->getRobotRadius();

		  ArUtil::sleep(100);
	  }
	 return range;
}

void boxMovement(ArRobot * robot, ArSonarDevice* sonar, vector<double>& location)
{	
	double range = simpleGetRange(robot, sonar);
		printf("Location: %f %f %f\n", location[0],location[1],location[2]);
		//if pip can't move forward, it will instead turn right
		/*if(range < 450)
		{
			srand(time(NULL));
			int randomAngle = rand() % 360; //generate random angle 
			turn(robot, randomAngle);
			location[2] = location[2] + double(-randomAngle); //update that angle
			printf("Random angle: %f\n", -randomAngle);
			printf("Random angle: %f\n", -location[2]);
			
			//adds a right turn to the moves array
			//moves[i] = 1;
			ArUtil::sleep(1000);
		}
		else
		{*/
		location[2] = 180;
		move(robot, 300);
		//move function is set to 100 and location 2 gives relative angle
		//location[0] = (100 *0.331)*sin((M_PI/180)*location[2]); //update X
		//location[1] = (100 *0.331)*cos((M_PI/180)*location[2]); //update Y
		location[0] = (100/2 *0.331)*sin((M_PI/180)*180); //update X
		location[1] = (100/2 *0.331)*cos((M_PI/180)*180); //update Y
			
			//adds a 'move forward' to moves array
			//moves[i] = 2;
		//}
		//range = simpleGetRange(robot, sonar);
	//}
}

//has pip 'pace' around the enclosure
void pace(ArRobot * robot, ArSonarDevice* sonar, vector<double>& location)
{	
	for(int i = 0; i<3 ; i++){
	double range = simpleGetRange(robot, sonar);
		printf("Location: %f %f %f\n", location[0],location[1],location[2]);
		//if pip can't move forward, it will instead turn right
		if(range < 450)
		{
			srand(time(NULL));
			int randomAngle = rand() % 360; //generate random angle 
			turn(robot, randomAngle);
			location[2] = location[2] + double(-randomAngle); //update that angle
			printf("Random angle: %f\n", -randomAngle);
			printf("Random angle: %f\n", -location[2]);
			
			//adds a right turn to the moves array
			//moves[i] = 1;
			ArUtil::sleep(1000);
		}
		else
		{
			
			move(robot, 300);
			//move function is set to 100 and location 2 gives relative angle
			location[0] = (400/2 *0.331)*sin((M_PI/180)*location[2]); //update X
			location[1] = (400/2 *0.331)*cos((M_PI/180)*location[2]); //update Y
			
			//adds a 'move forward' to moves array
			//moves[i] = 2;
		}
		range = simpleGetRange(robot, sonar);
	}
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

std::pair<double, double> leastSquaresBestFitLine(std::vector<std::pair<std::pair<double, double>, double> > Vector)
{
	double xSum = 0;
	double ySum = 0;
	double slope;
	double yInt;
	for(double c=0; c<Vector.size();c++)
	{
		xSum = xSum + Vector[c].first.first;
		ySum = ySum + Vector[c].first.second;
	}
	double xMean = xSum / 10;
	double yMean = ySum / 10;
	//get the slope. for every value: ( (xVal - xMean)(yVal - yMean) ) / (xVal - xMean)^2
	double topCalc = 0;
	double botCalc = 0;

	for(int d=0; d<10;d++)
	{
		double xVal = Vector[d].first.first;
		double yVal = Vector[d].first.second;

		topCalc = topCalc + ((xVal - xMean)*(yVal - yMean));
		botCalc = botCalc + pow((xVal -xMean), 2);
	}
	if(botCalc == 0)
	{
		printf("botcalc is 0 \n");
		botCalc = 1;

	}
	else
	{
		slope = topCalc / botCalc;
	}
	//get y intercept. yIntercept = yMean - (slope)(xMean)
	yInt = yMean - (slope * xMean);
	std::pair<double, double> result;
	result = std::make_pair(slope, yInt);
	return result;

}

void putDataInFile(std::vector<double> X, std::vector<double> Y){
	FILE *fp = NULL;
	fp = fopen("map_data.tmp", "a");
	
	
	for (int i = 0; i< X.size() ; i++){
			
			if (X[i] < 0 && Y[i] < 0){
				fprintf(fp, "-%d -%d\n", abs(int(X[i])), abs(int(Y[i])));
			}
			else if (X[i] < 0){
				fprintf(fp, "-%d %d\n", abs(int(X[i])), int(Y[i]));
			}
			else if (Y[i] < 0){
				fprintf(fp, "%d -%d\n", int(X[i]), abs(int(Y[i])));
			}else{
			fprintf(fp, "%d %d\n", int(X[i]), int(Y[i]));	
			}
	}
	
	fclose(fp);
}
//return file and gnupipe
FILE* intializePlot(){
	FILE *gnupipe = NULL;
	gnupipe = popen("gnuplot --persist", "w");	
	fprintf(gnupipe, "set title \"Demo\"\nplot 'map_data.tmp'\n");
	return gnupipe;
}
char *GnuCommands[] = {"set title \"Demo\"", "plot 'map_data.tmp'"};
//I think we need to have a reader where gnupipe is?? instead of just a write function so it still has access to the plot
/*void updatePlot(){
	FILE *gnupipe = NULL;
	gnupipe = popen("gnuplot --persist", "w");	
	char *GnuCommands[] = {"refresh"};
	for (int i = 0; i<2; i++){
			fprintf(gnupipe, "%s\n", GnuCommands[i]);
	}
	pclose(gnupipe);
}*/


int main(int argc, char **argv)
{
  
  Aria::init();
  ArArgumentParser parser(&argc, argv);
  parser.loadDefaultArguments();
  ArRobot robot;
  ArSonarDevice sonar;
  ArRobotConnector robotConnector(&parser, &robot);
  ArLaserConnector laserConnector(&parser, &robot, &robotConnector);
  
  if(!laserConnector.connectLasers())
  {
    ArLog::log(ArLog::Normal, "Warning: unable to connect to requested lasers, will wander using robot sonar only.");
  }
  
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
  
    
   //Intialize Array
    int* distance_list;

    float convert = 3.14159/180;
    
    vector<double> X;
    vector<double> Y;
    bool initialPlot = true;
    bool initialBox = true;
    
    //clear file   
    FILE *clear_fp = NULL;
    clear_fp = fopen("map_data.tmp", "w");
    fclose(clear_fp);

    FILE *gnupipe = NULL;
    
    vector<double> pipLocation(3,0.0); //x,y,theata
    double currentAngle = 0;
    
  // motion commands here
    while(!cb.done){

    //while(getRange(&robot,&sonar)>100 && !cb.done){
    int scan_angle = 180;
	int n_steps = 6;
	int mostUsed[]= {0,0,0,0,0,0,0,0,0};
	double offset = -scan_angle/2;
	
	
    while(!cb.done){
		//initialize
		int* distance_list;
	    float convert = 3.14159/180;

	    vector<double> X;
	    vector<double> Y;

	    X.clear();
	    Y.clear();

		double angle_steps = scan_angle/n_steps;
		double relative_angle = 0;
		//pace(&robot,&sonar,pipLocation);

		//turn it all the way to the left to save time and do one sweep
		//turn(&robot, scan_angle);
		//move(&robot, 300);

		turn(&robot, scan_angle/2);


		//turn(&robot,270)
		//printf("Turned: %d", scan_angle/2 -360);

		
		
		//do a 180 scan to get more points
		for (int i=0; i< n_steps+1; i++){

			distance_list = getRange(&robot,&sonar);

			//0.331 is the speed of sound in meters/milisecond
			//double distance = (distance_list[3] / 2) * 0.331;
			//account for relative distance for xy of robot
			//iterate over the readings if one is too bad then we leave it out
			double sonarThreshold = 800;
			
			if (robot.getTh() < 0){
				currentAngle = robot.getTh() + 360;
			}else{
				currentAngle = robot.getTh();
			}
			
			pipLocation[0]=robot.getX()/100;
			pipLocation[1]=robot.getY()/100;
			

			for (int i=0; i<8; i++){
				if (distance_list[i] > sonarThreshold || distance_list[i+8] == 500){ //check for 500 bc set impossible angle to 500 incase no object observed
					distance_list[i] = -42; //set to negative because it's impossible for sonar

					//printf("%i, \n",i);
				}
				else{
					mostUsed[i] += 1;
					switch(i){
					case 0:
						
						X.push_back(-1*(pipLocation[0]+(sin(convert*((currentAngle+distance_list[i+8])))*(distance_list[i]/2 *0.331))));
						Y.push_back(pipLocation[1]+(cos(convert*((currentAngle+distance_list[i+8])))*(distance_list[i]/2 *0.331)));
						break;
					case 1:
						//X.push_back(-12.5+(sin(convert*((currentAngle+50)*(-1)))*(distance_list[i]/2 *0.331)));
						//Y.push_back(5+(cos(convert*((currentAngle+50)*(-1)))*(distance_list[i]/2 *0.331)));
						X.push_back(-1*(pipLocation[0]+((sin(convert*((currentAngle+distance_list[i+8])))*(distance_list[i]/2 *0.331)))));
						Y.push_back(pipLocation[1]+((cos(convert*((currentAngle+distance_list[i+8])))*(distance_list[i]/2 *0.331))));
						break;
					case 2:
						//X.push_back(-7.5+(sin(convert*((currentAngle+30)*(-1)))*(distance_list[i]/2 *0.331)));
						//Y.push_back(8+(cos(convert*(30*(-1)))*(distance_list[i]/2 *0.331)));
						X.push_back(-1*(pipLocation[0]+((sin(convert*((currentAngle+distance_list[i+8])))*(distance_list[i]/2 *0.331)))));
						Y.push_back(pipLocation[1]+((cos(convert*((currentAngle+distance_list[i+8])))*(distance_list[i]/2 *0.331))));
						break;
					case 3:
						//X.push_back(-2.5+(sin(convert*((currentAngle+10)*(-1)))*(distance_list[i]/2 *0.331)));
						//Y.push_back(10+(cos(convert*((currentAngle+10)*(-1)))*(distance_list[i]/2 *0.331)));
						X.push_back(-1*(pipLocation[0]+(sin(convert*((currentAngle+distance_list[i+8])))*(distance_list[i]/2 *0.331))));
						Y.push_back(pipLocation[1]+(cos(convert*((currentAngle+distance_list[i+8])))*(distance_list[i]/2 *0.331)));
						break;
					case 4:
						X.push_back(-1*(pipLocation[0]+(sin(convert*((currentAngle+distance_list[i+8])))*(distance_list[i]/2 *0.331))));
						Y.push_back(pipLocation[1]+(cos(convert*((currentAngle+distance_list[i+8])))*(distance_list[i]/2 *0.331)));
						break;
					case 5:
						X.push_back(-1*(pipLocation[0]+(sin(convert*((currentAngle+distance_list[i+8])))*(distance_list[i]/2 *0.331))));
						Y.push_back(pipLocation[1]+(cos(convert*((currentAngle+distance_list[i+8])))*(distance_list[i]/2 *0.331)));
						break;
					case 6:
						X.push_back(-1*(pipLocation[0]+(sin(convert*((currentAngle+distance_list[i+8])))*(distance_list[i]/2 *0.331))));
						Y.push_back(pipLocation[1]+(cos(convert*((currentAngle+distance_list[i+8])))*(distance_list[i]/2 *0.331)));
						break;
					case 7:
						X.push_back(-1*(pipLocation[0]+(sin(convert*((currentAngle+distance_list[i+8])))*(distance_list[i]/2 *0.331))));
						Y.push_back(pipLocation[1]+(cos(convert*((currentAngle+distance_list[i+8])))*(distance_list[i]/2 *0.331)));
						break;

					}
				}
				
			}
			printf("Current position: %f %f %f\n", pipLocation[0],pipLocation[1],pipLocation[2]+currentAngle);
			printf("Gyroscope Poistion(x,y,theta): %f %f %f\n", robot.getX(),robot.getY(),robot.getTh());
					

			//turn(&robot,-30);
			putDataInFile(X,Y);
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
			//on the last scan do not turn 
			if (i< n_steps){
				  
				turn(&robot, -angle_steps);
			}
			
			for (int i=0; i<8; i++){
				printf("For %i: %i\n" ,i,mostUsed[i]);

			}
			//ArUtil::sleep(4000);
		}
		
		printf("Current Angle: %f\n", pipLocation[2]);		
		  
		turn(&robot, scan_angle/2);
		//currentAngle = 0;
		
		if (initialBox){
			  
			turn(&robot, 180);
			initialBox = false;
		}
		
		
		boxMovement(&robot,&sonar,pipLocation);

		printf("Current position: %f %f %f\n", pipLocation[0],pipLocation[1],pipLocation[2]);
		printf("Gyroscope Poistion(x,y,theta): %f %f %f\n", robot.getX(),robot.getY(),robot.getTh());
		
		//putDataInFile(X,Y);
		
		//printf("Data in file");
		
		//plotMyData();
		/*FILE* gnupipe = intializePlot();
					fflush(gnupipe);*/

					
					//printf("Data in file\n");
					/*if (initialPlot){
						plotMyData();
						initialPlot = false;
					}else{
						updatePlot();
					}
					
					turn(&robot,-angle_steps);
					*/
		
			//printf("%f\n", cos(90*convert));
	    /*
		double n = X.size();

	    printf("%f vectors used\n", n);

		double sum_X = 0, sum_Y = 0;

		for (int i=0; i<n; i++){
			sum_X += X[i];
			printf("%f, ", X[i]);
		}
		printf("X \n");
		for (int i=0; i<n; i++){
			sum_Y += Y[i];
			printf("%f, ", Y[i]);
		}
		printf("Y \n");
		double xbar = sum_X/n, ybar = sum_Y/n;

		vector<double> A; //x-xbar
		vector<double> B; //y-ybar
		vector<double> C; //estimated Y

		A.clear();
		B.clear();
		C.clear();

		//X-xbar, y-ybar
		for (int i=0; i<n; i++){
			A.push_back(X[i]-xbar);
			B.push_back(Y[i]-ybar);
		}

		//calculate slope and intercept
		sum_X = 0;
		sum_Y = 0;
		for (int i=0; i<n; i++){ //sqaures values
			sum_X += (A[i] *A[i]); //summation of (x-xbar)^2
			sum_Y += (A[i] *B[i]);//summation of (x-xbar)*(y-ybar)
		}

		double slope = sum_Y/sum_X; //slope = sum(x-xbar)(y-ybar)/(sum(x-xbar))^2
		double intercept = ybar - (slope*xbar);


		//calculate R^2
		//predicted Y-values
		for (int i=0; i<n; i++){
			C.push_back((slope*X[i])+intercept);
		}
		double sum_1 = 0;
		double sum_2 = 0;
		for (int i=0; i<n; i++){
			sum_1 = sum_1 + ((C[i]-ybar)*(C[i]-ybar)); //ypred-ybar^2
			sum_2 = sum_2 + ((Y[i]-ybar)*(Y[i]-ybar)); //yactual-ybar^2
		}

		double R = 1 - (sum_1/sum_2);

		printf("%f\n", sum_1);
		printf("%f\n", sum_2);

		printf("Slope: %f\n", slope);
		printf("Intercept: %f\n", intercept);
		printf("R^2: %f\n", R);
		*/

		/*glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
		glutInitWindowPosition(80,80);
		glutInitWindowSize(700,700);
		glutCreateWindow("test");*/

		//f(x) = intercept + slope*x;

		//plot();
		
        
    	//printf("Total Range: %f\n", );
    	//ArUtil::sleep(2500);
    	//delay(15000);
    	//moves[numMoves]++;
        //move(&robot);
        //turn(&robot,-30);


    }
    
	//close pipe when terminate program
	fprintf(gnupipe, "q\n");	
	pclose(gnupipe);
    
    if (!cb.done){
      numMoves++;
      //printf("Total Range: %f\n", getRange(&robot,&sonar));
      printf("wall turn\n");
      turn(&robot,-90); // turn right
      //meauring
      ArUtil::sleep(10000);
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
		   move(&robot, 300);
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

