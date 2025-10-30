#include "tools.h"
#include <QGroupBox>
#include <QFile>
#include <QDebug>

Tools::Tools(QWidget *parent)
    : QWidget(parent)
{
    init();
    initUi();
    createConnect();
}

Tools::~Tools() {}

void Tools::init()
{
    loadCsvToList();
}

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
    labelWidth = new QLabel(tr("宽度"));
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

    for (size_t i = 0; i < m_calcPara.size(); ++i) {
        QListWidgetItem *listWidgetItem = new QListWidgetItem();
        listWidgetItem->setText(QString::fromStdString(m_calcPara[i].name));
        listWidgetType->addItem(listWidgetItem);
    }

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

void Tools::loadCsvToList()
{
    QString filePath = ":/data/CheckSumPara.csv";
    QFile calcData;
    calcData.setFileName(filePath);
    calcData.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&calcData);
    in.setEncoding(QStringConverter::Utf8);
    while(!in.atEnd()) {
        QString line = in.readLine().trimmed();
        QStringList strList = line.split(",");
        CalcPara tmp;
        bool isOK;
        tmp.name = strList.at(0).toStdString();
        tmp.poly = strList.at(1).toUInt(&isOK, 16);
        tmp.init = strList.at(2).toUInt(&isOK, 16);
        tmp.xOrOut = strList.at(3).toUInt(&isOK, 16);
        tmp.refIn = strList.at(4) == "TRUE" ? true : false;
        tmp.refOut = strList.at(5) == "TRUE" ? true : false;
        tmp.width = strList.at(6).toUInt();
        m_calcPara.push_back(tmp);
    }
}

void Tools::createConnect()
{
    void (Tools:: *setType)(QListWidgetItem *item) = &Tools::setCalcType;
    connect(listWidgetType, &QListWidget::itemClicked, this, setType);
    connect(calcButton, &QPushButton::clicked, this, &Tools::calcRes);
    connect(clearButton, &QPushButton::clicked, this, [=](){
        textEditIn->clear();
        textEditOut->clear();
    });
    // 模糊匹配槽函数
    void(Tools:: *fuzzy)(const QString &) = &Tools::fuzzyQuery;
    connect(lineEditQuery, &QLineEdit::textChanged, this, fuzzy);
}

void Tools::setCalcType(QListWidgetItem *item)
{
    int row = listWidgetType->row(item);
    m_calcType = static_cast<CalcType>(row);
    bool bEnabled = m_calcType <= DOUBLE_TO_HEX ? true : false;
    comboBoxWidth->setEnabled(bEnabled);
    lineEditPoly->setEnabled(bEnabled);
    lineEditInit->setEnabled(bEnabled);
    lineEditXOROut->setEnabled(bEnabled);
    checkBoxRefIn->setEnabled(bEnabled);
    checkBoxRefOut->setEnabled(bEnabled);
    comboBoxWidth->setCurrentText(QString::number(m_calcPara[row].width));
    lineEditPoly->setText(QString("0x%1").arg(m_calcPara[row].poly, m_calcPara[row].width /4, 16, QLatin1Char('0')).toUpper());
    lineEditInit->setText(QString("0x%1").arg(m_calcPara[row].init, m_calcPara[row].width /4, 16, QLatin1Char('0')).toUpper());
    lineEditXOROut->setText(QString("0x%1").arg(m_calcPara[row].xOrOut, m_calcPara[row].width /4, 16, QLatin1Char('0')).toUpper());
    checkBoxRefIn->setChecked(m_calcPara[row].refIn);
    checkBoxRefOut->setChecked(m_calcPara[row].refOut);
}

void Tools::fuzzyQuery(const QString &searchText)
{
    for (int i = 0; i < listWidgetType->count(); ++i) {
        QListWidgetItem *item = listWidgetType->item(i);
        QString itemText = item->text();
        bool match = itemText.contains(searchText, Qt::CaseInsensitive);
        item->setHidden(!match);
    }
}

void Tools::calcRes()
{

    switch (m_calcType) {
    case CUSTOM:
        //TODO:CUSTOM对应函数
        break;
    default:
        break;
    }
}
