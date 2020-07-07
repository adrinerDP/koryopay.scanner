import processing.serial.*; //<>//
import processing.sound.*;
import http.requests.*;

Serial myPort;
SoundFile beep;
SoundFile success;
SoundFile locked;
SoundFile unregistered;

void setup() {
  printArray(Serial.list());
  myPort = new Serial(this, Serial.list()[4], 9600);
  // Sound Files
  beep         = new SoundFile(this, "BEEP.mp3");
  success      = new SoundFile(this, "200.mp3");
  locked       = new SoundFile(this, "400.mp3");
  unregistered = new SoundFile(this, "404.mp3");
}

void draw() {
  while (myPort.available() > 0) {
    String buffer = myPort.readStringUntil(10);
    if (buffer != null) {
      newCardPresent(buffer);
    }
  }
}

void newCardPresent(String buffer) {
  String fingerprint = formatFingerprint(buffer);
  String result = updateFingerprint(fingerprint);
  beep.play();
  delay(100);
  switch (result) {
    case "UPDATED":
      delay(400);
      success.play();
      break;
    case "LOCKED":
      beep.play();
      delay(500);
      locked.play();
      break;
    case "UNREGISTERED":
      beep.play();
      delay(500);
      unregistered.play();
      break;
  }
}

String updateFingerprint(String fingerprint) {
  GetRequest update = new GetRequest("http://koryopay.tk/activity/update?fingerprint=" + trim(fingerprint));
  update.send();
  return update.getContent();
}

String formatFingerprint(String buffer) {
  buffer = trim(buffer);
  String A = buffer.substring(0,2);
  String B = buffer.substring(2,4);
  String C = buffer.substring(4,6);
  String D = buffer.substring(6,8);
  return A + ":" + B + ":" + C + ":" + D;
}
