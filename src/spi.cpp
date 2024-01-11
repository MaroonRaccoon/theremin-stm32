#include "spi.hpp"
#include "stm32f401xe.h"

SPIMaster::SPIMaster(spi::Settings settings) : not_chip_select(settings.not_chip_select)
{
    switch(settings.id) {
        case spi::ID::SPI_1:
            this->spi = SPI1;
            RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
            break;
        case spi::ID::SPI_2:
            this->spi = SPI2;
            RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
            break;
        case spi::ID::SPI_3:
            this->spi = SPI3;
            RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
            break;
        case spi::ID::SPI_4:
            this->spi = SPI4;
            RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;
            break;
    }

    { // CR1 setup
        spi->CR1 &= ~(
            SPI_CR1_CPHA
            | SPI_CR1_CPOL
            | SPI_CR1_MSTR
            | SPI_CR1_BR
            | SPI_CR1_SPE
            | SPI_CR1_LSBFIRST
            | SPI_CR1_SSI
            | SPI_CR1_SSM
            | SPI_CR1_RXONLY
            | SPI_CR1_DFF
            | SPI_CR1_CRCNEXT
            | SPI_CR1_CRCEN
            | SPI_CR1_BIDIOE
            | SPI_CR1_BIDIMODE
        );
        spi->CR1 &= ~SPI_CR1_SSI;
        spi->CR1 |= (
            (settings.clockPhase == spi::ClockPhase::SampleSecond ? SPI_CR1_CPHA : 0)
            | (settings.clockPolarity == spi::ClockPolarity::IdleHigh ? SPI_CR1_CPOL : 0)
            | SPI_CR1_MSTR // master mode
            | (settings.baudRateControl << SPI_CR1_BR_Pos)
            | (settings.dataFrameOrder == spi::DataFrameOrder::LsbFirst ? SPI_CR1_LSBFIRST : 0)
            | (settings.softwareSlaveManagement ? SPI_CR1_SSM : 0)
            | (settings.dataFrameFormat == spi::DataFrameFormat::Frame16Bit ? SPI_CR1_DFF : 0)
            | SPI_CR1_BIDIMODE // 1 line
            | SPI_CR1_BIDIOE // output-only
        );
    }

    { // CR2 setup
        spi->CR2 &= ~(
            SPI_CR2_RXDMAEN
            | SPI_CR2_TXDMAEN
            | SPI_CR2_SSOE
            | SPI_CR2_FRF
            | SPI_CR2_ERRIE
            | SPI_CR2_RXNEIE
            | SPI_CR2_TXEIE
        );
        spi->CR2 |= SPI_CR2_SSOE;
    }

    // enable
    spi->CR1 |= SPI_CR1_SPE;
}

void SPIMaster::select_slave(bool select)
{
    not_chip_select.write(select ? 0 : 1);
}

void SPIMaster::write(uint16_t data)
{
    select_slave(true);
    *reinterpret_cast<volatile uint16_t *>(&spi->DR) = data;
    await_transmit_complete();
    select_slave(false);
}

void SPIMaster::await_transmit_complete()
{
    while(spi->SR & SPI_SR_BSY_Msk);
}
