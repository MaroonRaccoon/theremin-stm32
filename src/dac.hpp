#pragma once

#include "spi.hpp"

namespace dac
{
struct Settings
{
    SPIMaster &spi;
};
} // namespace dac

class DAC
{
  public:
    DAC( dac::Settings settings );
    bool write(uint16_t val); 

  private:
    SPIMaster &spi;
};
