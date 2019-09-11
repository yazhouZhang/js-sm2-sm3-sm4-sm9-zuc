/*
 * sm4-1.0.js
 * 
 * Copyright (c) 2019 RuXing Liang
 */
/**
 * @name sm4-1.0.js
 * @author RuXing Liang
 * @version 1.0.0 (2019-04-19)
 */
var debug = false;
function SM4(){
	
	this.sbox = new Array(
		0xd6,0x90,0xe9,0xfe,0xcc,0xe1,0x3d,0xb7,0x16,0xb6,0x14,0xc2,0x28,0xfb,0x2c,0x05,
		0x2b,0x67,0x9a,0x76,0x2a,0xbe,0x04,0xc3,0xaa,0x44,0x13,0x26,0x49,0x86,0x06,0x99,
		0x9c,0x42,0x50,0xf4,0x91,0xef,0x98,0x7a,0x33,0x54,0x0b,0x43,0xed,0xcf,0xac,0x62,
		0xe4,0xb3,0x1c,0xa9,0xc9,0x08,0xe8,0x95,0x80,0xdf,0x94,0xfa,0x75,0x8f,0x3f,0xa6,
		0x47,0x07,0xa7,0xfc,0xf3,0x73,0x17,0xba,0x83,0x59,0x3c,0x19,0xe6,0x85,0x4f,0xa8,
		0x68,0x6b,0x81,0xb2,0x71,0x64,0xda,0x8b,0xf8,0xeb,0x0f,0x4b,0x70,0x56,0x9d,0x35,
		0x1e,0x24,0x0e,0x5e,0x63,0x58,0xd1,0xa2,0x25,0x22,0x7c,0x3b,0x01,0x21,0x78,0x87,
		0xd4,0x00,0x46,0x57,0x9f,0xd3,0x27,0x52,0x4c,0x36,0x02,0xe7,0xa0,0xc4,0xc8,0x9e,
		0xea,0xbf,0x8a,0xd2,0x40,0xc7,0x38,0xb5,0xa3,0xf7,0xf2,0xce,0xf9,0x61,0x15,0xa1,
		0xe0,0xae,0x5d,0xa4,0x9b,0x34,0x1a,0x55,0xad,0x93,0x32,0x30,0xf5,0x8c,0xb1,0xe3,
		0x1d,0xf6,0xe2,0x2e,0x82,0x66,0xca,0x60,0xc0,0x29,0x23,0xab,0x0d,0x53,0x4e,0x6f,
		0xd5,0xdb,0x37,0x45,0xde,0xfd,0x8e,0x2f,0x03,0xff,0x6a,0x72,0x6d,0x6c,0x5b,0x51,
		0x8d,0x1b,0xaf,0x92,0xbb,0xdd,0xbc,0x7f,0x11,0xd9,0x5c,0x41,0x1f,0x10,0x5a,0xd8,
		0x0a,0xc1,0x31,0x88,0xa5,0xcd,0x7b,0xbd,0x2d,0x74,0xd0,0x12,0xb8,0xe5,0xb4,0xb0,
		0x89,0x69,0x97,0x4a,0x0c,0x96,0x77,0x7e,0x65,0xb9,0xf1,0x09,0xc5,0x6e,0xc6,0x84,
		0x18,0xf0,0x7d,0xec,0x3a,0xdc,0x4d,0x20,0x79,0xee,0x5f,0x3e,0xd7,0xcb,0x39,0x48
	);
	
	this.fk = new Array(0xa3b1bac6, 0x56aa3350, 0x677d9197, 0xb27022dc);
	this.ck = new Array(
		0x00070e15,0x1c232a31,0x383f464d,0x545b6269,
		0x70777e85,0x8c939aa1,0xa8afb6bd,0xc4cbd2d9,
		0xe0e7eef5,0xfc030a11,0x181f262d,0x343b4249,
		0x50575e65,0x6c737a81,0x888f969d,0xa4abb2b9,
		0xc0c7ced5,0xdce3eaf1,0xf8ff060d,0x141b2229,
		0x30373e45,0x4c535a61,0x686f767d,0x848b9299,
		0xa0a7aeb5,0xbcc3cad1,0xd8dfe6ed,0xf4fb0209,
		0x10171e25,0x2c333a41,0x484f565d,0x646b7279
	);
}

SM4.prototype = {
	expandKey:function(key){
		var k = new Array(36);
		var mk = byteArrayToIntArray(key);
		k[0] = mk[0] ^ this.fk[0];
		k[1] = mk[1] ^ this.fk[1];
		k[2] = mk[2] ^ this.fk[2];
		k[3] = mk[3] ^ this.fk[3];
		var rk = new Array(32);
		for (var i = 0; i < 32; i++)
		{
			k[(i + 4)] = (k[i] ^ this.T1(k[(i + 1)] ^ k[(i + 2)] ^ k[(i + 3)] ^ this.ck[i]));
			rk[i] = k[(i + 4)];
		}
		return rk;
	},
	T1:function(ta){
		var rk = 0;
		var b = new Array(4);
		var a = intToByte(ta);
		b[0] = this.sbox[a[0] & 0xFF];
		b[1] = this.sbox[a[1] & 0xFF];
		b[2] = this.sbox[a[2] & 0xFF];
		b[3] = this.sbox[a[3] & 0xFF];
		var bint = byteToInt(b,0);
		var rk = bint ^ (bint<<13|(bint>>>(32-13))) ^ (bint<<23|(bint>>>(32-23)));
		return rk;
	},
	one_encrypt:function(rk, data){
		var x = new Array(36);
		x[0] = byteToInt(data, 0);
		x[1] = byteToInt(data, 4);
		x[2] = byteToInt(data, 8);
		x[3] = byteToInt(data, 12);
		for(var i = 0;i < 32;i++)
		{
			x[(i + 4)] = x[i] ^ this.T0(x[(i + 1)]^x[(i + 2)]^x[(i + 3)]^rk[i]);
		}
		var tmpx = new Array(4); 
		for(var i = 35;i >= 32;i--){
			tmpx[35-i] = x[i];
		}
		var xbyte = intArrayToByteArray(tmpx);
		
		return xbyte;
	},
	T0:function(ta){
		var a = intToByte(ta);
		var b = new Array(4);
		b[0] = this.sbox[a[0] & 0xFF];
		b[1] = this.sbox[a[1] & 0xFF];
		b[2] = this.sbox[a[2] & 0xFF];
		b[3] = this.sbox[a[3] & 0xFF];
		var bint = byteToInt(b,0);
		var c = bint ^ (bint<<2|(bint>>>(32-2))) ^ (bint<<10|(bint>>>(32-10))) ^ (bint<<18|(bint>>>(32-18))) ^ (bint<<24|(bint>>>(32-24)));
		return c;
	},
	pkcs7padding:function(input,mode){
		if (input == null)
		{
			return null;
		}

		var ret = null;
		if (mode == 1)//填充
		{
			var p = 16 - input.length % 16;
			ret = new Array(input.length + p);
			arrayCopy(input, 0, ret, 0, input.length);
			for (var i = 0; i < p; i++)
			{
				ret[input.length + i] = p;
			}
		}
		else//去除填充
		{
			var p = input[input.length - 1];
			ret = new Array(input.length - p);
			arrayCopy(input, 0, ret, 0, input.length - p);
		}
		return ret;
	},
	encrypt_ecb:function(key,data){
		if(key == undefined || key == null || key.length%16 != 0){
			console.log("sm4 key is error!");
			return null;
		}
		if(data == undefined || data == null || data.length <= 0){
			console.log("data is error!");
			return null;
		}
		var rk = this.expandKey(key);
		/*if(debug){
			var rkb = intArrayToByteArray(rk);
			console.log(Hex.encode(rkb,0,rkb.length));
		}*/
		
		var blockLen = 16;
		var loop = parseInt(data.length/blockLen);//注意不能整除会有小数，要取整
		var cipher = new Array((loop+1)*blockLen);
		var tmp = new Array(blockLen);
		var oneCipher = null;
		
		for(var i = 0;i<loop;i++){
			arrayCopy(data,i*blockLen,tmp,0,blockLen);
			oneCipher = this.one_encrypt(rk,tmp);
			arrayCopy(oneCipher,0,cipher,i*blockLen,blockLen);
		}
		
		var lessData = new Array(data.length%blockLen);
		if(lessData.length > 0){
			arrayCopy(data,loop*blockLen,lessData,0,data.length%blockLen);
		}
		var padding = this.pkcs7padding(lessData,1);
		oneCipher = this.one_encrypt(rk,padding);
		arrayCopy(oneCipher,0,cipher,loop*blockLen,blockLen);
		
		return cipher;
	},
	decrypt_ecb:function(key,data){
		if(key == undefined || key == null || key.length%16 != 0){
			console.log("sm4 key is error!");
			return null;
		}
		if(data == undefined || data == null || data.length%16 != 0){
			console.log("data is error!");
			return null;
		}
		var rk = this.expandKey(key);
		var nrk = new Array(32);
		for(var i = 0;i<rk.length;i++){
			nrk[i] = rk[32-i-1];
		}
		/*if(debug){
			var rkb = intArrayToByteArray(rk);
			console.log(Hex.encode(rkb,0,rkb.length));
		}*/
		var blockLen = 16;
		var loop = data.length/blockLen - 1;
		var tmp = new Array(blockLen);
		var onePlain = null;
		var plain = null;
		//先解密最后一部分，确定数据长度
		arrayCopy(data,loop*blockLen,tmp,0,blockLen);
		onePlain = this.one_encrypt(nrk,tmp);
		var lastPart = this.pkcs7padding(onePlain,0);
		
		plain = new Array(loop*blockLen+lastPart.length);
		arrayCopy(lastPart,0,plain,loop*blockLen,lastPart.length);
		
		//解密剩下部分数据
		for(var i = 0;i<loop;i++){
			arrayCopy(data,i*blockLen,tmp,0,blockLen);
			onePlain = this.one_encrypt(nrk,tmp);
			arrayCopy(onePlain,0,plain,i*blockLen,blockLen);
		}
		
		return plain;
	},
	encrypt_cbc:function(key,iv,data){
		if(key == undefined || key == null || key.length%16 != 0){
			console.log("sm4 key is error!");
			return null;
		}
		if(data == undefined || data == null || data.length <= 0){
			console.log("data is error!");
			return null;
		}
		if(iv == undefined || iv == null || iv.length%16 != 0){
			console.log("iv is error!");
			return null;
		}
		var rk = this.expandKey(key);
		/*if(debug){
			var rkb = intArrayToByteArray(rk);
			console.log(Hex.encode(rkb,0,rkb.length));
		}*/
		
		var blockLen = 16;
		var loop = parseInt(data.length/blockLen);//注意不能整除会有小数，要取整
		var cipher = new Array((loop+1)*blockLen);
		var tmp = new Array(blockLen);
		var oneCipher = null;
		
		for(var i = 0;i<loop;i++){
			arrayCopy(data,i*blockLen,tmp,0,blockLen);
			for(var j = 0;j<blockLen;j++){
				tmp[j] = tmp[j] ^ iv[j];
			}
			iv = this.one_encrypt(rk,tmp);
			arrayCopy(iv,0,cipher,i*blockLen,blockLen);
		}
		
		var lessData = new Array(data.length%blockLen);
		if(lessData.length > 0){
			arrayCopy(data,loop*blockLen,lessData,0,data.length%blockLen);
		}
		var padding = this.pkcs7padding(lessData,1);
		for(var i = 0;i<blockLen;i++){
			padding[i] = padding[i] ^ iv[i];
		}
		iv = this.one_encrypt(rk,padding);
		arrayCopy(iv,0,cipher,loop*blockLen,blockLen);
		
		return cipher;
	},
	decrypt_cbc:function(key,iv,data){
		if(key == undefined || key == null || key.length%16 != 0){
			console.log("sm4 key is error!");
			return null;
		}
		if(data == undefined || data == null || data.length%16 != 0){
			console.log("data is error!");
			return null;
		}
		if(iv == undefined || iv == null || iv.length%16 != 0){
			console.log("iv is error!");
			return null;
		}
		var rk = this.expandKey(key);
		var nrk = new Array(32);
		for(var i = 0;i<rk.length;i++){
			nrk[i] = rk[32-i-1];
		}
		/*if(debug){
			var rkb = intArrayToByteArray(rk);
			console.log(Hex.encode(rkb,0,rkb.length));
		}*/
		var blockLen = 16;
		var loop = data.length/blockLen;
		var tmp = new Array(blockLen);
		var onePlain = null;
		var plain = null;
		
		
		//解密
		plain = new Array(data.length);
		for(var i = 0;i<loop;i++){
			arrayCopy(data,i*blockLen,tmp,0,blockLen);
			onePlain = this.one_encrypt(nrk,tmp);
			for(var j = 0;j<blockLen;j++){
				onePlain[j] = onePlain[j] ^ iv[j];
			}
			arrayCopy(tmp,0,iv,0,blockLen);
			arrayCopy(onePlain,0,plain,i*blockLen,blockLen);
		}
		
		//去填充，确定数据长度
		//arrayCopy(data,data.length-blockLen,tmp,0,blockLen);
		var lastPart = this.pkcs7padding(onePlain,0);
		
		var realPlain = new Array(plain.length-blockLen+lastPart.length);
		arrayCopy(plain,0,realPlain,0,plain.length-blockLen);
		arrayCopy(lastPart,0,realPlain,plain.length-blockLen,lastPart.length);
		
		
		//先解密最后一部分，确定数据长度
		/*arrayCopy(data,loop*blockLen,tmp,0,blockLen);
		onePlain = this.one_encrypt(nrk,tmp);
		for(var i = 0;i<blockLen;i++){
			onePlain[i] = onePlain[i] ^ iv[i];
		}
		var lastPart = this.pkcs7padding(onePlain,0);
		
		plain = new Array(loop*blockLen+lastPart.length);
		arrayCopy(lastPart,0,plain,loop*blockLen,lastPart.length);
		
		//解密剩下部分数据
		for(var i = 0;i<loop;i++){
			arrayCopy(data,i*blockLen,tmp,0,blockLen);
			onePlain = this.one_encrypt(nrk,tmp);
			arrayCopy(onePlain,0,plain,i*blockLen,blockLen);
		}*/
		
		
		return realPlain;
	}
}
