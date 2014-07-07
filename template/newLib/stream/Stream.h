#ifndef STREAM_H
#define STREAM_H
#include <stm32f10x.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "util/util.h"
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
class InputStream : public StreamBase {

    protected:
      InputStream& opRead(void *buffer,uint32_t bufferSize);

    public:
      virtual ~InputStream() {}

      // some convenience operators when you don't care about errors or endian-ness

      InputStream& operator>>(int8_t& c);
      InputStream& operator>>(int16_t& c);
      InputStream& operator>>(int32_t& c);
      InputStream& operator>>(uint8_t& c);
      InputStream& operator>>(uint16_t& c);
      InputStream& operator>>(uint32_t& c);

      /**
       * Read a byte from the stream, potentially blocking until data
       * is available.
       *
       * @return If the read succeeds then the byte
       * is returned in the lower 8 bytes, hence the return value is in the
       * range 0..255. If the read fails then the return value is E_STREAM_ERROR.
       * If the end of the stream has been reached then the return value is
       * E_END_OF_STREAM.
       */

      virtual int16_t read()=0;

      /**
       * Read a sequence of bytes from the stream, potentially blocking
       * if data is not available.
       *
       * @param[in,out] buffer The location to store the data.
       * @param[in] size The number of bytes to try to read.
       * @param[in] actuallyRead The number of bytes actually read, which may be less than the size
       *  requested if the end of the file is reached.
       * @return false if the read fails.
       */

      virtual bool read(void *buffer,uint32_t size,uint32_t& actuallyRead)=0;

      /**
       * Seek forward by the given number of bytes.
       * @param[in] howMuch The amount to try to seek.
       * @return false if it fails.
       */

      virtual bool skip(uint32_t howMuch)=0;

      /**
       * Peek into a blocking stream and return true if data is available
       * to read. Allows the caller to prevent blocking by checking if there
       * is data available.
       *
       * @return true if there is data available, false if not.
       */

      virtual bool available()=0;


      /**
       * Seek the stream back to the beginning.
       * @return false if the stream does not support being reset or an error occurred.
       */

      virtual bool reset()=0;
  };
class TextOutputStream : public OutputStream {

    protected:
      OutputStream& _stream;

    public:

      TextOutputStream(OutputStream& stream);

      // can't do these as a template with specialisation because you can't specialise
      // members in a template class that isn't also fully specialised

      TextOutputStream& operator<<(const char *str);
      TextOutputStream& operator<<(char c);
      TextOutputStream& operator<<(int32_t val);
      TextOutputStream& operator<<(uint32_t val);
      TextOutputStream& operator<<(int16_t val);
      TextOutputStream& operator<<(uint16_t val);
		   TextOutputStream& operator<<(const DoublePrecision& val);
      TextOutputStream& operator<<(double val);

      // overrides from OutputStream

      virtual bool write(uint8_t c);
      virtual bool write(const void *buffer,uint32_t size) ;
      virtual bool close() ;
      virtual bool flush() ;
  };


  /**
   * Constructor
   * @param stream The stream that we are wrapping
   */

  inline TextOutputStream::TextOutputStream(OutputStream& stream)
    : _stream(stream) {

  }

  /**
   * character string operator
   * @param str The character string
   * @return self reference
   */

  inline TextOutputStream& TextOutputStream::operator<<(const char *str) {
    write(str,strlen(str));
    return *this;
  }


  /**
   * Character writer
   * @param c The character
   * @return self-reference
   */

  inline TextOutputStream& TextOutputStream::operator<<(char c) {
    write(c);
    return *this;
  }


  /**
   * integer writer
   * @param val The integer
   * @return self reference
   */

  inline TextOutputStream& TextOutputStream::operator<<(int32_t val) {

    char buf[15];
		StringUtil::itoa(val,buf,10);
    write(buf,strlen(buf));

    return *this;
  }


  /**
   * Unsigned int writer
   * @param val The integer
   * @return self reference
   */

  inline TextOutputStream& TextOutputStream::operator<<(uint32_t val) {

    char buf[15];
    StringUtil::modp_uitoa10(val,buf);
    write(buf,strlen(buf));

    return *this;
  }


  /**
   * Signed 16-bit writer
   * @param val The integer
   * @return self reference
   */

  inline TextOutputStream& TextOutputStream::operator<<(int16_t val) {
    return operator<<((int32_t)val);
  }


  /**
   * Unsigned 16-bit writer
   * @param val The integer
   * @return self reference
   */

  inline TextOutputStream& TextOutputStream::operator<<(uint16_t val) {
    return operator<<((uint32_t)val);
  }


  /**
   * Double writer
   * @param val The double to write
   * @return self reference
   */

  inline TextOutputStream& TextOutputStream::operator<<(const DoublePrecision& val) {

    char str[25];

    StringUtil::modp_dtoa(val.Value,val.Precision,str);
    write(str,strlen(str));
    return *this;
  }


  /**
   * Double writer
   *  @param val The double to write
   * @return self reference
   */

  inline TextOutputStream& TextOutputStream::operator<<(double val) {
    operator<<(DoublePrecision(val,DoublePrecision::MAX_DOUBLE_FRACTION_DIGITS));
    return *this;
  }


  /**
   * Write a byte - call through to the underlying stream
   * @param buffer The buffer
   * @param size The size
   * @return the underlying stream success result
   */

  inline bool TextOutputStream::write(uint8_t c) {
    return _stream.write(c);
  }


  /**
   * Write a buffer - call through to the underlying stream
   * @param buffer The buffer
   * @param size The size
   * @return the underlying stream success result
   */

  inline bool TextOutputStream::write(const void *buffer,uint32_t size) {
    return _stream.write(buffer,size);
  }


  /**
   * Close the stream (do nothing)
   * @return true
   */

  inline bool TextOutputStream::close() {
    return true;
  }


  /**
   * Flush the stream
   * @return the underlying stream flush result
   */

  inline bool TextOutputStream::flush() {
    return _stream.flush();
  }
#endif