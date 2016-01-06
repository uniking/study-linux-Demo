#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

static int pen_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
 printk(KERN_INFO "Pen drive (%04X:%04X) plugged\n", id->idVendor,
        id->idProduct);
 return 0;
}

static void pen_disconnect(struct usb_interface *interface)
{
 printk(KERN_INFO "Pen drive removed\n");
}

static struct usb_device_id pen_table[] =
{
 { USB_DEVICE(0x1F75, 0x0817) }, //改为自己U盘的vid和pid
 {} /* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, pen_table);

static struct usb_driver pen_driver =
{
 .name = "pen_driver",
 .id_table = pen_table,
 .probe = pen_probe,
 .disconnect = pen_disconnect,
};

static int __init pen_init(void)
{
 return usb_register(&pen_driver);
}

static void __exit pen_exit(void)
{
 usb_deregister(&pen_driver);
}

module_init(pen_init);
module_exit(pen_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anil Kumar Pugalia <email@sarika-pugs.com>");
MODULE_DESCRIPTION("USB Pen Registration Driver");
