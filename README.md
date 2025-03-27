# 太空战机小游戏

![](/assets/readme/image6.png)

![](/assets/readme/image7.png)

![](/assets/readme/image8.png)

想要从头做一个一样的？可以看 [B站教程](https://www.bilibili.com/video/BV1wSCFYQEyc) 

这是一个小游戏，玩家控制一个飞机。
通过射击敌人来获得分数，同时也要注意躲避敌人的攻击。游戏的目标是尽可能多地获得分数，并挑战自己的极限。

## Windows 系统如何构建并运行？

1. 克隆仓库或下载代码到本地

2. 下载 [VS 生成工具](https://visualstudio.microsoft.com/zh-hans/visual-cpp-build-tools/)

3. 打开 `VS 生成工具`，选择 `C++ 桌面的工作负载` 并安装

![alt text](/assets/readme/image4.png)

4.  下载 [VSCode](https://code.visualstudio.com/) 并安装

5. 打开 `VSCode` 安装 `CMake Tools` 插件

![alt text](/assets/readme/image.png)

6. 用 `VSCode` 打开下载（克隆）好的仓库目录，一般第一次打开 `CMake` 项目的时候，稍等下插件会自动配置项目并让你选编译工具链，这里就看你的 `CPU` 选择项目的编译工具链，`Intel 64位` 就选 `x86_amd64`，`AMD 64位` 就选 `amd64`

![alt text](/assets/readme/image2.png)

> 如果插件没有自动配置项目，可以手动配置，点击侧边栏的 `CMake` 图标，点击 顶部的文件图标，然后选择编译工具链

![alt text](/assets/readme/image5.png)

5. 点击启动按钮，等待编译完成 编译完成会自动打开游戏窗口

![alt text](/assets/readme/image3.png) 

## 游戏规则

通过键盘方向键控制飞机移动
通过键盘空格键发射子弹
尽可能多地击毁敌人
注意躲避敌人的攻击
游戏目标：尽可能多地获得分数，并挑战自己的极限