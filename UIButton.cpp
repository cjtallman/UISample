
/********************************************************************
	
	filename: 	UIButton.cpp
	author:		Chris Tallman
	created:	08/12/11

	purpose:	

*********************************************************************/

#include "UIButton.h"
#include "UIImageShader.h"
#include "Globals.h"
#include "DevConsole.h"
#include "ResourceManager.h"

UIButton::UIButton( void )
{
    SetShader( new UIImageShader() );
}

UIButton::~UIButton( void )
{

}

void UIButton::Draw( void ) const
{
    PushTransform();

    ResourceManager * res = Globals::GetResourceManager();

    BindShaders();

    SharedTexture image = _images[GetState()];
    if( !image )
        image = _images[UIRegion::STATE_DEFAULT];

    res->BindTexture(image);

    DrawQuad();

    res->UnbindTexture(image);

    UnbindShaders();

    DrawChildren();

    PopTransform();
}

void UIButton::OnMouseDown()
{
    _onMouseDown.Call();
}

void UIButton::OnMouseEnter()
{
    _onMouseEnter.Call();
}

void UIButton::OnMouseLeave()
{
    _onMouseLeave.Call();
}

void UIButton::OnMouseUp()
{
    _onMouseUp.Call();
}

void UIButton::SetImage(const std::string & image, UIRegion::eState state /* = UIRegion::STATE_DEFAULT */)
{
    ResourceManager * res = Globals::GetResourceManager();
    _images[state] = res->GetTexture(image);
}

void UIButton::SetImage(const SharedTexture & image, UIRegion::eState state /* = UIRegion::STATE_DEFAULT */)
{
    _images[state] = image;
}

//////////////////////////////////////////////////////////////////////////

LUNAR_START_REGISTER(UIButton)
    LUNAR_REGISTER_METHOD(UIRegion, IsEnabled)
    LUNAR_REGISTER_METHOD(UIRegion, SetHeight)
    LUNAR_REGISTER_METHOD(UIRegion, SetOriginOffset)
    LUNAR_REGISTER_METHOD(UIRegion, SetParent)
    LUNAR_REGISTER_METHOD(UIRegion, SetPosition)
    LUNAR_REGISTER_METHOD(UIRegion, SetSize)
    LUNAR_REGISTER_METHOD(UIRegion, SetWidth)

    LUNAR_REGISTER_METHOD(UIButton, SetImage)
    LUNAR_REGISTER_METHOD(UIButton, SetOnMouseDown)
    LUNAR_REGISTER_METHOD(UIButton, SetOnMouseEnter)
    LUNAR_REGISTER_METHOD(UIButton, SetOnMouseLeave)
    LUNAR_REGISTER_METHOD(UIButton, SetOnMouseUp)
LUNAR_END_REGISTER

LUNAR_METHOD(UIButton, SetImage)
{
    const char * image;
    int state = 0;

    switch( lua_gettop(ls) )
    {
    case 2:
        state = lua_tointeger(ls, 2);
    case 1:
        image = luaL_checkstring(ls, 1);
        SetImage( image, UIRegion::eState(state) );
        break;
    }
    
    return 0;
}

LUNAR_METHOD(UIButton, SetOnMouseDown)
{
    _onMouseDown.SetFunction(ls);

    return 0;
}

LUNAR_METHOD(UIButton, SetOnMouseEnter)
{
    _onMouseEnter.SetFunction(ls);

    return 0;
}

LUNAR_METHOD(UIButton, SetOnMouseLeave)
{
    _onMouseLeave.SetFunction(ls);

    return 0;
}

LUNAR_METHOD(UIButton, SetOnMouseUp)
{
    _onMouseUp.SetFunction(ls);

    return 0;
}