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
    explicit TransportBar(QWidget *parent = nullptr, unsigned int maxValue = 0);
    ~TransportBar();

    void setFileName(QString name = "FILENAME");
    void setValue(unsigned int value = 0);
    void setMaxValue(unsigned int value = 0);

private:
    Ui::TransportBar *ui;

    unsigned int _max_value;
};

#endif // TRANSPORTBAR_H
