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
#include <QCheckBox>
#include "global.h"

class Tools : public QWidget
{
    Q_OBJECT

public:
    Tools(QWidget *parent = nullptr);
    ~Tools();

    void init();

    void initUi();

    void loadCsvToList();

    void createConnect();

private slots:
    //设置转换参数槽函数
    void setCalcType(QListWidgetItem *item);
    // 模糊查询槽函数
    void fuzzyQuery(const QString &searchText);

private:
    // 数据
    std::vector<unsigned char> m_inputData;
    std::vector<crcType<uint8_t>> m_calcPara8;
    std::vector<crcType<uint16_t>> m_calcPara16;
    std::vector<crcType<uint32_t>> m_calcPara32;
    CalcType m_calcType;

    // 输入
    QTextEdit *textEditIn;

    // 转换
    QLabel *labelQuery;
    QLineEdit *lineEditQuery;
    QListWidget *listWidgetType;
    QPushButton *calcButton;
    QPushButton *clearButton;
    QLabel *labelWidth;
    QLineEdit *lineEditWidth;
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

    void calcRes();

    template <class T>
    unsigned int calcChkRes(crcType<T>& option);
};
#endif // TOOLS_H
