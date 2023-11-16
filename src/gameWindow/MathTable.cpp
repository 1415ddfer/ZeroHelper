//
// Created by 17834 on 2023/11/15.
//

#include "MathTable.h"

MathTable::MathTable():
table1(),
table2(),
table3(),
table4(),
tableM() {
    pic1 = QPixmap(":rule/res/RulePic/1.png");
    pic2 = QPixmap(":rule/res/RulePic/2.png");
    pic3 = QPixmap(":rule/res/RulePic/3.png");
    pic4 = QPixmap(":rule/res/RulePic/4.png");
    picM = QPixmap(":rule/res/RulePic/0.png");

    table1.setPixmap(pic1);
    table2.setPixmap(pic2);
    table3.setPixmap(pic3);
    table4.setPixmap(pic4);
    tableM.setPixmap(picM);

    table1.setWindowFlags( Qt::FramelessWindowHint);
    table2.setWindowFlags( Qt::FramelessWindowHint);
    table3.setWindowFlags( Qt::FramelessWindowHint);
    table4.setWindowFlags( Qt::FramelessWindowHint);
    tableM.setWindowFlags( Qt::FramelessWindowHint);

    table1.setAttribute(Qt::WA_TranslucentBackground, true);
    table2.setAttribute(Qt::WA_TranslucentBackground, true);
    table3.setAttribute(Qt::WA_TranslucentBackground, true);
    table4.setAttribute(Qt::WA_TranslucentBackground, true);
    tableM.setAttribute(Qt::WA_TranslucentBackground, true);

    table1.show();
    table2.show();
    table3.show();
    table4.show();
    tableM.show();
}

void MathTable::move(int x0, int y0, int bottom, int right) {
    int moveX2 = x0 - table2.width();
    int moveY2 = y0 + bottom - table2.height();
    table2.move(moveX2, moveY2);
    table1.move(moveX2, moveY2 - table2.height() - 20);

    int moveX4 = x0 + right;
    int moveY4 = y0 + bottom - table4.height();
    table4.move(moveX4, moveY4);
    table3.move(moveX4, moveY4 - table4.height() - 20);


    tableM.move(x0 + (int)(right/2) - (int)(tableM.width()/2), y0 + bottom);
}

void MathTable::free() {
    table1.close();
    table2.close();
    table3.close();
    table4.close();
    tableM.close();
    delete this;
}

