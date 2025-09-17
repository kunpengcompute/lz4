# 项目介绍

LZ4是一个以超快的压缩/解压速度著称的压缩库。相对于较老的zlib库，虽然LZ4的压缩率只有zlib的八成左右，但压缩速率却是zlib的10倍以上，对于追求超快的压缩/解压速度的场景来说不失为一种优秀的选择。鲲鹏优化后的LZ4库是华为基于鲲鹏硬件平台对开源LZ4进行优化后的解压缩库，利用NEON指令、内联汇编、代码结构调整、内存预取、指令流水线排布优化等方法，实现LZ4在鲲鹏计算平台上压缩性能的提高。

# 版本说明

<table><thead align="left"><tr id="row184154974115"><th class="cellrowborder" valign="top" width="33.333333333333336%" id="mcps1.1.4.1.1"><p id="p184151099417"><a name="p184151099417"></a><a name="p184151099417"></a>Kunpeng <span>LZ4</span></p>
</th>
<th class="cellrowborder" valign="top" width="33.333333333333336%" id="mcps1.1.4.1.2"><p id="p1641514916411"><a name="p1641514916411"></a><a name="p1641514916411"></a>开源<span>LZ4</span></p>
</th>
<th class="cellrowborder" valign="top" width="33.333333333333336%" id="mcps1.1.4.1.3"><p id="p18415179184110"><a name="p18415179184110"></a><a name="p18415179184110"></a>特性</p>
</th>
</tr>
</thead>
<tbody><tr id="row1241518915419"><td class="cellrowborder" valign="top" width="33.333333333333336%" headers="mcps1.1.4.1.1 "><p id="p44151996418"><a name="p44151996418"></a><a name="p44151996418"></a>v1.0.0</p>
</td>
<td class="cellrowborder" valign="top" width="33.333333333333336%" headers="mcps1.1.4.1.2 "><p id="p941519984113"><a name="p941519984113"></a><a name="p941519984113"></a>1.9.3</p>
</td>
<td class="cellrowborder" valign="top" width="33.333333333333336%" headers="mcps1.1.4.1.3 "><p id="p8415394412"><a name="p8415394412"></a><a name="p8415394412"></a>优化压缩性能</p>
</td>
</tr>
</tbody>
</table>

# 环境部署

鲲鹏优化后的LZ4库仅支持运行在华为鲲鹏硬件平台上。

执行目录中的build.sh脚本即可编译生成相关动态库以及可执行程序，完成安装部署。

执行安装脚本后，会编译生成相关动态库以及可执行程序。

动态库位于当前源码目录的lz4-1.9.3/lib子目录中：

```
lrwxrwxrwx 1 root root     15 Sep 16 17:03 liblz4.so -> liblz4.so.1.9.3
lrwxrwxrwx 1 root root     15 Sep 16 17:03 liblz4.so.1 -> liblz4.so.1.9.3
-rwxr-xr-x 1 root root 209344 Sep 16 17:03 liblz4.so.1.9.3
```

可执行程序位于源码目录的lz4-1.9.3子目录中：

```
-rwxr-xr-x 1 root root 283120 Sep 16 17:03 lz4
```

# 快速上手

安装生成的LZ4二进制文件以及相关动态库的使用方法和开源lz4均保持一致。若鲲鹏优化版本LZ4动态库位于/home/kplz4/lib下，可以通过替换原有动态库或者设置环境变量LD\_LIBRARY\_PATH=/home/kplz4/lib/:$LD\_LIBRARY\_PATH的方式使用鲲鹏优化版本的lz4动态库。

可以使用ldd命令查看上层软件程序所依赖的lz4动态库路径是否与鲲鹏优化版本lz4动态库保持一致。

```
[root@localhost lzbench]# ldd lzbench
	linux-vdso.so.1 (0x0000ffff94bbd000)
	libz.so.1 => /usr/lib64/libz.so.1 (0x0000ffff94b4f000)
	libzstd.so.1 => /usr/lib64/libzstd.so.1 (0x0000ffff94a4e000)
	liblz4.so.1 => /home/kplz4/lib/liblz4.so.1 (0x0000ffff94a0d000)
	libstdc++.so.6 => /usr/lib64/libstdc++.so.6 (0x0000ffff94817000)
	libm.so.6 => /usr/lib64/libm.so.6 (0x0000ffff94776000)
	libgcc_s.so.1 => /usr/lib64/libgcc_s.so.1 (0x0000ffff94745000)
	libc.so.6 => /usr/lib64/libc.so.6 (0x0000ffff94596000)
	/lib/ld-linux-aarch64.so.1 (0x0000ffff94b80000)
```

# 安装后验证

运行lz4-1.9.3子目录下的LZ4可执行程序，打印版本信息，回显如下：

```
[root@localhost lz4-1.9.3]# ./lz4 -V
*** LZ4 command line interface 64-bits v1.9.3, by Yann Collet ***
*** This version is optimized by Kunpeng, based on the open source lz4. ***
```

# 贡献指南

如果使用过程中有任何问题，或者需要反馈特性需求和bug报告，可以提交isssues联系我们，具体贡献方法可参考[这里](https://gitcode.com/boostkit/community/blob/master/docs/contributor/contributing.md)。

# 免责声明

此代码仓计划参与lz4软件开源，仅作性能提升，编码风格遵照原生开源软件，继承原生开源软件安全设计，不破坏原生开源软件设计及编码风格和方式，软件的任何漏洞与安全问题，均由相应的上游社区根据其漏洞和安全响应机制解决。请密切关注上游社区发布的通知和版本更新。鲲鹏计算社区对软件的漏洞及安全问题不承担任何责任。

