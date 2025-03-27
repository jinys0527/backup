#pragma once

namespace note
{
	enum noteType
	{
		Chorus = 0,
		Chorus_Do,
		Chorus_Fa,
		Shout
	};

	struct Note
	{
		long long startTime; //ms����
		long long endTime;   //ms����
		noteType curNoteType;
		long long duration;  //ms����
	};

	void InitNote();
	void CalcDuration(Note*& note, int size);
	Note*& GetNotes(int num);
}