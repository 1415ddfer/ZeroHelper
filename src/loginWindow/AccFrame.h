//
// Created by 17834 on 2023/6/19.
//

#ifndef ZEROHELPER_ACCFRAME_H
#define ZEROHELPER_ACCFRAME_H


#include <QWidget>
#include <QGridLayout>
#include <QComboBox>
#include <QScrollArea>
#include <QLabel>
#include <QMenu>
#include <QContextMenuEvent>
#include <QLineEdit>
#include <QPainter>
#include <QMessageBox>
#include <QCloseEvent>
#include "../playerData/TeamManager.h"
#include "../playerData/ServicesManager.h"
#include "ArtButton.h"

class AccFrame;

class TeamAdd : public QWidget{
    Q_OBJECT
    int selfId;

    QGridLayout mLayout;
    QLabel hintLine;
    ArtLineEdit nameLine;

    bool isPressed{};
    QPoint startPos;
    void mousePressEvent(QMouseEvent  *e) override;
    void mouseReleaseEvent(QMouseEvent  *e) override;
    void mouseMoveEvent(QMouseEvent  *e) override;
    QPixmap background;
    void paintEvent(QPaintEvent*) override;
//    void closeEvent(QCloseEvent *event) override;
    signals:
    void needReload();
private slots:
    void onDone();
    void onDelTeam();
public:
    ArtButton delBtn;
    ArtButton closeBtn;
    ArtButton doneBtn;
    explicit TeamAdd(int id=-1);
};

class MemberEdit : public QDialog{
Q_OBJECT
    QGridLayout mLayout;
    QLabel sidLabel;
    ArtLineEdit sidEdit;
    QLabel userLabel;
    ArtLineEdit userEdit;
    QLabel pwdLabel;
    ArtLineEdit pwdEdit;
    QLabel nickLabel;
    ArtLineEdit nickEdit;
    ArtComboBox serverBox;
    ArtButton closeBtn;
    AccData *member;

    QPixmap background;
    bool mActive;
    bool isAdd;
    int selfIndex;
    void paintEvent(QPaintEvent*) override;
    void closeEvent(QCloseEvent *event) override;
    bool isPressed{};
    QPoint startPos;
    void mousePressEvent(QMouseEvent  *e) override;
    void mouseReleaseEvent(QMouseEvent  *e) override;
    void mouseMoveEvent(QMouseEvent  *e) override;
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *) override;
private slots:

    void onDone();
public:
    ArtButton doneBTN;
    explicit MemberEdit(QWidget *p, AccData *acc= nullptr, int index=0);
};

class MemberBtn : public QWidget {
Q_OBJECT
    QLabel imgLabel;
    QLabel textLabel;
    QVBoxLayout mLayout;

    QMenu menuChangeTeam;

    QPixmap icon;
    AccFrame *parent;
    AccData member;
    int selfIndex;
    bool needReload{};

    void onMove(int index);
    void mousePressEvent(QMouseEvent  *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void doLogin();
private slots:
    void onEdit();
    void onDel();
    void onEditDone();
public:
    void contextMenuEvent(QContextMenuEvent *) override;
    explicit MemberBtn(AccFrame *p, const AccData& member, int index);
};

class AccFrame : public QWidget {
    Q_OBJECT
    QGridLayout mLayout;
//    QScrollArea mScroll;
    QList<MemberBtn *> btnList;
    void contextMenuEvent(QContextMenuEvent *) override;
private slots:
    void onEditTeam() const;
    void onAddMember();
    static void onAddTeam();
    void onDelTeam();
public:
    int selfIndex;

    void reloadTeam();
    void delMember(int, const QString&);
    void delMember(int) const;
    void updateMember(int index, AccData member) const;
    explicit AccFrame(QWidget *p);
};


#endif //ZEROHELPER_ACCFRAME_H
