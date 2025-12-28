#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "MemoryManager.h"
#include "MemoryWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
            void onInitClicked();
    void onAllocClicked();
    void onFreeClicked();

private:
    MemoryManager memoryManager; // 核心逻辑实例

    // UI 控件
    QLineEdit *editTotalSize;
    QLineEdit *editPid;
    QLineEdit *editSegCount;
    QLineEdit *editSegSizes; // 输入格式 "10 20 30"

    MemoryWidget *memoryView;
    QLabel *statusLabel;
};

#endif // MAINWINDOW_H