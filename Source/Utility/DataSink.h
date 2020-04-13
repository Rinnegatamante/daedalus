#ifndef UTILITY_DATASINK_H_
#define UTILITY_DATASINK_H_

#include <stdio.h>

class DataSink {
public:
  virtual ~DataSink();
  virtual size_t Write(const void *p, size_t len) = 0;
  virtual void Flush() = 0;
};

class FileSink : public DataSink {
public:
  FileSink();
  ~FileSink() override;

  bool Open(const char *filename, const char *mode);

  size_t Write(const void *p, size_t len) override;
  void Flush() override;

private:
  FILE *Handle;
};

#endif // UTILITY_DATASINK_H_
