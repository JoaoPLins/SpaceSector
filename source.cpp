#include <iostream>;
#include <string>;
#include <vector>;
#include <utility>;
#include <random>;
#include <thread>;
#include <cmath>;
#include <atomic>;
#include <chrono>;

using namespace std;

atomic<bool> statusofgame;
atomic<int> speed = 1;
int ticknumber = 0;
int minutetick = 0;
int hourtick = 0;

class MapTile
{
public:
	MapTile(int Placex, int Placey)
	{
		SetxandY(Placex, Placey);
	};
	
	// important tiles in the map are in here. 

	void SetxandY(int xa, int ya)
	{
		x = xa;
		y = ya;
	}

	pair<int,int> buildxandy()
	{
		return make_pair(x, y);
	
	}
	void seexandy()
	{
		cout << " X: " << buildxandy().first << "|Y: " << buildxandy().second << " |" << endl;
	}

private:
	int x;
	int y;
};


class Planet : public MapTile
{
public:
	Planet(int Placex, int Placey, string name) : MapTile(Placex, Placey)
	{
		planet_name = name;
		signal_equipment = 0;
		population = 0;
		is_sun = false;
	};
	//that is not prety 
	void set_sun()
	{
		is_sun = true;
	}

	bool move()
	{
		return is_sun;
	}
	void Popchange()
	{
		//thinking how to implement this... 
		if (is_sun == false)
		{
			population + 100;
		}
	};


private:
	bool is_sun;
	string planet_name;
	int signal_equipment;
	int population;

};



class WormHole : public MapTile
{
public:

	WormHole(int Placex,int Placey,string name) : MapTile(Placex, Placey)
	{
		cout << "debugmode:" << endl;
		Wormhole_name = name;
		cout << Wormhole_name << endl;
		wormhole_active = 1;
		cout << wormhole_active << endl;
		pair<int,int> owc = buildxandy();
		cout << owc.first << owc.second << endl;
		pair<int, int > indic = make_pair(450, 1500);
		cout << indic.first << indic.second << endl;
		pair<int,int> temppair (owc.first+indic.first,owc.second+indic.second);
		cout << temppair.first << temppair.second << endl;
		wormholeb = temppair;
		cout << wormholeb.first << wormholeb.second << endl;
		pair<int, int> temppairb(wormholeb.first + indic.first, wormholeb.second + indic.second);
		cout << temppairb.first << temppair.second << endl;
		wormholec = temppairb;
		cout << wormholec.first << wormholec.second << endl;
		pair<int, int> temppairc(wormholec.first + indic.first, wormholec.second + indic.second);
		cout << temppairc.first << temppairc.second << endl;
		wormholed = temppairc;
		cout << wormholed.first << wormholed.second << endl;
	};

	void seeWormhole()
	{
		cout << Wormhole_name << endl;
		if (wormhole_active == 1)
		{
			cout << "->";
			seexandy();
		}
		else
		{
			seexandy();
		}
		
		if (wormhole_active == 2)
		{
			cout << "-> X:" << wormholeb.first << "|Y: " << wormholeb.second << " |" << endl;
		}
		else
		{
			cout << "   X: " << wormholeb.first << "|Y: " << wormholeb.second << " |" << endl;
		}
		if (wormhole_active == 3)
		{
			cout << "-> X:" << wormholec.first << "|Y: " << wormholec.second << " |" << endl;
		}
		else
		{
			cout << "   X: " << wormholec.first << "|Y: " << wormholec.second << " |" << endl;
		}
		if (wormhole_active == 4)
		{
			cout << "-> X:" << wormholed.first << "|Y: " << wormholed.second << " |" << endl;
		}
		else
		{
			cout << "   X: " << wormholed.first << "|Y: " << wormholed.second << " |" << endl;
		}
	};

	/*void Wormholeevent()
	{
		
	};
	*/
	
	void wormhole_change()
	{
		if (minutetick == 45)
		{
			wormhole_active++;
			if (wormhole_active > 5)
			{
				wormhole_active = 1;
			}
			seeWormhole();
		}
	}
private:
	string Wormhole_name;
	int wormhole_active;
	pair<int, int> wormholeb;
	pair<int, int> wormholec;
	pair<int, int> wormholed;
};

//1000 (placeholder,
Spaceship* shipPointers[1000];

void createShip(int x, int y)
{

	Spaceship* newship = new Spaceship(x, y);
	shipPointers[Spaceship::shipcount] = newship;

};

void DeleteShip(int shipkey)
{
	delete shipPointers[Spaceship::shipcount];
};

class Spaceship : public MapTile
{
public:

	static int shipcount;

	Spaceship(int Placex, int Placey) : MapTile(Placex, Placey)
	{
		shipcount++;
		shipkey = shipcount;
	};
	~Spaceship()
	{
		delete shipPointers[shipcount];
	};

	int select_ship_event()
	{
		return shipkey;
	}
	void Displayship()
	{
		cout << shipname << endl;
		cout << faction << endl;
		cout << signal << endl;
		seexandy();
	};

private:
	string shipname;
	string faction;
	int signal;
	int shipkey;
};

int Spaceship::shipcount = 0;


/*
okok I need to work on this latter, but I need to make a list of pointers that are created in the ship contructor. this will be used to point to ships in the sectors... ok. 
*/
// thid was already done, 


	





void gametime()
{
	// uh ok, this will define the time dialation;.... maybe I should make a class to this... 
	// gona leave this for monday... 
	// ok this is how this is going to work, events will be in a class, the class will reference the ticknumber, and then fire it when it happens. 

	
	while (statusofgame == true)
	{
		this_thread::sleep_for(chrono::milliseconds ( static_cast<long>(1000 * speed)));
		
		ticknumber++;
		if (ticknumber > 60)
		{
			ticknumber = 0;
			minutetick++;
		};
		if (minutetick > 60)
		{
			minutetick = 0;
			hourtick++;
		};
		if (hourtick > 18000)
		{
			hourtick = 0;
		};
	}

//ok this sounds ok so far, now I want to try this inside a class probably
};





int distance(pair<int, int> one, pair<int, int> two)
{
	//there you go, this one will see the distance.
	//ok, this is working now, I'll comment the debug ones 

	float xside = one.first - two.first;
	//cout << xside << endl;
	float yside = one.second - two.second;
	//cout << yside << endl;
	if (xside < 0)
	{
		xside = xside * -1;
		//cout << xside << endl;
	}
	if (yside < 0)
	{
		yside = yside * -1;
		//cout << yside << endl;
	}
	xside = pow(xside,2);
	//cout << xside << endl;
	yside = pow(yside,2);
	//cout << yside << endl;
	float result = xside + yside;
	//cout << result << endl;
	result = pow(result, 0.5f);
	//cout << result << endl;
	return result;
	
};

float timescale(pair<int, int > a, pair <int, int > b)
{
	int result = distance(a, b)/100;
	return result;
	
};

// make this a bunch of static functions in a event class. 
class GameEvents

{
public:
	static void eventNow(pair<int, int> a, pair<int, int> b)
	{
		//random_event();
		//cout << random_event << endl;
		int timelag = timescale(a, b);
		cout << timelag << endl;
		int laghourtick = 0;
		int lagminutetick = 0;

		if (timelag > 3600)
		{
			laghourtick = timelag / 3600;
			timelag = timelag - (laghourtick * 3600);
			laghourtick = laghourtick + check_current_tik_hour();
		}
		if (timelag > 60)
		{
			lagminutetick = timelag / 60;
			timelag = timelag - (lagminutetick * 60);
		}
		cout << laghourtick << endl;
		cout << lagminutetick << endl;
		cout << timelag << endl;
		bool eventhappend = false;
		while (!eventhappend)
		{if ( laghourtick =! 0 && laghourtick == hourtick)
			{
				if (lagminutetick =! 0 && lagminutetick == minutetick)
				{
					if (timelag == ticknumber)
					{
					cout << "event happend" << endl;
					break;
					}
				}
			}
			if (lagminutetick = !0 && lagminutetick == minutetick)
			{
				if (timelag == ticknumber)
				{
				cout << "event happend" << endl;
				break;
				}
			}
			if (timelag == ticknumber)
			{
			cout << "event happend" << endl;
			break;
			}
		}
	}

	static int check_current_tik_second()
	{
		return ticknumber;
	};
	static int check_current_tik_minute()
	{
		return minutetick;
	};
	static int check_current_tik_hour()
	{
		return hourtick;
	};

	static int random_event()
	{
		//OH GOD;
	
	};

	/*static pair<int, int> move_event()
	{

	};

	static int atack_event()
	{

	};
	static int figthit_event()
	{

	};

	static pair<int, int> move_planet()
	{

	};
	*/
};

pair <int, int> shipselected(a)
{

}

void Run_Event()
{
	while (statusofgame == true)
	{
		pair<int, int> a = ;
		GameEvents::random_event();
		pair <int, int> b = make_pair(0, 600);
		GameEvents::eventNow(a, b);
	}
}

 int main()
{
	Planet HomePlanet(0, 6000, "Home Planet");
	HomePlanet.seexandy();
	Planet testPlanet(299, 149, "planet1");
	testPlanet.seexandy();
	Planet sun(0, 0, "Sun");
	sun.set_sun();
	cout << distance(HomePlanet.buildxandy(), testPlanet.buildxandy()) << endl;
	cout << distance(HomePlanet.buildxandy(), sun.buildxandy()) << endl;
	cout << timescale(HomePlanet.buildxandy(), sun.buildxandy()) << endl;
	WormHole Alpha(9000,12000,"Alpha");
	Alpha.seexandy();
	Alpha.seeWormhole();
	WormHole Bravo(7000, 58000, "Bravo");
	Bravo.seeWormhole();
	statusofgame = true; 
	std::thread TimeThread(gametime);
	//std::thread Events(Run_Event);
	
	cout << "game is running in time" << endl;
	while (statusofgame == true)
	{
		//cout << ticknumber << endl;
		Alpha.wormhole_change();
		Bravo.wormhole_change();
	};
	TimeThread.join();
	
	
	
 }

