# lz4

#### 介绍
LZ4是一个以超快的压缩/解压速度著称的压缩库。它由Yann Collet开发，压缩率与其他流行的压缩库相比也不弱。相对于较老的zlib库，虽然LZ4的压缩率只有zlib的八成左右，但压缩速率却是zlib的10倍以上，对于追求超快的压缩/解压速度的场景来说不失为一种优秀的选择。
KZL是华为内源实现的基于鲲鹏平台对lz4进行优化的代码仓。


#### 安装教程

直接执行编译脚本：sh build.sh

#### 使用说明

生成的lz4二进制文件和lib库，使用方法和开源lz4一致

假设安装鲲鹏优化版本lz4的目录是/home/kplz4/lib/
```shell
[root@localhost /home/kplz4/lib]# ll
total 1080
...
lrwxrwxrwx 1 root root     15 Aug 19 19:29 liblz4.so -> liblz4.so.1.9.3
lrwxrwxrwx 1 root root     15 Aug 19 19:29 liblz4.so.1 -> liblz4.so.1.9.3
-rwxr-xr-x 1 root root 209344 Aug 19 19:29 liblz4.so.1.9.3
...
```
可以通过替换原来lz4.so或者通过LD_LIBRARY_PATH=/kplz4_libpath/:$LD_LIBRARY_PATH的方式使用鲲鹏优化版本的lz4动态库。
可以对对于软件二进制ldd查看依赖的lz4.so是否对应优化的鲲鹏版本路径一致。

#### 测试

cd tests
make
./fullbench datagen
