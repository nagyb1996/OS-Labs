#include "pch.h"
#include "stdio.h"
#include "stdlib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

//custom comparator to ensure Map is properly ordered
struct CustomOrder {
	bool operator()(const std::string& a, const std::string& b) const {
		string aLessP = a;
		string bLessP = b;

		aLessP = aLessP.erase(0, 1);	//remove the P from process ID
		bLessP = bLessP.erase(0, 1);

		return stoi(aLessP) < stoi(bLessP);	//cast string to int and compare
	}
};

vector<string> split(string content, string delim); //declare split, used to split string based on arguement defined delim
vector<string> instruction_parser(string instruct, int percent, int numProc); //declare instruction_parser, parses instructions and updates map
bool contains(vector<string> vec, string content); // declare contains, that checks if a vector contains an element
void manipulate(vector<string> in, int percent, int numProc); //method to call other methods so main isnt repetative
int fileNumber = 0;//keep track of file number
string line = "_______________________________________________________________________________________________________________________________________";

const string instructionCommands[8] = { "dispatched", "requests", "swapped in", "swapped out", "interrupt", " terminated", "expires" };//array of possible commands
map<string, string, CustomOrder> stateMap;	//instantiate map

int latency = 0;
int memorySize = 20;

vector<string> instruction_parser(string instruct, int percent, int numProc) {

	int blockThreshold = percent * memorySize / 100;

	vector<string> instructVector = {};			//new string vector, holds supplied instructions
	vector<string> processes = {};                 //accumulate the list of modified processes
	vector<string> psToDelete = {};			//vector to collect list of exiting processes for removal
	int flagExit = 0;

	//Checked map for an exiting processes, add them to vector for removal
	map<string, string>::iterator it3;
	for (it3 = stateMap.begin(); it3 != stateMap.end(); it3++) {
		if (it3->second == "Exit") {
			psToDelete.push_back(it3->first); //add exiting processes to vector to be deleted 
		}
	}

	// do the special stuff for assignment 4
	if (fileNumber == 1 || fileNumber == 2)
	{
		int blockedCounter = 0;
		int flagBlocked = 0;
		//int flagExit = 0;

		// count how many processes are blocked
		map<string, string>::iterator it3;
		for (it3 = stateMap.begin(); it3 != stateMap.end(); it3++) {
			if (it3->second == "Blocked") {
				blockedCounter++;
			}
		}

		// If blocked processes exceed specified percent
		if (blockedCounter >= blockThreshold) {
			for (auto it3 = stateMap.rbegin(); it3 != stateMap.rend(); it3++) {
				if (it3->second == "Blocked") {
					stateMap[it3->first] = "Blocked/Suspend";
					processes.push_back(it3->first);
					cout << "Swapped " << it3->first << " out because blocked processes exceed specified percent" << endl;
					latency++;
					break;
				}

			}

			if (numProc == 2) {
				for (auto it3 = stateMap.rbegin(); it3 != stateMap.rend(); it3++) {
					if (it3->second == "Blocked") {
						stateMap[it3->first] = "Blocked/Suspend";
						processes.push_back(it3->first);
						cout << "Swapped " << it3->first << " out to make room for New Process" << endl;
						latency++;
						break;
					}

				}
			}
		}

		for (it3 = stateMap.begin(); it3 != stateMap.end(); it3++) {
			if (it3->second == "New") {
				stateMap[it3->first] = "Ready";
				processes.push_back(it3->first);
				cout << "Added " << it3->first << " to main memory to fill space" << endl;
				flagBlocked = 1;
				latency++;
				break;
			}
			if (flagBlocked == 0) {
				flagBlocked = 2;
			}
		}

	}


	// Do initial map state checks for exited process
	//map<string, string>::iterator it2;
	for (it3 = stateMap.begin(); it3 != stateMap.end(); it3++) {
		if (it3->second == "Exit") {
			psToDelete.push_back(it3->first);
			cout << it3->first << " Exits" << endl;
			flagExit = 1;
			break;
		}
	}

	if (flagExit == 1) {
		for (it3 = stateMap.begin(); it3 != stateMap.end(); it3++) {
			if (it3->second == "New") {
				stateMap[it3->first] = "Ready";
				processes.push_back(it3->first);
				cout << "Added " << it3->first << " to main memory to fill space" << endl;
				flagExit = 0;
				break;
			}
		}

		if (flagExit == 1) {
			flagExit = 2;
		}
	}



	if (flagExit == 2) {
		for (it3 = stateMap.begin(); it3 != stateMap.end(); it3++) {
			if (it3->second == "Ready/Suspend") {
				stateMap[it3->first] = "Ready";
				processes.push_back(it3->first);
				cout << "Swapped " << it3->first << " in to fill space from exited process" << endl;
				flagExit = 0;
				break;
			}
		}

	}

	if (flagExit == 2) {
		for (auto it3 = stateMap.rbegin(); it3 != stateMap.rend(); it3++) {
			if (it3->second == "Block/Suspend") {
				stateMap[it3->first] = "Block";
				processes.push_back(it3->first);
				cout << "Swapped " << it3->first << " in to fill space from exited process" << endl;
				break;
			}
		}
	}






	//Delete exiting processes
	for (int c = 0; c < psToDelete.size(); c++) {
		stateMap.erase(psToDelete[c]);
	}


	instructVector.push_back(instruct);		//add instruction to vector, if there is more than instruction, if statement will catch and split it

	if (instruct.find(";") != string::npos) { //if there are multiple instructions on one line, split them up

		instructVector = split(instruct, ";");

	}

	// for each instruction, process it
	for (int i = 0; i < instructVector.size(); i++) {
		string state = ""; // The calculated state based on the previous state and the instruction
		string process = ""; // The process this instruction contains
		vector<string> parts = split(instructVector[i], " "); //The words of this instruction, split individually

		// Go through each word in this instruction, and remove extraneous spaces and periods
		for (int m = 0; m < parts.size(); m++) {
			if (parts[m].find(".") != string::npos) {
				int location = parts[m].find(".");
				parts[m] = parts[m].erase(location, 1);
			}
			if (parts[m].find(" ") != string::npos) {
				int location = parts[m].find(" ");
				parts[m] = parts[m].erase(location, 1);
			}
		}
		// get the correct process from the instruction
		map<string, string>::iterator it1;
		for (it1 = stateMap.begin(); it1 != stateMap.end(); it1++) { //find the process that the instruction will apply to

			for (int k = 0; k < parts.size(); k++) { // for each word in the instruction, see if that word is a process
				if (parts[k] == it1->first) { // if the word is the process,
					processes.push_back(it1->first); //add it to the accumulated list of modified processes
					process = it1->first; //save it as the process whose state we will modify
				}
			}

		}

		// get the correct instruction, and set the state accordingly
		for (int j = 0; j < 7; j++) {
			if (instructVector[i].find(instructionCommands[j]) != string::npos) { // if we found the right instruction
				switch (j) {
				case 0: //dispatched -> running
					state = "Running";
					stateMap[process] = state; //mutate the global stateMap variable with updates
					break;
				case 1: //request -> blocked
					state = "Blocked";
					stateMap[process] = state; //mutate the global stateMap variable with updates
					break;
				case 2: //swapped in -> ready
					if (stateMap[process] == "Blocked/Suspend") {
						state = "Blocked";
					}
					else {
						state = "Ready";
					}
					stateMap[process] = state; //mutate the global stateMap variable with updates
					break;
				case 3: //suspend occurs (swapped out): blocked -> blocked/suspend, Ready -> Ready/suspend, Running -> Ready/Suspend
					if (stateMap[process] == "Blocked") {
						state = "Blocked/Suspend";
					}
					else if (stateMap[process] == "Ready" || stateMap[process] == "Running") {
						state = "Ready/Suspend";
					}
					stateMap[process] = state; //mutate the global stateMap variable with updates
					break;
				case 4: //interrupt -> ready
					if (stateMap[process] == "Blocked/Suspend") {
						state = "Ready/Suspend";
					}
					else {
						state = "Ready";
					}
					stateMap[process] = state; //mutate the global stateMap variable with updates
					break;
				case 5: //terminated -> exit -> on next run DELETE FROM MAP
					state = "Exit";
					stateMap[process] = state; //mutate the global stateMap variable with updates
					break;
				case 6: //expires -> ready
					state = "Ready";
					stateMap[process] = state; //mutate the global stateMap variable with updates
					break;
				}
			}

		}
	}
	return processes; //return the accumulated list of modified processes
}


// Given a vector of strings, return if the vector contains the given string
bool contains(vector<string> vec, string content) {
	bool result = false;
	for (int k = 0; k < vec.size(); k++) {
		if (vec[k] == content) {
			result = true;
			break;
		}
	}
	return result;
}

// Given a string content, and a string delimiter, split the string into a vector, and remove delimters
vector<string> split(string content, string delim) {
	vector<string> inits;

	int position = 0;
	string token;

	while ((position = content.find(delim)) != string::npos) { //split strings based on supplied delim token
		token = content.substr(0, position);
		inits.push_back(token);
		content.erase(0, position + delim.length());
	}
	inits.push_back(content); //Add whatever is left, with no delimeters, fixes issue of dropping final state

	return inits;
}

void manipulate(vector<string> in, int percent, int numProc) {

	//print initial state
	cout << in[0] << endl;
	cout << "\n";

	//parse and split initialization values
	vector<string> initLineArgs = split(in[0], " ");

	//Assign each process (key) with its corresponding state (value) in stateMap
	for (int i = 0; i != initLineArgs.size(); i += 2) {
		//add i as key and i+1 as value to dictioanry
		string key = initLineArgs[i];
		string value = initLineArgs[i + 1];
		stateMap.insert(pair<string, string>(key, value));
	}

	//send each individual line to the line parser
	for (int j = 1; j < in.size(); j++) {
		cout << in[j] << endl; //print the original line
		vector<string> processes = instruction_parser(in[j], percent, numProc); //returns the modified processes from that line

		//print map
		map<string, string>::iterator it1;
		for (it1 = stateMap.begin(); it1 != stateMap.end(); it1++) { //for each state in the map
			if (contains(processes, it1->first)) { // if it was one of the modified processes, add a *
				cout << it1->first << "->" << it1->second << "*" << endl;
			}
			else {
				cout << it1->first << "->" << it1->second << endl;
			}
		}
		cout << "\n";
	}

	printf("Simulation Ends \n");
	cout << line << endl;
	printf("\n");

	stateMap.clear(); // Clear out statemap for next input file
}

int main() {

	int percent;
	int numProc;
	bool valid = false;

	while (!valid) {
		printf("Please select the percantage at which Processes are swapped out: \n");
		printf("Enter 80 for 80% \nEnter 90 for 90% \nEnter 100 for 100% \n");
		cin >> percent;

		if (percent == 80) {

			valid = true;
		}

		else if (percent == 90) {

			valid = true;
		}

		else if (percent == 100) {

			valid = true;
		}

		else {
			printf("You have not selected a valid option please try again! \n");
		}
	}

	while (valid) {
		printf("Please select the number of Processes to swap: \n");
		printf("Enter 1 for 1 Process at a time \nEnter 2 for 2 Processes at a time \n");
		cin >> numProc;

		if (numProc == 1) {

			valid = false;
		}

		else if (numProc == 2) {

			valid = false;
		}

		else {
			printf("You have not selected a valid option please try again! \n");
		}
	}

	printf("Running Simulation with the following perameters: \n Blocked Percent Threshold = %d% \n Number of Processes to swap at once: %d \n", percent, numProc);

	printf("Simulation Begins \n");
	printf("Reading Input 1 \n");
	cout << "\n";
	printf("Initial State \n");

	ifstream input("input1.txt");

	fileNumber = 1;
	int count = 0;
	vector<string> in;
	string data;

	if (input.good())
	{
		while (getline(input, data)) //while there are lines to read, read them
		{
			in.push_back(data);		//store line
			count++;				//increase counter
		}
		count = 0; //reset counter
	}
	input.close();

	manipulate(in, percent, numProc);

	int latency1 = latency;


	printf("Simulation Begins \n");
	printf("Reading Input 2 \n");
	cout << "\n";
	printf("Initial State \n");

	ifstream input2("input2.txt");

	fileNumber = 2;
	count = 0;
	in.clear();
	data.clear();
	latency = 0;

	if (input2.good())
	{
		while (getline(input2, data)) //while there are lines to read, read them
		{
			in.push_back(data);		//store line
			count++;				//increase counter
		}
		count = 0; //reset counter
	}
	input2.close();

	manipulate(in, percent, numProc);

	printf("\nTotal Latency for file 1: %d \n", latency1);

	printf("\nTotal Latency for file 2: %d \n", latency);


}