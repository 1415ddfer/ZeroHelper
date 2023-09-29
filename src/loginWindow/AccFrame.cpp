//
// Created by 17834 on 2023/6/19.
//

#include "AccFrame.h"
#include "../playerData/GameManager.h"
#include <utility>

AccFrame::AccFrame(QWidget *p) :
        mLayout(this),
//        mScroll(this),
        btnList() {
    setParent(p);

//    mLayout.addWidget(&mScroll, 0, 6, 1, 9);
    mLayout.setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mLayout.setSpacing(35);
    mLayout.setContentsMargins(17, 5, 17, 0);
    auto findRet = QJsonValue{};
    streamer.findValue("lastTeamIndex", &findRet);
    auto team = Team{};
    selfIndex = findRet.toInt(0);
    TeamManager::getTeam(findRet.toInt(0), &team);
    int width = 5, x = 0, y = 0, index = 0;
    for (auto &i: team.member) {
//        qDebug() << i.serverId << "|" << i.userName << "|" << i.userPWD;
        auto btn = new MemberBtn(this, i, index++);
        btnList.append(btn);
        mLayout.addWidget(btn, y, x++);
        if (x == width) ++y, x = 0;
    }
}

void AccFrame::reloadTeam() {
    for (auto &btn: btnList) {
        mLayout.removeWidget(btn);
        delete btn;
    }
    btnList.clear();
    auto team = Team{};
    TeamManager::getTeam(selfIndex, &team);
    int width = 5, x = 0, y = 0, index = 0;
    for (auto &i: team.member) {
        auto btn = new MemberBtn(this, i, index++);
        btnList.append(btn);
        mLayout.addWidget(btn, y, x++);
        if (x == width) ++y, x = 0;
    }
}

void AccFrame::delMember(int index, const QString& name) {
    auto messageBox = QMessageBox(this);
    messageBox.setText(QString("即将删除账号%1").arg(name));
    messageBox.setInformativeText("是否继续？");
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Cancel);
    if (QMessageBox::Yes == messageBox.exec()) {
        TeamManager::delMember(selfIndex, index);
    }
}
void AccFrame::delMember(int index) const {
    TeamManager::delMember(selfIndex, index);
}

void AccFrame::updateMember(int index, AccData member) const {
    TeamManager::updateMember(selfIndex, index, std::move(member));
}

void AccFrame::contextMenuEvent(QContextMenuEvent *e) {
    auto *menu = new QMenu(this);

    auto *actionAM = new QAction("新增账号", menu);
    menu->addAction(actionAM);
    connect(actionAM, &QAction::triggered, this, &AccFrame::onAddMember);

    auto *editMenu = new QMenu("管理队伍", menu);
    menu->addMenu(editMenu);
    auto *actionEdit = new QAction("编辑当前队伍", editMenu);
    editMenu->addAction(actionEdit);
    connect(actionEdit, &QAction::triggered, this, &AccFrame::onEditTeam);
    auto *actionAdd = new QAction("新建一个队伍", editMenu);
    editMenu->addAction(actionAdd);
    connect(actionAdd, &QAction::triggered, this, &AccFrame::onAddTeam);


    auto teams = TeamManager::getAllTeamName();
    auto name = teams->firstTeam;

    if(name){
        auto *move_menu = new QMenu("打开其他队伍", menu);
        menu->addMenu(move_menu);
        int index = 0;
        while (name) {
            if (index != selfIndex) {
                auto *action = new QAction(name->name, move_menu);
                move_menu->addAction(action);
                connect(action, &QAction::triggered, this, [index, this]() {
                    selfIndex = index;
                    reloadTeam();
                });
            }
            index++;
            name = name->next;
        }
    }

    teams->free();
    menu->exec(e->globalPos());
    delete menu; // 这会自动删除action和菜单里的其他所有对象 因为 new QAction(menu) 设置了父子关系
}

void AccFrame::onEditTeam() const {
    auto *edit = new TeamAdd(selfIndex);
    edit->setAttribute(Qt::WA_DeleteOnClose);
    connect(edit, &TeamAdd::needReload, this, &AccFrame::onDelTeam);
    edit->show();
}

void AccFrame::onAddMember() const {
    auto edit = new MemberEdit(nullptr, selfIndex);
    connect(&edit->doneBTN, &QPushButton::clicked, this, &AccFrame::reloadTeam);
    edit->initDoneEvent();
    edit->show();
}

void AccFrame::onAddTeam() {
    auto *edit = new TeamAdd();
    edit->setAttribute(Qt::WA_DeleteOnClose);
    edit->show();
}

void AccFrame::onDelTeam(){
    selfIndex--;
    reloadTeam();
}

MemberBtn::MemberBtn(AccFrame *p, const AccData &m, int index) :
        imgLabel(this),
        textLabel(this),
        menuChangeTeam(this),
        mLayout(this) {
    setParent(p);
    parent = p;
    member = m;
    selfIndex = index;
    mLayout.setSpacing(7);
    mLayout.setContentsMargins(0, 0, 0, 0);
    mLayout.setAlignment(Qt::AlignCenter | Qt::AlignTop);

    setFixedSize(84, 100);
    auto ft = QFont();
    ft.setPointSize(9);
    textLabel.setFont(ft);

    mLayout.addWidget(&imgLabel);
    mLayout.addWidget(&textLabel);
    textLabel.setText(ServicesManager::GetServiceShortName(member.providerId)+ '-' + member.nickName);
    icon.load(":/button/res/player_ico1.png");
    imgLabel.setPixmap(icon);

    //setContextMenuPolicy(Qt::ActionsContextMenu);
//    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
//            this, SLOT(contextMenuEvent(QPoint)));
}

void MemberBtn::contextMenuEvent(QContextMenuEvent *event) {
    needReload = false;
    auto *menu = new QMenu(this);
    auto *actionEdit = new QAction("编辑", menu);
    auto *actionDel = new QAction("删除", menu);
    auto *menuTeam = new QMenu("移动到", menu);

    menu->addAction(actionEdit);
    menu->addAction(actionDel);
    menu->addMenu(menuTeam);

    connect(actionEdit, &QAction::triggered, this, &MemberBtn::onEdit);
    connect(actionDel, &QAction::triggered, this, &MemberBtn::onDel);

    auto teams = TeamManager::getAllTeamName();
    auto name = teams->firstTeam;
    auto index = 0;
    while (name) {
        if(index != parent->selfIndex){
            auto *action = new QAction(name->name, menuTeam);
            connect(action, &QAction::triggered, [index, this]() { onMove(index); });
            menuTeam->addAction(action);
        }
        index++;
        name = name->next;
    }
    teams->free();
    menu->exec(event->globalPos());
    delete menu; // 这会自动删除action和菜单里的其他所有对象 因为 new QAction(menu) 设置了父子关系
    if (needReload) {
        parent->reloadTeam();
    }
}

void MemberBtn::onEdit() {
    auto editBox = new MemberEdit(&member);
    connect(&editBox->doneBTN, &QPushButton::clicked, this, &MemberBtn::onEditDone);
    editBox->initDoneEvent();
    editBox->show();
}

void MemberBtn::onDel() {
    parent->delMember(selfIndex, textLabel.text());
    needReload = true;
}

void MemberBtn::onMove(int newIndex) {
    TeamManager::addMember(newIndex, member);
    parent->delMember(selfIndex);
    needReload = true;
}

void MemberBtn::onEditDone() {
    parent->updateMember(selfIndex, member);
    textLabel.setText(ServicesManager::GetServiceShortName(member.providerId)+ '-' + member.nickName);
}

void MemberBtn::doLogin() {
    auto info = gameManager.getInfo(&member);
    if(info!= nullptr){
        auto messageBox = QMessageBox(this);
        messageBox.setText(QString("账号%1已经启动了%2").arg(member.nickName).arg(info->getGameState()));
        messageBox.setInformativeText("继续启动游戏将会关掉已登录的窗口，是否继续登录？");
        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        messageBox.setDefaultButton(QMessageBox::Cancel);
        if (QMessageBox::Cancel == messageBox.exec()) {
            return;
        }
        gameManager.closeGame(&member);
    }
    gameManager.createGame(&member);
}

void MemberBtn::mousePressEvent(QMouseEvent *e) {
    auto loc = geometry();
    move(loc.x() + 3, loc.y() + 3);
    QWidget::mousePressEvent(e);
}

void MemberBtn::mouseReleaseEvent(QMouseEvent *e) {
    auto loc = geometry();
    move(loc.x() - 3, loc.y() - 3);
    QWidget::mouseReleaseEvent(e);
    if(e->pos().x() > this->width() or e->pos().y() > this->height()) return;
    if(e->button() == Qt::RightButton) return;
    doLogin();
}

MemberEdit::MemberEdit(AccData *acc, int index) :
        mLayout(this),
        sidLabel(this),
        sidEdit(this),
        userLabel(this),
        userEdit(this),
        pwdLabel(this),
        pwdEdit(this),
        nickLabel(this),
        nickEdit(this),
        serverBox(this, "请选择服务器"),
        doneBTN(QIcon(":/button/res/25.png"), this),
        closeBtn(QIcon(":/button/res/btn_close1.png"), this) {
    background = QPixmap(":/background/res/2.png");
    if (acc == nullptr) {
        member = new AccData{};
        isAdd = true;
        selfIndex = index;
    } else {
        member = acc;
        isAdd = false;
    }

    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    mLayout.setAlignment(Qt::AlignTop);
    mLayout.setSpacing(10);
    mLayout.setContentsMargins(15, 5, 15, 9);

    setFixedSize(400, 210);
    closeBtn.setFixedSize(34, 23);
    closeBtn.setIconSize(QSize(34, 23));
    doneBTN.setFixedSize(60, 26);
    doneBTN.setIconSize(QSize(60, 26));

    mLayout.addWidget(&closeBtn, 0, 4);
    mLayout.addWidget(&sidLabel, 1, 0);
    mLayout.addWidget(&serverBox, 1, 1);
    mLayout.addWidget(&sidEdit, 1, 2);
    mLayout.addWidget(&userLabel, 2, 0);
    mLayout.addWidget(&userEdit, 2, 1, 1, 2);
    mLayout.addWidget(&pwdLabel, 3, 0);
    mLayout.addWidget(&pwdEdit, 3, 1, 1, 2);
    mLayout.addWidget(&nickLabel, 4, 0);
    mLayout.addWidget(&nickEdit, 4, 1, 1, 2);
    mLayout.addWidget(&doneBTN, 5, 3, 1, 2);

    sidLabel.setText("游戏区服:");
    userLabel.setText("游戏账号:");
    pwdLabel.setText("账号密码:");
    nickLabel.setText("角色备注:");
    //doneBTN     .setText("确定");

    pwdEdit.setEchoMode(QLineEdit::Password);

    {
        auto services = QList<QString>();
        ServicesManager::GetAllServices(&services);
        for (auto &i: services) {
            serverBox.addItem(i);
        }
    }

    connect(&doneBTN, &QPushButton::clicked, this, &MemberEdit::onDone);
    connect(&closeBtn, &QPushButton::clicked, this, &QWidget::close);
    if (isAdd) return;
    serverBox.removeItem(0);
    serverBox.showHint = false;
    serverBox.setCurrentIndex(member->providerId);
    sidEdit.setText(member->serverId);
    userEdit.setText(member->userName);
    pwdEdit.setText(member->userPWD);
    nickEdit.setText(member->nickName);
}

void MemberEdit::initDoneEvent() {
    connect(&doneBTN, &QPushButton::clicked, this, &QWidget::close);
    connect(&closeBtn, &QPushButton::clicked, this, &QWidget::close);
}

void MemberEdit::onDone() {
    QString username = userEdit.text(), pwd = pwdEdit.text();
    judgeMessage(username!= nullptr and pwd != nullptr, "用户名或密码不能为空")
    judgeMessage(!serverBox.showHint, "请选择服务器")
    *member = AccData{serverBox.currentIndex(), userEdit.text(), pwdEdit.text(), nickEdit.text(), sidEdit.text()};
    if (isAdd) TeamManager::addMember(selfIndex, *member);
}

void MemberEdit::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap(rect(), background, QRect());
}

void MemberEdit::closeEvent(QCloseEvent *event) {
    QWidget::closeEvent(event);
    if (isAdd) delete member;
    delete this;
}

void MemberEdit::mouseReleaseEvent(QMouseEvent *e) {
    isPressed = false;
    QWidget::mouseReleaseEvent(e);
}

void MemberEdit::mouseMoveEvent(QMouseEvent *e) {
    if (isPressed) {
        // 这个窗口不会最大化所以忽略这一判断 省略showNormal()
        QPoint move_pos = e->globalPosition().toPoint() - startPos;
        startPos =  e->globalPosition().toPoint();
        move(pos() + move_pos);
    }
    QWidget::mouseMoveEvent(e);
}

void MemberEdit::mousePressEvent(QMouseEvent *e) {
    isPressed = true;
    startPos =  e->globalPosition().toPoint();
    QWidget::mousePressEvent(e);
}

void TeamAdd::mouseReleaseEvent(QMouseEvent *e) {
    isPressed = false;
    QWidget::mouseReleaseEvent(e);
}

void TeamAdd::mouseMoveEvent(QMouseEvent *e) {
    if (isPressed) {
        // 这个窗口不会最大化所以忽略这一判断 省略showNormal()
        QPoint move_pos = e->globalPosition().toPoint() - startPos;
        startPos = e->globalPosition().toPoint();
        move(pos() + move_pos);
    }
    QWidget::mouseMoveEvent(e);
}

void TeamAdd::mousePressEvent(QMouseEvent *e) {
    isPressed = true;
    startPos = e->globalPosition().toPoint();
    QWidget::mousePressEvent(e);
}

//void TeamAdd::closeEvent(QCloseEvent *event) {
//    QWidget::closeEvent(event);
//    delete this;
//}

TeamAdd::TeamAdd(int id) :
        selfId{id},
        mLayout(this),
        hintLine(QString("队伍名称："), this),
        nameLine(this),
        delBtn(QIcon(":/button/res/btn_close1.png"), this),
        doneBtn(QIcon(":/button/res/25.png"), this),
        closeBtn(QIcon(":/button/res/btn_close1.png"), this) {
    background = QPixmap(":/background/res/2.png");
    mLayout.setAlignment(Qt::AlignTop);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    mLayout.setSpacing(10);
    mLayout.setContentsMargins(15, 5, 15, 15);
    closeBtn.setFixedSize(34, 23);
    closeBtn.setIconSize(QSize(34, 23));
    doneBtn.setFixedSize(60, 26);
    doneBtn.setIconSize(QSize(60, 26));
    setFixedHeight(200);


    auto *lineLayout = new QHBoxLayout();
    lineLayout->addWidget(&hintLine);
    lineLayout->addWidget(&nameLine);
    if (id < 0){
        setWindowTitle("新建队伍");

        mLayout.addWidget(&closeBtn, 0, 2);
        mLayout.addItem(new QSpacerItem(30, 30, QSizePolicy::Preferred, QSizePolicy::Expanding), 1, 0);
        mLayout.addLayout(lineLayout,2, 0);
        mLayout.addItem(new QSpacerItem(30, 30, QSizePolicy::Preferred, QSizePolicy::Expanding), 3, 0);
        delBtn.setVisible(false);
        mLayout.addWidget(&doneBtn, 4, 1, 1, 2);
    } else{
        setWindowTitle("更改队伍");
        mLayout.addWidget(&closeBtn, 0, 5);
        mLayout.addItem(new QSpacerItem(30, 30, QSizePolicy::Preferred, QSizePolicy::Expanding), 1, 0);
        mLayout.addLayout(lineLayout,2, 0, 1, 3);
        mLayout.addItem(new QSpacerItem(30, 30, QSizePolicy::Preferred, QSizePolicy::Expanding), 3, 0);
        mLayout.addWidget(&delBtn, 4, 2, 1, 2);
        mLayout.addWidget(&doneBtn, 4, 4, 1, 2);
        connect(&delBtn, &QPushButton::clicked, this, &TeamAdd::onDelTeam);
    }


    connect(&closeBtn, &QPushButton::clicked, this, &QWidget::close);
    connect(&doneBtn, &QPushButton::clicked, this, &TeamAdd::onDone);
}

void TeamAdd::onDone() {
    auto text = nameLine.text();
    judgeMessage(text!= nullptr, "名字不能为空")
    TeamManager::addTeam(nameLine.text());
    close();
}

void TeamAdd::onDelTeam() {
    auto messageBox = QMessageBox(this);
    messageBox.setText(QString("即将删除队伍:%1").arg(nameLine.text()));
    messageBox.setInformativeText("是否继续？");
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Cancel);
    if (QMessageBox::Yes == messageBox.exec()) {
        TeamManager::delTeam(selfId);
        emit needReload();
        close();
    }

}

void TeamAdd::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawPixmap(rect(), background, QRect());
}


