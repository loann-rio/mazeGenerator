// Includes
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <algorithm>
#include <thread>





// Constantes du programme
const int WIN_WIDTH = 1400;
const int WIN_HEIGHT = 1400;

// Namespaces
using namespace sf;
using namespace std;

// Prototypes
void mainLoop();
void generateMaze();
void solveMaze();
void searchMaze();
void traceWay();
Vector2i setNextPos(Vector2i pos);

// struct cell
struct Cell
{
	bool visited = false;
	vector<Vector2<int>> neighbors;
	Vector2<int> from;
};

// maze
const int width = 100;
const int height = 100;

Vector2i startPos = { 0, 0 };
Vector2i endPos = { width-1, height-1 };

Vector2f sizeCell = Vector2f((WIN_WIDTH-100) / width, (WIN_HEIGHT-100) / width);

Cell maze[width][height];

// rand
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> distribution(1, 25);


// solving:
struct posWweight { int x; int y; float weight; };
vector<posWweight> possiblePos;
