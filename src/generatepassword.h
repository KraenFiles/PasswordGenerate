#ifndef GENERATEPASSWORD_H
#define GENERATEPASSWORD_H

#include<string>

class GeneratePassword
{
public:
	enum Choice{
		LetAndNum = 0x011,
		ExceptionSpecialSymbols = 0x100,
		All = 0x111
	};


	GeneratePassword(Choice mode);
	GeneratePassword(Choice mode, std::string specialSymbols);

	std::string generate();

	inline std::string GetPassword() { return password; }

private:
	Choice modeGenerate;
	std::string letters = "abcdefghijklmnopqrstuvwxyz";
	std::string upLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::string numbers = "0123456789";
	std::string special = "*()!@#_$%&?-+=~";
	std::string exceptionSpecial = "";

	int passwordSize = 10;
	std::string password = "";
};

#endif // GENERATEPASSWORD_H
