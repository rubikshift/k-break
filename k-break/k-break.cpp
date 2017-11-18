#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

const int ALPHABET_SIZE = 23;
const int ERROR = -1;

struct encryptionKey
{
	char decryptedLetter, encryptedLetter;
};

struct encryptedCharacter
{
	unsigned long long int counter;
	char encryptedLetter;
};

void initEncryptionKeys(encryptionKey* keys)
{
	keys[0].decryptedLetter = 'a';
	keys[1].decryptedLetter = 'e';
	keys[2].decryptedLetter = 'o';
	keys[3].decryptedLetter = 'i';
	keys[4].decryptedLetter = 'z';
	keys[5].decryptedLetter = 'n';
	keys[6].decryptedLetter = 's';
	keys[7].decryptedLetter = 'r';
	keys[8].decryptedLetter = 'w';
	keys[9].decryptedLetter = 'c';
	keys[10].decryptedLetter = 't';
	keys[11].decryptedLetter = 'l';
	keys[12].decryptedLetter = 'y';
	keys[13].decryptedLetter = 'k';
	keys[14].decryptedLetter = 'd';
	keys[15].decryptedLetter = 'p';
	keys[16].decryptedLetter = 'm';
	keys[17].decryptedLetter = 'u';
	keys[18].decryptedLetter = 'j';
	keys[19].decryptedLetter = 'b';
	keys[20].decryptedLetter = 'g';
	keys[21].decryptedLetter = 'h';
	keys[22].decryptedLetter = 'f';
	return;
}

void initEncryptedCharacters(encryptedCharacter* characters)
{
	char a = 'a';
	for (int i = 0; i < ALPHABET_SIZE && a <= 'z'; i++, a++)
	{
		if (a != 'q' && a != 'v' && a != 'x')
		{
			characters[i].encryptedLetter = a;
			characters[i].counter = 0;
		}
		else
			i--;
	}
	return;
}

void sortEncryptedCharacters(encryptedCharacter* characters)
{
	encryptedCharacter temp;

	for(int i = 0; i < ALPHABET_SIZE; i++)
		for (int q = 0; q < ALPHABET_SIZE; q++)
		{
			if (characters[q].counter < characters[i].counter)
			{
				temp = characters[i];
				characters[i] = characters[q];
				characters[q] = temp;
			}
		}
	return;
}

int findInEncryptedCharacters(const encryptedCharacter* characters, char encryptedLetter)
{
	for (int i = 0; i < ALPHABET_SIZE; i++)
		if (characters[i].encryptedLetter == tolower(encryptedLetter))
			return i;
	return ERROR;
}

int findInEncryptionKeys(const encryptionKey* keys, char encryptedLetter)
{
	for (int i = 0; i < ALPHABET_SIZE; i++)
		if (keys[i].encryptedLetter == tolower(encryptedLetter))
			return i;
	return ERROR;
}

void analyze(unsigned long long int* totalCounter, const char* fileName, encryptedCharacter* characters)
{
	fstream inputStream;
	string data;
	int id;

	inputStream.open(fileName, ios::in);

	while (!inputStream.eof() && inputStream.good())
	{
		inputStream >> data;
		for(int i = 0; i < data.length(); i++)
		{
			id = findInEncryptedCharacters(characters, data[i]);
			if (id == ERROR)
				continue;
			characters[id].counter++;
			(*totalCounter)++;
		}
	}
	inputStream.close();
	return;
}

void updateEncryptionKeys(encryptedCharacter* characters, encryptionKey* keys)
{
	for (int i = 0; i < ALPHABET_SIZE; i++)
		keys[i].encryptedLetter = characters[i].encryptedLetter;
	return;
}

void sortEncryptionKeysAlphbeticalOrder(encryptionKey* keys)
{
	encryptionKey temp;
	for (int i = 0; i < ALPHABET_SIZE; i++)
		for (int q = i; q < ALPHABET_SIZE; q++)
		{
			if (keys[q].encryptedLetter > keys[i].encryptedLetter)
			{
				temp = keys[i];
				keys[i] = keys[q];
				keys[q] = temp;
			}
		}
	return;
}

void sortEncryptedCharactersAlphbeticalOrder(encryptedCharacter* characters)
{
	encryptedCharacter temp;
	for (int i = 0; i < ALPHABET_SIZE; i++)
		for (int q = i; q < ALPHABET_SIZE; q++)
		{
			if (characters[q].encryptedLetter < characters[i].encryptedLetter)
			{
				temp = characters[i];
				characters[i] = characters[q];
				characters[q] = temp;
			}
		}
	return;
}

void printAnalyzedData(const encryptedCharacter* characters, const unsigned long long int* totalCounter)
{
	double result, fractionalPart;
	int count;

	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		result = ((double)characters[i].counter / (double)*totalCounter) * 100;
		count = (int)result;
		fractionalPart = result - (double)count;
		if (fractionalPart >= 0.5)
			count++;

		cout << "\t" << characters[i].encryptedLetter << ": ";
		for (int q = 0; q < count; q++)
			cout << "#";
		cout << " " << setprecision(3) << result << "%" << endl;
	}
}

void printEncryptionKeys(const encryptionKey* keys)
{
	for (int i = ALPHABET_SIZE - 1; i >= 0; i--)
		cout << "\t" << keys[i].encryptedLetter << " => " << keys[i].decryptedLetter << endl;
	return;
}

void modifyEncryptionKeys(encryptionKey* keys)
{
	char newEncryptedLetter, newDecryptedLetter, temp;
	int id;

	cout << "Znak w zaszyfrowanym pliku: ";
	cin >> newEncryptedLetter;
	cout << "Znak w odszyfrowanym pliku: ";
	cin >> newDecryptedLetter;

	id = findInEncryptionKeys(keys, newEncryptedLetter);
	temp = keys[id].decryptedLetter;
	for (int i = 0; i < ALPHABET_SIZE; i++)
		if (keys[i].decryptedLetter == newDecryptedLetter)
			keys[i].decryptedLetter = temp;
	keys[id].decryptedLetter = newDecryptedLetter;
}

void decrypt(const char* fileName, const encryptionKey* keys)
{
	fstream inputStream, outputStream;
	string data;
	int id;
	char* outputFile = "output.txt";
	
	inputStream.open(fileName, ios::in);
	outputStream.open(outputFile, ios::out);

	while (!inputStream.eof() && inputStream.good())
	{
		inputStream >> data;
		for (int i = 0; i < data.length(); i++)
		{
			id = findInEncryptionKeys(keys, data[i]);
			if (id == ERROR)
				continue;
			if (data[i] == toupper(keys[id].encryptedLetter))
				data[i] = toupper(keys[id].decryptedLetter);
			else
				data[i] = keys[id].decryptedLetter;
		}
		outputStream << data << " ";
	}
	inputStream.close();
	outputStream.close();

	return;
}

int main(int argc, char** argv)
{
	unsigned long long int totalCounter = 0;
	encryptionKey keys[ALPHABET_SIZE];
	encryptedCharacter characters[ALPHABET_SIZE];
	bool ok = false;
	char choice;

	if (argc != 2)
		return 0;

	initEncryptionKeys(keys);
	initEncryptedCharacters(characters);

	analyze(&totalCounter, argv[1], characters);

	sortEncryptedCharacters(characters);
	updateEncryptionKeys(characters, keys);
	sortEncryptionKeysAlphbeticalOrder(keys);
	sortEncryptedCharactersAlphbeticalOrder(characters);

	while (!ok)
	{
		system("cls");

		cout << "Histogram:" << endl;
		printAnalyzedData(characters, &totalCounter);
		cout << "------------------------------" << endl;
		cout << "Tablica podstawien:\nZNAK W ZASZYFROWANYM PLIKU => ZNAK W ODSZYFROWANYM PLIKU" << endl;
		printEncryptionKeys(keys);
		
		cout << "Czy chcesz zmodyfikowac tablice podstawien?\n\tT - TAK, N - NIE" << endl;
		cout << "Wybor: ";
		cin >> choice;

		switch (choice)
		{
			case 't':
			case 'T': 
				modifyEncryptionKeys(keys); 
				sortEncryptionKeysAlphbeticalOrder(keys); break;
			case 'n':
			case 'N': 
				ok = true; break;
			default:
				continue; break;
		}
	}
	decrypt(argv[1], keys);
	return 0;
}