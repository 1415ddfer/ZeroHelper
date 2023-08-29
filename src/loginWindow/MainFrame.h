//
// Created by 17834 on 5/27/2023.
//

#ifndef ZEROHELPER_MAINFRAME_H
#define ZEROHELPER_MAINFRAME_H
#include <QPixmap>
#include <QWidget>
#include <QVBoxLayout>
#include <QPainter>
#include <QMouseEvent>
#include <QPushButton>
#include <QTabWidget>
#include <QButtonGroup>
#include "ArtButton.h"
#include "AccFrame.h"

class TitleBar :public QWidget{
//    QWidget *parent;
//    QSize btn_size = QSize(33, 22);
    QHBoxLayout mLayout;
    void mousePressEvent(QMouseEvent  *e) override;
    void mouseReleaseEvent(QMouseEvent  *e) override;
    void mouseMoveEvent(QMouseEvent  *e) override;
public:
    bool isPressed = false;
    QPoint startPos;
    ArtButton closeBtn;
    ArtButton miniSizeBtn;
    explicit TitleBar(QWidget *p);
};

class ToolBar :public QWidget{
    QHBoxLayout mLayout;
    QPushButton loginFrameBTN;
    QPushButton pluginFrameBTN;
    AccFrame loginFrame;

public:
    explicit ToolBar(QWidget *p);
};


class MainFrame :public QWidget{
    QPixmap background;
    QVBoxLayout mLayout;
    TitleBar titleBar;
    AccFrame accFrame;

    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
public:
    explicit MainFrame();
};


#endif //ZEROHELPER_MAINFRAME_H
