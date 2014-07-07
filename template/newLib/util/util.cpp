#include <stm32f10x.h>
#include "util/util.h"
#include <math.h>
using namespace std;
namespace Util {

    /**
     * Convert input values to lower case hex (like bin2hex).
     * @param buffer The input values
     * @param bufferSize How many bytes to convert
     * @param output Where to store output. Requires 2x bufferSize bytes available.
     */
		//获取给点地址开始bufferSize个字节的原始值（HEX格式）
		/*
		int x=3;
		Util::toHex(&x,3,buf); => buf= 0x00 0x03 0x00 0x00 0x00 0x00 0x00 0x00
		*/
    void toHex(const void *buffer,uint32_t bufferSize,char *output) {

      const char *ptr;
      static const char *hexSet="0123456789abcdef";

      ptr=reinterpret_cast<const char *>(buffer);
      while(bufferSize--) {

        *output++=hexSet[(*ptr & 0xf0) >> 4];
        *output++=hexSet[*ptr & 0xf];
        ptr++;
      }
    }


    /*
     * itoa - not present in standard libraries
     */
		//int转char*，base代表输出进制
    char* itoa(int value,char* result,int base) {

      char* ptr=result,*ptr1=result,tmp_char;
      int tmp_value;

      do {
        tmp_value=value;
        value/=base;
        *ptr++="zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35+(tmp_value-value*base)];
      } while(value);

      // Apply negative sign

      if(tmp_value<0)
        *ptr++='-';

      *ptr--='\0';

      while(ptr1<ptr) {
        tmp_char=*ptr;
        *ptr--=*ptr1;
        *ptr1++=tmp_char;
      }
      return result;
    }


    /*
     * modp_uitoa10 (from stringencoders library)
     */
			//10进制int转char*
    uint32_t modp_uitoa10(uint32_t value,char *str) {

      char *wstr=str;
      uint32_t size;

      // Conversion. Number is reversed.

      do *wstr++ = (char)(48 + (value % 10)); while (value /= 10);
      *wstr='\0';

      // andy's mod: reverse the string in place. this is probably optimal.

      size=wstr-str;
      wstr--;

      while(str<wstr) {

        *str^=*wstr;
        *wstr^=*str;
        *str^=*wstr;

        wstr--;
        str++;
      }

      return size;
    }


    /*
     * Derived from http://code.google.com/p/stringencoders/source/browse/trunk/src/modp_numtoa.c
     * "string encoders: A collection of high performance c-string transformations".
     *
     * The following license applies:
     *
     * MODP_B64 - High performance base64 encoder/decoder
     * http://code.google.com/p/stringencoders/
     *
     * Copyright (c) 2005, 2006, 2007  Nick Galbreath -- nickg [at] modp [dot] com
     * All rights reserved.
     *
     * Redistribution and use in source and binary forms, with or without
     * modification, are permitted provided that the following conditions are
     * met:
     *
     *   Redistributions of source code must retain the above copyright
     *   notice, this list of conditions and the following disclaimer.
     *
     *   Redistributions in binary form must reproduce the above copyright
     *   notice, this list of conditions and the following disclaimer in the
     *   documentation and/or other materials provided with the distribution.
     *
     *   Neither the name of the modp.com nor the names of its
     *   contributors may be used to endorse or promote products derived from
     *   this software without specific prior written permission.
     *
     * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
     * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
     * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
     * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
     * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
     * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
     * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
     * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
     * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
     * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
     * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
     *
     * This is the standard "new" BSD license:
     * http://www.opensource.org/licenses/bsd-license.php
     */
	
		//double转char*，小数点不要太长
    uint16_t modp_dtoa(double value,int8_t prec,char *str) {

      static const double pow10[] = { 1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000 };
      uint16_t length;

      if(!(value==value)) {

        str[0]='n';
        str[1]='a';
        str[2]='n';
        str[3]='\0';
        return 3;
      }

      int8_t count;
      double diff=0.0;
      char *wstr=str;

    // we'll work in positive values and deal with the negative sign issue later

      bool neg=false;

      if(value<0) {
        neg=true;
        value=-value;
      }

      uint32_t whole=(uint32_t)value;
      double tmp=(value-whole)*pow10[prec];
      uint32_t frac=(uint32_t)(tmp);

      diff=tmp-frac;

      if(diff>0.5) {

        ++frac;

        // handle rollover, e.g.  case 0.99 with prec 1 is 1.0

        if(frac>=pow10[prec]) {
          frac=0;
          ++whole;
        }

      } else if(diff==0.5 && ((frac==0) || (frac & 1))) {

        // if halfway, round up if odd, OR if last digit is 0.  That last part is strange

        ++frac;
      }

      if(prec==0) {

        diff=value-whole;

        if(diff>0.5) {
        // greater than 0.5, round up, e.g. 1.6 -> 2

          ++whole;

        } else if(diff==0.5 && (whole & 1)) {

          // exactly 0.5 and ODD, then round up
          // 1.5 -> 2, but 2.5 -> 2 */

          ++whole;
        }

      } else if(frac) {

        count=prec;

        // now do fractional part, as an unsigned number
        // we know it is not 0 but we can have leading zeros, these should be removed

        while(!(frac % 10)) {
          --count;
          frac/=10;
        }

      // now do fractional part, as an unsigned number

        do {
          --count;
          *wstr++=(char)(48 + (frac % 10));
        } while(frac/=10);

        // add extra 0s

        while(count-- > 0)
          *wstr++='0';

        // add decimal

        *wstr++='.';
      }

      // do whole part. Take care of sign conversion. Number is reversed.

      do {
        *wstr++=(char)(48 + (whole % 10));
      } while(whole/=10);

      if(neg)
        *wstr++='-';

      *wstr='\0';
      length=wstr-str;

      // andy's mod: reverse the string in place. this is probably optimal.

      wstr--;

      while(str<wstr) {

        *str^=*wstr;
        *wstr^=*str;
        *str^=*wstr;

        wstr--;
        str++;
      }

      return length;
    }

double factorial(double i)
{
  double Height;
  static const double dv0[170] = { 1.0, 2.0, 6.0, 24.0, 120.0, 720.0, 5040.0,
    40320.0, 362880.0, 3.6288E+6, 3.99168E+7, 4.790016E+8, 6.2270208E+9,
    8.71782912E+10, 1.307674368E+12, 2.0922789888E+13, 3.55687428096E+14,
    6.402373705728E+15, 1.21645100408832E+17, 2.43290200817664E+18,
    5.109094217170944E+19, 1.1240007277776077E+21, 2.5852016738884978E+22,
    6.2044840173323941E+23, 1.5511210043330986E+25, 4.0329146112660565E+26,
    1.0888869450418352E+28, 3.0488834461171384E+29, 8.8417619937397008E+30,
    2.6525285981219103E+32, 8.2228386541779224E+33, 2.6313083693369352E+35,
    8.6833176188118859E+36, 2.9523279903960412E+38, 1.0333147966386144E+40,
    3.7199332678990118E+41, 1.3763753091226343E+43, 5.23022617466601E+44,
    2.0397882081197442E+46, 8.1591528324789768E+47, 3.3452526613163803E+49,
    1.4050061177528798E+51, 6.0415263063373834E+52, 2.6582715747884485E+54,
    1.1962222086548019E+56, 5.5026221598120885E+57, 2.5862324151116818E+59,
    1.2413915592536073E+61, 6.0828186403426752E+62, 3.0414093201713376E+64,
    1.5511187532873822E+66, 8.0658175170943877E+67, 4.2748832840600255E+69,
    2.3084369733924138E+71, 1.2696403353658276E+73, 7.1099858780486348E+74,
    4.0526919504877221E+76, 2.3505613312828789E+78, 1.3868311854568986E+80,
    8.3209871127413916E+81, 5.0758021387722484E+83, 3.1469973260387939E+85,
    1.98260831540444E+87, 1.2688693218588417E+89, 8.2476505920824715E+90,
    5.4434493907744307E+92, 3.6471110918188683E+94, 2.4800355424368305E+96,
    1.711224524281413E+98, 1.197857166996989E+100, 8.5047858856786218E+101,
    6.1234458376886077E+103, 4.4701154615126834E+105, 3.3078854415193856E+107,
    2.4809140811395391E+109, 1.8854947016660498E+111, 1.4518309202828584E+113,
    1.1324281178206295E+115, 8.9461821307829729E+116, 7.1569457046263779E+118,
    5.7971260207473655E+120, 4.75364333701284E+122, 3.9455239697206569E+124,
    3.314240134565352E+126, 2.8171041143805494E+128, 2.4227095383672724E+130,
    2.1077572983795269E+132, 1.8548264225739836E+134, 1.6507955160908452E+136,
    1.4857159644817607E+138, 1.3520015276784023E+140, 1.24384140546413E+142,
    1.1567725070816409E+144, 1.0873661566567424E+146, 1.0329978488239052E+148,
    9.916779348709491E+149, 9.6192759682482062E+151, 9.426890448883242E+153,
    9.33262154439441E+155, 9.33262154439441E+157, 9.4259477598383536E+159,
    9.6144667150351211E+161, 9.9029007164861754E+163, 1.0299016745145622E+166,
    1.0813967582402903E+168, 1.1462805637347078E+170, 1.2265202031961373E+172,
    1.3246418194518284E+174, 1.4438595832024928E+176, 1.5882455415227421E+178,
    1.7629525510902437E+180, 1.9745068572210728E+182, 2.2311927486598123E+184,
    2.5435597334721862E+186, 2.9250936934930141E+188, 3.3931086844518965E+190,
    3.969937160808719E+192, 4.6845258497542883E+194, 5.5745857612076033E+196,
    6.6895029134491239E+198, 8.09429852527344E+200, 9.8750442008335976E+202,
    1.2146304367025325E+205, 1.5061417415111404E+207, 1.8826771768889254E+209,
    2.3721732428800459E+211, 3.0126600184576582E+213, 3.8562048236258025E+215,
    4.9745042224772855E+217, 6.4668554892204716E+219, 8.4715806908788174E+221,
    1.1182486511960039E+224, 1.4872707060906852E+226, 1.9929427461615181E+228,
    2.6904727073180495E+230, 3.6590428819525472E+232, 5.01288874827499E+234,
    6.9177864726194859E+236, 9.6157231969410859E+238, 1.346201247571752E+241,
    1.89814375907617E+243, 2.6953641378881614E+245, 3.8543707171800706E+247,
    5.5502938327393013E+249, 8.0479260574719866E+251, 1.17499720439091E+254,
    1.7272458904546376E+256, 2.5563239178728637E+258, 3.8089226376305671E+260,
    5.7133839564458505E+262, 8.6272097742332346E+264, 1.3113358856834518E+267,
    2.0063439050956811E+269, 3.0897696138473489E+271, 4.7891429014633912E+273,
    7.47106292628289E+275, 1.1729568794264138E+278, 1.8532718694937338E+280,
    2.9467022724950369E+282, 4.714723635992059E+284, 7.5907050539472148E+286,
    1.2296942187394488E+289, 2.0044015765453015E+291, 3.2872185855342945E+293,
    5.423910666131586E+295, 9.0036917057784329E+297, 1.5036165148649983E+300,
    2.5260757449731969E+302, 4.2690680090047027E+304, 7.257415615307994E+306 };

  if ((i < 0.0) || (floor(i) != i)) {
    Height = 0.0;
  } else if ((int)i > 170) {
    Height = 1.7976931348623157E+308;
  } else if ((int)i < 1) {
    Height = 1.0;
  } else {
    Height = dv0[(int)i - 1];
  }

  return Height;
}
}