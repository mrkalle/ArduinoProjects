// V1 input pin. Blynk app virtual button issues voice command on Raspberry Pi.
// V3 output pin. Pin controls Blynk app virtual LED, on/off every 200ms.
// V9 output pin. Pin controls Blynk app virtaul display, sends counter value.

var BlynkLib = require('blynk-library');
var http = require('http');

var blynk = new BlynkLib.Blynk('<AUTH KEY>');
var v1 = new blynk.VirtualPin(1); 
var v3 = new blynk.VirtualPin(3); 
var v9 = new blynk.VirtualPin(9);

// Write to LED in Blynk app every 200ms
var on = false;
setInterval(function(){
  if (on) {
    v3.write(255);
  } else {
    v3.write(0);
  }
  
  on = on == false;
}, 200);

// Blynk app writes on button press
v1.on('write', function(param) {
  http.get('http://localhost:5005/Mat/say/HOHO/sv-se/30', (res) => { console.log("Voice command OK"); });
});

// Blynk app reads every X time interval (set time in app)
var counter = 0;
v9.on('read', function() {
  v9.write(counter++);
});
