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
    window.clear(Color::White);
    generateMaze();

    while (window.isOpen())
    {

        Event event; // Variable pour gérer l'événement
        // On boucle sur les événements
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }


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
    rect.setSize({701, 701});
    window.draw(rect);

    setNextPos({ 0, 0 });
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
    window.display();
}

void setNextPos(Vector2i pos)
{
    maze[pos.x][pos.y].visited = true;

    vector<Vector2i> v { {-1, 0}, { 1, 0 }, { 0, -1 }, { 0, 1 }};

    // Shuffle the vector
    shuffle(v.begin(), v.end(), gen);
   
    bool stuck = true;

    for (Vector2i dir : v)
    {

        if (pos.x + dir.x >= 0 && pos.y + dir.y >= 0 &&
            pos.x + dir.x < width && pos.y + dir.y < height) 
        {
            if (!maze[pos.x + dir.x][pos.y + dir.y].visited)
            {
                stuck = false;
                
                drawCell(pos, dir);
                maze[pos.x][pos.y].neighbors.emplace_back(Vector2i(pos.x + dir.x, pos.y + dir.y));
                maze[pos.x + dir.x][pos.y + dir.y].neighbors.emplace_back(Vector2i(pos.x, pos.y));


                setNextPos(pos + dir);
            }
            else if(distribution(gen) == 1)
            {
                drawCell(pos, dir);

                maze[pos.x][pos.y].neighbors.emplace_back(Vector2i(pos.x + dir.x, pos.y + dir.y));
                maze[pos.x + dir.x][pos.y + dir.y].neighbors.emplace_back(Vector2i(pos.x, pos.y));
            }
        }        
    }

    if (stuck)
    {
        RectangleShape rect = RectangleShape();
        rect.setFillColor(Color(200, 200, 200));
        rect.setPosition(51 + sizeCell.x * pos.x, 51 + sizeCell.y * pos.y);
        rect.setSize(sizeCell - Vector2f(1,1));
        window.draw(rect);
    }
}

void solveMaze()
{

}


