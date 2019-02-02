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

using namespace std;



string instruction_parser (string initalInstruc);

string instruction_parser (string initalInstruc) {

	string test = "";

	/*
	"Ready"
	"Running"
	"Blocked"
	"New"
	"Blocked/Suspend"

	"At time "
	"requests"
	"Time slice"
	"expires"
	"is dispatched"
	*/
	


	return test;
}

vector<string> split (string content, string delim);

vector<string> split (string content, string delim) {
	vector<string> inits;
	
	int position = 0;
	string token;

	while ((position = content.find(delim)) != string::npos) {
		token = content.substr(0, position);
		inits.push_back(token);
		content.erase(0, position + delim.length());
	}
	inits.push_back(content); //Add watever is left, with no delimeters

	// for (int i = 0; i != inits.size(); i++) {
	//	cout << inits[i] << endl;
	//}

	return inits;
}


int main() {

	printf("Simulation Begins \n");
	printf("Initial State \n");

	ifstream input("input1.txt");

	int count = 0;
	string in[50];
	string data;
	map<string, string> stateMap;

	if (input.good())
	{
		while (getline(input, data)) //while there are lines to read, read them
		{
			in[count] = data;		//store line in index postion counter
			printf("%s\n", in[count].c_str());
			count++;				//increase counter
		}
		count = 0; //reset counter
	}
	input.close();

	//parsing inital values
	string initLine = in[0];
	vector<string> initLineArgs = split(initLine, " ");
	for (int i = 0; i != initLineArgs.size(); i+=2) {
		
	}

	
	/*if (initLine.find(";") != string::npos) {

	}
	*/

	

}