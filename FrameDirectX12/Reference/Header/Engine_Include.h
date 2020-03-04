#ifndef Engine_Include_h__
#define Engine_Include_h__

#include "vld.h"
// #include <Windows.h>
#include <iostream>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <functional>
#include <ctime>
#include <fstream>
#include <io.h>
#include <tchar.h>
#include <comdef.h>
#include <memory>

using namespace std;

/*____________________________________________________________________
DirectX12
______________________________________________________________________*/
#include <wrl.h>
#include <shellapi.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <d3d12shader.h>
#include <dxgi1_4.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

/*____________________________________________________________________
DirectX Master SDK.
______________________________________________________________________*/
#include <d3dx12.h>
#include "DDSTextureLoader.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "Engine_Const.h"
#include "Engine_Enum.h"
#include "Engine_Function.h"
#include "Engine_Functor.h"
#include "Engine_Macro.h"
#include "Engine_Typedef.h"
#include "Engine_Struct.h"
#include "Engine_Template.h"
#include "Engine_Geometry.h"

#pragma warning(disable : 4251)

/*____________________________________________________________________
DirectInput
______________________________________________________________________*/
#define DIRECTINPUT_VERSION		0x0800
#include <dinput.h>

/*____________________________________________________________________
DirectSound
______________________________________________________________________*/
#include <mmsystem.h>
#include <dsound.h>
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")


/*_______//ÆùÆ® ¿ë 
___________________________________________________________________________________________________
[ DirectX 11 ]
____________________________________________________________________________________________________________*/
#include <d3d11on12.h>
#include <d2d1_3.h>
#include <dwrite_2.h>



#endif // Engine_Include_h__
