#ifndef TRANSPORTWIDGET_H
#define TRANSPORTWIDGET_H

#include <QWidget>

namespace Ui {
class TransportWidget;
}

class TransportWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TransportWidget(QWidget *parent = nullptr);
    ~TransportWidget();

    void turnToTabUpload();
    void turnToTabDownload();

private:
    Ui::TransportWidget *ui;
};

#endif // TRANSPORTWIDGET_H
