#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int sbox[256] =  
/* 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F	*/
{0x63 ,0x7c ,0x77 ,0x7b ,0xf2 ,0x6b ,0x6f ,0xc5 ,0x30 ,0x01 ,0x67 ,0x2b ,0xfe ,0xd7 ,0xab ,0x76 // 0
	,0xca ,0x82 ,0xc9 ,0x7d ,0xfa ,0x59 ,0x47 ,0xf0 ,0xad ,0xd4 ,0xa2 ,0xaf ,0x9c ,0xa4 ,0x72 ,0xc0 // 1
		,0xb7 ,0xfd ,0x93 ,0x26 ,0x36 ,0x3f ,0xf7 ,0xcc ,0x34 ,0xa5 ,0xe5 ,0xf1 ,0x71 ,0xd8 ,0x31 ,0x15 // 2
		,0x04 ,0xc7 ,0x23 ,0xc3 ,0x18 ,0x96 ,0x05 ,0x9a ,0x07 ,0x12 ,0x80 ,0xe2 ,0xeb ,0x27 ,0xb2 ,0x75 // 3
		,0x09 ,0x83 ,0x2c ,0x1a ,0x1b ,0x6e ,0x5a ,0xa0 ,0x52 ,0x3b ,0xd6 ,0xb3 ,0x29 ,0xe3 ,0x2f ,0x84 // 4
		,0x53 ,0xd1 ,0x00 ,0xed ,0x20 ,0xfc ,0xb1 ,0x5b ,0x6a ,0xcb ,0xbe ,0x39 ,0x4a ,0x4c ,0x58 ,0xcf // 5
		,0xd0 ,0xef ,0xaa ,0xfb ,0x43 ,0x4d ,0x33 ,0x85 ,0x45 ,0xf9 ,0x02 ,0x7f ,0x50 ,0x3c ,0x9f ,0xa8 // 6
		,0x51 ,0xa3 ,0x40 ,0x8f ,0x92 ,0x9d ,0x38 ,0xf5 ,0xbc ,0xb6 ,0xda ,0x21 ,0x10 ,0xff ,0xf3 ,0xd2 // 7
		,0xcd ,0x0c ,0x13 ,0xec ,0x5f ,0x97 ,0x44 ,0x17 ,0xc4 ,0xa7 ,0x7e ,0x3d ,0x64 ,0x5d ,0x19 ,0x73 // 8
		,0x60 ,0x81 ,0x4f ,0xdc ,0x22 ,0x2a ,0x90 ,0x88 ,0x46 ,0xee ,0xb8 ,0x14 ,0xde ,0x5e ,0x0b ,0xdb // 9
		,0xe0 ,0x32 ,0x3a ,0x0a ,0x49 ,0x06 ,0x24 ,0x5c ,0xc2 ,0xd3 ,0xac ,0x62 ,0x91 ,0x95 ,0xe4 ,0x79 // A
		,0xe7 ,0xc8 ,0x37 ,0x6d ,0x8d ,0xd5 ,0x4e ,0xa9 ,0x6c ,0x56 ,0xf4 ,0xea ,0x65 ,0x7a ,0xae ,0x08 // B
		,0xba ,0x78 ,0x25 ,0x2e ,0x1c ,0xa6 ,0xb4 ,0xc6 ,0xe8 ,0xdd ,0x74 ,0x1f ,0x4b ,0xbd ,0x8b ,0x8a // C
		,0x70 ,0x3e ,0xb5 ,0x66 ,0x48 ,0x03 ,0xf6 ,0x0e ,0x61 ,0x35 ,0x57 ,0xb9 ,0x86 ,0xc1 ,0x1d ,0x9e // D
		,0xe1 ,0xf8 ,0x98 ,0x11 ,0x69 ,0xd9 ,0x8e ,0x94 ,0x9b ,0x1e ,0x87 ,0xe9 ,0xce ,0x55 ,0x28 ,0xdf // E
		,0x8c ,0xa1 ,0x89 ,0x0d ,0xbf ,0xe6 ,0x42 ,0x68 ,0x41 ,0x99 ,0x2d ,0x0f ,0xb0 ,0x54 ,0xbb ,0x16}; // F

int mixColArray[4][4] = {
	{2, 3, 1, 1},
	{1, 2, 3, 1},
	{1, 1, 2, 3},
	{3, 1, 1, 2}
};

uint8_t invmixColArray[4][4] = {
	{0xE, 0xB, 0xD, 0x9},
	{0x9, 0xE, 0xB, 0xD},
	{0xD, 0x9, 0xE, 0xB},
	{0xB, 0xD, 0x9, 0xE}
};

int Rcon[10] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

int *gbox(int **words, int word, int rcon)
{
	int *temp = malloc(4 * sizeof(int));

#if 0
	temp = words[word][0];
	words[word][0] = words[word][1];
	words[word][1] = words[word][2];
	words[word][2] = words[word][3];
	words[word][3] = temp;
#endif
	temp[0] = words[word][1];
	temp[1] = words[word][2];
	temp[2] = words[word][3];
	temp[3] = words[word][0];

	for (int i = 0; i < 4; i++)
		temp[i] = sbox[temp[i]];

	for (int i = 0; i < 4; i++){
		if (i == 0)
			temp[i] = temp[i] ^ Rcon[rcon];
	}
	return temp;
}

void KeyExpansion(short int *key, int **words, int num_words)
{
	int word, byte, count=0, rcon = 0;
	int *gbox_op, gbox_flag = 0;
	int temp_gbox_op[4];

	for (word = 0; word < 4; word++)
		for (byte = 0; byte < 4; byte++)
			words[word][byte] = key[count++];

	for (word = 4; word < num_words; word++){
		// Call g-box here 
		if (word % 4 == 0){
			gbox_op = gbox(words, word-1, rcon++);
			gbox_flag = 1;
			// Just copying the data from Dynamically allocated returned memory to local int array and freeing it.
			memcpy(&temp_gbox_op, gbox_op, 4*sizeof(int));
			free(gbox_op);
		}

		for (byte = 0; byte < 4; byte++){
			if (gbox_flag == 1)
				words[word][byte] = words[word-4][byte] ^ temp_gbox_op[byte];
			else
				words[word][byte] = words[word-4][byte] ^ words[word-1][byte];
		}
		gbox_flag = 0;
	}
}

uint8_t** AddRoundKey(uint8_t text[][4], uint8_t subkey[][4])
{
	int row, col;
	uint8_t **resultText = (uint8_t**)malloc(4* sizeof(uint8_t));

	for (row = 0; row < 4; row++)
		resultText[row] = (uint8_t *)malloc(4 * sizeof(uint8_t));

	for (row = 0; row < 4; row++){
		for (col = 0; col < 4; col++){
			printf("subkey[%d][%d]: %X\ttext[%d][%d]: %X\n", row, col, subkey[row][col], row, col, text[row][col]);
			resultText[row][col] = subkey[row][col] ^ text[row][col];
		}
	}
	return resultText;
}

uint8_t **SubBytes(uint8_t **text)
{
	uint8_t **RowCol = malloc(4);
	for (int i = 0; i < 4; i++)
		RowCol[i] = malloc(4);

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			RowCol[i][j] = sbox[text[i][j]];

//	free(text);
	return RowCol;
}

uint8_t **ShiftRows(uint8_t **RowCol)
{
	int row, col, temp;
	int nr = 4, nc = 4;

	for (row = 3; row >= 0; row--){
		if (row != 0)
		{
			for (col = row; col > 0; col--){
				temp = RowCol[row][0];
				RowCol[row][0] = RowCol[row][1];
				RowCol[row][1] = RowCol[row][2];
				RowCol[row][2] = RowCol[row][3];
				RowCol[row][3] = temp;
			}
		}
	}
	return RowCol;
}

uint8_t MixColumns(uint8_t **RowCol, uint8_t MixCol[][4])
{
	int row, col, nr = 4, nc = 4, k =0;

	printf("MixCol\n"); 
	for (row = 0; row < nr; row++)
	{
		for (col = 0; col < nc; col++)
		{
			MixCol[row][col] = (mixColArray[row][0] * RowCol[0][col]) ^
				(mixColArray[row][1] * RowCol[1][col]) ^
				(mixColArray[row][2] * RowCol[2][col]) ^
				(mixColArray[row][3] * RowCol[3][col]);
			printf("%X\t",MixCol[row][col]); 
		}
	printf("\n");
	}
	printf("\n");

	for (row = 0; row < nr; row++)
	{
		for (col = 0; col < nc; col++)
		{
			MixCol[row][col] = (invmixColArray[row][0] * RowCol[0][col]) ^
				(invmixColArray[row][1] * RowCol[1][col]) ^
				(invmixColArray[row][2] * RowCol[2][col]) ^
				(invmixColArray[row][3] * RowCol[3][col]);
			printf("%X\t",MixCol[row][col]); 
		}
	printf("\n");
	}

	free(RowCol);
}

void ConvRowstoCols(short int *textDec, uint8_t DecText[][4])
{
#if 1
	int count = 0;
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			DecText[j][i] = textDec[count++];
		}
	}
#endif
	return;
}

void ConvRowtoColKey(int **expKeys, uint8_t DecKey[][4][4], int num_rounds)
{
	for (int round = 0; round <= num_rounds; round++){
		for (int word = 0; word < 4; word++){
			for (int byte = 0; byte < 4; byte++){
				//				DecKey[round][word*4+byte] = expKeys[byte+4*round][word];
				DecKey[round][word][byte] = expKeys[byte+4*round][word];
				//				expKeys[round*4+word][byte] = expKeys[byte][round*4+word];
				//				expKeys[byte][round*4+word] = temp;
			}
		}
	}
#if 0
	for (int round = 0; round <= num_rounds; round++){
		for (int word = 0; word < 4; word++)
			for (int byte = 0; byte < 4; byte++)
				printf("DecKey[%d][%d][%d]: %X\n", round, word, byte, DecKey[round][word][byte]);
	}
#endif
}

int main(void)
{
	int byte, word;
	//	char key[] = "0123456789ABCDEF";
	char key[] = "Thats my Kung Fu";
	//	char text[] = "Hi venky how are";
	char text[] = "Two One Nine Two";
	short int keysDec[16], textDec[16];
	uint8_t **resultText;
	int num_rounds, num_words;
	uint8_t DecText[4][4] = {0};
	//	uint8_t DecKeys[11][16];
	uint8_t DecKeys[11][4][4];

	int keyLen = strlen(key);
	if (keyLen == 16)
		num_rounds = 10;
	else if (keyLen == 24)
		num_rounds = 12;
	else if (keyLen == 32)
		num_rounds = 14;

	printf("Number of rounds: %d\n", num_rounds);
	num_words = (num_rounds * 4) + 4;
	int *expKeys[num_words];

	for (word = 0; word < num_words; word++)
		expKeys[word] = (int*)malloc(4 * sizeof(char));

	for (int i = 0; i < 16; i++){
		keysDec[i] = (int)(key[i]);
		textDec[i] = (int)(text[i]);
	}
	ConvRowstoCols(textDec, DecText);
#if 0
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			printf("DecText[%d][%d]: %X\n", i, j, DecText[i]);
		}
	}
#endif
	// 1. Key expansion from 4 words(i.e 16 bytes) to 44 words(in case of 128-bit key).
	KeyExpansion(keysDec, expKeys, num_words);
	ConvRowtoColKey(expKeys, DecKeys, num_rounds);

	// 2. Initial Add Round Key function
	resultText = AddRoundKey(DecText, DecKeys[0]);

	// 3. Round Operations depends on Num of Rounds calculated based on Key length
	uint8_t **intBytes;
	uint8_t MixCol[4][4];
//	for (int round = 0; round < num_rounds; round++)
	for (int round = 0; round < 1; round++)
	{
		intBytes = SubBytes(resultText);
		intBytes = ShiftRows(intBytes);
		for (int row = 0; row < 4; row++)
			for (int col = 0; col < 4; col++)
				printf("intBytes[%d][%d]: %X\n", row, col, intBytes[row][col]);
		if (round != (num_rounds-1))
			MixColumns(intBytes, MixCol);

#if 0
		resultText = AddRoundKey(MixCol, DecKeys[round+1]);
#endif
	}
}
