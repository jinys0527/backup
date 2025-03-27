#include <iostream>
#include <windows.h>
#include "RenderSystem.h"
#include "Anim.h"
#include "FmodEffect.h"
#include "Fmod.h"
using namespace std;

namespace anim
{
	// ĳ���� ������
	Character characters[4];

	Character* chorus1 = nullptr;
	Character* chorus2 = nullptr;
	Character* player = nullptr;

	bool isShoutAnimating = false;

	// �ִϸ��̼� ������ �迭
	const char* chantFrames[] = {
		"Character/Chorus_boys_silence.txt",
		"Character/Chorus_boys_sp3.txt",
		"Character/Chorus_boys_sp2.txt",
		"Character/Chorus_boys_sp1.txt",
		"Character/Chorus_boys_sp0.txt",
		"Character/Chorus_boys_default.txt"
	};

	const char* muteChantFrames[] = {
		"Character/Chorus_boys_default.txt",
		"Character/Chorus_boys_sp0.txt",
		"Character/Chorus_boys_sp1.txt",
		"Character/Chorus_boys_sp2.txt",
		"Character/Chorus_boys_sp3.txt",
		"Character/Chorus_boys_silence.txt"
	};

	const char* fastChantFrames[] =	{
		"Character/Chorus_boys_silence.txt",
		"Character/Chorus_boys_sp2.txt",
		"Character/Chorus_boys_default.txt"
	};

	const char* fastMuteChantFrames[] = {
		"Character/Chorus_boys_default.txt",
		"Character/Chorus_boys_sp2.txt",
		"Character/Chorus_boys_silence.txt"
	};

	const char* shoutFrames[] = {
		"Character/Chorus_boys_silence.txt",
		"Character/Chorus_boys_enter3.txt",
		"Character/Chorus_boys_enter1.txt",
		"Character/Chorus_boys_enter0.txt",
		"Character/Chorus_boys_enter1.txt",
		"Character/Chorus_boys_enter3.txt",
		"Character/Chorus_boys_silence.txt"
	};

	const char* muteShoutFrames[] = {
		"Character/Chorus_boys_enter0.txt",
		"Character/Chorus_boys_enter1.txt",
		"Character/Chorus_boys_enter2.txt",
		"Character/Chorus_boys_enter3.txt",
		"Character/Chorus_boys_silence.txt"
	};

	const char* playerShoutFrames[] = {
		"Character/Chorus_boys_silence.txt",
		"Character/Chorus_boys_enter3.txt",
		"Character/Chorus_boys_enter2.txt",
		"Character/Chorus_boys_enter1.txt",
		"Character/Chorus_boys_enter1.txt",
		"Character/Chorus_boys_enter0.txt",
		"Character/Chorus_boys_enter0.txt",
		"Character/Chorus_boys_enter0.txt",
		"Character/Chorus_boys_enter1.txt",
		"Character/Chorus_boys_enter2.txt",
		"Character/Chorus_boys_enter3.txt",
		"Character/Chorus_boys_silence.txt"
	};

	const char* fastShoutFrames[] = {
		"Character/Chorus_boys_silence.txt",
		"Character/Chorus_boys_enter2.txt",
		"Character/Chorus_boys_enter0.txt"
	};

	const char* fastMuteShoutFrames[] = {
		"Character/Chorus_boys_enter0.txt",
		"Character/Chorus_boys_enter2.txt",
		"Character/Chorus_boys_silence.txt"
	};

	const char* shameFrames[] = {
		"Character/Chorus_boys_shame.txt"
	};

	// �� �ִϸ��̼� �������� ������ ����
	const int chantFrameCount = sizeof(chantFrames) / sizeof(chantFrames[0]);
	const int muteChantFrameCount = sizeof(muteChantFrames) / sizeof(muteChantFrames[0]);
	const int fastChantFrameCount = sizeof(fastChantFrames) / sizeof(fastChantFrames[0]);
	const int fastMuteChantFrameCount = sizeof(fastMuteChantFrames) / sizeof(fastMuteChantFrames[0]);
	const int shoutFrameCount = sizeof(shoutFrames) / sizeof(shoutFrames[0]);
	const int muteShoutFrameCount = sizeof(muteShoutFrames) / sizeof(muteShoutFrames[0]);
	const int fastShoutFrameCount = sizeof(fastShoutFrames) / sizeof(fastShoutFrames[0]);
	const int fastMuteShoutFrameCount = sizeof(fastMuteShoutFrames) / sizeof(fastMuteShoutFrames[0]);
	const int playerShoutFrameCount = sizeof(playerShoutFrames) / sizeof(playerShoutFrames[0]);
	const int shameFrameCount = sizeof(shameFrames) / sizeof(shameFrames[0]);

	void InitCharacter(Character* character, int x, int y, const char* currentFrame, AnimState currentState, int frameIndex, bool isAnimating)
	{
		character->x = x;
		character->y = y;
		character->currentFrame = currentFrame;
		character->currentState = currentState;
		character->frameIndex = frameIndex;
		character->isAnimating = isAnimating;
	}

	void StartGame()
	{
		render::InitScreen();

		//�ڷ��� 1
		InitCharacter(&characters[0], 70, 1, "Character/Chorus_boys_silence.txt", NONE, 0, false);
		//�ڷ��� 2
		InitCharacter(&characters[1], 120, 6, "Character/Chorus_boys_silence.txt", NONE, 0, false);
		//�÷��̾�
		InitCharacter(&characters[2], 170, 11, "Character/Chorus_boys_default.txt", NONE, 0, false);
		//������
		InitCharacter(&characters[3], 10, 40, "Character/Conductor.txt", NONE, 0, false);
		chorus1 = &characters[0];
		chorus2 = &characters[1];
		player = &characters[2];
		// ���� ȭ�� ���
		DrawScreen();
	}

	void DrawScreen()
	{
		// ���� ĳ���� �������� �׸���
		for (int i = 0; i < 4; i++)
		{
			DrawCharacter(characters[i].x, characters[i].y, characters[i].currentFrame);
		}
	}

	void DrawCharacter(int x, int y, const char* filename)
	{
		FILE* fp;
		fopen_s(&fp, filename, "rb");
		if (fp == NULL)
		{
			return;
		}
		char* Word = new char[MAX_CHARACTER_SIZE];
		while (fgets(Word, MAX_CHARACTER_SIZE, fp) != NULL)
		{
			render::ScreenDraw(x, y++, Word);
		}
		fclose(fp);
		delete[] Word; 
	}

	void UpdateAnimation()
	{
		UpdateAIAnimation(GetCharacter(0), note::GetNotes(1), 22);
		UpdateAIAnimation(GetCharacter(1), note::GetNotes(2), 22);
		UpdateAnimation(GetCharacter(0));
		UpdateAnimation(GetCharacter(1));
		UpdateAnimation(GetCharacter(2));
	}

	void UpdateAnimation(Character*& character)
	{
		LARGE_INTEGER end1;

		if (character->currentState == AnimState::PLAYER_SHOUT && character->frameIndex == 0)
		{
			effectsound::EffectPlaySound(1, effectsound::GetChannel(1));
		}

		QueryPerformanceCounter(&end1);

		if (end1.QuadPart - character->lastAnimTick < 140000)
		{
			return;
		}

		character->isAnimating = true;
		character->frameIndex++;

		// �ִϸ��̼� ���¿� ���� ������ ������Ʈ
		switch (character->currentState)
		{
		case CHANT:
			if (character->frameIndex < chantFrameCount)
			{
				character->currentFrame = chantFrames[character->frameIndex];
			}
			else
			{
 				ResetState(character, false, NONE);
			}
			break;

		case MUTE_CHANT:
			if (character->frameIndex < muteChantFrameCount)
			{
				character->currentFrame = muteChantFrames[character->frameIndex];
			}
			else
			{
				ResetState(character, false, NONE);
			}
			break;

		case FAST_CHANT:
			if (character->frameIndex < fastChantFrameCount)
			{
				character->currentFrame = fastChantFrames[character->frameIndex];
			}
			else
			{
				ResetState(character, false, NONE);
			}
			break;

		case FAST_MUTE_CHANT:
			if (character->frameIndex < fastMuteChantFrameCount)
			{
				character->currentFrame = fastMuteChantFrames[character->frameIndex];
			}
			else
			{
				ResetState(character, false, NONE);
			}
			break;

		case SHOUT:
			if (character->frameIndex < shoutFrameCount)
			{
				isShoutAnimating = true;
				character->currentFrame = shoutFrames[character->frameIndex];
			}
			else
			{
				ResetState(character, false, NONE);
			}
			break;

		case MUTE_SHOUT:
			if (character->frameIndex < shoutFrameCount)
			{
				character->currentFrame = muteShoutFrames[character->frameIndex];
			}
			else
			{
				isShoutAnimating = false;
				ResetState(character, false, NONE);
			}
			break;

		case PLAYER_SHOUT:
			if (character->frameIndex < playerShoutFrameCount)
			{
				isShoutAnimating = true;
				character->currentFrame = playerShoutFrames[character->frameIndex];
			}
			else
			{
				isShoutAnimating = false;
				ResetState(character, false, NONE);
			}
			break;

		case SHAME:
			if (character->frameIndex < shameFrameCount)
			{
				character->currentFrame = shameFrames[character->frameIndex];
			}
			else
			{
				ResetState(character, false, NONE);
			}
			break;
		case NONE:
			character->isAnimating = false;
			character->frameIndex--;
			break;
		}
		QueryPerformanceCounter(&end1);
		character->lastAnimTick = end1.QuadPart;
	}

	void ResetState(Character*& character, bool isAnimating, AnimState currentState)
	{
		character->isAnimating = isAnimating;
		character->currentState = currentState;
	}

	void StartAnimation(Character*& character, AnimState state)
	{
		character->currentState = state;
		character->frameIndex = 0;
		character->isAnimating = true;

		// �ִϸ��̼� ���� �� ù ������ ����
		switch (state)
		{
		case CHANT:
			character->currentFrame = chantFrames[0];
			break;
		case MUTE_CHANT:
			character->currentFrame = muteChantFrames[0];
			break;
		case FAST_CHANT:
			character->currentFrame = fastChantFrames[0];
			break;
		case FAST_MUTE_CHANT:
			character->currentFrame = fastMuteChantFrames[0];
			break;
		case SHOUT:
			character->currentFrame = shoutFrames[0];
			break;
		case SHAME:
			character->currentFrame = shameFrames[0];
			break;
		}
	}

	bool IsShoutAnimating()
	{
		return isShoutAnimating;
	}

	void AssignState(Character*& character, note::Note*& note, int index)
	{
		if (index >= 0 && index < 22) {
			switch (note[index].curNoteType)
			{
			case note::noteType::Chorus:
				character->currentState = CHANT;
				break;
			case note::noteType::Shout:
				character->currentState = SHOUT;
				break;
			}
		}
	}

	Character*& GetCharacter(int num)
	{
		switch (num)
		{
		case 0:
			return chorus1;
		case 1:
			return chorus2;
		case 2:
			return player;
		}
	}

	void UpdateAIAnimation(Character*& character, note::Note*& note, int noteSize)
	{
		unsigned int playPos = sound::GetPlayPosition();
		LARGE_INTEGER end;
		QueryPerformanceCounter(&end);
		character->lastPlayPos = end.QuadPart;
		const int tolerance = 20; //30ms ���� ����

		for (int i = 0; i < noteSize; i++)
		{
			if (i >= 0 && i < 22)	
			{
				if ((note[i].startTime - tolerance) <= playPos && playPos <= (note[i].endTime + tolerance))
				{
					AssignState(character, note, i);
					if (note[i].duration < 200)
					{
						if (character->currentState == CHANT)
						{
							character->currentState = FAST_CHANT;
						}
						else if (character->currentState == SHOUT)
						{
							character->currentState = FAST_SHOUT;
						}
						bool animEnd = AIAnimEnd(character, note[i].startTime, note[i].duration);
						while (!animEnd)
						{
							animEnd = AIAnimEnd(character, note[i].startTime, note[i].duration);
						}
						break;
					}
					else if (note[i].duration >= 200)
					{
						bool animEnd = AIAnimEnd(character, note[i].startTime, note[i].duration);
						while (!animEnd)
						{
							animEnd = AIAnimEnd(character, note[i].startTime, note[i].duration);
						}
						break;
					}
				}
			}
		}
	}

	bool AIAnimEnd(Character*& character, long long start, double duration)
	{
		LARGE_INTEGER frequency, end; 
		QueryPerformanceCounter(&end);
		
		int animTime = 0;
		switch (character->currentState)
		{
		case CHANT:
			StartAnimation(character, CHANT);
			animTime = 33 * muteChantFrameCount;
			break;
		case FAST_CHANT:
			StartAnimation(character, FAST_CHANT);
			animTime = 33 * fastMuteChantFrameCount;
			break;
		case SHOUT:
			StartAnimation(character, SHOUT);
			animTime = 33 * shoutFrameCount;
			break;
		case FAST_SHOUT:
			StartAnimation(character, FAST_SHOUT);
			animTime = 33 * fastShoutFrameCount;
			break;
		case TOGGLE_CHANT:
			animTime = 0;
			DrawCharacter(character->x, character->y, "Character / Chorus_boys_default.txt");
			break;
		}

		long long lastAnimStartTime = duration - animTime;


		long long dduration = (end.QuadPart - character->lastPlayPos);

		if (dduration >= lastAnimStartTime)
		{
			switch (character->currentState)
			{
			case CHANT:
				StartAnimation(character, MUTE_CHANT);
				break;
			case FAST_CHANT:
				StartAnimation(character, FAST_MUTE_CHANT);
				break;
			case TOGGLE_CHANT:
				DrawCharacter(character->x, character->y, "Character / Chorus_boys_silence.txt");
				break;
			}
			return true;
		}

		return false;
	}
}