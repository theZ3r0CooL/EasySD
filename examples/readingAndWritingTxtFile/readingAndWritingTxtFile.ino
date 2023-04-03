#include <EasySD.h>

EasySD sdCard(5); // Create an EasySD object on pin 5

void setup() {
  #ifdef ARDUINO_ARCH_ESP32
    // esp32
    Serial.begin(115200);
  #else
    // esp8266
    Serial.begin(9600);
  #endif
  while (!Serial) {
    ; // Wait for serial port to connect
  }

  if (!sdCard.begin()) {
    Serial.println("Error: SD card initialization failed");
    return;
  }

  Serial.println("SD card initialized");
}

void loop() {
  if (sdCard.fileExists("test.txt")) {
    Serial.println("test.txt exists on SD card");
  } else {
    Serial.println("test.txt does not exist on SD card");
  }

  if (sdCard.openFile("test.txt", FILE_WRITE)) {
    Serial.println("test.txt opened for writing");

    const char* message = "Hello, world!";
    size_t bytesWritten = sdCard.writeToFile((const uint8_t*) message, strlen(message));

    if (bytesWritten > 0) {
      Serial.print("Wrote ");
      Serial.print(bytesWritten);
      Serial.println(" bytes to test.txt");
    } else {
      Serial.println("Error writing to test.txt");
    }

    sdCard.closeFile();
  } else {
    Serial.println("Error opening test.txt for writing");
  }

  delay(5000); // Wait 5 seconds before checking for file again
}