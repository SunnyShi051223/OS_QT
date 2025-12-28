#ifndef MEMORYWIDGET_H
#define MEMORYWIDGET_H

#include <QWidget>
#include "MemoryManager.h"

class MemoryWidget : public QWidget {
    Q_OBJECT
public:
    explicit MemoryWidget(QWidget *parent = nullptr);
    void setManager(MemoryManager *mgr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    MemoryManager *manager;
};

#endif // MEMORYWIDGET_H