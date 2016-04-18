
/********************************************************************
	
	filename: 	UIRegion.cpp
	author:		Chris Tallman
	created:	08/10/11

	purpose:	

*********************************************************************/

#include "UIRegion.h"
#include "BasicUIShader.h"
#include "MatrixPipeline.h"
#include "Globals.h"
#include "GraphicsEngine.h"
#include "UIEngine.h"

//////////////////////////////////////////////////////////////////////////

UIRegion::UIRegion() : _parent(0), _shader(0), _originOffset(0,0), _position(0,0), _isEnabled(true), _isVisible(true), _state(STATE_DEFAULT)
{
    SetMargins(0);
    SetPadding(0);
    SetPositionMode(MODE_POINT);
    SetSize(0, 0);
    SetShader( new BasicUIShader() );

    SetClipChildren(true);
}

UIRegion::~UIRegion()
{
    SAFE_DELETE(_shader);

    SetParent(0);

    uint sz = _children.size();
    for( uint i = 0; i < sz; ++i )
        SAFE_DELETE(_children[i]);

    _children.clear();

    Globals::GetUI()->CheckHittestReset(this);
}

//////////////////////////////////////////////////////////////////////////

void UIRegion::Draw(void) const
{
    PushTransform();
    

    BindShaders();
    DrawQuad();
    UnbindShaders();

    DrawChildren();

    PopTransform();
}

const Float2 & UIRegion::GetHalfSize( void ) const
{
    return _halfSize;
}

const Float4 & UIRegion::GetMargins( void ) const
{
    return _margins;
}

const Float2 & UIRegion::GetOriginOffset( void ) const
{
    return _originOffset;
}

const Float4 & UIRegion::GetPadding( void ) const
{
    return _padding;
}

UIRegion * UIRegion::GetParent( void ) const
{
    return _parent;
}

const Float2 & UIRegion::GetPosition( void ) const
{
    // with respect to top-left of parent
    return _position;
}

const Float2 & UIRegion::GetPositionGlobal( void ) const
{
    // with respect to top-left of screen
    return _positionGlobal;
}

const Float4 & UIRegion::GetRegion( bool local /* = true */ ) const
{
    return (local) ? GetRegionLocal() : GetRegionGlobal();
}

const Float4 & UIRegion::GetRegionGlobal( void ) const
{
    // with respect to top-left of screen
    return _regionGlobal;
}

const Float4 & UIRegion::GetRegionLocal( void ) const
{
    // with respect to top-left of parent region
    return _region;
}

ShaderEffect * UIRegion::GetShader( void ) const
{
    return _shader;
}

const Float2 & UIRegion::GetSize( void ) const
{
    return _size;
}

UIRegion::eState UIRegion::GetState( void ) const
{
    return _state;
}

UIRegion * UIRegion::Hittest( float x, float y ) const
{
    UIRegion * region = 0;

    if( ShouldClipChildren() )
    {
        if( IsPointInside(x,y) )
        {
            region = const_cast<UIRegion*>(this);

            for( uint i = 0; i < _children.size(); ++i )
            {
                UIRegion * test = _children[i]->Hittest(x,y);
                if( test )
                    region = test;
            }

        }
    }
    else
    {
        if( IsPointInside(x,y) )
        {
            region = const_cast<UIRegion*>(this);
        }

        for( uint i = 0; i < _children.size(); ++i )
        {
            UIRegion * test = _children[i]->Hittest(x,y);
            if( test )
                region = test;
        }
    }

    return region;
}

bool UIRegion::IsEnabled( void ) const
{
    return _isEnabled;
}

bool UIRegion::IsPointInside( float x, float y ) const
{
    const Float4 & region = GetRegionGlobal();
    if( x < region[0] || x > region[2] || y < region[1] || y > region[3] )
        return false;
    else
        return true;
}

bool UIRegion::IsVisible( void ) const
{
    return _isVisible;
}

void UIRegion::SetClipChildren(bool clip)
{
    _shouldClipChildren = clip;
}

void UIRegion::SetEnabled(bool enabled)
{
    _isEnabled = enabled;
}

void UIRegion::SetMargins( float ltrb )
{
    _margins.set(ltrb, ltrb, ltrb, ltrb);
}

void UIRegion::SetMargins( float lr, float tb )
{
    _margins.set(lr, tb, lr, tb);
}

void UIRegion::SetMargins( float l, float t, float r, float b )
{
    _margins.set(l,t,r,b);
}

void UIRegion::SetOriginOffset( float u, float v )
{
    _originOffset.set(u,v);
}

void UIRegion::SetOriginOffset( const Float2 & offset )
{
    _originOffset = offset;
}

void UIRegion::SetPadding( float ltrb )
{
    _padding.set(ltrb, ltrb, ltrb, ltrb);
}

void UIRegion::SetPadding( float lr, float tb )
{
    _padding.set(lr, tb, lr, tb);
}

void UIRegion::SetPadding( float l, float t, float r, float b )
{
    _padding.set(l,t,r,b);
}

void UIRegion::SetParent( UIRegion * parent )
{
    // remove from old parent
    if( _parent )
        _parent->RemoveElement(this);

    // add to new parent
    if( parent )
        parent->AddElement(this);

    _parent = parent;
}

void UIRegion::SetPosition( float x, float y )
{
    _position.set(x,y);
}

void UIRegion::SetPositionMode(UIRegion::ePositionMode mode)
{
    _positionMode = mode;
}

void UIRegion::SetShader( ShaderEffect * shader )
{
    SAFE_DELETE(_shader);
    _shader = shader;
}

void UIRegion::SetSize( const Float2 & size )
{
    _size = size;
    _halfSize = _size * 0.5f;
}

void UIRegion::SetSize( float w, float h )
{
    _size.set(w,h);
    _halfSize = _size * 0.5f;
}

void UIRegion::SetVisible(bool visible)
{
    _isVisible = visible;
}

bool UIRegion::ShouldClipChildren() const
{
    return _shouldClipChildren;
}

void UIRegion::Update(float dt)
{
    _positionGlobal = _position;
    if( _parent )
    {
        const Float4 & r = _parent->GetRegionGlobal();
        _positionGlobal += Float2(r[0], r[1]);
    }

    UpdateRegion();

    // Update children
    UpdateChildren(dt);
}

//////////////////////////////////////////////////////////////////////////

void UIRegion::AddElement( UIRegion * element )
{
    _children.push_back(element);
}

void UIRegion::BindShaders(int pass /* = 0 */) const
{
    if( _shader )
    {
        _shader->Bind(pass);
        _shader->SetData(pass);
    }
}

void UIRegion::DrawChildren( void ) const
{
    if( ShouldClipChildren() )
    {
        EnableClippingRegion(true);
    }

    uint sz = _children.size();
    for( uint i = 0; i < sz; ++i )
    {
        const UIRegion * child = _children[i];
        if( child )
            child->Draw();
    }

    if( ShouldClipChildren() )
    {
        EnableClippingRegion(false);
    }
}

void UIRegion::DrawQuad() const
{
    glBegin(GL_QUADS);

    glTexCoord2f(0,1);
    glVertex2f(0,0);

    glTexCoord2f(0,0);
    glVertex2f(0,_size[1]);

    glTexCoord2f(1,0);
    glVertex2f(_size[0],_size[1]);

    glTexCoord2f(1,1);
    glVertex2f(_size[0],0);

    glEnd();
}

void UIRegion::EnableClippingRegion(bool enable) const
{
    if( enable )
    {
        Matrix::Push( Matrix::PROJECTION );
        Viewport::Push();

        Float4x4 P;
        const Float4 & regionG = GetRegionGlobal();

        cml::matrix_orthographic_RH<float>(P, regionG[0], regionG[2], regionG[3], regionG[1], 0.1f, 100000.0f, cml::z_clip_zero);
        Matrix::LoadMatrix( P, Matrix::PROJECTION );

        Viewport::Set(regionG);
    }
    else
    {
        Matrix::Pop( Matrix::PROJECTION );
        Viewport::Pop();
    }
}

void UIRegion::OnMouseDown( void )
{
    // TODO:
}

void UIRegion::OnMouseEnter( void )
{
    // TODO:
}

void UIRegion::OnMouseLeave( void )
{
    // TODO:
}

void UIRegion::OnMouseUp( void )
{
    // TODO:
}

void UIRegion::PopTransform() const
{
    Matrix::Pop( Matrix::MODEL );
    
}

void UIRegion::PushTransform() const
{
    Matrix::Push( Matrix::MODEL );

    const Float4 & region = GetRegionLocal();

    Float4x4 T;
    cml::matrix_translation(T, Float2(region[0], region[1]) );
    Matrix::MultMatrix( T, Matrix::MODEL );
}

void UIRegion::SetState( eState state )
{
    _state = state;
}

void UIRegion::UnbindShaders(int pass /* = 0 */) const
{
    if( _shader )
    {
        _shader->Unbind(pass);
    }
}

void UIRegion::UpdateChildren(float dt)
{
    uint sz = _children.size();
    for( uint i = 0; i < sz; ++i )
    {
        UIRegion * child = _children[i];
        if( child )
        {
            child->Update(dt);
        }
    }
}

void UIRegion::UpdateRegion( void )
{
    const Float2 & pos = GetPosition();
    const Float2 & posG = GetPositionGlobal();
    const Float2 & offset = GetOriginOffset();
    const Float2 & size = GetSize();

    _region[0] = pos[0] - offset[0] * size[0];
    _region[1] = pos[1] - offset[1] * size[1];
    _region[2] = pos[0] + (1 - offset[0]) * size[0];
    _region[3] = pos[1] + (1 - offset[1]) * size[1];    

    _regionGlobal[0] = posG[0] - offset[0] * size[0];
    _regionGlobal[1] = posG[1] - offset[1] * size[1];
    _regionGlobal[2] = posG[0] + (1 - offset[0]) * size[0];
    _regionGlobal[3] = posG[1] + (1 - offset[1]) * size[1]; 
}

//////////////////////////////////////////////////////////////////////////

void UIRegion::MouseDown( void )
{
    if( IsEnabled() && IsVisible() )
        OnMouseDown();
}

void UIRegion::MouseEnter(void)
{
    if( IsEnabled() && IsVisible() )
        OnMouseEnter();
}

void UIRegion::MouseLeave(void)
{
    if( IsEnabled() && IsVisible() )
        OnMouseLeave();
}

void UIRegion::MouseUp( void )
{
    if( IsEnabled() && IsVisible() )
        OnMouseUp();
}

void UIRegion::RemoveElement( const UIRegion * element )
{
    typedef std::vector<UIRegion*>::iterator UI_Itr;

    UI_Itr itr = _children.begin();
    UI_Itr end = _children.end();

    while( itr != end )
    {
        if( *itr == element )
        {
            _children.erase(itr);
            break;
        }

        ++itr;
    }
}

//////////////////////////////////////////////////////////////////////////

LUNAR_START_REGISTER(UIRegion)
    LUNAR_REGISTER_METHOD(UIRegion, IsEnabled)
    LUNAR_REGISTER_METHOD(UIRegion, SetHeight)
    LUNAR_REGISTER_METHOD(UIRegion, SetOriginOffset)
    LUNAR_REGISTER_METHOD(UIRegion, SetParent)
    LUNAR_REGISTER_METHOD(UIRegion, SetPosition)
    LUNAR_REGISTER_METHOD(UIRegion, SetSize)
    LUNAR_REGISTER_METHOD(UIRegion, SetWidth)
LUNAR_END_REGISTER

LUNAR_METHOD(UIRegion, IsEnabled)
{
    lua_pushboolean(ls, IsEnabled());
    return 1;
}

LUNAR_METHOD(UIRegion, SetHeight)
{
    float height = (float)luaL_checknumber(ls, 1);
    SetSize(_size[0], height);

    return 0;
}

LUNAR_METHOD(UIRegion, SetOriginOffset)
{
    float x, y;

    x = (float)luaL_checknumber(ls, 1);
    y = (float)luaL_checknumber(ls, 2);
    SetOriginOffset( x, y );

    return 0;
}

LUNAR_METHOD(UIRegion, SetParent)
{
    if( lua_isnil(ls, 1) )
    {
        lua_pop(ls, 1);
        SetParent(0);
    }
    else
    {
        UIRegion * parent = Lunar<UIRegion>::check(ls, 1);
        SetParent(parent);
    }

    return 0;
}

LUNAR_METHOD(UIRegion, SetPosition)
{
    float x, y;

    x = (float)luaL_checknumber(ls, 1);
    y = (float)luaL_checknumber(ls, 2);
    SetPosition(x,y);
    
    return 0;
}

LUNAR_METHOD(UIRegion, SetSize)
{
    float w, h;

    w = (float)luaL_checknumber(ls, 1);
    h = (float)luaL_checknumber(ls, 2);
    SetSize(w,h);

    return 0;
}

LUNAR_METHOD(UIRegion, SetWidth)
{
    float w;

    w = (float)luaL_checknumber(ls, 1);
    SetSize(w, _size[1]);

    return 0;
}