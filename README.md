# E2DIT

Now e2dit is under development, and under rewriting code!

old repository is [here](https://bitbucket.org/lveteam/lve-mapeditor-framework)

## Description

E2DIT - is a 2D map editor for yours games

It uses the OpenGL

## Screenshots

![Image](<https://www.linux.org.ru/images/16366/1500px.jpg>)

![Image](<http://hsto.org/files/7b3/c85/958/7b3c85958c004fafbd1200b9aab3abc7.png>)

![Image](<https://hsto.org/files/735/1c6/565/7351c6565f1243f7b02e698d205fd4d3.png>)

![Image](<https://hsto.org/files/db5/99c/e05/db599ce05d1d40b9ab46bf85581c29d1.png>)

![Image](<https://hsto.org/files/1bf/504/3a4/1bf5043a42c444ba84f5b4c64614ba1f.png>)

## Get E2DIT

You can either build E2DIT yourself (see below)

## Build instructions (Linux)

## Dependencies

Editor use C++14 standart.

If You use clang, first you need compile SFML 2.+ and boost for C++14 with flags `-std=c++14 -stdlib=libc++`

**Make sure to install these libs first**

1. soil;
2. glew32;
3. sfwml-2.1+;
4. ftgl;
5. freetype2;
6. glm;
7. tinyxml;
8. boost::filesystem;
9. boost::assign;
10. jpeg.

**Build**

---

mkdir build && cd build

cmake ..

make -j4

---
