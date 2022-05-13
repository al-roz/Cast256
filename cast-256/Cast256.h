#pragma once
#include"Sbox1_4.h"
#include<bitset>
#include<vector>
#include<iostream>

using byte = unsigned char;
const uint64_t mod2_32 = 4294967296;

class Cast256
{
private:
	std::bitset<32> A;
	std::bitset<32> B;
	std::bitset<32> C;
	std::bitset<32> D;

	std::bitset<32> Key_A;
	std::bitset<32> Key_B;
	std::bitset<32> Key_C;
	std::bitset<32> Key_D;
	std::bitset<32> Key_E;
	std::bitset<32> Key_F;
	std::bitset<32> Key_G;
	std::bitset<32> Key_H;
	std::vector <std::vector <uint8_t>> Kr, Tr;
	std::vector <std::vector <uint32_t>> Km, Tm;

	void BlockBitsetToByte(std::bitset<32>);

	uint32_t ROL(uint32_t, uint8_t);

	std::bitset<32> F1(std::bitset<32>, uint32_t KmI, uint8_t KrI);
	std::bitset<32> F2(std::bitset<32>, uint32_t KmI, uint8_t KrI);
	std::bitset<32> F3(std::bitset<32>, uint32_t KmI, uint8_t KrI);

	void W(const uint8_t);

	void Q(const uint8_t&);
	void QBar(const uint8_t&);

	std::vector<uint8_t> kr();
	std::vector<uint32_t> km();

	void CpyKeyOnMas(const byte _Key[], byte _Mas[]);
	std::bitset<32> StrToBitset(std::string _Block);
	void Bitset128ToFour32Bitset(byte _Line[],
		std::bitset<32>& , std::bitset<32>&, std::bitset<32>&, std::bitset<32>&);
	void KeySet();


public:
	Cast256(byte _Line[], byte _Key[]);
	void encryption();
	void decryption();
};

