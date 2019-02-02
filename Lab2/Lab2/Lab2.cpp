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

vector<string> split(string content, string delim);

void instruction_parser(string instruct, map<string, string> dic);



void instruction_parser(string instruct, map<string, string> dic) {

	vector<string> instructVector;

	instructVector.push_back(instruct);

	if (instruct.find(";") != string::npos) {

		instructVector = split(instruct, ";");

	}


	for (std::size_t i = 0; i < instructVector.size(); i++) {

		string node;

		map<string, string>::iterator it1;
		for (it1 = dic.begin(); it1 != dic.end(); ++it1) {
			if (instructVector[i].find(it1->first) != string::npos) {
				node = it1->first;
			}
		}





	}



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


}



vector<string> split(string content, string delim) {
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

	for (int i = 0; i != initLineArgs.size(); i += 2) {

		//add i as key and i+1 as value to dictioanry

		string key = initLineArgs[i];

		string value = initLineArgs[i + 1];

		stateMap.insert(pair<string, string>(key, value));

	}

	map<string, string>::iterator it1;

	for (it1 = stateMap.begin(); it1 != stateMap.end(); ++it1)
		cout << it1->first << "->" << it1->second << endl;






}