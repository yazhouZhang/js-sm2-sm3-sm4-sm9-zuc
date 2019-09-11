#!/usr/bin/env node
if (process.platform === 'ios' || process.platform === 'android') {
	console.log(`android or ios use javascript crypt`)
	process.exit();
}
var cp = require('child_process'),
	fs = require('fs'),
	path = require('path');

// Parse args
var force = false, debug = false;
var
	arch = process.arch,
	platform = process.platform;
var args = process.argv.slice(2).filter(function(arg) {
	if (arg === '-f') {
		force = true;
		return false;
	} else if (arg.substring(0, 13) === '--target_arch') {
		arch = arg.substring(14);
	} else if (arg === '--debug') {
		debug = true;
	}
	return true;
});
if (!debug) {
	args.push('--release');
}
if (!{ia32: true, x64: true, arm: true, arm64: true, ppc: true, ppc64: true, s390: true, s390x: true}.hasOwnProperty(arch)) {
	console.error('Unsupported (?) architecture: `'+ arch+ '`');
}

// Test for pre-built library
var modPath = platform+ '-'+ arch+ '-'+ process.versions.modules;
if (!force) {
	try {
		fs.statSync(path.join(__dirname, 'bin', modPath, 'softcrypt.node'));
		console.log('`'+ modPath+ '` exists; testing');
		cp.execFile(process.execPath, ['quick-test'], function(err, stdout, stderr) {
			if (err || stdout !== 'pass' || stderr) {
				console.log('Problem with the binary; manual build incoming');
				build();
			} else {
				console.log('Binary is fine; exiting');
			}
		});
	} catch (ex) {
		// Stat failed
		build();
	}
} else {
	build();
}

// Build it
function build() {
	if (process.versions.electron) {
		args.push('--target='+ process.versions.electron,  '--dist-url=https://atom.io/download/atom-shell');
	}
	cp.spawn(
		process.platform === 'win32' ? 'node-gyp.cmd' : 'node-gyp',
		['rebuild'].concat(args),
		{ stdio: [process.stdin, process.stdout, process.stderr] })
		.on('exit', function (err) {
			if (err) {
				console.error(
					'node-gyp exited with code: ' + err + '\n' +
					'Please make sure you are using a supported platform and node version. If you\n' +
					'would like to compile softcrypt on this machine please make sure you have setup your\n' +
					'build environment--\n' +
					'Windows + OS X instructions here: https://github.com/nodejs/node-gyp\n' +
					'Ubuntu users please run: `sudo apt-get install g++ build-essential`\n' +
					'Alpine users please run: `sudo apk add python make g++`'
				);
				return;
			}
			afterBuild();
		})
		.on('error', function (err) {
			console.error(
				'node-gyp not found! Please ensure node-gyp is in your PATH--\n' +
				'Try running: `sudo npm install -g node-gyp`'
			);
			console.log(err.message);
		});
}

// Move it to expected location
function afterBuild() {
	var targetPath = path.join(__dirname, 'build', debug ? 'Debug' : 'Release', 'softcrypt.node');
	var installPath = path.join(__dirname, 'bin', modPath, 'softcrypt.node');

	try {
		if(!fs.existsSync(path.join(__dirname, 'bin'))) fs.mkdirSync(path.join(__dirname, 'bin'));
		fs.mkdirSync(path.join(__dirname, 'bin', modPath));
	} catch (ex) {
		console.log(ex)
	}

	try {
		fs.statSync(targetPath);
	} catch (ex) {
		console.error('Build succeeded but target not found');
	}

	try {
		fs.renameSync(targetPath, installPath);
	} catch (ex) {
		console.error('Build succeeded but renameSync failed');
	}
	
	console.log('Installed in `'+ installPath+ '`');
	removeALL(path.join(__dirname, 'build'))
	if (process.versions.electron) {
		process.nextTick(function() {
			require('electron').app.quit();
		});
	}
}


function removeALL(filePath) {
	if (fs.statSync(filePath).isDirectory()) {
		fs.readdirSync(filePath).forEach(function(fileName){
			removeALL(path.join(filePath, fileName))
		})
		fs.rmdirSync(filePath)
	} else {
		fs.unlinkSync(filePath)
	}
}