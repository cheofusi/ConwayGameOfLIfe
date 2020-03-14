#ifndef _SHADER_H_INCLUDED_
#define _SHADER_H_INCLUDED_

class Shader{
    private:
        std::string readFile(const char*) const; //so all shader programs don't copy this function as all their initialization, linking and attaching of shaders
                                                 // occurs in the constructor

    public:
        // the shader program ID
        GLuint Program;
    
        // constructor reads and builds the shader
        Shader(const char* vertex_path, const char* fragment_path);
        // use/activate the shader
        void use();
        // utility uniform functions
        void setFloat(const std::string &name, float value) const; // overload this for setting multiple float attributes
        void setFloat(const std::string &name, float value0, float value1) const;
        void setFloat(const std::string &name, float value0, float value1, float value2) const;
        void setFloat(const std::string &name, float value0, float value1, float value2, float value3) const;
};
  
#endif