#include "usb_serial.h"
#include "usb_cdc.h"
#include "tty.h"
#include "sys.h"
#include <stdlib.h>


usb_serial_t *
usb_serial_init (const usb_serial_cfg_t *cfg)
{
    usb_serial_t *dev;

    tty_cfg_t usb_cdc_tty_cfg =
        {
            .read = (void *)usb_cdc_read,
            .write = (void *)usb_cdc_write,
            .linebuffer_size = 80
        };
    const usb_cdc_cfg_t usb_cdc_cfg =
        {
            .read_timeout_us = cfg->read_timeout_us,
            .write_timeout_us = cfg->write_timeout_us,
        };

    dev = calloc (1, sizeof (*dev));
    if (! dev)
        return 0;

    dev->usb_cdc = usb_cdc_init (&usb_cdc_cfg);
    if (! dev->usb_cdc)
    {
        free (dev);
        return 0;
    }

    dev->tty = tty_init (&usb_cdc_tty_cfg, dev->usb_cdc);
    if (! dev->tty)
    {
        free (dev);
        return 0;
    }

    usb_serial_echo_set (dev, cfg->echo);
    
    sys_device_register (cfg->devname, &tty_file_ops, dev->tty);
    return dev;
}


void usb_serial_echo_set (usb_serial_t *dev, bool echo)
{
    tty_echo_set (dev->tty, echo);
}

void usb_serial_shutdown (usb_serial_t *dev)
{
    usb_cdc_shutdown ();
}
