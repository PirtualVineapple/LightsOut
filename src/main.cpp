#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define DIM 600


int cellDim;

struct Cell{
    bool light;
    const static int cellDim;
    float xcoord, ycoord;

    Cell(float, float);
    void state(){light = !light;}
};

const int Cell::cellDim = cellDim;

Cell::Cell(float xcoord, float ycoord)
{
    this->xcoord = xcoord;
    this->ycoord = ycoord;
    light = false;
}

Cell*** createCellList()
{
    float x = -1.0f;
    float y = -1.0f;
    Cell*** CellList = new Cell**[cellDim];

    for (int i = 0; i <= (cellDim - 1); i++)
    {
        CellList[i] = new Cell*[cellDim];

        for(int j = 0; j <= (cellDim - 1); j++)
        {
            CellList[i][j] = new Cell(x, y);
            y += 2/(float)cellDim;
        }

        x += 2/(float)cellDim;
        y = -1.0f;
    }

    return CellList;

}

Cell*** toggleLights(Cell*** CellList, int x, int y)
{
    if (x <= (cellDim - 1) && y <= (cellDim - 1))
    {
        CellList[x][y]->state();
        if (x == 0){
            CellList[x+1][y]->state();}
        else if (x == (cellDim - 1)){
            CellList[x-1][y]->state();}
        else{ CellList[x+1][y]->state();
            CellList[x-1][y]->state();}
        if (y == 0){
            CellList[x][y+1]->state();}
        else if (y == (cellDim -1 )){
            CellList[x][y-1]->state();}
        else{ CellList[x][y+1]->state();
            CellList[x][y-1]->state();}
    }
    return CellList;

}

GLFWwindow* initWindow()
{
    if (!glfwInit()){
        printf("glfwInit"); return NULL;}

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* Window = glfwCreateWindow(DIM, DIM, "Lights Out", NULL, NULL);
    if(!Window){
        printf("glfwCreateWindow"); return NULL;}

    glfwMakeContextCurrent(Window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK){
        printf("glewInit"); return NULL;}

    return Window;
}

GLfloat* createCellVertexArray(GLfloat* vertexArray, Cell*** cellList)
{
    GLfloat diff = 2/cellDim;
    vertexArray = new GLfloat[cellDim*cellDim*12];
    for (int i = 0; i <= (cellDim - 1); i++)
    {
        for (int j = 0; j <= (cellDim - 1); j++){
            GLfloat x = (GLfloat) cellList[i][j]->xcoord;
            GLfloat y = (GLfloat) cellList[i][j]->ycoord;
            int index = (i * cellDim + j) * 12;
            vertexArray[index + 1] = x        ; vertexArray[index + 1] = y        ; vertexArray[index + 2] = 0.0f ;
            vertexArray[index + 3] = x + diff ; vertexArray[index + 4] = y        ; vertexArray[index + 5] = 0.0f ;
            vertexArray[index + 6] = x        ; vertexArray[index + 7] = y + diff ; vertexArray[index + 8] = 0.0f ;
            vertexArray[index + 9] = x + diff ; vertexArray[index + 10] = y + diff; vertexArray[index + 11] = 0.0f;
        }
            
    }
    return vertexArray;

}

void startGame()
{
    GLFWwindow* Window = initWindow();
    Cell*** cellList = createCellList();
    GLfloat* cellVertexArray = createCellVertexArray(cellVertexArray, cellList);
    printf("%f", cellVertexArray[1]);
    while (!glfwWindowShouldClose(Window)){
        glfwSwapBuffers(Window);
        glfwWaitEvents();
    }
    glfwDestroyWindow(Window);
    glfwTerminate();
    

}

int main(int argc, char** argv)
{   
    if(argc == 2){
        cellDim = atoi(argv[1]);
        printf("%d",cellDim);
        if(cellDim <= 0 && cellDim > 10){
            printf("Please input an integer greater than 0 and less than 11");
            return 0;
        }
    }else{
        printf("Please input an integer greater than 0 and less than 11 after 'LightsOut' in terminal");
        return 0;
    }

    startGame();

}