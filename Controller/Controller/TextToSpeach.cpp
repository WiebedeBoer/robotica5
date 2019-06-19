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
	speak_normal_nl("Hallo allemaal, mijn naam is al meerdere keren door de gangen van de school gegaan, zo populair ben ik.")
	speak_normal_nl("Mijn naam is Wall-a. Volgens de makers de beste robot die hier vandaag staat.")
	speak_normal_nl("Misschien zie ik er bekend uit, en dat zou kunnen kloppen.")
    speak_normal_nl("Mijn projectgroep is groep 5, ze hebben in de WTB werkplaats uit de aller diepste lade een blauwdruk gevonden van mij.")
    speak_normal_nl("Ik ben namelijk de voorloper van de bekende Wall-e robots. Ik ben prototype a.")
    speak_normal_nl("Groep 5 heeft besloten dat ik de beste robot zou zijn voor de wedstrijd na het zien van de blauwdruk.")
    speak_normal_nl("Zij hebben mij in de korte tijd proberen op te bouwen, dit met enkele tegenslagen.)
    speak_normal_nl("Eieren rapen en blauwe blokjes volgens kan ik heel erg goed. Bij kippen blijf ik liever uit de buurt, want die vind ik een beetje eng.")
    speak_normal_nl("Ik luister ook graag naar muziek en kan hier erg goed op dansen. Als mijn favoriete muziek afgespeeld wordt veranderen mijn ogen van kleur.")
    speak_normal_nl("Door mijn rupsbanden kan ik heel goed over de bak met grind rijden, dit is één van mijn beste eigenschappen.")
    speak_normal_nl("Ik maak kans om te winnen, omdat groep 5 mijn blauwdruk heeft gezien en mij op zijn best hebben gemaakt.")
    speak_normal_nl("Door mijn rupsbanden kan ik heel goed over de bak met grind rijden, dit is één van mijn beste eigenschappen.")
    speak_normal_nl("Hierdoor ben ik zo goed gemaakt dat verliezen geen optie is. WALL-A: Wil om Altijd Lekker Lui te winnen, Altijd.")
    speak_normal_nl("Wil je nog meer van mijn goede eigenschappen zien? Kom dan eens langs in lokaal F2060, dan zal groep 5 alles over mij vertellen.")
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
	espeak(text, "dutch+f4", 40, 90);
}
//says something  in the happy voice
void TextToSpeach::speak_happy_nl(std::string text) {
	espeak(text, "dutch+f4", 99, 90);
}
//says something  in the angry voice
void TextToSpeach::speak_angry_nl(std::string text) {
	espeak(text, "dutch+f4", 0, 90);
}

void TextToSpeach::speak_quite(std::string text, int speed) {
	std::string cmd = ("espeak \"" + text + "\" -v " + "en-german" + " -a0 -s" + std::to_string(speed));
	char* ccmd = convertstrtochar(cmd);
	system(ccmd);
	delete(ccmd);

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