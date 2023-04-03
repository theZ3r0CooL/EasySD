#include "EasySD.h"
#include "esp_camera.h"
#include "Arduino.h"

#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

EasySD sdCard(5); // change to the appropriate chip select pin

const int MAX_SNAPSHOTS = 10; // maximum number of snapshots to save
const char* FILE_PREFIX = "/snapshot"; // prefix for the snapshot filenames
const char* FILE_EXTENSION = ".jpg"; // extension for the snapshot filenames

int numSnapshots = 0; // current number of saved snapshots

void setup() {
  #ifdef ARDUINO_ARCH_ESP32
    // esp32
    Serial.begin(115200);
  #else
    // esp8266
    Serial.begin(9600);
  #endif

  // initialize the camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // initialize the SD card
  if (!sdCard.begin()) {
    Serial.println("SD card initialization failed");
    return;
  }

  // count how many snapshot files already exist on the SD card
  int fileCount = 0;
  for (int i = 1; i <= MAX_SNAPSHOTS; i++) {
    char filename[20];
    sprintf(filename, "%s%d%s", FILE_PREFIX, i, FILE_EXTENSION);
    if (sdCard.fileExists(filename)) {
      fileCount++;
    } else {
      break;
    }
  }
  numSnapshots = fileCount;

  Serial.printf("Found %d snapshot files on SD card\n", numSnapshots);
}

void loop() {
  // take a snapshot
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  // create the filename for the snapshot
  numSnapshots++;
  if (numSnapshots > MAX_SNAPSHOTS) {
    numSnapshots = 1;
  }
  char filename[20];
  sprintf(filename, "%s%d%s", FILE_PREFIX, numSnapshots, FILE_EXTENSION);

  // save the snapshot to the SD card
  if (!sdCard.saveFile(filename, (const uint8_t*)fb->buf, fb->len)) {
    Serial.printf("Failed to save snapshot to file: %s\n", filename);
  } else {
    Serial.printf("Saved snapshot to file: %s\n", filename);
  }

  // free the memory used by the snapshot buffer
  esp_camera_fb_return(fb);

  // check if we need to delete the oldest snapshot file
  if (numSnapshots == MAX_SNAPSHOTS && sdCard.fileExists(FILE_PREFIX + "1" + FILE_EXTENSION)) {
    sdCard.deleteFile(FILE_PREFIX + "1" + FILE_EXTENSION);
    Serial.println("Deleted oldest snapshot file");
  }

  delay(5000); // wait 5 seconds before taking the next snapshot
}