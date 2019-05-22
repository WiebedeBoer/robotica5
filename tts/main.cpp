#include <stdio.h>
#include <bits/stdc++.h>

char* convertstrtochar(std::string s);
void tts(std::string text, std::string lang, int pitch, int speed);
void speak_normal(std::string text);
void speak_happy(std::string text);
void speak_angry(std::string text);


int main(int argc, char **argv)
{
	//system("espeak \"test espeak in c++\"");
	//tts("something witty", "en-us", 99, 60);
	std::string str= "I am wall-e and i live on a planet with eve";
	speak_normal(str);	
	speak_happy(str);
	speak_angry("I am wall-e and i am your father");

}

void tts(std::string text, std::string lang, int pitch, int speed){
	std::string cmd = ("espeak \"" + text +"\" -v " + lang + " -k -s" + std::to_string(speed) + " -p" + std::to_string(pitch));
	char* ccmd = convertstrtochar(cmd);
	system(ccmd);
	delete(ccmd);
}

void speak_happy(std::string text){
	tts(text, "en-german+f4", 99, 100);
}


void speak_normal(std::string text){
	tts(text, "en-german+f4", 40, 90);
}

void speak_angry(std::string text){
	tts(text, "en-german", 0, 90);
}

char* convertstrtochar(std::string s) {
	int i;
	const int x = s.length();
	char* p = new char[x];
	for (i = 0; i < s.length(); i++) {
		p[i] = s[i];
	}
	return p;
}