function Hex(){
    
}

Hex.encode = function(b,pos,len) {
    var hexCh = new Array(len*2);
    var hexCode = new Array('0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F');
    
    for(var i = pos,j = 0;i<len+pos;i++,j++) {
        hexCh[j] = hexCode[(b[i]&0xFF)>>4];
        hexCh[++j] = hexCode[(b[i]&0x0F)];
    }
    
    return hexCh.join('');
}

Hex.decode = function(hex) {
    
    if(hex == null || hex == '') {
        return null;
    }
    if(hex.length%2 != 0) {
        return null;
    }
    
    var ascLen = hex.length/2;
    var hexCh = this.toCharCodeArray(hex);
    var asc = new Array(ascLen);
    
    for(var i = 0;i<ascLen;i++) {
        
        if(hexCh[2*i]>=0x30 && hexCh[2*i]<=0x39) {
            asc[i] = ((hexCh[2*i]-0x30)<<4);
        }else if(hexCh[2*i]>=0x41 && hexCh[2*i]<=0x46) {//A-F : 0x41-0x46
            asc[i] = ((hexCh[2*i]-0x41+10)<<4);
        }else if(hexCh[2*i]>=0x61 && hexCh[2*i]<=0x66) {//a-f  : 0x61-0x66
            asc[i] = ((hexCh[2*i]-0x61+10)<<4);
        }else {
            return null;
        }
        
        if(hexCh[2*i+1]>=0x30 && hexCh[2*i+1]<=0x39) {
            asc[i] = (asc[i] | (hexCh[2*i+1]-0x30));
        }else if(hexCh[2*i+1]>=0x41 && hexCh[2*i+1]<=0x46) {
            asc[i] = (asc[i] | (hexCh[2*i+1]-0x41+10));
        }else if(hexCh[2*i+1]>=0x61 && hexCh[2*i+1]<=0x66) {
            asc[i] = (asc[i] | (hexCh[2*i+1]-0x61+10));
        }else {
            return null;
        }
        
        
    }
    
    return asc;
}

Hex.utf8StrToHex = function(utf8Str){
	var ens = encodeURIComponent(utf8Str);
	var es = unescape(ens);
	
	
	var esLen = es.length;

    // Convert
    var words = [];
    for (var i = 0; i < esLen; i++) {
        words[i] = (es.charCodeAt(i).toString(16));
    }
    return words.join('');
}

Hex.utf8StrToBytes = function(utf8Str){
	var ens = encodeURIComponent(utf8Str);
	var es = unescape(ens);
	
	
	var esLen = es.length;

    // Convert
    var words = [];
    for (var i = 0; i < esLen; i++) {
        words[i] = es.charCodeAt(i);
    }
    return words;
}

Hex.hexToUtf8Str = function(utf8Str){
	
	var utf8Byte = Hex.decode(utf8Str);
    var latin1Chars = [];
    for (var i = 0; i < utf8Byte.length; i++) {
        latin1Chars.push(String.fromCharCode(utf8Byte[i]));
    }
    return decodeURIComponent(escape(latin1Chars.join('')));
}

Hex.bytesToUtf8Str = function(bytesArray){
	
	var utf8Byte = bytesArray;
    var latin1Chars = [];
    for (var i = 0; i < utf8Byte.length; i++) {
        latin1Chars.push(String.fromCharCode(utf8Byte[i]));
    }
    return decodeURIComponent(escape(latin1Chars.join('')));
}

Hex.toCharCodeArray = function(chs){
    var chArr = new Array(chs.length);
    for(var i = 0;i<chs.length;i++){
        chArr[i] = chs.charCodeAt(i);
    }
    return chArr;
}