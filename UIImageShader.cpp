
/********************************************************************
	
	filename: 	UIImageShader.cpp
	author:		Chris Tallman
	created:	08/10/11

	purpose:	

*********************************************************************/

#include "UIImageShader.h"

#include "Globals.h"
#include "ResourceManager.h"
#include "MatrixPipeline.h"

UIImageShader::UIImageShader(void)
{
    _shaders.resize(SHADER_COUNT);
    _params.resize(PARAM_COUNT);
    
    Load();
}

void UIImageShader::Bind(int pass /* = 0 */)
{
    ResourceManager * res = Globals::GetResourceManager();

    res->BindShader(_shaders[VERT]);
    res->BindShader(_shaders[FRAG]);
}

void UIImageShader::SetData(int pass /* = 0 */)
{
    if( _params[MVP] )
        _params[MVP]->SetMatrix( Matrix::ModelViewProj() );
}

void UIImageShader::Unbind(int pass /* = 0 */)
{
    ResourceManager * res = Globals::GetResourceManager();

    res->UnbindShader(_shaders[VERT]);
    res->UnbindShader(_shaders[FRAG]);
}

void UIImageShader::Load(void)
{
    ResourceManager * res = Globals::GetResourceManager();

    _shaders[VERT] = res->GetVertShader("uiimage.vert");
    _shaders[FRAG] = res->GetFragShader("uiimage.frag");

    if( _shaders[VERT] )
    {
        _params[MVP] = _shaders[VERT]->GetParameter("ModelViewProj");
    }
    
}