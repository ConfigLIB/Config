# Config库

## 基本信息

### 代码分布

```text
/───.vscode     # VSCode相关配置
├───bin         # 存放最终产物（仅保留文件夹）
├───build       # 存放构建中间产物（仅保留文件夹）
├───example     # 示例代码
├───include     # 头文件代码
├───script      # 构建相关脚本
├───src         # 实现代码
└───test        # 测试代码
```

## 构建支持

* 支持C++20的编译器（Clang、GCC、MSVC）
  * `using enum class`
  * `std::format`
* 非Windows环境下需要安装libfmt

### Ubuntu

使用命令行直接编译：

```bash
# Ubuntu

sudo apt install libfmt-dev   # 前置条件

cd build
../script/build.sh
```

### Windows

使用命令行直接编译：

```pwsh
cd build
../script/build.bat
```

## 包含内容

* Error.h
* FormatAdapter.h

### JSON的解析和构造

* JSONParser.h

### XML的解析和构造

### HTML5的解析和构造

### ini的解析和构造

### TOML的解析和构造

### YAML的解析和构造

## 需要解决的问题

* `std::format`支持问题
  * Clang15/GCC13才正式支持
  * 考虑使用`fmt::format`替代
* 字符集的问题
  * 使用`wstring`还是`string`，或是`u16string`
  * 工具库是否需要支持Unicode
  * MinGW的`wcout`使用有问题
  * Linux上GCC的`wchar_t`长度为4 Bytes，Windows上的`wchar_t`长度为2 Bytes
* JSON解析支持不完全
  * 比如科学计数法
  * 可编译期计算的eDSL构造
  * 运行时的动态处理（插入、删除、更改、查询）
* 编译工具的支持
  * make
  * xmake
  * CMake
  * VS使用方便性的问题
* 编码风格上的考虑
  * 抛弃使用`union`
    * 解决内存泄漏的关键
    * `std::variant`
    * 继承和虚函数
  * 加入clang-format的支持
  * `(w)string_view`替换右值和左值版本的构造函数
* 可靠性控制
  * 解决内存泄漏的问题
  * 单元测试的加入
  * 性能测试的加入
  * 文档的加入

## 参考

* [JSON](https://www.json.org/json-en.html)
* [【C++项目】JSON解析器，从设计到实现](https://www.bilibili.com/video/BV1N8411h7hK/)
