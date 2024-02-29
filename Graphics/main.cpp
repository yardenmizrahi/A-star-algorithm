
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "glut.h"
#include <queue>
#include <vector>
#include "Cell.h"
#include <iostream>

using namespace std;

const int MSZ = 100;

const int SPACE = 0;
const int WALL = 1;
const int START = 2;
const int TARGET = 3;
const int GRAY = 4;
const int BLACK = 5;
const int PATH = 6;

int maze[MSZ][MSZ] = { 0 }; // space
bool runBestFirstSearch = false;


struct CompareHeuristic {
	bool operator()(const Cell* lhs, const Cell* rhs) const {
		return lhs->getHeuristic(lhs) > rhs->getHeuristic(rhs);
	}
};

priority_queue<Cell*, vector<Cell*>, CompareHeuristic> grays;
Cell* target = nullptr;

void InitMaze();

void init()
{
	glClearColor(0.5, 0.5, 0.5, 0);// color of window background
	glOrtho(0, MSZ, 0, MSZ, -1, 1); // set the coordinates system

	srand(time(0));

	InitMaze();
}

void InitMaze()
{
	int i, j;

	for (i = 0; i < MSZ; i++)
	{
		maze[0][i] = WALL;
		maze[MSZ - 1][i] = WALL;
		maze[i][0] = WALL;
		maze[i][MSZ - 1] = WALL;
	}

	for (i = 1; i < MSZ - 1; i++)
		for (j = 1; j < MSZ - 1; j++)
		{
			if (i % 2 == 1) // mostly spaces
			{
				if (rand() % 10 < 2) // WALL
					maze[i][j] = WALL;
				else maze[i][j] = SPACE;
			}
			else // even lines
			{
				if (rand() % 10 < 4) // space
					maze[i][j] = SPACE;
				else
					maze[i][j] = WALL;
			}
		}
	maze[MSZ / 2][MSZ / 2] = START;
	int rand_num = rand() % MSZ;
	maze[rand_num][rand_num] = TARGET;

	Cell* pc = new Cell(MSZ / 2, MSZ / 2, nullptr);  // pointer to a cell
	target = new Cell(rand_num, rand_num, nullptr);
	grays.push(pc); // insert first cell to grays

}

void DrawMaze()
{
	int i, j;

	for (i = 0; i < MSZ; i++)
		for (j = 0; j < MSZ; j++)
		{
			switch (maze[i][j]) // set convinient color of a cell
			{
			case SPACE:
				glColor3d(1, 1, 1); // white
				break;
			case WALL:
				glColor3d(0, 0, 0); // black
				break;
			case START:
				glColor3d(0.5, 0.5, 1); // blue
				break;
			case TARGET:
				glColor3d(1, 0, 0); // red
				break;
			case GRAY:
				glColor3d(1, 0.7, 0); // orange
				break;
			case BLACK:
				glColor3d(0.8, 1, 0.8); // green
				break;
			case PATH:
				glColor3d(1, 0, 1); // magenta
				break;

			}// switch

			glBegin(GL_POLYGON);
			glVertex2d(j, i);
			glVertex2d(j, i + 1);
			glVertex2d(j + 1, i + 1);
			glVertex2d(j + 1, i);
			glEnd();
		}
}

void RestorePath(Cell* pc)
{
	while (pc != nullptr)
	{
		maze[pc->getRow()][pc->getCol()] = PATH;
		pc = pc->getParent();
	}
}

void RunBestFirstSearchIteration(Cell* target)
{
	if (!grays.empty())
	{
		Cell* current = grays.top();
		grays.pop();

		if (current->getRow() == target->getRow() && current->getCol() == target->getCol())
		{
			// Target reached, restore the path
			RestorePath(current);
			runBestFirstSearch = false; // Stop the algorithm
			return;
		}

		// Mark the current cell as black (visited)
		maze[current->getRow()][current->getCol()] = BLACK;

		// Generate successor cells
		for (int i = -1; i <= 1; ++i)
		{
			for (int j = -1; j <= 1; ++j)
			{
				// Skip the current cell and diagonals
				if (i == 0 && j == 0 || abs(i) == abs(j))
					continue;

				int newRow = current->getRow() + i;
				int newCol = current->getCol() + j;

				// Check if the new position is within the maze boundaries
				if (newRow >= 0 && newRow < MSZ && newCol >= 0 && newCol < MSZ)
				{
					if (maze[newRow][newCol] == SPACE || maze[newRow][newCol] == TARGET)
					{
						// Calculate the heuristic value for the successor
						double heuristic = current->getHeuristic(target);

						// Create a new cell and add it to the priority queue
						Cell* successor = new Cell(newRow, newCol, current);
						grays.push(successor);

						// Mark the successor cell as gray
						maze[newRow][newCol] = GRAY;
					}
				}
			}
		}
	}
}


// drawings are here
void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer with background color

	DrawMaze();

	glutSwapBuffers(); // show all
}

void idle()
{

	if (runBestFirstSearch) {
		RunBestFirstSearchIteration(target);
		glutPostRedisplay();
	}

}

void menu(int choice)
{
	if (choice == 1) {
		runBestFirstSearch = true;
	}
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(900, 900);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("BestFirstSearch Example");

	glutDisplayFunc(display); // refresh function
	glutIdleFunc(idle);  // updates function
	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("run BestFirstSearch", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	glutMainLoop();
}