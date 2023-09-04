#include "timer.hpp"

BasicInputCaptureTimer::BasicInputCaptureTimer(timer::Id id, timer::BasicInputCaptureSettings settings)
{
    using namespace timer;

    // set timer pointer
    switch(id) {
        case Id::Tim1:
            tim = TIM1;
            break;
        case Id::Tim2:
            tim = TIM2;
            break;
        case Id::Tim3:
            tim = TIM3;
            break;
        case Id::Tim4:
            tim = TIM4;
            break;
        case Id::Tim5:
            tim = TIM5;
            break;
        case Id::Tim9:
            tim = TIM9;
            break;
        case Id::Tim10:
            tim = TIM10;
            break;
        case Id::Tim11:
            tim = TIM11;
            break;
    }

    // enable bus clock
    enable_bus_clock(id);

    // configure mode
    tim->CCMR1 &= ~(TIM_CCMR1_IC1F | TIM_CCMR1_IC1PSC | TIM_CCMR1_CC1S);
    tim->CCMR1 |= get_ccmr_ic1f_mask(settings.filter) | get_ccmr_ic1psc_mask(settings.prescaler) | TIM_CCMR1_CC1S_0;

    // configure interrupt
    tim->DIER &= ~(TIM_DIER_CC1IE | TIM_DIER_UIE);
    if (settings.interruptEnable) {
        tim->DIER |= TIM_DIER_CC1IE;
        if (id == timer::Id::Tim1 || id == timer::Id::Tim10) {
            NVIC_ClearPendingIRQ(TIM1_UP_TIM10_IRQn);
            NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 4U);
            NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
        }
    }

    // configure polarity
    tim->CCER &= ~(TIM_CCER_CC1NP | TIM_CCER_CC1P | TIM_CCER_CC1E);
    tim->CCER |= get_ccer_input_polarity_mask( settings.polarity );

    // enable capture/compare
    tim->CCER |= TIM_CCER_CC1E;
}

void BasicInputCaptureTimer::restart()
{
    tim->CNT &= ~0xFFFFU;
    start();
}

void BasicInputCaptureTimer::start()
{
    tim->CR1 |= 1UL;
}

void BasicInputCaptureTimer::stop()
{
    tim->CR1 &= ~1UL;
}

uint32_t BasicInputCaptureTimer::read_counter()
{
    return tim->CCR1;
}

uint32_t BasicInputCaptureTimer::read_us()
{
    // TODO: take clock, prescaler, etc into account
    // for now, assuming default 16 MHz clock with no prescaler, capturing every event
    return read_counter() / 16;
}

void BasicInputCaptureTimer::clear_interrupt_flag()
{
    tim->SR &= ~TIM_SR_CC1IF;
}

constexpr uint32_t BasicInputCaptureTimer::get_ccer_input_polarity_mask( timer::CaptureCompareInputPolarity polarity )
{
    switch(polarity) {
        case timer::CaptureCompareInputPolarity::RisingEdge:
            return 0;
        case timer::CaptureCompareInputPolarity::FallingEdge:
            return TIM_CCER_CC1P;
        case timer::CaptureCompareInputPolarity::BothEdges:
            return TIM_CCER_CC1NP | TIM_CCER_CC1P;
    }
    return 0;
}

constexpr uint32_t BasicInputCaptureTimer::get_ccmr_ic1psc_mask(timer::InputCapturePrescaler prescaler)
{
    switch(prescaler) {
        case timer::InputCapturePrescaler::Every:
            return 0UL << TIM_CCMR1_IC1PSC_Pos;
        case timer::InputCapturePrescaler::Every2:
            return 1UL << TIM_CCMR1_IC1PSC_Pos;
        case timer::InputCapturePrescaler::Every4:
            return 2UL << TIM_CCMR1_IC1PSC_Pos;
        case timer::InputCapturePrescaler::Every8:
            return 3UL << TIM_CCMR1_IC1PSC_Pos;
    }
    return 0;
}

constexpr uint32_t BasicInputCaptureTimer::get_ccmr_ic1f_mask(timer::InputCaptureFilter filter)
{
    switch(filter) {
        case timer::InputCaptureFilter::None:
            return 0UL << TIM_CCMR1_IC1F_Pos;
        case timer::InputCaptureFilter::CkIntN2:
            return 1UL << TIM_CCMR1_IC1F_Pos;
        case timer::InputCaptureFilter::CkIntN4:
            return 2UL << TIM_CCMR1_IC1F_Pos;
        case timer::InputCaptureFilter::CkIntN8:
            return 3UL << TIM_CCMR1_IC1F_Pos;
        case timer::InputCaptureFilter::CkDts2N6:
            return 4UL << TIM_CCMR1_IC1F_Pos;
        case timer::InputCaptureFilter::CkDts2N8:
            return 5UL << TIM_CCMR1_IC1F_Pos;
        case timer::InputCaptureFilter::CkDts4N6:
            return 6UL << TIM_CCMR1_IC1F_Pos;
        case timer::InputCaptureFilter::CkDts4N8:
            return 7UL << TIM_CCMR1_IC1F_Pos;
        case timer::InputCaptureFilter::CkDts8N6:
            return 8UL << TIM_CCMR1_IC1F_Pos;
        case timer::InputCaptureFilter::CkDts8N8:
            return 9UL << TIM_CCMR1_IC1F_Pos;
        case timer::InputCaptureFilter::CkDts16N5:
            return 10UL << TIM_CCMR1_IC1F_Pos;
        case timer::InputCaptureFilter::CkDts16N6:
            return 11UL << TIM_CCMR1_IC1F_Pos;
        case timer::InputCaptureFilter::CkDts16N8:
            return 12UL << TIM_CCMR1_IC1F_Pos;
        case timer::InputCaptureFilter::CkDts32N5:
            return 13UL << TIM_CCMR1_IC1F_Pos;
        case timer::InputCaptureFilter::CkDts32N6:
            return 14UL << TIM_CCMR1_IC1F_Pos;
        case timer::InputCaptureFilter::CkDts32N8:
            return 15UL << TIM_CCMR1_IC1F_Pos;
    }
    return 0;
}

void BasicInputCaptureTimer::enable_bus_clock(timer::Id timer_id)
{
    get_bus_enable_register(timer_id) |= get_bus_enable_mask(timer_id);
}

constexpr uint32_t BasicInputCaptureTimer::get_bus_enable_mask(timer::Id timer_id)
{
    using namespace timer;
    switch(timer_id) {
        case Id::Tim1:
            return RCC_APB2ENR_TIM1EN;
        case Id::Tim2:
            return RCC_APB1ENR_TIM2EN;
        case Id::Tim3:
            return RCC_APB1ENR_TIM3EN;
        case Id::Tim4:
            return RCC_APB1ENR_TIM4EN;
        case Id::Tim5:
            return RCC_APB1ENR_TIM5EN;
        case Id::Tim9:
            return RCC_APB2ENR_TIM9EN;
        case Id::Tim10:
            return RCC_APB2ENR_TIM10EN;
        case Id::Tim11:
            return RCC_APB2ENR_TIM11EN;
    }
    return 0;
}

constexpr volatile uint32_t &BasicInputCaptureTimer::get_bus_enable_register(timer::Id timer_id)
{
    using namespace timer;
    switch(timer_id) {
        case Id::Tim2:
        case Id::Tim3:
        case Id::Tim4:
        case Id::Tim5:
            return RCC->APB1ENR;
        case Id::Tim1:
        case Id::Tim9:
        case Id::Tim10:
        case Id::Tim11:
            return RCC->APB2ENR;
    }

    // unreachable
    return RCC->APB1ENR;
}
