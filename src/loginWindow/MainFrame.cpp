//
// Created by 17834 on 5/27/2023.
//

#include "MainFrame.h"

MainFrame::MainFrame() :
        mLayout(this), titleBar(this), accFrame(this) {
    setFixedSize(600, 400);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    background = QPixmap(":/background/res/1.png");
    mLayout.setSpacing(0);
    mLayout.setContentsMargins(0, 0, 0, 0);
    mLayout.addWidget(&titleBar);
//    mLayout.addSpacerItem(new QSpacerItem(30, 30, QSizePolicy::Preferred, QSizePolicy::Expanding));
    mLayout.addWidget(&accFrame);
    accFrame.setFixedSize(600, 370);
    connect(&titleBar.miniSizeBtn, SIGNAL(clicked()), this, SLOT(showMinimized()));
    connect(&titleBar.closeBtn, SIGNAL(clicked()), this, SLOT(close()));
}

void MainFrame::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap(rect(), background, QRect());
}


TitleBar::TitleBar(QWidget *p) :
        mLayout(this),
        closeBtn(QIcon(":/button/res/btn_close1.png"), this),
        miniSizeBtn(QIcon(":/button/res/btn_mini1.png"), this) {
    setFixedHeight(30);
    setParent(p);
    mLayout.setSpacing(3);
    mLayout.setContentsMargins(15, 0, 15, 0);
    mLayout.addSpacerItem(new QSpacerItem(30, 30, QSizePolicy::Expanding));

    mLayout.addWidget(&miniSizeBtn);
    mLayout.addWidget(&closeBtn);

    closeBtn.setFixedSize(34, 23);
    closeBtn.setIconSize(QSize(34, 23));

    miniSizeBtn.setFixedSize(32, 22);
    miniSizeBtn.setIconSize(QSize(32, 22));
}

void TitleBar::mouseReleaseEvent(QMouseEvent *e) {
    isPressed = false;
    QWidget::mouseReleaseEvent(e);
}

void TitleBar::mouseMoveEvent(QMouseEvent *e) {
    if (isPressed) {
        // 这个窗口不会最大化所以忽略这一判断 省略showNormal()
        QPoint move_pos = e->globalPos() - startPos;
        startPos = e->globalPos();
        auto p = (QWidget *) parent();
        p->move(p->pos() + move_pos);
    }
    QWidget::mouseMoveEvent(e);
}

void TitleBar::mousePressEvent(QMouseEvent *e) {
    isPressed = true;
    startPos = e->globalPos();
    QWidget::mousePressEvent(e);
}

ToolBar::ToolBar(QWidget *p) :
mLayout(this),
loginFrameBTN(this),
pluginFrameBTN(this),
loginFrame(this)
{
    setParent(p);
    mLayout.addWidget(&loginFrameBTN);
    mLayout.addWidget(&pluginFrameBTN);

    loginFrameBTN.setText("登录");
    pluginFrameBTN.setText("脚本");
}
