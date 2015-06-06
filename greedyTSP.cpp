#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

struct city{
	int id;
	int x;
	int y;
};

vector<city> remainingCities;
vector<city> visitedCities;

void usage(int argc, char **argv){
	if (argc != 2){
		fprintf(stderr, "Usage: %s inputFile\n", argv[0]);
		exit (EXIT_FAILURE);
	}
	return;
}

void print(vector<city> cities){
	for (int i=0; i<cities.size() ;i++){
		cout << cities[i].id << ": x=" << cities[i].x << " y=" << cities[i].y << endl;
	}
}

int calcDist(city A, city B){
	int dist = round( sqrt( pow((float)A.x - B.x, 2) + pow((float)A.y - B.y, 2) ));
	return dist;
}
 
int greedyTSP(city startCity, vector<city> remainingCities, vector<city> visitedCities){
	int i; 
	int dist = 0, distTraveled = 0;
	int nextCityIdx = -1;
	int curMinDist = 1000000;
	city nextCity = {0, 0, 0};
	
	while (remainingCities.size() >= 1){
	
		printf("-----Now at city %d...\n", startCity.id);
		curMinDist = 1000000;
		for (i=0; i<remainingCities.size(); i++){
			dist = calcDist(startCity, remainingCities.at(i));
			if (dist < curMinDist){
				curMinDist = dist;
				nextCityIdx = i;
				nextCity = remainingCities.at(i);
				
			}
		}
		distTraveled += curMinDist;
		
		visitedCities.push_back(nextCity);
			
		remainingCities.erase(remainingCities.begin() + nextCityIdx);
		
		startCity = nextCity;
		
	}
		
	printf("Traveling from last city %d back to start %d...%d miles away\n", nextCity.id, visitedCities.front().id, calcDist(visitedCities.front(), nextCity));
	distTraveled +=calcDist(visitedCities.front(), nextCity);
		
		
	printf("TOTAL DIST TRAVELED: %d miles\n", distTraveled);
	printf("********************************************\n\n");
	return distTraveled;
}



int main(int argc, char * argv[]){

	usage(argc, argv);
		
	//open file
	ifstream myfile;
	string line;
	myfile.open(argv[1]);

	while (getline(myfile, line)){
		istringstream iss(line);
		int id, x, y;
		iss >> id >> x >> y;
		city c = {id, x, y};
		remainingCities.push_back(c);
	}
	myfile.close();
	
	
	city startCity = remainingCities.front();
	remainingCities.erase(remainingCities.begin());
	visitedCities.push_back(startCity);
	printf("starting tour at %d\n...", startCity.id);
	int distTraveled = 0;
	distTraveled = greedyTSP(startCity, remainingCities, visitedCities);

	return 0;
}