#pragma once

#include <stdio.h>
#include <bits/stdc++.h>
#include <future>
#include <ctime>
#include <cstdlib>
class TextToSpeach
{

public:
	TextToSpeach();
	~TextToSpeach();
	std::string ttttttt;
	char* convertstrtochar(std::string s);
	//base function to make the espeak command
	void espeak(std::string text, std::string lang, int pitch, int speed);
	//says something  in the normal voice
	void speak_normal(std::string text);
	//says something  in the happy voice
	void speak_happy(std::string text);
	//says something  in the angry voice
	void speak_angry(std::string text);

	//says something  in the normal voice
	void speak_normal_nl(std::string text);
	//says something  in the happy voice
	void speak_happy_nl(std::string text);
	//says something  in the angry voice
	void speak_angry_nl(std::string text);

	void speak_normal_germ(std::string text);

	void speak_normal_dan(std::string text);

	void speak_normal_french(std::string text);

	void speak_normal_itl(std::string text);

	void speak_normal_chin(std::string text);

	void speak_normal_rus(std::string text);

	void speak_normal_greek(std::string text);

	//gives the pitch
	void give_pitch();
	//says hello
	void hello();
	//says good bye
	void good_bye();
	void angry_father();
	void wall_a();
	//self destructs
	void self_destruct();
	void shutdown();
	void error();
	void bugged_count();
	//says something quite to wait a but
	void speak_quite(std::string text, int speed);
	void random_quote();

};

