#include <iostream>;
#include <string>;
#include <vector>;
#include <utility>;
#include <random>;
#include <fstream>
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
-messages

Functions:
-Rng  
-create ship
-gametime
-capturestring
-Log function
-shipselected
-runEvent

HALF-DONE: 
Classes:
-Ship
-Event
-Wormhole

Functions: 


-capturestring
PlayerCommands.



TODO

functions:


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
	//log("random number made");
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
	messageQueue.pop();
	return output;
};

string use_string(int a)
{
	return std::to_string(a);
};

//string use_string_from_pair(pair <int,int> a)
//{
//	string b = std::to_string(a.first);
//	b.append(" ", a.second);
//	return b;
//};
//ok this kind of work "sometimes" great, lets make a workaround shall we?

void log(string a)
{
	ofstream outputFile("log.txt", std::ios::app);
	if (outputFile.is_open())
	{
		outputFile << a << endl;
		outputFile.close();
	}
};

void LogValue(string a, int b)
{
	ofstream outputFile("log.txt", std::ios::app);
	if (outputFile.is_open())
	{
		outputFile << a <<" | " << b << endl;
		outputFile.close();
	}
}
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


	pair <int,int> returnActiveWormhole()
	{
		
		if (wormhole_active == 1)
		{
			pair <int, int> a = buildxandy();
			return a;
		}

		if (wormhole_active == 2)
		{
			return wormholeb;
		}
		
		if (wormhole_active == 3)
		{
			return wormholec;
		}
		
		if (wormhole_active == 4)
		{
			return wormholed;
		}
		

	};
	
	
	void wormhole_change()
	{
		if (minutetick == 45)
		{
			wormhole_active++;
			if (wormhole_active == 5)
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

	static atomic<int> shipcount;

	Spaceship(int Placex, int Placey) : MapTile(Placex, Placey)
	{
		shipkey = shipcount;
		LogValue("SPACESHIP_CONTRUCTOR-> shipKey",shipkey);
		int a = Rng();
		LogValue("SPACESHIP_CONTRUCTOR-> RNG",a);
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
		//set_position();(redundant)
		log("SPACESHIP_CONTRUCTOR-> the fleet composition was set.");
		LogValue("SPACESHIP_CONTRUCTOR-> Picket		->", picket);
		LogValue("SPACESHIP_CONTRUCTOR-> corvete	->", corvete);
		LogValue("SPACESHIP_CONTRUCTOR-> destroyer	->", destroyer);
		LogValue("SPACESHIP_CONTRUCTOR-> cruiser	->", cruiser);
		LogValue("SPACESHIP_CONTRUCTOR-> carrier	->", carrier);
		LogValue("SPACESHIP_CONTRUCTOR-> merchant	->", merchant);
		LogValue("SPACESHIP_CONTRUCTOR-> mining		->", mining);
		spoted = false;
	};
	~Spaceship()
	{
		
	};
	void updateShipCount()
	{
		shipcount++;
	}
	int select_ship_event()
	{
		return shipkey;
	}
	void Displayship()
	{
		cout << shipname << endl;
		cout << faction << endl;
		cout << signal << endl;
		see_target();
		seexandy();
		cout << shipkey << endl;
		cout << spoted << endl;
		
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

		LogValue("SET_TARGET() -> Shipselected:", shipkey);
		LogValue("SET_TARGET() -> INT X: ", k);
		LogValue("SET_TARGET() -> int X: ", t);

		move_target = make_pair(k, t);
	};

	pair<int, int> ret_target()
	{
		return move_target;
	}
	void see_target()
	{
		cout << move_target.first << move_target.second << endl;
	}
	
	void shipWasSpoted()
	{
		spoted = true;
		cout << "ship spoted setted to true" << endl;
	}
	bool shipisSpoted()
	{
		
		return spoted;
	}

	pair<int, int> ret_position()
	{
		int importx = buildxandy().first;
		LogValue("RET_POSITION() -> int X: ", importx);
		int importy = buildxandy().second;
		LogValue("RET_POSITION() -> int Y: ", importy);
		pair<int, int> b = make_pair(importx, importy);
		return b;
	}
	void setPointerManualy();
	
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
	pair<int, int> position;
	bool spoted;
};

atomic<int> Spaceship::shipcount = 0;

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
		if (a < 97)
		{
			return 1;
		}
		else if (a < 98)
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

int shipselected(int locationA,int locationB)
{
	int locationX = locationA;
	int locationY = locationB;
	log("ship is about to be selected");
	for (int i = 0; i > Spaceship::shipcount ; i++)
	{
		int PosX = shipPointers[i]->ret_position().first;
		int PosY = shipPointers[i]->ret_position().second;
		if (PosX == locationX && locationY == PosY)
		{
			LogValue("shipselected() -> int X: ",i );
			log("shipselected() -> ship was selected");
			return i;
		}
	
	}
	
}

void Run_Event()
{
	while (statusofgame == true)
	{
		string c;
		log("RUN_EVENT()->starting event");
		pair<int, int> a;
		int theevent = GameEvents::random_event();
		LogValue("the event selected was", theevent);
		if (theevent == 1)
		{
			//a = GameEvents::event_target();
			c = "nothing happens";
		}
		else if (theevent == 2)
		{
			a = WormholePointers[0]->returnActiveWormhole();
			LogValue("RUN_EVENT() -> wormhole used x:", a.first);
			LogValue("RUN_EVENT() -> wormhole used y:", a.second);
			createShip(a.first, a.second);
			int shipindex = shipselected(a.first, a.second);
			LogValue("RUN_EVENT() -> shipindex", shipindex);
			shipPointers[shipindex]->set_target(WormholePointers[1]->buildxandy().first, WormholePointers[1]->buildxandy().second);
			c = "a ship was spoted leaving wormhole Alpha";
			shipPointers[shipindex]->updateShipCount();
		}
		else
		{

			a = WormholePointers[1]->returnActiveWormhole();
			LogValue("RUN_EVENT() -> wormhole used x:", a.first);
			LogValue("RUN_EVENT() -> wormhole used y:", a.second);
			createShip(a.first, a.second);
			int shipindex = shipselected(a.first, a.second);
			LogValue("RUN_EVENT() -> shipindex", shipindex);
			shipPointers[shipindex]->set_target(WormholePointers[0]->buildxandy().first, WormholePointers[1]->buildxandy().second);
			c = "a ship was spoted leaving wormhole Bravo";
			shipPointers[shipindex]->updateShipCount();
		}
		pair <int, int> b = make_pair(0, 6000);
		GameEvents::eventNow(a, b, c);
		int i = Spaceship::shipcount;
		if (theevent > 1)
		{
			shipPointers[i - 1]->shipWasSpoted();
		}
	}
}
//this is bugged, gotta work out what is happening here. 
/*void WormHoleManneger()
{
	for (int i = 0; WormHole::wormholeIndex; i++)
	{
		WormholePointers[i]->wormhole_change();
	}
}
*/
void movementMannager()
{
	int a = 0;
	LogValue("Moviment mannager() -> A VALUE", a);
	log("Moviment mannager()->Ship count is on:");
	LogValue("Moviment mannager() ->Ship count is on : ", Spaceship::shipcount);

	//int shipcountrealforlog = Spaceship::shipcount - 1;
	//log(use_string(shipcountrealforlog));
	int ships = Spaceship::shipcount ;
	// 1000 é temporario
	//for (int i = 0; i <= ships; i++)
	while (ships > a)
	{
		//log("I value:");
		//log(use_string(i));
		LogValue("Moviment mannager() -> A VALUE SET TO 0", a);
		log("setting pair value");
		//what a fucking dumb way to go about this
		int importx = shipPointers[a]->ret_position().first;
		LogValue("Moviment mannager() -> X", importx);
		int importy = shipPointers[a]->ret_position().second;
		LogValue("Moviment mannager() -> Y", importy);
		pair<int, int> b = make_pair(importx, importy);
		log("pair value for ship is set.");
		importx = shipPointers[a]->ret_target().first;
		LogValue("Moviment mannager() -> X", importx);
		importy = shipPointers[a]->ret_target().second;
		LogValue("Moviment mannager() -> Y", importy);
		pair<int, int> c = make_pair(importx, importy);
		log("Moviment mannager() ->pair value for ship target is set.");
		
		if (b.first > c.first && b.second > c.second)
		{
			shipPointers[a]->SetxandY(b.first + 1, b.second + 1);
			LogValue("Moviment mannager() -> moved x+1 y+1 ->", a);
		}
		if (b.first > c.first && b.second == c.second)
		{
			shipPointers[a]->SetxandY(b.first + 1, b.second + 0);
			LogValue("Moviment mannager() -> moved x+1 ship ->", a);
		}
		if (b.first == c.first && b.second > c.second)
		{
			shipPointers[a]->SetxandY(b.first + 0, b.second + 1);
			LogValue("Moviment mannager() -> moved y+1 ship ->", a);
		}
		if (b.first < c.first && b.second < c.second)
		{
			shipPointers[a]->SetxandY(b.first - 1, b.second - 1);
			LogValue("Moviment mannager() -> moved x-1 y-1 ship ->", a);
		}
		if (b.first < c.first && b.second == c.second)
		{
			shipPointers[a]->SetxandY(b.first - 1, b.second + 0);
			LogValue("Moviment mannager() -> moved x-1  ship ->", a);
		}
		if (b.first == c.first && b.second < c.second)
		{
			shipPointers[a]->SetxandY(b.first + 0, b.second - 1);
			LogValue("Moviment mannager() -> moved y-1 ship ->", a);
		}
		log("Moviment mannager() ->ship probably moved.");
		a++;
		log("Moviment mannager() ->A adedd one,");
		LogValue("Moviment mannager() -> A VALUE", a);
	}
}

void runMovimentMannager()
{
	while (statusofgame == true)
	{
		string a = "Moviment_mannager Running";
		log(a);
		messages(a);
		movementMannager();
		
		
	};
};


//hello there, seeeting it up over here, this is spoilers... dont be spoiled. 
int partition(int arr[], int low, int high)
{
	int pivot = arr[high];
	int i = (low - 1);

	for (int j = low; j <= high - 1; ++j) {
		if (arr[j] < pivot) {
			++i;
			std::swap(arr[i], arr[j]);
		}
	}

	std::swap(arr[i + 1], arr[high]);
	return (i + 1);
};


void sortDist(int arr[], int low, int high)
{
	if (low < high) 
	{
		int pivotIndex = partition(arr, low, high);

		sortDist(arr, low, pivotIndex - 1);
		sortDist(arr, pivotIndex + 1, high);
	}
}





void PlayerCommands()
{
	string a;
	cin >> a;
	int b;
	b = 0;
	if (a == "Help" || a == "help" || a == "h" || a == "H")
	{
		b = 0;
	}
	if (a == "Radar" || a == "radar" || a == "r" || a == "R")
	{
		b = 1;
	}
	if (a == "Move" || a == "move" || a == "m" || a == "M")
	{
		b = 2;
	}
	if (a == "RTB" || a == "rtb" || a == "b" || a == "B")
	{
		b = 3;
	}
	if (a == "Wormhole" || a == "wormhole" || a == "w" || a == "W")
	{
		b = 4;
	}
	int contactNumber = Spaceship::shipcount - 1;
	cout << "contact number is on " << contactNumber << endl;
	pair <int, int> source = make_pair(0, 6000);
	pair <int, int> Pares;
	int shipx[1000];
	int shipy[1000];
	int DistanceFromSource[1000];
	int TimeShips[1000];
	int ticks;
	int shipsspoted = 0;
	

	switch (b)
	{
	case 0:
		cout << "Commands: " << endl;
		cout << "H or Help command  -> it displays the player commands" << endl ;
		cout << "R or Radar command -> it displays the Radar contacts. " << endl;
		cout << "M or Move Command  -> moves the Home fleet" << endl;
		cout << "B or RTB command   -> returns the fleet home" << endl;
		break;
	case 1:
		//not implemented yet;
		// 
		// as expected bugs were found 
		
		cout << "Active radar ping starting..." << endl;
		cout << "Pulse time: ";
		cout << hourtick <<":"<< minutetick << ":" << ticknumber << endl;
		// ok so I'll have to use event now a lot of times? how exactly I'll do this, I guess I'll first take all the targets and then sort them by distance, and then display thm 
		// letsgo making those fun sorting algorithins my teachers love 
		
		// ok there we go, this should do it; 
		
		for (int i = 0; i <= contactNumber; i++)
		{
			shipx[i] = shipPointers[i]->ret_position().first;
		}
		for (int i = 0; i <= contactNumber; i++)
		{
			shipy[i] = shipPointers[i]->ret_position().second;
		}
		// numbers were asagin;
		for (int i = 0; i <= contactNumber; i++)
		{
			cout << shipx[i] << " " << shipy[i] << " " << endl;
		}


		cout << "numbers were asagin" << endl;
		
		for (int i = 0; i <= contactNumber; i++)
		{
			Pares = make_pair(shipx[i], shipy[i]);
			cout << "Pares were set" << Pares.first << Pares.second <<endl;
			cout << shipPointers[i]->shipisSpoted() << endl;
			if (shipPointers[i]->shipisSpoted() == true)
			{
				cout << "shipfound" << i << endl;
				DistanceFromSource[i] = distance(Pares, source);
				shipsspoted++;
			}
		};
		cout << "distance" << endl;
		for (int i = 0; i <= shipsspoted; i++)
		{
			cout << DistanceFromSource[i] << endl;
		}
		//cout << "now we delete the first ones " << endl;
		// I dont need those two anymore. 
		// wtf just stop leaking you bastard 
		//moved downards 

		// ok now lets work with what we got to sort things 
		//using quicksort... it is upp there ^
		cout << "sort the distance" << endl;
		sortDist(DistanceFromSource, 0, shipsspoted);
		for (int i = 0; i <= shipsspoted; i++)
		{
			cout << DistanceFromSource[i] << endl;
			TimeShips[i] = DistanceFromSource[i]/100;
		}
		//cout << "delete this shit" << endl;
		ticks = 0;
		//I'm inplementing this still 
		/*
		while (TimeShips[shipsspoted] > ticks)
		{
			for (int i = 0; i <= shipsspoted; i++)
			{
				if (TimeShips[i] == ticks)
				{
					
				}
			}
		}
		*/
		break;
	case 2:
		cout << "Please provide the integer orders values :" << endl;
		cout << "The X value:" << endl;
		int a;
		int b;
		char confirm;
		cin >> a;
		cout << "value added was : " << a << endl;
		cout << "if you desire to change it press N and Enter to confirm" << endl;
		cin >> confirm;
		while (confirm == 110 || confirm == 78) {
			cin >> a;
			cout << "value added was : " << a << endl;
			cout << "if you desire to change it press N and Enter to confirm" << endl;
			cin >> confirm;
		}
		cout << "The Y value:" << endl;
		cin >> b;
		cout << "value added was : " << b << endl;
		cout << "if you desire to change it press N and Enter to confirm" << endl;
		cin >> confirm;
		while (confirm == 110 || confirm == 78) {
			cin >> b;
			cout << "value added was : " << b << endl;
			cout << "if you desire to change it press N and Enter to confirm" << endl;
			cin >> confirm;
		}
		shipPointers[0]->set_target(a, b);

		break;
	case 3: 
		cout << "home fleet will return to base";
		shipPointers[0]->set_target(0, 6000);

		break;
	default:
		cout << "Invalid option, use H to get the correct commands" << endl;

		break;

	case 4:
		WormholePointers[0]->seeWormhole();
		WormholePointers[1]->seeWormhole();
		break;
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
	Alpha.set_pointer();
	WormHole Bravo(7000, 58000, "Bravo");
	Bravo.seeWormhole();
	Bravo.set_pointer();
	Spaceship HomeFleet(0,6000);
	HomeFleet.set_name("Home Fleet", "Civ");
	HomeFleet.set_target(0, 6000);
	//HomeFleet.Displayship();
	HomeFleet.setPointerManualy();
	HomeFleet.updateShipCount();
	HomeFleet.shipWasSpoted();
	HomeFleet.Displayship();
	//cout << GameEvents::event_target().second;
	cout << shipPointers[0] -> ret_position().second << endl;
	statusofgame = true; 
	std::thread TimeThread(gametime);
	std::thread Events(Run_Event);
	//std::thread Wormholes(WormHoleManneger);
	std::thread movimentMannager(runMovimentMannager);

	cout << "game is running in time" << endl;
	while (statusofgame == true)
	{
		PlayerCommands();
		//cout << "debug on" << endl;
		std::unique_lock<std::mutex> lock(queueMutex);
		queueCondition.wait(lock, [] { return !messageQueue.empty(); });
		messageQueue.pop();
		//cout << ticknumber << endl;

	};
	TimeThread.join();
	Events.join();
	movimentMannager.join();
	
	
 }

void Spaceship::setPointerManualy()
	{
		shipPointers[Spaceship::shipkey] = this;
	}
