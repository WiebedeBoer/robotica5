#include <stdio.h>
#include <bits/stdc++.h>

char* convertstrtochar(std::string s);
void tts(std::string text, std::string lang, int pitch, int speed);
int main(int argc, char **argv)
{
	//system("espeak \"test espeak in c++\"");
	tts("something witty", "en-us", 99, 60);
}
void tts(std::string text, std::string lang, int pitch, int speed){
	std::string cmd = ("espeak \"" + text +"\" -v " + lang + " -k -s" + std::to_string(speed) + " -p" + std::to_string(pitch));
	char* ccmd = convertstrtochar(cmd);
	system(ccmd);
	delete(ccmd);
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