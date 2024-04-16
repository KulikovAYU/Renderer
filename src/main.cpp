//
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#include "platforms/OpenGl/opengl_debug.hpp"
//#include "core/Sample/core.hpp"
//#include "core/base.hpp"
//
//#include <imgui/imgui.h>
//#include <glm/glm.hpp>
////#include <imgui/backends/imgui_impl_opengl3.h>
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow *window);
//
//// settings
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;
//
//
//void prepareForMyFirstTriangle();
//void checkVertexShaderCompilationResult(GLuint vertexShaderId);
//GLuint createShaderProgram();
//
//int main()
//{
//    // glfw: initialize and configure
//    // ------------------------------
//    glfwInit();
//
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//#ifdef __APPLE__
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//
//    // glfw window creation
//    // --------------------
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    //ImGui_ImplOpenGL3_Init("#version 410");
//
//    ImGuiIO& io = ImGui::GetIO();
//
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//    // glad: load all OpenGL function pointers
//    // ---------------------------------------
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    GLuint shaderProgramId = createShaderProgram();
//
//    // Координаты вершин
//    std::vector<float> vertices = {
//        0.5f,  0.5f, 0.0f,  // top right
//        0.5f, -0.5f, 0.0f,  // bottom right
//       -0.5f, -0.5f, 0.0f,  // bottom left
//       -0.5f,  0.5f, 0.0f   // top left
//    };
//
//    // Массив индексов
//    std::vector<unsigned int> indices = {
//            0, 1, 3,  // first Triangle
//            1, 2, 3,  // second Triangle
//            0, 2, 3   //thirsd triangle
//    };
//
//    using namespace gl_render_application_sample;
//
//    vertex_array_object vertexArrayObject(
//            gl_render_application::CreateScope<vertex_buffer_object<float>>(vertices),
//            gl_render_application::CreateScope<element_buffer_object<unsigned int>>(indices));
//
//    vertexArrayObject.SetVertexAttribPointer(0, 3, 3);
//    vertexArrayObject.init(GL_STATIC_DRAW);
//
//    GLuint vaoId = vertexArrayObject.get_render_id();
//
//
//
//    // uncomment this call to draw in wireframe polygons.
//    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    // render loop
//    // -----------
//    while (!glfwWindowShouldClose(window)){
//        // input
//        // -----
//        processInput(window);
//
//        // render
//        // ------
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        GLCall(glUseProgram(shaderProgramId));
//
//        GLCall(glBindVertexArray(vaoId));
//        //glDrawArrays(GL_TRIANGLES, 0, 3);
//        //glPointSize(5.0f);
//        GLCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));
//
//        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//        // -------------------------------------------------------------------------------
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    //glDeleteVertexArrays(1, &vao);
//   //glDeleteBuffers(1, &VBO);
//   //glDeleteBuffers(1, &EBO);
//    glDeleteProgram(shaderProgramId);
//
//    // glfw: terminate, clearing all previously allocated GLFW resources.
//    // ------------------------------------------------------------------
//    glfwTerminate();
//    return 0;
//}
//
//
//
//// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//// ---------------------------------------------------------------------------------------------------------
//void processInput(GLFWwindow *window){
//    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//}
//
//// glfw: whenever the window size changed (by OS or user resize) this callback function executes
//// ---------------------------------------------------------------------------------------------
//void framebuffer_size_callback(GLFWwindow* window, int width, int height){
//    // make sure the viewport matches the new window dimensions; note that width and
//    // height will be significantly larger than specified on retina displays.
//    glViewport(0, 0, width, height);
//}
//
//GLuint compileVertexShader(){
//
//    const char* vertextShaderSource = "#version 330 core\n"
//                                      "layout (location = 0) in vec3 aPos;\n"
//                                      "void main()\n"
//                                      "{\n"
//                                      " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//                                      "}\0";
//
//    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertextShaderSource, NULL);
//    glCompileShader(vertexShader);
//
//    checkVertexShaderCompilationResult(vertexShader);
//
//    return vertexShader;
//}
//
//void checkVertexShaderCompilationResult(GLuint vertexShaderId){
//    int sucsess;
//    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &sucsess);
//
//    if(!sucsess){
//        char infoLog[512];
//        glGetShaderInfoLog(vertexShaderId,sizeof(infoLog)/sizeof(char), NULL, infoLog);
//        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
//        << infoLog << std::endl;
//    }
//}
//
//GLuint compileFragmentShader(){
//    const char* fragmentShaderSource = "#version 330 core \n"
//                                       "out vec4 FragColor; \n"
//                                       "void main()\n"
//                                       "{\n"
//                                       "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//                                       "}\n\0";
//
//    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShaderId, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShaderId);
//
//    return fragmentShaderId;
//}
//
//GLuint createShaderProgram(){
//    GLuint shaderProgramId = glCreateProgram();
//
//    GLuint vertexShaderId = compileVertexShader();
//    GLuint fragmentShaderId = compileFragmentShader();
//
//    glAttachShader(shaderProgramId, vertexShaderId);
//    glAttachShader(shaderProgramId, fragmentShaderId);
//    glLinkProgram(shaderProgramId);
//
//    int sucsess;
//    glGetProgramiv(shaderProgramId, GL_LINK_STATUS,&sucsess);
//    if(!sucsess){
//        char infoLog[512];
//        glGetProgramInfoLog(shaderProgramId, 512, NULL, infoLog);
//        std::cout << "ERROR::SHADER::LINKING_FAILED\n"
//                  << infoLog << std::endl;
//        goto free_shader;
//    }
//
//    free_shader:
//        glDeleteShader(vertexShaderId);
//        glDeleteShader(fragmentShaderId);
//
//
//    return shaderProgramId;
//}

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <SPIRV-Cross/spirv_glsl.hpp>

int main(){
  int n = 1;
  std::vector<uint32_t> spirv_binary;
  spirv_cross::CompilerGLSL glsl(std::move(spirv_binary));
}