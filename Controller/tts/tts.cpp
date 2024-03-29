#include "tts.h"

tts::tts()
{
	std::cout << "hio";
}

tts::~tts()
{
}

void tts::espeak(std::string text, std::string lang, int pitch, int speed){
	std::string cmd = ("espeak \"" + text +"\" -v " + lang + " -k -s" + std::to_string(speed) + " -p" + std::to_string(pitch));
	char* ccmd = convertstrtochar(cmd);
	system(ccmd);
	delete(ccmd);
}

void tts::give_pitch(){
	std::string text = "My name is Wall-A";
	speak_normal(text);

	text = "I am a robot created by group 5";
	speak_normal(text);

	text = "1 of my creators is sven";
	speak_normal(text);

	text = "My purpose in this world is to defeat the other robots in a serie of challenges";
	speak_normal(text);

	text = "a";
	speak_quite(text, 90);

	text = "I am the first creation of the wall-project, you probally know my younger brother wall-e";
	speak_normal(text);

	text = "this is  ";
	speak_quite(text, 90);
	
	text = "There are some differences between us";
	speak_normal(text);
	
	text = "I can search for eggs instead of garbage";
	speak_normal(text);
}

void tts::hello(){
	std::string text = "Hello I'm wall-A nice to meet you";
	speak_normal(text);
}

void tts::wall_a() {
	std::string text = "Waaaallll-A";
	espeak(text,"en-german+f4", 40, 30);
}

void tts::self_destruct() {
	std::string text = "10 seconds to self destruct";
	
	speak_normal(text);
	speak_normal("10");
	speak_normal("9");
	speak_normal("8");
	speak_normal("7");
	speak_normal("6");
	speak_normal("5");
	speak_normal("4");
	speak_normal("3");
	speak_normal("2");
	speak_normal("1");
	speak_angry("goodbye");

}

void tts::good_bye(){
	std::string text = "good bye ";
	speak_normal(text);
}

void tts::angry_father() {
	speak_angry("I'm your father");
}



void tts::speak_happy(std::string text){
	espeak(text, "en-german+f4", 99, 100);
}


void tts::speak_normal(std::string text){
	espeak(text, "en-german+f4", 40, 90);
}

void tts::speak_angry(std::string text){
	espeak(text, "en-german", 0, 90);
}

void tts::speak_quite(std::string text, int speed){
	std::string cmd = ("espeak \"" + text +"\" -v " + "en-german" + " -a0 -s" + std::to_string(speed));
	char* ccmd = convertstrtochar(cmd);
	system(ccmd);
	delete(ccmd);

}

void tts::shutdown(){
	speak_normal("time to sleep");
	
}

void tts::error(){
	speak_angry("error");
}

void tts::bugged_count(){
	espeak("1 2 3 4 5 6 7 8 9 10", "zh-yue", 40, 90);
}

char* tts::convertstrtochar(std::string s) {
	int i;
	const int x = s.length();
	char* p = new char[x];
	for (i = 0; i < s.length(); i++) {
		p[i] = s[i];
	}
	return p;
}