#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include "Shaders/shader.h"
#include "Camera/camera.h"
#include "conwaysGame.h"
  
const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;

int GridWidth, GridHeight; // No of cells = GridWidth * GridHeight
float threshold; // probability that a cell is alive or dead on initialization


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void getGridVertices(std::vector<GLfloat>&, ConwayGame&); //computes grid vertices with colors based on game object grid
void updateCellColors(GLfloat*&, ConwayGame&);

void ParseArgs(int, char**&);



int main(int argc, char** argv){
    ParseArgs(argc, argv);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Conway Game of Life", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    
    glewExperimental = GL_TRUE; 
    glewInit();


    glEnable(GL_DEPTH_TEST);

    srand(time(nullptr));

    Shader shader("/home/brandon/Algorithms Playground/C++/Graphicx/ConwayGameOfLife/Shaders/vertShader.glsl", 
                    "/home/brandon/Algorithms Playground/C++/Graphicx/ConwayGameOfLife/Shaders/fragShader.glsl");
    Camera camera(window);
    ConwayGame conwayGame(GridWidth, GridHeight, threshold);



    std::vector<GLfloat> verticesVector;
    getGridVertices(verticesVector, conwayGame);
    verticesVector.shrink_to_fit();
    GLfloat vertices[verticesVector.size()];
    std::copy(verticesVector.begin(), verticesVector.end(), vertices);


    // playing buffers
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
    

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents(); 

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //then activate shader
        shader.use(); 

        // Create MVP transformations
        glm::mat4 model(1.0f);
        glm::mat4 view;
        glm::mat4 projection(1.0f);

        model = glm::rotate(model, -55.0f ,glm::vec3(1.0f, 0.0f, 0.0f));
        view = camera.getViewMatrix();
        projection = glm::perspective(camera.fov, (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

        // Get their uniform location
        GLint modelLoc = glGetUniformLocation(shader.Program, "model");
        GLint viewLoc = glGetUniformLocation(shader.Program, "view");
        GLint projLoc = glGetUniformLocation(shader.Program, "projection"); 
        // Pass them to the shaders
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection)); 

        conwayGame.updateCellStates();//updateCellStates();

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        GLfloat* ptr = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
        if(ptr){
            updateCellColors(ptr, conwayGame);
            glUnmapBuffer(GL_ARRAY_BUFFER);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(VAO); 
        GLuint IsgridLoc = glGetUniformLocation(shader.Program, "Isgrid");
        glUniform1i(IsgridLoc, 1);
        glDrawArrays(GL_LINES, 0, verticesVector.size());
        glUniform1i(IsgridLoc, 0);
        glDrawArrays(GL_TRIANGLES, 0, verticesVector.size());
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();

}

void updateCellColors(GLfloat*& ptr, ConwayGame& game){
    std::vector<GLfloat> newVertices;
    getGridVertices(newVertices, game); // recalculate vertices with new cellGrid values
    GLfloat vertices[newVertices.size()];
    std::move(newVertices.begin(), newVertices.end(), vertices);
    std::vector<GLfloat>().swap(newVertices);
    memcpy(ptr, vertices, sizeof(vertices));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // making sure the viewport matches the new window dimensions on any window resize by the user;
    glViewport(0, 0, width, height);
}

void getGridVertices(std::vector<GLfloat>& verticesVector, ConwayGame& game){
    int grid_X = GridWidth, grid_Y = GridHeight, grid_spacing = 1.0f;
    for(int j = 0; j < grid_Y * grid_spacing; j+= grid_spacing){
        for(int i = 0; i < grid_X * grid_spacing; i+= grid_spacing){ 
            verticesVector.push_back(j);
            verticesVector.push_back(i);
            verticesVector.push_back(0.0f);
            verticesVector.push_back((GLfloat)game.cellGrid[j][i]); //color

            verticesVector.push_back(j);
            verticesVector.push_back(i + grid_spacing);
            verticesVector.push_back(0.0f);
            verticesVector.push_back((GLfloat)game.cellGrid[j][i]); //color

            verticesVector.push_back(j + grid_spacing);
            verticesVector.push_back(i + grid_spacing);
            verticesVector.push_back(0.0f);
            verticesVector.push_back((GLfloat)game.cellGrid[j][i]); //color

            verticesVector.push_back(j + grid_spacing);
            verticesVector.push_back(i + grid_spacing);
            verticesVector.push_back(0.0f);
            verticesVector.push_back((GLfloat)game.cellGrid[j][i]); //color

            verticesVector.push_back(j + grid_spacing);
            verticesVector.push_back(i);
            verticesVector.push_back(0.0f);
            verticesVector.push_back((GLfloat)game.cellGrid[j][i]); //color

            verticesVector.push_back(j);
            verticesVector.push_back(i);
            verticesVector.push_back(0.0f);
            verticesVector.push_back((GLfloat)game.cellGrid[j][i]); //color

        }
    }
}   

static void show_usage(){
    std::cerr << "Usage: " << "Conway Game of Life" << " <option(s)>\n"
              << "Options:\n"
              << "\t-h, --help\t\tShow this help message\n"
              << "\t-gw, --gridwidth\tSpecifies the Width of the Grid\n"
              << "\t-gh, --gridheight\tSpecifies the Height of the Grid\n"
              << "\t-th, --threshold\tProbabilty of a cell being alive on first iteration"
              << std::endl;
}

void ParseArgs(int argc, char**& argv){
    if (argc < 4) {
        show_usage();
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help")) {
            show_usage();
            exit(EXIT_SUCCESS);
        } else if ((arg == "-gw") || (arg == "--gridwidth")) {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                GridWidth = std::stoi(argv[i+1]); // Increment 'i' so we don't get the argument as the next argv[i].
            } else {  // Uh-oh, there was no argument to the option.
                std::cerr << "--gridwidth option requires one argument." << std::endl;
                exit(EXIT_FAILURE);
            }  
        } else if ((arg == "-gh") || (arg == "--gridheight")) {
            if (i + 1 < argc) { 
                GridHeight = std::stoi(argv[++i]);
            } else { 
                std::cerr << "--gridheight option requires one argument." << std::endl;
                exit(EXIT_FAILURE);
            }
        } else if ((arg == "-th") || (arg == "--threshold")) {
            if (i + 1 < argc) { 
                threshold = std::stof(argv[++i]); 
            } else { 
                std::cerr << "--threshold option requires one argument." << std::endl;
                exit(EXIT_FAILURE);
            }
      
        }
    }
}