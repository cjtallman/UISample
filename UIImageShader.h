
/********************************************************************
	
	filename: 	UIImageShader.h
	author:		Chris Tallman
	created:	08/10/11

	purpose:	

*********************************************************************/

#ifndef UIImageShader_h__
#define UIImageShader_h__

#include "Shader.h"

class UIImageShader : public ShaderEffect
{
public:
    enum eSubShaders { BASIC, UIIMAGE };

public:
    UIImageShader(void);

    void Bind(int pass = 0);
    void SetData(int pass = 0);
    void Unbind(int pass = 0);

private:
    void Load(void);
    
    enum eShaderIndices
    {
        VERT,
        FRAG,

        SHADER_COUNT
    };

    enum eParamIndices
    {
        MVP,

        PARAM_COUNT
    };
};

#endif // UIImageShader_h__