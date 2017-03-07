#include "Monster.h"


Monster::Monster(int zone, int encounterLevel, bool treasure1Found, bool treasure6Found, bool construct4Activated)
{
	switch(zone)
	{
		case 1:
			if (encounterLevel == 1)
			{
				name = "an Ice Bear";
				maxAttack = 1;
				hitRange = 5;
				isSpirit = false;
			}
			else if (encounterLevel == 2)
			{
				name = "Roving Bandits";
				maxAttack = 1;
				hitRange = 6;
				isSpirit = false;
			}
			else if (encounterLevel == 3)
			{
				name = "Blood Wolves";
				maxAttack = 2;
				hitRange = 6;
				isSpirit = false;
			}
			else if (encounterLevel == 4)
			{
				name = "a Horse Eater Hawk";
				maxAttack = 3;
				hitRange = 6;
				isSpirit = false;
			}
			else if (encounterLevel == 5)
			{
				name = "the Giant of the Peaks";
				maxAttack = 4;
				hitRange = 6;
				isSpirit = false;
				cout << "You have encountered " << name << "! It is a legendary creature. Defeat it and it could drop a legendary item!" << endl;
				cin.get();
				cout << "You take damage on a die roll of " << maxAttack << " or less, and kill the monster on a roll of " << hitRange << " (or, if possible, greater). (Press ENTER to continue)" << endl;
				cin.get();
			}
			
			break;
		case 2:
			if (encounterLevel == 1)
			{
				name = "a Rogue Thief";
				maxAttack = 2;
				hitRange = 5;
				isSpirit = false;
			}
			else if (encounterLevel == 2)
			{
				name = "a Blanket of Crows";
				maxAttack = 1;
				hitRange = 6;
				isSpirit = false;
			}
			else if (encounterLevel == 3)
			{
				name = "a Hornback Bison";
				maxAttack = 1;
				hitRange = 6;
				isSpirit = false;
			}
			else if (encounterLevel == 4)
			{
				name = "a Grassyback Troll";
				maxAttack = 3;
				hitRange = 5;
				isSpirit = false;
			}
			else if (encounterLevel == 5)
			{
				name = "the Thunder King";
				maxAttack = 4;
				hitRange = 6;
				isSpirit = true;
				cout << "You have encountered " << name << "! It is a legendary creature. Defeat it and it could drop a legendary item!" << endl;
				cin.get();
				cout << "You take damage on a die roll of " << maxAttack << " or less, and kill the monster on a roll of " << hitRange << " (or, if possible, greater). (Press ENTER to continue)" << endl;
				cin.get();
			}
			
			break;
		case 3:
			if (encounterLevel == 1)
			{
				name = "a Gemscale Boa";
				maxAttack = 1;
				hitRange = 5;
				isSpirit = false;
			}
			else if (encounterLevel == 2)
			{
				name = "an Ancient Alligator";
				maxAttack = 2;
				hitRange = 6;
				isSpirit = false;
			}
			else if (encounterLevel == 3)
			{
				name = "a Land Shark";
				maxAttack = 2;
				hitRange = 6;
				isSpirit = false;
			}
			else if (encounterLevel == 4)
			{
				name = "an Abyssal Leech";
				maxAttack = 3;
				hitRange = 6;
				isSpirit = false;
			}
			else if (encounterLevel == 5)
			{
				name = "the Dweller in the Tides";
				maxAttack = 4;
				hitRange = 6;
				isSpirit = false;
				cout << "You have encountered " << name << "! It is a legendary creature. Defeat it and it could drop a legendary item!" << endl;
				cin.get();
				cout << "You take damage on a die roll of " << maxAttack << " or less, and kill the monster on a roll of " << hitRange << " (or, if possible, greater). (Press ENTER to continue)" << endl;
				cin.get();
			}
			
			break;
		case 4:
			if (encounterLevel == 1)
			{
				name = "a Fiesty Gremlin";
				maxAttack = 1;
				hitRange = 5;
				isSpirit = false;
			}
			else if (encounterLevel == 2)
			{
				name = "a Glasswing Drake";
				maxAttack = 1;
				hitRange = 6;
				isSpirit = false;
			}
			else if (encounterLevel == 3)
			{
				name = "Reaching Claws";
				maxAttack = 2;
				hitRange = 6;
				isSpirit = true;
			}
			else if (encounterLevel == 4)
			{
				name = "a Terrible Wurm";
				maxAttack = 3;
				hitRange = 6;
				isSpirit = false;
			}
			else if (encounterLevel == 5)
			{
				name = "the Leviathan Serpent";
				maxAttack = 4;
				hitRange = 6;
				isSpirit = false;
				cout << "You have encountered " << name << "! It is a legendary creature. Defeat it and it could drop a legendary item!" << endl;
				cin.get();
				cout << "You take damage on a die roll of " << maxAttack << " or less, and kill the monster on a roll of " << hitRange << " (or, if possible, greater). (Press ENTER to continue)" << endl;
				cin.get();
			}
			
			break;
		case 5:
			if (encounterLevel == 1)
			{
				name = "Grave Robbers";
				maxAttack = 1;
				hitRange = 5;
				isSpirit = false;
			}
			else if (encounterLevel == 2)
			{
				name = "Ghost Lights";
				maxAttack = 1;
				hitRange = 6;
				isSpirit = true;
			}
			else if (encounterLevel == 3)
			{
				name = "a Vengeful Shade";
				maxAttack = 2;
				hitRange = 6;
				isSpirit = true;
			}
			else if (encounterLevel == 4)
			{
				name = "a Nightmare Crab";
				maxAttack = 3;
				hitRange = 6;
				isSpirit = true;
			}
			else if (encounterLevel == 5)
			{
				name = "the Unnamed";
				maxAttack = 4;
				hitRange = 6;
				isSpirit = true;
				cout << "You have encountered " << name << "! It is a legendary creature. Defeat it and it could drop a legendary item!" << endl;
				cin.get();
				cout << "You take damage on a die roll of " << maxAttack << " or less, and kill the monster on a roll of " << hitRange << " (or, if possible, greater). (Press ENTER to continue)" << endl;
				cin.get();
			}
			
			break;
		case 6:
			if (encounterLevel == 1)
			{
				name = "a Minor Imp";
				maxAttack = 1;
				hitRange = 5;
				isSpirit = false;
			}
			else if (encounterLevel == 2)
			{
				name = "a Renegade Warlock";
				maxAttack = 2;
				hitRange = 5;
				isSpirit = false;
			}
			else if (encounterLevel == 3)
			{
				name = "a Giant Flame Lizard";
				maxAttack = 3;
				hitRange = 5;
				isSpirit = false;
			}
			else if (encounterLevel == 4)
			{
				name = "a Spark Elemental";
				maxAttack = 3;
				hitRange = 6;
				isSpirit = true;
			}
			else if (encounterLevel == 5)
			{
				name = "the Volcano Spirit";
				maxAttack = 4;
				hitRange = 6;
				isSpirit = true;
				cout << "You have encountered " << name << "! It is a legendary creature. Defeat it and it could drop a legendary item!" << endl;
				cin.get();
				cout << "You take damage on a die roll of " << maxAttack << " or less, and kill the monster on a roll of " << hitRange << " (or, if possible, greater). (Press ENTER to continue)" << endl;
				cin.get();
			}
			
			break;
	}

	if (encounterLevel < 5)
	{
		cout << "You have encountered " << name << "!" << endl;
		cin.get();
		cout << "You take damage on a die roll of ";
		if (treasure1Found && maxAttack != 1)
		{
			cout << maxAttack - 1;
		}
		else
		{
			cout << maxAttack;
		}
		cout << " (or, if possible, less), and kill the monster on a roll of ";
		if (treasure6Found)
		{
			cout << hitRange - 1;
		}
		else
		{
			cout << hitRange;
		}
		cout << " (or, if possible, greater). (Press ENTER to continue)" << endl;
		cin.get();
	}
	if (isSpirit && construct4Activated)
	{
		cout << name << " is also a spirit, so your activated Golden Chassis lowers the attack and hit range of the spirit by 1." << endl;
		maxAttack--;
		hitRange--;
	}
		
}

Monster::~Monster()
{
}
