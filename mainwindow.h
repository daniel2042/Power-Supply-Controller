#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QSlider>
#include <QLabel>
#include <QMessageBox>
#include <QComboBox>
#include <QFrame>
#include <QPainter>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "vcontrol.h"


#define ASK_BEFORE_ENABLE


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    VControl        control;

    QTimer*         tim;
    QPushButton*    power_on;
    QPushButton*    ramp_button;
    QPushButton*    sinus_button;
    QPushButton*    serialport_open_button;
    QPushButton*    refresh;
    QLabel*         ramp_label;
    QLabel*         sinus_label;
    QLabel*         ramp_unit;
    QLabel*         sinus_unit;
    QLineEdit*      ramp_value;
    QLineEdit*      sinus_amplitude;
    QCheckBox*      ramp_checkbox;
    QCheckBox*      sinus_checkbox;
    QCheckBox*      softstart_checkbox;
    QComboBox*      version_combobox;
    QComboBox*      serialport_combobox;
    QSlider*        sinus_frequency;
    QLabel*         sinus_frequency_label;
    QLabel*         sinus_frequency_unit;
    QPushButton*    sinus_frequency_button;
    QFrame*         line1;
    QFrame*         line2;
    QLabel*         out_voltage_label;
    QLabel*         out_voltage_unit;
    QLineEdit*      out_voltage;
    QLabel*         out_voltage_read;
    QPushButton*    out_voltage_button;
    QPushButton*    high_voltage_indicator;
    QSerialPort*    serial;

public:
    explicit MainWindow(QWidget *parent = 0);

public slots:
    void Output();
    void Voltage_Set();
    void Rise_Set();
    void Sinus_Set();
    void Frequency_Set();
    void Soft_Start(bool n);
    void Find_COM();
    void Open_COM();
    void COM();

};

#endif // MAINWINDOW_H
