#include <fstream>
#include <iostream>
#include <ctime>
#include <string.h>
#include "Graph.h"
#include "AstarAlgorithm.h"

using namespace std;

const size_t STRING_SIZE = 128;

int main(int argc, char* argv[])
{
	fstream f;
	string map;
	char c[STRING_SIZE];

	f.open("map.txt",ios::in);
	int w = 127;
	int h = 127;

	while (true)
	{
		memset(c, 0, STRING_SIZE);
		f.get(c, STRING_SIZE);
		map.append(c);
		if (f.eof())
			break;
	}
	
	Graph g(&map, w, h);
	Point startPos;
	startPos.x=39;
	startPos.y=43;
	Point endPos;
	endPos.x=126;
	endPos.y=115;

	time_t time1;
	time1 = time(&time1);


	list<Point*> result;

	list<Point*>::iterator ite;

	AstarAlgorithm astar;

	std::srand(std::time(0));

	for (int i=0; i<1000; i++)
	{
		for ( ite = result.begin(); ite != result.end(); ite++ )
		{
			delete *ite;
		}
		result.clear();
		astar.search(result, &g, &startPos, &endPos);

		startPos.x = std::rand() % w;
		startPos.y = std::rand() % h;

		endPos.x = std::rand() % w;
		endPos.y = std::rand() % h;
	}

	for ( ite = result.begin(); ite != result.end(); ite++ )
	{
		cout<<(*ite)->x<<" "<<(*ite)->y<<endl;
	}

	time_t time2 = time(&time2);
	time_t t = time2 - time1;
	cout<<"xp astar.search used "<<t<<endl;

	int i;
	cin>>i;
	
	return 0;
}
