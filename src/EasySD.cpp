#include "EasySD.h"

EasySD::EasySD(int cs_pin) {
  chipSelectPin = cs_pin;
}

bool EasySD::begin() {
  return SD.begin(chipSelectPin);
}

bool EasySD::openFile(const char* filename, const char* mode) {
  file = SD.open(filename, mode);
  if (!file) {
    return false;
  }

  return true;
}

void EasySD::closeFile() {
  file.close();
}

bool EasySD::fileExists(const char* filename) {
  return SD.exists(filename);
}

size_t EasySD::getFileSize(const char* filename) {
  File file = SD.open(filename, FILE_READ);
  if (!file) {
    return 0;
  }

  size_t size = file.size();
  file.close();

  return size;
}

size_t EasySD::writeToFile(const uint8_t* buffer, size_t size) {
  if (!file) {
    return 0;
  }

  return file.write(buffer, size);
}

size_t EasySD::readFromFile(uint8_t* buffer, size_t size) {
  if (!file) {
    return 0;
  }

  return file.read(buffer, size);
}

bool EasySD::deleteFile(const char* filename) {
  return SD.remove(filename);
}