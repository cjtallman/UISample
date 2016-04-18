
/********************************************************************
	
	filename: 	UIComponent.h
	author:		Chris Tallman
	created:	08/08/11

	purpose:	

*********************************************************************/

#ifndef UIComponent_h__
#define UIComponent_h__

#include "Component.h"
#include "Scripting.h"
#include "DataTypes.h"
#include "Texture.h"
#include "Interpolator.h"
#include "BasicUIShader.h"

class Entity;
class TransformComponent;

class UIComponent : public Component
{
public:
    enum eOrigin 
    {
        ORIGIN_CENTER,                                                          // Middle (default)
        ORIGIN_LEFT, ORIGIN_RIGHT, ORIGIN_TOP, ORIGIN_BOTTOM,                   // Sides
        ORIGIN_TOPLEFT, ORIGIN_TOPRIGHT, ORIGIN_BOTTOMLEFT, ORIGIN_BOTTOMRIGHT  // Corners
    };

    enum eState
    {
        STATE_DEFAULT,
        STATE_MOUSE_OVER,
        STATE_MOUSE_DOWN,
        STATE_DISABLED,

        STATE_LAST
    };

    static const int MaxLayers = 100;

public:
    //////////////////////////////////////////////////////////////////////////

    UIComponent(bool isTemplate = false);
    ~UIComponent(void);
    Component *             Clone(void) const;
    void                    Render(void);
    static eComponent       StaticType(void);
    void                    Update(float dt = 0.0f);

    //////////////////////////////////////////////////////////////////////////

    float                   GetAngle(void) const;
    float                   GetHeight(void) const;
    int                     GetLayer(void) const;
    const Float4 &          GetMargins(void) const;
    eOrigin                 GetOrigin(void) const;
    const UIComponent *     GetParent(void) const;
    const Float2 &          GetPosition(void) const;
    const Float4 &          GetRegion(void) const;
    eState                  GetState(void) const;
    float                   GetWidth(void) const;
    bool                    IsPointInside(float x, float y) const;
    void                    SetAngle(float degree, float time = 0.0f );
    void                    SetHeight(float h, float time = 0.0f );
    void                    SetImage(const SharedTexture & image, eState state = STATE_DEFAULT);
    void                    SetImage(const std::string & image, eState state = STATE_DEFAULT);
    void                    SetLayer(int layer);
    void                    SetMargins(float all);
    void                    SetMargins(float lr, float tb);
    void                    SetMargins(float l, float t, float r, float b);
    void                    SetOrigin(eOrigin origin);
    void                    SetParent(const UIComponent * parent);
    void                    SetPositionGlobal(float x, float y, float time = 0.0f );
    void                    SetPositionLocal(float x, float y, float time = 0.0f );
    void                    SetSizeGlobal(float w, float h, float time = 0.0f);
    void                    SetSizeLocal(float w, float h, float time = 0.0f);
    void                    SetState(eState state);
    void                    SetWidth(float w, float time = 0.0f );
    

private:

    Interpolator<float>         _angle;
    std::vector<SharedTexture>  _images;
    int                         _layer;
    Float4                      _margins;
    eOrigin                     _origin;
    const UIComponent *         _parent;
    BasicUIShader               _shader;
    eState                      _state;
    TransformComponent *        _transform;

    NOCOPY(UIComponent);

    LUNAR_NAME_CLASS(UIComponent);
    LUNAR_NAME_METHOD(GetAngle);
    LUNAR_NAME_METHOD(GetHeight);
    LUNAR_NAME_METHOD(GetWidth);
    LUNAR_NAME_METHOD(SetHeight);
    LUNAR_NAME_METHOD(SetWidth);
};

#endif // UIComponent_h__