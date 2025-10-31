#include "tools.h"
#include <QGroupBox>
#include <QFile>
#include <QDebug>
#include <QMessageBox>

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
    lineEditWidth = new QLineEdit;
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
    layoutCalcType->addWidget(lineEditWidth, 0, 1);
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

    for (size_t i = 0; i < m_calcPara8.size(); ++i) {
        QListWidgetItem *listWidgetItem = new QListWidgetItem();
        listWidgetItem->setText(QString::fromStdString(m_calcPara8[i].name));
        listWidgetType->addItem(listWidgetItem);
    }

    for (size_t i = 0; i < m_calcPara16.size(); ++i) {
        QListWidgetItem *listWidgetItem = new QListWidgetItem();
        listWidgetItem->setText(QString::fromStdString(m_calcPara16[i].name));
        listWidgetType->addItem(listWidgetItem);
    }

    for (size_t i = 0; i < m_calcPara32.size(); ++i) {
        QListWidgetItem *listWidgetItem = new QListWidgetItem();
        listWidgetItem->setText(QString::fromStdString(m_calcPara32[i].name));
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

        if(strList.at(6).toUInt() <= 8) {
            crcType<uint8_t> tmp;
            bool isOK;
            tmp.name = strList.at(0).toStdString();
            tmp.poly = strList.at(1).toUInt(&isOK, 16);
            tmp.init = strList.at(2).toUInt(&isOK, 16);
            tmp.xOrOut = strList.at(3).toUInt(&isOK, 16);
            tmp.refIn = strList.at(4) == "TRUE" ? true : false;
            tmp.refOut = strList.at(5) == "TRUE" ? true : false;
            tmp.width = strList.at(6).toUInt();
            m_calcPara8.push_back(tmp);
        }
        else if(strList.at(6).toUInt() <= 16) {
            crcType<uint16_t> tmp;
            bool isOK;
            tmp.name = strList.at(0).toStdString();
            tmp.poly = strList.at(1).toUInt(&isOK, 16);
            tmp.init = strList.at(2).toUInt(&isOK, 16);
            tmp.xOrOut = strList.at(3).toUInt(&isOK, 16);
            tmp.refIn = strList.at(4) == "TRUE" ? true : false;
            tmp.refOut = strList.at(5) == "TRUE" ? true : false;
            tmp.width = strList.at(6).toUInt();
            m_calcPara16.push_back(tmp);
        }
        else {
            crcType<uint32_t> tmp;
            bool isOK;
            tmp.name = strList.at(0).toStdString();
            tmp.poly = strList.at(1).toUInt(&isOK, 16);
            tmp.init = strList.at(2).toUInt(&isOK, 16);
            tmp.xOrOut = strList.at(3).toUInt(&isOK, 16);
            tmp.refIn = strList.at(4) == "TRUE" ? true : false;
            tmp.refOut = strList.at(5) == "TRUE" ? true : false;
            tmp.width = strList.at(6).toUInt();
            m_calcPara32.push_back(tmp);
        }
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
    lineEditWidth->setEnabled(bEnabled);
    lineEditPoly->setEnabled(bEnabled);
    lineEditInit->setEnabled(bEnabled);
    lineEditXOROut->setEnabled(bEnabled);
    checkBoxRefIn->setEnabled(bEnabled);
    checkBoxRefOut->setEnabled(bEnabled);
    if(m_calcType < CRC16_ARC) {
        lineEditWidth->setText(QString::number(m_calcPara8[row].width));
        lineEditPoly->setText(QString("0x%1").arg(m_calcPara8[row].poly, 2, 16, QLatin1Char('0')).toUpper());
        lineEditInit->setText(QString("0x%1").arg(m_calcPara8[row].init, 2, 16, QLatin1Char('0')).toUpper());
        lineEditXOROut->setText(QString("0x%1").arg(m_calcPara8[row].xOrOut, 2, 16, QLatin1Char('0')).toUpper());
        checkBoxRefIn->setChecked(m_calcPara8[row].refIn);
        checkBoxRefOut->setChecked(m_calcPara8[row].refOut);
    }
    else if(m_calcType < CRC32_AIXM) {
        lineEditWidth->setText(QString::number(m_calcPara16[row - CRC16_ARC].width));
        lineEditPoly->setText(QString("0x%1").arg(m_calcPara16[row - CRC16_ARC].poly, 4, 16, QLatin1Char('0')).toUpper());
        lineEditInit->setText(QString("0x%1").arg(m_calcPara16[row - CRC16_ARC].init, 4, 16, QLatin1Char('0')).toUpper());
        lineEditXOROut->setText(QString("0x%1").arg(m_calcPara16[row - CRC16_ARC].xOrOut, 4, 16, QLatin1Char('0')).toUpper());
        checkBoxRefIn->setChecked(m_calcPara16[row - CRC16_ARC].refIn);
        checkBoxRefOut->setChecked(m_calcPara16[row - CRC16_ARC].refOut);
    }
    else {
        lineEditWidth->setText(QString::number(m_calcPara32[row - CRC32_AIXM].width));
        lineEditPoly->setText(QString("0x%1").arg(m_calcPara32[row - CRC32_AIXM].poly, 8, 16, QLatin1Char('0')).toUpper());
        lineEditInit->setText(QString("0x%1").arg(m_calcPara32[row - CRC32_AIXM].init, 8, 16, QLatin1Char('0')).toUpper());
        lineEditXOROut->setText(QString("0x%1").arg(m_calcPara32[row - CRC32_AIXM].xOrOut, 8, 16, QLatin1Char('0')).toUpper());
        checkBoxRefIn->setChecked(m_calcPara32[row - CRC32_AIXM].refIn);
        checkBoxRefOut->setChecked(m_calcPara32[row - CRC32_AIXM].refOut);
    }
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
    QString inputStr = textEditIn->toPlainText();
    QString res = "";
    bool isOK;
    m_inputData.clear();
    if(m_calcType != DEC_TO_HEX && m_calcType != FLOAT_TO_HEX && m_calcType != DOUBLE_TO_HEX) {
        if (inputStr.length() % 2 != 0) {
            QMessageBox::critical(this, "错误", "输入数据长度错误！");
            return;
        }
        for (int i = 0; i < inputStr.size(); i += 2) {
            m_inputData.push_back(inputStr.sliced(i, 2).toInt(&isOK, 16));
        }
    }

    switch (m_calcType) {
    case SUM:
        res = QString("%1").arg(calcChkSum(m_inputData), 2, 16, QLatin1Char('0')).toUpper();
        break;
    case XOR:
        res = QString("%1").arg(calcXorSum(m_inputData), 2, 16, QLatin1Char('0')).toUpper();
        break;
    case HEX_TO_DEC:
        res = QString::number(inputStr.toInt(&isOK, 16));
        break;
    case HEX_TO_FLOAT:
        res = QString::number(hexToFloat(inputStr.toStdString()));
        break;
    case HEX_TO_DOUBLE:
        res = QString::number(hexToDouble(inputStr.toStdString()));
        break;
    case DEC_TO_HEX:
        res = QString::number(inputStr.toInt(), 16);
        break;
    case FLOAT_TO_HEX:
        res = QString::fromStdString(floatToHex(inputStr.toFloat()));
        break;
    case DOUBLE_TO_HEX:
        res = QString::fromStdString(doubleToHex(inputStr.toDouble()));
        break;
    default:
        if(m_calcType < CRC16_ARC) {
            crcType<uint8_t> custom;
            res = QString("%1").arg(calcChkRes(custom), 2, 16, QLatin1Char('0')).toUpper();
        }
        else if(m_calcType < CRC32_AIXM) {
            crcType<uint16_t> custom;
            res = QString("%1").arg(calcChkRes(custom), 4, 16, QLatin1Char('0')).toUpper();
        }
        else {
            crcType<uint32_t> custom;
            res = QString("%1").arg(calcChkRes(custom), 8, 16, QLatin1Char('0')).toUpper();
        }
        break;
    }

    textEditOut->setText(res);
}

template<class T>
unsigned int Tools::calcChkRes(crcType<T>& option)
{
    bool isOK;
    option.width = lineEditWidth->text().toInt();
    option.poly = lineEditPoly->text().toUInt(&isOK, 16);
    option.init = lineEditInit->text().toUInt(&isOK, 16);
    option.refIn = checkBoxRefIn->isChecked();
    option.refOut = checkBoxRefOut->isChecked();
    option.xOrOut = lineEditXOROut->text().toUInt(&isOK, 16);
    return crcCheck(option, m_inputData, m_inputData.size());
}
