#include "MainWindow.h"
#include <QMessageBox>
#include <QStringList>
#include <QGroupBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *centralWidget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // --- 1. 初始化区域 ---
    // (保持不变) ...
    QHBoxLayout *initLayout = new QHBoxLayout;
    editTotalSize = new QLineEdit("100");
    editTotalSize->setPlaceholderText("总内存大小 (KB)");
    QPushButton *btnInit = new QPushButton("初始化系统");
    initLayout->addWidget(new QLabel("总内存:"));
    initLayout->addWidget(editTotalSize);
    initLayout->addWidget(btnInit);
    layout->addLayout(initLayout);

    // --- 2. 可视化视图 ---
    memoryView = new MemoryWidget;
    memoryView->setManager(&memoryManager);
    layout->addWidget(memoryView);

    // --- 3. 操作区域 (分配) ---
    QGroupBox *allocGroup = new QGroupBox("进程内存申请");
    QVBoxLayout *allocLayout = new QVBoxLayout(allocGroup);

    QHBoxLayout *pidLayout = new QHBoxLayout;
    editPid = new QLineEdit();
    editPid->setPlaceholderText("进程ID (如 1)");
    pidLayout->addWidget(new QLabel("进程PID:"));
    pidLayout->addWidget(editPid);

    QHBoxLayout *segLayout = new QHBoxLayout;
    editSegCount = new QLineEdit();
    editSegCount->setPlaceholderText("段的数量");
    segLayout->addWidget(new QLabel("段数:"));
    segLayout->addWidget(editSegCount);

    editSegSizes = new QLineEdit();
    editSegSizes->setPlaceholderText("各段大小，用空格隔开 (如: 10 20 5)");

    // --- 新增：初始化算法下拉框 ---
    QHBoxLayout *algoLayout = new QHBoxLayout;
    comboAlgo = new QComboBox();
    comboAlgo->addItem("最先适应算法 (First Fit)");
    comboAlgo->addItem("最佳适应算法 (Best Fit)");
    comboAlgo->addItem("最坏适应算法 (Worst Fit)");
    // 默认选中最坏适应 (索引2)，或者你也可以默认选第一个
    comboAlgo->setCurrentIndex(0);

    algoLayout->addWidget(new QLabel("分配算法:"));
    algoLayout->addWidget(comboAlgo);

    QPushButton *btnAlloc = new QPushButton("执行申请分配"); // 文字改通用一点

    allocLayout->addLayout(pidLayout);
    allocLayout->addLayout(segLayout);
    allocLayout->addWidget(new QLabel("各段大小:"));
    allocLayout->addWidget(editSegSizes);
    allocLayout->addLayout(algoLayout); // 添加算法选择栏
    allocLayout->addWidget(btnAlloc);

    layout->addWidget(allocGroup);

    // --- 4. 操作区域 (回收) ---
    // (保持不变) ...
    QHBoxLayout *freeLayout = new QHBoxLayout;
    QPushButton *btnFree = new QPushButton("回收该进程");
    freeLayout->addWidget(btnFree);
    layout->addLayout(freeLayout);

    // --- 底部状态栏 ---
    statusLabel = new QLabel("就绪");
    layout->addWidget(statusLabel);

    setCentralWidget(centralWidget);
    setWindowTitle("段式存储管理模拟系统"); // 标题改通用一点
    resize(600, 750); //稍微调高一点高度

    connect(btnInit, &QPushButton::clicked, this, &MainWindow::onInitClicked);
    connect(btnAlloc, &QPushButton::clicked, this, &MainWindow::onAllocClicked);
    connect(btnFree, &QPushButton::clicked, this, &MainWindow::onFreeClicked);

    onInitClicked();
}

MainWindow::~MainWindow() {}

void MainWindow::onInitClicked() {
    // (保持不变) ...
    int size = editTotalSize->text().toInt();
    if (size <= 0) {
        QMessageBox::warning(this, "错误", "内存大小必须为正整数");
        return;
    }
    memoryManager.initMemory(size);
    memoryView->update();
    statusLabel->setText(QString("系统已初始化，总内存 %1 KB").arg(size));
}

void MainWindow::onAllocClicked() {
    bool ok;
    int pid = editPid->text().toInt(&ok);
    if (!ok) { QMessageBox::warning(this, "Input Error", "PID 无效"); return; }

    int count = editSegCount->text().toInt(&ok);
    if (!ok || count <= 0) { QMessageBox::warning(this, "Input Error", "段数无效"); return; }

    QString sizeStr = editSegSizes->text();
    // 使用新版Qt写法
    QStringList strList = sizeStr.split(" ", Qt::SkipEmptyParts);

    if (strList.size() != count) {
        QMessageBox::warning(this, "Input Error", "输入的段大小数量与段数不匹配！");
        return;
    }

    std::vector<int> sizes;
    for (const QString &s : strList) {
        int sz = s.toInt(&ok);
        if (!ok || sz <= 0) {
            QMessageBox::warning(this, "Input Error", "段大小必须为正整数");
            return;
        }
        sizes.push_back(sz);
    }

    AllocationAlgo selectedAlgo;
    int index = comboAlgo->currentIndex();
    if (index == 0) selectedAlgo = FIRST_FIT;
    else if (index == 1) selectedAlgo = BEST_FIT;
    else selectedAlgo = WORST_FIT;

    if (memoryManager.requestMemory(pid, count, sizes, selectedAlgo)) {
        statusLabel->setText(QString("进程 %1 分配成功 (%2)").arg(pid).arg(comboAlgo->currentText()));
        memoryView->update();
    } else {
        QMessageBox::critical(this, "分配失败", "内存不足，分配失败！(已回滚)");
        statusLabel->setText(QString("进程 %1 分配失败").arg(pid));
    }
}

void MainWindow::onFreeClicked() {
    bool ok;
    int pid = editPid->text().toInt(&ok);
    if (!ok) return;

    memoryManager.releaseMemory(pid);
    memoryView->update();
    statusLabel->setText(QString("进程 %1 已回收").arg(pid));
}