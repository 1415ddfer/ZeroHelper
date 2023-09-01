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
class TeamsEdit;

class TeamAdd : public QWidget{
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
    void closeEvent(QCloseEvent *event) override;
private slots:
    void onDone();
public:
    ArtButton closeBtn;
    ArtButton doneBtn;
    explicit TeamAdd();
};

class MemberEdit : public QWidget {
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
    bool isAdd;
    int selfIndex;
    void paintEvent(QPaintEvent*) override;
    void closeEvent(QCloseEvent *event) override;
    bool isPressed{};
    QPoint startPos;
    void mousePressEvent(QMouseEvent  *e) override;
    void mouseReleaseEvent(QMouseEvent  *e) override;
    void mouseMoveEvent(QMouseEvent  *e) override;
private slots:

    void onDone();
public:
    ArtButton doneBTN;
    explicit MemberEdit(AccData *acc= nullptr, int index=0);
    void initDoneEvent();
};

class TeamLine : public QWidget{
    ArtLineEdit teamName;
    ArtButton editBtn;
    ArtButton delBtn;

    QHBoxLayout mLayout;

    QPixmap background;

    bool Editing;
    int selfIndex;
    TeamsEdit* parent;
    void enterEvent(QEnterEvent *) override;
    void leaveEvent(QEvent *) override;
private slots:
    void onDel();
    void onEdit();
    void onDone();

public:
    explicit TeamLine(TeamsEdit* p, const QString& name, int index);
};

class TeamsEdit: public QWidget{
    QGridLayout mLayout;
    QWidget lines;
    QVBoxLayout linesLayout;
    ArtButton closeBtn;
    ArtButton doneBtn;
    AccFrame *parent;

    QPixmap background;
    void paintEvent(QPaintEvent*) override;
    void closeEvent(QCloseEvent *event) override;

    void reloadLines();
public:
    void delTeam(int index);
    explicit TeamsEdit(AccFrame* p);
};

class MemberBtn : public QWidget {
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
    QGridLayout mLayout;
//    QScrollArea mScroll;
    QList<MemberBtn *> btnList;
    void contextMenuEvent(QContextMenuEvent *) override;
private slots:
    void onEditMenu();
    void onAddMember() const;
    static void onAddTeam();
public:
    int selfIndex;

    void reloadTeam();
    void delTeam(int index=-1);
    void delMember(int, const QString&);
    void delMember(int) const;
    void updateMember(int index, AccData member) const;
    explicit AccFrame(QWidget *p);
};


#endif //ZEROHELPER_ACCFRAME_H
