# export ARCH=arm 
# export CROSS_COMPILE=arm-none-eabi-
# make mx6qsabresd_config

#mkimage -n mx6q_4x_mt41j128.cfg.cfgtmp -T imximage -e 0x17800000 -d u-boot.bin u-boot.imx

dd if=kernel.imx of=/dev/mmcblk0 bs=512 seek=2
