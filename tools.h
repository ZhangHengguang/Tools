#ifndef TOOLS_H
#define TOOLS_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QComboBox>
#include <QCheckBox>

class Tools : public QWidget
{
    Q_OBJECT

public:
    Tools(QWidget *parent = nullptr);
    ~Tools();

    void initUi();

private:
    // 数据
    std::vector<uint8_t> m_inputData;

    // 输入
    QTextEdit *textEditIn;

    // 转换
    QLabel *labelQuery;
    QLineEdit *lineEditQuery;
    QListWidget *listWidgetType;
    QPushButton *calcButton;
    QPushButton *clearButton;
    QLabel *labelWidth;
    QComboBox *comboBoxWidth;
    QLabel *labelPoly;
    QLineEdit *lineEditPoly;
    QLabel *labelInit;
    QLineEdit *lineEditInit;
    QLabel *labelXOROut;
    QLineEdit *lineEditXOROut;
    QCheckBox *checkBoxRefIn;
    QCheckBox *checkBoxRefOut;

    // 输出
    QTextEdit *textEditOut;
};
#endif // TOOLS_H
