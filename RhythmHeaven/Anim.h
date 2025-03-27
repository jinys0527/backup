#pragma once
#include "Note.h"
#define MAX_CHARACTER_SIZE 256

namespace anim
{
    enum AnimState {
        NONE,
        CHANT,              // case 1: silence -> sp3 -> sp2 -> sp1 -> sp0 -> default
        MUTE_CHANT,         // case 2: default -> sp0 -> sp1 -> sp2 -> sp3 -> silence
        FAST_CHANT,         // case 3: silence -> sp2 -> default
        FAST_MUTE_CHANT,    // case 4: default -> sp2 -> silence
        SHOUT,              // case 5: silence -> enter3 -> enter2 -> enter1 -> enter0
        MUTE_SHOUT,         // case 6: enter0 -> enter1 -> enter2 -> enter3 -> silence
        PLAYER_SHOUT,       // case 7: silence -> enter3 -> enter2 -> enter1 -> enter0 -> enter0 -> enter1 -> enter2 -> enter3 -> silence
        FAST_SHOUT,         // case 8: silence -> enter2 -> enter0
        FAST_MUTE_SHOUT,    // case 9: enter0 -> enter2 -> silence
        SHAME,              // case 10: any -> shame
        TOGGLE_CHANT        // case 11: toggle silence <-> default
    };

    struct Character {
        int x, y;                     // ��ġ
        const char* currentFrame;     // ���� ������
        AnimState currentState;       // ���� �ִϸ��̼� ����
        int frameIndex;               // ���� ������ �ε���
        bool isAnimating;             // �ִϸ��̼� ������ ����
        ULONGLONG lastAnimTick;       // �� ĳ���ͺ� �ִϸ��̼� Ÿ�̸� �߰�
        ULONGLONG lastPlayPos;        // ��Ʈ ���� �ð�
    };

    void InitCharacter(Character* character, int x, int y, 
                    const char* currentFrame, AnimState currentState, 
                    int frameIndex, bool isAnimating); //�ʱ�ȭ �Լ�

    void StartGame();
    void DrawScreen();
    void DrawCharacter(int x, int y, const char* filename);
    void UpdateAnimation();
    void UpdateAnimation(Character*& character);
    void ResetState(Character*& character, bool isAnimating, AnimState currentState);
    void StartAnimation(Character*& character, AnimState state);
    bool IsShoutAnimating();

    void AssignState(Character*& character, note::Note*& note, int index);

    Character*& GetCharacter(int num);

    void UpdateAIAnimation(Character*& character, note::Note*& note, int noteSize);
    bool AIAnimEnd(Character*& character, long long startTime, double duration);
}