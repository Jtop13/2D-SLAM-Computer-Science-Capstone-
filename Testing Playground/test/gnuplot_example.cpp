#include <stdio.h>

int main(void){
	int x = 0;
	int y = 0;
	FILE *fp = NULL;
	FILE *gnupipe = NULL;
	char *GnuCommands[] = {"set title \"Demo\"", "plot 'map_data.tmp'"};
	
	
	fp = fopen("map_data.tmp", "w");
	gnupipe = popen("gnuplot -persitent", "w");
	
	for (int i = 0; i<11; i++){
		fprintf(fp, "%d %d\n", x, y);
		
		x = x - 1;
		y = y - 1;
		
	}
	//for each command that we need to send to our pipe
	for (int i = 0; i<2; i++){
		fprintf(gnupipe, "%s\n", GnuCommands[i]);
	}
	
	return 0;
}
