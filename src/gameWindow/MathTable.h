//
// Created by 17834 on 2023/11/15.
//

#ifndef ZEROHELPER_MATHTABLE_H
#define ZEROHELPER_MATHTABLE_H
#include <QLabel>

struct MathTable{
    explicit MathTable();
    void move(int x, int y, int bottom, int right);
    void free();
private:
    QPixmap pic1;
    QPixmap pic2;
    QPixmap pic3;
    QPixmap pic4;
    QPixmap picM;

    QLabel table1;
    QLabel table2;
    QLabel table3;
    QLabel table4;
    QLabel tableM;
};

#endif //ZEROHELPER_MATHTABLE_H
