#include "dac.hpp"

DAC::DAC(dac::Settings settings) : spi(settings.spi)
{
}

bool DAC::write(uint16_t val)
{
    const unsigned int outputShutdownControlBit = 1 << 12;
    const unsigned int outputGainSelectionBit = 1 << 13;

    // ensure the most significant 4 bits are empty
    if (val >= 1 << 13) return false;

    val = val | (
        outputGainSelectionBit // no gain
        | outputShutdownControlBit // active mode, no shutdown
    );

    spi.write(val);

    return true;
}
