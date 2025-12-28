#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox> // --- 新增头文件 ---
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
    MemoryManager memoryManager;

    QLineEdit *editTotalSize;
    QLineEdit *editPid;
    QLineEdit *editSegCount;
    QLineEdit *editSegSizes;

    // --- 新增：算法选择下拉框 ---
    QComboBox *comboAlgo;

    MemoryWidget *memoryView;
    QLabel *statusLabel;
};

#endif // MAINWINDOW_H