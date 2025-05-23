#include <iostream>
#include <windows.h>
#include "Anim.h"
#include "RenderSystem.h"
#include "InputSystem.h"
#include "Fmod.h"
#include "FmodEffect.h"

// 애니메이션 상태를 관리하는 기본 값
int currentAnimationState = 0;
bool isRun = true;

void Render()
{
    render::ScreenClear();
    anim::DrawScreen();
    render::ScreenFlipping();
}

void ProcessInput()
{
    input::UpdateInput();
    // 예시 - 키 입력에 따라 애니메이션 시작
   
    if (GetAsyncKeyState('5') & 0x8000)
    {
        // 모든 캐릭터에 shame 애니메이션 적용
        for (int i = 0; i < 2; i++)
        {
            anim::StartAnimation(anim::GetCharacter(i), anim::SHAME);
        }
    }
}

void Update()
{
    effectsound::Update();
}

bool IsGameRun()
{
    return isRun;
}

void EndGame()
{
    sound::Releasesound();
    effectsound::ReleaseEffectSound();
}

int main()
{   
    LARGE_INTEGER start, end; // QPC

    anim::StartGame();

    sound::SoundSetUp();
    effectsound::EffectSoundSetUp();
    sound::Playsound(0);

    QueryPerformanceCounter(&start);

    effectsound::EffectPlaySound(0, effectsound::GetChannel(0));
    note::InitNote();

    Render();
    bool isPlaying;

    while (IsGameRun()) //IsGameRun())
    {
        ProcessInput(); // 입력 처리 활성화
        Update();
        QueryPerformanceCounter(&end);

        anim::UpdateAnimation(); //애니메이션 업데이트

        if (end.QuadPart - start.QuadPart >= 40)
        {
            Render();
            start.QuadPart = end.QuadPart;
        }

        sound::GetChannel(0)->isPlaying(&isPlaying);
        if (!isPlaying)
        {
            isRun = false;
        }
    }

    EndGame();
    return 0;
}