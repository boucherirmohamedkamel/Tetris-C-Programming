#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

float Y = 18.0;//position initial y
float X = 2.0;//position initial x
float gravity = 0.05;//valeur a soustraire de Y a chaque fois pour descendre le bloc
int blockType = 3;
int grid[20][10] = {0};//playground est creer avec une taille de 20X10 elle vaut 0 de base et 1 si occupee
float gridColor[20][10][3] = {0};//matrice pour stocker la couleur de chaque bloc
float r = 1.0, g = 1.0, b = 1.0;
int gameover = 0;
int score = 0;
int blocs[5][4][4] = {//5 blocs de taille 4x4 1 occupe 0 libre
//bloc cube
{{1,1,0,0},
{1,1,0,0},
{0,0,0,0},
{0,0,0,0}},
//bloc I
{{0,0,0,0},
{1,1,1,1},
{0,0,0,0},
{0,0,0,0}},
//bloc T
{{0,1,0,0},
{1,1,1,0},
{0,0,0,0},
{0,0,0,0}},
//bloc L
{{1,0,0,0},
{1,1,1,0},
{0,0,0,0},
{0,0,0,0}},
//bloc Z
{{0,1,1,0},
{1,1,0,0},
{0,0,0,0},
{0,0,0,0}},};
float blockColors[5][3] = {
{0.0f, 1.0f, 1.0f},
{0.0f, 0.0f, 1.0f},
{1.0f, 0.5f, 0.0f},
{1.0f, 1.0f, 0.0f},
{0.6f, 0.0f, 0.6f}};


//Game over drawing (fonction drawtext)
void drawText(float x, float y, const char *text) {
    glColor3f(1.0, 1.0, 1.0);      // blanc
    glRasterPos2f(x, y);           // coordo dans le plan du game
    for (int i = 0; text[i] != '\0'; i++) //continuite de lire jusqu'a 0
    	{glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);}}

//grid
void drawGrid()
{glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINES);
    for (int x = 0; x <= 10; x++)
    {glVertex3f(x, 0, 0);
     glVertex3f(x, 20, 0);}
    for (int y = 0; y <= 20; y++)
    {glVertex3f(0, y, 0);
     glVertex3f(10, y, 0);}
    glEnd();}

//Partie Affichage
void display(void)
{glClear(GL_COLOR_BUFFER_BIT);//fonction de clear pour effacer l'ecran
 drawGrid();
 glColor3f(r, g, b);
//creation du bloc
    for(int i = 0; i < 4; i++)//difinition de la ligne du bloc 4x4
        for(int j = 0; j < 4; j++)//difinition de la colonne du bloc 4x4
            if(blocs[blockType][i][j])
            {
                float x = j + X; //X est la position finale du bloc dans la grille , J dans la 4X4
                float y = Y + i; //Y est la position finale du bloc dans la grille
                glBegin(GL_POLYGON);//desin d'un carre avec le cordination au dessus
                glVertex3f(x,     y,     0.0);
                glVertex3f(x+1.0, y,     0.0);
                glVertex3f(x+1.0, y+1.0, 0.0);
                glVertex3f(x,     y+1.0, 0.0);
                glEnd();
            }
//memorisation de chaque bloc deja posee
    for(int y = 0; y < 20; y++)
        for(int x = 0; x < 10; x++)
            if(grid[y][x] == 1)//verification de si l'un des blocs est deja dans le chemin
            {glColor3f(gridColor[y][x][0],
                          gridColor[y][x][1],
                          gridColor[y][x][2]);//garde la meme couleur du bloc
                glBegin(GL_POLYGON);
                glVertex3f(x,     y,     0.0);
                glVertex3f(x+1.0, y,     0.0);
                glVertex3f(x+1.0, y+1.0, 0.0);
                glVertex3f(x,     y+1.0, 0.0);
                glEnd();}
    if (gameover) {//appel de la fonction drawtext
        drawText(2.5f, 10.0f, "GAME OVER :)");}
    //score
    char s[32];
    sprintf(s, "Score : %d", score);
    drawText(0.5f, 18.5f, s);
    glFlush();//envoi du resultat vers l'ecran
}


void clearFullLines()
{for (int y = 0; y < 20; y++)
    {int full = 1;
        for (int x = 0; x < 10; x++)
        {if (grid[y][x] == 0)
            {full = 0;
                break;}}
        if (full)
        {score=score+100;
            for (int yy = y; yy < 19; yy++)
            {for (int x = 0; x < 10; x++)
                {grid[yy][x] = grid[yy + 1][x];
                 gridColor[yy][x][0] = gridColor[yy + 1][x][0];
                 gridColor[yy][x][1] = gridColor[yy + 1][x][1];
                 gridColor[yy][x][2] = gridColor[yy + 1][x][2];}}
            for (int x = 0; x < 10; x++)
            {grid[19][x] = 0;}
            y--;}}}


void update(int value)
{float y = Y - gravity;//descente du bloc
 int collision = 0;
//detection de collision
for(int i = 0; i < 4; i++)//verification de la position sur 4X4
   for(int j = 0; j < 4; j++)
       if(blocs[blockType][i][j] == 1)//verification l'existance d'un bloc
            {int gx = j + X;//position dans la playground
             int gy = floor(y + i);//conversion la position du dernier bloc et la mettre comme le floor
                   if(gy < 0 || grid[gy][gx] == 1)
                    collision = 1;}
//position du bloc dans la grille
    if(collision)
        {for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                if(blocs[blockType][i][j] == 1)
                    {int gx = j + X;
                    int gy = floor(Y + i);
                    if(gy >= 0 && gy < 20)
                    	if(gy >= 0 && gy < 20)
                    	    {grid[gy][gx] = 1;//memorisation du bloc
                    	    gridColor[gy][gx][0] = blockColors[blockType][0]; // stockage couleur rouge
                    	    gridColor[gy][gx][1] = blockColors[blockType][1]; // stockage couleur vert
                    	    gridColor[gy][gx][2] = blockColors[blockType][2];}} // stockage couleur bleu
        clearFullLines();
        if (Y >= 18.0) {
            gameover = 1;} else
            {Y = 18.0;
            blockType = rand() % 5;
            r = blockColors[blockType][0];
            g = blockColors[blockType][1];
            b = blockColors[blockType][2];}}
        else if (!gameover)
            {Y = y;}
    glutPostRedisplay();
    glutTimerFunc(30, update, 0);}

void rotateBlock(int type)
{int temp[4][4];//TABLEAU TEMPORAIRE POUR REMPLACER le vrai
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            temp[i][j] = blocs[type][i][j];//copie du bloc dans celle temporaire
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            blocs[type][i][j] = temp[3 - j][i];}//rotation faite par switch entre ligne et colonnes


void clavier(unsigned char touche, int x, int y)
{if(touche == 's')
        Y -= 1.0;
if(touche == 'r')
        rotateBlock(blockType);
if(touche == 'q')
        X -= 1.0;
if(touche == 'd')
        X += 1.0;
if(touche == 27)
        exit(0);
    glutPostRedisplay();}


int main(int argc, char **argv)
{glutInit(&argc, argv);
 glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
 glutInitWindowPosition(100, 100);
 glutInitWindowSize(300, 600);
 glutCreateWindow("square");
 glClearColor(0.0, 0.0, 0.0, 0.0);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(0.0, 10.0, 0.0, 20.0, -1.0, 1.0);
 glutDisplayFunc(display);
 glutKeyboardFunc(clavier);
 glutTimerFunc(30, update, 0);
 glutMainLoop();
 return 0;}
