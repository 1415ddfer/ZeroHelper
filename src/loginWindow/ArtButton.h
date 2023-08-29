//
// Created by 17834 on 5/29/2023.
//

#ifndef ZEROHELPER_ARTBUTTON_H
#define ZEROHELPER_ARTBUTTON_H


#include <QPushButton>
#include <QLineEdit>
#include <QPainter>
#include <QComboBox>
#include <QHBoxLayout>
#define judgeMessage(condition, message) \
    if (!(condition)) { \
        QMessageBox::warning(nullptr, "警告", message); \
        return; \
    }

class ArtButton : public QPushButton{
    void mousePressEvent(QMouseEvent  *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
public:
    explicit ArtButton(const QIcon&, QWidget *p);
};

//struct ArtBackGround{
//    int height, width;
//    QString a1, a2, a3, b1, b2, b3, c1, c2, c3;
//    QPixmap background;
//    void update();
//};
class ArtLineEdit : public QLineEdit{
    QPixmap background;
    void paintEvent(QPaintEvent*) override;
public:
    explicit ArtLineEdit(QWidget* p);
};

class ArtComboBox : public QComboBox{
    QPixmap background;
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
public:
    bool showHint;
    explicit ArtComboBox(QWidget*p, const QString& hint = nullptr);
};


#endif //ZEROHELPER_ARTBUTTON_H
