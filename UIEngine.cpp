
#include "UIEngine.h"
#include "UIRegion.h"
#include "Globals.h"
#include "GraphicsEngine.h"
#include "InputManager.h"
#include "DevConsole.h"

UIEngine::UIEngine()
    : _root(0), _hittest(0)
{
    _size.set(1024,768);
}

void UIEngine::CheckHittestReset(UIRegion * region)
{
    if( _hittest == region )
        ResetHittest();
}

void UIEngine::OnLoad()
{
    _root = new UIRegion();
    _root->SetSize( _size[0], _size[1] );
}

void UIEngine::OnUnload()
{
    delete _root;
}

UIRegion * UIEngine::GetRegionAtPoint( float x, float y ) const
{
    return _root->Hittest(x,y);
}

UIRegion * UIEngine::GetRoot( void ) const
{
    return _root;
}

const Float2 & UIEngine::GetSize( void ) const
{
    return _size;
}

void UIEngine::Render( void ) const
{
    if( _root )
    {
        GraphicsEngine * gfx = Globals::GetGraphicsEngine();
        gfx->Enable2D();

        _root->Draw();

        gfx->Disable2D();
    }

    Globals::GetConsole()->Draw();
}

void UIEngine::Resize(uint w, uint h)
{
    if( w && h && _root )
    {
        _size.set((float)w, (float)h);
        _root->SetSize(_size[0], _size[1]);
    }
}

void UIEngine::Update(float dt)
{
    if( _root )
    {
        _root->Update(dt);

        UpdateStates();
    }
}

//////////////////////////////////////////////////////////////////////////

void UIEngine::ResetHittest()
{
    if( _hittest )
    {
        _hittest->MouseLeave();
        _hittest->SetState(UIRegion::STATE_DEFAULT);
        _hittest = 0;
    }
}

void UIEngine::UpdateStates( void )
{
    InputManager * input = Globals::GetInputManager();

    const Int2 & pos = input->MousePosition();
    float x = (float)pos[0];
    float y = (float)pos[1];

    UIRegion * region = GetRegionAtPoint(x,y);

    if( region != _hittest )
    {
        
        ResetHittest();

        if( region )
        {
            region->MouseEnter();
            region->SetState(UIRegion::STATE_HOVER);
        }

        _hittest = region;
    }

    if( _hittest )
    {
        if( input->IsTriggered(InputMap::MOUSE_LEFT) )
        {
            _hittest->MouseDown();
            _hittest->SetState(UIRegion::STATE_CLICK);
        }
        else if( input->IsReleased( InputMap::MOUSE_LEFT ) )
        {
            _hittest->MouseUp();
            _hittest->SetState(UIRegion::STATE_HOVER);
        }
    }

}

//////////////////////////////////////////////////////////////////////////


LUNAR_START_REGISTER(UIEngine)
    LUNAR_REGISTER_METHOD(UIEngine, GetRoot)
LUNAR_END_REGISTER

LUNAR_METHOD(UIEngine, GetRoot)
{
    if( _root )
        _root->PushToLua(ls);
    else
        lua_pushnil(ls);
    
    return 1;
}

