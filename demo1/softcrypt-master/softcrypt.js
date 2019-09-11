if(process.softcrypt) {
	module.exports = process.softcrypt
} else {
	var fs = require('fs'),
		path = require('path');

	// Look for binary for this platform
	var modPath = path.join(__dirname, 'bin', process.platform + '-' + process.arch + '-' + process.versions.modules, 'softcrypt');

	var softcrypt = null
	try {
		softcrypt = require(modPath)
	} catch (ex) {
		softcrypt = require('./jssrc')
	}

	process.softcrypt = module.exports = softcrypt;
}
