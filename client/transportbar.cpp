#include "transportbar.h"
#include "ui_transportbar.h"

TransportBar::TransportBar(QWidget *parent, const QString &fileName, const unsigned int maxValue)
    : QWidget(parent)
    , ui(new Ui::TransportBar)
{
    ui->setupUi(this);

    ui->lbFileName->setText(fileName);
    setMaxValue(maxValue);
    setValue(0);
}


TransportBar::~TransportBar()
{
    delete ui;
}

/**
 * @brief TransportBar::setFileName 设置问价名
 * @param name 文件名
 */
void TransportBar::setFileName(QString name)
{
    if (name.isEmpty())
    {
        qWarning() << "File name is empty";
    }

    ui->lbFileName->setText(name);
}

/**
 * @brief TransportBar::setValue 设置当前值
 * @param value
 */
void TransportBar::setValue(const unsigned int value)
{
    ui->progressBar->setValue(value);
    ui->lbPercent->setText(QString::number(value / _max_value, 'f', 2));
}

/**
 * @brief TransportBar::setMaxValue 设置最大值
 * @param value
 */
void TransportBar::setMaxValue(const unsigned int value)
{
    if (0 == value)
    {
        qWarning() << "Max value is 0";
    }
    _max_value = value;
    ui->progressBar->setMaximum(_max_value);
}
