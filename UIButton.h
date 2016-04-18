
/********************************************************************
	
	filename: 	UIButton.h
	author:		Chris Tallman
	created:	08/12/11

	purpose:	

*********************************************************************/

#ifndef UIButton_h__
#define UIButton_h__

#include "UIRegion.h"
#include "Texture.h"
#include "LuaHelper.h"

#include <string>

class UIButton : public UIRegion
{
public:     //////////////////////////////////////////////////////////////////////////

    UIButton();
    virtual ~UIButton();

    virtual void    Draw(void) const;

    void            OnMouseDown();
    void            OnMouseEnter();
    void            OnMouseLeave();
    void            OnMouseUp();

    void            SetImage( const SharedTexture & image, UIRegion::eState state = UIRegion::STATE_DEFAULT);
    void            SetImage( const std::string & image, UIRegion::eState state = UIRegion::STATE_DEFAULT);

private:    //////////////////////////////////////////////////////////////////////////

    SharedTexture   _images[UIRegion::STATE_LAST];

    LuaFunction _onMouseDown;
    LuaFunction _onMouseEnter;
    LuaFunction _onMouseLeave;
    LuaFunction _onMouseUp;

    //////////////////////////////////////////////////////////////////////////////////

    LUNAR_NAME_NEWABLE_CLASS(UIButton);
    LUNAR_NAME_METHOD(SetImage);
    LUNAR_NAME_METHOD(SetOnMouseDown);
    LUNAR_NAME_METHOD(SetOnMouseEnter);
    LUNAR_NAME_METHOD(SetOnMouseLeave);
    LUNAR_NAME_METHOD(SetOnMouseUp);
};

#endif // UIButton_h__