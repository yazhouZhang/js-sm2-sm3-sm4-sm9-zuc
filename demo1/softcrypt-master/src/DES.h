// DES.h: interface for the DES class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_DES_H__B57DD3C3_FEB0_4306_B5B6_73FDC8246526__INCLUDED_)
#define AFX_DES_H__B57DD3C3_FEB0_4306_B5B6_73FDC8246526__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <string.h>

class DES  
{
private:
	unsigned char KEYBLOCK[17],DATABLOCK[17],ANSBLOCK[17];
	int  KEY[64];	/* 0,1 */
	int  BUFFER[64],BUFOUT[64];	/* 0,1 */
	int  KWORK[56];			/* 0,1 */
	int  WORKA[48],KN[48];		/* 0,1 */
	char OPTION;
	int  TEMP1,TEMP2;		/* 0,1 */
	int  NBROFSHIFT,VALINDEX;


private:
	void MAINPROCESS();
	bool CHSTR2BCH(char& ch);
	char CHBCH2STR(char rch);
public:
	DES();
	virtual ~DES();
	bool STR2BCH(char* strSource, char* pszTarget);
	void SingleDES(char *source, char *dest, char *in_key, char flag);
	void TripleDES(char *source, char *dest, char *in_key, char flag);

	bool CalDES(char* pszSource, char* pszDest, char* pszKey, char flag);
	bool CalX99MAC(char* source, char* dest, int length, char* in_key);
	bool CalMAC(char* source, char* dest, int length, char* in_key);
	bool CalDesMac(char* source, char* dest, int length, char* in_key);
	bool CalFixLenMac(char* source, char* dest, int length, char* in_key);
	bool ANSIX99(char* pszMacKey, char* pszSource, int nLength, char* pszDest);
	void CalANSI919MAC(char *dest, char *source, int nlength, char *in_key);
	void CalMACFORQDCCB(char *dest,char *source,int length, const char *in_key);    //add by yanli 2015-5-18
	void _DES(char *key, char *text,  char *mtext);
	void starexpand(char *in,char *out);
	void setkeystar(char *bits);
	void starcompress(char *out,  char *in);
	void starencrypt( char *text,  char *mtext);
	void starson(char *,char *,char *);
	void starip( char *,char *,char *);
	void _starip(char *,char *,char *);
	void starF(int n,char *ll,char *rr,char *LL,char *RR);
	void stars_box(char *aa,char *bb);
private:
	char m_atcC[56];
	char m_atcD[56];
	char m_atcK[17][48];
};

#endif // !defined(AFX_DES_H__B57DD3C3_FEB0_4306_B5B6_73FDC8246526__INCLUDED_)
