Computer Graphics
=======
此repo包含计算机图形学的一些练习代码。<br>

开发环境
-------
* 运行环境：Windows，Visual Studio 2019
* 需要 EasyX 图形库，本项目使用版本：20200520（beta）

说明
-------
* 解决方案内包含多个VS项目，将欲执行项目设为启动项目即可运行。
* 建议在Release模式下运行。

内容介绍
-------
Chapter 4 - Ray Tracing
>4-1 光线生成与相交计算
>>演示光线追踪的基本原理，包括按像素顺序生成光线、光线与几何图形相交的判断。<br>
>>修改4-1.cpp的134-144行，可以改变三角形的属性。
>
>4-2 Lambertian 反射
>>演示 Lambertian 反射。<br>
>>修改4-2.cpp的192-204行，可以改变几何对象的属性。<br>
>>修改<code>e_to_ScreenCenter</code>可改变视角方向。<br>
>>几何对象必须是一个数组，在程序中定义为<code>Surface* s[]</code>。<br>
>>可通过键盘进行交互，详见4-2.cpp：145-187。
>
>4-3 Blinn-Phong 模型
>>实现 Blinn-Phong 光照模型。<br>
>>修改4-3.cpp：96-130，可以改变几何对象的属性。<br>
>>修改<code>e_to_ScreenCenter</code>可改变视角方向。<br>
>>几何对象必须是一个数组，在程序中定义为<code>Surface* s[]</code>。<br>
>>可通过键盘进行交互，详见4-3.cpp：49-91。

Chapter 8 - The Graphics Pipeline
>8-1 二维图元的光栅化绘制
>>使用光栅化方法绘制直线、三角形等。
