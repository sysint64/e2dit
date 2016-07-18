# E2DIT

Now e2dit is under development, and under rewriting code!

old repository is [here](https://bitbucket.org/lveteam/lve-mapeditor-framework)

##Description
E2DIT - is a 2D map editor for yours games

It uses the OpenGL

##Screenshots
![Image](<https://www.linux.org.ru/gallery/11239761.jpg>)
![Image](<http://hsto.org/files/7b3/c85/958/7b3c85958c004fafbd1200b9aab3abc7.png>)

##Get E2DIT
You can either build E2DIT yourself (see below)

##Build instructions (Linux)

##Dependencies
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

##Donations
I made this project in my spare time because I love what I'm doing. If you are enjoying it and you want to buy me a beer click [here](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=sys%2eint64%40gmail%2ecom&lc=US&item_name=e2dit&currency_code=USD&bn=PP%2dDonationsBF%3abtn_donateCC_LG%2egif%3aNonHosted).
