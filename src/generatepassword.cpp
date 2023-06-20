#include "generatepassword.h"
#include <random>

GeneratePassword::GeneratePassword(Choice mode)
	:modeGenerate(mode)
{
	srand(time(NULL));
	passwordSize = rand() % 10 + 10;
}

GeneratePassword::GeneratePassword(Choice mode, std::string specialSymbols)
	:modeGenerate(mode)
{
	srand(time(NULL));
	exceptionSpecial = specialSymbols;
	passwordSize = rand() % 10 + 10;
}

std::string GeneratePassword::generate()
{
	srand(time(NULL));

	password.clear();

	int randomNumbersSimbols = 3;
	std::string specialSimbols = special;
	if(modeGenerate == 273 || modeGenerate == 256)
	{
		randomNumbersSimbols = 4;
		if(modeGenerate == 256)
		{
			for(int j = 0; j < exceptionSpecial.length(); j++)
			{
				int i = 0; int k = 0;
				while(i < specialSimbols.length()-1)
				{
					if(exceptionSpecial[j] == specialSimbols[i])
					{
						k++;
					}
					specialSimbols[i] = specialSimbols[k];
					i++;
					k++;
				}
				specialSimbols.pop_back();
			}
		}
	}

	for(int i = 0; i < passwordSize; i++){
		int rn = rand() % randomNumbersSimbols + 1;
		switch (rn) {
		case 1:{
			int ltrn = rand() % (letters.length()-1);
			password += letters[ltrn];
			break;
		}

		case 2:{
			int ltuprn = rand() % (upLetters.length()-1);
			password += upLetters[ltuprn];
			break;
		}
		case 3:{
			int numrn = rand() % (numbers.length()-1);
			password += numbers[numrn];
			break;
		}
		case 4:{
			int numrn = rand() % (specialSimbols.length()-1);
			password += specialSimbols[numrn];
			break;
		}
		}
	}

	return password;
}
