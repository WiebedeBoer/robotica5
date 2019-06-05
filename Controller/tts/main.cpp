#include <stdio.h>
#include <bits/stdc++.h>
#include "tts.h"

int main(int argc, char **argv)
{
	tts speach = tts();
	
	//system("espeak \"test espeak in c++\"");
	//tts("something witty", "en-us", 99, 60);
	//std::string str= "I am wall-e and i live on a planet with eve";
	//speak_normal(str);	
	//speak_happy(str);
	//speak_angry("I am wall-e and i am your father");
	//tts("a s d f g t h j h gf r d e s w q a w s ed f r g h j hg f d s w a s e  d f gh j k j h g f d s w d f g h j h g f d e f g h j h g f d r f g 45454 345363456453643564536", "en-german+f4", 40, 30);
	//tts("some numbers 10 100 10000 12345 12 132 12341234567    1234567888889 1234567890 54545456565  12343234443554444", "en-german+f4", 40, 50);
	//speak_normal("Hello, my name is wall-A");
	speach.give_pitch();
	speach.shutdown();
	speach.error();

	speach.hello();
	speach.wall_a();
	speach.give_pitch();
	speach.good_bye();
	speach.self_destruct();

}

