#pragma once
class CSM4Alg
{
public:
	CSM4Alg();
	virtual ~CSM4Alg();
	bool SM4MACGenerated(char* szData, char* szSM4Key, char* szResult);
	bool SM4CryptECB(char* szData, short sCryptFlag, char* szSM4Key, char* szResult);
private:
	void HEXStrXOR(char* str1, char* str2);
};
