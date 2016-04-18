
/********************************************************************
	
	filename: 	UIImage.cpp
	author:		Chris Tallman
	created:	08/10/11

	purpose:	

*********************************************************************/

#include "UIImage.h"
#include "Globals.h"
#include "ResourceManager.h"
#include "MatrixPipeline.h"
#include "UIImageShader.h"

UIImage::UIImage(void)
{
    SetShader( new UIImageShader() );
}

UIImage::~UIImage(void)
{

}

void UIImage::Draw(void) const
{
    PushTransform();

    ResourceManager * res = Globals::GetResourceManager();

    BindShaders();

    res->BindTexture(_image, 0);

    DrawQuad();

    res->UnbindTexture(_image, 0);

    UnbindShaders();

    DrawChildren();

    PopTransform();
}

Float2 UIImage::GetImageSize(void) const
{
    if( _image )
        return Float2( (float)_image->GetWidth(), (float)_image->GetHeight() );
    else
        return Float2(0,0);
}

void UIImage::SetImage( const SharedTexture & image )
{
    _image = image;
}

void UIImage::SetImage( const std::string & image )
{
    ResourceManager * res = Globals::GetResourceManager();
    _image = res->GetTexture(image);
}

//////////////////////////////////////////////////////////////////////////

LUNAR_START_REGISTER(UIImage)
    LUNAR_REGISTER_METHOD(UIRegion, SetHeight)
    LUNAR_REGISTER_METHOD(UIRegion, SetOriginOffset)
    LUNAR_REGISTER_METHOD(UIRegion, SetParent)
    LUNAR_REGISTER_METHOD(UIRegion, SetPosition)
    LUNAR_REGISTER_METHOD(UIRegion, SetSize)
    LUNAR_REGISTER_METHOD(UIRegion, SetWidth)

    LUNAR_REGISTER_METHOD(UIImage, SetImage)
LUNAR_END_REGISTER

LUNAR_METHOD(UIImage, SetImage)
{
    const char * image = luaL_checkstring(ls, 1);

    if( image )
        SetImage(image);

    return 0;
}

