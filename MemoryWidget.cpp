#include "MemoryWidget.h"
#include <QPainter>
#include <QDebug>

MemoryWidget::MemoryWidget(QWidget *parent) : QWidget(parent), manager(nullptr) {
    // 设置最小高度，确保能看到
    setMinimumHeight(150);
    // 设置黑色背景
    setStyleSheet("background-color: #2b2b2b;");
}

void MemoryWidget::setManager(MemoryManager *mgr) {
    this->manager = mgr;
    update(); // 触发重绘
}

void MemoryWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    if (!manager || manager->getTotalMemory() <= 0) return;

    QPainter painter(this);
    int total = manager->getTotalMemory();
    double scale = (double)width() / total; // 计算比例尺 (像素/KB)

    // 1. 绘制底色 (灰色表示未使用)
    painter.setBrush(QColor(200, 200, 200));
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, width(), height());

    // 2. 绘制已分配块 (彩色)
    const auto& allocList = manager->getAllocList();
    for (const auto& block : allocList) {
        int x = (int)(block.startAddr * scale);
        int w = (int)(block.size * scale);

        // 给不同的进程不同的颜色 (简单的哈希颜色)
        QColor color = QColor::fromHsv((block.pid * 50) % 360, 200, 200);

        // 绘制矩形
        painter.setBrush(color);
        painter.setPen(Qt::black);
        painter.drawRect(x, 0, w, height());

        // 绘制文字信息
        if (w > 20) { // 如果宽度足够才写字
            painter.setPen(Qt::black);
            QString info = QString("PID:%1\nS:%2").arg(block.pid).arg(block.segId);
            painter.drawText(x + 2, 20, info);
        }
    }

    // 3. 绘制空闲块文字标识 (可选)
    const auto& freeList = manager->getFreeList();
    painter.setPen(Qt::darkGray);
    for (const auto& block : freeList) {
        int x = (int)(block.startAddr * scale);
        int w = (int)(block.size * scale);
        if (w > 30) {
            painter.drawText(x + 5, height() - 10, QString("Free: %1").arg(block.size));
        }
    }
}