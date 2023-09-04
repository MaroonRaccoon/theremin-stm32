#pragma once

#include "stm32f401xe.h"
#include <cstdint>

namespace timer
{

enum class Id
{
    Tim1,
    Tim2,
    Tim3,
    Tim4,
    Tim5,
    Tim9,
    Tim10,
    Tim11
};

enum class TimerMode
{
    InputCapture,
    OutputCompare
};

enum class ClockDivision
{
    InternalClock,
    InternalClock2, // 2 * t_{ck_int}
    InternalClock4  // 4 * t_{ck_int}
};

// IC1F
enum class InputCaptureFilter
{
    None,
    CkIntN2,
    CkIntN4,
    CkIntN8,
    CkDts2N6,
    CkDts2N8,
    CkDts4N6,
    CkDts4N8,
    CkDts8N6,
    CkDts8N8,
    CkDts16N5,
    CkDts16N6,
    CkDts16N8,
    CkDts32N5,
    CkDts32N6,
    CkDts32N8,
};

// IC1PSC
// capture is done every N events
enum class InputCapturePrescaler
{
    Every,
    Every2,
    Every4,
    Every8
};

enum class CaptureCompareInputPolarity
{
    RisingEdge,
    FallingEdge,
    BothEdges
};

struct BasicInputCaptureSettings
{
    ClockDivision         clockDivision;
    bool                  interruptEnable;
    InputCaptureFilter    filter;
    InputCapturePrescaler prescaler;
    CaptureCompareInputPolarity polarity;
};


} // namespace timer


class BasicInputCaptureTimer
{
  public:
    BasicInputCaptureTimer( timer::Id, timer::BasicInputCaptureSettings settings );
    void     start();
    void     stop();
    void     restart();
    void     clear_interrupt_flag();
    uint32_t read_counter();
    uint32_t read_us();

  private:
    static constexpr uint32_t           get_bus_enable_mask( timer::Id timer_id );
    static constexpr volatile uint32_t &get_bus_enable_register( timer::Id timer_id );
    static void                         enable_bus_clock( timer::Id timer_id );
    static constexpr uint32_t           get_ccmr_ic1f_mask( timer::InputCaptureFilter filter );
    static constexpr uint32_t           get_ccmr_ic1psc_mask( timer::InputCapturePrescaler prescaler );
    static constexpr uint32_t get_ccer_input_polarity_mask( timer::CaptureCompareInputPolarity polarity );

    TIM_TypeDef *tim;
};

class DelayTimer
{
  public:
    DelayTimer();
    void ms( std::uint32_t ms );
    void us( std::uint32_t us );
};
