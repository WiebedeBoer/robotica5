#include "TextToSpeach.h"



TextToSpeach::TextToSpeach()
{
}


TextToSpeach::~TextToSpeach()
{
}
//base function to make the espeak command
void TextToSpeach::espeak(std::string text, std::string lang, int pitch, int speed) {
	std::string cmd = ("espeak \"" + text + "\" -v " + lang + " -s" + std::to_string(speed) + " -p" + std::to_string(pitch));
	char* ccmd = convertstrtochar(cmd);
	std::future<void> r = std::async(std::launch::async, [ccmd] {system(ccmd); });

	delete(ccmd);
}

//gives the pitch
void TextToSpeach::give_pitch() {
	speak_quite("a", 1200);
	speak_normal("Initializing. Three. Two. One.");
	speak_quite("a", 1200);
	speak_normal_nl("Welkom allemaal, mijn naam is vast al bekend");
	speak_quite("a", 1200);
	speak_normal("Initializing...3...2...1");
	speak_quite("a", 1200);
	speak_normal_nl("Welkom allemaal bij deze wall-I ficatie. Mijn naam is Wall-a. De beste robot die hier vandaag staat!");
	speak_quite("a", 1200);
	speak_normal_nl("Mijn projectgroep is groep 5. Zij hebben uit de allerdiepste lade in de werkplaats mijn blauwdruk gevonden");
	speak_quite("a", 1200);
	speak_normal_nl("Misschien zie ik er wel bekend uit en heb je mijn blauwdruk al gezien.");
	speak_quite("a", 1200);
	speak_normal_nl("Ik ben de voorloper van de bekende Wall-I robot. Ik ben prototype alfa");
	speak_quite("a", 1200);
	speak_normal_nl("Na het zien van de blauwdruk heeft groep 5 besloten dat ik de beste robot zou zijn voor de wedstrijd.");
	speak_quite("a", 1200);
	speak_normal_nl("Zij hebben mij in de korte tijd proberen op te bouwen.");
	speak_quite("a", 1200);
	speak_normal_nl("Eieren rapen en blauwe blokjes volgen kan ik heel erg goed. Bij kippen blijf ik liever uit de buurt want die vind ik een beetje eng");
	speak_quite("a", 1200);
	speak_normal_nl("Ik luister ook graag naar muziek en kan heel erg goed dansen wanneer mijn favoriete nummer wordt afgespeeld."); // veranderen mijn ogen van kleur
	speak_quite("a", 1200);
	speak_normal_nl("Door mijn rupsbanden kan ik heel goed over de bak met grind rijden, dit is één van mijn beste eigenschappen.");
	speak_quite("a", 1200);
	speak_normal_nl("Ik ga winnen omdat ik niet geprogrammeerd ben om te valen.");
	speak_quite("a", 1200);
	speak_normal_nl("Wall-a. Wil om Altijd Lekker Lui te winnen, Altijd.");
	speak_quite("a", 1200);
	speak_normal_nl("Wil je nog meer van mijn goede eigenschappen zien");
	speak_quite("a", 5000);
	speak_normal_nl("Kom dan eens langs in lokaal F2060 dan zal groep 5 alles over mij vertellen");
	speak_quite("a", 1200);
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
	espeak(text, "dutch+f4", 40, 120);
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
	std::string cmd = ("espeak \"" + text + "\" -v " + "dutch" + " -a0 -s" + std::to_string(speed));
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
	int r = (rand() % 15) + 1;

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
	case 11:
		speak_normal("oh geez rick");
		break;
	case 12:
		speak_normal("I'll be back!");
		break;

	case 13:
		speak_normal("ROGER ROGER");
		break;
	case 14:
		speak_normal("Kiss my metal shiny ass!");
		break;
	case 15:
		speak_normal("Paniek?! Paniek?! Paniek?!");
		break;
	case 16:
		speak_normal("80085");
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