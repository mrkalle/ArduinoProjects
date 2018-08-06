var Blynk = require('blynk-library');
var request = require('request');
var AUTH = '...';

var blynk = new Blynk.Blynk(AUTH, options = { connector: new Blynk.TcpClient() });

var v1 = new blynk.VirtualPin(1);

v1.on('write', function(param) {
	var input = param[0];
	
	if (input == "1" || input == 1) {
		request("http://localhost:5005/clipall/doorbell-1.mp3/50", function(error, response, body) {});
	}
});
