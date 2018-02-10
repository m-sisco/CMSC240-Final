#include "FileData.h"

//default constructor for FileData
FileData::FileData(){

}

/*
This method takes in a file and parses it line by line.
The method handles separating the lines of the file based
on content and formatting the data
*/
void FileData::parseFile(string filename) {

	ifstream ifs;
	ifs.open(filename.c_str());
	string line;

	//makes sure the file is open
	while(ifs.is_open()){
	
		//parses the file line by line
		while (getline(ifs, line)){

			//checks if the data has reached the end and gets the 
			//first character of the line
			if(!(line.substr(0,3) == "END")){
				char c = line.at(0);

				//if the first character is not a digit, parse message
				if(!isdigit(c)){
					parseMsg(line);
					
				//if the first character is a digit, separate the data
				//and parse accordingly	
				} else {
					vector<string> data = tokenize(line);
					
					//makes sure the data is not a blink, and parses
					if(isValidNum(data)){
						parseData(data);
						
					//otherwise the blinks are removed	
					} else {
						removeBlinks(entries);

						//skips 50 lines after a blink occurs
						for (int i = 0; i < 50; i++){

							//skips lines if not end of file
							if (!(line.substr(0,3) == "END")){
								getline(ifs,line);

							//otherwise normalize the data and close stream	
							} else {
								normalizeData(entries);
								normalizeMsg(targets);
								ifs.close();
							}
							
						}
						
					}

				}

			//normalizes data and closes stream at end of file	
			} else {
				normalizeData(entries);
				normalizeMsg(targets);
				ifs.close();
			}
			
				
		}
			
	}	
	
}

/*
This method takes in a line of the input file beginning
with a non-digit and deermines if it is useful data. It
then either stores the data or continues parsing
*/
void FileData::parseMsg(string line){

	//checks if the line begins with MSG and tokenizes if so
	if (line.substr(0,3) == "MSG"){
		vector<string> message = tokenize(line);
		
		//checks if the line contains TARGET_POS and stores the data from
		//the lines if so
		if(message.size() >=5 && message[4] == "TARGET_POS"){

			msgs newMsg;
			newMsg.timestamp = stod(message[1]);

			//these lines remove the parenthesis from the data

			int end = message[6].find(',');

			string x = message[6].substr(1,end);
			string y = message[7].substr(0,3);

			newMsg.xpos = stod(x);
			newMsg.ypos = stod(y);

			targets.push_back(newMsg);

		} 

	}
}

/*
This method takes in a line of the file and separates it
by spaces and tabs into smaller strings to be stored easier when
parsing
*/
vector<string> FileData::tokenize(string line){

	vector<string> lineData;
	char* cstr = new char[line.length() + 1];
	strcpy(cstr, line.c_str());
	char* token;
	token = strtok(cstr, " \t");

	//while there are still strings, add them to vector
	while(token != NULL){

		string s(token);
		lineData.push_back(s);
		token = strtok(NULL, " \t");
	}

	return lineData;
}

/*
This method takes in a vector of strings and converts
the data to doubles to be stored in the class instance
vector of data
*/
void FileData::parseData(vector<string> eyeData){
		
	data newEntry;
	
	newEntry.timestamp = stod(eyeData[0]);
	
	newEntry.leye_x = stod(eyeData[1]);

	newEntry.leye_y = stod(eyeData[2]);

	newEntry.l_pupil = stod(eyeData[3]);

	newEntry.reye_x = stod(eyeData[4]);

	newEntry.reye_y = stod(eyeData[5]);

	newEntry.r_pupil= stod(eyeData[6]);

	newEntry.leye_xvel = stod(eyeData[7]);

	newEntry.leye_yvel = stod(eyeData[8]);

	newEntry.reye_xvel = stod(eyeData[9]);

	newEntry.reye_yvel= stod(eyeData[10]);

	newEntry.xres = stod(eyeData[11]);

	newEntry.yres = stod(eyeData[12]);

	entries.push_back(newEntry);
	
}

/*
This method takes in a data vector and removes the 
line containing a blink, denoted by a 0 pupil size
and removes the 50 previous entries as well
*/
void FileData::removeBlinks(vector<data> res){

	//iterates through 50 entires, or stops at an 
	//empty vector, whichever comes first
	for (int i = 0; (i < 50 || res.empty()); i++){
		res.pop_back();
	}
}

/*
This method takes in a string vector and determines
if the line from the input file is valid. It outputs
true if no blink is detected, and false otherwise.
*/
bool FileData::isValidNum(vector<string> input){

	//iterates through the inpupt vector entries
	for (int i = 0; i < input.size() - 1; i++){
		string s = input[i];

		//checks if the string starts with a minus
		//sign followed by a digit
		if (s[0] == '-'){

			//if no digit after minus sign, return false
			if(!isdigit(s[1])){
				return false;
			}

		//otherwise checks if the string is not a digit 
		//to return false	
		} else if (!isdigit(s[0]) && s[0] != '-'){
			return false;
		}
	}

	return true;
}

/*
This method takes in a data vector and normalizes it
so that the data is uniform and can be represented
more easily
*/
void FileData::normalizeData(vector<data> &eyeData){
	double xavg = findAvgXRes(entries);
	double yavg = findAvgYRes(entries);
	
	//iterates through the vector adjusting the values of the x,y coords
	for (int i = 0; i < eyeData.size(); i++){

		eyeData[i].leye_x = (eyeData[i].leye_x -(1920/2)) / xavg;
		eyeData[i].leye_y = (eyeData[i].leye_y - (1200/2)) / yavg;
		eyeData[i].reye_x = (eyeData[i].reye_x - (1920/2)) / xavg;
		eyeData[i].reye_y = (eyeData[i].reye_y - (1200/2)) / yavg;
	}

	//updates the velocity data 
	updateVel(eyeData);
}

/*
This method takes in a msgs vector and normalizes it
so that the data is uniform and can be represented
more easily
*/
void FileData::normalizeMsg(vector<msgs> &message){
	double xavg = findAvgXRes(entries);
	double yavg = findAvgYRes(entries);

	//iterates through the vector adjusting the values of the x,y coords
	for (int i = 0; i < message.size(); i++){

		message[i].xpos = (message[i].xpos - (1920/2)) / xavg;
		message[i].ypos = (message[i].ypos - (1200/2)) / yavg;
	}

}

/*
This method takes in a data vector and finds the average
of the x coord resolutions and returns the result
as a double
*/
double FileData::findAvgXRes(vector<data> eyeData){
	double xsum;

	//adds up the resolutions from the vector
	for (int i = 0; i < eyeData.size(); i++){
		xsum += eyeData[i].xres;
	}

	xsum = (double)(xsum / eyeData.size());	
	return xsum;
}

/*
This method takes in a data vector and finds the average
of the y coord resolutions and returns the result
as a double
*/
double FileData::findAvgYRes(vector<data> eyeData){
	double ysum;

	//adds up the resolutions from the vector
	for (int i = 0; i < eyeData.size(); i++){
		ysum += eyeData[i].yres;
	}

	ysum= (double)(ysum / eyeData.size());
	return ysum;

}

/*
This method returns a double representing
the timestamp at a given index
*/
double FileData::getTime(int index){
	return entries[index].timestamp;
}

/*
This method returns a double representing
the left x coord at a given index
*/
double FileData::getLx(int index){
	return entries[index].leye_x;
}

/*
This method returns a double representing
the left y coord at a given index
*/
double FileData::getLy(int index){
	return entries[index].leye_y;
}

/*
This method returns a double representing
the right x coord at a given index
*/
double FileData::getRx(int index){
	return entries[index].reye_x;
}

/*
This method returns a double representing
the right y coord at a given index
*/
double FileData::getRy(int index){
	return entries[index].reye_y;
}

/*
This method returns a double representing
the left pupil size at a given index
*/
double FileData::getLPupil(int index){
	return entries[index].l_pupil;
}

/*
This method returns a double representing
the right pupil size at a given index
*/
double FileData::getRPupil(int index){
	return entries[index].r_pupil;
}

/*
This method returns a double representing
the left x velocity at a given index
*/
double FileData::getLXVel(int index){
	return entries[index].leye_xvel;
}

/*
This method returns a double representing
the right x velocity at a given index
*/
double FileData::getRXVel(int index){
	return entries[index].reye_xvel;
}

/*
This method returns a double representing
the left y velocity at a given index
*/
double FileData::getLYVel(int index){
	return entries[index].leye_yvel;
}

/*
This method returns a double representing
the right y velocity at a given index
*/
double FileData::getRYVel(int index){
	return entries[index].reye_yvel;
}

/*
This method returns a double representing
the x resolution at a given index
*/
double FileData::getXres(int index){
	return entries[index].xres;
}

/*
This method returns a double representing
the y resolution at a given index
*/
double FileData::getYres(int index){
	return entries[index].yres;
}

/*
This method returns a double representing
the msg timestamp at a given index
*/
double FileData::getMsgTime(int index){
	return targets[index].timestamp;
}

/*
This method returns a double representing
the target x position at a given index
*/
double FileData::getXPos(int index){
	return targets[index].xpos;
}

/*
This method returns a double representing
the target y position at a given index
*/
double FileData::getYPos(int index){
	return targets[index].ypos;
}

/*
This method returns the data vector containing
the eye data information
*/
vector<data> FileData::getData(){
	return entries;
}

/*
This method returns the msgs vector containing
the information for the targets
*/
vector<msgs> FileData::getMsgs(){
	return targets;
}

/*
This method takes in a vector of doubles and returns
the average in a double of the values
*/
double FileData::movingAvg(vector<double> velocity){
	int sum;

	//adds the values of the vactor
	for (int i = 0; i < velocity.size(); i++){
		sum += velocity[i];
	}

	double avg = sum / (double)velocity.size();

	return avg;

}

/*
This method takes in a data vector and updates all
values to be moving averages to keep the data
more uniform
*/
void FileData::updateVel(vector<data> &eyeData){
	int count = 50;

	//vectors for the velocities
	vector<double> lxvel;
	vector<double> lyvel;
	vector<double> rxvel;
	vector<double> ryvel;

	//adds the first 50 values from the input to a vector
	for (int i = 0; i < count; i++){
		lxvel.push_back(eyeData[i].leye_xvel);
		lyvel.push_back(eyeData[i].leye_yvel);
		rxvel.push_back(eyeData[i].reye_xvel);
		ryvel.push_back(eyeData[i].reye_yvel);

	}

	//updates the velocities and computes the new velocities
	for (int i = count; i < eyeData.size(); i++){
		
		//assigns the averages to the velocity values
		eyeData[i - 1].leye_xvel = movingAvg(lxvel);
		eyeData[i - 1].leye_yvel = movingAvg(lyvel);
		eyeData[i - 1].reye_xvel = movingAvg(rxvel);
		eyeData[i - 1].reye_yvel = movingAvg(ryvel);

		//updates the vectors to hold the updated 50 values
		lxvel.erase(lxvel.begin());
		lxvel.push_back(eyeData[i].leye_xvel);
		lyvel.erase(lyvel.begin());
		lyvel.push_back(eyeData[i].leye_yvel);
		rxvel.erase(rxvel.begin());
		rxvel.push_back(eyeData[i].reye_xvel);
		ryvel.erase(ryvel.begin());
		ryvel.push_back(eyeData[i].reye_yvel);

	}
}

