
/********************************************************************
	
	filename: 	UIImage.h
	author:		Chris Tallman
	created:	08/10/11

	purpose:	

*********************************************************************/

#ifndef UIImage_h__
#define UIImage_h__

#include "UIRegion.h"
#include "Texture.h"

class UIImage : public UIRegion
{
public:     //////////////////////////////////////////////////////////////////////////

    UIImage(void);
    virtual ~UIImage(void);

    virtual void    Draw(void) const;
    Float2          GetImageSize(void) const;
    void            SetImage( const SharedTexture & image );
    void            SetImage( const std::string & image );

private:    //////////////////////////////////////////////////////////////////////////

    SharedTexture   _image;

    //////////////////////////////////////////////////////////////////////////////////

    LUNAR_NAME_NEWABLE_CLASS(UIImage);
    LUNAR_NAME_METHOD(SetImage);
};

#endif // UIImage_h__