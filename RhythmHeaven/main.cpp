#include <iostream>
#include <windows.h>
#include "Anim.h"
#include "RenderSystem.h"
#include "InputSystem.h"
#include "Fmod.h"
#include "FmodEffect.h"

// �ִϸ��̼� ���¸� �����ϴ� �⺻ ��
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
    // ���� - Ű �Է¿� ���� �ִϸ��̼� ����
   
    if (GetAsyncKeyState('5') & 0x8000)
    {
        // ��� ĳ���Ϳ� shame �ִϸ��̼� ����
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
        ProcessInput(); // �Է� ó�� Ȱ��ȭ
        Update();
        QueryPerformanceCounter(&end);

        anim::UpdateAnimation(); //�ִϸ��̼� ������Ʈ

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