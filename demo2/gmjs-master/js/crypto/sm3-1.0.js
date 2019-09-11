/*
 * sm3-1.0.js
 * 
 * Copyright (c) 2019 RuXing Liang
 */
/**
 * @name sm3-1.0.js
 * @author RuXing Liang
 * @version 1.0.0 (2019-04-16)
 */

//加密数据不能超过500M


function SM3Digest(){

	this.ivByte = new Array( 0x73,  0x80, 0x16, 0x6f, 0x49,
		0x14,  0xb2, 0xb9, 0x17, 0x24, 0x42, 0xd7,
		0xda,  0x8a, 0x06, 0x00, 0xa9, 0x6f, 0x30,
		0xbc,  0x16, 0x31, 0x38, 0xaa, 0xe3,
		0x8d,  0xee, 0x4d, 0xb0, 0xfb, 0x0e,
		0x4e );
	this.iv = byteArrayToIntArray(
				this.ivByte
			);
	this.tj = new Array(64);
	this.BLOCK_BYTE_LEN = 64;
	
	this.vbuf = new Array(8);
	//数据缓冲区
	this.dataBuf = new Array(64);
	//缓冲区长度
	this.dataBufLen = 0;
	//缓冲区总长度
	this.totalLen = 0;//事实上需要long,后续需改进
	
	for(var i = 0;i<64;i++) {
		if(i<=15) {
			this.tj[i] = 0x79cc4519;
		}else {
			this.tj[i] = 0x7a879d8a;
		}
	}
	arrayCopy(this.iv, 0, this.vbuf, 0, this.vbuf.length);
	
}



SM3Digest.prototype = {
	ffj:function(x,y,z,i){
		var tmp;
		if(i<=15) {
			tmp = x^y^z;
		}else{
			tmp = (x&y)|(x&z)|(y&z);
		}
		
		return tmp;
	},
	ggj:function(x,y,z,i){
		var tmp = 0;
		if(i<=15) {
			tmp = x^y^z;
		}else {
			tmp = (x&y)|(~x&z);
		}
		
		return tmp;
	},
	p0:function(x) {
		//这里的公式是：对于一个二进制有n位的数字循环左移(循环右移)m位，
	//可以将此数字左移(无符号右移)m位的结果与此数字 无符号 右移(左移)n-m位的结果进行或操作。 
		return x^(x<<9|(x>>>(32-9)))^(x<<17|(x>>>(32-17)));
	},
	p1:function(x) {
		//这里的公式是：对于一个二进制有n位的数字循环左移(循环右移)m位，
	//可以将此数字左移(无符号右移)m位的结果与此数字 无符号 右移(左移)n-m位的结果进行或操作。 
		return x^(x<<15|(x>>>(32-15)))^(x<<23|(x>>>(32-23)));
	},
	
	
	/**
	 * 循环左移
	 */
	cycleLeft:function(x,moveLen) {
		return x<<moveLen|(x>>>(32-moveLen));
	},
	
	
	
	/**
	 * 消息填充函数
	 * @param data
	 * @return
	 */
	padding:function(data) {
		var k = 0;
		var len = data.length;
		var padding;
		
		k = 64 - (len + 1 + 8)%64;
		if(k>=64) {
			k = 0;
		}
		padding = new Array(k+1+len+8);
		padding[len] = 1<<7;
		
		
		arrayCopy(data, 0, padding, 0, len);
		arrayCopy(longToByte(this.totalLen<<3), 0, padding, len+k+1, 8);
		
		return padding;
	},
	
	/**
	 * 对数据进行分组迭代，每64个字节迭代一次
	 * <br>1、对消息进行分组，由于是int类型，则每16个分一组，对每一组再调用{@link #expand}进行拓展
	 * <br>2、使用上一轮的迭代结果V，调用{@link #cf}进行本轮迭代
	 * <br>3、最后一轮迭代结果复制进缓冲区vbuf
	 * @param message
	 */
	iterate:function(message) {
		var len = message.length;
		var n = parseInt(len/16);
		var v,b;
		var ep;
		
		v = this.vbuf;
		b = new Array(16);
		
		for(var i = 0;i<n;i++) {
			arrayCopy(message, i*16, b, 0, b.length);
			ep = this.expand(b);
			v = this.cf(v, ep[0], ep[1]);
		}
		arrayCopy(v, 0, this.vbuf, 0, v.length);
	},
	
	/**
	 * 消息数据拓展函数
	 * @param b
	 * @return
	 */
	expand:function(b) {
		var w1 = new Array(68);
		var w2 = new Array(64);
		
		arrayCopy(b, 0, w1, 0, b.length);
		
		for(var i = 16;i<w1.length;i++) {
			w1[i] = this.p1(w1[i-16]^w1[i-9]^this.cycleLeft(w1[i-3], 15))^this.cycleLeft(w1[i-13],7)^w1[i-6];
		}
		
		for(var i = 0;i<w2.length;i++) {
			w2[i] = w1[i]^w1[i+4];
		}
		
		return new Array(w1,w2);
	},
	
	/**
	 * 迭代压缩函数
	 * 
	 * @param v
	 * @param w1
	 * @param w2
	 * @return
	 */
	cf:function(v,w1,w2) {
		var result;
		var a,b,c,d,e,f,g,h,ss1,ss2,tt1,tt2;
		a = v[0];
		b = v[1];
		c = v[2];
		d = v[3];
		e = v[4];
		f = v[5];
		g = v[6];
		h = v[7];
		
		for(var i = 0;i<64;i++) {
			ss1 = this.cycleLeft(this.cycleLeft(a, 12)+e+this.cycleLeft(this.tj[i], i),7);
			ss2 = ss1^this.cycleLeft(a, 12);
			tt1 = this.ffj(a, b, c,i)+d+ss2+w2[i];
			tt2 = this.ggj(e,f,g,i)+h+ss1+w1[i];
			d = c;
			c = this.cycleLeft(b, 9);
			b = a;
			a = tt1;
			h = g;
			g = this.cycleLeft(f, 19);
			f = e;
			e = this.p0(tt2);
		}
		
		result = new Array(8);
		result[0] = a^v[0];
		result[1] = b^v[1];
		result[2] = c^v[2];
		result[3] = d^v[3];
		result[4] = e^v[4];
		result[5] = f^v[5];
		result[6] = g^v[6];
		result[7] = h^v[7];
		
		
		return result;
	},
	
	digest:function(data) {
		var mac;
		
		var padding = this.padding(data);
		var paddingInt = byteArrayToIntArray(padding);
		this.iterate(paddingInt);
		var macInt = this.vbuf;
		mac = intArrayToByteArray(macInt);
		return mac;
	},
	
	
	update:function(data,pos,len) {
			
		var loop = parseInt((len+this.dataBufLen)/64);//向下取整
		this.totalLen += len;
		
		if(len+this.dataBufLen<this.BLOCK_BYTE_LEN) {
			arrayCopy(data, 0, this.dataBuf, this.dataBufLen, len);
			this.dataBufLen = len+this.dataBufLen;
		}else {
			var dataInt;
			arrayCopy(data, 0 , this.dataBuf, this.dataBufLen, this.BLOCK_BYTE_LEN-this.dataBufLen);
			dataInt = byteArrayToIntArray(this.dataBuf);
			this.iterate(dataInt);
			for(var i = 1;i<loop;i++) {
				arrayCopy(data, i*this.BLOCK_BYTE_LEN-this.dataBufLen, this.dataBuf, 0, this.BLOCK_BYTE_LEN);
				dataInt = byteArrayToIntArray(this.dataBuf);
				this.iterate(dataInt);
			}
			arrayCopy(data, loop*this.BLOCK_BYTE_LEN-this.dataBufLen , this.dataBuf, 0, len-(loop*this.BLOCK_BYTE_LEN-this.dataBufLen));
			this.dataBufLen = len-(loop*this.BLOCK_BYTE_LEN-this.dataBufLen);
		}
		
	},
	
	doFinal:function() {
		var mac;
		var finalData = new Array(this.dataBufLen);
		
		arrayCopy(this.dataBuf, 0, finalData, 0, this.dataBufLen);
		//对不足64字节的数据进行填充
		var paddingArr = this.padding(finalData);
		var paddingInt = byteArrayToIntArray(paddingArr);
		this.iterate(paddingInt);
		var macInt = this.vbuf;
		mac = intArrayToByteArray(macInt);
		return mac;
			
	}

}