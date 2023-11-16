// Menu D3D9 C++
// Source code made by Minh Dat VIET NAM
// Don't Copyright

// ==================================================== Include and Library

#include "MinHook/MinHook.h"
#include "MinHook/kiero.h"
#include <iostream>
#include <Windows.h>
#include <assert.h>
#include <d3dx9.h>
#include <d3d9.h>

#define GWL_WNDPROC GWLP_WNDPROC
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

bool ShowMenu = false;
int a = 50;  // It calculates position X
int b = 80;  // It calculates position Y
int c = 250;  // It calculates weight
int d = 500;  // It calculates height
int e = 2;  // It calculates the border
int sizeOnOff_sizeText = 15;
int g = 20;

#pragma comment (lib, "MinHook/MinHookx64.lib")    // Library for 64 bit
//#pragma comment (lib, "MinHook/MinHookx86.lib")    // Library for 32 bit

// ==================================================== Include and Library

// ==================================================== Color
#define Maujkobt	   D3DCOLOR_ARGB(1,  153,  153,  153)
#define Black		   0xFF000000
#define White          0xFFFFFFFF
#define Green		   0xFF00FF00
#define Red	           0xFFFF1100
#define Blue		   0xFF00739f
#define Yellow         0xFFFFFF00
#define Violet         0xFFFF00EE
#define Grey           0xFF292929
#define MatteBlue      0x9008FFEB
#define MatteGray      0x90999999
// ==================================================== Color

// ==================================================== Menu

LPD3DXFONT font;

void CreateFontText(LPDIRECT3DDEVICE9 lmdD3D9)
{
    if (!font)
    {
        D3DXCreateFontA(lmdD3D9, 15, 0, FW_BOLD, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Tahoma", &font);
    }
}

void ResetFont()
{
    font->OnLostDevice();
    font->OnResetDevice();
}

void Text(int x, int y, const char* text, DWORD Mau)   // Function
{
    RECT rect, rect2;
    SetRect(&rect, x, y, x, y);
    SetRect(&rect2, x - 0.1, y + 0.2, x - 0.1, y + 0.);
    font->DrawTextA(NULL, text, -1, &rect2, DT_LEFT | DT_NOCLIP, 0x00000000);
    font->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, Mau);
}

void DrawBox(int x, int y, int w, int h, D3DCOLOR Mau, LPDIRECT3DDEVICE9 dD3D9)   // Function
{
    struct Vertex
    {
        float x, y, z, ht;
        DWORD Mau;
    }
    V[4] =
    {
                {x,y + h, 0.0f, 0.0f, Mau},
                {x,y, 0.0f, 0.0f, Mau},
                {x + w,y + h, 0.0f, 0.0f, Mau},
                {x + w,y, 0.0f, 0.0f, Mau}
    };

    dD3D9->SetTexture(0, NULL);
    dD3D9->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    dD3D9->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    dD3D9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    dD3D9->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, V, sizeof(Vertex));
    return;
}

void AddItem(const char* Name, int NumberPosition, LPDIRECT3DDEVICE9 dD3D9, bool on_off)
{
    int g;
    g = b + 15 + (NumberPosition * 20);

    if (on_off) 
    {
        Text(a + ((e * 3) * 4), g, Name, Green);
    }
    else 
    {
        Text(a + ((e * 3) * 4), g, Name, Red);
    }

    //Text(a + ((e * 3) * 4), g, Name, White);

}


bool AutoHS = false;

void DrawMenu(LPDIRECT3DDEVICE9 lmdDevice)   // DRAW MAIN MENU HERE
{
    CreateFontText(lmdDevice);

    AddItem("HACK BY Dark.StarVN", 1, lmdDevice, AutoHS);

    ResetFont();
}


typedef uint32_t uint150_t;   // If you want it run in 32bit. Please you write : "typedef uint32_t uint150_t;"
typedef long(__stdcall* EndScene)(LPDIRECT3DDEVICE9);
EndScene Original_EndScene = NULL;
static uint150_t* GlobalMethodsTable = NULL;

void Device() 
{
    IDirect3D9* MenuD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!MenuD3D) 
    {
        return;
    }

    D3DPRESENT_PARAMETERS dellbiet = { 0 };
    dellbiet.SwapEffect = D3DSWAPEFFECT_DISCARD;
    dellbiet.hDeviceWindow = GetForegroundWindow();
    dellbiet.Windowed = true;

    IDirect3DDevice9* lmdDevice = nullptr;

    HRESULT ketqua = MenuD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, dellbiet.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &dellbiet, &lmdDevice);
    if (FAILED(ketqua) || !lmdDevice) 
    {
        MenuD3D->Release();
        return;
    }

    void** dTable = *reinterpret_cast<void***>(lmdDevice);

    lmdDevice->Release();
    MenuD3D->Release();

    return;
}

long __stdcall _EndSceneIsHooked(LPDIRECT3DDEVICE9 lmdDevice)
{
    Device();
    DrawMenu(lmdDevice);
    return Original_EndScene(lmdDevice);
}

// ==================================================== Menu

// ==================================================== Hook

static HWND window = NULL;
WNDPROC oWndProc;

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
    DWORD wndProcId;
    GetWindowThreadProcessId(handle, &wndProcId);

    if (GetCurrentProcessId() != wndProcId)
        return TRUE; // skip to next window

    window = handle;
    return FALSE; // window found abort search
}

HWND GetProcessWindow()
{
    window = NULL;
    EnumWindows(EnumWindowsCallback, NULL);
    return window;
}

DWORD WINAPI Hook(LPVOID lpReserved)   // It uses 'CreateHook' to inject 'EndSceneIsHooked'
{
    if (kiero::init(kiero::RenderType::D3D9) == kiero::Status::Success) 
    {
        kiero::hook(42, (void**)&Original_EndScene, _EndSceneIsHooked);
        do
            window = GetProcessWindow();
        while (window == NULL);
    }

    return TRUE;
}

// ==================================================== Hook

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, 0, Hook, NULL, 0, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}