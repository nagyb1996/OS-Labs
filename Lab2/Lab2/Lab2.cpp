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
#include <array>

using namespace std;



vector<string> split(string content, string delim); //declare split, used to split string based on arguement defined delim
vector<string> instruction_parser(string instruct); //declare instruction_parser, parses instructions and updates map
vector<string> instruction_parserTwo(string instruct); //declare instruction_parser, parses instructions and updates map
bool contains(vector<string> vec, string content); // declare contains, that checks if a vector contains an element


const string instructionCommands[8] = { "dispatched", "requests", "swapped in", "swapped out", "interrupt", " terminated", "expires" };
map<string, string> stateMap;
map<string, string> stateMapTwo;



vector<string> instruction_parserTwo(string instruct) {

	vector<string> instructVectorTwo;			//new string vector, holds supplied instructions
	vector<string> processTwo;                 //accumulate the list of modified processes

	instructVectorTwo.push_back(instruct);		//add instruction to vector, if there is more than instruction, if statement will catch and split it

	if (instruct.find(";") != string::npos) { //if there are multiple instructions on one line, split them up

		instructVectorTwo = split(instruct, ";");

	}

	// for each instruction, process it
	for (std::size_t i = 0; i < instructVectorTwo.size(); i++) {
		string state;

		// get the correct process from the instruction
		map<string, string>::iterator it2;
		for (it2 = stateMapTwo.begin(); it2 != stateMapTwo.end(); ++it2) { //find the process that the instruction will apply to
			if (instructVectorTwo[i].find(it2->first) != string::npos) {
				processTwo.push_back(it2->first); //add it to the accumulated list of modified processes
			}
		}

		// get the correct instruction, and set the state accordingly
		for (int j = 0; j < 7; j++) {
			if (instructVectorTwo[i].find(instructionCommands[j]) != string::npos) { // if we found the right instruction
				string process = processTwo[i];
				switch (j) {
				case 0: //dispatched -> running
					state = "Running";
					stateMapTwo[processTwo[i]] = state; //mutate the global stateMap variable with updates
					break;
				case 1: //request -> blocked
					state = "Blocked";
					stateMapTwo[processTwo[i]] = state; //mutate the global stateMap variable with updates
					break;
				case 2: //swapped in -> ready
					if (stateMapTwo[process] == "Blocked/Suspend") {
						state = "Blocked";
						stateMapTwo[processTwo[i]] = state; //mutate the global stateMap variable with updates
						break;
					}
					else {
						state = "Ready";
						stateMapTwo[processTwo[i]] = state; //mutate the global stateMap variable with updates
						break;
					}
					
				case 3: //swapped out -> blocked/suspend
					if (stateMapTwo[process] == "Blocked") {
						state = "Blocked/Suspend";
						stateMapTwo[processTwo[i]] = state; //mutate the global stateMap variable with updates
						break;
					}
					else if (stateMapTwo[process] == "Ready" || stateMapTwo[process] == "Running") {
						state = "Ready/Suspend";
						stateMapTwo[processTwo[i]] = state; //mutate the global stateMap variable with updates
						break;
					}

				case 4: //interrupt -> ready OR Ready/Suspend
					if (stateMapTwo[process] == "Blocked/Suspend") {
						state = "Ready/Suspend";
						stateMapTwo[processTwo[i]] = state; //mutate the global stateMap variable with updates
						break;
					}
					else {
						state = "Ready";
						stateMapTwo[processTwo[i]] = state; //mutate the global stateMap variable with updates
						break;
					}

				case 5: //terminated -> DELETE FROM MAP
					stateMapTwo.erase(processTwo[i]); //mutate the global stateMap variable with updates
					break;
				case 6: //expires -> ready
					state = "Ready";
					stateMapTwo[processTwo[i]] = state; //mutate the global stateMap variable with updates
					break;
				}
			}

		}
	}
	return processTwo; //return the accumulated list of modified processes
}


vector<string> instruction_parser(string instruct) {

	vector<string> instructVector = {};			//new string vector, holds supplied instructions
	vector<string> process = {};                 //accumulate the list of modified processes

	instructVector.push_back(instruct);		//add instruction to vector, if there is more than instruction, if statement will catch and split it

	if (instruct.find(";") != string::npos) { //if there are multiple instructions on one line, split them up

		instructVector = split(instruct, ";");

	}

	// for each instruction, process it
	for (std::size_t i = 0; i < instructVector.size(); i++) {
		string state;

		// get the correct process from the instruction
		map<string, string>::iterator it1;
		for (it1 = stateMap.begin(); it1 != stateMap.end(); it1++) { //find the process that the instruction will apply to
			if (instructVector[i].find(it1->first) != string::npos) {
				process.push_back(it1->first); //add it to the accumulated list of modified processes
			}
		}

		// get the correct instruction, and set the state accordingly
		for (int j = 0; j < 7; j++) {
			if (instructVector[i].find(instructionCommands[j]) != string::npos) { // if we found the right instruction
				string Currentprocess = process[i];
				switch (j) {
				case 0: //dispatched -> running
					state = "Running";
					stateMap[process[i]] = state; //mutate the global stateMap variable with updates
					break;
				case 1: //request -> blocked
					state = "Blocked";
					stateMap[process[i]] = state; //mutate the global stateMap variable with updates
					break;
				case 2: //swapped in -> ready
					if (stateMap[Currentprocess] == "Blocked/Suspend") {
						state = "Blocked";
					}
					else {
						state = "Ready";
					}
					stateMap[process[i]] = state; //mutate the global stateMap variable with updates
					break;
				case 3: //suspend occurs (swapped out): blocked -> blocked/suspend, Ready -> Ready/suspend, Running -> Ready/Suspend
					if (stateMap[Currentprocess] == "Blocked") {
						state = "Blocked/Suspend";
					}
					else if(stateMap[Currentprocess] == "Ready" || stateMap[Currentprocess] == "Running") {
						state = "Ready/Suspend";
					}
					stateMap[process[i]] = state; //mutate the global stateMap variable with updates
					break;
				case 4: //interrupt -> ready
					if (stateMap[Currentprocess] == "Blocked/Suspend") {
						state = "Ready/Suspend";
					}
					else {
						state = "Ready";
					}					
					stateMap[process[i]] = state; //mutate the global stateMap variable with updates
					break;
				case 5: //terminated -> DELETE FROM MAP
					stateMap.erase(process[i]); //mutate the global stateMap variable with updates
					break;
				case 6: //expires -> ready
					state = "Ready";
					stateMap[process[i]] = state; //mutate the global stateMap variable with updates
					break;
				}
			}

		}
	}
	return process; //return the accumulated list of modified processes
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
		stateMapTwo.insert(pair<string, string>(keyTwo, valueTwo));
	}

	//send each individual line to the line parser
	for (int j = 1; j < inTwo.size(); j++) {
		cout << inTwo[j] << endl; //print the original line
		vector<string> processesTwo = instruction_parserTwo(inTwo[j]); //returns the modified processes from that line

		//print map
		map<string, string>::iterator it2;
		for (it2 = stateMapTwo.begin(); it2 != stateMapTwo.end(); ++it2) { //for each state in the map
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