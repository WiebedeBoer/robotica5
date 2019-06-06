#pragma once
#include <stdio.h>
#include <bits/stdc++.h>
class tts
{
public:
	tts();
	std::string ttttttt;
	char* convertstrtochar(std::string s);
	void espeak(std::string text, std::string lang, int pitch, int speed);
	void speak_normal(std::string text);
	void speak_happy(std::string text);
	void speak_angry(std::string text);
	void give_pitch();
	void hello();
	void good_bye();
	void angry_father();
	void wall_a();
	void self_destruct();
	void shutdown();
	void error();
	void bugged_count();
	void speak_quite(std::string text, int speed);
	
	~tts();

};

