/** @file sdcard.h
 *  @author Michael Hayes
 *  @date 06/08/03
 * 
 *  @brief Routines to communicate with an sdcard.
 */
 
#ifndef SDCARD_H
#define SDCARD_H


#include "config.h"
#include "iovec.h"
#include "pio.h"
#include "spi.h"


typedef struct
{
    spi_cfg_t spi;
    pio_t wp;
    uint16_t pages;
    uint16_t page_size;
    uint16_t sector_size;
} sdcard_cfg_t;    


typedef struct
{
    spi_t spi;
    uint8_t page_bits;
    uint32_t size;
    const sdcard_cfg_t *cfg;
} sdcard_dev_t;



typedef sdcard_dev_t *sdcard_t;

typedef enum
{
    SPI_FLASH_OK,
    SPI_FLASH_TIMEOUT,
    SPI_FLASH_SECTOR_INVALID,
    SPI_FLASH_BLOCK_INVALID,
} spi_flash_err_t;


typedef uint32_t sdcard_addr_t;
typedef uint32_t sdcard_size_t;
typedef int32_t sdcard_ret_t;
typedef uint16_t sdcard_block_t;


extern sdcard_ret_t
sdcard_read (sdcard_t dev, sdcard_addr_t addr,
             void *buffer, sdcard_size_t len);


/** Write to dataflash using a gather approach from a vector of
    descriptors.  The idea is to coalesce writes to the dataflash
    to minimise the number of erase operations.  */
extern sdcard_ret_t
sdcard_writev (sdcard_t dev, sdcard_addr_t addr,
               iovec_t *iov, iovec_count_t iov_count);


/** Read from dataflash using a scatter approach to a vector of
    descriptors.  */
extern sdcard_ret_t
sdcard_readv (sdcard_t dev, sdcard_addr_t addr,
              iovec_t *iov, iovec_count_t iov_count);


extern sdcard_ret_t
sdcard_write (sdcard_t dev, sdcard_addr_t addr,
              const void *buffer, sdcard_size_t len);


extern sdcard_t
sdcard_init (const sdcard_cfg_t *cfg);


extern void
sdcard_shutdown (sdcard_t dev);

#endif