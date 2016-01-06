dd if=/dev/zero of=/tmp/disk1 bs=512 count=2000  生存一个文件

losetup /dev/loop0 /tmp/disk1   将文件disk1映射到设备loop上

./format_real_sfs /dev/loop0      将块设备loop0格式化为real sfs文件系统

mount -t real_sfs /dev/loop0 /mnt/sfs   用real_sfs文件系统将块设备loop0挂载到/mnt/sfs

dmesg
