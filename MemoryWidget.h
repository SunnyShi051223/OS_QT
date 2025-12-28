#ifndef MEMORYWIDGET_H
#define MEMORYWIDGET_H

#include <QWidget>
#include "MemoryManager.h"

class MemoryWidget : public QWidget {
    Q_OBJECT
public:
    explicit MemoryWidget(QWidget *parent = nullptr);
    // 传入管理器指针，以便获取数据绘图
    void setManager(MemoryManager *mgr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    MemoryManager *manager;
};

#endif // MEMORYWIDGET_H