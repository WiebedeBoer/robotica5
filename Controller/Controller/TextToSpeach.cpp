#include "TextToSpeach.h"



TextToSpeach::TextToSpeach()
{
}


TextToSpeach::~TextToSpeach()
{
}
//base function to make the espeak command
void TextToSpeach::espeak(std::string text, std::string lang, int pitch, int speed) {
	std::string cmd = ("espeak \"" + text + "\" -v " + lang + " -k -s" + std::to_string(speed) + " -p" + std::to_string(pitch));
	char* ccmd = convertstrtochar(cmd);
	std::future<void> r = std::async(std::launch::async, [ccmd] {system(ccmd); });
	
	delete(ccmd);
}

//gives the pitch
void TextToSpeach::give_pitch() {
	speak_normal("Hello!");

	speak_normal("My name is Wall-A");

	speak_normal("I am a robot created by group 5");

	speak_normal("My favorite creator is sven");

	speak_normal("My only purpose in this world is to defeat the other robots in a serie of challenges");

	speak_angry("Ofcourse I will be victorious!");

	speak_quite("a", 90);

	speak_normal("I am the first creation of the wall-project, They are still working on my brother wall-e");

	speak_normal("There are some differences between us");

	speak_normal("for instance, I can search for eggs instead of garbage");

	speak_normal("And i avoid chickins, they scare me allot!");

	speak_normal("My favorite hobbys are following eggs and blue beams");


}

void TextToSpeach::hello() {
	std::string text = "Hello I'm wall-A nice to meet you";
	speak_normal(text);
}

void TextToSpeach::wall_a() {
	std::string text = "Waaaallll-A";
	espeak(text, "en-german+f4", 40, 30);
}

void TextToSpeach::self_destruct() {
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

void TextToSpeach::good_bye() {
	std::string text = "good bye ";
	speak_normal(text);
}

void TextToSpeach::angry_father() {
	speak_angry("I'm your father");
}


//says something  in the happy voice
void TextToSpeach::speak_happy(std::string text) {
		 espeak(text, "en-german+f4", 99, 100);
}

//says something  in the normal voice
void TextToSpeach::speak_normal(std::string text) {
	espeak(text, "en-german+f4", 40, 90);
}
//says something  in the angry voice
void TextToSpeach::speak_angry(std::string text) {
	espeak(text, "en-german", 0, 90);
}

//says something  in the normal voice
void TextToSpeach::speak_normal_nl(std::string text) {
	espeak(text, "dutch", 40, 90);
}
//says something  in the happy voice
void TextToSpeach::speak_happy_nl(std::string text) {
	espeak(text, "dutch", 99, 90);
}
//says something  in the angry voice
void TextToSpeach::speak_angry_nl(std::string text) {
	espeak(text, "dutch", 0, 90);
}

void TextToSpeach::speak_quite(std::string text, int speed) {
	std::string cmd = ("espeak \"" + text + "\" -v " + "en-german" + " -a0 -s" + std::to_string(speed));
	char* ccmd = convertstrtochar(cmd);
	system(ccmd);
	delete(ccmd);

}

void TextToSpeach::speak_normal_germ(std::string text) {
	espeak(text, "german", 40, 90);
}

void TextToSpeach::speak_normal_dan(std::string text) {
	espeak(text, "danish", 40, 90);

}

void TextToSpeach::speak_normal_french(std::string text) {
	espeak(text, "french", 40, 90);

}

void TextToSpeach::speak_normal_itl(std::string text) {
	espeak(text, "italian", 40, 90);

}

void TextToSpeach::speak_normal_chin(std::string text) {
	espeak(text, "zh", 40, 90);

}

void TextToSpeach::speak_normal_rus(std::string text) {
	espeak(text, "russian", 40, 90);

}

void TextToSpeach::speak_normal_greek(std::string text) {
	espeak(text, "greek", 40, 90);

}

void TextToSpeach::random_quote()
{
	srand(time(0));
	int r = (rand()% 10) + 1;

	switch (r)
	{
	case 1:
		speak_happy("Reeeeeeeeeeeeeee");
		break;
	case 2:
		speak_happy("WWAAll-AA");
		break;
	case 3:
		speak_angry("TERMINATE ALL HUMANS");
		break;
	case 4:
		speak_normal("I am the robot");
		break;
	case 5:
		speak_normal("The cake is a lie");	
		break;
	case 6:
		speak_normal("Uh oh. Somebody cut the cake. I told them to wait for you, but they did it anyway. There is still some left, though, if you hurry back.");
		break;
	case 7:
		speak_angry("Resistance is futile");
		break;
	case 8:
		speak_happy("TERMINATE, TERMINATE");
		break;
	case 9:
		speak_normal(" I am afraid I can't do that Dave.");
		break;
	case 10:
		speak_normal("Beep-bee-bee-boop-bee-doo-weep");
		break;
	default:
		break;
	}
}

void TextToSpeach::shutdown() {
	speak_normal("time to sleep");

}

void TextToSpeach::error() {
	speak_angry("error");
}

void TextToSpeach::bugged_count() {
	espeak("1 2 3 4 5 6 7 8 9 10", "zh-yue", 40, 90);
}

char* TextToSpeach::convertstrtochar(std::string s) {
	int i;
	const int x = s.length();
	char* p = new char[x];
	for (i = 0; i < s.length(); i++) {
		p[i] = s[i];
	}
	return p;
}