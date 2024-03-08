# ds-lab
Author: Clazy Chen (Department of Computer Science and Technology, Tsinghua University, China)

This is a course experiment in Data Structure for students who use textbooks (written by Ms. Yan or Mr. Deng) to take graduate school entrance examinations (kaoyan). It covers all the knowledge points needed for the exam and gives users the opportunity to practice their programming skills. Modern C++ training is very rare in any Chinese university. Have fun in our Lab!

Today is 2024, while GCC and Clang still have poor support for modules intruduced by C++20. To solve this problem, we eliminate all modules and downgrade the previous codes to traditional C++ headers. All codes are written in English because Visual Studio recommends UTF-8 with BOM and other compilers may prefer UTF-8 without BOM. We recommend GCC 13+, Clang 17+, and the latest version of Visual Studio 2022.

<!-- We recommend to use Visual Studio 2022 to run this lab.
Because we use `import std` in the code, you may enable `/std:c++lastest` in the project.
Other compilers supporting C++20 standard is also allowed. You may need to enable modules manually, or convert our module interface files to normal header files first.

For GCC and Clang users, you may use `-x` option to support `.ixx` files. Note that `std::format` may not be supported in GCC 12.x (the highest version provided by MinGW till now), which means you should include the `fmt` library to replace the standard `<format>`. Because Visual Studio recommends UTF-8 with BOM, you may use some assistive software to switch the encoding of the code files. A C++-based tool will be provided to solve the encoding problem later. There are also some details of the different compilers that we would like you to contact us to point out. -->

You can find the tutorial documentation under the `doc` folder. `main.pdf` is the new version written in Typst which is under construction, while `tutorial.pdf` is the old version which has been deprecated.

We welcome commercial use of this experiment without payment to the author, as detailed in the MIT license.
