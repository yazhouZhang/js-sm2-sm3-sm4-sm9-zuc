// DES.cpp: implementation of the DES class.
//
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "DES.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static int starip_tab[64]={
	58,50,42,34,26,18,10,2,
	60,52,44,36,28,20,12,4,
	62,54,46,38,30,22,14,6,
	64,56,48,40,32,24,16,8,
	57,49,41,33,25,17,9,1,
	59,51,43,35,27,19,11,3,
	61,53,45,37,29,21,13,5,
	63,55,47,39,31,23,15,7};

static int _starip_tab[64]={
    40,8,48,16,56,24,64,32,
	39,7,47,15,55,23,63,31,
	38,6,46,14,54,22,62,30,
	37,5,45,13,53,21,61,29,
	36,4,44,12,52,20,60,28,
	35,3,43,11,51,19,59,27,
	34,2,42,10,50,18,58,26,
	33,1,41,9,49,17,57,25};

static int starpc_1_c[28]={
	56,48,40,32,24,16,8
	,0,57,49,41,33,25,17
	,9,1,58,50,42,34,26
	,18,10,2,59,51,43,35};
	
static int starpc_1_d[28]={
	62,54,46,38,30,22,14
	,6,61,53,45,37,29,21
	,13,5,60,52,44,36,28
	,20,12,4,27,19,11,3};
		
static int starpc_2[48]={
	14,17,11,24,1,5,
	3,28,15,6,21,10,
	23,19,12,4,26,8,
	16,7,27,20,13,2,
	41,52,31,37,47,55,
	30,40,51,45,33,48,
	44,49,39,56,34,53,
	46,42,50,36,29,32};
			
static int starls_count[16]={
	1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

static int stare_r[48]={
	32,1,2,3,4,5,4,5,6,7,8,9,
	8,9,10,11,12,13,12,13,14,15,16,17,
	16,17,18,19,20,21,20,21,22,23,24,25,
	24,25,26,27,28,29,28,29,30,31,32,1};

static int starP[32]={
	16,7,20,21,29,12,28,17,
	1,15,23,26,5,18,31,10,
	2,8,24,14,32,27,3,9,
	19,13,30,6,22,11,4,25};

	static int starSSS[16][4][16]={
		14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
		0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
		4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
		15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13,
			
		15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
		3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
		0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
		13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9,
			
		10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
		13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
		13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
		1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12,
			
		7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
		13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
		10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
		3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14,
			
		2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
		14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
		4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
		11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3,
			
		12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
		10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
		9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
		4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13,
			
		4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
		13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
		1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
		6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12,
		
		13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
		1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
		7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
		2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11};
		
static char bit_box[16][4]={
	{0,0,0,0},
	{0,0,0,1},
	{0,0,1,0},
	{0,0,1,1},
	{0,1,0,0},
	{0,1,0,1},
	{0,1,1,0},
	{0,1,1,1},
	{1,0,0,0},
	{1,0,0,1},
	{1,0,1,0},
	{1,0,1,1},
	{1,1,0,0},
	{1,1,0,1},
	{1,1,1,0},
    {1,1,1,1}};

static int S1[4][16]={
	{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
	{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
	{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
	{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}};	

static int S2[4][16]={
	{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
	{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
	{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
	{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}};

static int S3[4][16]={
	{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
	{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
	{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
	{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}};

static int S4[4][16]={
	{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
	{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
	{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
	{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}};

static int S5[4][16]={
	{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
	{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
	{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
	{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}};

static int S6[4][16]={
	{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
	{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
	{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
	{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}};

static int S7[4][16]={
	{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
	{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
	{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
	{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}};

static int S8[4][16]={
	{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
	{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
	{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
	{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}};

static int SHIFT[16]={1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

static char BINARY[64]={
	0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,1
    ,0,1,0,0,0,1,0,1,0,1,1,0,0,1,1,1
    ,1,0,0,0,1,0,0,1,1,0,1,0,1,0,1,1 
    ,1,1,0,0,1,1,0,1,1,1,1,0,1,1,1,1};

DES::DES()
{
	memset(m_atcC, 0X00, sizeof(m_atcC));
	memset(m_atcD, 0X00, sizeof(m_atcD));
	memset(m_atcK, 0X00, sizeof(m_atcK));
}

DES::~DES()
{

}

void DES::MAINPROCESS()
{
	int i,j,k;
	int iter;
	
	for(i=0;i<16;i++) {
		j = i*4;
		switch(KEYBLOCK[i]){
	          case '0' : 	KEY[j+0]=0;
				KEY[j+1]=0;
				KEY[j+2]=0;
				KEY[j+3]=0;
				break;
	          case '1' :	KEY[j+0]=0;
				KEY[j+1]=0;
				KEY[j+2]=0;
				KEY[j+3]=1;
				break;
		  case '2' :	KEY[j+0]=0;
				KEY[j+1]=0;
				KEY[j+2]=1;
				KEY[j+3]=0;
				break;
		  case	'3' :	KEY[j+0]=0;
				KEY[j+1]=0;
				KEY[j+2]=1;
				KEY[j+3]=1;
				break;
	          case	'4' :	KEY[j+0]=0;
				KEY[j+1]=1;
				KEY[j+2]=0;
				KEY[j+3]=0;
				break;
		  case '5' :	KEY[j+0]=0;
				KEY[j+1]=1;
				KEY[j+2]=0;
				KEY[j+3]=1;
				break;
		 case	'6' :	KEY[j+0]=0;
				KEY[j+1]=1;
				KEY[j+2]=1;
				KEY[j+3]=0;
				break;
           	  case  '7'  :  KEY[j+0]=0;
                     		KEY[j+1]=1;
                     		KEY[j+2]=1;
                     		KEY[j+3]=1;
				break;
        	   case	'8'  :  KEY[j+0]=1;
                     		KEY[j+1]=0;
                     		KEY[j+2]=0;

				break;
           	   case '9'  :  KEY[j+0]=1;
                     		KEY[j+1]=0;
                     		KEY[j+2]=0;
                     		KEY[j+3]=1;
				break;
           	case	'a'  :
                case	'A'  :
                         	KEY[j+0]=1;
                     		KEY[j+1]=0;
                     		KEY[j+2]=1;
                     		KEY[j+3]=0;
                   		break;
           	case	'b'  :
                case	'B'  :
                                KEY[j+0]=1;
                     		KEY[j+1]=0;
                     		KEY[j+2]=1;
                     		KEY[j+3]=1;
                   		break;
           	case	'C'  :
                case	'c'  :
                                KEY[j+0]=1;
                     		KEY[j+1]=1;
                     		KEY[j+2]=0;
                     		KEY[j+3]=0;
                   		break;
           	case	'D'  :
                case	'd'  :
                                KEY[j+0]=1;
                     		KEY[j+1]=1;
                     		KEY[j+2]=0;
                     		KEY[j+3]=1;
                   		break;
           	case	'E'  :
                case	'e'  :
                                KEY[j+0]=1;
                     		KEY[j+1]=1;
                     		KEY[j+2]=1;
                     		KEY[j+3]=0;
                   		break;
           	case	'F'  :
                case	'f'  :
                                KEY[j+0]=1;
                     		KEY[j+1]=1;
                     		KEY[j+2]=1;
                     		KEY[j+3]=1;
                   		break;
		}/* switch */
	}/* for */

	for(i=0;i<16;i++){
		j = i*4;
		switch(DATABLOCK[i]){
		case	'0' : 	BUFFER[j+0]=0;
				BUFFER[j+1]=0;
				BUFFER[j+2]=0;
				BUFFER[j+3]=0;
				break;
		case	'1' :	BUFFER[j+0]=0;
				BUFFER[j+1]=0;
				BUFFER[j+2]=0;
				BUFFER[j+3]=1;
				break;
		case	'2' :	BUFFER[j+0]=0;
				BUFFER[j+1]=0;
				BUFFER[j+2]=1;
				BUFFER[j+3]=0;
				break;
		case	'3' :	BUFFER[j+0]=0;
				BUFFER[j+1]=0;
				BUFFER[j+2]=1;
				BUFFER[j+3]=1;
				break;
		case	'4' :	BUFFER[j+0]=0;
				BUFFER[j+1]=1;
				BUFFER[j+2]=0;
				BUFFER[j+3]=0;
				break;
		case	'5' :	BUFFER[j+0]=0;
				BUFFER[j+1]=1;
				BUFFER[j+2]=0;
				BUFFER[j+3]=1;
				break;
		case	'6' :	BUFFER[j+0]=0;
				BUFFER[j+1]=1;
				BUFFER[j+2]=1;
				BUFFER[j+3]=0;
				break;
           	case	'7'  :  BUFFER[j+0]=0;
                     		BUFFER[j+1]=1;
                     		BUFFER[j+2]=1;
                     		BUFFER[j+3]=1;
				break;
        	case   	'8'  :  BUFFER[j+0]=1;
                     		BUFFER[j+1]=0;
                     		BUFFER[j+2]=0;
                     		BUFFER[j+3]=0;
				break;
           	case	'9'  :  BUFFER[j+0]=1;
                     		BUFFER[j+1]=0;
                     		BUFFER[j+2]=0;
                     		BUFFER[j+3]=1;
				break;
           	case	'A'  :
                case	'a'  :
                                BUFFER[j+0]=1;
                     		BUFFER[j+1]=0;
                     		BUFFER[j+2]=1;
                     		BUFFER[j+3]=0;
                   		break;
           	case	'B'  :
                case	'b'  :
                                BUFFER[j+0]=1;
                     		BUFFER[j+1]=0;
                     		BUFFER[j+2]=1;
                     		BUFFER[j+3]=1;
                   		break;
           	case	'C'  :
            case	'c'  :
                                BUFFER[j+0]=1;
                     		BUFFER[j+1]=1;
                     		BUFFER[j+2]=0;
                     		BUFFER[j+3]=0;
                   		break;
           	case	'D'  :
                case	'd'  :
                                BUFFER[j+0]=1;
                     		BUFFER[j+1]=1;
                     		BUFFER[j+2]=0;
                     		BUFFER[j+3]=1;
                   		break;
           	case	'E'  :
                case	'e'  :
                                BUFFER[j+0]=1;
                     		BUFFER[j+1]=1;
                     		BUFFER[j+2]=1;
                     		BUFFER[j+3]=0;
                   		break;
           	case	'F'  :
                case	'f'  :
                                BUFFER[j+0]=1;
                     		BUFFER[j+1]=1;
                     		BUFFER[j+2]=1;
                     		BUFFER[j+3]=1;
                   		break;
			
		}/* end switch */
	}/* for */

  /* INITIAL PERMUTATION OF DATA */
  BUFOUT [  0 ] = BUFFER [  57 ];
  BUFOUT [  1 ] = BUFFER [  49 ];
  BUFOUT [  2 ] = BUFFER [  41 ];
  BUFOUT [  3 ] = BUFFER [  33 ];
  BUFOUT [  4 ] = BUFFER [  25 ];
  BUFOUT [  5 ] = BUFFER [  17 ];
  BUFOUT [  6 ] = BUFFER [   9 ];
  BUFOUT [  7 ] = BUFFER [   1 ];
  BUFOUT [  8 ] = BUFFER [  59 ];
  BUFOUT [  9 ] = BUFFER [  51 ];
  BUFOUT [ 10 ] = BUFFER [  43 ];
  BUFOUT [ 11 ] = BUFFER [  35 ];
  BUFOUT [ 12 ] = BUFFER [  27 ];
  BUFOUT [ 13 ] = BUFFER [  19 ];
  BUFOUT [ 14 ] = BUFFER [  11 ];
  BUFOUT [ 15 ] = BUFFER [   3 ];
  BUFOUT [ 16 ] = BUFFER [  61 ];
  BUFOUT [ 17 ] = BUFFER [  53 ];
  BUFOUT [ 18 ] = BUFFER [  45 ];
  BUFOUT [ 19 ] = BUFFER [  37 ];
  BUFOUT [ 20 ] = BUFFER [  29 ];
  BUFOUT [ 21 ] = BUFFER [  21 ];
  BUFOUT [ 22 ] = BUFFER [  13 ];
  BUFOUT [ 23 ] = BUFFER [   5 ];
  BUFOUT [ 24 ] = BUFFER [  63 ];
  BUFOUT [ 25 ] = BUFFER [  55 ];
  BUFOUT [ 26 ] = BUFFER [  47 ];
  BUFOUT [ 27 ] = BUFFER [  39 ];
  BUFOUT [ 28 ] = BUFFER [  31 ];
  BUFOUT [ 29 ] = BUFFER [  23 ];
  BUFOUT [ 30 ] = BUFFER [  15 ];
  BUFOUT [ 31 ] = BUFFER [   7 ];
  BUFOUT [ 32 ] = BUFFER [  56 ];
  BUFOUT [ 33 ] = BUFFER [  48 ];
  BUFOUT [ 34 ] = BUFFER [  40 ];
  BUFOUT [ 35 ] = BUFFER [  32 ];
  BUFOUT [ 36 ] = BUFFER [  24 ];
  BUFOUT [ 37 ] = BUFFER [  16 ];
  BUFOUT [ 38 ] = BUFFER [   8 ];
  BUFOUT [ 39 ] = BUFFER [   0 ];
  BUFOUT [ 40 ] = BUFFER [  58 ];
  BUFOUT [ 41 ] = BUFFER [  50 ];
  BUFOUT [ 42 ] = BUFFER [  42 ];
  BUFOUT [ 43 ] = BUFFER [  34 ];
  BUFOUT [ 44 ] = BUFFER [  26 ];
  BUFOUT [ 45 ] = BUFFER [  18 ];
  BUFOUT [ 46 ] = BUFFER [  10 ];
  BUFOUT [ 47 ] = BUFFER [   2 ];
  BUFOUT [ 48 ] = BUFFER [  60 ];
  BUFOUT [ 49 ] = BUFFER [  52 ];
  BUFOUT [ 50 ] = BUFFER [  44 ];
  BUFOUT [ 51 ] = BUFFER [  36 ];
  BUFOUT [ 52 ] = BUFFER [  28 ];
  BUFOUT [ 53 ] = BUFFER [  20 ];
  BUFOUT [ 54 ] = BUFFER [  12 ];
  BUFOUT [ 55 ] = BUFFER [   4 ];
  BUFOUT [ 56 ] = BUFFER [  62 ];
  BUFOUT [ 57 ] = BUFFER [  54 ];
  BUFOUT [ 58 ] = BUFFER [  46 ];
  BUFOUT [ 59 ] = BUFFER [  38 ];
  BUFOUT [ 60 ] = BUFFER [  30 ];
  BUFOUT [ 61 ] = BUFFER [  22 ];
  BUFOUT [ 62 ] = BUFFER [  14 ];
  BUFOUT [ 63 ] = BUFFER [   6 ];
 
  /* INITIAL PERMUTATION OF KEY */
  KWORK [  0 ] = KEY[ 56 ];
  KWORK [  1 ] = KEY[ 48 ];
  KWORK [  2 ] = KEY[ 40 ];
  KWORK [  3 ] = KEY[ 32 ];
  KWORK [  4 ] = KEY[ 24 ];
  KWORK [  5 ] = KEY[ 16 ];
  KWORK [  6 ] = KEY[  8 ];
  KWORK [  7 ] = KEY[  0 ];
  KWORK [  8 ] = KEY[ 57 ];
  KWORK [  9 ] = KEY[ 49 ];
  KWORK [ 10 ] = KEY[ 41 ];
  KWORK [ 11 ] = KEY[ 33 ];
  KWORK [ 12 ] = KEY[ 25 ];
  KWORK [ 13 ] = KEY[ 17 ];
  KWORK [ 14 ] = KEY[  9 ];
  KWORK [ 15 ] = KEY[  1 ];
  KWORK [ 16 ] = KEY[ 58 ];
  KWORK [ 17 ] = KEY[ 50 ];
  KWORK [ 18 ] = KEY[ 42 ];
  KWORK [ 19 ] = KEY[ 34 ];
  KWORK [ 20 ] = KEY[ 26 ];
  KWORK [ 21 ] = KEY[ 18 ];
  KWORK [ 22 ] = KEY[ 10 ];
  KWORK [ 23 ] = KEY[  2 ];
  KWORK [ 24 ] = KEY[ 59 ];
  KWORK [ 25 ] = KEY[ 51 ];
  KWORK [ 26 ] = KEY[ 43 ];
  KWORK [ 27 ] = KEY[ 35 ];
  KWORK [ 28 ] = KEY[ 62 ];
  KWORK [ 29 ] = KEY[ 54 ];
  KWORK [ 30 ] = KEY[ 46 ];
  KWORK [ 31 ] = KEY[ 38 ];
  KWORK [ 32 ] = KEY[ 30 ];
  KWORK [ 33 ] = KEY[ 22 ];
  KWORK [ 34 ] = KEY[ 14 ];
  KWORK [ 35 ] = KEY[  6 ];
  KWORK [ 36 ] = KEY[ 61 ];
  KWORK [ 37 ] = KEY[ 53 ];
  KWORK [ 38 ] = KEY[ 45 ];
  KWORK [ 39 ] = KEY[ 37 ];
  KWORK [ 40 ] = KEY[ 29 ];
  KWORK [ 41 ] = KEY[ 21 ];
  KWORK [ 42 ] = KEY[ 13 ];
  KWORK [ 43 ] = KEY[  5 ];
  KWORK [ 44 ] = KEY[ 60 ];
  KWORK [ 45 ] = KEY[ 52 ];
  KWORK [ 46 ] = KEY[ 44 ];
  KWORK [ 47 ] = KEY[ 36 ];
  KWORK [ 48 ] = KEY[ 28 ];
  KWORK [ 49 ] = KEY[ 20 ];
  KWORK [ 50 ] = KEY[ 12 ];
  KWORK [ 51 ] = KEY[  4 ];
  KWORK [ 52 ] = KEY[ 27 ];
  KWORK [ 53 ] = KEY[ 19 ];
  KWORK [ 54 ] = KEY[ 11 ];
  KWORK [ 55 ] = KEY[ 3  ];

	for(iter=0;iter<16;iter++){
		for(i=0;i<32;i++)
			BUFFER[i] = BUFOUT[32+i];

     		/* CALCULATION OF F(R,K)  */
      		/* PERMUTE - E */
      		WORKA[  0 ] = BUFFER[ 31 ];
      		WORKA[  1 ] = BUFFER[  0 ];
      		WORKA[  2 ] = BUFFER[  1 ];
      		WORKA[  3 ] = BUFFER[  2 ];
      		WORKA[  4 ] = BUFFER[  3 ];
     		WORKA[  5 ] = BUFFER[  4 ];
      		WORKA[  6 ] = BUFFER[  3 ];
      		WORKA[  7 ] = BUFFER[  4 ];
      		WORKA[  8 ] = BUFFER[  5 ];
      		WORKA[  9 ] = BUFFER[  6 ];
      		WORKA[ 10 ] = BUFFER[  7 ];
      		WORKA[ 11 ] = BUFFER[  8 ];
      		WORKA[ 12 ] = BUFFER[  7 ];
      		WORKA[ 13 ] = BUFFER[  8 ];
      		WORKA[ 14 ] = BUFFER[  9 ];
     	 	WORKA[ 15 ] = BUFFER[ 10 ];
     	 	WORKA[ 16 ] = BUFFER[ 11 ];
      		WORKA[ 17 ] = BUFFER[ 12 ];
      		WORKA[ 18 ] = BUFFER[ 11 ];
      		WORKA[ 19 ] = BUFFER[ 12 ];
      		WORKA[ 20 ] = BUFFER[ 13 ];
      		WORKA[ 21 ] = BUFFER[ 14 ];
     		WORKA[ 22 ] = BUFFER[ 15 ];
     		WORKA[ 23 ] = BUFFER[ 16 ];
      		WORKA[ 24 ] = BUFFER[ 15 ];
      		WORKA[ 25 ] = BUFFER[ 16 ];
      		WORKA[ 26 ] = BUFFER[ 17 ];
      		WORKA[ 27 ] = BUFFER[ 18 ];
      		WORKA[ 28 ] = BUFFER[ 19 ];
      		WORKA[ 29 ] = BUFFER[ 20 ];
      		WORKA[ 30 ] = BUFFER[ 19 ];
      		WORKA[ 31 ] = BUFFER[ 20 ];
      		WORKA[ 32 ] = BUFFER[ 21 ];
      		WORKA[ 33 ] = BUFFER[ 22 ];
      		WORKA[ 34 ] = BUFFER[ 23 ];
      		WORKA[ 35 ] = BUFFER[ 24 ];
      		WORKA[ 36 ] = BUFFER[ 23 ];
      		WORKA[ 37 ] = BUFFER[ 24 ];
      		WORKA[ 38 ] = BUFFER[ 25 ];
      		WORKA[ 39 ] = BUFFER[ 26 ];
      		WORKA[ 40 ] = BUFFER[ 27 ];
      		WORKA[ 41 ] = BUFFER[ 28 ];
      		WORKA[ 42 ] = BUFFER[ 27 ];
      		WORKA[ 43 ] = BUFFER[ 28 ];
      		WORKA[ 44 ] = BUFFER[ 29 ];
      		WORKA[ 45 ] = BUFFER[ 30 ];
      		WORKA[ 46 ] = BUFFER[ 31 ];
      		WORKA[ 47 ] = BUFFER[  0 ];

		/* KS FUNCTION BEGIN */
		if (OPTION == 'E'){
			NBROFSHIFT = SHIFT[iter];
			for(i=0;i<NBROFSHIFT;i++){
				TEMP1 = KWORK[0];
				TEMP2 = KWORK[28];
				for (j=0;j<27;j++){
					KWORK[j]=KWORK[j+1];
					KWORK[j+28]=KWORK[j+29];
				}
				KWORK[27]=TEMP1;
				KWORK[55]=TEMP2;
			}
		}
		else 	if(iter>0){
				NBROFSHIFT=SHIFT[16-iter];
				for(i=0;i<NBROFSHIFT;i++){
					TEMP1 = KWORK[27];
					TEMP2 = KWORK[55];
					for(j=27;j>0;j--){
						KWORK[j]=KWORK[j-1];
						KWORK[j+28]=KWORK[j+27];
					}
					KWORK[0]=TEMP1;
					KWORK[28]=TEMP2;
				}
			}	
		
      /* PERMUTE KWORK - PC2 */
      	KN [  0 ] = KWORK[ 13 ];
      	KN [  1 ] = KWORK[ 16 ];
      	KN [  2 ] = KWORK[ 10 ];
      	KN [  3 ] = KWORK[ 23 ];
      	KN [  4 ] = KWORK[  0 ];
      	KN [  5 ] = KWORK[  4 ];
      	KN [  6 ] = KWORK[  2 ];
      	KN [  7 ] = KWORK[ 27 ];
      	KN [  8 ] = KWORK[ 14 ];
      	KN [  9 ] = KWORK[  5 ];
      	KN [ 10 ] = KWORK[ 20 ];
      	KN [ 11 ] = KWORK[ 9  ];
      	KN [ 12 ] = KWORK[ 22 ];
      	KN [ 13 ] = KWORK[ 18 ];
      	KN [ 14 ] = KWORK[ 11 ];
      	KN [ 15 ] = KWORK[  3 ];
      	KN [ 16 ] = KWORK[ 25 ];
      	KN [ 17 ] = KWORK[  7 ];
      	KN [ 18 ] = KWORK[ 15 ];
      	KN [ 19 ] = KWORK[  6 ];
      	KN [ 20 ] = KWORK[ 26 ];
      	KN [ 21 ] = KWORK[ 19 ];
      	KN [ 22 ] = KWORK[ 12 ];
      	KN [ 23 ] = KWORK[ 1  ];
      	KN [ 24 ] = KWORK[ 40 ];
      	KN [ 25 ] = KWORK[ 51 ];
      	KN [ 26 ] = KWORK[ 30 ];
      	KN [ 27 ] = KWORK[ 36 ];
      	KN [ 28 ] = KWORK[ 46 ];
      	KN [ 29 ] = KWORK[ 54 ];
      	KN [ 30 ] = KWORK[ 29 ];
      	KN [ 31 ] = KWORK[ 39 ];
      	KN [ 32 ] = KWORK[ 50 ];
      	KN [ 33 ] = KWORK[ 44 ];
      	KN [ 34 ] = KWORK[ 32 ];
      	KN [ 35 ] = KWORK[ 47 ];
      	KN [ 36 ] = KWORK[ 43 ];
      	KN [ 37 ] = KWORK[ 48 ];
      	KN [ 38 ] = KWORK[ 38 ];
      	KN [ 39 ] = KWORK[ 55 ];
      	KN [ 40 ] = KWORK[ 33 ];
      	KN [ 41 ] = KWORK[ 52 ];
      	KN [ 42 ] = KWORK[ 45 ];
      	KN [ 43 ] = KWORK[ 41 ];
      	KN [ 44 ] = KWORK[ 49 ];
      	KN [ 45 ] = KWORK[ 35 ];
      	KN [ 46 ] = KWORK[ 28 ];
      	KN [ 47 ] = KWORK[ 31 ];
		
		for(i=0;i<48;i++)
			WORKA[i]=(WORKA[i]+KN[i]) % 2;
		
		
      VALINDEX = S1[  2 * WORKA[0] + WORKA[5]][ 2 * ( 2 * ( 2 * WORKA[1] + WORKA[2] ) + WORKA[3] ) + WORKA[4] ];
      VALINDEX = VALINDEX * 4;
      KN [ 0 ] = BINARY [ 0 + VALINDEX ];
      KN [ 1 ] = BINARY [ 1 + VALINDEX ];
      KN [ 2 ] = BINARY [ 2 + VALINDEX ];
      KN [ 3 ] = BINARY [ 3 + VALINDEX ];
      VALINDEX = S2[  2 * WORKA[6] + WORKA[11] ][ 2 * ( 2 * ( 2 * WORKA[7] + WORKA[8] ) + WORKA[9] ) + WORKA[10]  ];
      VALINDEX = VALINDEX * 4;
      KN [ 4 ] = BINARY [ 0 + VALINDEX ];
      KN [ 5 ] = BINARY [ 1 + VALINDEX ];
      KN [ 6 ] = BINARY [ 2 + VALINDEX ];
      KN [ 7 ] = BINARY [ 3 + VALINDEX ];
      VALINDEX = S3[  2 * WORKA[12] + WORKA[17] ][ 2 * ( 2 * ( 2 * WORKA[13] + WORKA[14] ) + WORKA[15] ) + WORKA[16]  ];
      VALINDEX = VALINDEX * 4;
      KN [  8 ] = BINARY [ 0 + VALINDEX ];
      KN [  9 ] = BINARY [ 1 + VALINDEX ];
      KN [ 10 ] = BINARY [ 2 + VALINDEX ];
      KN [ 11 ] = BINARY [ 3 + VALINDEX ];
      VALINDEX = S4[  2 * WORKA[18] + WORKA[23] ][ 2 * ( 2 * ( 2 * WORKA[19] + WORKA[20] ) + WORKA[21] ) + WORKA[22]  ];
      VALINDEX = VALINDEX * 4;
      KN [ 12 ] = BINARY [ 0 + VALINDEX ];
      KN [ 13 ] = BINARY [ 1 + VALINDEX ];
      KN [ 14 ] = BINARY [ 2 + VALINDEX ];
      KN [ 15 ] = BINARY [ 3 + VALINDEX ];
      VALINDEX = S5[  2 * WORKA[24] + WORKA[29] ][ 2 * ( 2 * ( 2 * WORKA[25] + WORKA[26] ) + WORKA[27] ) + WORKA[28]  ];
      VALINDEX = VALINDEX * 4;
      KN [ 16 ] = BINARY [ 0 + VALINDEX ];
      KN [ 17 ] = BINARY [ 1 + VALINDEX ];
      KN [ 18 ] = BINARY [ 2 + VALINDEX ];
      KN [ 19 ] = BINARY [ 3 + VALINDEX ];
      VALINDEX = S6[  2 * WORKA[30] + WORKA[35] ][ 2 * ( 2 * ( 2 * WORKA[31] + WORKA[32] ) + WORKA[33] ) + WORKA[34] ];
      VALINDEX = VALINDEX * 4;
      KN [ 20 ] = BINARY [ 0 + VALINDEX ];
      KN [ 21 ] = BINARY [ 1 + VALINDEX ];
      KN [ 22 ] = BINARY [ 2 + VALINDEX ];
      KN [ 23 ] = BINARY [ 3 + VALINDEX ];
      VALINDEX = S7[  2 * WORKA[36] + WORKA[41] ][ 2 * ( 2 * ( 2 * WORKA[37] + WORKA[38] ) + WORKA[39] ) + WORKA[40] ];
      VALINDEX = VALINDEX * 4;
      KN [ 24 ] = BINARY [ 0 + VALINDEX ];
      KN [ 25 ] = BINARY [ 1 + VALINDEX ];
      KN [ 26 ] = BINARY [ 2 + VALINDEX ];
      KN [ 27 ] = BINARY [ 3 + VALINDEX ];
      VALINDEX = S8[  2 * WORKA[42] + WORKA[47]  ][ 2 * ( 2 * ( 2 * WORKA[43] + WORKA[44] ) + WORKA[45] ) + WORKA[46]  ];
      VALINDEX = VALINDEX * 4;
      KN [ 28 ] = BINARY [ 0 + VALINDEX ];
      KN [ 29 ] = BINARY [ 1 + VALINDEX ];
      KN [ 30 ] = BINARY [ 2 + VALINDEX ];
      KN [ 31 ] = BINARY [ 3 + VALINDEX ];
 
      WORKA [  0 ] = KN[ 15 ];
      WORKA [  1 ] = KN[  6 ];
      WORKA [  2 ] = KN[ 19 ];
      WORKA [  3 ] = KN[ 20 ];
      WORKA [  4 ] = KN[ 28 ];
      WORKA [  5 ] = KN[ 11 ];
      WORKA [  6 ] = KN[ 27 ];
      WORKA [  7 ] = KN[ 16 ];
      WORKA [  8 ] = KN[  0 ];
      WORKA [  9 ] = KN[ 14 ];
      WORKA [ 10 ] = KN[ 22 ];
      WORKA [ 11 ] = KN[ 25 ];
      WORKA [ 12 ] = KN[  4 ];
      WORKA [ 13 ] = KN[ 17 ];
      WORKA [ 14 ] = KN[ 30 ];
      WORKA [ 15 ] = KN[  9 ];
      WORKA [ 16 ] = KN[  1 ];
      WORKA [ 17 ] = KN[  7 ];
      WORKA [ 18 ] = KN[ 23 ];
      WORKA [ 19 ] = KN[ 13 ];
      WORKA [ 20 ] = KN[ 31 ];
      WORKA [ 21 ] = KN[ 26 ];
      WORKA [ 22 ] = KN[  2 ];
      WORKA [ 23 ] = KN[  8 ];
      WORKA [ 24 ] = KN[ 18 ];
      WORKA [ 25 ] = KN[ 12 ];
      WORKA [ 26 ] = KN[ 29 ];
      WORKA [ 27 ] = KN[  5 ];
      WORKA [ 28 ] = KN[ 21 ];
      WORKA [ 29 ] = KN[ 10 ];
      WORKA [ 30 ] = KN[  3 ];
      WORKA [ 31 ] = KN[ 24 ];
	
		/* BUFOUT XOR WORKA */
		for (j=0;j<32;j++){
			BUFOUT[j+32]=(BUFOUT[j]+WORKA[j]) % 2;
			BUFOUT[j]=BUFFER[j];
		}
	}/* for iter*/

	for (i=0;i<32;i++)	{
		j=BUFOUT[i];
		BUFOUT[i]=BUFOUT[32+i];
		BUFOUT[32+i]=j;
	}


  BUFFER [  0 ] = BUFOUT[ 39 ];
  BUFFER [  1 ] = BUFOUT[  7 ];
  BUFFER [  2 ] = BUFOUT[ 47 ];
  BUFFER [  3 ] = BUFOUT[ 15 ];
  BUFFER [  4 ] = BUFOUT[ 55 ];
  BUFFER [  5 ] = BUFOUT[ 23 ];
  BUFFER [  6 ] = BUFOUT[ 63 ];
  BUFFER [  7 ] = BUFOUT[ 31 ];
  BUFFER [  8 ] = BUFOUT[ 38 ];
  BUFFER [  9 ] = BUFOUT[  6 ];
  BUFFER [ 10 ] = BUFOUT[ 46];
  BUFFER [ 11 ] = BUFOUT[ 14 ];
  BUFFER [ 12 ] = BUFOUT[ 54 ];
  BUFFER [ 13 ] = BUFOUT[ 22 ];
  BUFFER [ 14 ] = BUFOUT[ 62 ];
  BUFFER [ 15 ] = BUFOUT[ 30 ];
  BUFFER [ 16 ] = BUFOUT[ 37 ];
  BUFFER [ 17 ] = BUFOUT[  5 ];
  BUFFER [ 18 ] = BUFOUT[ 45 ];
  BUFFER [ 19 ] = BUFOUT[ 13 ];
  BUFFER [ 20 ] = BUFOUT[ 53 ];
  BUFFER [ 21 ] = BUFOUT[ 21 ];
  BUFFER [ 22 ] = BUFOUT[ 61 ];
  BUFFER [ 23 ] = BUFOUT[ 29 ];
  BUFFER [ 24 ] = BUFOUT[ 36 ];
  BUFFER [ 25 ] = BUFOUT[  4 ];
  BUFFER [ 26 ] = BUFOUT[ 44 ];
  BUFFER [ 27 ] = BUFOUT[ 12 ];
  BUFFER [ 28 ] = BUFOUT[ 52 ];
  BUFFER [ 29 ] = BUFOUT[ 20 ];
  BUFFER [ 30 ] = BUFOUT[ 60 ];
  BUFFER [ 31 ] = BUFOUT[ 28 ];
  BUFFER [ 32 ] = BUFOUT[ 35 ];
  BUFFER [ 33 ] = BUFOUT[  3 ];
  BUFFER [ 34 ] = BUFOUT[ 43 ];
  BUFFER [ 35 ] = BUFOUT[ 11 ];
  BUFFER [ 36 ] = BUFOUT[ 51 ];
  BUFFER [ 37 ] = BUFOUT[ 19 ];
  BUFFER [ 38 ] = BUFOUT[ 59 ];
  BUFFER [ 39 ] = BUFOUT[ 27 ];
  BUFFER [ 40 ] = BUFOUT[ 34 ];
  BUFFER [ 41 ] = BUFOUT[  2 ];
  BUFFER [ 42 ] = BUFOUT[ 42 ];
  BUFFER [ 43 ] = BUFOUT[ 10 ];
  BUFFER [ 44 ] = BUFOUT[ 50 ];
  BUFFER [ 45 ] = BUFOUT[ 18 ];
  BUFFER [ 46 ] = BUFOUT[ 58 ];
  BUFFER [ 47 ] = BUFOUT[ 26 ];
  BUFFER [ 48 ] = BUFOUT[ 33 ];
  BUFFER [ 49 ] = BUFOUT[  1 ];
  BUFFER [ 50 ] = BUFOUT[ 41 ];
  BUFFER [ 51 ] = BUFOUT[  9 ];
  BUFFER [ 52 ] = BUFOUT[ 49 ];
  BUFFER [ 53 ] = BUFOUT[ 17 ];
  BUFFER [ 54 ] = BUFOUT[ 57 ];
  BUFFER [ 55 ] = BUFOUT[ 25 ];
  BUFFER [ 56 ] = BUFOUT[ 32 ];
  BUFFER [ 57 ] = BUFOUT[  0 ];
  BUFFER [ 58 ] = BUFOUT[ 40 ];
  BUFFER [ 59 ] = BUFOUT[  8 ];
  BUFFER [ 60 ] = BUFOUT[ 48 ];
  BUFFER [ 61 ] = BUFOUT[ 16 ];
  BUFFER [ 62 ] = BUFOUT[ 56 ];
  BUFFER [ 63 ] = BUFOUT[ 24 ];

	j =0;
	for (i=0;i<8;i++){
		ANSBLOCK[i]=0;
		for (k=0;k<=6;k++)
			ANSBLOCK[i]=(ANSBLOCK[i]+BUFFER[j+k]) * 2;
		ANSBLOCK[i]=ANSBLOCK[i]+BUFFER[j+7]; 
		j=j+8;		
	}
}/* end */

void DES::SingleDES(char *source, char *dest, char *in_key, char flag)
{
	int i;
	
	memset(KEYBLOCK,0x00,17);
	memset(DATABLOCK,0x00,17);
	memset(ANSBLOCK,0x00,17);

        for(i=0;i<64;i++)
        {
                KEY[i]=0;	/* 0,1 */
                BUFFER[i]=0,BUFOUT[i]=0;	/* 0,1 */
        }
        for(i=0;i<48;i++)
        {
                WORKA[i]=0;	/* 0,1 */
                KN[i]=0;
        }
        for(i=0;i<56;i++)
        {
                KWORK[i]=0;	/* 0,1 */
        }
        TEMP1=0,TEMP2=0;		/* 0,1 */
        NBROFSHIFT=0,VALINDEX=0;

	
	memcpy(KEYBLOCK,in_key,16);
	KEYBLOCK[16]='\0';
	memcpy(DATABLOCK,source,16);
	DATABLOCK[16]='\0';

	OPTION = flag;
	MAINPROCESS();

	for(i=0;i<8;i++)
	{
		int n;
		unsigned char ch1;
		char ch2[3];

		ch1 = ANSBLOCK[i];
		n = ch1;

		sprintf(ch2, "%02X", ch1);

		sprintf(&dest[2*i],"%02X",ANSBLOCK[i]);
	}
	dest[16]='\0';

}

void DES::TripleDES(char *source, char *dest, char *in_key, char flag)
{
	char keya[17],keyb[17];
	char tmpbuf1[17],tmpbuf2[17];
	
	memset(keya,0x00,sizeof(keya));
	memset(keyb,0x00,sizeof(keyb));
	memset(tmpbuf1,0x00,sizeof(tmpbuf1));
	memset(tmpbuf2,0x00,sizeof(tmpbuf2));
	
	strncpy(keya,in_key,16);
	strncpy(keyb,&in_key[16],16);
	
	if(flag == 'E')
	{
		SingleDES(source,tmpbuf1,keya,'E');
		SingleDES(tmpbuf1,tmpbuf2,keyb,'D');
		SingleDES(tmpbuf2,dest,keya,'E');
	}
	else if(flag == 'D')
	{
		SingleDES(source,tmpbuf1,keya,'D');
		SingleDES(tmpbuf1,tmpbuf2,keyb,'E');
		SingleDES(tmpbuf2,dest,keya,'D');
	}
}

bool DES::CalDES(char* pszSource, char* pszDest, char* pszKey, char flag)
{
		if (pszSource == NULL || pszDest == NULL || pszKey == NULL)
		{
			return false;
		}
		int nKeyLen;
		nKeyLen = strlen(pszKey);
		if (nKeyLen != 16 && nKeyLen != 32)
		{
			return false;
		}
		int nLen, nCount, nCopyLen;
		nLen = strlen(pszSource);
		nCount = (nLen + 15) / 16;
		char szPartSource[17], szPartDest[17];
		memset(szPartSource, 0x00, 17);			//\C8\F4\B3\A4\B6Ȳ\BBΪ16\B5\C4\D5\FB\CA\FD\B1\B6\A3\AC\D4\F2Ĭ\C8\CF\CC\EE\B3\E4'\0'
		memset(szPartDest, 0x00, 17);

		for (int i=0; i<nCount; i++)
		{
			if (i == (nCount-1))
				nCopyLen = nLen - i*16;
			else
				nCopyLen = 16;
			
			memcpy(szPartSource, pszSource+i*16, nCopyLen);
			if (nKeyLen == 16)
			{

				SingleDES(szPartSource, szPartDest, pszKey, flag);
			}
			else
			{
				TripleDES(szPartSource, szPartDest, pszKey, flag);
			}
			strcat(pszDest, szPartDest);
			memset(szPartSource, 0x00, 17);
			memset(szPartDest, 0x00, 17);
		}
		return true;

}

bool DES::CalMAC(char* pszSource, char* pszDest, int nLength, char* pszKey)
{

		if (pszSource == NULL || pszDest == NULL || nLength <=0 || pszKey == NULL)
		{
			return false;
		}
		int nKeyLen;
		nKeyLen = strlen(pszKey);
		if (nKeyLen != 32)
		{
			return false;
		}
		char szKey1[17], szKey2[17];
		memset(szKey1, 0x00, sizeof(szKey1));
		memset(szKey2, 0x00, sizeof(szKey2));
		memcpy(szKey1, pszKey, 16);
		memcpy(szKey2, pszKey+16, 16);

		char szASC[17], szBCD[17];
		memset(szASC, 0x00, sizeof(szASC));
		memset(szBCD, 0x00, sizeof(szBCD));
		
		char szBlock[17];
		memset(szBlock, 0x00, sizeof(szBlock));
		memcpy(szBlock, "0000000000000000", 16);
		//added by weiling ends 2014/03/13
		int nLoopCount, nCopyLen;
//		nLoopCount = (nLength+7)/8;
		nLoopCount = (nLength*2 +15)/16;   //Edited by weiling ends 2014/03/13
		for (int i=0; i<nLoopCount; i++)
		{
			if (i == nLoopCount-1) 
				nCopyLen = nLength - i*8;
			else  
				nCopyLen = 8 ;

//			memcpy(szASC, pszSource+i*8, nCopyLen);
			memcpy(szASC, pszSource+i*16, nCopyLen*2);  //Edited by weiling ends 2014/03/13
			for(int j=0; j<16; j++)
			{	
//				sprintf(&szBCD[2*j],"%02X", szASC[j]);	
				szBCD[j] = szASC[j];           //Edited by weiling ends 2014/03/13
			}
			for (int j=0; j<16; j++)
			{
				char a = szBCD[j], b = szBlock[j];
				CHSTR2BCH(a);
				CHSTR2BCH(b);
				szBCD[j] = CHBCH2STR(a^b);
			}
			memset(szBlock, 0x00, 16);
			SingleDES(szBCD, szBlock, szKey1, 'E');
		
			memset(szASC, 0x00, sizeof(szASC));
			memset(szBCD, 0x00, sizeof(szBCD));
		}

		char szTmp[16];
		memset(szTmp, 0x00, sizeof(szTmp));
		SingleDES(szBlock, szTmp, szKey2, 'D');
		SingleDES(szTmp, pszDest, szKey1, 'E');

		return true;

}

bool DES::CalDesMac(char* pszSource, char* pszDest, int nLength, char* pszKey)
{

		if (pszSource == NULL || pszDest == NULL || nLength <=0 || pszKey == NULL)
		{
			return false;
		}
		int nKeyLen;
		nKeyLen = strlen(pszKey);
		if (nKeyLen != 16)
		{
			return false;
		}
		char szKey[17];
		memset(szKey, 0x00, sizeof(szKey));
		memcpy(szKey, pszKey, 16);
		
		char szASC[9], szBCD[17];
		memset(szASC, 0x00, sizeof(szASC));
		memset(szBCD, 0x00, sizeof(szBCD));
		
		memcpy(pszDest, "0000000000000000", 17);
		int nLoopCount, nCopyLen;
		nLoopCount = (nLength+7)/8;
		for (int i=0; i<nLoopCount; i++)
		{
			if (i == nLoopCount-1) 
				nCopyLen = nLength - i*8;
			else  
				nCopyLen = 8 ;
			
			memcpy(szASC, pszSource+i*8, nCopyLen);
			for(int j=0; j<8; j++)
			{	
				sprintf(&szBCD[2*j],"%02X", szASC[j]);	
			}
			for (int j=0; j<16; j++)
			{
				char a = szBCD[j], b = pszDest[j];
				CHSTR2BCH(a);
				CHSTR2BCH(b);
				szBCD[j] = CHBCH2STR(a^b);
			}
			SingleDES(szBCD, pszDest, szKey, 'E');
			
			memset(szASC, 0x00, sizeof(szASC));
			memset(szBCD, 0x00, sizeof(szBCD));
		}
		return true;

}

bool DES::CalFixLenMac(char* pszSource, char* pszDest, int nLength, char* pszMacKey)
{

		ANSIX99((char *)pszMacKey, pszSource, nLength, pszDest);
		return true;


}


bool DES::CHSTR2BCH(char& ch)
{
	if (ch>=0x30 && ch<=0x39)
	{
		ch -= 0x30;
	}
	else if (ch>=0x41 && ch<=0x47)
	{
		ch -= 0x37;
	}
	else if (ch >= 0x61 && ch<0x67)
	{
		ch -= 0x57;
	}
	else {
		return false;
	}
	return true;
}

char DES::CHBCH2STR(char ch)
{
	if (ch>=0x00 && ch<=0x09)
	{
		ch += 0x30;
	}
	else if (ch>=0x0A && ch<=0x0F)
	{
		ch += 0x37;
	}
	return ch;
}

bool DES::ANSIX99(char* mac_key, char* buf, int len, char* mac)
{

		int 	i,j;
		char 	tmp[20];
		memset(tmp, 0x00, sizeof(tmp));
		
		/* MAC/ECB */
		for (i=0; i<8; i++) mac[i]=0;
		for (i=0; i<len; i+=8) 
		{
			/* right-justified with append 0x00 */
			if ((len-i) < 8) 
			{
				memset(tmp, 0x00, 8);
				memcpy(tmp, buf+i, len-i);
				for (j=0; j<8; j++) mac[j]=mac[j]^tmp[j];
				_DES(mac_key,mac, mac);
			} 
			else 
			{
				for (j=0; j<8; j++) mac[j]=mac[j]^buf[i+j];
				_DES(mac_key,mac, mac);
			}
		}
		return true;

}


void DES::_DES(char *key,char *text, char *mtext)
{
	char tmp[64];
	union {
		char buff1[8];
		unsigned long i[2];
	}a;	
	starexpand(key,tmp);
	setkeystar(tmp);
	starcompress( tmp , a.buff1);
	starencrypt(text,mtext);
}


void DES::starexpand(char *in,char *out)
{
	int i,j;
	
	for (i=0;i<8;i++) {
		for (j=7;j>=0;j--) {
			*out++=(in[i]>>j)&1;
		}
	}
}

void DES::setkeystar(char *bits)
{
	int i,j;
	for (i=0;i<28;i++) {
		m_atcC[i]=m_atcC[i+28]=bits[starpc_1_c[i]];
		m_atcD[i]=m_atcD[i+28]=bits[starpc_1_d[i]];
	}
	for (i=0,j=0;j<16;j++) {
		i+=starls_count[j];
		starson(m_atcC+i,m_atcD+i,m_atcK[j+1]);
	}
}

void DES::starcompress(char *out,char *in)
{
	int i,j;
	
	for (i=0;i<8;i++) {
		for (in[i]=0,j=7;j>=0;j--)
			in[i]+=(*out++)<<j;
	}
}

void DES::starencrypt(char *text, char *mtext)
{
	char ll[64],rr[64],LL[64],RR[64];
	char tmp[64];
	int i; /* ,j;*/
	starip(text,ll,rr);
	
	for (i=1;i<17;i++) {
		starF(i,ll,rr,LL,RR);
		starF(++i,LL,RR,ll,rr);
	}
	_starip(tmp,rr,ll);
	starcompress(tmp,mtext);
}

void DES::starson(char *cc,char *dd,char *kk)
/*char *cc,*dd,*kk;*/
{
	int i;	/*,j;*/
	char buffer[56];
	memcpy(buffer,cc,28);
	memcpy(buffer+28,dd,28);
	for (i=0;i<48;i++)
		*kk++=buffer[starpc_2[i]-1];
}


void DES::starip( char *text,char *ll,char *rr)
{
	int i;	/*,j;*/
	char buffer[64];
	/*char tmp[64];*/
	starexpand(text,buffer);
	
	for (i=0;i<32;i++) {
		ll[i]=buffer[starip_tab[i]-1];
		rr[i]=buffer[starip_tab[i+32]-1];
	}
}

void DES::_starip(char *text,char *ll,char *rr)
{
	int i;	/*,j;*/
	char tmp[64];
	memcpy(tmp,ll,32);
	memcpy(tmp+32,rr,32);
	for (i=0;i<64;i++)
		text[i]=tmp[_starip_tab[i]-1];
}

void DES::starF(int n,char *ll,char *rr,char *LL,char *RR)
{
	int i;	/*,j;*/
	char buffer[64],tmp[64];
	for (i=0;i<48;i++)
		buffer[i]=rr[stare_r[i]-1]^m_atcK[n][i];
	
	stars_box(buffer,tmp);
	
	for (i=0;i<32;i++)
		RR[i]=tmp[starP[i]-1]^ll[i];
	
	memcpy(LL,rr,32);
}

void DES::stars_box(char *aa,char *bb)
{
	int i,j,m,y,z;
	for (i=0,m=0,j=0;i<8;i++,j+=6)
	{
		y=(aa[j]<<1)+aa[j+5];
		z=(aa[j+1]<<3)+(aa[j+2]<<2)+(aa[j+3]<<1)+aa[j+4];
		memcpy(bb+m,bit_box[ starSSS[i][y][z] ],4);
		m+=4;
	}
}


bool DES::CalX99MAC(char* pszSource, char* pszDest, int nLength, char* pszKey)
{

		if (pszSource == NULL || pszDest == NULL || nLength <=0 || pszKey == NULL)
		{
			return false;
		}
		int nKeyLen;
		nKeyLen = strlen(pszKey);
		if (nKeyLen != 16)
		{
			return false;
		}
		char szKey1[17];
		memset(szKey1, 0x00, sizeof(szKey1));
		memcpy(szKey1, pszKey, 16);
		
	//	char szASC[9], szBCD[17];
		char szASC[17], szBCD[17];
		memset(szASC, 0x00, sizeof(szASC));
		memset(szBCD, 0x00, sizeof(szBCD));
		
		char szBlock[17];
		memset(szBlock, 0x00, sizeof(szBlock));
		memcpy(szBlock, "0000000000000000", 16);
		char* strSource = new char[nLength * 2];
		for (int k=0; k<nLength; k++)
		{
			sprintf(&strSource[2 * k], "%02X", pszSource[k]);
		}
		int nLoopCount, nCopyLen;
		nLoopCount = (nLength+7)/8;
		nLoopCount = (nLength*2 +15)/16;
		for (int i=0; i<nLoopCount; i++)
		{
			if (i == nLoopCount-1) 
				nCopyLen = nLength - i*8;
			//	nCopyLen = strSource.GetLength() -i*16;
			else  
				nCopyLen = 8 ;
			//	nCopyLen = 16;
			
			memcpy(szASC, strSource +i*16, nCopyLen*2);
			for(int j=0; j<16; j++)
			{	
				szBCD[j] = szASC[j];
			//	sprintf(&szBCD[2*j],"%02X", (UCHAR)szASC[j]);	
			}
			for (int j=0; j<16; j++)
			{
				char a= szBCD[j], b= szBlock[j];
				CHSTR2BCH(a);
				CHSTR2BCH(b);
				szBCD[j] = CHBCH2STR(a^b);
			}
			memset(szBlock, 0x00, 16);
			SingleDES(szBCD, szBlock, szKey1, 'E');
			memset(szASC, 0x00, sizeof(szASC));
			memset(szBCD, 0x00, sizeof(szBCD));
		}
		memcpy(pszDest, szBlock, sizeof(szBlock));
		delete[] strSource;
		strSource = NULL;
		return true;

}

bool DES::STR2BCH(char* strSource, char* pszTarget)
{

		if (!strlen(strSource))
		{
			return false;
		}
		if (pszTarget == NULL)
		{
			return false;
		}
		
		int nSourceLen;
		nSourceLen = strlen(strSource);
		if (nSourceLen%2 != 0)
		{
			memcpy(&strSource[nSourceLen], "0", 1);
		}
		
		char chTmp, chHigh, chLow;
		for (int i=0; i<nSourceLen/2; i++)
		{
			chHigh = strSource[i*2];
			chLow = strSource[i*2+1];
			if (CHSTR2BCH(chHigh) && CHSTR2BCH(chLow))
			{
				chTmp = (chHigh<<4)|(chLow&0x0f);
				*(pszTarget+i) = chTmp;
			}
			else
			{
				return false;
			}
		}
		return true;

}


void DES::CalANSI919MAC(char *dest, char *source, int length, char *in_key)
{
	int iCount;
	unsigned char byBlock1[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	unsigned char byBlock2[8];
	unsigned char byData[10];
	unsigned char byResult[12];
	int iKeyLen = 0;   //BAP_0006
	
	char szKey1[17], szKey2[17];
	memset(szKey1, 0x00, sizeof(szKey1));
	memset(szKey2, 0x00, sizeof(szKey2));
	memcpy(szKey1, in_key, 16);
	memcpy(szKey2, in_key+16, 16);
	
	char data[20],buf[20];
	unsigned int ikey = 0;
	
	//	iCount = (strlen(source)+ 7)/ 8 ;	//\BC\C6\CB\E3ѭ\BB\B7\B4\CE\CA\FD
	iCount = (length + 7)/ 8;
	memset(byBlock1,0x00,sizeof(byBlock1));
	memset(byBlock2,0x00,sizeof(byBlock2));
	memset(byData,0x00,sizeof(byData));
	memset(byResult,0x00,sizeof(byResult));
	
	int mcopylen;
	iKeyLen = strlen(in_key);  //BAP_0006
	
	
	if ( iKeyLen < 32 )
	{		
		return ;
	}
	
	for(int ii=0; ii<iCount; ii++)
	{
		if (ii == (iCount-1) ) 
			mcopylen = length - ii*8;
		else  
			mcopylen = 8 ;
		
		memset(byBlock2, 0x00 , sizeof(byBlock2));
		memcpy(byBlock2,(void*)(source+ii*8),mcopylen);
		for(int ij=0; ij<8; ij++)
			byData[ij] = byBlock1[ij] ^ byBlock2[ij];
		
		memset(data,0x00,sizeof(data));
		
		for(int j = 0; j < 8; j++)
		{	
			ikey =  byData[j];
			sprintf(&data[2*j],"%02X",ikey);	
			
		}
		//transfer 8byte to 16byte
		
		memset(buf,0x00,sizeof(buf));
		
		SingleDES(data,buf,in_key,'E');
		
		for(int i = 0; i < 8; i++)
		{	
			ikey = 0;
			sscanf(&buf[2*i],"%2X",&ikey);
			byResult[i] = ikey;
		}//transfer 16byte to 8byte
		
		memcpy(byBlock1,byResult,8);
	}
	
	for(int jj = 0; jj < 8; jj++)
	{	
		ikey =  byResult[jj];
		sprintf(&dest[2*jj],"%02X",ikey);
		
	}
	
	/* Decipher the CBC output using K' */
	SingleDES(dest,buf,szKey2,'D');
	
	/* Encipher the deciphered CBC output using K */
	SingleDES(buf,dest,szKey1,'E');
	
	return ;	//OK
}

void DES::CalMACFORQDCCB(char *dest,char *source,int length, const char *in_key)
{
	int iCount;
	unsigned char byBlock1[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	unsigned char byBlock2[8];
	unsigned char byData[10];
	unsigned char byResult[12];
	
	char data[20],buf[20];
	unsigned int ikey = 0;
	
	iCount = (length + 7)/ 8;
	memset(byBlock1,0x00,sizeof(byBlock1));
	memset(byBlock2,0x00,sizeof(byBlock2));
	memset(byData,0x00,sizeof(byData));
	memset(byResult,0x00,sizeof(byResult));
	
	int mcopylen;
    char achEnckey[16];
    memset(achEnckey,0x00,sizeof(achEnckey));
    char achDeckey[16];
    memset(achDeckey,0x00,sizeof(achDeckey));
    memcpy(achEnckey,in_key,16);
    memcpy(achDeckey,in_key+16,16);
	
	for(int ii=0; ii<iCount; ii++){
		if (ii == (iCount-1) ) 
			mcopylen = length - ii*8;
		else  
			mcopylen = 8 ;
		
		memset(byBlock2, 0x00 , sizeof(byBlock2));
		memcpy(byBlock2,(void*)(source+ii*8),mcopylen);
		for(int ij=0; ij<8; ij++)
			byData[ij] = byBlock1[ij] ^ byBlock2[ij];
		
		memset(data,0x00,sizeof(data));
		
		for(int j = 0; j < 8; j++)
		{	
			ikey =  byData[j];
			sprintf(&data[2*j],"%02X",ikey);	
		}//transfer 8byte to 16byte
		
		memset(buf,0x00,sizeof(buf));
		SingleDES(data,buf,achEnckey,'E');
		
		for(int i = 0; i < 8; i++)
		{	
			ikey = 0;
			sscanf(&buf[2*i],"%2X",&ikey);
			byResult[i] = ikey;
		}//transfer 16byte to 8byte
		
		memcpy(byBlock1,byResult,8);
	}
	
	for(int jj = 0; jj < 8; jj++)
	{	
		ikey =  byResult[jj];
		sprintf(&dest[2*jj],"%02X",ikey);	
	}
	SingleDES(dest,buf,achDeckey,'D');
	SingleDES(buf,dest,achEnckey,'E');
	return ;
}
