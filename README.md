# Gluttonous-Snake-Written-in-Cpp
rt，大一无聊的时候用c++写的贪吃蛇，那时候没接触面向对象，大二优化了一下，算是练练手的小程序。
![压力不大](压力不大.jpg)
## 遇到的课上不讲的东东
* 通过调用句柄对控制台光标的位置、显隐等进行操作（用以解决屏闪问题）
* 通过多线程实现非阻塞输入，做到按下按键立即相应
* 用容器Vector实现动态数组
* 还有吗，记不太清了……

## 不知道有什么好写强行加上的一些东西
* 之前有写过随机地图（随机生成不重叠的房间），这里用来随机生成障碍物，只是几个方块，有点蠢🐷~~貌似偶尔还会碰到生成不了的情况~~
* C++好像要手动释放内存，而我一个free都没写👀
* 因为控制台的每个字符都是长方形的，在蛇每秒移动相同的格子数的情况下，上下移动的速度看上去要比左右移动的速度快一倍，我嫌麻烦不想改就让它那样了。~~毕竟真正做游戏的时候不会有这种问题，而且我懒了~~
* 因为用到了多线程，所以编译的时候要在至少C++11的标准才能通过
