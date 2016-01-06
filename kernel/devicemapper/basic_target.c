#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include <linux/bio.h>
#include <linux/device-mapper.h>

/* This is a structure which will store  information about the underlying device
*  Param:
* dev : underlying device
* start:  Starting sector number of the device
*/


struct my_dm_target
{
        struct dm_dev *dev;
        sector_t start;
};

/* This is map function of basic target. This function gets called whenever you get a new bio
 * request.The working of map function is to map a particular bio request to the underlying device.
 *The request that we receive is submitted to out device so  bio->bi_bdev points to our device.
 * We should point to the bio-> bi_dev field to bdev of underlying device. Here in this function,
 * we can have other processing like changing sector number of bio request, splitting bio etc.
 *
 *  Param :
 *  ti : It is the dm_target structure representing our basic target
 *  bio : The block I/O request from upper layer
 *  map_context : Its mapping context of target.
 *
 *: Return values from target map function:
 *  DM_MAPIO_SUBMITTED :  Your target has submitted the bio request to underlying request
 *  DM_MAPIO_REMAPPED  :  Bio request is remapped, Device mapper should submit bio.
 *  DM_MAPIO_REQUEUE   :  Some problem has happened with the mapping of bio, So
 *                                                re queue the bio request. So the bio will be submitted
 *                                                to the map function
 */

static int basic_target_map(struct dm_target *ti, struct bio *bio,union map_info *map_context)
{
        struct my_dm_target *mdt = (struct my_dm_target *) ti->private;
        printk(KERN_CRIT "\n<<in function basic_target_map \n");
        bio->bi_bdev = mdt->dev->bdev;
        if((bio->bi_rw & WRITE) == WRITE)
                printk(KERN_CRIT "\n basic_target_map : bio is a write request.... \n");
        else
                printk(KERN_CRIT "\n basic_target_map : bio is a read request.... \n");
        submit_bio(bio->bi_rw,bio);
        printk(KERN_CRIT "\n>>out function basic_target_map \n");
        return DM_MAPIO_SUBMITTED;
}

/* This is Constructor Function of basic target
 *  Constructor gets called when we create some device of type 'basic_target'.
 *  So it will get called when we execute command 'dmsetup create'
 *  This  function gets called for each device over which you want to create basic
 *  target. Here it is just a basic target so it will take only one device so it
 *  will get called once.
 */

static int
basic_target_ctr(struct dm_target *ti,unsigned int argc,char **argv)
{
        struct my_dm_target *mdt;
        unsigned long long start;
        printk(KERN_CRIT "\n >>in function basic_target_ctr \n");
        if (argc != 2) {
                printk(KERN_CRIT "\n Invalid no.of arguments.\n");
                ti->error = "Invalid argument count";
                return -EINVAL;
        }

        mdt = kmalloc(sizeof(struct my_dm_target), GFP_KERNEL);
        if(mdt==NULL)
        {
                printk(KERN_CRIT "\n Mdt is null\n");
                ti->error = "dm-basic_target: Cannot allocate linear context";
                return -ENOMEM;
        }

        if(sscanf(argv[1], "%llu", &start)!=1)
        {
                ti->error = "dm-basic_target: Invalid device sector";
                goto bad;
        }

        mdt->start=(sector_t)start;


        /* dm_get_table_mode
         * Gives out you the Permissions of device mapper table.
         * This table is nothing but the table which gets created
         * when we execute dmsetup create. This is one of the
         * Data structure used by device mapper for keeping track of its devices.
         *
         * dm_get_device
         * The function sets the mdt->dev field to underlying device dev structure.
         */


        if (dm_get_device(ti, argv[0], dm_table_get_mode(ti->table), &mdt->dev)) {
                ti->error = "dm-basic_target: Device lookup failed";
                goto bad;
        }

        ti->private = mdt;
        printk(KERN_CRIT "\n>>out function basic_target_ctr \n");
        return 0;

  bad:
        kfree(mdt);
        printk(KERN_CRIT "\n>>out function basic_target_ctr with errorrrrrrrrrr \n");
        return -EINVAL;

}

/*
 * This is destruction function
 * This gets called when we remove a device of type basic target. The function gets
 * called per device.
 */

static void basic_target_dtr(struct dm_target *ti)

{

        struct my_dm_target *mdt = (struct my_dm_target *) ti->private;
        printk(KERN_CRIT "\n<<in function basic_target_dtr \n");
        dm_put_device(ti, mdt->dev);
        kfree(mdt);
        printk(KERN_CRIT "\n>>out function basic_target_dtr \n");

}

/*

 * This structure is fops for basic target.

 */

static struct target_type basic_target = {
        .name = "basic_target",
        .version = {1,0,0},
        .module = THIS_MODULE,
        .ctr = basic_target_ctr,
        .dtr = basic_target_dtr,
        .map = basic_target_map,
};


/*---------Module Functions -----------------*/

static int init_basic_target(void)
{
        int result;
        result = dm_register_target(&basic_target);
        if(result < 0)
                printk(KERN_CRIT "\n Error in registering target \n");
        return 0;
}

static void cleanup_basic_target(void)
{
        dm_unregister_target(&basic_target);
}

module_init(init_basic_target);
module_exit(cleanup_basic_target);
MODULE_LICENSE("GPL");
