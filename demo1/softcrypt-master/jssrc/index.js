var des, sm4;
try {
    des = require('./des.js')
} catch (error) {
    des = new DES()
}

try {
    sm4 = require('./sm4.js')
} catch (error) {
    sm4 = SM4
}

var softcrypt = {
    CalDES: des.CalDES,
    Cal919MAC: des.CalANSI919MAC,
    Cal99MAC: des.CalX99MAC,
    CalSM4: sm4.SM4CryptECB,
    CalSM4MAC: sm4.SM4MACGenerated
}

try {
    module.exports = softcrypt
} catch (error) {

}
