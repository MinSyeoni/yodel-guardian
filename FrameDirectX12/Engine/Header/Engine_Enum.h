#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum BUFFERID		{	BUFFER_RCCOL, BUFFER_RCTEX, BUFFER_TRICOL, BUFFER_TERRAINTEX, BUFFER_CUBE, BUFFER_END	};

	enum TEXTURETYPE	{ TEX_NORMAL, TEX_CUBE, TEX_END };

	enum LAYERID		{ LAYER_ENVIRONMENT, LAYER_GAMEOBJECT, LAYER_UI };

	/*____________________________________________________________________
	ID_DYNAMIC : 매 프레임마다 업데이트가 되어야 할 컴포넌트들의 집합
	ID_STATIC  : 매 프레임마다 업데이트가 필요 없는 컴포넌트들의 집합
	______________________________________________________________________*/
	enum COMPONENTID	{ ID_DYNAMIC, ID_STATIC, ID_END };

	// enum INFO			{ IFNO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	// enum ROTATION		{ ROT_X, ROT_Y, ROT_Z, ROT_END };
	// enum RENDERID		{ RENDER_PRIORITY, RENDER_NONEALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

	enum MESHTYPE		{ TYPE_STATIC, TYPE_DYNAMIC, TYPE_NAVI };

	// enum MOUSEKEYSTATE  { DIM_LB, DIM_RB, DIM_WB, DIM_END };
	// enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };
	enum MOUSEBUTTON	{ DIM_LB, DIM_RB, DIM_WB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };
	
	enum SCENEID		
	{ 
		SCENE_STATIC,
		SCENE_LOGO, 
		SCENE_MENU,
		SCENE_STAGE, 
		SCENE_BOSS, 
		SCENE_END 
	};
}



#endif // Engine_Enum_h__
