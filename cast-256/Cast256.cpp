#include "Cast256.h"


uint32_t Cast256::ROL(uint32_t Number, uint8_t Repetitions)
{
    Repetitions = Repetitions % 32;
    int tmp = Number >> Repetitions;
    int tmp2 = Number << Repetitions;
    Number = tmp | tmp2;
    return Number;
}

void Cast256::BlockBitsetToByte(std::bitset<32> _Block)
{
    std::bitset<4> CharBitset;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 4; j++)
            CharBitset[j] = _Block[j + i * 4];
        int tmp = CharBitset.to_ullong();
        std::cout << tmp << " ";
    }
}


std::bitset<32> Cast256::F1(std::bitset<32> Block, uint32_t KmI, uint8_t KrI)
{
    std::bitset<32> TMP(ROL(Block.to_ulong() + KmI, KrI));

    std::bitset <8> Ia(0);
    std::bitset <8> Ib(0);
    std::bitset <8> Ic(0);
    std::bitset <8> Id(0);

    for (int i = 0; i < 8; i++)
    {
        Ia[i] = TMP[24 + i];
        Ib[i] = TMP[16 + i];
        Ic[i] = TMP[8 + i];
        Id[i] = TMP[i];
    }
    unsigned long long tmp = Sbox::S1[Ia.to_ullong()] ^ Sbox::S2[Ib.to_ullong()];
    tmp = tmp - Sbox::S3[Ic.to_ullong()];
    tmp = tmp + Sbox::S4[Id.to_ullong()];
    tmp = tmp % mod2_32;
    std::bitset<32> res(tmp);
    return res;
}

std::bitset<32> Cast256::F2(std::bitset<32> Block, uint32_t KmI, uint8_t KrI)
{
    std::bitset<32> TMP(ROL(Block.to_ulong() + KmI, KrI));

    std::bitset <8> Ia(0);
    std::bitset <8> Ib(0);
    std::bitset <8> Ic(0);
    std::bitset <8> Id(0);

    for (int i = 0; i < 8; i++)
    {
        Ia[i] = TMP[24 + i];
        Ib[i] = TMP[16 + i];
        Ic[i] = TMP[8 + i];
        Id[i] = TMP[i];
    }

    unsigned long long tmp = Sbox::S1[Ia.to_ulong()] - Sbox::S2[Ib.to_ulong()];
    tmp = tmp + Sbox::S3[Ic.to_ulong()];
    tmp = tmp % mod2_32;
    tmp = tmp ^ Sbox::S4[Id.to_ulong()];
    tmp = tmp % mod2_32;

    std::bitset<32> res(tmp);

    return res;
}

std::bitset<32> Cast256::F3(std::bitset<32> Block, uint32_t KmI, uint8_t KrI)
{
    std::bitset<32> TMP(ROL(Block.to_ulong() + KmI, KrI));

    std::bitset <8> Ia(0);
    std::bitset <8> Ib(0);
    std::bitset <8> Ic(0);
    std::bitset <8> Id(0);

    for (int i = 0; i < 8; i++)
    {
        Ia[i] = TMP[24 + i];
        Ib[i] = TMP[16 + i];
        Ic[i] = TMP[8 + i];
        Id[i] = TMP[i];
    }

    unsigned long long tmp = Sbox::S1[Ia.to_ulong()];
    tmp = tmp + Sbox::S2[Ib.to_ulong()];
    tmp = tmp % mod2_32;
    tmp = tmp ^ Sbox::S3[Ic.to_ulong()];
    tmp = tmp - Sbox::S4[Id.to_ulong()];
    tmp = tmp % mod2_32;

    std::bitset<32> res(tmp);
    return res;
}

void Cast256::W(const uint8_t i)
{
    Key_G ^= F1(Key_H, Tm[0][i], Tr[0][i]);
    Key_F ^= F2(Key_G, Tm[1][i], Tr[1][i]);
    Key_E ^= F3(Key_F, Tm[2][i], Tr[2][i]);
    Key_D ^= F1(Key_E, Tm[3][i], Tr[3][i]);
    Key_C ^= F2(Key_D, Tm[4][i], Tr[4][i]);
    Key_B ^= F3(Key_C, Tm[5][i], Tr[5][i]);
    Key_A ^= F1(Key_B, Tm[6][i], Tr[6][i]);
    Key_H ^= F2(Key_A, Tm[7][i], Tr[7][i]);

}

void Cast256::Q(const uint8_t& i)
{
    C ^= F1(D, Km[i][0], Kr[i][0]);
    B ^= F2(C, Km[i][1], Kr[i][1]);
    A ^= F3(B, Km[i][2], Kr[i][2]);
    D ^= F1(A, Km[i][3], Kr[i][3]);
}

void Cast256::QBar(const uint8_t& i)
{
    D ^= F1(A, Km[i][3], Kr[i][3]);
    A ^= F3(B, Km[i][2], Kr[i][2]);
    B ^= F2(C, Km[i][1], Kr[i][1]);
    C ^= F1(D, Km[i][0], Kr[i][0]);

}

std::vector<uint8_t> Cast256::kr()
{
    std::vector<uint8_t> res;

    res.push_back(Key_A.to_ulong() % 32);
    res.push_back(Key_C.to_ulong() % 32);
    res.push_back(Key_E.to_ulong() % 32);
    res.push_back(Key_G.to_ulong() % 32);

    return res;
}

std::vector<uint32_t> Cast256::km()
{
    std::vector<uint32_t> res;

    res.push_back(Key_H.to_ulong());
    res.push_back(Key_F.to_ulong());
    res.push_back(Key_D.to_ulong());
    res.push_back(Key_B.to_ulong());

    return res;
}

void Cast256::CpyKeyOnMas(const byte _Key[], byte _Mas[])
{
    for (int i = 0; i < 16; i++)
    {
        _Mas[i] = _Key[i];
    }
}


std::bitset<32> Cast256::StrToBitset(std::string _Block)
{
    std::string convert;

    for (auto i : _Block)
    {
        std::bitset<8> tmp(i);
        for (auto j : tmp.to_string())
            convert.push_back(j);
    }
    std::bitset<32> result(convert);

    return result;
}

void Cast256::Bitset128ToFour32Bitset(byte _Line[], std::bitset<32>& _FirstBitset,
	std::bitset<32>& _SecondBitset, std::bitset<32>& _ThirdBitset, std::bitset<32>& _FourthBitset)
{

    for (int i = 0; i < 4; i++)
    {
        std::string Tmp = "";
        for (int j = 0; j < 4; j++)
        {
            char x = _Line[j + i * 4];
            Tmp += x;
        }

        std::bitset<32> TmpBitsetAfterConver = StrToBitset(Tmp);

        switch (i)
        {
            case 0:
            {
                _FirstBitset = TmpBitsetAfterConver;
                break;
            }
            case 1:
            {
                _SecondBitset = TmpBitsetAfterConver;
                break;
            }
            case 2:
            {
                _ThirdBitset = TmpBitsetAfterConver;
                break;
            }
            case 3:
            {
                _FourthBitset = TmpBitsetAfterConver;
                break;
            }
        }
    }
}

void Cast256::KeySet()
{
    long long Cm = 0x5A827999, Mm = 0x6ED9EBA1, Cr = 19, Mr = 17;

    std::vector <uint8_t> tmp(24, 0);
    std::vector <uint32_t> tmp2(24, 0);

    Tm.push_back(tmp2); Tr.push_back(tmp);

    for (uint8_t x = 0; x < 8; x++) {
        Tm.push_back(tmp2);
        Tr.push_back(tmp);
    }

    for (uint8_t i = 0; i < 24; i++) {
        for (uint8_t j = 0; j < 8; j++) {
            Tm[j][i] = Cm;
            Cm = (Cm + Mm) % mod2_32;
            Tr[j][i] = Cr;
            Cr = (Cr + Mr) % 32;
        }
    }

    for (uint8_t i = 0; i < 12; i++) {
        W(i << 1);
        W((i << 1) + 1);
        Kr.push_back(kr());
        Km.push_back(km());
    }
}

Cast256::Cast256(byte _Line[], byte _Key[])
{
    Bitset128ToFour32Bitset(_Line, A, B, C, D);
    
    byte ABCDKeyBitset[16];
    CpyKeyOnMas(_Key, ABCDKeyBitset);
    Bitset128ToFour32Bitset(ABCDKeyBitset, Key_A, Key_B, Key_C, Key_D);

    byte EFGHKeyBitset[16];
    CpyKeyOnMas(_Key + 16, EFGHKeyBitset);
    Bitset128ToFour32Bitset(EFGHKeyBitset, Key_E, Key_F, Key_G, Key_H);

    KeySet();
}

void Cast256::encryption()
{
    

    for (uint8_t i = 0; i < 6; i++)
        Q(i);
    for (uint8_t i = 6; i < 12; i++)
        QBar(i);   

    BlockBitsetToByte(A);
    std::cout << std::endl;

    BlockBitsetToByte(B);
    std::cout << std::endl;

    BlockBitsetToByte(C);
    std::cout << std::endl;

    BlockBitsetToByte(D);
    std::cout << std::endl;
}

void Cast256::decryption()
{
    KeySet();
    std::reverse(Kr.begin(), Kr.end());
    std::reverse(Km.begin(), Km.end());

    for (uint8_t i = 0; i < 6; i++)
        Q(i);
    for (uint8_t i = 6; i < 12; i++)
        QBar(i);

    BlockBitsetToByte(A);
    std::cout << std::endl;

    BlockBitsetToByte(B);
    std::cout << std::endl;

    BlockBitsetToByte(C);
    std::cout << std::endl;

    BlockBitsetToByte(D);
    std::cout << std::endl;
}
