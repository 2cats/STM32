#ifndef UTIL_H
#define UTIL_H
#include <stm32f10x.h>

namespace Util {
    char* itoa(int value,char* result,int base);
    uint32_t modp_uitoa10(uint32_t value,char *str);
    uint16_t modp_dtoa(double value,int8_t prec,char *str);
    void toHex(const void *buffer,uint32_t bufferSize,char *output);
		double factorial(double i);
struct DoublePrecision {

    enum {
      MAX_DOUBLE_FRACTION_DIGITS = 5
    };

    /**
     * The encapsulated value
     */

    double Value;

    /**
     * The number of fractional digits
     */

    uint8_t Precision;


    /**
     * Constructor
     * @param d The value to encapsulate
     * @param precision The number of fractional digits
     */

    DoublePrecision(double d,uint8_t precision) {
      Value=d;
      Precision=precision;
    }
  };

  template<typename T>
inline const T& Max(const T& a,const T& b) {
    return (a>b) ? a : b;
  }

  /**
   * Min implementation with templates
   * @param a
   * @param b
   * @return The lesser of a,b
   */

  template<typename T>
 inline const T& Min(const T& a,const T& b) {
    return (a<b) ? a : b;
  }

  /**
   * Abs implementation with templates
   * @param a negative or positive value
   * @return A positive version of the parameter
   */

  template<typename T>
  inline const T Abs(const T a) {
    return a<0 ? -a : a;
  }
}
#endif