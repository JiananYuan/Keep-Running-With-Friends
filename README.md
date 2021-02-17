# Keep-Running-With-Friends 使用说明书

![](https://www.png8.com/imgs/2021/02/750582a774ea6f4a.jpg)

![](https://www.png8.com/imgs/2021/02/bbcb2d3a9e173d64.jpg)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210217231203382.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDAyNjYwNA==,size_16,color_FFFFFF,t_70)



## 一、模型绘制总览

![](https://www.png8.com/imgs/2021/02/5dbadc0a0e83a6de.jpg)



## 二、模型层次图



![](https://www.png8.com/imgs/2021/02/f8c52b100189643b.png)



其中，两个人物的层级模型层次为：

![](https://www.png8.com/imgs/2021/02/5891c610fb044b94.png)



## 三、模型运行环境

- 硬件平台：

  - Windows 10 OS Laptop（接上电源并设置电源模式为高性能）

  - Intel(R) Core(TM) i7-8750H CPU @ 2.20GHz

  - Intel(R) UHD Graphics 630

  - NVIDIA GeForce GTX 1050 Ti

- 软件平台：

  - Microsoft Visual Studio 2019

  - GLFW + GLEW + OpenGL



## 四、交互说明

**对窗口的控制**

​    ESC: 退出程序

​    p: 在控制台打印帮助信息

**对相机的控制**

​    c: 切换相机观察模式 (两种模式：TPS/FPS)，下面使用前均需要切换到相应模式，FPS指第一人称观察，TPS指第三人称观察

​    w: FPS模式下，向前移动相机

​    s: FPS模式下，向后移动相机

​    a: FPS模式下，向左移动相机

​    d: FPS模式下，向右移动相机

​	u/j: TPS模式下，水平顺逆时针移动相机，360度全景浏览

​    i/k: TPS模式下，垂轴顺逆时针移动相机，限定地平面以上浏览

​    o/l: TPS模式下，控制相机拍摄半径

​	鼠标(左键)移动/拖拽: TPS模式下，水平方向旋转相机

​    鼠标右键拖拽: TPS模式下，铅直方向旋转相机

**对人物的控制**

​	↑: TPS模式下，两个人物协同跑步前进

​    ↓: TPS模式下，两个人物协同俯倾前进

​    ←: TPS模式下，两个人物协同左转向

​    →: TPS模式下，两个人物协同右转向

​	h: TPS模式下，2号人物跑步前进

​     n: TPS模式下，2号人物俯倾前进

​     b: TPS模式下，2号人物左转向

​     m: TPS模式下，2号人物右转向

​    空格键: TPS模式下，2号人物坐下

**对静态模型的控制**

​    +/-: 控制场景中两个模型的缩放 (不建议过度放缩，会导致界面不美观)

​    中央回车键: 复原场景中两个模型的大小

**对背景的控制**

​    主键区1~5: 切换天空盒背景



## 五、参考材料

- MagicaVoxel建模软件：http://ephtracy.github.io/

- 天空盒纹理PNG搜索网：https://pngss.com/posts/7a0ea63846abd197c01707bd7290a3d1

- LearnOpenGL CN： https://learnopengl-cn.github.io/

- 现代OpenGL教程： https://wiki.jikexueyuan.com/project/modern-opengl-tutorial

- GLFW 文档：https://www.glfw.org/documentation.html