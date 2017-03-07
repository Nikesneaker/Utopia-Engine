#include<iostream>
#include<chrono>
#include<thread>
#include<set>
#include<stdlib.h>
#include<time.h>
#include"Monster.h"
#ifdef __cplusplus__

#include <string>

void ClearScreen() //declare function that clears the screen
{
	cout << string(100, '\n');
}

#else

#include <stdio.h>

void ClearScreen()
{
	int n;
	for (n = 0; n < 10; n++)
		printf("\n\n\n\n\n\n\n\n\n\n");
}

#endif 

using std::cout;
using std::endl;
using std::cin;
using std::set;
using namespace std::chrono;
using namespace std::this_thread;

//This function outputs instructions and an introduction for the player
void intro()
{
	cout << "Welcome to Utopia Engine! (Press enter to continue.)" << endl;
	cin.get();
	cout << "In this game you play as Isodoros, a talented Artificer who has been charged with reconstructing a fabled device called the Utopia Engine." << endl;
	cin.get();
	cout << "Your goal is to activate the Utopia Engine before Doomsday arrives. To do this you must search six deadly regions to find the six Constructs that make up the Utopia Engine." << endl;
	cin.get();
	cout << "After finding the Constructs, you must activate them and assemble them into the Utopia Engine. Only after you've reassembled the Engine can you finally attempt to bring it to life and avert the apocalypse." << endl;
	cin.get();
	cout << "Time will be a constant pressure. However, you may be able to produce enough excess energy by activating Constructs to power the God's Hand device for a short time. This will briefly delay Doomsday, which may give you the time you need to complete your task." << endl;
	cin.get();
	cout << "You are also equipped with a tool belt carrying three personal artifacts to help you in your quest." << endl;
	cin.get();
	cout << "If you die in wilderness or fail to activate the Utopia Engine in time, the game ends and you will earn a number of points based on your progress." << endl;
	cin.get();
	cout << "Good luck!" << endl;
	cin.get();
	ClearScreen();
}

//This function is called any time that the player is searching an area
int diceRollSearch();

//This function is called any time that the player attempts to activate a construct
int diceRollActivate(int &health); 

//This function allows the player to take up to 10 points off of their search result
void takeUpToTenOffSearchResult(int &result);

//This function is called on specific event days, and rerolls the events in each region
void rerollEventDays(int &activeMonsters, int &fleetingVisions, int &goodFortune, int &foulWeather);

//This function is called any time the player takes damage to see if they have been killed or knocked unconcious.
void checkIfDead(int &hp, bool constructActivated, bool &fightOver, int &day);

void main() {
	srand(time(NULL));
#pragma region Variables
	bool gameOver = false;	//do while(!gameover)
	int day = 0; //the day, initialized to 0 so the first search can increment by oen
	set<int> eventDays = { 2, 5, 8, 11, 14, 17, 20 }; //set so i could use .find()
	int Doomsday = 15; //can be moved later (added to)
	int lead = 0, quartz = 0, silica = 0, gum = 0, wax = 0, silver = 0; //components
	bool dowsingRodUsed = false, paralysisWandUsed = false, focusCharmUsed = false; //artifacts that you start with
	int hp = 6; //6 is max health
	int godsHandCharge = 0; //once it gets to so many charges, can push doomsday back
	int activeMonstersEvent = 0, fleetingVisionsEvent = 0, goodFortuneEvent = 0, foulWeatherEvent = 0; //four event days
	int HalebeardPeak[6] = { 1, 1, 0, 1, 0, 0 }; //each region, ones mean that the search takes a day
	int TheGreatWilds[6] = { 1, 0, 0, 1, 0, 0 };
	int RootStrangledMarshes[6] = { 1, 0, 1, 0, 1, 0 };
	int GlassrockCanyon[6] = { 1, 0, 1, 0, 1, 0 };
	int RuinedCityOfTheAncients[6] = { 1, 0, 0, 1, 0, 0 };
	int TheFieryMaw[6] = { 1, 1, 0, 1, 0, 0 };
	bool construct1Found = false, construct2Found = false, construct3Found = false, construct4Found = false, construct5Found = false, construct6Found = false;
	bool construct1Activated = false, construct2Activated = false, construct3Activated = false, construct4Activated = false, construct5Activated = false, construct6Activated = false;
	bool treasure1Found = false, treasure2Found = false, treasure3Found = false, treasure4Found = false, treasure5Found = false, treasure6Found = false;
	bool connection12 = false, connection14 = false, connection15 = false, connection46 = false, connection43 = false, connection23 = false;
	bool noConnections = true, noConstructs = true;
	int wasteBasket; //values can be moved to the waste basket during activation
#pragma endregion

	intro();

	do
	{
#pragma region Tool Belt
		cout << "Stores:" << endl << "Lead: " << lead << " Quartz: " << quartz << " Silica: " << silica << " Gum: " << gum << " Wax: " << wax << " Silver: " << silver << endl;
		cout << "Day: " << day << endl; cout << "Doomsday in " << Doomsday - day << " days. HP remaining: " << hp << ". God's Hand Charges: " << godsHandCharge << endl << endl;

		cout << "Event Cycles: " << endl;
		if (activeMonstersEvent == 0) {
			cout << "Not active." << endl << endl;
		}
		else
		{
			cout << "Active Monsters (+2 Levels to all encounters): " << activeMonstersEvent << endl;
			cout << "Fleeting Visions (Add one energy point when activating this construct): " << fleetingVisionsEvent << endl;
			cout << "Good Fortune (Up to -10 on all the search results): " << goodFortuneEvent << endl;
			cout << "Foul Weather (Lose one additional day each time a day is lost): " << foulWeatherEvent << endl << endl;
		}
		cout << "Available Tools: " << endl;
		if (dowsingRodUsed == true && paralysisWandUsed == true && focusCharmUsed == true)
			cout << "None" << endl;
		if (dowsingRodUsed == false)
		{
			cout << "Dowsing Rod (Up to -100 on any search)" << endl;
		}
		if (paralysisWandUsed == false)
		{
			cout << "Paralysis Wand (+2 to each die in combat)" << endl;
		}
		if (focusCharmUsed == false)
		{
			cout << "Focus Charm (Adds 2 energy points during activation)" << endl;
		}
		cout << endl << "Constructs Found: " << endl;
		if (construct1Found == false && construct2Found == false && construct3Found == false && construct4Found == false && construct5Found == false && construct6Found == false)
		{
			cout << "None" << endl;
		}
		if (construct1Found) {
			cout << "Seal of Balance (cancel event in a region once per game) - ";
			if (!construct1Activated) cout << "Not Activated" << endl; else cout << "Activated" << endl;
		}
		if (construct2Found) {
			cout << "Hermetic Mirror (subtract up to 10 from a search result in Halebeard Peaks or Fiery Maw) - ";
			if (!construct2Activated) cout << "Not Activated" << endl; else cout << "Activated" << endl;
		}
		if (construct3Found) {
			cout << "Void Gate (you recover from unconciousness in 4 days instead of 6) - ";
			if (!construct3Activated) cout << "Not Activated" << endl; else cout << "Activated" << endl;
		}
		if (construct4Found) {
			cout << "Golden Chassis (lowers the attack and hit range of spirits by 1) - ";
			if (!construct4Activated) cout << "Not Activated" << endl; else cout << "Activated" << endl;
		}
		if (construct5Found) {
			cout << "Scrying Lens (subtract up to 10 from a search result in Glassrock Canyon or Root Strangled Marshes) - ";
			if (!construct5Activated) cout << "Not Activated" << endl; else cout << "Activated" << endl;
		}
		if (construct6Found) {
			cout << "Crystal Battery (can spend 3 components to recharge a tool belt artifact once per game) - ";
			if (!construct6Activated) cout << "Not Activated" << endl; else cout << "Activated" << endl;
		}
#pragma endregion
		//ask for options, will be more
		cout << endl << "What would you like to do? S for Search, W for Workshop, R for Rest (1 hp/day), G for God's Hand" << endl;
		char action;
		//make sure it's s or w or r or g
		do
		{
			cin >> action;
			if (tolower(action != 's') && tolower(action) != 'w' && tolower(action) != 'r' && tolower(action) != 'g')
			{
				cout << "Invalid option. S for Search, W for Workshop, R for Rest, G for God's Hand" << endl;
			}
		} while (tolower(action) != 's' && tolower(action) != 'w' && tolower(action) != 'r' && tolower(action) != 'g');

		if (tolower(action) == 's') //search
		{
			int zone;
#pragma region Region Search Output
			cout << "Which region would you like to search?" << endl;
			cout << "1 for Halebeard Peak (days lost per search: 1, 1, 0, 1, 0, 0) - Contruct: Seal of Balance - ";
			if (construct1Found == true)
			{
				cout << "Found." << endl;
			}
			else cout << "Not found." << endl;
			cout << "2 for The Great Wilds (days lost per search: 1, 0, 0, 1, 0, 0) - Construct: Hermetic Mirror - ";
			if (construct2Found == true)
			{
				cout << "Found." << endl;
			}
			else cout << "Not found." << endl;
			cout << "3 for Root Strangled Marshes (days lost per search: 1, 0, 1, 0, 1, 0) - Construct: Void Gate - ";
			if (construct3Found == true)
			{
				cout << "Found." << endl;
			}
			else cout << "Not found." << endl;
			cout << "4 for Glassrock Canyon (days lost per search: 1, 0, 1, 0, 1, 0) - Construct: Golden Chassis - ";
			if (construct4Found == true)
			{
				cout << "Found." << endl;
			}
			else cout << "Not found." << endl;
			cout << "5 for Ruined City of the Ancients (days lost per search: 1, 0, 0, 1, 0, 0) - Construct: Scrying Lens - ";
			if (construct5Found == true)
			{
				cout << "Found." << endl;
			}
			else cout << "Not found." << endl;
			cout << "6 for The Fiery Maw (days lost per search: 1, 1, 0, 1, 0, 0) - Construct: Crystal Battery - ";
			if (construct6Found == true)
			{
				cout << "Found." << endl;
			}
			else cout << "Not found." << endl;
#pragma endregion

			cin >> zone;
			//make sure it's between 1 and 6
			while (zone == NULL || zone < 1 || zone >6)
			{
				if (zone < 1 || zone > 6)
				{
					cout << "Invalid option. Please choose a region between 1 and 6." << endl;
					cin >> zone;
				}
			}

#pragma region More Variables
			int searchNumber; //number of times we've searched
			int die1, die2;
			int result; //difference between 2 3-digit numbers

			char searchResultRemoved;
			int searchResultAmount;

			char searchFilled;
			char searchOrNot;
#pragma endregion

			switch (zone)
			{
			case 1:
				cout << "Entering Halebeard Peak..." << endl;
				cin.get();
				searchNumber = 0;

				//loop while they want to continue searching
				do
				{
					//update days depending on search #
					searchNumber++;
					cout << "Search " << searchNumber << endl;
					day = day + HalebeardPeak[searchNumber - 1];

					//check for treasure 2 adding a god's hand charge
					if (treasure2Found == true)
					{
						if (HalebeardPeak[searchNumber - 1] == 1)
						{
							godsHandCharge++;
							cout << "Because of the Bracelet of Ios, one charge was added to the God's Hand Device for crossing a day off the time track." << endl;
							cin.get();
						}
					}

					//check for treasure4 for adding hp
					if (treasure4Found)
					{
						if (HalebeardPeak[searchNumber - 1] == 1)
						{
							hp++;
							cout << "Because of the Scale of the Infinity Wurm, you recovered one hp for crossing a day off the time track." << endl;
							cin.get();
						}
					}

					//update event day
					if (HalebeardPeak[searchNumber - 1] == 1)
					{
						for (set<int>::iterator it = eventDays.begin(); it != eventDays.end(); it++)
						{
							if (*it == day)
							{
								rerollEventDays(activeMonstersEvent, fleetingVisionsEvent, goodFortuneEvent, foulWeatherEvent);
							}
						}
					}

					//take off an extra day for searching in this region
					if (foulWeatherEvent == zone)
					{
						day = day + HalebeardPeak[searchNumber - 1];
						//check for treasure 2 adding a god's hand charge
						if (treasure2Found == true)
						{
							if (HalebeardPeak[searchNumber - 1] == 1)
							{
								godsHandCharge++;
								cout << "Because of the Bracelet of Ios, one charge was added to the God's Hand Device for crossing a day off the time track." << endl;
								cin.get();
							}
						}
						//check for treasure4 for adding hp
						if (treasure4Found)
						{
							if (HalebeardPeak[searchNumber - 1] == 1)
							{
								hp++;
								cout << "Because of the Scale of the Infinity Wurm, you recovered one hp for crossing a day off the time track." << endl;
								cin.get();
							}
						}
					}



					result = diceRollSearch();


					//adjust results based on goodFortune & if construct 2 is activated
					if (goodFortuneEvent == zone)
					{
						cout << "Good Fortune is over your current location! Would you like to take up to 10 off of your search result?  (y/n)" << endl;
						takeUpToTenOffSearchResult(result);
					}

					if (construct2Activated == true)
					{

						cout << "You currently have an activated Hermetic Mirror! Would you like to take up to 10 off of your search result? (y/n)" << endl;
						takeUpToTenOffSearchResult(result);
					}


					if (result >= 100 || result < 0) //encounter
					{
						char ignoreFight = 'n';
						if (treasure3Found)
						{
							cout << "The Shimmering Moonlace allows you to ignore any encounter. Would you like to ignore this encounter? (y/n)" << endl;
							cin >> ignoreFight;
						}
						if (tolower(ignoreFight) == 'n')
						{
							bool fightOver = false;
							int tempAttack, encounterLevel, tempRange;
							if (result >= 500 || result <= -401)
							{
								//level 5 encounter
								encounterLevel = 5;
							}
							else if (result >= 400 || result <= -301)
							{
								//level 4 encounter
								encounterLevel = 4;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 5;
								}
							}
							else if (result >= 300 || result <= -201)
							{
								//level 3 encounter
								encounterLevel = 3;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 5;
								}
							}
							else if (result >= 200 || result <= -101)
							{
								//level 2 encounter
								encounterLevel = 2;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 4;
								}
							}
							else
							{
								//level 1 encounter
								encounterLevel = 1;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 3;
								}
							}
							if (encounterLevel != 5)
							{
								Monster monster(zone, encounterLevel, treasure1Found, treasure6Found, construct4Activated);
								if (treasure1Found == true && monster.maxAttack != 1)
								{
									tempAttack = monster.maxAttack - 1;
								}
								else
								{
									tempAttack = monster.maxAttack;
								}
								if (treasure6Found)
								{
									tempRange = monster.hitRange - 1;
								}
								else
								{
									tempRange = monster.hitRange;
								}

								while (!fightOver)
								{
									die1 = rand() % 6 + 1;
									die2 = rand() % 6 + 1;
									cout << "Rolls: " << die1 << " " << die2 << endl;
									if (die1 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}
									}
									if (die2 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}
										else if (hp < 0)
										{
											gameOver = true;
											break;
										}
									}
									if (die1 >= tempRange || die2 >= tempRange)
									{
										fightOver = true;
										cout << "Congratulations! You've defeated " << monster.name << "! Press enter to see if it dropped something." << endl;
										cin.get();
										//drop something
										die1 = rand() % 6 + 1;
										cout << "Roll (You need a " << encounterLevel << " or lower to get a component): " << die1 << endl;
										if (die1 <= encounterLevel)
										{
											if (silver < 4)
											{
												silver++;
												cout << "You got a silver!" << endl;
											}
											else
											{
												cout << "Sorry, your silver stores are full!" << endl;
											}
											cin.get();
										}
										else
										{
											cout << "Sorry, not this time!" << endl;
											cin.get();
										}
									}
									cout << "Press ENTER to continue." << endl;
									cin.get();
								}
							}
							else
							{
								Monster monster(zone, encounterLevel, treasure1Found, treasure6Found, construct4Activated);

								if (treasure1Found == true && monster.maxAttack != 1)
								{
									tempAttack = monster.maxAttack - 1;
								}
								else
								{
									tempAttack = monster.maxAttack;
								}
								if (treasure6Found)
								{
									tempRange = monster.hitRange - 1;
								}
								else
								{
									tempRange = monster.hitRange;
								}

								while (!fightOver)
								{
									die1 = rand() % 6 + 1;
									die2 = rand() % 6 + 1;
									cout << "Rolls: " << die1 << " " << die2 << endl;
									if (die1 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}


									}
									if (die2 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}
										else if (hp < 0)
										{
											gameOver = true;
											break;
										}
									}
									if (die1 >= tempRange || die2 >= tempRange)
									{
										fightOver = true;
										cout << "Congratulations! You've defeated " << monster.name << "! Press enter to see if it dropped something." << endl;
										cin.get();
										//drop something
										die1 = rand() % 6 + 1;
										cout << "Roll (You need a " << encounterLevel << " or lower to get the Legendary Treasure): " << die1 << endl;
										if (die1 <= encounterLevel)
										{
											if (treasure1Found == false)
											{
												treasure1Found = true;
												cout << "The Giant of the Peaks dropped the Ice Plate!" << endl;
												cin.get();
												cout << "The Ice Plate prevented his vengeful soul from escaping the empty cavity of his chest." << endl;
												cin.get();
												cout << "It subtracts 1 from the ATK range of all monsters. (Minimum ATK range is 1)" << endl;
											}
											else
											{
												cout << "You've already obtained his treasure... here's a component instead!" << endl;
												if (silver < 4)
												{
													silver++;
												}
												else
												{
													cout << "Sorry, your silver stores are full!" << endl;
												}
												cin.get();
											}
										}
										else
										{
											cout << "Sorry, not this time!" << endl;
										}
									}
									cout << "Press ENTER to continue." << endl;
									cin.get();
								}
							}
						}
					}
					else if (result >= 11)
					{
						//find a component	
						if (silver < 4) //change depending on zone
						{
							silver++; //change depending on zone
							cout << "You found a silver!" << endl;
						}
						else
						{
							cout << "Sorry, your silver stores are full!" << endl;
						}
						cin.get();
					}
					else if (result >= 1)
					{
						//find a construct
						if (construct1Found == true)
						{
							cout << "You've already found the Seal of Balance... here are two silvers instead!" << endl;
							if (silver == 4)
							{
								cout << "Your silver bags are full!" << endl;
								cin.get();
							}
							else if (silver == 3)
							{
								cout << "Your silver bags were filled to capacity! (4)" << endl;
								cin.get();
							}
							else silver += 2;
						}
						else
						{
							construct1Found = true;		//change depending on zone
							noConstructs = false;
							cout << "You found the Seal of Balance!" << endl;
							cin.get();
							cout << "Once you activate it, you can use it once per game to cancel all events in any one region!" << endl;
							cin.get();
						}
					}
					else if (result == 0)
					{
						construct1Found = true;		//change depending on zone
						noConstructs = false;
						construct1Activated = true;
						godsHandCharge += 5;
						cout << "WOW! A perfect result!" << endl;
						cin.get();
						cout << "You found an activated Seal of Balance! You can use it once per game to cancel all events in any one region." << endl;
						cin.get();
						cout << "The God's Hand now also has 5 more charges in it!" << endl;
					}
					if (searchNumber == 6)
					{
						cout << "You have searched all 6 times. Would you like to cross off an additional day in order to find the construct (or a component)? (y/n)" << endl;
						cin >> searchFilled;
						if (searchFilled == 'y')
						{
							day++;
							//check for treasure4 for adding hp
							if (treasure4Found)
							{
								hp++;
								cout << "Because of the Scale of the Infinity Wurm, you recovered one hp for crossing a day off the time track." << endl;
								cin.get();
							}
							if (treasure2Found)
							{

								godsHandCharge++;
								cout << "Because of the Bracelet of Ios, one charge was added to the God's Hand Device for crossing a day off the time track." << endl;
								cin.get();

							}
							if (construct1Found == true)
							{
								if (silver < 4)
								{
									cout << "You found a silver!" << endl;
									silver++;
								}
								else
								{
									cout << "Your silver stores are full!" << endl;
								}
							}
							else
							{
								construct1Found = true;
								noConstructs = false;
								cout << "You found the Seal of Balance!" << endl;
								cin.get();
								cout << "Once you activate it, you can use it once per game to cancel all events in any one region!" << endl;
								cin.get();
							}

						}
					}
					else
					{
						cout << "Continue searching in this region? (y/n)" << endl;
						cin >> searchOrNot;
					}
				} while (searchOrNot == 'y' && searchNumber != 6);


				break;
			case 2:
				cout << "Entering the Great Wilds..." << endl;
				cin.get();
				searchNumber = 0;

				//loop while they want to continue searching
				do
				{
					//update days depending on search #
					searchNumber++;
					cout << "Search " << searchNumber << endl;
					day = day + TheGreatWilds[searchNumber - 1];

					//check for treasure 2 adding a god's hand charge
					if (treasure2Found == true)
					{
						if (TheGreatWilds[searchNumber - 1] == 1)
						{
							godsHandCharge++;
							cout << "Because of the Bracelet of Ios, one charge was added to the God's Hand Device for crossing a day off the time track." << endl;
							cin.get();
						}
					}

					//check for treasure4 for adding hp
					if (treasure4Found)
					{
						if (HalebeardPeak[searchNumber - 1] == 1)
						{
							hp++;
							cout << "Because of the Scale of the Infinity Wurm, you recovered one hp for crossing a day off the time track." << endl;
							cin.get();
						}
					}

					//update event day
					if (TheGreatWilds[searchNumber - 1] == 1)
					{
						for (set<int>::iterator it = eventDays.begin(); it != eventDays.end(); it++)
						{
							if (*it == day)
							{
								rerollEventDays(activeMonstersEvent, fleetingVisionsEvent, goodFortuneEvent, foulWeatherEvent);
							}
						}
					}

					//take off an extra day for searching in this region
					if (foulWeatherEvent == zone)
					{
						day = day + TheGreatWilds[searchNumber - 1];
						//check for treasure 2 adding a god's hand charge
						if (treasure2Found == true)
						{
							if (TheGreatWilds[searchNumber - 1] == 1)
							{
								godsHandCharge++;
								cout << "Because of the Bracelet of Ios, one charge was added to the God's Hand Device for crossing a day off the time track." << endl;
								cin.get();
							}
						}
						//check for treasure4 for adding hp
						if (treasure4Found)
						{
							if (HalebeardPeak[searchNumber - 1] == 1)
							{
								hp++;
								cout << "Because of the Scale of the Infinity Wurm, you recovered one hp for crossing a day off the time track." << endl;
								cin.get();
							}
						}
					}

					result = diceRollSearch();

					//adjust results based on goodFortune
					if (goodFortuneEvent == zone)
					{
						cout << "Good Fortune is over your current location! Would you like to take up to 10 off of your search result?  (y/n)" << endl;
						takeUpToTenOffSearchResult(result);
					}

					if (result >= 100 || result < 0)
					{
						char ignoreFight = 'n';
						if (treasure3Found)
						{
							cout << "The Shimmering Moonlace allows you to ignore any encounter. Would you like to ignore this encounter? (y/n)" << endl;
							cin >> ignoreFight;
						}
						if (tolower(ignoreFight) == 'n')
						{
							bool fightOver = false;
							int tempAttack, encounterLevel, tempRange;
							if (result >= 500 || result <= -401)
							{
								//level 5 encounter
								encounterLevel = 5;

							}
							else if (result >= 400 || result <= -301)
							{
								//level 4 encounter
								encounterLevel = 4;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 5;
								}
							}
							else if (result >= 300 || result <= -201)
							{
								//level 3 encounter
								encounterLevel = 3;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 5;
								}
							}
							else if (result >= 200 || result <= -101)
							{
								//level 2 encounter
								encounterLevel = 2;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 4;
								}
							}
							else
							{
								//level 1 encounter
								encounterLevel = 1;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 3;
								}
							}
							if (encounterLevel != 5)
							{
								Monster monster(zone, encounterLevel, treasure1Found, treasure6Found, construct4Activated);
								if (treasure1Found == true && monster.maxAttack != 1)
								{
									tempAttack = monster.maxAttack - 1;
								}
								else
								{
									tempAttack = monster.maxAttack;
								}
								if (treasure6Found)
								{
									tempRange = monster.hitRange - 1;
								}
								else
								{
									tempRange = monster.hitRange;
								}

								while (!fightOver)
								{
									die1 = rand() % 6 + 1;
									die2 = rand() % 6 + 1;
									cout << "Rolls: " << die1 << " " << die2 << endl;
									if (die1 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}
									}
									if (die2 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}
										else if (hp < 0)
										{
											gameOver = true;
											break;
										}
									}
									if (die1 >= tempRange || die2 >= tempRange)
									{
										fightOver = true;
										cout << "Congratulations! You've defeated " << monster.name << "! Press enter to see if it dropped something." << endl;
										cin.get();
										//drop something
										die1 = rand() % 6 + 1;
										cout << "Roll (You need a " << encounterLevel << " or lower to get a component): " << die1 << endl;
										if (die1 <= encounterLevel)
										{
											if (quartz < 4)
											{
												quartz++;
												cout << "You got a quartz!" << endl;
											}
											else
											{
												cout << "Sorry, your quartz stores are full!" << endl;
											}
											cin.get();
										}
										else
										{
											cout << "Sorry, not this time!" << endl;
											cin.get();
										}
									}
									cout << "Press ENTER to continue." << endl;
									cin.get();
								}
							}
							else
							{
								Monster monster(zone, encounterLevel, treasure1Found, treasure6Found, construct4Activated);

								if (treasure1Found == true && monster.maxAttack != 1)
								{
									tempAttack = monster.maxAttack - 1;
								}
								else
								{
									tempAttack = monster.maxAttack;
								}
								if (treasure6Found)
								{
									tempRange = monster.hitRange - 1;
								}
								else
								{
									tempRange = monster.hitRange;
								}

								while (!fightOver)
								{
									die1 = rand() % 6 + 1;
									die2 = rand() % 6 + 1;
									cout << "Rolls: " << die1 << " " << die2 << endl;
									if (die1 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}
									}
									if (die2 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}
										else if (hp < 0)
										{
											gameOver = true;
											break;
										}
									}
									if (die1 >= tempRange || die2 >= tempRange)
									{
										fightOver = true;
										cout << "Congratulations! You've defeated the Thunder King! Press enter to see if it dropped something." << endl;
										cin.get();
										//drop something
										die1 = rand() % 6 + 1;
										cout << "Roll (You need a " << encounterLevel << " or lower to get the Legendary Treasure): " << die1 << endl;
										if (die1 <= encounterLevel)
										{
											if (treasure2Found == false)
											{
												treasure2Found = true;
												cout << "The Thunder King dropped the Bracelet of Ios!" << endl;
												cin.get();
												cout << "The Bracelet of Ios was a treasure worn by the Thunder King that is said to discharge bolts of lightning when he is enraged." << endl;
												cin.get();
												cout << "It adds 1 energy point to the God's Hand Device each time you cross out a day on the time track." << endl;
											}
											else
											{
												cout << "You've already obtained his treasure... here's a component instead!" << endl;
												if (quartz < 4)
												{
													quartz++;
												}
												else
												{
													cout << "Sorry, your quartz stores are full!" << endl;
												}
												cin.get();
											}
										}
										else
										{
											cout << "Sorry, not this time!" << endl;
										}
									}
									cout << "Press ENTER to continue." << endl;
									cin.get();
								}
							}
						}



					}

					else if (result >= 11)
					{
						//find a component	
						if (quartz < 4) //change depending on zone
						{
							quartz++; //change depending on zone
							cout << "You found a quartz!" << endl;
						}
						else
						{
							cout << "Sorry, your quartz stores are full!" << endl;
						}
						cin.get();
					}
					else if (result >= 1)
					{
						//find a construct
						if (construct2Found == true)
						{
							cout << "You've already found the Hermetic Mirror... here are two quartz instead!" << endl;
							if (quartz == 4)
							{
								cout << "Your quartz bags are full!" << endl;
								cin.get();
							}
							else if (quartz == 3)
							{
								cout << "Your quartz bags were filled to capacity! (4)" << endl;
								cin.get();
							}
							else quartz += 2;
						}
						else
						{
							construct2Found = true;		//change depending on zone
							noConstructs = false;
							cout << "You found the Hermetic Mirror!" << endl;
							cin.get();
							cout << "Once you activate it, you may subtract up to 10 from any search result in the Halebeard Peaks or the Fiery Maw!" << endl;
							cin.get();
						}
					}
					else if (result == 0)
					{
						construct2Found = true;		//change depending on zone
						noConstructs = false;
						construct2Activated = true;
						godsHandCharge += 5;
						cout << "WOW! A perfect result!" << endl;
						cin.get();
						cout << "You found an activated Hermetic Mirror! You may subtract up to 10 from any search result in the Halebeard Peaks or the Fiery Maw!" << endl;
						cin.get();
						cout << "The God's Hand now also has 5 more charges in it!" << endl;
					}
					if (searchNumber == 6)
					{
						cout << "You have searched all 6 times. Would you like to cross off an additional day in order to find the construct (or a component)? (y/n)" << endl;
						cin >> searchFilled;
						if (searchFilled == 'y')
						{
							day++;
							//check for treasure4 for adding hp
							if (treasure4Found)
							{
								hp++;
								cout << "Because of the Scale of the Infinity Wurm, you recovered one hp for crossing a day off the time track." << endl;
								cin.get();
							}
							if (treasure2Found == true)
							{
								godsHandCharge++;
								cout << "Because of the Bracelet of Ios, one charge was added to the God's Hand Device for crossing a day off the time track." << endl;
								cin.get();
							}

							if (construct2Found == true)
							{
								if (quartz < 4)
								{
									cout << "You found a quartz!" << endl;
									quartz++;
								}
								else
								{
									cout << "Your quartz stores are full!" << endl;
								}
							}
							else
							{
								construct2Found = true;
								noConstructs = false;
								cout << "You found the Hermetic Mirror!" << endl;
								cin.get();
								cout << "Once you activate it, you may subtract up to 10 from any search result in the Halebeard Peaks or the Fiery Maw!" << endl;
								cin.get();
							}

						}
					}
					else
					{
						cout << "Continue searching in this region? (y/n)" << endl;
						cin >> searchOrNot;
					}
				} while (searchOrNot == 'y' && searchNumber != 6);
				break;
			case 3:
				cout << "Entering the Root Strangled Marshes..." << endl;
				cin.get();
				searchNumber = 0;

				//loop while they want to continue searching
				do
				{
					//update days depending on search #
					searchNumber++;
					cout << "Search " << searchNumber << endl;
					day = day + RootStrangledMarshes[searchNumber - 1];

					//check for treasure 2 adding a god's hand charge
					if (treasure2Found == true)
					{
						if (RootStrangledMarshes[searchNumber - 1] == 1)
						{
							godsHandCharge++;
							cout << "Because of the Bracelet of Ios, one charge was added to the God's Hand Device for crossing a day off the time track." << endl;
							cin.get();
						}
					}

					//check for treasure4 for adding hp
					if (treasure4Found)
					{
						if (HalebeardPeak[searchNumber - 1] == 1)
						{
							hp++;
							cout << "Because of the Scale of the Infinity Wurm, you recovered one hp for crossing a day off the time track." << endl;
							cin.get();
						}
					}

					//update event day
					if (RootStrangledMarshes[searchNumber - 1] == 1)
					{
						for (set<int>::iterator it = eventDays.begin(); it != eventDays.end(); it++)
						{
							if (*it == day)
							{
								rerollEventDays(activeMonstersEvent, fleetingVisionsEvent, goodFortuneEvent, foulWeatherEvent);
							}
						}
					}


					//take off an extra day for searching in this region
					if (foulWeatherEvent == zone)
					{
						day = day + RootStrangledMarshes[searchNumber - 1];
						//check for treasure 2 adding a god's hand charge
						if (treasure2Found == true)
						{
							if (RootStrangledMarshes[searchNumber - 1] == 1)
							{
								godsHandCharge++;
								cout << "Because of the Bracelet of Ios, one charge was added to the God's Hand Device for crossing a day off the time track." << endl;
								cin.get();
							}
						}

						//check for treasure4 for adding hp
						if (treasure4Found)
						{
							if (HalebeardPeak[searchNumber - 1] == 1)
							{
								hp++;
								cout << "Because of the Scale of the Infinity Wurm, you recovered one hp for crossing a day off the time track." << endl;
								cin.get();
							}
						}
					}

					result = diceRollSearch();

					//adjust results based on goodFortune
					if (goodFortuneEvent == zone)
					{
						cout << "Would you like to take up to 10 off of your search result? (y/n)" << endl;
						takeUpToTenOffSearchResult(result);
					}

					if (result >= 100 || result < 0)
					{
						char ignoreFight = 'n';
						if (treasure3Found)
						{
							cout << "The Shimmering Moonlace allows you to ignore any encounter. Would you like to ignore this encounter? (y/n)" << endl;
							cin >> ignoreFight;
						}
						if (tolower(ignoreFight) == 'n')
						{
							bool fightOver = false;
							int tempAttack, encounterLevel, tempRange;
							if (result >= 500 || result <= -401)
							{
								//level 5 encounter
								encounterLevel = 5;

							}
							else if (result >= 400 || result <= -301)
							{
								//level 4 encounter
								encounterLevel = 4;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 5;
								}
							}
							else if (result >= 300 || result <= -201)
							{
								//level 3 encounter
								encounterLevel = 3;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 5;
								}
							}
							else if (result >= 200 || result <= -101)
							{
								//level 2 encounter
								encounterLevel = 2;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 4;
								}
							}
							else
							{
								//level 1 encounter
								encounterLevel = 1;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 3;
								}
							}
							if (encounterLevel != 5)
							{
								Monster monster(zone, encounterLevel, treasure1Found, treasure6Found, construct4Activated);
								if (treasure1Found == true && monster.maxAttack != 1)
								{
									tempAttack = monster.maxAttack - 1;
								}
								else
								{
									tempAttack = monster.maxAttack;
								}
								if (treasure6Found)
								{
									tempRange = monster.hitRange - 1;
								}
								else
								{
									tempRange = monster.hitRange;
								}

								while (!fightOver)
								{
									die1 = rand() % 6 + 1;
									die2 = rand() % 6 + 1;
									cout << "Rolls: " << die1 << " " << die2 << endl;
									if (die1 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}
									}
									if (die2 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}
										else if (hp < 0)
										{
											gameOver = true;
											break;
										}
									}
									if (die1 >= tempRange || die2 >= tempRange)
									{
										fightOver = true;
										cout << "Congratulations! You've defeated " << monster.name << "! Press enter to see if it dropped something." << endl;
										cin.get();
										//drop something
										die1 = rand() % 6 + 1;
										cout << "Roll (You need a " << encounterLevel << " or lower to get a component): " << die1 << endl;
										if (die1 <= encounterLevel)
										{
											if (quartz < 4)
											{
												quartz++;
												cout << "You got a quartz!" << endl;
											}
											else
											{
												cout << "Sorry, your quartz stores are full!" << endl;
											}
											cin.get();
										}
										else
										{
											cout << "Sorry, not this time!" << endl;
											cin.get();
										}
									}
									cout << "Press ENTER to continue." << endl;
									cin.get();
								}
							}
							else
							{
								Monster monster(zone, encounterLevel, treasure1Found, treasure6Found, construct4Activated);

								if (treasure1Found == true && monster.maxAttack != 1)
								{
									tempAttack = monster.maxAttack - 1;
								}
								else
								{
									tempAttack = monster.maxAttack;
								}
								if (treasure6Found)
								{
									tempRange = monster.hitRange - 1;
								}
								else
								{
									tempRange = monster.hitRange;
								}

								while (!fightOver)
								{
									die1 = rand() % 6 + 1;
									die2 = rand() % 6 + 1;
									cout << "Rolls: " << die1 << " " << die2 << endl;
									if (die1 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}
									}
									if (die2 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}
										else if (hp < 0)
										{
											gameOver = true;
											break;
										}
									}
									if (die1 >= tempRange || die2 >= tempRange)
									{
										fightOver = true;
										cout << "Congratulations! You've defeated " << monster.name << "! Press enter to see if it dropped something." << endl;
										cin.get();
										//drop something
										die1 = rand() % 6 + 1;
										cout << "Roll (You need a " << encounterLevel << " or lower to get the Legendary Treasure): " << die1 << endl;
										if (die1 <= encounterLevel)
										{
											if (treasure3Found == false)
											{
												treasure3Found = true;
												cout << "The Dweller in the Tides dropped the Shimmering Moonlace!" << endl;
												cin.get();
												cout << "The Shimmering Moonlace is an ethereal sea plant of unknown origin that causes anything hidden within its strands to become invisible." << endl;
												cin.get();
												cout << "It allows you to ignore any encounters." << endl;
											}
											else
											{
												cout << "You've already obtained his treasure... here's a component instead!" << endl;
												if (gum < 4)
												{
													gum++;
												}
												else
												{
													cout << "Sorry, your gum stores are full!" << endl;
												}
												cin.get();
											}
										}
										else
										{
											cout << "Sorry, not this time!" << endl;
										}
									}
									cout << "Press ENTER to continue." << endl;
									cin.get();
								}
							}
						}
					}
					else if (result >= 11)
					{
						//find a component	
						if (gum < 4) //change depending on zone
						{
							gum++; //change depending on zone
							cout << "You found a gum!" << endl;
						}
						else
						{
							cout << "Sorry, your gum stores are full!" << endl;
						}
						cin.get();
					}
					else if (result >= 1)
					{
						//find a construct
						if (construct3Found == true)
						{
							cout << "You've already found the Void Gate... here are two gum instead!" << endl;
							if (gum == 4)
							{
								cout << "Your gum bags are full!" << endl;
								cin.get();
							}
							else if (gum == 3)
							{
								gum = 4;
								cout << "Your gum bags were filled to capacity! (4)" << endl;
								cin.get();
							}
							else gum += 2;
						}
						else
						{
							construct3Found = true;		//change depending on zone
							noConstructs = false;
							cout << "You found the Void Gate!" << endl;
							cin.get();
							cout << "Once you activate it, if you fall unconcious, you recover in 4 days instead of 6!" << endl;
							cin.get();
						}
					}
					else if (result == 0)
					{
						construct3Found = true;		//change depending on zone
						noConstructs = false;
						construct3Activated = true;
						godsHandCharge += 5;
						cout << "WOW! A perfect result!" << endl;
						cin.get();
						cout << "You found an activated Void Gate! If you fall unconcious, you recover in 4 days instead of 6!" << endl;
						cin.get();
						cout << "The God's Hand now also has 5 more charges in it!" << endl;
					}
					if (searchNumber == 6)
					{
						cout << "You have searched all 6 times. Would you like to cross off an additional day in order to find the construct (or a component)? (y/n)" << endl;
						cin >> searchFilled;
						if (searchFilled == 'y')
						{
							day++;
							//check for treasure4 for adding hp
							if (treasure4Found)
							{
								hp++;
								cout << "Because of the Scale of the Infinity Wurm, you recovered one hp for crossing a day off the time track." << endl;
								cin.get();
							}
							if (treasure2Found)
							{
								godsHandCharge++;
								cout << "Because of the Bracelet of Ios, one charge has been added to the God's Hand Device for crossing a day off the time track." << endl;
								cin.get();
							}
							if (construct3Found == true)
							{
								if (gum < 4)
								{
									cout << "You found a gum!" << endl;
									gum++;
								}
								else
								{
									cout << "Your gum stores are full!" << endl;
								}
							}
							else
							{
								construct3Found = true;
								noConstructs = false;
								cout << "You found the Void Gate!" << endl;
								cin.get();
								cout << "Once you activate it, if you fall unconcious, you recover in 4 days instead of 6!" << endl;
								cin.get();
							}

						}
					}
					else
					{
						cout << "Continue searching in this region? (y/n)" << endl;
						cin >> searchOrNot;
					}
				} while (searchOrNot == 'y' && searchNumber != 6);
				break;
			case 4:
				//something
				cout << "Entering Glassrock Canyon..." << endl;
				cin.get();
				searchNumber = 0;

				//loop while they want to continue searching
				do
				{
					//update days depending on search #
					searchNumber++;
					cout << "Search " << searchNumber << endl;
					day = day + GlassrockCanyon[searchNumber - 1];

					//check for treasure 2 adding a god's hand charge
					if (treasure2Found == true)
					{
						if (GlassrockCanyon[searchNumber - 1] == 1)
						{
							godsHandCharge++;
							cout << "Because of the Bracelet of Ios, one charge was added to the God's Hand Device for crossing a day off the time track." << endl;
							cin.get();
						}
					}

					//check for treasure4 for adding hp
					if (treasure4Found)
					{
						if (HalebeardPeak[searchNumber - 1] == 1)
						{
							hp++;
							cout << "Because of the Scale of the Infinity Wurm, you recovered one hp for crossing a day off the time track." << endl;
							cin.get();
						}
					}

					//update event day
					if (GlassrockCanyon[searchNumber - 1] == 1)
					{
						for (set<int>::iterator it = eventDays.begin(); it != eventDays.end(); it++)
						{
							if (*it == day)
							{
								rerollEventDays(activeMonstersEvent, fleetingVisionsEvent, goodFortuneEvent, foulWeatherEvent);
							}
						}
					}


					//take off an extra day for searching in this region
					if (foulWeatherEvent == zone)
					{
						day = day + GlassrockCanyon[searchNumber - 1];
						//check for treasure 2 adding a god's hand charge
						if (treasure2Found == true)
						{
							if (GlassrockCanyon[searchNumber - 1] == 1)
							{
								godsHandCharge++;
								cout << "Because of the Bracelet of Ios, one charge was added to the God's Hand Device for crossing a day off the time track." << endl;
								cin.get();
							}
						}

						//check for treasure4 for adding hp
						if (treasure4Found)
						{
							if (HalebeardPeak[searchNumber - 1] == 1)
							{
								hp++;
								cout << "Because of the Scale of the Infinity Wurm, you recovered one hp for crossing a day off the time track." << endl;
								cin.get();
							}
						}
					}

					result = diceRollSearch();

					//adjust results based on goodFortune
					if (goodFortuneEvent == zone)
					{
						cout << "Would you like to take up to 10 off of your search result? (y/n)" << endl;
						takeUpToTenOffSearchResult(result);
					}

					if (result >= 100 || result < 0)
					{
						char ignoreFight = 'n';
						if (treasure3Found)
						{
							cout << "The Shimmering Moonlace allows you to ignore any encounter. Would you like to ignore this encounter? (y/n)" << endl;
							cin >> ignoreFight;
						}
						if (tolower(ignoreFight) == 'n')
						{
							bool fightOver = false;
							int tempAttack, encounterLevel, tempRange;
							if (result >= 500 || result <= -401)
							{
								//level 5 encounter
								encounterLevel = 5;

							}
							else if (result >= 400 || result <= -301)
							{
								//level 4 encounter
								encounterLevel = 4;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 5;
								}
							}
							else if (result >= 300 || result <= -201)
							{
								//level 3 encounter
								encounterLevel = 3;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 5;
								}
							}
							else if (result >= 200 || result <= -101)
							{
								//level 2 encounter
								encounterLevel = 2;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 4;
								}
							}
							else
							{
								//level 1 encounter
								encounterLevel = 1;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 3;
								}
							}
							if (encounterLevel != 5)
							{
								Monster monster(zone, encounterLevel, treasure1Found, treasure6Found, construct4Activated);
								if (treasure1Found == true && monster.maxAttack != 1)
								{
									tempAttack = monster.maxAttack - 1;
								}
								else
								{
									tempAttack = monster.maxAttack;
								}
								if (treasure6Found)
								{
									tempRange = monster.hitRange - 1;
								}
								else
								{
									tempRange = monster.hitRange;
								}

								while (!fightOver)
								{
									die1 = rand() % 6 + 1;
									die2 = rand() % 6 + 1;
									cout << "Rolls: " << die1 << " " << die2 << endl;
									if (die1 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}
									}
									if (die2 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}
										else if (hp < 0)
										{
											gameOver = true;
											break;
										}
									}
									if (die1 >= tempRange || die2 >= tempRange)
									{
										fightOver = true;
										cout << "Congratulations! You've defeated " << monster.name << "! Press enter to see if it dropped something." << endl;
										cin.get();
										//drop something
										die1 = rand() % 6 + 1;
										cout << "Roll (You need a " << encounterLevel << " or lower to get a component): " << die1 << endl;
										if (die1 <= encounterLevel)
										{
											if (quartz < 4)
											{
												quartz++;
												cout << "You got a quartz!" << endl;
											}
											else
											{
												cout << "Sorry, your quartz stores are full!" << endl;
											}
											cin.get();
										}
										else
										{
											cout << "Sorry, not this time!" << endl;
											cin.get();
										}
									}
									cout << "Press ENTER to continue." << endl;
									cin.get();
								}
							}
							else
							{
								Monster monster(zone, encounterLevel, treasure1Found, treasure6Found, construct4Activated);

								if (treasure1Found == true && monster.maxAttack != 1)
								{
									tempAttack = monster.maxAttack - 1;
								}
								else
								{
									tempAttack = monster.maxAttack;
								}
								if (treasure6Found)
								{
									tempRange = monster.hitRange - 1;
								}
								else
								{
									tempRange = monster.hitRange;
								}

								while (!fightOver)
								{
									die1 = rand() % 6 + 1;
									die2 = rand() % 6 + 1;
									cout << "Rolls: " << die1 << " " << die2 << endl;
									if (die1 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}
									}
									if (die2 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}
										else if (hp < 0)
										{
											gameOver = true;
											break;
										}
									}
									if (die1 >= tempRange || die2 >= tempRange)
									{
										fightOver = true;
										cout << "Congratulations! You've defeated " << monster.name << "! Press enter to see if it dropped something." << endl;
										cin.get();
										//drop something
										die1 = rand() % 6 + 1;
										cout << "Roll (You need a " << encounterLevel << " or lower to get the Legendary Treasure): " << die1 << endl;
										if (die1 <= encounterLevel)
										{
											if (treasure4Found == false)
											{
												treasure4Found = true;
												cout << "The Leviathan Serpent dropped the Scale of the Infinity Wurm!" << endl;
												cin.get();
												cout << "The Scale of the Infinity Wurm is a large, flat scale that bestows rapid healing, shed from an Ancient diety called the Infinity Wurm." << endl;
												cin.get();
												cout << "It allows you to recover 1 hp each time you cross out a day on the time track." << endl;
											}
											else
											{
												cout << "You've already obtained his treasure... here's a component instead!" << endl;
												if (silica < 4)
												{
													silica++;
												}
												else
												{
													cout << "Sorry, your silica stores are full!" << endl;
												}
												cin.get();
											}
										}
										else
										{
											cout << "Sorry, not this time!" << endl;
										}
									}
									cout << "Press ENTER to continue." << endl;
									cin.get();
								}
							}
						}
					}
					else if (result >= 11)
					{
						//find a component	
						if (silica < 4) //change depending on zone
						{
							silica++; //change depending on zone
							cout << "You found a silica!" << endl;
						}
						else
						{
							cout << "Sorry, your silica stores are full!" << endl;
						}
						cin.get();
					}
					else if (result >= 1)
					{
						//find a construct
						if (construct4Found == true)
						{
							cout << "You've already found the Golden Chassis... here are two silica instead!" << endl;
							if (silica == 4)
							{
								cout << "Your silica bags are full!" << endl;
								cin.get();
							}
							else if (silica == 3)
							{
								silica = 4;
								cout << "Your silica bags were filled to capacity! (4)" << endl;
								cin.get();
							}
							else silica += 2;
						}
						else
						{
							construct4Found = true;		//change depending on zone
							noConstructs = false;
							cout << "You found the Golden Chassis!" << endl;
							cin.get();
							cout << "Once you activate it, you lower the max attack range and hit range of spirits by one!" << endl;
							cin.get();
						}
					}
					else if (result == 0)
					{
						construct4Found = true;		//change depending on zone
						noConstructs = false;
						construct4Activated = true;
						godsHandCharge += 5;
						cout << "WOW! A perfect result!" << endl;
						cin.get();
						cout << "You found an activated Golden Chassis! You lower the max attack range and hit range of spirits by one!" << endl;
						cin.get();
						cout << "The God's Hand now also has 5 more charges in it!" << endl;
					}
					if (searchNumber == 6)
					{
						cout << "You have searched all 6 times. Would you like to cross off an additional day in order to find the construct (or a component)? (y/n)" << endl;
						cin >> searchFilled;
						if (searchFilled == 'y')
						{
							day++;
							if (construct4Found == true)
							{
								if (silica < 4)
								{
									cout << "You found a silica!" << endl;
									silica++;
								}
								else
								{
									cout << "Your silica stores are full!" << endl;
								}
							}
							else
							{
								construct4Found = true;
								noConstructs = false;
								cout << "You found the Golden Chassis!" << endl;
								cin.get();
								cout << "Once you activate it, you lower the max attack range and hit range of spirits by one!" << endl;
								cin.get();
							}

						}
					}
					else
					{
						cout << "Continue searching in this region? (y/n)" << endl;
						cin >> searchOrNot;
					}
				} while (searchOrNot == 'y' && searchNumber != 6);
				break;
			case 5:
				//something
				cout << "Entering the Ruined City of the Ancients..." << endl;
				cin.get();
				searchNumber = 0;

				//loop while they want to continue searching
				do
				{
					//update days depending on search #
					searchNumber++;
					cout << "Search " << searchNumber << endl;
					day = day + RuinedCityOfTheAncients[searchNumber - 1];

					//check for treasure 2 adding a god's hand charge
					if (treasure2Found == true)
					{
						if (RuinedCityOfTheAncients[searchNumber - 1] == 1)
						{
							godsHandCharge++;
							cout << "Because of the Bracelet of Ios, one charge was added to the God's Hand Device for crossing a day off the time track." << endl;
							cin.get();
						}
					}

					//check for treasure4 for adding hp
					if (treasure4Found)
					{
						if (HalebeardPeak[searchNumber - 1] == 1)
						{
							hp++;
							cout << "Because of the Scale of the Infinity Wurm, you recovered one hp for crossing a day off the time track." << endl;
							cin.get();
						}
					}

					//update event day
					if (RuinedCityOfTheAncients[searchNumber - 1] == 1)
					{
						for (set<int>::iterator it = eventDays.begin(); it != eventDays.end(); it++)
						{
							if (*it == day)
							{
								rerollEventDays(activeMonstersEvent, fleetingVisionsEvent, goodFortuneEvent, foulWeatherEvent);
							}
						}
					}


					//take off an extra day for searching in this region
					if (foulWeatherEvent == zone)
					{
						day = day + RuinedCityOfTheAncients[searchNumber - 1];
						//check for treasure 2 adding a god's hand charge
						if (treasure2Found == true)
						{
							if (RuinedCityOfTheAncients[searchNumber - 1] == 1)
							{
								godsHandCharge++;
								cout << "Because of the Bracelet of Ios, one charge was added to the God's Hand Device for crossing a day off the time track." << endl;
								cin.get();
							}
						}

						//check for treasure4 for adding hp
						if (treasure4Found)
						{
							if (HalebeardPeak[searchNumber - 1] == 1)
							{
								hp++;
								cout << "Because of the Scale of the Infinity Wurm, you recovered one hp for crossing a day off the time track." << endl;
								cin.get();
							}
						}
					}

					result = diceRollSearch();

					//adjust results based on goodFortune
					if (goodFortuneEvent == zone)
					{
						cout << "Would you like to take up to 10 off of your search result? (y/n)" << endl;
						takeUpToTenOffSearchResult(result);
					}

					if (result >= 100 || result < 0)
					{
						char ignoreFight = 'n';
						if (treasure3Found)
						{
							cout << "The Shimmering Moonlace allows you to ignore any encounter. Would you like to ignore this encounter? (y/n)" << endl;
							cin >> ignoreFight;
						}
						if (tolower(ignoreFight) == 'n')
						{
							bool fightOver = false;
							int tempAttack, encounterLevel, tempRange;
							if (result >= 500 || result <= -401)
							{
								//level 5 encounter
								encounterLevel = 5;
							}
							else if (result >= 400 || result <= -301)
							{
								//level 4 encounter
								encounterLevel = 4;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 5;
								}
							}
							else if (result >= 300 || result <= -201)
							{
								//level 3 encounter
								encounterLevel = 3;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 5;
								}
							}
							else if (result >= 200 || result <= -101)
							{
								//level 2 encounter
								encounterLevel = 2;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 4;
								}
							}
							else
							{
								//level 1 encounter
								encounterLevel = 1;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 3;
								}
							}
							if (encounterLevel != 5)
							{
								Monster monster(zone, encounterLevel, treasure1Found, treasure6Found, construct4Activated);
								if (treasure1Found == true && monster.maxAttack != 1)
								{
									tempAttack = monster.maxAttack - 1;
								}
								else
								{
									tempAttack = monster.maxAttack;
								}
								if (treasure6Found)
								{
									tempRange = monster.hitRange - 1;
								}
								else
								{
									tempRange = monster.hitRange;
								}

								while (!fightOver)
								{
									die1 = rand() % 6 + 1;
									die2 = rand() % 6 + 1;
									cout << "Rolls: " << die1 << " " << die2 << endl;
									if (die1 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}
									}
									if (die2 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}
										else if (hp < 0)
										{
											gameOver = true;
											break;
										}
									}
									if (die1 >= tempRange || die2 >= tempRange)
									{
										fightOver = true;
										cout << "Congratulations! You've defeated " << monster.name << "! Press enter to see if it dropped something." << endl;
										cin.get();
										//drop something
										die1 = rand() % 6 + 1;
										cout << "Roll (You need a " << encounterLevel << " or lower to get a component): " << die1 << endl;
										if (die1 <= encounterLevel)
										{
											if (quartz < 4)
											{
												quartz++;
												cout << "You got a quartz!" << endl;
											}
											else
											{
												cout << "Sorry, your quartz stores are full!" << endl;
											}
											cin.get();
										}
										else
										{
											cout << "Sorry, not this time!" << endl;
											cin.get();
										}
									}
									cout << "Press ENTER to continue." << endl;
									cin.get();
								}
							}
							else
							{
								Monster monster(zone, encounterLevel, treasure1Found, treasure6Found, construct4Activated);

								if (treasure1Found == true && monster.maxAttack != 1)
								{
									tempAttack = monster.maxAttack - 1;
								}
								else
								{
									tempAttack = monster.maxAttack;
								}
								if (treasure6Found)
								{
									tempRange = monster.hitRange - 1;
								}
								else
								{
									tempRange = monster.hitRange;
								}

								while (!fightOver)
								{
									die1 = rand() % 6 + 1;
									die2 = rand() % 6 + 1;
									cout << "Rolls: " << die1 << " " << die2 << endl;
									if (die1 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}
									}
									if (die2 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}
										else if (hp < 0)
										{
											gameOver = true;
											break;
										}
									}
									if (die1 >= tempRange || die2 >= tempRange)
									{
										fightOver = true;
										cout << "Congratulations! You've defeated " << monster.name << "! Press enter to see if it dropped something." << endl;
										cin.get();
										//drop something
										die1 = rand() % 6 + 1;
										cout << "Roll (You need a " << encounterLevel << " or lower to get the Legendary Treasure): " << die1 << endl;
										if (die1 <= encounterLevel)
										{
											if (treasure4Found == false)
											{
												treasure4Found = true;
												cout << "The Unnamed dropped the Ancient Record!" << endl;
												cin.get();
												cout << "The Ancient Record is the ultimate treasure of the Artificers, a preserved text holding the secrets of Ancient technology." << endl;
												cin.get();
												cout << "It allows you to automatically succeed at any connection attempt." << endl;
											}
											else
											{
												cout << "You've already obtained his treasure... here's a component instead!" << endl;
												if (wax < 4)
												{
													wax++;
												}
												else
												{
													cout << "Sorry, your wax stores are full!" << endl;
												}
												cin.get();
											}
										}
										else
										{
											cout << "Sorry, not this time!" << endl;
										}
									}
									cout << "Press ENTER to continue." << endl;
									cin.get();
								}
							}
						}
					}
					else if (result >= 11)
					{
						//find a component	
						if (wax < 4) //change depending on zone
						{
							wax++; //change depending on zone
							cout << "You found a wax!" << endl;
						}
						else
						{
							cout << "Sorry, your wax stores are full!" << endl;
						}
						cin.get();
					}
					else if (result >= 1)
					{
						//find a construct
						if (construct5Found == true)
						{
							cout << "You've already found the Scrying Lens... here are two wax instead!" << endl;
							if (wax == 4)
							{
								cout << "Your wax bags are full!" << endl;
								cin.get();
							}
							else if (wax == 3)
							{
								wax = 4;
								cout << "Your wax bags were filled to capacity! (4)" << endl;
								cin.get();
							}
							else wax += 2;
						}
						else
						{
							construct5Found = true;		//change depending on zone
							noConstructs = false;
							cout << "You found the Scrying Lens!" << endl;
							cin.get();
							cout << "Once you activate it, you may take up to 10 from any search result in the Glassrock Canyon or Root Strangled Marshes!" << endl;
							cin.get();
						}
					}
					else if (result == 0)
					{
						construct5Found = true;		//change depending on zone
						noConstructs = false;
						construct5Activated = true;
						godsHandCharge += 5;
						cout << "WOW! A perfect result!" << endl;
						cin.get();
						cout << "You found an activated Scrying Lens! You may take up to 10 from any search result in the Glassrock Canyon or Root Strangled Marshes!" << endl;
						cin.get();
						cout << "The God's Hand now also has 5 more charges in it!" << endl;
					}
					if (searchNumber == 6)
					{
						cout << "You have searched all 6 times. Would you like to cross off an additional day in order to find the construct (or a component)? (y/n)" << endl;
						cin >> searchFilled;
						if (searchFilled == 'y')
						{
							day++;

							//check for treasure2 for adding god's hand charge
							if (treasure2Found == true)
							{
								godsHandCharge++;
								cout << "Because of the Bracelet of Ios, one charge was added to the God's Hand Device for crossing a day off the time track." << endl;
								cin.get();
							}

							//check for treasure4 for adding hp
							if (treasure4Found)
							{
								hp++;
								cout << "Because of the Scale of the Infinity Wurm, you recovered one hp for crossing a day off the time track." << endl;
								cin.get();
							}

							if (construct5Found == true)
							{
								if (wax < 4)
								{
									cout << "You found a wax!" << endl;
									wax++;
								}
								else
								{
									cout << "Your wax stores are full!" << endl;
								}
							}
							else
							{
								construct5Found = true;
								noConstructs = false;
								cout << "You found the Scrying Lens!" << endl;
								cin.get();
								cout << "Once you activate it, you may take up to 10 from any search result in the Glassrock Canyon or Root Strangled Marshes!" << endl;
								cin.get();
							}

						}
					}
					else
					{
						cout << "Continue searching in this region? (y/n)" << endl;
						cin >> searchOrNot;
					}
				} while (searchOrNot == 'y' && searchNumber != 6);
				break;
			case 6:
				//something
				cout << "Entering the Fiery Maw..." << endl;
				cin.get();
				searchNumber = 0;

				//loop while they want to continue searching
				do
				{
					//update days depending on search #
					searchNumber++;
					cout << "Search " << searchNumber << endl;
					day = day + TheFieryMaw[searchNumber - 1];

					//check for treasure 2 adding a god's hand charge
					if (treasure2Found == true)
					{
						if (TheFieryMaw[searchNumber - 1] == 1)
						{
							godsHandCharge++;
							cout << "Because of the Bracelet of Ios, one charge was added to the God's Hand Device for crossing a day off the time track." << endl;
							cin.get();
						}
					}

					//check for treasure4 for adding hp
					if (treasure4Found)
					{
						if (HalebeardPeak[searchNumber - 1] == 1)
						{
							hp++;
							cout << "Because of the Scale of the Infinity Wurm, you recovered one hp for crossing a day off the time track." << endl;
							cin.get();
						}
					}

					//update event day
					if (TheFieryMaw[searchNumber - 1] == 1)
					{
						for (set<int>::iterator it = eventDays.begin(); it != eventDays.end(); it++)
						{
							if (*it == day)
							{
								rerollEventDays(activeMonstersEvent, fleetingVisionsEvent, goodFortuneEvent, foulWeatherEvent);
							}
						}
					}


					//take off an extra day for searching in this region
					if (foulWeatherEvent == zone)
					{
						day = day + TheFieryMaw[searchNumber - 1];
						//check for treasure 2 adding a god's hand charge
						if (treasure2Found == true)
						{
							if (TheFieryMaw[searchNumber - 1] == 1)
							{
								godsHandCharge++;
								cout << "Because of the Bracelet of Ios, one charge was added to the God's Hand Device for crossing a day off the time track." << endl;
								cin.get();
							}
						}

						//check for treasure4 for adding hp
						if (treasure4Found)
						{
							if (HalebeardPeak[searchNumber - 1] == 1)
							{
								hp++;
								cout << "Because of the Scale of the Infinity Wurm, you recovered one hp for crossing a day off the time track." << endl;
								cin.get();
							}
						}
					}

					result = diceRollSearch();

					//adjust results based on goodFortune
					if (goodFortuneEvent == zone)
					{
						cout << "Would you like to take up to 10 off of your search result? (y/n)" << endl;
						takeUpToTenOffSearchResult(result);
					}

					if (result >= 100 || result < 0)
					{
						char ignoreFight = 'n';
						if (treasure3Found)
						{
							cout << "The Shimmering Moonlace allows you to ignore any encounter. Would you like to ignore this encounter? (y/n)" << endl;
							cin >> ignoreFight;
						}
						if (tolower(ignoreFight) == 'n')
						{
							bool fightOver = false;
							int tempAttack, tempRange, encounterLevel;
							if (result >= 500 || result <= -401)
							{
								//level 5 encounter
								encounterLevel = 5;

							}
							else if (result >= 400 || result <= -301)
							{
								//level 4 encounter
								encounterLevel = 4;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 5;
								}
							}
							else if (result >= 300 || result <= -201)
							{
								//level 3 encounter
								encounterLevel = 3;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 5;
								}
							}
							else if (result >= 200 || result <= -101)
							{
								//level 2 encounter
								encounterLevel = 2;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 4;
								}
							}
							else
							{
								//level 1 encounter
								encounterLevel = 1;
								if (activeMonstersEvent == zone)
								{
									encounterLevel = 3;
								}
							}
							if (encounterLevel != 5)
							{
								Monster monster(zone, encounterLevel, treasure1Found, treasure6Found, construct4Activated);
								int tempAttack, tempRange;
								if (treasure1Found == true && monster.maxAttack != 1)
								{
									tempAttack = monster.maxAttack - 1;
								}
								else
								{
									tempAttack = monster.maxAttack;
								}
								if (treasure6Found)
								{
									tempRange = monster.hitRange - 1;
								}
								else
								{
									tempRange = monster.hitRange;
								}

								while (!fightOver)
								{
									die1 = rand() % 6 + 1;
									die2 = rand() % 6 + 1;
									cout << "Rolls: " << die1 << " " << die2 << endl;
									if (die1 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}
									}
									if (die2 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}
										else if (hp < 0)
										{
											gameOver = true;
											break;
										}
									}
									if (die1 >= tempRange || die2 >= tempRange)
									{
										fightOver = true;
										cout << "Congratulations! You've defeated " << monster.name << "! Press enter to see if it dropped something." << endl;
										cin.get();
										//drop something
										die1 = rand() % 6 + 1;
										cout << "Roll (You need a " << encounterLevel << " or lower to get a component): " << die1 << endl;
										if (die1 <= encounterLevel)
										{
											if (quartz < 4)
											{
												quartz++;
												cout << "You got a quartz!" << endl;
											}
											else
											{
												cout << "Sorry, your quartz stores are full!" << endl;
											}
											cin.get();
										}
										else
										{
											cout << "Sorry, not this time!" << endl;
											cin.get();
										}
									}
									cout << "Press ENTER to continue." << endl;
									cin.get();
								}
							}
							else
							{
								Monster monster(zone, encounterLevel, treasure1Found, treasure6Found, construct4Activated);

								if (treasure1Found == true && monster.maxAttack != 1)
								{
									tempAttack = monster.maxAttack - 1;
								}
								else
								{
									tempAttack = monster.maxAttack;
								}
								if (treasure6Found)
								{
									tempRange = monster.hitRange - 1;
								}
								else
								{
									tempRange = monster.hitRange;
								}

								while (!fightOver)
								{
									die1 = rand() % 6 + 1;
									die2 = rand() % 6 + 1;
									cout << "Rolls: " << die1 << " " << die2 << endl;
									if (die1 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}
									}
									if (die2 <= tempAttack)
									{
										hp--;
										cout << "Ouch! One damage taken. HP remaining: " << hp << endl;
										if (hp == 0)
										{
											checkIfDead(hp, construct3Activated, fightOver, day);
											break;
										}
										else if (hp < 0)
										{
											gameOver = true;
											break;
										}
									}
									if (die1 >= tempRange || die2 >= tempRange)
									{
										fightOver = true;
										cout << "Congratulations! You've defeated " << monster.name << "! Press enter to see if it dropped something." << endl;
										cin.get();
										//drop something
										die1 = rand() % 6 + 1;
										cout << "Roll (You need a " << encounterLevel << " or lower to get the Legendary Treasure): " << die1 << endl;
										if (die1 <= encounterLevel)
										{
											if (treasure4Found == false)
											{
												treasure4Found = true;
												cout << "The Volcano Spirit dropped the Molten Shard!" << endl;
												cin.get();
												cout << "The Molten Shard is a blade of magma that fell from the sky and pierced the heart of the world, and is now lodged deep in the Fiery Maw." << endl;
												cin.get();
												cout << "It adds 1 to the hit range of all monsters." << endl;
											}
											else
											{
												cout << "You've already obtained his treasure... here's a component instead!" << endl;
												if (lead < 4)
												{
													lead++;
												}
												else
												{
													cout << "Sorry, your lead stores are full!" << endl;
												}
												cin.get();
											}
										}
										else
										{
											cout << "Sorry, not this time!" << endl;
										}
									}
									cout << "Press ENTER to continue." << endl;
									cin.get();
								}
							}
						}
					}
					else if (result >= 11)
					{
						//find a component	
						if (lead < 4) //change depending on zone
						{
							lead++; //change depending on zone
							cout << "You found a lead!" << endl;
						}
						else
						{
							cout << "Sorry, your lead stores are full!" << endl;
						}
						cin.get();
					}
					else if (result >= 1)
					{
						//find a construct
						if (construct6Found == true)
						{
							cout << "You've already found the Crystal Battery... here are two lead instead!" << endl;
							if (lead == 4)
							{
								cout << "Your lead bags are full!" << endl;
								cin.get();
							}
							else if (lead == 3)
							{
								lead = 4;
								cout << "Your lead bags were filled to capacity! (4)" << endl;
								cin.get();
							}
							else lead += 2;
						}
						else
						{
							construct6Found = true;		//change depending on zone
							noConstructs = false;
							cout << "You found the Crystal Battery!" << endl;
							cin.get();
							cout << "Once you activate it, you may spend any three components from your stores to recharge one of your tool belt artifacts once per game!" << endl;
							cin.get();
						}
					}
					else if (result == 0)
					{
						construct6Found = true;		//change depending on zone
						noConstructs = false;
						construct6Activated = true;
						godsHandCharge += 5;
						cout << "WOW! A perfect result!" << endl;
						cin.get();
						cout << "You found an activated Crystal Battery! You may spend any three components from your stores to recharge one of your tool belt artifacts once per game!" << endl;
						cin.get();
						cout << "The God's Hand now also has 5 more charges in it!" << endl;
					}
					if (searchNumber == 6)
					{
						cout << "You have searched all 6 times. Would you like to cross off an additional day in order to find the construct (or a component)? (y/n)" << endl;
						cin >> searchFilled;
						if (searchFilled == 'y')
						{
							day++;

							//check for treasure 2 adding a god's hand charge
							if (treasure2Found)
							{
								godsHandCharge++;
								cout << "Because of the Bracelet of Ios, one charge was added to the God's Hand Device for crossing a day off the time track." << endl;
								cin.get();
							}

							//check for treasure4 for adding hp
							if (treasure4Found)
							{
								hp++;
								cout << "Because of the Scale of the Infinity Wurm, you recovered one hp for crossing a day off the time track." << endl;
								cin.get();
							}

							if (construct6Found == true)
							{
								if (lead < 4)
								{
									cout << "You found a lead!" << endl;
									lead++;
								}
								else
								{
									cout << "Your lead stores are full!" << endl;
								}
							}
							else
							{
								construct6Found = true;
								noConstructs = false;
								cout << "You found the Crystal Battery!" << endl;
								cin.get();
								cout << "Once you activate it, you may spend any three components from your stores to recharge one of your tool belt artifacts once per game!" << endl;
								cin.get();
							}

						}
					}
					else
					{
						cout << "Continue searching in this region? (y/n)" << endl;
						cin >> searchOrNot;
					}
				} while (searchOrNot == 'y' && searchNumber != 6);
				break;

			}
		}
		else if (tolower(action) == 'w')	//workshop
		{
			char workshopAction;
			cout << "Welcome to the Workshop!" << endl;
			cin.get();
			cout << "Your current Constructs: " << endl;
			if (!construct1Found && !construct2Found && !construct3Found &&
				!construct4Found && !construct5Found && !construct6Found)
			{
				cout << "None" << endl;
			}
			else
			{
				if (construct1Found)
				{
					cout << "Seal of Balance - ";
					if (!construct1Activated)
					{
						cout << "Not";
					}
					cout << " Activated" << endl;
				}
				if (construct2Found)
				{
					cout << "Hermetic Mirror - ";
					if (!construct2Activated)
					{
						cout << "Not";
					}
					cout << " Activated" << endl;
				}
				if (construct3Found)
				{
					cout << "Void Gate - ";
					if (!construct3Activated)
					{
						cout << "Not";
					}
					cout << " Activated" << endl;
				}
				if (construct4Found)
				{
					cout << "Golden Chassis - ";
					if (!construct4Activated)
					{
						cout << "Not";
					}
					cout << " Activated" << endl;
				}
				if (construct5Found)
				{
					cout << "Scrying Lens - ";
					if (!construct5Activated)
					{
						cout << "Not";
					}
					cout << " Activated" << endl;
				}
				if (construct6Found)
				{
					cout << "Crystal Battery - ";
					if (!construct6Activated)
					{
						cout << "Not";
					}
					cout << " Activated" << endl;
				}
			}
			cin.get();
			cout << "Your current Connections: " << endl;
			if (!connection12 && !connection14 && !connection15 && !connection23 && !connection43 && !connection46)
			{
				cout << "None" << endl;
			}
			else
			{
				if (connection12) {
					cout << "Seal of Balance to Hermetic Mirror" << endl;
				}
				if (connection14) {
					cout << "Seal of Balance to Golden Chassis" << endl;
				}
				if (connection15) {
					cout << "Seal of Balance to Scrying Lens" << endl;
				}
				if (connection23) {
					cout << "Hermetic Mirror to Void Gate" << endl;
				}
				if (connection43) {
					cout << "Golden Chassis to Void Gate" << endl;
				}
				if (connection46) {
					cout << "Golden Chassis to Crystal Battery" << endl;
				}
			}
			cin.get();

			cout << "What would you like to do? A to Activate a Construct, C to Connect two constructs, or B to go Back" << endl;
			cin >> workshopAction;
			while (tolower(workshopAction) != 'a' && tolower(workshopAction) != 'c' && tolower(workshopAction) != 'b')
			{
				cout << "Invalid action. A to Activate a Construct, C to Connect two constructs, or B to go Back" << endl;
				cin >> workshopAction;
			}
			if (tolower(workshopAction) == 'c' && noConnections)
			{
				cout << "No connections are available." << endl;
				cin.get();
			}
			else if (tolower(workshopAction) == 'a' && noConstructs)
			{
				cout << "You don't have any constructs to activate!" << endl;
				cin.get();
			}
			else if (tolower(workshopAction) == 'a') //activate
			{
				int constructToActivate;
				int result;
				cout << "Your current Constructs: " << endl;
				if (construct1Found)
				{
					cout << "Seal of Balance - ";
					if (!construct1Activated)
					{
						cout << "Not";
					}
					cout << " Activated" << endl;
				}
				if (construct2Found)
				{
					cout << "Hermetic Mirror - ";
					if (!construct2Activated)
					{
						cout << "Not";
					}
					cout << " Activated" << endl;
				}
				if (construct3Found)
				{
					cout << "Void Gate - ";
					if (!construct3Activated)
					{
						cout << "Not";
					}
					cout << " Activated" << endl;
				}
				if (construct4Found)
				{
					cout << "Golden Chassis - ";
					if (!construct4Activated)
					{
						cout << "Not";
					}
					cout << " Activated" << endl;
				}
				if (construct5Found)
				{
					cout << "Scrying Lens - ";
					if (!construct5Activated)
					{
						cout << "Not";
					}
					cout << " Activated" << endl;
				}
				if (construct6Found)
				{
					cout << "Crystal Battery - ";
					if (!construct6Activated)
					{
						cout << "Not";
					}
					cout << " Activated" << endl;
				}
				cout << "Which would you like to activate?" << endl;
				cin >> constructToActivate;
				while (constructToActivate > 6 || constructToActivate < 1 ||
					(constructToActivate == 1 && (!construct1Found || construct1Activated)) ||
					(constructToActivate == 2 && (!construct2Found || construct2Activated)) ||
					(constructToActivate == 3 && (!construct3Found || construct3Activated)) ||
					(constructToActivate == 4 && (!construct4Found || construct4Activated)) ||
					(constructToActivate == 5 && (!construct5Found || construct5Activated)) ||
					(constructToActivate == 6 && (!construct6Found || construct6Activated)))
				{
					cout << "Invalid option. Pick a construct between 1 and 6 that you have already found." << endl;
					cin >> constructToActivate;
				}

				result = diceRollActivate(hp);
				cout << "Result: " << result << "of the four points needed to activate the construct." << endl;
				if (result >= 4)
				{
					godsHandCharge += (result - 4);
					cout << "Any extra points are transfered to the God's Hand device." << endl << "Construct Activated. " << (result - 4) << " God's Hand charges added." << endl;
					cin.get();
				}
				else
				{
					cout << "You did not activate the construct on your first attempt. You spend one more day continuing trying to activate it." << endl;
					day++;
					cin.get();
					result += diceRollActivate(hp);

					cout << "Result: " << result << "of the four points needed to activate the construct. Any extra points are transfered to the God's Hand device." << endl;
					if (result >= 4)
					{
						godsHandCharge += (result - 4);
						cout << "Construct Activated. " << (result - 4) << " God's Hand charges added." << endl;
						cin.get();
					}
					else
					{
						cout << "You did not activate the construct on your second attempt. You spend one more day to automatically activate it." << endl;
						day++;
					}
				}
				if (constructToActivate == 1)
					construct1Activated = true;
				if (constructToActivate == 2)
					construct2Activated = true;
				if (constructToActivate == 3)
					construct3Activated = true;
				if (constructToActivate == 4)
					construct4Activated = true;
				if (constructToActivate == 5)
					construct5Activated = true;
				if (constructToActivate == 6)
					construct6Activated = true;
			}
			else if (tolower(workshopAction) == 'c')
			{

			}
			else if (tolower(workshopAction) == 'b')
			{
				break;
			}


		}
		else if (tolower(action) == 'r') //rest
		{
			if (hp < 6)
			{
				day++;
				hp++;

				//check for treasure 2 adding a god's hand charge
				if (treasure2Found == true)
				{
					godsHandCharge++;
					cout << "Because of the Bracelet of Ios, one charge was added to the God's Hand Device for crossing a day off the time track." << endl;
					cin.get();
				}

				//update event days
				for (set<int>::iterator it = eventDays.begin(); it != eventDays.end(); it++)
				{
					if (*it == day)
					{
						rerollEventDays(activeMonstersEvent, fleetingVisionsEvent, goodFortuneEvent, foulWeatherEvent);
					}
				}
			}
			else
			{
				cout << "Resting has no effect at full hp (6)!" << endl;
			}


		}
		else if (tolower(action) == 'g') //god's hand
		{
			char godsHandOrNot;
			if (godsHandCharge < 3)
			{
				cout << "Not enough charges in the God's Hand Device to push Doomsday back a day. Current charges: " << godsHandCharge << ". Needed: 3" << endl;
			}
			else
			{
				cout << "Charges of God's Hand Device: " << godsHandCharge << ". Spend 3 to push Doomsday back a day? (y/n)" << endl;
				cin >> godsHandOrNot;
				if (tolower(godsHandOrNot) == 'y')
				{
					godsHandCharge -= 3;
					Doomsday++;
					cout << "God's Hand Charge Remaining: " << godsHandCharge << ". Doomsday now in " << Doomsday << " days." << endl;
					cin.get();
				}
			}
		}

		cout << endl << endl << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl;



	}
	while (!gameOver);
}

int diceRollSearch()
{
	int die1, die2; //two dice as we roll
	int one, two, three, four, five, six; //each space in a search box (1 2 3 // 4 5 6)

	bool oneTaken, twoTaken, threeTaken, fourTaken, fiveTaken, sixTaken; //are the search boxes filled?


	int dieRolled; //number of times the two dice were rolled

	int diePlace; //where user wants the number to go (1-6)
	int result; //difference between 2 3-digit numbers


	one = 0, two = 0, three = 0, four = 0, five = 0, six = 0;
	oneTaken = false, twoTaken = false, threeTaken = false, fourTaken = false, fiveTaken = false, sixTaken = false;
	dieRolled = 0;
	while (dieRolled < 3)
	{
		die1 = rand() % 6 + 1;
		die2 = rand() % 6 + 1;
		dieRolled++;
		cout << "Dice roll " << dieRolled << ": " << die1 << " " << die2 << endl;

		//placing die 1
		cout << "Where would you like the " << die1 << " placed?" << endl;
		cout << one << " " << two << " " << three << endl << four << " " << five << " " << six << endl << "(1 2 3)" << endl << "(4 5 6)" << endl;

		//make sure they don't put number in a box that's already full
		do
		{
			cin >> diePlace;
			if (diePlace > 6 || diePlace < 1 ||
				(diePlace == 1 && oneTaken == true) || (diePlace == 2 && twoTaken == true) ||
				(diePlace == 3 && threeTaken == true) || (diePlace == 4 && fourTaken == true) ||
				(diePlace == 5 && fiveTaken == true) || (diePlace == 6 && sixTaken == true))
			{
				cout << "Not a valid box number. Input an unfilled box number from 1-6." << endl;
			}
		} while (diePlace > 6 || diePlace < 1 ||
			(diePlace == 1 && oneTaken == true) || (diePlace == 2 && twoTaken == true) ||
			(diePlace == 3 && threeTaken == true) || (diePlace == 4 && fourTaken == true) ||
			(diePlace == 5 && fiveTaken == true) || (diePlace == 6 && sixTaken == true));


		//fill that box
		switch (diePlace)
		{
		case 1:
			one = die1;
			oneTaken = true;
			break;
		case 2:
			two = die1;
			twoTaken = true;
			break;
		case 3:
			three = die1;
			threeTaken = true;
			break;
		case 4:
			four = die1;
			fourTaken = true;
			break;
		case 5:
			five = die1;
			fiveTaken = true;
			break;
		case 6:
			six = die1;
			sixTaken = true;
			break;
		}

		//placing die 2
		cout << "Where would you like the " << die2 << " placed?" << endl;
		cout << one << " " << two << " " << three << endl << four << " " << five << " " << six << endl << "(1 2 3)" << endl << "(4 5 6)" << endl;

		//make sure they don't put number in a box that's already full
		do
		{
			cin >> diePlace;
			if (diePlace > 6 || diePlace < 1 ||
				(diePlace == 1 && oneTaken == true) || (diePlace == 2 && twoTaken == true) ||
				(diePlace == 3 && threeTaken == true) || (diePlace == 4 && fourTaken == true) ||
				(diePlace == 5 && fiveTaken == true) || (diePlace == 6 && sixTaken == true))
			{
				cout << "Not a valid box number. Input an unfilled box number from 1-6." << endl;
			}
		} while (diePlace > 6 || diePlace < 1 ||
			(diePlace == 1 && oneTaken == true) || (diePlace == 2 && twoTaken == true) ||
			(diePlace == 3 && threeTaken == true) || (diePlace == 4 && fourTaken == true) ||
			(diePlace == 5 && fiveTaken == true) || (diePlace == 6 && sixTaken == true));

		//fill that box
		switch (diePlace)
		{
		case 1:
			one = die2;
			oneTaken = true;
			break;
		case 2:
			two = die2;
			twoTaken = true;
			break;
		case 3:
			three = die2;
			threeTaken = true;
			break;
		case 4:
			four = die2;
			fourTaken = true;
			break;
		case 5:
			five = die2;
			fiveTaken = true;
			break;
		case 6:
			six = die2;
			sixTaken = true;
			break;
		}
	}

	//calculate result
	result = (one * 100 + two * 10 + three) - (four * 100 + five * 10 + six);
	cout << "Result: " << result << endl;

	return result;
}

int diceRollActivate(int &health)
{
	int die1, die2; //two dice as we roll
	int one, two, three, four, five, six, seven, eight; //each space in a search box (1 2 3 4// 5 6 7 8)

	bool oneTaken, twoTaken, threeTaken, fourTaken, fiveTaken, sixTaken, sevenTaken, eightTaken; //are the search boxes filled?



	int diePlace; //where user wants the number to go (1-8)
	int result;
	int result1 = 0; //SUM between 4 1-digit numbers
	int result2 = 0;
	int result3 = 0;
	int result4 = 0;

	one = 0, two = 0, three = 0, four = 0, five = 0, six = 0, seven = 0, eight = 0;
	oneTaken = false, twoTaken = false, threeTaken = false, fourTaken = false, fiveTaken = false, sixTaken = false, sevenTaken = false, eightTaken = false;

	while (!oneTaken || !twoTaken || !threeTaken || !fourTaken || !fiveTaken || !sixTaken || !sevenTaken || !eightTaken)
	{
		die1 = rand() % 6 + 1;
		die2 = rand() % 6 + 1;

		cout << "Dice roll: " << die1 << " " << die2 << endl;

		//placing die 1
		cout << "Where would you like the " << die1 << " placed?" << endl;
		cout << one << " " << two << " " << three << " " << four << endl << five << " " << six << " " << seven << " " << eight << endl << "(1 2 3 4)" << endl << "(5 6 7 8)" << endl;

		//make sure they don't put number in a box that's already full
		do
		{
			cin >> diePlace;
			if (diePlace > 8 || diePlace < 1 ||
				(diePlace == 1 && oneTaken) || (diePlace == 2 && twoTaken) ||
				(diePlace == 3 && threeTaken) || (diePlace == 4 && fourTaken) ||
				(diePlace == 5 && fiveTaken) || (diePlace == 6 && sixTaken) ||
				(diePlace == 7 && sevenTaken) || (diePlace == 8 && eightTaken))
			{
				cout << "Not a valid box number. Input an unfilled box number from 1-8." << endl;
			}
		} while (diePlace > 8 || diePlace < 1 ||
			(diePlace == 1 && oneTaken) || (diePlace == 2 && twoTaken) ||
			(diePlace == 3 && threeTaken) || (diePlace == 4 && fourTaken) ||
			(diePlace == 5 && fiveTaken) || (diePlace == 6 && sixTaken) ||
			(diePlace == 7 && sevenTaken) || (diePlace == 8 && eightTaken));


		//fill that box
		switch (diePlace)
		{
		case 1:
			one = die1;
			oneTaken = true;
			if (fiveTaken)
			{
				if (one == five)
				{
					one = 0; five = 0;
					oneTaken = false; fiveTaken = false;
				}
				if (one - five == 5)
					result1 = 2;
				else if (one - five == 4)
					result1 = 1;
				else if (one - five < 0)
				{
					result1 = 0;
					health--;
					cout << "Ouch! A negative result created a lock and zapped you for 1 hp." << endl;
				}
				else
				{
					result1 = 0;
				}
			}
			break;
		case 2:
			two = die1;
			twoTaken = true;
			if (sixTaken)
			{
				if (two == six)
				{
					two = 0; six = 0;
					twoTaken = false; sixTaken = false;
				}
				if (two - six == 5)
					result2 = 2;
				else if (two - six == 4)
					result2 = 1;
				else if (two - six < 0)
				{
					result2 = 0;
					health--;
					cout << "Ouch! A negative result created a lock and zapped you for 1 hp." << endl;
				}
				else
				{
					result2 = 0;
				}
			}
			break;
		case 3:
			three = die1;
			threeTaken = true;
			if (sevenTaken)
			{
				if (three == seven)
				{
					three = 0; seven = 0;
					threeTaken = false; sevenTaken = false;
				}
				if (three - seven == 5)
					result3 = 2;
				else if (three - seven == 4)
					result3 = 1;
				else if (three - seven < 0)
				{
					result3 = 0;
					health--;
					cout << "Ouch! A Negative result created a lock and zapped you for 1 hp." << endl;
				}
				else
				{
					result3 = 0;
				}
			}
			break;
		case 4:
			four = die1;
			fourTaken = true;
			if (eightTaken)
			{
				if (four == eight)
				{
					four = 0; eight = 0;
					fourTaken = false; eightTaken = false;
				}
				if (four - eight == 5)
					result4 = 2;
				else if (four - eight == 4)
					result4 = 1;
				else if (four - eight < 0)
				{
					result4 = 0;
					health--;
					cout << "Ouch! A Negative result created a lock and zapped you for 1 hp." << endl;
				}
				else
				{
					result4 = 0;
				}
			}
			break;
		case 5:
			five = die1;
			fiveTaken = true;
			if (oneTaken)
			{
				if (one == five)
				{
					one = 0; five = 0;
					oneTaken = false; fiveTaken = false;
				}
				if (one - five == 5)
					result1 = 2;
				else if (one - five == 4)
					result1 = 1;
				else if (one - five < 0)
				{
					result1 = 0;
					health--;
					cout << "Ouch! A negative result created a lock and zapped you for 1 hp." << endl;
				}
				else
				{
					result1 = 0;
				}
			}
			break;
		case 6:
			six = die1;
			sixTaken = true;
			if (twoTaken)
			{
				if (two == six)
				{
					two = 0; six = 0;
					twoTaken = false; sixTaken = false;
				}
				if (two - six == 5)
					result2 = 2;
				else if (two - six == 4)
					result2 = 1;
				else if (two - six < 0)
				{
					result2 = 0;
					health--;
					cout << "Ouch! A negative result created a lock and zapped you for 1 hp." << endl;
				}
				else
				{
					result2 = 0;
				}
			}
			break;
		case 7:
			seven = die1;
			sevenTaken = true;
			if (threeTaken)
			{
				if (three == seven)
				{
					three = 0; seven = 0;
					threeTaken = false; sevenTaken = false;
				}
				if (three - seven == 5)
					result3 = 2;
				else if (three - seven == 4)
					result3 = 1;
				else if (three - seven < 0)
				{
					result3 = 0;
					health--;
					cout << "Ouch! A Negative result created a lock and zapped you for 1 hp." << endl;
				}
				else
				{
					result3 = 0;
				}
			}
			break;
		case 8:
			eight = die1;
			eightTaken = true;
			if (fourTaken)
			{
				if (four == eight)
				{
					four = 0; eight = 0;
					fourTaken = false; eightTaken = false;
				}
				if (four - eight == 5)
					result4 = 2;
				else if (four - eight == 4)
					result4 = 1;
				else if (four - eight < 0)
				{
					result4 = 0;
					health--;
					cout << "Ouch! A Negative result created a lock and zapped you for 1 hp." << endl;
				}
				else
				{
					result4 = 0;
				}
			}
			break;
		}

		//placing die 2
		cout << "Where would you like the " << die2 << " placed?" << endl;
		cout << one << " " << two << " " << three << " " << four << endl << five << " " << six << " " << seven << " " << eight << endl << "(1 2 3 4)" << endl << "(5 6 7 8)" << endl;

		//make sure they don't put number in a box that's already full
		do
		{
			cin >> diePlace;
			if (diePlace > 8 || diePlace < 1 ||
				(diePlace == 1 && oneTaken) || (diePlace == 2 && twoTaken) ||
				(diePlace == 3 && threeTaken) || (diePlace == 4 && fourTaken) ||
				(diePlace == 5 && fiveTaken) || (diePlace == 6 && sixTaken) ||
				(diePlace == 7 && sevenTaken) || (diePlace == 8 && eightTaken))
			{
				cout << "Not a valid box number. Input an unfilled box number from 1-8." << endl;
			}
		} while (diePlace > 8 || diePlace < 1 ||
			(diePlace == 1 && oneTaken) || (diePlace == 2 && twoTaken) ||
			(diePlace == 3 && threeTaken) || (diePlace == 4 && fourTaken) ||
			(diePlace == 5 && fiveTaken) || (diePlace == 6 && sixTaken) ||
			(diePlace == 7 && sevenTaken) || (diePlace == 8 && eightTaken));

		//fill that box
		switch (diePlace)
		{
		case 1:
			one = die2;
			oneTaken = true;
			if (fiveTaken)
			{
				if (one == five)
				{
					one = 0; five = 0;
					oneTaken = false; fiveTaken = false;
				}
				if (one - five == 5)
					result1 = 2;
				else if (one - five == 4)
					result1 = 1;
				else if (one - five < 0)
				{
					result1 = 0;
					health--;
					cout << "Ouch! A negative result created a lock and zapped you for 1 hp." << endl;
				}
				else
				{
					result1 = 0;
				}
			}
			break;
		case 2:
			two = die2;
			twoTaken = true;
			if (sixTaken)
			{
				if (two == six)
				{
					two = 0; six = 0;
					twoTaken = false; sixTaken = false;
				}
				if (two - six == 5)
					result2 = 2;
				else if (two - six == 4)
					result2 = 1;
				else if (two - six < 0)
				{
					result2 = 0;
					health--;
					cout << "Ouch! A negative result created a lock and zapped you for 1 hp." << endl;
				}
				else
				{
					result2 = 0;
				}
			}
			break;
		case 3:
			three = die2;
			threeTaken = true;
			if (sevenTaken)
			{
				if (three == seven)
				{
					three = 0; seven = 0;
					threeTaken = false; sevenTaken = false;
				}
				if (three - seven == 5)
					result3 = 2;
				else if (three - seven == 4)
					result3 = 1;
				else if (three - seven < 0)
				{
					result3 = 0;
					health--;
					cout << "Ouch! A Negative result created a lock and zapped you for 1 hp." << endl;
				}
				else
				{
					result3 = 0;
				}
			}
			break;
		case 4:
			four = die2;
			fourTaken = true;
			if (eightTaken)
			{
				if (four == eight)
				{
					four = 0; eight = 0;
					fourTaken = false; eightTaken = false;
				}
				if (four - eight == 5)
					result4 = 2;
				else if (four - eight == 4)
					result4 = 1;
				else if (four - eight < 0)
				{
					result4 = 0;
					health--;
					cout << "Ouch! A Negative result created a lock and zapped you for 1 hp." << endl;
				}
				else
				{
					result4 = 0;
				}
			}
			break;
		case 5:
			five = die2;
			fiveTaken = true;
			if (oneTaken)
			{
				if (one == five)
				{
					one = 0; five = 0;
					oneTaken = false; fiveTaken = false;
				}
				if (one - five == 5)
					result1 = 2;
				else if (one - five == 4)
					result1 = 1;
				else if (one - five < 0)
				{
					result1 = 0;
					health--;
					cout << "Ouch! A negative result created a lock and zapped you for 1 hp." << endl;
				}
				else
				{
					result1 = 0;
				}
			}
			break;
		case 6:
			six = die2;
			sixTaken = true;
			if (twoTaken)
			{
				if (two == six)
				{
					two = 0; six = 0;
					twoTaken = false; sixTaken = false;
				}
				if (two - six == 5)
					result2 = 2;
				else if (two - six == 4)
					result2 = 1;
				else if (two - six < 0)
				{
					result2 = 0;
					health--;
					cout << "Ouch! A negative result created a lock and zapped you for 1 hp." << endl;
				}
				else
				{
					result2 = 0;
				}
			}
			break;
		case 7:
			seven = die2;
			sevenTaken = true;
			if (threeTaken)
			{
				if (three == seven)
				{
					three = 0; seven = 0;
					threeTaken = false; sevenTaken = false;
				}
				if (three - seven == 5)
					result3 = 2;
				else if (three - seven == 4)
					result3 = 1;
				else if (three - seven < 0)
				{
					result3 = 0;
					health--;
					cout << "Ouch! A Negative result created a lock and zapped you for 1 hp." << endl;
				}
				else
				{
					result3 = 0;
				}
			}
			break;
		case 8:
			eight = die2;
			eightTaken = true;
			if (fourTaken)
			{
				if (four == eight)
				{
					four = 0; eight = 0;
					fourTaken = false; eightTaken = false;
				}
				if (four - eight == 5)
					result4 = 2;
				else if (four - eight == 4)
					result4 = 1;
				else if (four - eight < 0)
				{
					result4 = 0;
					health--;
					cout << "Ouch! A Negative result created a lock and zapped you for 1 hp." << endl;
				}
				else
				{
					result4 = 0;
				}
			}
			break;
		}
	}

	//calculate result
	result = result1 + result2 + result3 + result4;
	

	return result;
}

void takeUpToTenOffSearchResult(int &result)
{
	char searchResultRemoved;
	int searchResultAmount;
	cin >> searchResultRemoved;
	if (searchResultRemoved == 'y')
	{
		cout << "How much?" << endl;
		do
		{
			cin >> searchResultAmount;
			if (searchResultAmount > 10 || searchResultAmount < 0)
			{
				cout << "Please enter an amount between 0 and 10." << endl;
			}
		} while (searchResultAmount > 10 || searchResultAmount < 0);
		result -= searchResultAmount;
		cout << "Result: " << result << endl;
	}
}

void rerollEventDays(int &activeMonsters, int &fleetingVisions, int &goodFortune, int &foulWeather)
{
	activeMonsters = rand() % 6 + 1;
	fleetingVisions = rand() % 6 + 1;
	goodFortune = rand() % 6 + 1;
	foulWeather = rand() % 6 + 1;
	cout << "Today was an eventful day! There are now four different events going on in a few of the different locations!" << endl;
	cin.get();
	cout << "The Active Monsters Event increases the level of all encounters in the region by two (to a max of 5). That is now in zone " << activeMonsters << endl;
	cin.get();
	cout << "The Fleeting Visions Event lets you begin with one free energy point in the activation bar when activating the region's construct. That is now in zone " << fleetingVisions << endl;
	cin.get();
	cout << "The Good Fortune Event allows you to subtract up to 10 from your search results in the region. That is now in zone " << goodFortune << endl;
	cin.get();
	cout << "The Foul Weather Event causes you to lose an extra day in the day tracker for a search in the region. That is now in zone " << foulWeather << endl;
	cin.get();
}

void checkIfDead(int &hp, bool constructActivated, bool &fightOver, int &day)
{

	cout << "You've been knocked unconcious!" << endl;
	cin.get();
	cout << "You spend ";
	if (constructActivated == true)
	{
		cout << "four";
	}
	else
	{
		cout << "six";
	}
	cout << " days recovering, but have regained all of your HP." << endl;
	cin.get();
	fightOver = true;
	if (constructActivated == true)
	{
		day += 4;
	}
	else
	{
		day += 6;
	}
	hp = 6;
}

