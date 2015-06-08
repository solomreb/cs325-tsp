#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <climits>

using namespace std;

struct city{
	int id;
	int x;
	int y;
	int grid;
};


int largeX = 0;
int largeY = 0;
int minX = INT_MAX;
int minY = INT_MAX;
	
void usage(int argc, char **argv){
	if (argc != 2){
		fprintf(stderr, "Usage: %s inputFile\n", argv[0]);
		exit (EXIT_FAILURE);
	}
	return;
}

//open filename. parse list of cities. find min and max x- and y- coordinates
vector<city> parseInput(char* filename){
	ifstream myfile;
	string line;
	vector<city> cities;
	int id, x, y;
	
	myfile.open(filename);

	while (getline(myfile, line)){
		istringstream iss(line);
		iss >> id >> x >> y;
		city c = {id, x, y};
		if(c.x > largeX){
			largeX = x;
		}
		if(c.x < minX){
			minX = x;
		}
		if(c.y > largeY){
			largeY = y;
		}
		if(c.y < minY){
			minY = y;
		}
		cities.push_back(c);
	}
	myfile.close();
	
	return cities;
}

//divide remainingCities into 4 quadrants based on x- and y- coordinates
void fillQuadrants (vector<city> &remainingCities, 
							vector<city> &quad0, 
							vector<city> &quad1, 
							vector<city> &quad2, 
							vector<city> &quad3){

	int halfX = (largeX + minX)/2;
	int halfY = (largeY + minY)/2;
	
	for(int i = 0; i < remainingCities.size(); i++){
		if(remainingCities[i].x <= halfX && remainingCities[i].y >= halfY){//top left quadrant
			remainingCities[i].grid = 0;
			quad0.push_back(remainingCities[i]);
		}
		else if(remainingCities[i].x >= halfX && remainingCities[i].y >= halfY){//top right quadrant 
			remainingCities[i].grid = 1;
			quad1.push_back(remainingCities[i]);
		}
		else if(remainingCities[i].x >= halfX && remainingCities[i].y <= halfY){//bottom right quadrant
			remainingCities[i].grid = 2;
			quad2.push_back(remainingCities[i]);
		}
		else{ //bottom left grid
			remainingCities[i].grid = 3;
			quad3.push_back(remainingCities[i]);
		}
	}
}

//calculate euclidian distance between points A and B
int calcDist(city A, city B){
	int dist = round( sqrt( pow((float)A.x - B.x, 2) + pow((float)A.y - B.y, 2) ));
	return dist;
}

//use greedy method to find shortest tour from currCity to lastCity in remainingCities. push city ids to visited in order visited. returns total distance of tour
int TSP(city currCity, vector<city> &remainingCities, city &lastCity, vector<int> &visited){
	
	int minIdx = 0;
	int distance = 0;
	int distTraveled = 0;
	int curDist;
	
	visited.push_back(remainingCities.back().id);
	remainingCities.pop_back(); //remove starting city from list of remaining cities
	
	while (!remainingCities.empty()){
		curDist = INT_MAX; //cur dist initialized to really large number
		for(int i = 0; i < remainingCities.size(); i++){ //find closest city to current city
			distance = calcDist(currCity, remainingCities[i]);
			if(distance < curDist){
				curDist = distance;
				minIdx = i;
			}
		}
		currCity = remainingCities[minIdx]; //closest city is new current city
		visited.push_back(remainingCities[minIdx].id); //add to list of visited cities
		remainingCities.erase(remainingCities.begin() + minIdx); //remove from list of remaining cities
		distTraveled += curDist; //add curDist to distTraveled
	}
	lastCity = currCity; //keep track of last city

	return distTraveled;
}

//outputs to filename solution to TSP problem (tour distance and order of cities visited)
void writeTourToFile(char* filename, int distance, vector<int> visited){
	char outputFile[100];

	strcpy(outputFile, filename);
	strcat(outputFile, ".tour");
	
	ofstream myfile;
  	myfile.open (outputFile);
  	myfile << distance<< endl;

	for (int i=0; i<visited.size(); i++){
		myfile << visited[i] << endl;
	}
  	myfile.close();

}
int main(int argc, char * argv[]){

	usage(argc, argv); //check command line syntax
		
	int id, x, y, distance, IdxMin;
	int distTraveled = 0;
	int finalDistTraveled = 0;
	int curDist = INT_MAX;
	city startCity, lastCity;
	vector<int> visited;
	vector<city> cities;
	vector<city> quad0;
	vector<city> quad1;
	vector<city> quad2;
	vector<city> quad3;
	cities = parseInput(argv[1]); //open file and get city data
	fillQuadrants(cities, quad0, quad1, quad2, quad3); //divide cities into 4 quadrants

	if(!quad0.empty()){ //travel around quad0
		startCity = quad0.back();
		finalDistTraveled += TSP(startCity, quad0, lastCity, visited);
	}	
	
	for(int i = 0; i < quad1.size(); i++){ //find out which city to go to first in quad1
		distance = calcDist(lastCity, quad1[i]);
		if(distance < curDist){
			curDist = distance;
			IdxMin = i;
		}
	}
	
	if(!quad1.empty()){ 
		startCity = quad1[IdxMin];
		finalDistTraveled += calcDist(lastCity, startCity);	//travel from quad0 to quad1
		finalDistTraveled += TSP(startCity, quad1, lastCity, visited);	//travel around quad1


	}
	curDist = INT_MAX;
	for(int i = 0; i < quad2.size(); i++){ ///find out which city to go to first in quad2
		distance = calcDist(lastCity, quad2[i]);
		if(distance < curDist){
			curDist = distance;
		}
	}
	
	if(!quad2.empty()){ 
		finalDistTraveled += calcDist(lastCity, quad2[IdxMin]);	//travel from quad1 to quad2
		startCity = quad2.back();
		finalDistTraveled += TSP(startCity, quad2, lastCity, visited); //travel around quad2
	}	
	curDist = INT_MAX;
	for(int i = 0; i < quad3.size(); i++){ //find out which city to go to first in quad3
		distance = calcDist(lastCity, quad3[i]);
		if(distance < curDist){
			curDist = distance;
		}
	}

	if(!quad3.empty()){ 
		finalDistTraveled += calcDist(lastCity, quad3[IdxMin]);	//travel from quad2 to quad3
		startCity = quad3.back();
		finalDistTraveled += TSP(startCity, quad3, lastCity, visited); //travel around quad3
	}

	writeTourToFile(argv[1], finalDistTraveled, visited);

return 0;
}
