
/********************************************************************
	
	filename: 	UIRegion.h
	author:		Chris Tallman
	created:	08/10/11

	purpose:	

*********************************************************************/

#ifndef BaseUIElement_h__
#define BaseUIElement_h__

#include "DataTypes.h"
#include "Shader.h"
#include "Scripting.h"

#include <vector>

class UIRegion
{
public:     //////////////////////////////////////////////////////////////////////////

    enum eHorizontalAlign { HALIGN_LEFT, HALIGN_CENTER, HALIGN_RIGHT };
    enum eVerticalAlign { VALIGN_TOP, VALIGN_CENTER, VALIGN_BOTTOM };
    enum ePositionMode { MODE_POINT, MODE_PERCENT };
    enum eEvents { EVT_MOUSEENTER, EVT_MOUSELEAVE, EVT_MOUSEDOWN, EVT_MOUSEUP };
    enum eState { STATE_DEFAULT, STATE_HOVER, STATE_CLICK, STATE_LAST };

public:     //////////////////////////////////////////////////////////////////////////

    UIRegion(void);
    virtual ~UIRegion(void);

    /////////////////////////////////////////////////////////////////////////////////
    
    virtual void                    Draw(void) const;
    const Float2 &                  GetHalfSize(void) const;
    const Float4 &                  GetMargins(void) const;
    const Float2 &                  GetOriginOffset(void) const;
    const Float4 &                  GetPadding(void) const;
    UIRegion *                      GetParent(void) const;
    const Float2 &                  GetPosition(void) const;
    const Float2 &                  GetPositionGlobal(void) const;
    const Float4 &                  GetRegion(bool local = true) const;
    const Float4 &                  GetRegionGlobal(void) const;
    const Float4 &                  GetRegionLocal(void) const;
    ShaderEffect *                  GetShader(void) const;
    const Float2 &                  GetSize(void) const;
    eState                          GetState(void) const;
    UIRegion *                      Hittest( float x, float y ) const;
    bool                            IsEnabled(void) const;
    bool                            IsPointInside( float x, float y ) const;
    bool                            IsVisible(void) const;
    void                            SetClipChildren(bool clip);
    void                            SetEnabled(bool enabled);
    void                            SetMargins(float ltrb);
    void                            SetMargins(float lr, float tb);
    void                            SetMargins(float l, float t, float r, float b);
    void                            SetOriginOffset( float u, float v );
    void                            SetOriginOffset( const Float2 & offset );
    void                            SetPadding(float ltrb);
    void                            SetPadding(float lr, float tb);
    void                            SetPadding(float l, float t, float r, float b);
    void                            SetParent(UIRegion * parent);
    void                            SetPosition(float x, float y);
    void                            SetPositionMode(ePositionMode mode);
    virtual void                    SetShader( ShaderEffect * shader );
    void                            SetSize(const Float2 & size);
    void                            SetSize(float w, float h);
    void                            SetVisible(bool visible);
    bool                            ShouldClipChildren(void) const;
    virtual void                    Update(float dt);
    
protected:  //////////////////////////////////////////////////////////////////////////
    
    virtual void                    AddElement( UIRegion * element );
    virtual void                    BindShaders(int pass = 0) const;
    virtual void                    DrawChildren(void) const;
    virtual void                    DrawQuad() const;
    void                            EnableClippingRegion(bool enable) const;
    virtual void                    OnMouseDown(void);
    virtual void                    OnMouseEnter(void);
    virtual void                    OnMouseLeave(void);
    virtual void                    OnMouseUp(void);
    void                            PushViewport(void) const;
    virtual void                    PushTransform(void) const;
    virtual void                    PopTransform(void) const;
    void                            PopViewport(void) const;
    void                            SetState(eState state);
    virtual void                    UnbindShaders(int pass = 0) const;
    virtual void                    UpdateChildren(float dt);
    void                            UpdateRegion(void);

    std::vector<UIRegion*>          _children;

private:    //////////////////////////////////////////////////////////////////////////
    
    friend class UIEngine;

    void                            MouseDown(void);
    void                            MouseEnter(void);
    void                            MouseLeave(void);
    void                            MouseUp(void);

    void                            RemoveElement( const UIRegion * element );
    
    Float2                          _halfSize;
    bool                            _isEnabled;
    bool                            _isVisible;
    Float4                          _margins;
    Float2                          _originOffset;
    Float4                          _padding;
    UIRegion *                      _parent;
    Float2                          _position;
    Float2                          _positionGlobal;    // derivative
    ePositionMode                   _positionMode;
    Float4                          _region;            // derivative
    Float4                          _regionGlobal;      // derivative
    ShaderEffect *                  _shader;
    bool                            _shouldClipChildren;
    Float2                          _size;
    eState                          _state;

    LUNAR_NAME_NEWABLE_CLASS(UIRegion);
    LUNAR_NAME_METHOD(IsEnabled);
    LUNAR_NAME_METHOD(SetHeight);
    LUNAR_NAME_METHOD(SetOriginOffset);
    LUNAR_NAME_METHOD(SetParent);
    LUNAR_NAME_METHOD(SetPosition);
    LUNAR_NAME_METHOD(SetSize);
    LUNAR_NAME_METHOD(SetWidth);
};

#endif // BaseUIElement_h__