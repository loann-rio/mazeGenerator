// Inclusion des fonctionnalités
#include "main.h"

// La fenêtre du jeu
RenderWindow window;

int main()
{
    // Options de la fenêtre
    ContextSettings options;
    options.antialiasingLevel = 8;

    // Création d'une fenêtre type RenderWindow
    window.create(VideoMode(WIN_WIDTH, WIN_HEIGHT, 32), "maze of sisyphe", Style::Default, options);

    // Activation du vsync
    window.setVerticalSyncEnabled(true);

    mainLoop();

    return 0;
}

void mainLoop()
{
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear(Color::White);
        generateMaze();
        solveMaze();
        window.display();
    }
}

void generateMaze()
{
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            maze[i][j] = Cell();
        }
    }

    RectangleShape rect = RectangleShape();
    rect.setPosition(50, 50);
    rect.setSize({WIN_WIDTH-99, WIN_HEIGHT-99});
    rect.setFillColor(Color::Black);
    window.draw(rect);
    int visitedCell = 0;
    Vector2i pos = { 0, 0 };

    while (visitedCell < width * height)
    {
        if (!maze[pos.x][pos.y].visited) { visitedCell++; }
        pos = setNextPos(pos);
    }
}


void drawCell(Vector2i pos, Vector2i dir)
{
    RectangleShape rect = RectangleShape();
    rect.setFillColor(Color(200, 200, 200));
    int pX = 51 + sizeCell.x * pos.x;
    int pY = 51 + sizeCell.y * pos.y;
    int sX = sizeCell.x;
    int sY = sizeCell.y;

    if (dir.x + dir.y == -1)
    {
        pX += dir.x;
        pY += dir.y;
    }

    if (dir.x == 0)
    {
        sX -= 1;
    }
    else
    {
        sY -= 1;
    }

    rect.setPosition(pX, pY);
    rect.setSize(Vector2f((float)sX, (float)sY));
    window.draw(rect);
    //window.display();
}

Vector2i setNextPos(Vector2i pos)
{
    maze[pos.x][pos.y].visited = true;
    

    vector<Vector2i> v { {-1, 0}, { 1, 0 }, { 0, -1 }, { 0, 1 }};

    // Shuffle the vector
    shuffle(v.begin(), v.end(), gen);

    for (Vector2i dir : v)
    {

        if (pos.x + dir.x >= 0 && pos.y + dir.y >= 0 &&
            pos.x + dir.x < width && pos.y + dir.y < height) 
        {
            if (!maze[pos.x + dir.x][pos.y + dir.y].visited)
            {                
                drawCell(pos, dir);
                maze[pos.x][pos.y].neighbors.emplace_back(Vector2i(pos.x + dir.x, pos.y + dir.y));
                maze[pos.x + dir.x][pos.y + dir.y].neighbors.emplace_back(Vector2i(pos.x, pos.y));
                maze[pos.x + dir.x][pos.y + dir.y].from = pos;
                return pos + dir;
            }
            else if(distribution(gen) == 1)
            {
                drawCell(pos, dir);
                maze[pos.x][pos.y].neighbors.emplace_back(Vector2i(pos.x + dir.x, pos.y + dir.y));
                maze[pos.x + dir.x][pos.y + dir.y].neighbors.emplace_back(Vector2i(pos.x, pos.y));
            }
        }        
    }

    RectangleShape rect = RectangleShape();
    rect.setFillColor(Color(200, 200, 200));
    rect.setPosition(51 + sizeCell.x * pos.x, 51 + sizeCell.y * pos.y);
    rect.setSize(sizeCell - Vector2f(1,1));
    window.draw(rect);
    return maze[pos.x][pos.y].from;
    
}

void addPos(Vector2i pos)
{
    float weight = sqrtf(powf(startPos.x - pos.x, 2) + powf(startPos.y - pos.y, 2)) + sqrtf(powf(endPos.x - pos.x, 2) + powf(endPos.y - pos.y, 2));
    auto it = possiblePos.begin();
    while (it != possiblePos.end() && weight >= it->weight) {
        ++it;
    }
    posWweight p = { pos.x, pos.y, weight };
    possiblePos.insert(it, p);
}


void solveMaze()
{
    searchMaze();
    traceWay();
    this_thread::sleep_for(chrono::seconds(2));

}

void searchMaze()
{
    possiblePos.clear();

    Vector2i pos = startPos;
    maze[pos.x][pos.y].visited = false;

    RectangleShape rect = RectangleShape();
    rect.setFillColor(Color(0, 0, 200));
    rect.setSize(sizeCell - Vector2f(1, 1));

    while (true)
    {
        if (pos == endPos)
        {
            break;
        }

        for (Vector2i p : maze[pos.x][pos.y].neighbors)
        {

            if (maze[p.x][p.y].visited)
            {
                maze[p.x][p.y].visited = false;
                maze[p.x][p.y].from = pos;
                addPos(p);
            }
        }

        pos = Vector2i(possiblePos[0].x, possiblePos[0].y);
        possiblePos.erase(possiblePos.begin());

        rect.setPosition(51 + sizeCell.x * pos.x, 51 + sizeCell.y * pos.y);
        window.draw(rect);
        window.display();
    }
}

void traceWay()
{
    RectangleShape rect = RectangleShape();
    rect.setFillColor(Color(0, 200, 200));
    rect.setSize(sizeCell - Vector2f(1, 1));

    Vector2i pos = endPos;

    while (true)
    {
        rect.setPosition(51 + sizeCell.x * pos.x, 51 + sizeCell.y * pos.y);
        window.draw(rect);
        window.display();

        pos = maze[pos.x][pos.y].from;

        if (pos == startPos) { break;  }
    }
}

