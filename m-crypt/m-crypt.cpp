#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

const int ALPHABET_SIZE = 23;
const int ERROR = -1;

struct encryptionKey
{
	char decryptedLetter, encryptedLetter;
};

void initEncryptionKeys(encryptionKey* keys)
{
	char a = 'a';
	for (int i = 0; i < ALPHABET_SIZE && a <= 'z'; i++, a++)
	{
		if (a != 'q' && a != 'v' && a != 'x')
			keys[i].decryptedLetter = a;
		else
			i--;
	}
	return;
}

int findInEncryptionKeys(const encryptionKey* keys, char decryptedLetter)
{
	for (int i = 0; i < ALPHABET_SIZE; i++)
		if (keys[i].decryptedLetter == tolower(decryptedLetter))
			return i;
	return ERROR;
}

void generateEncryptionKeys(encryptionKey* keys)
{
	char a;
	for(int i = 0; i < ALPHABET_SIZE; i++)
	{
		do
		{
			a = (rand() % 26) + 97;
			for (int q = 0; q < i; q++)
				if (a == keys[q].encryptedLetter)
					a = 'q';
		} while (a == keys[i].decryptedLetter || a == 'q' || a == 'v' || a == 'x');
		keys[i].encryptedLetter = a;
	}
	return;
}

void sortEncryptionKeysAlphbeticalOrder(encryptionKey* keys)
{
	encryptionKey temp;
	for (int i = 0; i < ALPHABET_SIZE; i++)
		for (int q = i; q < ALPHABET_SIZE; q++)
		{
			if (keys[q].encryptedLetter < keys[i].encryptedLetter)
			{
				temp = keys[i];
				keys[i] = keys[q];
				keys[q] = temp;
			}
		}
	return;
}

void saveEncryptionKeys(const encryptionKey* keys)
{
	fstream outputStream;
	char* fileName = "key.txt";
	outputStream.open(fileName, ios::out);
	
	for (int i = 0; i < ALPHABET_SIZE; i++)
		outputStream << keys[i].decryptedLetter << " " << keys[i].encryptedLetter << "\n";
	
	outputStream.close();
	return;
}

void encrypt(const char* fileName, const encryptionKey* keys)
{
	fstream inputStream, outputStream;
	string data;
	char* outputFile = "zaszyfrowane.txt";
	int id;

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
			if (data[i] == toupper(keys[id].decryptedLetter))
				data[i] = toupper(keys[id].encryptedLetter);
			else
				data[i] = keys[id].encryptedLetter;
		}
		outputStream << data << " ";
	}
	inputStream.close();
	outputStream.close();

	return;

}

int main(int argc, char** argv)
{
	encryptionKey keys[ALPHABET_SIZE];

	if (argc != 2)
		return 0;

	srand(time(NULL));
	initEncryptionKeys(keys);
	generateEncryptionKeys(keys); 
	sortEncryptionKeysAlphbeticalOrder(keys);
	saveEncryptionKeys(keys);
	encrypt(argv[1], keys);
	
	return 0;
}