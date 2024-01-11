#pragma once

#include "io.hpp"
#include "stm32f401xe.h"

namespace spi
{

enum class ID
{
    // underscores because SPI1, SPI2, SPI3, SPI4 are macros
    SPI_1,
    SPI_2,
    SPI_3,
    SPI_4,
};

enum class DataFrameFormat
{
    Frame8Bit,
    Frame16Bit,
};

enum class DataFrameOrder
{
    MsbFirst,
    LsbFirst,
};

enum class ClockPolarity
{
    IdleLow,
    IdleHigh,
};

enum class ClockPhase
{
    SampleFirst,
    SampleSecond,
};

struct Settings
{
    ID              id;
    GPIO           &not_chip_select;
    DataFrameFormat dataFrameFormat;
    bool            softwareSlaveManagement;
    DataFrameOrder  dataFrameOrder;
    unsigned char   baudRateControl; // resulting baud rate will be f_PCLK / 2^(baudRateControl + 1)
    ClockPolarity   clockPolarity;
    ClockPhase      clockPhase;
};

} // namespace spi

class SPIMaster
{
  public:
    SPIMaster( spi::Settings settings );
    void select_slave( bool select );
    void write( uint16_t data );
    void await_transmit_complete();

  private:
    SPI_TypeDef *spi;
    GPIO        &not_chip_select;
};
