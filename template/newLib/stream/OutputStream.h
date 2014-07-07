#include "StreamBase.h"
class OutputStream : public StreamBase {

    protected:
      OutputStream& opWrite(void *buffer,uint32_t bufferSize);

    public:
      virtual ~OutputStream() {}

      // some convenience operators when you don't care about errors or endian-ness

      OutputStream& operator<<(int8_t c);
      OutputStream& operator<<(int16_t c);
      OutputStream& operator<<(int32_t c);
      OutputStream& operator<<(uint8_t c);
      OutputStream& operator<<(uint16_t c);
      OutputStream& operator<<(uint32_t c);
      OutputStream& operator<<(const char *string);

      /**
       * Write a byte to the stream.
       * @param[in] c The byte to write.
       * @return false if it fails.
       */

      virtual bool write(uint8_t c)=0;

      /**
       * Write a buffer of data to the stream.
       * @param[in] buffer The buffer of data to write.
       * @param[in] size The number of bytes to write.
       * @return false if the write fails.
       */

      virtual bool write(const void *buffer,uint32_t size)=0;

      /**
       * Flush any cached data to the stream. If the stream does not support
       * caching then it returns true.
       * @return false if it fails.
       */

      virtual bool flush()=0;
  };