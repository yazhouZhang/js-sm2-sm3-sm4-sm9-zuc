/*
 * 
 * 字节流转换工具js
 * 
 */

/*
 * 数组复制
 */
function arrayCopy(src,pos1,dest,pos2,len){
	var realLen = len;
	if(pos1+len>src.length&&pos2+len<=dest.length){
		realLen = src.length-pos1;
	}else if(pos2+len>dest.length&&pos1+len<=src.length){
		realLen = dest.length-pos2;
	}else if(pos1+len<=src.length&&pos2+len<=dest.length){
		realLen = len;
	}else if(dest.length<src.length){
		realLen = dest.length-pos2;
	}else{
		realLen = src.length-pos2;
	}
	
	for(var i=0;i<realLen;i++){
		dest[i+pos2] = src[i+pos1];
	}
}

/*
 * 长整型转成字节，一个长整型为8字节
 * 返回：字节数组
 */
function longToByte(num){
	//TODO 这里目前只转换了低四字节，因为js没有长整型，得要封装
	return new Array(
				0,
				0,
				0,
				0,
        		(num >> 24)&0x000000FF,
        		(num >> 16)&0x000000FF,
        		(num >> 8)&0x000000FF,
        		(num)&0x000000FF
        );
}

/*
 * int数转成byte数组
 * 事实上只不过转成byte大小的数，实际占用空间还是4字节
 * 返回：字节数组
 */
function intToByte(num) {
    return new Array(
    		(num >> 24)&0x000000FF,
    		(num >> 16)&0x000000FF,
    		(num >> 8)&0x000000FF,
    		(num)&0x000000FF
    );
}

/*
 * int数组转成byte数组，一个int数值转成四个byte
 * 返回:byte数组
 */
function intArrayToByteArray(nums) {
	var b = new Array(nums.length*4);
	
	for(var i = 0;i<nums.length;i++) {
		arrayCopy(intToByte(nums[i]), 0, b, i*4, 4);
	}
	
	return b;
}

/*
 * byte数组转成int数值
 * 返回：int数值
 */
function byteToInt(b,pos) {
	if(pos+3<b.length) {
		return ((b[pos])<<24) | ((b[pos+1])<<16) | ((b[pos+2])<<8) | ((b[pos+3]));			
	}else if(pos+2<b.length) {
		return ((b[pos+1])<<16) | ((b[pos+2])<<8 )  | ((b[pos+3]));
	}else if(pos+1<b.length) {
		return ((b[pos])<<8) | ((b[pos+1]));
	}else {
		return ((b[pos]));
	}
}

/*
 * byte数组转成int数组,每四个字节转成一个int数值
 * 
 */
function byteArrayToIntArray(b) {
	// var arrLen = b.length%4==0 ? b.length/4:b.length/4+1;
	var arrLen = Math.ceil(b.length/4);//向上取整
	var out = new Array(arrLen);
	for(var i = 0;i<b.length;i++){
		b[i] = b[i]&0xFF;//避免负数造成影响
	}
	for(var i = 0;i<out.length;i++) {
		out[i] = byteToInt(b,i*4);
	}
	return out;
}