#ifndef STREAM_H
#define STREAM_H
#include <stm32f10x.h>
class StreamBase {

    protected:
      virtual ~StreamBase() {}

    public:

      /**
       * Error codes
       */

      enum {
        /// End of stream has been reached
        E_END_OF_STREAM=-1,

        /// An error occurred while reading from the stream
        E_STREAM_ERROR=-2,

        /// The operation is not supported
        E_OPERATION_NOT_SUPPORTED=-3
      };

    public:

      /**
       * Close the stream. If the stream does not support the operation
       * then it returns true.
       * @return false if it fails.
       */

      virtual bool close()=0;
  };
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
#endif