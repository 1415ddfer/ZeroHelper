//
// Created by 17834 on 5/29/2023.
//

#include "ArtButton.h"

ArtButton::ArtButton(const QIcon& icon, QWidget *p) {
    setIcon(icon);
    setParent(p);
}

void ArtButton::mousePressEvent(QMouseEvent *e) {
    auto loc = geometry();
    move(loc.x() + 3, loc.y() + 3);
    QAbstractButton::mousePressEvent(e);
}

void ArtButton::mouseReleaseEvent(QMouseEvent *e) {
    auto loc = geometry();
    move(loc.x() - 3, loc.y() - 3);
    QAbstractButton::mouseReleaseEvent(e);
}

void ArtLineEdit::paintEvent(QPaintEvent *e) {
    QLineEdit::paintEvent(e);
    QPainter painter(this);
    painter.drawPixmap(rect(), background, QRect());
}

ArtLineEdit::ArtLineEdit(QWidget *p) : QLineEdit(p) {
    setParent(p);
    setStyleSheet("background:transparent;border-width:0;border-style:outset");
    background = QPixmap(":/background/res/21.png");
}

void ArtComboBox::paintEvent(QPaintEvent *e) {
    QComboBox::paintEvent(e);
    QPainter painter(this);
    painter.drawPixmap(rect(), background, QRect());
}

ArtComboBox::ArtComboBox(QWidget *p, const QString& hint) : QComboBox(p) {
    setParent(p);
    if(hint!= nullptr){
        showHint = true;
        addItem(hint);
    } else{
        showHint = false;
    }
//    setStyleSheet("background:transparent;");
    background = QPixmap(":/background/res/24.png");
}

void ArtComboBox::mousePressEvent(QMouseEvent *e) {
    if(showHint){
        removeItem(0);
        showHint = false;
    }
    QComboBox::mousePressEvent(e);
}
