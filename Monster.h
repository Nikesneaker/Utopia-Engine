#pragma once
#include<string>
#include<iostream>

using std::cin;
using std::cout;
using std::endl;
using std::string;

class Monster
{
public:
	Monster(int zone, int encounterLevel, bool treasure1Found, bool treasure6Found, bool construct4Activated);
	~Monster();
	int maxAttack, hitRange, encounterLevel, zone;
	bool isSpirit;
	string name;
};

