# RsyncBackup
自研目录备份还原工具，基于rsync开发

# 基本介绍
具体的功能设计查看 Rsync备份还原功能设计.png
以及相关基础知识 https://jailbreakfox.github.io/2022/07/10/Rsync备份还原/

# 编译方法(不依赖任何库)
```sh
# 1. 使用本地备份方式
$ g++ rsyncbackupclient.cpp RsyncExec.cpp -o RsyncBackupClient

# 2. 使用远程备份方式
$ g++ rsyncbackupclient.cpp RsyncExec.cpp -o RsyncBackupClient
$ g++ rsyncbackupserver.cpp RsyncConfig.cpp -o RsyncBackupServer
```

# 使用方法
在使用前请根据自身情况修改rsyncbackupclient.cpp、rsyncbackupserver.cpp
在编译完成后，使用以下命令进行备份或还原操作
```sh
# 本地 备份/还原 文件
$ ./RsyncBackupClient --BackupLocal
$ ./RsyncBackupClient --RestoreLocal

# 注意在使用远程功能前，需要在存储服务器内执行
$ ./RsyncBackupServer
# 当需要关闭服务的时候，直接手动kill即可

# 远程 备份/还原 文件
$ ./RsyncBackupClient --BackupRemote '存储服务器IP'
$ ./RsyncBackupClient --RestoreRemote '存储服务器IP'
```
