#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define DIM 600
typedef struct Cell*** CellList;

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

CellList createCellList()
{
    float x = -1.0f;
    float y = -1.0f;
    CellList cellList = new Cell**[cellDim];

    for (int i = 0; i <= (cellDim - 1); i++)
    {
        cellList[i] = new Cell*[cellDim];

        for(int j = 0; j <= (cellDim - 1); j++)
        {
            cellList[i][j] = new Cell(x, y);
            y += 2/(float)cellDim;
        }

        x += 2/(float)cellDim;
        y = -1.0f;
    }

    return cellList;

}

CellList toggleLights(CellList cellList, int x, int y)
{
    if (x <= (cellDim - 1) && y <= (cellDim - 1) && x >= 0 && y >= 0)
    {
        cellList[x][y]->state();
        if (x == 0){
            cellList[x + 1][y]->state();}
        else if (x == (cellDim - 1)){
            cellList[x-1][y]->state();}
        else{ cellList[x+1][y]->state();
            cellList[x-1][y]->state();}
        if (y == 0){
            cellList[x][y+1]->state();}
        else if (y == (cellDim - 1)){
            cellList[x][y-1]->state();}
        else{ cellList[x][y+1]->state();
            cellList[x][y-1]->state();}
    }
    return cellList;

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

void mouseClick(GLFWwindow* Window, Cell*** cellList)
{
    double xcursor, ycursor;
    glfwGetCursorPos(Window, &xcursor, &ycursor);
    printf("A: %f %f ", xcursor, ycursor);
    xcursor = xcursor * cellDim / DIM;
    ycursor = cellDim - ycursor * cellDim / DIM;
    printf("B: %f %f ", xcursor, ycursor);
    toggleLights(cellList, (int) xcursor, (int) ycursor);
}

void constructVO(GLuint& VAO, GLuint* VBO, GLfloat* vertexArray, GLfloat* colorArray)
{
    glBindVertexArray(VAO);
    glGenBuffers(2, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, cellDim * cellDim * 12 * sizeof(GLfloat), vertexArray, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, cellDim * cellDim * 6 * sizeof(GLfloat), colorArray, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glBindVertexArray(0);

}

void constructLines(GLuint& VAO, GLuint* VBO)
{
    GLfloat vertexArray[cellDim * 8];
    GLfloat colorArray[cellDim * 4];
    float x = -1.0f;
    float y = -1.0f;

    for (int i = 0; cellDim * 4; i++)
    {
        vertexArray[i * 2] = x;
        vertexArray[i * 2 + 1] = y;
        x += 2/(float)cellDim;
        y += 2/(float)cellDim;
    }
    for (int i = 0; cellDim * 4; i++)
    {
        colorArray[i] = 0.5f;
    }
    glBindVertexArray(VAO);
    glGenBuffers(2, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, cellDim * 8 * sizeof(GLfloat), vertexArray, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, cellDim *  4 * sizeof(GLfloat), colorArray, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glBindVertexArray(0);

}

void createShaders()
{
    GLuint shaderProgram;
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar* vertexSource = "#version 330\n layout(location = 0) in vec3 Coords; layout(location = 1) in float iColor; out float eColor; void main(void){ gl_Position = vec4(Coords.x, Coords.y, 0.0, 1.0); eColor = iColor;}"; 
    const GLchar* fragmentSource = "#version 330\n in float eColor; out vec4 gl_FragColor; void main(void){ gl_FragColor = vec4(eColor, eColor, eColor, 1.0);}";

    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);


    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
}


GLfloat* createCellVertexArray(GLfloat* vertexArray, CellList cellList)
{
    GLfloat diff = 2/(float)cellDim;
    vertexArray = new GLfloat[cellDim*cellDim*12];

    for (int i = 0; i <= (cellDim - 1); i++)
    {
        for (int j = 0; j <= (cellDim - 1); j++)
        {
            GLfloat x = (GLfloat) cellList[i][j]->xcoord;
            GLfloat y = (GLfloat) cellList[i][j]->ycoord;
            int index = (i * cellDim + j) * 12;

            vertexArray[index + 0] = x        ; vertexArray[index + 1] = y        ; 
            vertexArray[index + 2] = x + diff ; vertexArray[index + 3] = y        ; 
            vertexArray[index + 4] = x        ; vertexArray[index + 5] = y + diff ; 

            vertexArray[index + 6] = x + diff ; vertexArray[index + 7] = y        ;
            vertexArray[index + 8] = x        ; vertexArray[index + 9] = y + diff ;
            vertexArray[index + 10] = x + diff; vertexArray[index + 11] = y + diff;
        }
            
    }
    return vertexArray;

}

GLfloat* constructCellColorArray(GLfloat* colorArray, CellList cellList, bool create)
{
    if(create){
        colorArray = new GLfloat[cellDim*cellDim*6]; create = true;}

    for (int i = 0; i <= (cellDim - 1); i++)
    {
        for (int j = 0; j <= (cellDim - 1); j++)
        {
            GLfloat light = cellList[i][j]->light;

            int index = (i * cellDim + j) * 6;

            colorArray[index] = light; colorArray[index + 1] = light;
            colorArray[index + 2] = light; colorArray[index + 3] = light;
            colorArray[index + 4] = light; colorArray[index + 5] = light;
            
        }

    }
    return colorArray;
}

void startGame()
{
    GLFWwindow* Window = initWindow();
    bool Clicked = false;
    GLuint VAO[2];
    GLuint VBO[2];
    GLuint LinesVBO[2];
    glGenVertexArrays(2, VAO);
    
    CellList cellList = createCellList();
    GLfloat* cellVertexArray = createCellVertexArray(cellVertexArray, cellList);
    GLfloat* cellColorArray = constructCellColorArray(cellColorArray, cellList, true);
    constructVO(VAO[0], VBO, cellVertexArray, cellColorArray);
    //constructLines(VAO[1], LinesVBO);
    createShaders();
    glClearColor(1.0, 1.0, 1.0, 1.0);

    while (!glfwWindowShouldClose(Window)){
        glClear(GL_COLOR_BUFFER_BIT);

        constructCellColorArray(cellColorArray, cellList, false);
        constructVO(VAO[0], VBO, cellVertexArray, cellColorArray);

        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, cellDim * cellDim * 12);

        glBindVertexArray(VAO[1]);
        //glDrawArrays(GL_LINES, 0, cellDim * 8);

        glfwSwapBuffers(Window);
        glfwWaitEvents();
        if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) == 1 && !Clicked){
            Clicked = true;
            mouseClick(Window, cellList);
        }
        if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) == 0 && Clicked)
            Clicked = false;
    }
    glfwDestroyWindow(Window);
    glfwTerminate();
    

}

int main(int argc, char** argv)
{   
    if(argc == 2){
        cellDim = atoi(argv[1]);
        printf("%d",cellDim);
        if(cellDim <= 1){
            printf("Please input an integer greater than 1");
            return 0;
        }
    }else{
        printf("Please input an integer greater than 0 and less than 11 after 'LightsOut' in terminal");
        return 0;
    }

    startGame();

}