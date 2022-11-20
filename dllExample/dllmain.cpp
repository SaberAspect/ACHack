// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <iostream>
#include "offsets.h"
#include <math.h>
#include "Helper.h"
#include "Hook.h"
#include "ESP.h"
#include "Drawing.h"

int hookLength = 5;
DWORD hookAddress = 0x429D1F;
uintptr_t jmpBackAddy = hookAddress + hookLength;
entPlayer* LocalPlayerPtr = Helpers::GetLocalPlayer();
uintptr_t base = (uintptr_t)GetModuleHandle(NULL);

/*Our godmode function, compares the person who is
//damaged and applies it if they are not the localPlayer, 
else we jump over the decrement health piece of code
*/
void  __declspec(naked) godMode()
{
    __asm {
        mov edi, 0xf4
        mov eax, [LocalPlayerPtr]
        add eax, edi
        cmp ebx, eax
        jne Og_Code
        jmp Exit

        Og_Code :
        sub[ebx + 0x04], edi
        mov eax, edi
  
        Exit:
        jmp[jmpBackAddy]
    }
    
}

//Defining types for funtion calling
typedef BOOL(__stdcall* twglSwapBuffers) (HDC hDc);
typedef char*(__cdecl* _ACPrintF)(const char* sFormat, ...);

_ACPrintF ACPrintF = (_ACPrintF)(base + 0x6b060);

const char* sFormat = "%s:\f%d %s";
twglSwapBuffers wglSwapBuffersGateway;

const char* hacks[6] = {
    "Aimbot (F1)",   //0
    "GodMode (F2)",  //1
    "ESP (F3)",      //2
    "NoRecoil (F4)", //3
    "Headshots Sniper only (F5)",//4
    "RapidFire (F6)" //5

};

bool bAimbot = false, bGodmode = false, bDrawMenu = false, bDrawESP = false, bNoRecoil = false, bHeadShots = false, bRapidFire = false;

ESP esp;
GL::Font font;
const int FONT_HEIGHT = 15;
const int FONT_WIDTH = 9;

const char* name = "Saber's ACHack";
float centeredX = font.centerText(50, 120, strlen(name) * FONT_WIDTH);
float centeredY = 50 - FONT_HEIGHT / 2;

void Draw()
{
    HDC currHDC = wglGetCurrentDC();
    if (!font.bBuilt || currHDC != font.hdc)
        font.Build(FONT_HEIGHT);

    GL::SetupOrtho();
    esp.Draw();
    GL::RestoreGL();
}
void drawMenu()
{    
    HDC currHDC = wglGetCurrentDC();
    if (!font.bBuilt || currHDC != font.hdc)
        font.Build(FONT_HEIGHT);
    GL::SetupOrtho();
    //Drawing outline of the menu
    GL::DrawOutline(50, 50, 120, 200, 2, rgb::green);

    font.Print(centeredX, centeredY, rgb::red, "%s", name);
    //Aimbot
    Helpers::DisplayToggleHack(font, 55, 83, hacks[0], bAimbot);
    //GodMode
    Helpers::DisplayToggleHack(font, 55, 116, hacks[1], bGodmode);
    //ESP
    Helpers::DisplayToggleHack(font, 55, 149, hacks[2], bDrawESP);
    //Recoil
    Helpers::DisplayToggleHack(font, 55, 182, hacks[3], bNoRecoil);
    //Headshots
    Helpers::DisplayToggleHack(font, 55, 215, hacks[4], bHeadShots);
    //Rapidfire
    Helpers::DisplayToggleHack(font, 55, 248, hacks[5], bRapidFire);


    GL::RestoreGL();
}
BOOL __stdcall hkwglSwapBUffers(HDC hDc)
{
        //AimBot
        if (GetAsyncKeyState(VK_F1) & 1)
            bAimbot = !bAimbot;
        //GodMode
        if (GetAsyncKeyState(VK_F2) & 1)
        {
            bGodmode = !bGodmode;
            if (bGodmode) {
                LocalPlayerPtr->currWeapon->currReserve->Amount = 999;
                LocalPlayerPtr->currWeapon->currAmmo->Amount = 999;
                LocalPlayerPtr->iArmor = 999;
                Detour32((BYTE*)hookAddress, (BYTE*)godMode, hookLength);
                Helpers::NOP((BYTE*)0x4637e9, 2);
            }
            else {
                LocalPlayerPtr->currWeapon->currReserve->Amount = 40;
                LocalPlayerPtr->currWeapon->currAmmo->Amount = 20;
                Helpers::Patch((BYTE*)0x4637e9, (BYTE*)"\xff\x0e", 2);
                Helpers::Patch((BYTE*)hookAddress, (BYTE*)"\x29\x7b\x04\x8b\xc7", 5);
            }
        }
        //ESP
        if (GetAsyncKeyState(VK_F3) & 1)
            bDrawESP = !bDrawESP;
        //Recoil
        if (GetAsyncKeyState(VK_F4) & 1)
        {
            bNoRecoil = !bNoRecoil;

            if (bNoRecoil)
            {
                Helpers::NOP((BYTE*)0x463786, 10);
            }
            else
            {
                Helpers::Patch((BYTE*)0x463786, (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10);
            }
        }
        if (GetAsyncKeyState(VK_F5) & 1)
        {
            bHeadShots = !bHeadShots;
        if(bHeadShots)
            Helpers::NOP((BYTE*)0x461767, 2);
        else
            Helpers::Patch((BYTE*)0x461767, (BYTE*)"\x75\x09", 2);
        }
        if (GetAsyncKeyState(VK_F6) & 1)
        {
            bRapidFire = !bRapidFire;
        }
        //Menu
        if (GetAsyncKeyState(VK_INSERT) & 1)
            bDrawMenu = !bDrawMenu;

        if (bRapidFire)
            LocalPlayerPtr->currWeapon->currShotDelay->Amount = 0;
        //Actuall aimbot code
        if (bAimbot)
        {
            bool smooth = false;
            if (Helpers::isKeyPressed(VK_XBUTTON1))
                Helpers::aimAt(LocalPlayerPtr, smooth, 5);
        }
       //Drawing menu call
        if (bDrawMenu)
            drawMenu();
        //ESP drawing call
        if (bDrawESP)
            Draw();
        
    return wglSwapBuffersGateway(hDc);
}

DWORD WINAPI HackThread(HMODULE hModule)
{   
    ACPrintF(sFormat, "ACHACK", 1, "Loading");

    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
   
    Hook SwapBuffersHook("wglSwapBuffers", "opengl32.dll", (BYTE*)hkwglSwapBUffers, (BYTE*)&wglSwapBuffersGateway, 5);

    SwapBuffersHook.Enable();

    ACPrintF(sFormat, "ACHACK", 1, "Loaded");

    while (!GetAsyncKeyState(VK_DELETE) & 1)
    {
    }
    ACPrintF(sFormat, "ACHACK", 1, "Unloading");

    SwapBuffersHook.Disable();


    LocalPlayerPtr->currWeapon->currReserve->Amount = 40;
    LocalPlayerPtr->currWeapon->currAmmo->Amount = 20;
    Helpers::Patch((BYTE*)0x461767, (BYTE*)"\x75\x09", 2);
    Helpers::Patch((BYTE*)0x463786, (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10);
    Helpers::Patch((BYTE*)hookAddress, (BYTE*)"\x29\x7b\x04\x8b\xc7", 5);
    Helpers::Patch((BYTE*)0x4637e9, (BYTE*)"\xff\x0e", 2);



    Sleep(100);
    ACPrintF(sFormat, "ACHACK", 1, "Unloaded");

    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HackThread, hModule, NULL, NULL);
        DisableThreadLibraryCalls(hModule);
        break;
    }
    return TRUE;
}

