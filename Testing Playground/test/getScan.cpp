//getScan(180, 6)
vector<double> vector<double> getScan(int scan_angle, int n_steps){

	//initialize
	int* distance_list;
    float convert = 3.14159/180;

    vector<double> X;
    vector<double> Y;

    X.clear();
    Y.clear();
    	
	double angle_steps = scan_angle/n_steps;
	double relative_angle = 0;

	//turn it all the way to the left to save time and do one sweep
	turn(&robot, scan_angle/2);


	double offset = -scan_angle/2;
	
	//do a 180 scan to get more points
	for (int i=0; i< n_steps; i++){

		distance_list = getRange(&robot,&sonar);

		//0.331 is the speed of sound in meters/milisecond
		//double distance = (distance_list[3] / 2) * 0.331;
		//account for relative distance for xy of robot
		//iterate over the readings if one is too bad then we leave it out
		double sonarThreshold = 2000;

		double currentAngle = offset + (i*angle_steps);

		for (int i=0; i<8; i++){
			if (distance_list[i] > sonarThreshold){
				distance_list[i] = -42; //set to negative because it's impossible for sonar

				//printf("%i, \n",i);
			}
			else{
				switch(i){
				case 0:
					X.push_back(sin(convert*((currentAngle+90)*(-1)))*(distance_list[i]/2 *0.331));
					Y.push_back(cos(convert*((currentAngle+90)*(-1)))*(distance_list[i]/2 *0.331));
					break;
				case 1:
					X.push_back(-12.5+(sin(convert*((currentAngle+50)*(-1)))*(distance_list[i]/2 *0.331)));
					Y.push_back(5+(cos(convert*((currentAngle+50)*(-1)))*(distance_list[i]/2 *0.331)));
					break;
				case 2:
					X.push_back(-7.5+(sin(convert*((currentAngle+30)*(-1)))*(distance_list[i]/2 *0.331)));
					Y.push_back(8+(cos(convert*(30*(-1)))*(distance_list[i]/2 *0.331)));
					break;
				case 3:
					X.push_back(-2.5+(sin(convert*((currentAngle+10)*(-1)))*(distance_list[i]/2 *0.331)));
					Y.push_back(10+(cos(convert*((currentAngle+10)*(-1)))*(distance_list[i]/2 *0.331)));
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
		}


		//turn(&robot,-30);
		turn(&robot,-30);

	}
	
		//printf("%f\n", cos(90*convert));
    double n = X.size();

    printf("%f vectors used\n", n);


    return X, Y

}