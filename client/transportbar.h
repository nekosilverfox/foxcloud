#ifndef TRANSPORTBAR_H
#define TRANSPORTBAR_H

#include <QWidget>

namespace Ui {
class TransportBar;
}

class TransportBar : public QWidget
{
    Q_OBJECT

public:
    explicit TransportBar(QWidget *parent = nullptr);
    ~TransportBar();

private:
    Ui::TransportBar *ui;
};

#endif // TRANSPORTBAR_H
