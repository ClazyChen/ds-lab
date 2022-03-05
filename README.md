# ds-lab

Author: Clazy Chen @ THU

这是一个为计算机考研的学生复习《数据结构》而设计的实验体系。

## 背景

这个实验体系主要面向学习邓俊辉《数据结构（C++版）》的学生，即清华大学本校的学生及面向清华大学计算机专业课考试（912）复习的学生；实验体系设计上，与邓俊辉教材的安排不完全一致。

由于市面上的教材种类繁多，我不能保证能够覆盖各种教材的知识点。比如，一些教材中提出的算法，在清华大学的课程体系中可能被放在了《离散数学》《数值分析》等课程里，这些在目前的实验体系设计上尚不完善。

当这个实验体系完工之后，期待能够对同学们有所帮助，也期待有同学能对它作出改进。

## 语言

使用这个实验体系，需要预先学习C++语言，关于环境的配置见下文的**环境配置**节。

总体上，您只需要学到C with class的程度就可以正常使用这个实验体系。对于实验体系中出现的C++的其他语言特性，下面分别进行一个简要的介绍。

#### 关于OOP

多态和继承是OOP语言的重要语法特性。您必须理解这个语法才能正常阅读和使用实验代码。

构造函数是您需要理解的项目。

运算符重载是您需要理解的项目。

关键字`const`相关的语法是一个optional的项目。它被用来对您的实验进行一些限制，比如限制您在`find`的时候不能修改数据结构的内容。

#### 关于模板

数据结构的存在不依赖于其中存放的数据类型。因此，通常实现数据结构的时候都需要使用泛型编程。理解简单的模板语法对您来说可能是必要的，我不确定不理解模板是否能够正常使用这个实验体系。

在实验代码中可能会出现一些较高级的语法特性，比如`requires`语法。它们对实验体系的使用没有影响。

#### 关于STL

这个实验体系本身实现了STL中一些容器（如`vector`、`list`）的简单版本，所以通常并不会使用这些STL容器。基于相同的原因，不会使用`sort`等STL算法。因此，原则上您不需要系统地学习STL；当然，学习STL对于机试是有益的。

#### 关于auto

关键字`auto`引入了自动类型推导，这是C++11引入并在后续版本不断增强的一个语法特性。使用`auto`可以很好地减少冗余代码，这是我推荐您学习和掌握的一项语法特性。

#### 关于枚举类

枚举类`enum class`是用来替代简单的枚举类型`enum`的语法特性，具有不会污染命名空间的优点。使用上和`enum`没有什么区别。

#### 关于Lambda函数

我经常会使用Lambda函数和`function`模板，而不是C语言的风格，来传递函数类型的参数。不过对于实验体系的使用者来说，您应该不需要了解这方面的语法。

#### 关于不定参数

C++中会使用三个点`...`来表示不定参数。我偶尔会使用这个语法，但和实验体系的使用无关。

#### 关于智能指针

和C语言风格不同，我经常会使用智能指针来管理内存。如果您选择在我的参考实现基础上进行实验，可能需要了解智能指针的使用方法；如果您不考虑这个问题，打算在实验框架上自己实现这些数据结构，则不需要考虑智能指针的问题。

#### 关于迭代器模式

C++的迭代器模式是一个特色，如果您要使用STL，必不可少地需要了解迭代器。在这个实验体系中，我会引入一些迭代器。迭代器相关的大多数代码都和实验内容无关，不需要您自己修改。

总而言之，如果您精通C++，愿意阅读、分析并改进实验框架，我会非常高兴，也欢迎您和我交流。如果您是一个C++初学者，只打算进行实验，我建议您不需要阅读实验框架中的所有代码。

## 环境配置

这一小节面向在Windows系统上使用Dev-C++进行开发的同学。

如果您使用的是更“高级”的系统和编辑器，我相信您并不需要我来展示如何进行环境配置。

1. 下载MinGW（[MinGW-w64 - for 32 and 64 bit Windows download | SourceForge.net](https://sourceforge.net/projects/mingw-w64/)）并安装。

   > 这是因为，在Dev-C++中自带的MinGW版本太低了。

2. 进入Dev-C++，工具—编译选项—由文件夹添加编译设置，选择刚装的MinGW的文件夹。

   编译器：编译时增加加入命令`-std=c++2a -fconcepts`。（开启C++20）

   目录：C++包含文件里，加入本实验框架中的`headers`文件夹。


应该这样就可以了，您可以编译一下`sources`文件夹里的`.cpp`文件查看效果。

