#include "InputSystem.h"
#include <cassert>
#include <windows.h>
#include "FmodEffect.h"
#include "Anim.h"
#include "inc/fmod_errors.h"

namespace input
{
	bool inputKeyTable[MAX_KEY];

	bool wasSpacePressed = false;

	void Set(const int keyIndex, bool bOn)
	{
		assert(keyIndex >= 0 && keyIndex < MAX_KEY);

		inputKeyTable[keyIndex] = bOn;
	}

	bool IsEscapeCmdOn()
	{
		return inputKeyTable[ESCAPE_KEY_INDEX];
	}

	bool IsUpCmdOn()
	{
		return inputKeyTable[USER_CMD_UP];
	}

	bool IsDownCmdOn()
	{
		return inputKeyTable[USER_CMD_DOWN];
	}

	bool IsSpaceCmdOn()
	{
		return inputKeyTable[USER_CMD_SPACE];
	}

	bool IsEnterCmdOn()
	{
		return inputKeyTable[USER_CMD_ENTER];
	}

	void UpdateInput()
	{
		bool isSpacePressed = (GetKeyState(VK_SPACE) & 0x8000) != 0;
		
		if (anim::IsShoutAnimating())
		{
			return;
		}

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		{
			input::Set(ESCAPE_KEY_INDEX, true);
		}
		else
		{
			input::Set(ESCAPE_KEY_INDEX, false);
		}

		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			input::Set(USER_CMD_UP, true);
		}
		else
		{
			input::Set(USER_CMD_UP, false);
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			input::Set(USER_CMD_DOWN, true);
		}
		else
		{
			input::Set(USER_CMD_DOWN, false);
		}

		if (isSpacePressed && !wasSpacePressed)
		{
			input::Set(USER_CMD_SPACE, true);
			anim::StartAnimation(anim::GetCharacter(2), anim::MUTE_CHANT); // �� ��° ĳ���� ���������� �ִϸ��̼�
 			effectsound::PauseChannel(effectsound::GetChannel(0));
		}
		else if(!isSpacePressed && wasSpacePressed)
		{
			input::Set(USER_CMD_SPACE, false); 
			anim::StartAnimation(anim::GetCharacter(2), anim::CHANT); // �� ��° ĳ���� ���ϱ� �ִϸ��̼�
			effectsound::UnpauseChannel(effectsound::GetChannel(0));
		}

		if (isSpacePressed && GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			input::Set(USER_CMD_ENTER, true);
			anim::StartAnimation(anim::GetCharacter(2), anim::PLAYER_SHOUT); // �� ��° ĳ���� ����Ʈ �ִϸ��̼�
		}
		else
		{
			input::Set(USER_CMD_ENTER, false);
		}
		wasSpacePressed = isSpacePressed;
	}
}