
#include "Aria.h"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>

//#include "dos.h"

using namespace std;

void move(ArRobot * robot){
	  robot->lock();
	  robot->setRotVel(0);
	  robot->setVel(100);
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

int*  getRange(ArRobot * robot, ArSonarDevice* sonar){
	  double left;
	  double right;
	  double left_10;
	  double right_10;
	  double left_50;
	  double right_50;
	  double left_30;
	  double right_30;
	  
	  int* distance_list = new int[8];

	  for (int i=0; i<100; i++){
		  //range = sonar->currentReadingPolar(-10, 10) - robot->getRobotRadius();
		  //printf("#%i: %f\n", i, range);
		  left = sonar->currentReadingPolar(80, 91) - robot->getRobotRadius();
		  left_50 = sonar->currentReadingPolar(45, 55) - robot->getRobotRadius();
		  left_30 = sonar->currentReadingPolar(20,40) - robot->getRobotRadius();
		  left_10 = sonar->currentReadingPolar(5, 15) - robot->getRobotRadius();
		  right_10 = sonar->currentReadingPolar(-15, -5) - robot->getRobotRadius();
		  right_30 = sonar->currentReadingPolar(-40, -20) - robot->getRobotRadius();		  
		  right_50 = sonar->currentReadingPolar(-55, -45) - robot->getRobotRadius();
		  right = sonar->currentReadingPolar(-91, -80) - robot->getRobotRadius();
		  
		  ArUtil::sleep(200);
	  }
	

	 distance_list[0] = left;
	 distance_list[1] = left_50;
	 distance_list[2] = left_30;
	 distance_list[3] = left_10;
	 distance_list[4] = right_10;
	 distance_list[5] = right_30;
	 distance_list[6] = right_50; 
	 distance_list[7] = right; 
	 
			 
	 return distance_list;
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

void putDataInFile(vector<double> X, vector<double> Y){
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
  //ArLaserConnector laserConnector(&parser, &robot, &robotConnector);
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

    //clear file   
    FILE *clear_fp = NULL;
    clear_fp = fopen("map_data.tmp", "w");
    fclose(clear_fp);

    FILE *gnupipe = NULL;

  // motion commands here
    while(!cb.done){

    //while(getRange(&robot,&sonar)>100 && !cb.done){
    int scan_angle = 180;
	int n_steps = 4;
	int mostUsed[]= {0,0,0,0,0,0,0,0,0};

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


		distance_list = getRange(&robot,&sonar);

		//0.331 is the speed of sound in meters/milisecond
		//double distance = (distance_list[3] / 2) * 0.331;
		//account for relative distance for xy of robot
		//iterate over the readings if one is too bad then we leave it out
		double sonarThreshold = 2000;

		double currentAngle = 0;

		for (int i=0; i<8; i++){
			if (distance_list[i] > sonarThreshold){
				distance_list[i] = -42; //set to negative because it's impossible for sonar

				//printf("%i, \n",i);
			}
			else{
				mostUsed[i] += 1;
				switch(i){
				case 0:
					X.push_back(sin(convert*((currentAngle+90)*(-1)))*(distance_list[i]/2 *0.331));
					Y.push_back(cos(convert*((currentAngle+90)*(-1)))*(distance_list[i]/2 *0.331));
					break;
				case 1:
					//X.push_back(-12.5+(sin(convert*((currentAngle+50)*(-1)))*(distance_list[i]/2 *0.331)));
					//Y.push_back(5+(cos(convert*((currentAngle+50)*(-1)))*(distance_list[i]/2 *0.331)));
					X.push_back((sin(convert*((currentAngle+50)*(-1)))*(distance_list[i]/2 *0.331)));
					Y.push_back((cos(convert*((currentAngle+50)*(-1)))*(distance_list[i]/2 *0.331)));
					break;
				case 2:
					//X.push_back(-7.5+(sin(convert*((currentAngle+30)*(-1)))*(distance_list[i]/2 *0.331)));
					//Y.push_back(8+(cos(convert*(30*(-1)))*(distance_list[i]/2 *0.331)));
					X.push_back((sin(convert*((currentAngle+30)*(-1)))*(distance_list[i]/2 *0.331)));
					Y.push_back((cos(convert*(30*(-1)))*(distance_list[i]/2 *0.331)));
					break;
				case 3:
					//X.push_back(-2.5+(sin(convert*((currentAngle+10)*(-1)))*(distance_list[i]/2 *0.331)));
					//Y.push_back(10+(cos(convert*((currentAngle+10)*(-1)))*(distance_list[i]/2 *0.331)));
					X.push_back((sin(convert*((currentAngle+10)*(-1)))*(distance_list[i]/2 *0.331)));
					Y.push_back((cos(convert*((currentAngle+10)*(-1)))*(distance_list[i]/2 *0.331)));
					break;
				case 4:
					X.push_back(sin(convert*((currentAngle+10)))*(distance_list[i]/2 *0.331));
					Y.push_back(cos(convert*((currentAngle+10)))*(distance_list[i]/2 *0.331));
					break;
				case 5:
					X.push_back(sin(convert*((currentAngle+30)))*(distance_list[i]/2 *0.331));
					Y.push_back(cos(convert*((currentAngle+30)))*(distance_list[i]/2 *0.331));
					break;
				case 6:
					X.push_back(sin(convert*((currentAngle+50)))*(distance_list[i]/2 *0.331));
					Y.push_back(cos(convert*((currentAngle+50)))*(distance_list[i]/2 *0.331));
					break;
				case 7:
					X.push_back(sin(convert*((currentAngle+90)))*(distance_list[i]/2 *0.331));
					Y.push_back(cos(convert*((currentAngle+90)))*(distance_list[i]/2 *0.331));
					break;

				}
			}


			//turn(&robot,-30);
			putDataInFile(X,Y);
			
			if (initialPlot){
				gnupipe = intializePlot();
				fflush(gnupipe);
				
				initialPlot = false;
			}else{
				fprintf(gnupipe, "replot\nrefresh\n");
				fflush(gnupipe);
			}
			

			Aria::exit(0);
			return 0;

			turn(&robot, 0);
			turn(&robot, -angle_steps);
			
			for (int i=0; i<8; i++){
				printf("For %i: %i\n" ,i,mostUsed[i]);

			}
			ArUtil::sleep(4000);
		}
		

		
		//close pipe when terminate program
		fprintf(gnupipe, "q\n");	
		pclose(gnupipe);
		
		
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
    	moves[numMoves]++;
        //move(&robot);
        turn(&robot,-30);


    }

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

