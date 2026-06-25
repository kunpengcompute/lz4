# LZ4介绍

## 最新消息

- [2026.06.30]：发布基于LZ4 v1.9.4版本优化的鲲鹏LZ4，包含NEON向量化内存拷贝、分支预测、数据预取等优化。

## 项目简介

### 简介

LZ4是由Yann Collet开发的**极快速无损压缩算法**，属于LZ77家族，以压缩速度著称（单核可达500MB/s以上），同时提供合理的压缩率。其核心优势为压缩/解压速度极快、内存占用小、支持流式与块式两种压缩模式，且提供了跨C、C++、Java和Python等多语言的绑定。

### 核心模块

LZ4整体分为**压缩模块**和**解压缩模块**两大核心，其内部模块划分如下：

- **压缩模块**：提供块压缩（LZ4_compress_default、LZ4_compress_fast、LZ4_compress_HC）、流式压缩（LZ4_compress_continue、LZ4_compress_fastContinue）等基础能力。
- **解压缩模块**：提供块解压（LZ4_decompress_safe、LZ4_decompress_fast）、部分解压（LZ4_decompress_safe_partial）等基础能力。
- **哈希模块**：实现4字节和5字节快速哈希计算，用于压缩时的匹配查找。
- **内存拷贝模块**：实现wildCopy8、wildCopy16、wildCopy32和wildCopy64等高效数据拷贝路径。
- **帧格式模块**：提供LZ4帧格式的压缩、解压接口（lz4frame），支持流式操作与校验。

### 对外核心接口函数

以下为LZ4（以C语言为例）对外的核心接口函数，覆盖压缩、解压缩、流式操作等核心能力。

| 模块分类 | 接口函数 | 功能描述 |
| -- | -- | -- |
| 块压缩 | `LZ4_compress_default` | 使用默认加速级别（1）压缩源数据到目标缓冲区。 |
| 块压缩 | `LZ4_compress_fast` | 使用指定加速级别（1~65537）压缩源数据，加速越高速度越快但压缩率越低。 |
| 块压缩 | `LZ4_compress_HC` | 使用高压缩率模式压缩源数据，速度较慢但压缩率更高。 |
| 块压缩 | `LZ4_compress_destSize` | 限制输出大小进行压缩，优先保证输出不超过指定大小。 |
| 块解压 | `LZ4_decompress_safe` | 安全解压，需提供原始数据大小，防止缓冲区溢出。 |
| 块解压 | `LZ4_decompress_fast` | 快速解压，需提供原始数据大小，不进行边界检查（速度更快但安全性较低）。 |
| 块解压 | `LZ4_decompress_safe_partial` | 部分安全解压，在目标缓冲区不足时解压尽可能多的数据。 |
| 流式压缩 | `LZ4_compress_continue` | 流式压缩（使用内部状态），支持多块连续压缩。 |
| 流式压缩 | `LZ4_compress_fastContinue` | 流式压缩（使用内部状态），支持指定加速级别。 |
| 帧格式 | `LZ4F_compressBegin` | 开始帧格式压缩，初始化压缩上下文。 |
| 帧格式 | `LZ4F_compressUpdate` | 帧格式压缩数据块（流式）。 |
| 帧格式 | `LZ4F_decompress` | 帧格式解压，支持流式解压。 |

## 目录结构

项目目录层级介绍如下：

```text
# 文档目录
README.md                          # 项目说明
LICENSE                            # 代码许可证
kunpeng_lz4_v1.0.patch             # 鲲鹏优化补丁文件
docs/
├── LICENSE                        # 文档许可证
└── zh/
    ├── api_reference.md           # API参考
    ├── installation_guide.md      # 安装指南
    ├── quick_start.md             # 快速入门
    ├── menu_lz4.md                # 导航菜单
    └── release_notes.md           # 版本说明书

```

## 版本说明

基于开源LZ4优化的鲲鹏LZ4的每个发布版本特性变更详细信息，请参见《[版本说明书](docs/zh/release_notes.md)》。

## 环境部署

基于开源LZ4优化的鲲鹏LZ4的编译环境、依赖获取与安装步骤参见《[安装指南](docs/zh/installation_guide.md)》。

## 快速入门

安装LZ4后如何快速上手使用LZ4请参见《[快速入门](docs/zh/quick_start.md)》。

## 文档

|资源名称|资源简介|
|--|--|
|[版本说明书](docs/zh/release_notes.md)|提供基于开源LZ4优化的鲲鹏LZ4的每个发布版本的基础信息和特性更新说明。|
|[快速入门](docs/zh/quick_start.md)|提供基于开源LZ4优化的鲲鹏LZ4的快速上手示例与编译运行说明。|
|[API参考](docs/zh/api_reference.md)|提供NEON向量化内存拷贝、哈希优化等相关宏与函数的接口说明与定义。|
|[安装指南](docs/zh/installation_guide.md)|提供基于开源LZ4优化的鲲鹏LZ4的环境配置与编译安装的详细指导。|

## 免责声明

**致LZ4使用者**

- 本软件仅供调试和开发之用，使用者需自行承担使用风险，并理解以下内容：

    - 此代码仓计划参与LZ4软件开源，仅对LZ4部分函数在鲲鹏处理器上进行性能优化，编码风格遵照原生开源软件，继承原生开源软件安全设计，不破坏原生开源软件设计及编码风格和方式。软件的任何漏洞与安全问题，均由相应的上游社区根据其漏洞和安全响应机制解决。请密切关注上游社区发布的通知和版本更新。鲲鹏计算社区对软件的漏洞及安全问题不承担任何责任。
    - 数据处理及删除：用户在使用本软件过程中产生的数据属于用户责任范畴。建议用户在使用完毕后及时删除相关数据，以防信息泄露。
    - 数据保密与传播：使用者了解并同意不得将通过本软件产生的数据随意外发或传播。对于由此产生的信息泄露、数据泄露或其他不良后果，本软件及其开发者概不负责。
    - 用户输入安全性：用户需自行保证输入的命令行的安全性，并承担因输入不当而导致的任何安全风险或损失。对于输入命令行不当所导致的问题，本软件及其开发者概不负责。

- 免责声明范围：本免责声明适用于所有使用本软件的个人或实体。使用本软件即表示您同意并接受本声明的内容，并愿意承担因使用该软件而产生的风险和责任，如有异议请停止使用本软件。
- 在使用本软件之前，请**谨慎阅读并理解以上免责声明的内容**。对于使用本软件所产生的任何问题或疑问，请及时联系开发者。

**致数据所有者**

如果您不希望您的数据集等信息在LZ4优化仓库中被提及，或希望更新相关描述，请在GitCode提交issue，我们将根据您的要求删除或更新相关描述。感谢您的理解与支持。

## License

- LZ4核心库采用BSD-2-Clause许可证，具体请参见[LICENSE文件](LICENSE)。

- LZ4项目的文档适用CC-BY 4.0许可证，具体请参见[LICENSE文件](docs/LICENSE)。

## 贡献声明

欢迎大家为社区做贡献，如果使用过程中有任何问题/建议，或者需要反馈特性需求和bug报告，可以提交[Issues](https://gitcode.com/boostkit/community/blob/master/docs/contributor/issue-submit.md)联系我们，具体贡献方法可参考[这里](https://gitcode.com/boostkit/community/blob/master/docs/contributor/contributing.md)。同时也欢迎大家在[讨论专区](https://gitcode.com/boostkit/community/discussions)展开讨论交流。感谢您的支持。

## 致谢

LZ4由华为公司的下列部门联合贡献：

- 鲲鹏计算Boostkit开发部

感谢来自社区的每一个PR，欢迎贡献LZ4！
