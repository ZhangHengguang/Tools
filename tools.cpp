#include "tools.h"
#include <QGroupBox>

Tools::Tools(QWidget *parent)
    : QWidget(parent)
{
    initUi();
}

Tools::~Tools() {}

void Tools::initUi()
{
    this->resize(900, 600);
    this->setWindowTitle(tr("工具合集v1.0.0"));

    //输入
    textEditIn = new QTextEdit;
    auto groupBoxInput = new QGroupBox(tr("输入"));
    auto layoutInput = new QVBoxLayout;
    layoutInput->addWidget(textEditIn);
    groupBoxInput->setLayout(layoutInput);

    //计算过程
    labelQuery = new QLabel(tr("计算方式(支持模糊查询)"));
    lineEditQuery = new QLineEdit;
    listWidgetType = new QListWidget;
    calcButton = new QPushButton(tr("计算"));
    clearButton = new QPushButton(tr("清空"));
    labelWidth = new QLabel(tr("计算方式(支持模糊查询)"));
    comboBoxWidth = new QComboBox;
    comboBoxWidth->addItem("8");
    comboBoxWidth->addItem("16");
    comboBoxWidth->addItem("32");
    labelPoly = new QLabel(tr("多项式(HEX)"));
    lineEditPoly = new QLineEdit;
    labelInit = new QLabel(tr("初始值(HEX)"));
    lineEditInit = new QLineEdit;
    labelXOROut = new QLabel(tr("结果异或值(HEX)"));
    lineEditXOROut = new QLineEdit;
    checkBoxRefIn = new QCheckBox("输入数据反转");
    checkBoxRefOut = new QCheckBox("输出数据反转");
    auto layoutCalcType = new QGridLayout;
    layoutCalcType->addWidget(labelWidth, 0, 0);
    layoutCalcType->addWidget(comboBoxWidth, 0, 1);
    layoutCalcType->addWidget(labelPoly, 1, 0);
    layoutCalcType->addWidget(lineEditPoly, 1, 1);
    layoutCalcType->addWidget(labelInit, 2, 0);
    layoutCalcType->addWidget(lineEditInit, 2, 1);
    layoutCalcType->addWidget(labelXOROut, 3, 0);
    layoutCalcType->addWidget(lineEditXOROut, 3, 1);
    layoutCalcType->addWidget(checkBoxRefIn, 4, 0);
    layoutCalcType->addWidget(checkBoxRefOut, 4, 1);
    layoutCalcType->addWidget(calcButton, 5, 0);
    layoutCalcType->addWidget(clearButton, 5, 1);
    auto groupBoxCalc = new QGroupBox(tr("计算过程"));
    auto layoutCalc = new QVBoxLayout;
    layoutCalc->addWidget(labelQuery);
    layoutCalc->addWidget(lineEditQuery);
    layoutCalc->addWidget(listWidgetType);
    layoutCalc->addLayout(layoutCalcType);
    groupBoxCalc->setLayout(layoutCalc);

    //输出
    textEditOut = new QTextEdit;
    auto groupBoxOutput = new QGroupBox(tr("输出"));
    auto layoutOutput = new QVBoxLayout;
    layoutOutput->addWidget(textEditOut);
    groupBoxOutput->setLayout(layoutOutput);

    auto mainLayout = new QHBoxLayout;
    mainLayout->addWidget(groupBoxInput);
    mainLayout->addWidget(groupBoxCalc);
    mainLayout->addWidget(groupBoxOutput);

    this->setLayout(mainLayout);
}
