#include <iostream>;
#include <string>;
#include <vector>;
#include <utility>;
#include <random>;
#include <thread>;
#include <cmath>;
#include <atomic>;
#include <chrono>;
#include <queue>;
#include <mutex>;
#include <sstream>;



using namespace std;
std::mutex queueMutex;
std::queue<std::string> messageQueue;
std::condition_variable queueCondition;

//ok setting the objectives here 
/* 
objective n1 -> have a code all in one file, yes it is going to be messy, but well it is a interesting concept in trying to keep all to one file, the next one I'll make it all in the oposite way, as if forcing my self only to work with header files; 
objective n2 -> have time lag inplemented, ( it is almost done... the time lag calculation is there, I just need to define that the player can be in both the ship and the home_planet)
objective n2 -> have the commands that the player can use; (this is not yet inplemented, but it will run like a linux computer I guess, helping command will be important to it) 
objective n3 -> keep it organized ( yes.) 
*/

// curent status of development 
/*
DONE:
classes: 
-mapTile  
-Planet

Functions:
-Rng  
create ship
gametime

HALF-DONE: 
Classes:
-Ship
-Event
-Wormhole

Functions: 

-runEvent



shipselected 

TODO

functions:
PlayerCommands.

*/
atomic<bool> statusofgame;
atomic<int> speed = 1;
int ticknumber = 0;
int minutetick = 0;
int hourtick = 0;

int Rng()
{
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> dist(1, 100);
	return dist(rng);
};

// oh GOD, ok, messages now need to be transported between messages... how to implement this? here we go..
//note, now all messages will need to go trough the message command, and the main I'll be made as a message reader. 


void messages(string a)
{
	std::lock_guard<std::mutex> lock(queueMutex);
	messageQueue.push(a);
	queueCondition.notify_one();
};
string captureString(void x())
{
	std::ostringstream oss;
	std::streambuf* coutBuffer = std::cout.rdbuf();
	x();
	std::cout.rdbuf(coutBuffer);
	std::string output = oss.str();
	//messageQueue.pop();
	return output;
};


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

	static int wormholeIndex;

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
		Iworm = wormholeIndex;
		wormholeIndex++;
		
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
	void set_pointer();
	
private:
	string Wormhole_name;
	int wormhole_active;
	pair<int, int> wormholeb;
	pair<int, int> wormholec;
	pair<int, int> wormholed;
	int Iworm;
};

int WormHole::wormholeIndex = 0;
WormHole* WormholePointers[2];

void WormHole::set_pointer()
{
	WormholePointers[Iworm] = this;
};

class Spaceship : public MapTile
{
public:

	static int shipcount;

	Spaceship(int Placex, int Placey) : MapTile(Placex, Placey)
	{
		shipcount++;
		shipkey = shipcount;
		int a = Rng();
		set_fleet(0, 0, 0, 0, 0, 1, 0);
		//set_fleet(int CV, int CL, int DD, int KE, int PK, int AMC, int MN)
		if (a < 80)
		{
			set_fleet(0, 0, 0, 0, 0, 1, 0);
				
		}
		else if (a < 85)
		{
			set_fleet(0, 0, 0, 0, 1, 10, 1);
			
		}
		else if (a < 90)
		{
			set_fleet(0, 0, 1, 2, 4, 10, 0);
		}
		else if (a < 95)
		{
			set_fleet(1, 2, 10, 0, 5, 0, 0);
		}
		else if (a < 100)
		{
			set_fleet(1, 10, 40, 0, 10, 0, 0);
		}
	};
	~Spaceship()
	{
		
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
	void set_name(string fleet, string tempfaction)
	{
		shipname = fleet;
		faction = tempfaction;
	}
	void set_fleet(int CV, int CL, int DD, int KE, int PK, int AMC, int MN)
	{
		carrier = CV;
		cruiser = CL;
		destroyer = DD;
		corvete = KE;
		picket = PK;
		merchant = AMC;
		mining = MN;
	};
	void set_target(int k, int t)
	{
		move_target = make_pair(k, t);
	};

	pair<int, int> ret_target()
	{
		return move_target;
	}

private:
	string shipname;
	string faction;
	int signal;
	int shipkey;
	int picket;
	int corvete;
	int destroyer;
	int cruiser;
	int carrier;
	int merchant;
	int mining;
	int drive_status;
	pair<int, int> move_target;
};

int Spaceship::shipcount = 0;

//1000 (placeholder,
Spaceship* shipPointers[1000];

void createShip(int x, int y)
{

	Spaceship* newship = new Spaceship(x, y);
	shipPointers[Spaceship::shipcount] = newship;

};

void DeleteShip(int shipkey)
{
	delete shipPointers[shipkey];
};

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
	static void eventNow(pair<int, int> a, pair<int, int> b,string d)
	{
		//random_event();
		
		int timelag = timescale(a, b);
		//cout << timelag << endl;
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
		//cout << laghourtick << endl;
		//cout << lagminutetick << endl;
		//cout << timelag << endl;
		bool eventhappend = false;
		while (!eventhappend)
		{if ( laghourtick =! 0 && laghourtick == hourtick)
			{
				if (lagminutetick =! 0 && lagminutetick == minutetick)
				{
					if (timelag == ticknumber)
					{
					cout << d << endl;
					messages(d);
					break;
					}
				}
			}
			if (lagminutetick = !0 && lagminutetick == minutetick)
			{
				if (timelag == ticknumber)
				{
				cout << d << endl;
				messages(d);
				break;
				}
			}
			if (timelag == ticknumber)
			{
			cout << d << endl;
			messages(d);
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

	static pair <int, int> event_target()
	{
		
		for (int i = 0; Spaceship::shipcount; i++)
		{
			int a = Rng();
			if (a > 99)
				return shipPointers[i]->buildxandy();
		}
		
		

	}
	static int random_event()
	{
		//OH GOD this goin to take some time to implement;
		int a = Rng();
		if (a < 1)
		{
			return 1;
		}
		else if (a < 80)
		{
			return 2;
		}
		else
			return 3;
	};
	/*

	static pair<int, int> move_event()
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

pair <int, int> shipselected(int shipkey)
{
	Spaceship* a = shipPointers[shipkey];
	a->seexandy();
	return a->buildxandy() ;
	
}

void Run_Event()
{
	while (statusofgame == true)
	{
		string c;
		pair<int, int> a;
		int theevent = GameEvents::random_event();
		if (theevent == 1)
		{
			//a = GameEvents::event_target();
			c = "ship has done something";
		}
		else if (theevent == 2)
		{
			a = WormholePointers[0]->buildxandy();
			createShip(a.first, a.second);
			c = "a ship was spoted leaving wormhole Alpha";
		}
		else
		{
			a = WormholePointers[1]->buildxandy();
			createShip(a.first, a.second);
			c = "a ship was spoted leaving wormehole Bravo";
		}
		pair <int, int> b = make_pair(0, 6000);
		GameEvents::eventNow(a, b, c);
	}
}

void WormHoleManneger()
{
	for(int i=0; WormHole::wormholeIndex;i++)
	WormholePointers[i]->wormhole_change();
}

void movementMannager()
{
	// 1000 é temporario
	for (int i = 0; 1000; i++);
	{
		int a = 0;
		pair<int,int> b = shipPointers[a]->buildxandy();
		pair<int, int> c = shipPointers[a]->ret_target();
		
		if (b.first > c.first && b.second > c.second)
		{
		shipPointers[a]->SetxandY(b.first + 1, b.second + 1);
		}
		if (b.first > c.first && b.second == c.second)
		{
			shipPointers[a]->SetxandY(b.first + 1, b.second + 0);
		}
		if (b.first == c.first && b.second > c.second)
		{
			shipPointers[a]->SetxandY(b.first + 0, b.second + 1);
		}
		if (b.first < c.first && b.second < c.second)
		{
			shipPointers[a]->SetxandY(b.first - 1, b.second - 1);
		}
		if (b.first < c.first && b.second == c.second)
		{
			shipPointers[a]->SetxandY(b.first - 1, b.second + 0);
		}
		if (b.first == c.first && b.second < c.second)
		{
			shipPointers[a]->SetxandY(b.first + 0, b.second - 1);
		}
		
		a++;
	}
}

void runMovimentMannager()
{
	while (statusofgame == true)
	{
		movementMannager();
	};
};

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
	Alpha.set_pointer();
	WormHole Bravo(7000, 58000, "Bravo");
	Bravo.seeWormhole();
	Bravo.set_pointer();

	statusofgame = true; 
	std::thread TimeThread(gametime);
	std::thread Events(Run_Event);
	std::thread Wormholes(WormHoleManneger);
	std::thread movimentMannager(runMovimentMannager);

	cout << "game is running in time" << endl;
	while (statusofgame == true)
	{
		std::unique_lock<std::mutex> lock(queueMutex);
		queueCondition.wait(lock, [] { return !messageQueue.empty(); });
		cout << ticknumber << endl;

	};
	TimeThread.join();
	
	
	
 }

