注意事项：

# 1 位宽选择
老的固件版本中，SD总线选择有问题，即使选择的4位，实际上用的还是1位，详细见：
* https://community.st.com/s/question/0D53W000020H9NGSA0/sdcard-does-not-work-frnotready-when-migrating-from-fwf4-v1270-to-newer-fwf4-v1271
* https://community.st.com/s/question/0D53W0000216T7oSAE/mxsdiosdinit-wrong-bus-wide-initialization

而默认情况下，在初始化阶段必须要用1位总线，4位总线会初始化失败。所以正确做法是在CubeMX配置成1位，然后在软件初始化完毕后，再配置成4位（1位也行，但理论上四位更快）
* https://blog.csdn.net/Veabol/article/details/51946010

# 2 文件名长度
FATFS文件系统配置时使能了长文件名，最长255字节，若不支持长文件名，则文件名最多8个字节

# 3 SDIO中断
需要开启SDIO全局中断，DMA才生效

# 4 写入SD卡
注意使用fatfs的f_sync函数同步，写效率较低，注意写优先级以及频率。

4.1 注意sd卡需要先mount然后才能open进行写入操作，且在写入的过程中不能umount，会导致写入失败。
