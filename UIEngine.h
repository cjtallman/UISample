
/********************************************************************
	
	filename: 	UIEngine.h
	author:		Chris Tallman
	created:	08/12/11

	purpose:	

*********************************************************************/

#ifndef UIEngine_h__
#define UIEngine_h__

#include "DataTypes.h"
#include "BaseEngine.h"
#include "Scripting.h"

class UIRegion;

class UIEngine : public BaseEngine
{
public:     //////////////////////////////////////////////////////////////////////////

    UIEngine();

    void                CheckHittestReset(UIRegion * region);
    UIRegion *          GetRegionAtPoint( float x, float y ) const;
    UIRegion *          GetRoot(void) const;
    const Float2 &      GetSize(void) const;
    void                OnLoad(void);
    void                OnUnload(void);
    void                Render(void) const;
    void                Resize(uint w, uint h);
    void                Update(float dt);

private:    //////////////////////////////////////////////////////////////////////////

    void                ResetHittest();
    void                UpdateStates(void);

    UIRegion *          _hittest;       // derivative
    UIRegion *          _root;
    Float2              _size;


    //////////////////////////////////////////////////////////////////////////////////

    LUNAR_NAME_CLASS(UIEngine);
    LUNAR_NAME_METHOD(GetRoot);

};

#endif // UIEngine_h__