#ifndef __INC_CLOCKLESS_ARM_STM32_H
#define __INC_CLOCKLESS_ARM_STM32_H

FASTLED_NAMESPACE_BEGIN
// Definition for a single channel clockless controller for the stm32 family of chips, like that used in the spark core
// See clockless.h for detailed info on how the template parameters are used.

#define FASTLED_HAS_CLOCKLESS 1

template <int DATA_PIN, int T1, int T2, int T3, EOrder RGB_ORDER = RGB, int XTRA0 = 0, bool FLIP = false, int WAIT_TIME = 50>
class ClocklessController : public CPixelLEDController<RGB_ORDER> {
  typedef typename FastPin<DATA_PIN>::port_ptr_t data_ptr_t;
  typedef typename FastPin<DATA_PIN>::port_t data_t;

  data_t mPinMask;
  data_ptr_t mPort;
  CMinWait<WAIT_TIME> mWait;
public:
  virtual void init() {
    FastPin<DATA_PIN>::setOutput();
    mPinMask = FastPin<DATA_PIN>::mask();
    mPort = FastPin<DATA_PIN>::port();
  }

	virtual uint16_t getMaxRefreshRate() const { return 400; }

protected:

  virtual void showPixels(PixelController<RGB_ORDER> & pixels) {
    mWait.wait();
    if(!showRGBInternal(pixels)) {
      sei(); delayMicroseconds(WAIT_TIME); cli();
      showRGBInternal(pixels);
    }
    mWait.mark();
  }

#define _CYCCNT (*(volatile uint32_t*)(0xE0001004UL))

  template<int BITS> __attribute__ ((always_inline)) inline static void writeBits(register uint32_t & next_mark, register data_ptr_t port, register data_t hi, register data_t lo, register uint8_t & b)  {
    for(register uint32_t i = BITS-1; i > 0; i--) {
      while(_CYCCNT < (T1+T2+T3-20));
      FastPin<DATA_PIN>::fastset(port, hi);
      _CYCCNT = 4;
      if(b&0x80) {
        while(_CYCCNT < (T1+T2-20));
        FastPin<DATA_PIN>::fastset(port, lo);
      } else {
        while(_CYCCNT < (T1-10));
        FastPin<DATA_PIN>::fastset(port, lo);
      }
      b <<= 1;
    }

    while(_CYCCNT < (T1+T2+T3-20));
    FastPin<DATA_PIN>::fastset(port, hi);
    _CYCCNT = 4;

    if(b&0x80) {
      while(_CYCCNT < (T1+T2-20));
      FastPin<DATA_PIN>::fastset(port, lo);
    } else {
      while(_CYCCNT < (T1-10));
      FastPin<DATA_PIN>::fastset(port, lo);
    }
  }

#ifdef __STM32F1__

#ifdef __cplusplus
  #define   __I     volatile             /*!< Defines 'read only' permissions                 */
#else
  #define   __I     volatile const       /*!< Defines 'read only' permissions                 */
#endif
#define     __O     volatile             /*!< Defines 'write only' permissions                */
#define     __IO    volatile             /*!< Defines 'read / write' permissions              */


typedef struct
{
  __IO uint32_t DHCSR;                   /*!< Offset: 0x000 (R/W)  Debug Halting Control and Status Register    */
  __O  uint32_t DCRSR;                   /*!< Offset: 0x004 ( /W)  Debug Core Register Selector Register        */
  __IO uint32_t DCRDR;                   /*!< Offset: 0x008 (R/W)  Debug Core Register Data Register            */
  __IO uint32_t DEMCR;                   /*!< Offset: 0x00C (R/W)  Debug Exception and Monitor Control Register */
} CoreDebug_Type;

#define CoreDebug_BASE      (0xE000EDF0UL)                            /*!< Core Debug Base Address            */
#define CoreDebug           ((CoreDebug_Type *)     CoreDebug_BASE)   /*!< Core Debug configuration struct    */

#define CoreDebug_DEMCR_TRCENA_Pos         24                                             /*!< CoreDebug DEMCR: TRCENA Position */
#define CoreDebug_DEMCR_TRCENA_Msk         (1UL << CoreDebug_DEMCR_TRCENA_Pos)            /*!< CoreDebug DEMCR: TRCENA Mask */

typedef struct
{
  __IO uint32_t CTRL;                    /*!< Offset: 0x000 (R/W)  Control Register                          */
  __IO uint32_t CYCCNT;                  /*!< Offset: 0x004 (R/W)  Cycle Count Register                      */
  __IO uint32_t CPICNT;                  /*!< Offset: 0x008 (R/W)  CPI Count Register                        */
  __IO uint32_t EXCCNT;                  /*!< Offset: 0x00C (R/W)  Exception Overhead Count Register         */
  __IO uint32_t SLEEPCNT;                /*!< Offset: 0x010 (R/W)  Sleep Count Register                      */
  __IO uint32_t LSUCNT;                  /*!< Offset: 0x014 (R/W)  LSU Count Register                        */
  __IO uint32_t FOLDCNT;                 /*!< Offset: 0x018 (R/W)  Folded-instruction Count Register         */
  __I  uint32_t PCSR;                    /*!< Offset: 0x01C (R/ )  Program Counter Sample Register           */
  __IO uint32_t COMP0;                   /*!< Offset: 0x020 (R/W)  Comparator Register 0                     */
  __IO uint32_t MASK0;                   /*!< Offset: 0x024 (R/W)  Mask Register 0                           */
  __IO uint32_t FUNCTION0;               /*!< Offset: 0x028 (R/W)  Function Register 0                       */
       uint32_t RESERVED0[1];
  __IO uint32_t COMP1;                   /*!< Offset: 0x030 (R/W)  Comparator Register 1                     */
  __IO uint32_t MASK1;                   /*!< Offset: 0x034 (R/W)  Mask Register 1                           */
  __IO uint32_t FUNCTION1;               /*!< Offset: 0x038 (R/W)  Function Register 1                       */
       uint32_t RESERVED1[1];
  __IO uint32_t COMP2;                   /*!< Offset: 0x040 (R/W)  Comparator Register 2                     */
  __IO uint32_t MASK2;                   /*!< Offset: 0x044 (R/W)  Mask Register 2                           */
  __IO uint32_t FUNCTION2;               /*!< Offset: 0x048 (R/W)  Function Register 2                       */
       uint32_t RESERVED2[1];
  __IO uint32_t COMP3;                   /*!< Offset: 0x050 (R/W)  Comparator Register 3                     */
  __IO uint32_t MASK3;                   /*!< Offset: 0x054 (R/W)  Mask Register 3                           */
  __IO uint32_t FUNCTION3;               /*!< Offset: 0x058 (R/W)  Function Register 3                       */
} DWT_Type;


#define DWT_BASE            (0xE0001000UL)                            /*!< DWT Base Address                   */
#define DWT                 ((DWT_Type       *)     DWT_BASE      )   /*!< DWT configuration struct           */

#define DWT_CTRL_CYCCNTENA_Pos              0                                          /*!< DWT CTRL: CYCCNTENA Position */
#define DWT_CTRL_CYCCNTENA_Msk             (0x1UL << DWT_CTRL_CYCCNTENA_Pos)           /*!< DWT CTRL: CYCCNTENA Mask */

#endif

  // This method is made static to force making register Y available to use for data on AVR - if the method is non-static, then
  // gcc will use register Y for the this pointer.
  static uint32_t showRGBInternal(PixelController<RGB_ORDER> pixels) {
    // Get access to the clock
    CoreDebug->DEMCR  |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    DWT->CYCCNT = 0;

    register data_ptr_t port = FastPin<DATA_PIN>::port();
    register data_t hi = *port | FastPin<DATA_PIN>::mask();;
    register data_t lo = *port & ~FastPin<DATA_PIN>::mask();;
    *port = lo;

    // Setup the pixel controller and load/scale the first byte
    pixels.preStepFirstByteDithering();
    register uint8_t b = pixels.loadAndScale0();

    cli();

    uint32_t next_mark = (T1+T2+T3);

    DWT->CYCCNT = 0;
    while(pixels.has(1)) {
      pixels.stepDithering();
      #if (FASTLED_ALLOW_INTERRUPTS == 1)
      cli();
      // if interrupts took longer than 45µs, punt on the current frame
      if(DWT->CYCCNT > next_mark) {
        if((DWT->CYCCNT-next_mark) > ((WAIT_TIME-INTERRUPT_THRESHOLD)*CLKS_PER_US)) { sei(); return 0; }
      }

      hi = *port | FastPin<DATA_PIN>::mask();
      lo = *port & ~FastPin<DATA_PIN>::mask();
      #endif

      // Write first byte, read next byte
      writeBits<8+XTRA0>(next_mark, port, hi, lo, b);
      b = pixels.loadAndScale1();

      // Write second byte, read 3rd byte
      writeBits<8+XTRA0>(next_mark, port, hi, lo, b);
      b = pixels.loadAndScale2();

      // Write third byte, read 1st byte of next pixel
      writeBits<8+XTRA0>(next_mark, port, hi, lo, b);
      b = pixels.advanceAndLoadAndScale0();
      #if (FASTLED_ALLOW_INTERRUPTS == 1)
      sei();
      #endif
    };

    sei();
    return DWT->CYCCNT;
  }
};

FASTLED_NAMESPACE_END

  #endif
