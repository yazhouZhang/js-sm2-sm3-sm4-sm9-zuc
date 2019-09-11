/*
 * sm2-1.0.js
 * 
 * Copyright (c) 2019 RuXing Liang
 */
/**
 * @name sm2-1.0.js
 * @author RuXing Liang
 * @version 1.0.0 (2019-04-16)
 */

var debug = false;
//国密推荐曲线
var sm2_ecParams = {
    "p":"FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF",
    "a":"FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC",
    "b":"28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93",
    "n":"FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123",
    "gx":"32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7",
    "gy":"BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0",
    "keylen":256
};

function SM2(){

    this.ecc_p = new BigInteger(sm2_ecParams['p'],16);
    this.ecc_a = new BigInteger(sm2_ecParams['a'],16);
    this.ecc_b = new BigInteger(sm2_ecParams['b'],16);
    this.ecc_n = new BigInteger(sm2_ecParams['n'],16);
    this.ecc_gx = new BigInteger(sm2_ecParams['gx'],16);
    this.ecc_gy = new BigInteger(sm2_ecParams['gy'],16);
    this.rng = new SecureRandom();
    
    this.ecCurve = new ECCurveFp(this.ecc_p, this.ecc_a, this.ecc_b);
    this.ecPointG = ECPointFp.decodeFromHex(this.ecCurve,"04"+sm2_ecParams['gx']+sm2_ecParams['gy']);
    if(debug == true){
        console.log("ax1="+this.ecCurve.getA().toBigInteger().toString(16));
        console.log("by1="+this.ecCurve.getB().toBigInteger().toString(16));
        console.log("gx1="+this.ecPointG.getX().toBigInteger().toString(16));
        console.log("gy1="+this.ecPointG.getY().toBigInteger().toString(16));
    }
}

SM2.prototype = {
    getBigRandom:function(limit) {
        return new BigInteger(limit.bitLength(), this.rng)
        .mod(limit.subtract(BigInteger.ONE))
        .add(BigInteger.ONE)
        ;
    },
    generateKeyPairHex:function(){
		var key = this.generateKeyPairBigInteger();
        var biX = key['pubkey'].getX().toBigInteger();
        var biY = key['pubkey'].getY().toBigInteger();

        var charlen = sm2_ecParams['keylen'] / 4;
        var hPrv = ("0000000000" + key['privkey'].toString(16)).slice(- charlen);
        var hX   = ("0000000000" + biX.toString(16)).slice(- charlen);
        var hY   = ("0000000000" + biY.toString(16)).slice(- charlen);
        var hPub = "04" + hX + hY;

        return {'privkeyhex': hPrv, 'pubkeyhex': hPub};
    },
    generateKeyPairBigInteger:function(){
		var biN = this.ecc_n;
		var biPrv = null;
		var epPub = null;
		while(true){
			do
			{
				biPrv = this.getBigRandom(biN);
			}
			while (biPrv.equals(BigInteger.ZERO) || biPrv.compareTo(biN) >= 0 || biPrv.bitLength() < 249);

			epPub = this.ecPointG.multiply(biPrv);
			if(epPub.getX().toBigInteger().bitLength() >= 249 && epPub.getY().toBigInteger().bitLength() >= 249){
				
				break;
			}
		}
        return {'privkey': biPrv, "pubkey":epPub};
    },
    formartXY:function(bg,needLength) {
		
        var tmp = new Array(needLength);
        for(var i = 0;i<tmp.length;i++){
            tmp[i] = 0;
        }
		var bgByte = bg.toByteArray();
		if(bgByte == null) {
			return null;
		}
		
		if(bgByte.length > needLength) {
			arrayCopy(bgByte, bgByte.length - needLength, tmp, 0, needLength);
		}else if(bgByte.length == needLength) {
			tmp = bgByte;
		}else {
			arrayCopy(bgByte, 0, tmp, needLength - bgByte.length, bgByte.length);
		}
		
		return tmp;
	},
    kdf:function(xy,data) {
		
//		int ct = 1;
		// var loop = data.length%32!=0 ? (data.length/32+1):data.length/32;
		var loop = Math.ceil(data.length/32);//向上取整
		var sm3;
		var hash = new Array(32);
		
		for(var i = 0;i<loop;i++) {
			sm3 = new SM3Digest();
			sm3.update(xy,0,xy.length);
			sm3.update(intToByte(i+1),0,4);
			hash = sm3.doFinal();
	
			for(var  j = 0;j<hash.length&&(i*32+j)<data.length;j++) {
				data[i*32+j] ^= hash[j];
			}
		}
		
		return 0;
	},
	arrayCompare:function(src1,pos1,src2,pos2,len) {
			
		if(src1.length - pos1 < len) {
			return -1;
		}
		if(src2.length - pos2 < len) {
			return -1;
		}
		for(var i = 0;i<len;i++) {
			if(src1[pos1++] != src2[pos2++]) {
				return -1;
			}
		}
		
		return 0;
	},
    encrypt:function(pubkey,dataHex){
        var cipher;
		
		//1、创建sm2曲线，公钥P1加压缩标志0x04	2、将公钥转为曲线上的点 u1 
		//3、生成随机公私钥:k、P2 ，新公钥P2即为（x1,y1）点 ，x1y1拼接，即为:c1 （注意长度保持64字节）
		//4、新私钥k与公钥P1相乘，即为（x2,y2）点，通过kdf算法，得到t字符串，t字符串和明文做异或运算，得到c2
		//5、x2||明文||y2 三者拼接然后做sm3运算，得到c3 
		//说明，kdf主要是：循环次数loop（明文字节长度除以32,向上取整，运算时依次从1到最大值）拼接x2y2，即：x2||y2||loop（loop是int类型，须转成四字节），
		//然后对拼接后的字符串做sm3运算，依次将每次的结果拼接，得到最终结果
		if(pubkey == null || pubkey.length == 0 || dataHex == null || dataHex.length == 0) {
			return null;
		}
		
		if(pubkey.length == 128) {
			pubkey = "04"+pubkey;
		}
		
		var data = Hex.decode(dataHex);
		var userKey = ECPointFp.decodeFromHex(this.ecCurve,pubkey);
		var c2 = null;
		var c1 = null;
		var x2 = null;
		var y2 = null;
		var loop = 0;
		do{
			var kp = this.generateKeyPairBigInteger();//创建公私钥对，这里私钥当做随机数k，公钥为c1点(x1,y1)
			if(debug == true){
				console.log("priv"+kp['privkey'].toString(16));
				console.log("x1="+kp['pubkey'].getX().toBigInteger().toString(16));
				console.log("y1="+kp['pubkey'].getY().toBigInteger().toString(16));
			}
			c1 = kp['pubkey'];
			
			
			var x2y2 = userKey.multiply(kp['privkey']);//(x2,y2)
			x2 = this.formartXY(x2y2.getX().toBigInteger(), 32);
			y2 = this.formartXY(x2y2.getY().toBigInteger(), 32);
			if(debug == true){
				console.log("x2="+x2);
				console.log("y2="+y2);
				console.log("x2="+Hex.encode(x2,0,x2.length));
				console.log("y2="+Hex.encode(y2,0,y2.length));
			}

			c2 = new Array(data.length);
			arrayCopy(data,0,c2,0,data.length);
			var xy = new Array(x2.length+y2.length);
			arrayCopy(x2, 0, xy, 0, x2.length);
			arrayCopy(y2, 0, xy, x2.length, y2.length);
			this.kdf(xy, c2);

			loop++;
			
		}while(this.arrayCompare(c2,0,data,0,data.length) == 0 && loop < 10);
		if(loop >= 10){//失败超过一定次数，不再尝试
			return null;
		}

		var sm3 = new SM3Digest();
		sm3.update(x2,0,x2.length);
		sm3.update(data,0,data.length);
		sm3.update(y2,0,y2.length);
		var c3 = sm3.doFinal();
		if(debug == true){
			console.log("data="+ Hex.encode(data,0,data.length));
			console.log("c3="+ Hex.encode(c3,0,c3.length));
		}

		
//		cipher = new Array(96+c2.length);
		
		/*var c1x = this.formartXY(c1.getX().toBigInteger(), 33);
		var c1y = this.formartXY(c1.getY().toBigInteger(), 33);*/
		var c1x = c1.getX().toBigInteger().toByteArray();
		var c1y = c1.getY().toBigInteger().toByteArray();
		/*arrayCopy(c1x, 0, cipher, 0, c1x.length);
		arrayCopy(c1y, 0, cipher, c1x.length, c1y.length);
		arrayCopy(c3, 0, cipher, c1x.length+c1y.length, c3.length);
		arrayCopy(c2, 0, cipher, c1x.length+c1y.length+c3.length, c2.length);
		console.log(Hex.encode(cipher,0,cipher.length));*/
		var cipher = this.cipherToDer(c1x,c1y,c2,c3);
		
		return Hex.encode(cipher,0,cipher.length);
	},
	decrypt:function(privkey,cipherHex) {
		cipher = Hex.decode(cipherHex);
		var dec = this.derDecode(cipher);
		
		var c1 = new Array(64+1);
		var c2 = new Array(dec["c2"].length);
		var c3 = new Array(32);
		
		//1、创建sm2曲线，截取密文前64字节，转化为点（x1,y1）
		//2、点（x1,y1）与私钥k相乘，得到（x2,y2）
		//3、x2y2通过kdf算法得到字符串t，t再和密文c2做异或运算，得到明文M
		//4、x2||M||y2 拼接，然后做sm3运算，得到hash值，对比密文的c3，一致则解密成功
		for(var i = 0;i<c1.length;i++){
			c1[i] = 0;
		}
		if(dec["c1x"].length <= 32){//由于c1x/y两个元素长度可能不等于32，须调整为32
			arrayCopy(dec["c1x"], 0, c1, 1+(32-dec["c1x"].length), dec["c1x"].length);
		}else{
			arrayCopy(dec["c1x"], dec["c1x"].length-32, c1, 1, 32);
		}
		if(dec["c1y"].length <= 32){
			arrayCopy(dec["c1y"], 0, c1, 1+32+(32-dec["c1y"].length), dec["c1y"].length);
		}else{
			arrayCopy(dec["c1y"], dec["c1y"].length-32, c1, 1+32, 32);
		}
		
		c1[0] = 0x04;
		var c1Point = ECPointFp.decodeFromHex(this.ecCurve,Hex.encode(c1,0,c1.length));
		
		var x2y2Point = c1Point.multiply(new BigInteger(privkey,16));
		var x2 = this.formartXY(x2y2Point.getX().toBigInteger(), 32);
		var y2 = this.formartXY(x2y2Point.getY().toBigInteger(), 32);
		var xy = new Array(x2.length+y2.length);
		arrayCopy(x2, 0, xy, 0, x2.length);
		arrayCopy(y2, 0, xy, x2.length, y2.length);
		
		arrayCopy(dec["c2"], 0, c2, 0, c2.length);
		var c2Copy = new Array(c2.length);
		arrayCopy(c2, 0, c2Copy, 0, c2.length);
		this.kdf(xy, c2);
		if(this.arrayCompare(c2Copy,0,c2,0,c2.length) == 0){
			console.log("t is all 0 and decrypt is failed!");
			return null;
		}
		
		var sm3 = new SM3Digest();
		var hash = new Array(32);
//		arrayCopy(cipher, c1.length-1, c3, 0, c3.length);
		sm3.update(x2,0,x2.length);
		sm3.update(c2,0,c2.length);
		sm3.update(y2,0,y2.length);
		hash = sm3.doFinal();
		
		if(this.arrayCompare(hash, 0, dec["c3"], 0, 32) != 0) {
			return null;
		}
		
		
		return Hex.encode(c2,0,c2.length);
	},
	cipherToDer:function(c1x,c1y,c2,c3){
		var c2Len = c2.length;
		var c2Tag = [];
		if(c2Len < 0x80){
			c2Tag[0] = 0x04;
			c2Tag[1] = c2Len;
		}else{
			c2Tag[0] = 0x04;
			var c2LenBytes = intToByte(c2Len);
			var i = 0;
			while(c2LenBytes[i] == 0 && i < c2LenBytes.length){
				i++;
			}
			c2Tag[1] = 0x80 | (c2LenBytes.length - i);
			for(var j = 2;i<c2LenBytes.length;i++,j++){
				c2Tag[j] = c2LenBytes[i];
			}
		}
		var totalTagLen = c1x.length+c1y.length+c2.length+c3.length+6+c2Tag.length;
		var totalTag = [];
		totalTag[0] = 0x30;
		if(totalTagLen < 0x80){
			totalTag[1] = totalTagLen;
		}else{
			var totalTagLenBytes = intToByte(totalTagLen);
			var i = 0;
			while(totalTagLenBytes[i] == 0 && i < totalTagLenBytes.length){
				i++;
			}
			totalTag[1] = 0x80 | (totalTagLenBytes.length - i);
			for(var j = 2;i<totalTagLenBytes.length;i++,j++){
				totalTag[j] = totalTagLenBytes[i];
			}
		}
		
		var der = new Array(totalTagLen+totalTag.length);
		var derLen = 0;
		
		arrayCopy(totalTag, 0, der, 0, totalTag.length);
		derLen += totalTag.length;
		
		der[derLen++] = 0x02;
		der[derLen++] = c1x.length;
		arrayCopy(c1x, 0, der, derLen, c1x.length);
		derLen += c1x.length;
		
		der[derLen++] = 0x02;
		der[derLen++] = c1y.length;
		arrayCopy(c1y, 0, der, derLen, c1y.length);
		derLen += c1y.length;
		
		der[derLen++] = 0x04;
		der[derLen++] = c3.length;
		arrayCopy(c3, 0, der, derLen, c3.length);
		derLen += c3.length;
		
		arrayCopy(c2Tag, 0, der, derLen, c2Tag.length);
		derLen += c2Tag.length;
		arrayCopy(c2, 0, der, derLen, c2.length);
		
		return der;
	},
	derDecode:function(der){
		var pos = 0;
		var totalLen = 0;
		
		if(der[pos++] != 0x30){
			return null;
		}
		if((der[pos] & 0xFF) <= 0x7F){
//			totalLen = der[pos++] & 0xFF;
			pos++;
		}else{
			pos += (der[pos] & 0x7F)+1;
		}
		
		pos++;
		var c1xLen = der[pos];
		var c1x = new Array(c1xLen);
		arrayCopy(der, ++pos , c1x, 0, c1xLen);
		pos += c1xLen;
		
		pos++;
		var c1yLen = der[pos];
		var c1y = new Array(c1yLen);
		arrayCopy(der, ++pos , c1y, 0, c1yLen);
		pos += c1yLen;
		
		pos++;
		var c3Len = der[pos];
		var c3 = new Array(c3Len);
		arrayCopy(der, ++pos , c3, 0, c3Len);
		pos += c3Len;
		
		pos++;
		var c2Len = 0;
		if((der[pos] & 0xFF) <= 0x7F){
			c2Len = der[pos] & 0xFF;
		}else{
			for(var i = 0,j = (der[pos] & 0x7F)-1;i<(der[pos] & 0x7F);i++,j--){
				c2Len = c2Len | ((der[pos+i+1]&0xFF) << (j*8)) ;
			}
			pos += (der[pos] & 0x7F);
		}
		var c2 = new Array(c2Len);
		arrayCopy(der, ++pos , c2, 0, c2Len);
		pos += c2Len;
		
		return {'c1x': c1x, "c1y":c1y,"c2":c2,"c3":c3};
	}
}