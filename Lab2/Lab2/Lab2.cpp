// Lab2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "stdio.h"
#include "stdlib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
//#include <array>
//#include <algorithm>



using namespace std;

struct CustomOrder {
	bool operator()(const std::string& a, const std::string& b) const {
		string aLessP = a;
		string bLessP = b;

		aLessP = aLessP.erase(0, 1);

		bLessP = bLessP.erase(0, 1);

		return stoi(aLessP) < stoi(bLessP);
	}
};

vector<string> split(string content, string delim); //declare split, used to split string based on arguement defined delim
vector<string> instruction_parser(string instruct); //declare instruction_parser, parses instructions and updates map
bool contains(vector<string> vec, string content); // declare contains, that checks if a vector contains an element


const string instructionCommands[8] = { "dispatched", "requests", "swapped in", "swapped out", "interrupt", " terminated", "expires" };
map<string, string, CustomOrder> stateMap;

vector<string> instruction_parser(string instruct) {
	
	vector<string> instructVector = {};			//new string vector, holds supplied instructions
	vector<string> processes = {};                 //accumulate the list of modified processes

	vector<string> psToDelete = {};

	int blockedCounter = 0;
	// count how many processes are blocked
	map<string, string>::iterator it3;
	for (it3 = stateMap.begin(); it3 != stateMap.end(); it3++) {
		if (it3->second == "Blocked") {
			blockedCounter++;
		}
	}

	// If all processes are either blocked
	if (blockedCounter == stateMap.size()) {
		stateMap.begin()->second = "Blocked/Suspend"; // swap out the first one for Blocked/Suspend
	}

	// Do initial map state checks for special cases B)
	map<string, string>::iterator it2;
	for (it2 = stateMap.begin(); it2 != stateMap.end(); it2++) {
		if (it2->second == "Exit") {
			psToDelete.push_back(it2->first);
		}
		else if (it2->second == "New") {
			stateMap[it2->first] = "Ready";
			processes.push_back(it2->first);
		}
	}

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
					else if(stateMap[process] == "Ready" || stateMap[process] == "Running") {
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


int main() {


	printf("Simulation Begins \n");
	printf("Reading Input 1 \n");
	printf("Initial State \n");

	ifstream input("input1.txt");

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

	//print initial state
	cout << in[0] << endl;

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
		vector<string> processes = instruction_parser(in[j]); //returns the modified processes from that line

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
	}

	printf("Simulation Ends \n");
	printf("\n");

	stateMap.clear(); // Clear out statemap for next input file

	printf("Simulation Begins \n");
	printf("Reading Input 2 \n");
	printf("Initial State \n");

	ifstream inputTwo("input2.txt");

	int countTwo = 0;
	vector<string> inTwo;
	string dataTwo;

	if (inputTwo.good())
	{
		while (getline(inputTwo, dataTwo)) //while there are lines to read, read them
		{
			inTwo.push_back(dataTwo);		//store line
			countTwo++;				//increase counter
		}
		countTwo = 0; //reset counter
	}
	inputTwo.close();

	//print initial state
	cout << inTwo[0] << endl;

	//parse and split initialization values
	vector<string> initLineArgsTwo = split(inTwo[0], " ");

	//Assign each process (key) with its corresponding state (value) in stateMap
	for (int i = 0; i != initLineArgsTwo.size(); i += 2) {
		//add i as key and i+1 as value to dictioanry
		string keyTwo = initLineArgsTwo[i];
		string valueTwo = initLineArgsTwo[i + 1];
		stateMap.insert(pair<string, string>(keyTwo, valueTwo));
	}

	//send each individual line to the line parser
	for (int j = 1; j < inTwo.size(); j++) {
		cout << inTwo[j] << endl; //print the original line
		vector<string> processesTwo = instruction_parser(inTwo[j]); //returns the modified processes from that line

		//print map
		map<string, string>::iterator it2;
		for (it2 = stateMap.begin(); it2 != stateMap.end(); ++it2) { //for each state in the map
			if (contains(processesTwo, it2->first)) { // if it was one of the modified processes, add a *
				cout << it2->first << "->" << it2->second << "*" << endl;
			}
			else {
				cout << it2->first << "->" << it2->second << endl;
			}
		}
	}

	printf("Simulation Ends \n");
	printf("\n");


}