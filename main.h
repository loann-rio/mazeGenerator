// Includes
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <algorithm>





// Constantes du programme
const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 800;

// Namespaces
using namespace sf;
using namespace std;

// Prototypes
void mainLoop();
void generateMaze();
void solveMaze();
void setNextPos(Vector2i pos);

// struct cell
struct Cell
{
	bool visited = false;
	vector<Vector2<int>> neighbors;
	Vector2<int> from;
};

// maze
const int width = 10;
const int height = 10;

Vector2i startPos = { 0, 0 };
Vector2i endPos = { width-1, height-1 };

Vector2f sizeCell = Vector2f(700 / width, 700 / width);

Cell maze[width][height];

// rand
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> distribution(1, 24);



