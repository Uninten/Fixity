# Fixity

<div align="center">

[![Windows](https://img.shields.io/badge/Windows-Passing-61C263?logo=windows)](https://github.com/Uninten/Fixity)
[![Qt](https://img.shields.io/badge/Qt-5.12%2B%20%7C%206.x-41CD52?logo=qt)](https://www.qt.io/)
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![C++](https://img.shields.io/badge/C++-11%2B-00599C?logo=c%2B%2B)](https://isocpp.org/)

**基于 Qt Widget 的 FluentUI 风格综合工具平台**

</div>

---

## 📖 简介

**Fixity** 是一个基于 Qt Widget 开发的 FluentUI 风格桌面应用程序，集成了串口调试、机器人模型可视化、笔记管理等多种实用功能，适用于嵌入式开发与机器人工程调试场景。

UI 组件库基于 [ElaWidgetTools](https://github.com/Liniyous/ElaWidgetTools)，感谢 **Liniyous** 大佬的开源贡献 🎉

> 目前 `master` 分支支持 **Qt 5.12 以上**所有版本（Linux 为 Qt 5.15 以上），推荐版本为 **Qt 5.15.2** 和 **Qt 6.6.2**。

---

## ✨ 功能模块

| 模块 | 描述 |
|------|------|
| 🏠 **主页** | 项目概览与快速导航入口 |
| 🖊️ **绘图** | 数据曲线可视化绘图工具 |
| 🔌 **协议与连接** | 串口调试助手，支持自动调试、环形缓冲区和 3D 机器人模型可视化 |
| 📝 **笔记** | 富文本笔记管理，支持多标签页编辑 |
| ⚙️ **设置** | 主题切换、界面个性化配置 |

### 🔌 串口调试模块（Serial）
- 串口通信收发，支持多种波特率与数据格式配置
- **自动调试**：定时发送指令，自动记录通信数据
- **环形缓冲区**：高效数据缓存管理
- **3D 机器人可视化**：加载 STL / URDF 文件，OpenGL 实时渲染机器人模型
- 支持 STL 文件解析与 URDF 文件解析

### 📝 笔记模块（Note）
- 富文本编辑器，支持字体、颜色、格式化等
- 多标签页管理，文件的创建、打开、保存
- Markdown 友好的编辑体验

---

## 🛠️ 构建环境

| 依赖项 | 版本要求 |
|--------|----------|
| Qt | 5.12+ / 6.x |
| C++ | 11 及以上 |
| CMake | 3.5+ |
| OpenGL | 任意支持版本 |
| ElaWidgetTools | 配套版本 |

---

## 🚀 快速开始

### 1. 克隆仓库

```bash
git clone git@github.com:Uninten/Fixity.git
cd Fixity
```

### 2. 准备 ElaWidgetTools

将 `ElaWidgetTools.dll`（运行时库）放置在以下位置之一：
- Qt Creator 构建输出目录（如 `build-Fixity-.../`）
- 系统 PATH 路径下

将 `libElaWidgetTools.dll.a`（链接库）放置在项目 `lib/` 目录下。

### 3. 用 Qt Creator 打开

使用 Qt Creator 打开 `CMakeLists.txt`，选择对应 Kit 后直接构建运行。

---

## 📁 项目结构

```
Fixity/
├── main.cpp              # 程序入口
├── mainwindow.cpp/h      # 主窗口
├── CMakeLists.txt        # 构建配置
├── ExamplePage/          # 各功能页面
│   ├── T_Home            # 主页
│   ├── T_Graphics        # 绘图页
│   ├── T_Setting         # 设置页
│   └── ...
├── Serial/               # 串口与机器人可视化模块
│   ├── T_Serial          # 串口调试主页
│   ├── stlfileloader     # STL 文件解析
│   ├── urdfparser        # URDF 文件解析
│   ├── rrglwidget        # OpenGL 渲染组件
│   └── ...
├── Note/                 # 笔记模块
│   ├── T_Note            # 笔记主页
│   └── notewidget        # 富文本编辑器
├── include/              # ElaWidgetTools 头文件
├── lib/                  # ElaWidgetTools 链接库
└── bin/                  # 资源文件（STL/URDF 模型等）
```

---

## ⚠️ 注意事项

- 运行时需将 `ElaWidgetTools.dll` 放置在可执行文件同目录下，否则程序将无法启动。
- `.gitignore` 已配置忽略 `.dll` 文件，请勿将大体积 DLL 提交至仓库（GitHub 单文件限制 100 MB）。

---

## 📄 许可证

本项目基于 **MIT License** 开源，使用请保留版权声明。

UI 组件库 [ElaWidgetTools](https://github.com/Liniyous/ElaWidgetTools) 同样基于 MIT License，使用须保留其授权声明。

版权所有 © 2026 Uninten