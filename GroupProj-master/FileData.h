#ifndef FILEDATA_H
#define FILEDATA_H


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

//struct to hold the data for the eye movements
struct data{
	double timestamp;
	double leye_x;
	double leye_y;
	double reye_x;
	double reye_y;
	double l_pupil;
	double r_pupil;
	double leye_xvel;
	double leye_yvel;
	double reye_xvel;
	double reye_yvel;
	double xres;
	double yres;

};

//struct to hold the data from the target position changes
struct msgs{
	double timestamp;
	double xpos;
	double ypos;
};


/*
This class reads in data from input files and stores it accordingly. The class
parses the data by number and word to determine the information to be stored. The
data stored is normalized so that it can be used for analysis

*/
class FileData{



public:

	//default constructor 
	FileData();

	//takes in a file and parses the data, storing it accordingly
	void parseFile(string filename);

	//gets the timestamp given an index in an vector
	double getTime(int index);

	//gets the left x coordinate given an index in an vector
	double getLx(int index);

	//gets the left y coordinate given an index in an vector
	double getLy(int index);

	//gets the right x coordinate given an index in an vector
	double getRx(int index);

	//gets the right y coordinate given an index in an vector
	double getRy(int index);

	//gets the left pupil diameter given an index in an vector
	double getLPupil(int index);

	//gets the right pupil diameter given an index in an vector
	double getRPupil(int index);

	//gets the left x velocity given an index in an vector
	double getLXVel(int index);

	//gets the right x velocity given an index in an vector
	double getRXVel(int index);

	//gets the left y velocity given an index in an vector
	double getLYVel(int index);

	//gets the right y velocity given an index in an rray
	double getRYVel(int index);

	//gets the x resolution given an index in an vector
	double getXres(int index);

	//gets the y resolution given an index in an vector
	double getYres(int index);

	//gets the timestamp for the target position change given 
	//an index in a vector
	double getMsgTime(int index);

	//gets the x coordinate of the position change given a vector index
	double getXPos(int index);

	//gets the y coordinate of the position change given a vector index
	double getYPos(int index);

	//returns the vector containing the eye movement data
	vector<data> getData();

	//returns the vector containing the target position data
	vector<msgs> getMsgs();


private:
	vector<data> entries;
	vector<msgs> targets;

	//takes a line of data and splits it into smaller 
	//strings separated by spaces and tabs
	vector<string> tokenize(string line);

	//if the data read is a word, gathers the needed information and stores
	void parseMsg(string line);

	//if the data read is numbers, gathers the needed information and stores
	void parseData(vector<string> eyeData);


	/*
	normalizes the data so that the coordinate system starts 
	in the center of the screen and accounts for resolution.
	The method takes in a position in an vector and changes the data 
	accordingly
	*/
	void normalizeData(vector<data> &eyeData);

	/*
	normalizes the data so that the coordinate system starts 
	in the center of the screen and accounts for resolution.
	The method takes in a position in an vector and changes the data 
	accordingly
	*/
	void normalizeMsg(vector<msgs> &message);

	//takes a line in a vector and determines if all are numbers or not
	//and returns true or false accordingly
	bool isValidNum(vector<string> input);

	//takes a vector and removes the preious 50 entries
	//when a blink is detected
	void removeBlinks(vector<data> res);

	//takes a series of numbers from a vector and finds
	//the average
	double findAvgXRes(vector<data> eyeData);

	//takes a series of numbers from a vector and finds
	//the average
	double findAvgYRes(vector<data> eyeData);

	//takes a vector of doubles and computes the moving
	//average
	double movingAvg(vector<double> velocity);

	//takes a vector and updates the velocity
	//values to a moving average
	void updateVel(vector<data> &eyeData);


};

#endif