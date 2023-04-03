#ifndef EASYSDFILE_H
#define EASYSDFILE_H

#include <SPI.h>
#include <FS.h>
#include <SD.h>

class EasySD {
  public:
    EasySD(int cs_pin);

    bool begin();
    bool openFile(const char* filename, const char* mode);
    void closeFile();
    bool fileExists(const char* filename);
    size_t getFileSize(const char* filename);
    size_t writeToFile(const uint8_t* buffer, size_t size);
    size_t readFromFile(uint8_t* buffer, size_t size);
    bool deleteFile(const char* filename);

    // enum OpenMode {
    //   READ,
    //   WRITE,
    //   APPEND
    // };


  private:
    File file;
    int chipSelectPin;
};

#endif