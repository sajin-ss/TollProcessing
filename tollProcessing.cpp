#include <iostream>
#include <map>
#include <vector>

using namespace std;

// enum veh_type_enum
// {
	string veh[] = {"scooter"};//, "bike", "car", "jeep"};
// };

map<string, int> veh_type_conv;

struct tollData
{
	int vehid;
	float toll_paid;
};

class TollStations
{
public:
	static int nextID;
	vector<tollData> tollList;
	float totalSum;

	TollStations()
	{
		stationID = nextID++;
		fillCharges();
		totalSum = 0;
	}

	void addToList(int v, float p)
	{
		tollData t;
		t.vehid = v;
		t.toll_paid = p;
		totalSum += p;
		tollList.push_back(t);
	}

	void showList()
	{
		for (vector<tollData>::iterator i = tollList.begin(); i != tollList.end(); ++i)
		{
			cout << i->vehid << " | " << i->toll_paid << endl;
		}
	}
	
	int stationID;
	map<int, float> charges;

	void fillCharges()
	{
		cout << "\n For Toll Station " << stationID << endl << "----------------------";
		for (map<string, int>::iterator i = veh_type_conv.begin(); i != veh_type_conv.end(); ++i)
		{
			cout << "\n Enter toll for " << i->first << " : ";
			cin >> charges[i->second];
		}
	}
};

int TollStations::nextID = 0;

class User
{
public:
	User()
	{
		user_id = -1;
		isVIP = -1;
	}
	void detailFill()
	{
		cout << "\n Enter user id : ";
		cin >> user_id;
		cout << "\n is VIP : ";
		cin >> isVIP;
	}

	void getDetails()
	{
		cout << endl << "UserID : " << user_id << "  isVIP : " << isVIP << endl; 
	}

	int user_id;
	int isVIP;
};

class Journey
{
public:
	Journey(int s, int d, float a, float b)
	{
		start = s;
		dest = d;
		amt1 = a;
		amt2 = b;
	}

	float getJourney()
	{
		cout << endl << start << " -> " << dest << " : " << amt1 << " : " << amt2 << endl;
		return amt2;
	}

	int start;
	int dest;

	float amt1;
	float amt2;
};

class Vehicle
{
public:
	Vehicle()
	{}
	Vehicle(int id, int type)
	{
		veh_id = id;
		veh_type = type;
		owner.detailFill();
	}

	void entry(int id, int type)
	{
		veh_id = id;
		veh_type = type;
		owner.detailFill();
	}

	void addJourney(int s, int d, float a, float b)
	{
		Journey j(s, d, a, b);
		journeys.push_back(j);
	}

	void showJourneys()
	{
		float totalPay = 0;
		cout << "\n Vehicle ID : " << veh_id;
		cout << "\n Vehicle Type : " << veh_type;
		cout << "\n Owner details : " ;
		owner.getDetails();
		for (vector<Journey>::iterator i=journeys.begin(); i!= journeys.end(); ++i)
		{
			totalPay += i->getJourney();
		}
		cout << "\n Total paid : Rs." << totalPay;
	}
	
	int veh_id;
    int veh_type;
	User owner;
	vector<Journey> journeys;
};

float calcTotalToll(map<int, Vehicle* > vehData, TollStations ts[], int n, int vehicle_id, int vehicle_type, int start, int dest)
{
	int toll = 0;
	if ((start < 0 || start >= n) || (dest < 0 || dest >=n))
		return -1;
	int i = start;
	while (i!=dest)
	{
		toll += ts[i].charges[vehicle_type];

		if (!vehData[vehicle_id]->owner.isVIP)
			ts[i].addToList(vehicle_id, ts[i].charges[vehicle_type]);
		else
			ts[i].addToList(vehicle_id, 0.8*ts[i].charges[vehicle_type]);


		if (i<dest)
			i++;
		else
			i--;
	}

	toll += ts[i].charges[vehicle_type];
	if (!vehData[vehicle_id]->owner.isVIP)
			ts[i].addToList(vehicle_id, ts[i].charges[vehicle_type]);
		else
			ts[i].addToList(vehicle_id, 0.8*ts[i].charges[vehicle_type]);

	return toll;
}

void findRouteAndToll(int s, int d, int vtype, TollStations ts[], int n)
{
	int i = s;
	float toll1=0, toll2=0;
	if (s<d)	//s -> d by right ===>
	{
		while(i<=d)
			toll1 += ts[i++].charges[vtype];
	}
	else
	{
		while (i<n)
			toll1 += ts[i++].charges[vtype]; 
		i=0;
		while (i<=d)
			toll1 += ts[i++].charges[vtype];
	}

	i = s;
	if (s>d)	//s -> d by left <===
	{
		while(i>=d)
			toll2 += ts[i--].charges[vtype];
	}
	else
	{
		while (i>=0)
			toll2 += ts[i--].charges[vtype]; 
		i=n-1;
		while (i>=d)
			toll2 += ts[i--].charges[vtype];
	}

	cout << toll1 << ":" << toll2 << endl;
	if (toll1>=toll2)
		cout << "\n Toll (right) : " <<  toll2;
	else
		cout << "\n Toll (left) : " <<  toll1;

}

int main(int argc, char const *argv[])
{
	for (int i = 0; i < sizeof(veh)/sizeof(veh[0]); ++i)
		veh_type_conv[veh[i]] = i;

	int n, ch;
	cout << "\n Enter no of toll stations : ";
	cin >> n;

	TollStations stn[n];
	Vehicle vehDB[100];
	int veh_index = 0;
	map<int, Vehicle* > vehicles;



	cout << "\nMENU\n1.Vehicle toll\n2.Station Details\n3.Vehicle Details\n4.Circular Path\n";

	while(ch!=5)
	{
		cout << endl << "===> ";
		cin >> ch;
		switch(ch)
		{
			case 1:		
			{			               //Given the details of vehicle type, start and destination…
				int vehicle_type, start, dest, vehid;		// …display the total toll paid during
				cout << "\n Enter veh_type (1-scooter; 2-bike; 3-car; 4-jeep) ";
				cin >> vehicle_type;			// the journey and print the amount after applying the discount.
				vehicle_type--;
				cout << "\n Enter veh ID : ";
				cin >> vehid;

				cout << "\n Enter start and dest : ";
				cin >> start >> dest;

				Vehicle v;

				if (vehicles.find(vehid) == vehicles.end())
				{
					v.entry(vehid, vehicle_type);
					vehDB[veh_index++] = v;
					vehicles.insert(make_pair(vehid, &vehDB[veh_index-1]));
				}
				
				float x = calcTotalToll(vehicles, stn, n, vehid, vehicle_type, start, dest);
				float y;

				if (vehicles[vehid]->owner.isVIP)
					y = 0.8 * x;
				else
					y = x;
				cout << "\n Total Toll = " << x;
				cout << "\n Total Toll after Discount = " << y;

				if (x!=-1)
					vehicles[vehid]->addJourney(start, dest, x, y);
				break;
			}
			case 2:
				int d;
				cout << "\n Enter station no : ";
				cin >> d;
				stn[d].showList();

				break;
			case 3:
				for (map<int, Vehicle*>::iterator i = vehicles.begin(); i != vehicles.end(); ++i)
				{
					i->second->showJourneys();
				}
				break;
			case 4:
			{
				int start, dest, vehicle_type;
				cout << "\n Enter toll stations : ";
				cin >> start >> dest;
				cout << "\n Enter veh_type (1-scooter; 2-bike; 3-car; 4-jeep) ";
				cin >> vehicle_type;
				vehicle_type--;
				findRouteAndToll(start, dest, vehicle_type, stn, n);
				break;
			}
			default:
			ch=5;
			break;
		}
	}
	return 0;
}