
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "servominmax.h"
#include "ucrc16lib.h"

//#define DEBUG_HT

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    waitingOnParameters = false;
    msgbox = new QMessageBox(this);

    // Start the board interface
    nano33ble = new BoardNano33BLE(&trkset);
    bno055 = new BoardBNO055(&trkset);

    // Add it to the list of available boards
    boards.append(nano33ble);
    boards.append(bno055);

    // Once correct board is discovered this will be set to one of the above boards
    currentboard = nullptr;
    ui->tabBLE->setCurrentIndex(0);

    setWindowTitle(windowTitle() + " " + version + " " + versionsuffix);

    // Serial Connection
    serialcon = new QSerialPort;

    // Diagnostic Display + Serial Debug
    diagnostic = new DiagnosticDisplay(&trkset,this);
    diagnostic->setWindowFlags(Qt::Window);
    serialDebug = new QTextEdit(this);
    serialDebug->setWindowFlags(Qt::Window);
    serialDebug->setWindowTitle("Serial Information");
    serialDebug->resize(600,300);
    channelviewer = new ChannelViewer(&trkset, this);
    channelviewer->setWindowFlags(Qt::Window);

#ifdef DEBUG_HT
    serialDebug->show();
    diagnostic->show();
#endif

    // Hide these buttons until connected
    ui->cmdStartGraph->setVisible(false);
    ui->cmdStopGraph->setVisible(false);
    ui->chkRawData->setVisible(false);

    // Firmware loader loader dialog
    firmwareWizard = nullptr;

    // Get list of available ports
    findSerialPorts();

    // Use system default fixed witdh font
    QFont serifFont("Times", 10, QFont::Bold);
    serialDebug->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    // Update default settings to UI
    updateToUI();

    // Board Connections, connects all boards signals to same end points
    foreach(BoardType *brd, boards) {
        connect(brd,SIGNAL(paramSendStart()), this, SLOT(paramSendStart()));
        connect(brd,SIGNAL(paramSendComplete()), this, SLOT(paramSendComplete()));
        connect(brd,SIGNAL(paramSendFailure(int)), this, SLOT(paramSendFailure(int)));
        connect(brd,SIGNAL(paramReceiveStart()), this, SLOT(paramReceiveStart()));
        connect(brd,SIGNAL(paramReceiveComplete()), this, SLOT(paramReceiveComplete()));
        connect(brd,SIGNAL(paramReceiveFailure(int)), this, SLOT(paramReceiveFailure(int)));
        connect(brd,SIGNAL(calibrationSuccess()), this, SLOT(calibrationSuccess()));
        connect(brd,SIGNAL(calibrationFailure()), this, SLOT(calibrationFailure()));
        connect(brd,SIGNAL(serialTxReady()), this, SLOT(serialTxReady()));
        connect(brd,SIGNAL(addToLog(QString,int)),this,SLOT(addToLog(QString,int)));
        connect(brd,SIGNAL(needsCalibration()),this,SLOT(needsCalibration()));
        connect(brd,SIGNAL(boardDiscovered(BoardType *)),this,SLOT(boardDiscovered(BoardType *)));
        connect(brd,SIGNAL(statusMessage(QString,int)),this,SLOT(statusMessage(QString,int)));
    }

    // Serial data ready
    connect(serialcon,SIGNAL(readyRead()),this,SLOT(serialReadReady()));
    connect(serialcon, SIGNAL(errorOccurred(QSerialPort::SerialPortError)),this,SLOT(serialError(QSerialPort::SerialPortError)));

    // Buttons
    connect(ui->cmdConnect,SIGNAL(clicked()),this,SLOT(serialConnect()));
    connect(ui->cmdDisconnect,SIGNAL(clicked()),this,SLOT(serialDisconnect()));    
    connect(ui->cmdStore,SIGNAL(clicked()),this,SLOT(storeToRAM()));
    connect(ui->cmdSend,SIGNAL(clicked()),this,SLOT(manualSend()));
    //connect(ui->cmdStartGraph,SIGNAL(clicked()),this,SLOT(startGraph()));
    //connect(ui->cmdStopGraph,SIGNAL(clicked()),this,SLOT(stopGraph()));
    connect(ui->cmdResetCenter,SIGNAL(clicked()),this, SLOT(resetCenter()));
    connect(ui->cmdCalibrate,SIGNAL(clicked()),this, SLOT(startCalibration()));
    connect(ui->cmdSaveNVM,SIGNAL(clicked()),this,SLOT(storeToNVM()));
    connect(ui->cmdReboot,SIGNAL(clicked()),this,SLOT(reboot()));
    connect(ui->cmdChannelViewer,SIGNAL(clicked()),ui->actionChannel_Viewer, SLOT(trigger()));
    connect(ui->cmdRefresh,&QPushButton::clicked,this,&MainWindow::findSerialPorts);

    // Check Boxes
    connect(ui->chkpanrev,SIGNAL(clicked(bool)),this,SLOT(updateFromUI()));
    connect(ui->chkrllrev,SIGNAL(clicked(bool)),this,SLOT(updateFromUI()));
    connect(ui->chktltrev,SIGNAL(clicked(bool)),this,SLOT(updateFromUI()));
    connect(ui->chkInvertedPPM,SIGNAL(clicked(bool)),this,SLOT(updateFromUI()));
    connect(ui->chkInvertedPPMIn,SIGNAL(clicked(bool)),this,SLOT(updateFromUI()));
    connect(ui->chkResetCenterWave,SIGNAL(clicked(bool)),this,SLOT(updateFromUI()));
    connect(ui->chkSbusInInv,SIGNAL(clicked(bool)),this,SLOT(updateFromUI()));
    connect(ui->chkSbusOutInv,SIGNAL(clicked(bool)),this,SLOT(updateFromUI()));

    //connect(ui->chkRawData,SIGNAL(clicked(bool)),this,SLOT(setDataMode(bool)));

    // Spin Boxes
    connect(ui->spnLPPan,SIGNAL(valueChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->spnLPTiltRoll,SIGNAL(valueChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->spnLPPan2,SIGNAL(valueChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->spnLPTiltRoll2,SIGNAL(valueChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->spnPPMSync,SIGNAL(valueChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->spnPPMFrameLen,SIGNAL(valueChanged(double)),this,SLOT(updateFromUI()));
    connect(ui->spnA5Gain,SIGNAL(valueChanged(double)),this,SLOT(updateFromUI()));
    connect(ui->spnA5Off,SIGNAL(valueChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->spnA6Gain,SIGNAL(valueChanged(double)),this,SLOT(updateFromUI()));
    connect(ui->spnA6Off,SIGNAL(valueChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->spnA7Gain,SIGNAL(valueChanged(double)),this,SLOT(updateFromUI()));
    connect(ui->spnA7Off,SIGNAL(valueChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->spnRotX,SIGNAL(valueChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->spnRotY,SIGNAL(valueChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->spnRotZ,SIGNAL(valueChanged(int)),this,SLOT(updateFromUI()));

    // Gain Sliders
    connect(ui->til_gain,SIGNAL(valueChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->rll_gain,SIGNAL(valueChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->pan_gain,SIGNAL(valueChanged(int)),this,SLOT(updateFromUI()));
    ui->til_gain->setMaximum(TrackerSettings::MAX_GAIN*10);
    ui->rll_gain->setMaximum(TrackerSettings::MAX_GAIN*10);
    ui->pan_gain->setMaximum(TrackerSettings::MAX_GAIN*10);

    // Servo Scaling Widgets
    connect(ui->servoPan,&ServoMinMax::minimumChanged,this,&MainWindow::updateFromUI);
    connect(ui->servoPan,&ServoMinMax::maximumChanged,this,&MainWindow::updateFromUI);
    connect(ui->servoPan,&ServoMinMax::centerChanged,this,&MainWindow::updateFromUI);
    connect(ui->servoTilt,&ServoMinMax::minimumChanged,this,&MainWindow::updateFromUI);
    connect(ui->servoTilt,&ServoMinMax::maximumChanged,this,&MainWindow::updateFromUI);
    connect(ui->servoTilt,&ServoMinMax::centerChanged,this,&MainWindow::updateFromUI);
    connect(ui->servoRoll,&ServoMinMax::minimumChanged,this,&MainWindow::updateFromUI);
    connect(ui->servoRoll,&ServoMinMax::maximumChanged,this,&MainWindow::updateFromUI);
    connect(ui->servoRoll,&ServoMinMax::centerChanged,this,&MainWindow::updateFromUI);

    // On Live Data Change
    connect(&trkset,&TrackerSettings::rawOrientChanged,this,&MainWindow::offOrientChanged);
    connect(&trkset,&TrackerSettings::offOrientChanged,this,&MainWindow::offOrientChanged);
    connect(&trkset,&TrackerSettings::ppmOutChanged,this,&MainWindow::ppmOutChanged);
    connect(&trkset,&TrackerSettings::liveDataChanged,this,&MainWindow::liveDataChanged);
    connect(&trkset,&TrackerSettings::bleAddressDiscovered,this,&MainWindow::bleAddressDiscovered);

    // Combo Boxes
        // Add Remap Choices + The corresponding values
    ui->cmbRemap->addItem("X,Y,Z",AXES_MAP(AXIS_X,AXIS_Y,AXIS_Z));
    ui->cmbRemap->addItem("X,Z,Y",AXES_MAP(AXIS_X,AXIS_Z,AXIS_Y));
    ui->cmbRemap->addItem("Y,X,Z",AXES_MAP(AXIS_Y,AXIS_X,AXIS_Z));
    ui->cmbRemap->addItem("Y,Z,X",AXES_MAP(AXIS_Y,AXIS_Z,AXIS_X));
    ui->cmbRemap->addItem("Z,X,Y",AXES_MAP(AXIS_Z,AXIS_X,AXIS_Y));
    ui->cmbRemap->addItem("Z,Y,Z",AXES_MAP(AXIS_Z,AXIS_Y,AXIS_X));
    ui->cmbRemap->setCurrentIndex(0);

    connect(ui->cmbpanchn,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->cmbtiltchn,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->cmbrllchn,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->cmbRemap,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->cmbSigns,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->cmbButtonPin,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->cmbPpmInPin,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->cmbPpmOutPin,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->cmbBtMode,SIGNAL(currentIndexChanged(int)),this,SLOT(BTModeChanged()));
    connect(ui->cmbResetOnPPM,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->cmbPPMChCount,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->cmbA6Ch,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->cmbA7Ch,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->cmbAuxFn0,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->cmbAuxFn1,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->cmbAuxFn0Ch,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->cmbAuxFn1Ch,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->cmbPWM0,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->cmbPWM1,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->cmbPWM2,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->cmbPWM3,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFromUI()));
    connect(ui->cmbBTRmtMode,SIGNAL(currentIndexChanged(int)),this,SLOT(updateFromUI()));

    // Menu Actions
    connect(ui->action_Save_to_File,SIGNAL(triggered()),this,SLOT(saveSettings()));
    connect(ui->action_Load,SIGNAL(triggered()),this,SLOT(loadSettings()));
    connect(ui->actionE_xit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionFirmware_Wizard,SIGNAL(triggered()),this,SLOT(uploadFirmwareWizard()));
    connect(ui->actionShow_Data,SIGNAL(triggered()),this,SLOT(showDiagsClicked()));
    connect(ui->actionShow_Serial_Transmissions,SIGNAL(triggered()),this,SLOT(showSerialDiagClicked()));
    connect(ui->actionChannel_Viewer,SIGNAL(triggered()),this,SLOT(showChannelViewerClicked()));

    // Tab Widget
    connect(ui->tabBLE,&QTabWidget::currentChanged,this,&MainWindow::BLE33tabChanged);

    // Timers    
    connect(&rxledtimer,SIGNAL(timeout()),this,SLOT(rxledtimeout()));
    rxledtimer.setInterval(100);
    connect(&txledtimer,SIGNAL(timeout()),this,SLOT(txledtimeout()));
    txledtimer.setInterval(100);
    connect(&connectTimer,SIGNAL(timeout()),this,SLOT(connectTimeout()));
    connectTimer.setSingleShot(true);
    connect(&requestTimer,SIGNAL(timeout()),this,SLOT(requestTimeout()));
    requestTimer.setSingleShot(true);
    connect(&saveToRAMTimer,SIGNAL(timeout()),this,SLOT(saveToRAMTimeout()));
    saveToRAMTimer.setSingleShot(true);
    connect(&requestParamsTimer,SIGNAL(timeout()),this,SLOT(requestParamsTimeout()));
    requestParamsTimer.setSingleShot(true);

    // Called to initalize GUI state to disconnected
    serialDisconnect();
}

MainWindow::~MainWindow()
{
    delete serialcon;
    delete nano33ble;
    if(firmwareWizard != nullptr)
        delete firmwareWizard;
    delete serialDebug;
    delete ui;
}

// Connects to the serial port

void MainWindow::serialConnect()
{
    QString port = ui->cmbPort->currentText();
    if(port.isEmpty())
        return;

    // If open close it first
    if(serialcon->isOpen())
        serialDisconnect();

    // Setup serial port 8N1, 57600 Baud
    serialcon->setPortName(port);
    serialcon->setParity(QSerialPort::NoParity);
    serialcon->setDataBits(QSerialPort::Data8);
    serialcon->setStopBits(QSerialPort::OneStop);
    serialcon->setBaudRate(QSerialPort::Baud115200); // CDC doesn't actuall make a dif.. cool
    serialcon->setFlowControl(QSerialPort::NoFlowControl);

    if(!serialcon->open(QIODevice::ReadWrite)) {
        QMessageBox::critical(this,"Error",tr("Could not open Com ") + serialcon->portName());
        addToLog("Could not open " + serialcon->portName(),2);
        return;
    }

    logd.clear();
    serialDebug->clear();
    trkset.clear();

    ui->cmdDisconnect->setEnabled(true);
    ui->cmdConnect->setEnabled(false);
    ui->cmdChannelViewer->setEnabled(false);
    addToLog("Connected to " + serialcon->portName());
    statusMessage(tr("Connected to ") + serialcon->portName());

    ui->stackedWidget->setCurrentIndex(1);

    serialcon->setDataTerminalReady(true);   

    requestTimer.stop();
    requestTimer.start(4000);
}

// Disconnect from the serial port
// Reset all gui settings to disable mode

void MainWindow::serialDisconnect()
{
    if(serialcon->isOpen()) {
        // Check if user wants to save first
        if(!checkSaved())
            return;

        addToLog("Disconnecting from " + serialcon->portName());

        serialcon->flush();
        serialcon->close();
    }

    foreach(BoardType *brd, boards) {
        brd->_disconnected();
        brd->allowAccess(false);
    }

    statusMessage(tr("Disconnected"));
    ui->cmdDisconnect->setEnabled(false);
    ui->cmdChannelViewer->setEnabled(false);
    ui->cmdConnect->setEnabled(true);
    ui->cmdStopGraph->setEnabled(false);
    ui->cmdStartGraph->setEnabled(false);
    ui->cmdSaveNVM->setEnabled(false);
    ui->cmdReboot->setEnabled(false);
    ui->cmdStore->setEnabled(false);
    ui->servoPan->setShowActualPosition(true);
    ui->servoTilt->setShowActualPosition(true);
    ui->servoRoll->setShowActualPosition(true);
    ui->cmdSend->setEnabled(false);
    ui->cmdCalibrate->setEnabled(false);    
    ui->stackedWidget->setCurrentIndex(0);
    ui->servoPan->setShowActualPosition(false);
    ui->servoTilt->setShowActualPosition(false);
    ui->servoRoll->setShowActualPosition(false);

    sending = false;
    currentboard = nullptr;
    boardRequestIndex=0;
    connectTimer.stop();
    saveToRAMTimer.stop();
    requestTimer.stop();
    requestParamsTimer.stop();
    waitingOnParameters = false;
    serialData.clear();
    channelviewer->setBoard(nullptr);
    channelviewer->hide();
    trkset.clearDataItems();

    // Notify all boards we have disconnected
}

void MainWindow::serialError(QSerialPort::SerialPortError err)
{
    switch(err) {
    // Issue with connection - device unplugged
    case QSerialPort::ResourceError: {
        addToLog(tr("Connection lost"),2);
        serialcon->close();
        serialDisconnect();
        break;
    }
    default: {
        qDebug() << "SERIAL PORT ERROR" << err;
    }
    }
}

/* - Checks if the boards firmware was actually found.
 *   This is called 1sec after a request is sent for the board version
 */

void MainWindow::connectTimeout()
{
    if(currentboard != nullptr && serialcon->isOpen()) {
        QMessageBox::information(this,"Error", "No valid board detected\nPlease check COM port or flash proper code");
        serialDisconnect();
    }
}

/* sendSerialData()
 *      Send Raw Data To The Serial Port
 */

void MainWindow::sendSerialData(QByteArray data)
{
    if(data.isEmpty() || !serialcon->isOpen())
        return;

    bool done = true;
    while(done) {
        int nlindex = data.indexOf("\r\n");
        if(nlindex < 0)
           break;  // No New line found

        // Strip data up the the CR LF \r\n
        QByteArray sdata = data.left(nlindex) + "\r\n";

        serialDataOut.enqueue(sdata);
        slowSerialSend();

        // Skip nuisance I'm here message
        if(QString(sdata).contains("{\"Cmd\":\"IH\"}"))
            return;

        addToLog("GUI: " + sdata + "\n");
        data = data.right(data.length()-nlindex-2);
    }

    ui->txled->setState(true);
    txledtimer.start();
}

void MainWindow::slowSerialSend()
{
    if(sending)
        return;

    sending = true;

    while(serialDataOut.length()) {
        QByteArray sdata = serialDataOut.dequeue();
        // Delay sends no more tha 64 bytes at a time
        int pos=0;
        while(pos<sdata.length()) {
            qDebug() << "Serial Out:" << sdata.mid(pos,64);
            serialcon->write(sdata.mid(pos,64));
            pos +=64;
            QTime dieTime= QTime::currentTime().addMSecs(5);
            while (QTime::currentTime() < dieTime)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    }

    sending = false;
}

/* addToLog()
 *      Add other information to the LOG
 */

void MainWindow::addToLog(QString log, int ll)
{
    QString color = "black";
    if(ll==2) // Debug
        color = "red";
    else if(log.contains("HT:"))
        color = "green";
    else if(log.contains("GUI:"))
        color = "blue";
    else if(log.contains("\"Cmd\":\"Data\"")) // Don't show return measurment data
        return;

    logd += "<font color=\"" + color + "\">" + log + "</font><br>";

    // Limit Max Log Length
    int loglen = logd.length();
    if(loglen > MAX_LOG_LENGTH)
        logd = logd.mid(logd.length() - MAX_LOG_LENGTH);

    // Set Gui text from local string
    serialDebug->setHtml(logd);

    // Scroll to bottom
    serialDebug->verticalScrollBar()->setValue(serialDebug->verticalScrollBar()->maximum());
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->type() == QKeyEvent::KeyPress) {
        if((event->modifiers() & Qt::ControlModifier) &&
           (event->key() & Qt::Key_D)) {
            // Ctrl - D Pressed
            diagnostic->show();
            diagnostic->activateWindow();
            diagnostic->raise();
        }
    }
}

// Finds available serial ports
void MainWindow::findSerialPorts()
{
    ui->cmbPort->clear();
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    foreach(QSerialPortInfo port,ports) {
        ui->cmbPort->addItem(port.portName(),port.serialNumber());
    }
}

// Update the UI Settings from the settings class
void MainWindow::updateToUI()
{
    // Don't update GUI if haven't got data from the device yet
    if(waitingOnParameters)
        return;

    ui->servoTilt->setCenter(trkset.Tlt_cnt());
    ui->servoTilt->setMaximum(trkset.Tlt_max());
    ui->servoTilt->setMinimum(trkset.Tlt_min());

    ui->servoPan->setCenter(trkset.Pan_cnt());
    ui->servoPan->setMaximum(trkset.Pan_max());
    ui->servoPan->setMinimum(trkset.Pan_min());

    ui->servoRoll->setCenter(trkset.Rll_cnt());
    ui->servoRoll->setMaximum(trkset.Rll_max());
    ui->servoRoll->setMinimum(trkset.Rll_min());

    ui->chkpanrev->setChecked(trkset.isPanReversed());
    ui->chkrllrev->setChecked(trkset.isRollReversed());
    ui->chktltrev->setChecked(trkset.isTiltReversed());
    ui->chkInvertedPPM->setChecked(trkset.invertedPpmOut());
    ui->chkInvertedPPMIn->setChecked(trkset.invertedPpmIn());
    ui->chkResetCenterWave->setChecked(trkset.resetOnWave());
    ui->chkSbusInInv->setChecked(trkset.invertedSBUSIn());
    ui->chkSbusOutInv->setChecked(trkset.invertedSBUSOut());

    // Prevents signals from these items causing another update
    ui->cmbpanchn->blockSignals(true);
    ui->cmbrllchn->blockSignals(true);
    ui->cmbtiltchn->blockSignals(true);
    ui->cmbRemap->blockSignals(true);
    ui->cmbPpmOutPin->blockSignals(true);
    ui->spnPPMFrameLen->blockSignals(true);
    ui->cmbPPMChCount->blockSignals(true);
    ui->spnPPMSync->blockSignals(true);
    ui->cmbPpmInPin->blockSignals(true);
    ui->cmbButtonPin->blockSignals(true);
    ui->cmbBtMode->blockSignals(true);    
    ui->cmbBTRmtMode->blockSignals(true);
    ui->cmbResetOnPPM->blockSignals(true);
    ui->spnLPPan->blockSignals(true);
    ui->spnLPTiltRoll->blockSignals(true);
    ui->spnLPPan2->blockSignals(true);
    ui->spnLPTiltRoll2->blockSignals(true);
    ui->til_gain->blockSignals(true);
    ui->rll_gain->blockSignals(true);
    ui->pan_gain->blockSignals(true);
    ui->cmbA5Ch->blockSignals(true);
    ui->spnA5Gain->blockSignals(true);
    ui->spnA5Off->blockSignals(true);
    ui->cmbA6Ch->blockSignals(true);
    ui->spnA6Gain->blockSignals(true);
    ui->spnA6Off->blockSignals(true);
    ui->cmbA7Ch->blockSignals(true);
    ui->spnA7Gain->blockSignals(true);
    ui->spnA7Off->blockSignals(true);
    ui->cmbAuxFn0->blockSignals(true);
    ui->cmbAuxFn1->blockSignals(true);
    ui->cmbAuxFn0Ch->blockSignals(true);
    ui->cmbAuxFn1Ch->blockSignals(true);
    ui->cmbPWM0->blockSignals(true);
    ui->cmbPWM1->blockSignals(true);
    ui->cmbPWM2->blockSignals(true);
    ui->cmbPWM3->blockSignals(true);    
    ui->spnRotX->blockSignals(true);
    ui->spnRotY->blockSignals(true);
    ui->spnRotZ->blockSignals(true);

    ui->spnLPTiltRoll->setValue(trkset.lpTiltRoll());
    ui->spnLPPan->setValue(trkset.lpPan());
    ui->spnLPTiltRoll2->setValue(trkset.lpTiltRoll());
    ui->spnLPPan2->setValue(trkset.lpPan());
    ui->spnA5Gain->setValue(trkset.analog5Gain());
    ui->spnA5Off->setValue(trkset.analog5Offset());
    ui->spnA6Gain->setValue(trkset.analog6Gain());
    ui->spnA6Off->setValue(trkset.analog6Offset());
    ui->spnA7Off->setValue(trkset.analog7Offset());
    ui->spnA7Gain->setValue(trkset.analog7Gain());

    int panCh = trkset.panCh();
    int rllCh = trkset.rollCh();
    int tltCh = trkset.tiltCh();
    int a5Ch = trkset.analog5Ch();
    int a6Ch = trkset.analog6Ch();
    int a7Ch = trkset.analog7Ch();
    int auxF0Ch = trkset.auxFunc0Ch();
    int auxF1Ch = trkset.auxFunc1Ch();
    int pwm0Ch = trkset.pwmCh(0);
    int pwm1Ch = trkset.pwmCh(1);
    int pwm2Ch = trkset.pwmCh(2);
    int pwm3Ch = trkset.pwmCh(3);

    // Tilt/Rll/Pan Ch
    ui->cmbpanchn->setCurrentIndex(panCh==-1?0:panCh);
    ui->cmbrllchn->setCurrentIndex(rllCh==-1?0:rllCh);
    ui->cmbtiltchn->setCurrentIndex(tltCh==-1?0:tltCh);
    // Analog CH
    ui->cmbA5Ch->setCurrentIndex(a5Ch==-1?0:a5Ch);
    ui->cmbA6Ch->setCurrentIndex(a6Ch==-1?0:a6Ch);
    ui->cmbA7Ch->setCurrentIndex(a7Ch==-1?0:a7Ch);
    // Aux Funcs
    ui->cmbAuxFn0Ch->setCurrentIndex(auxF0Ch==-1?0:auxF0Ch);
    ui->cmbAuxFn1Ch->setCurrentIndex(auxF1Ch==-1?0:auxF1Ch);
    ui->cmbAuxFn0->setCurrentIndex(trkset.auxFunc0());
    ui->cmbAuxFn1->setCurrentIndex(trkset.auxFunc1());
    // PWM Chs
    ui->cmbPWM0->setCurrentIndex(pwm0Ch==-1?0:pwm0Ch);
    ui->cmbPWM1->setCurrentIndex(pwm1Ch==-1?0:pwm1Ch);
    ui->cmbPWM2->setCurrentIndex(pwm2Ch==-1?0:pwm2Ch);
    ui->cmbPWM3->setCurrentIndex(pwm3Ch==-1?0:pwm3Ch);

    ui->cmbRemap->setCurrentIndex(ui->cmbRemap->findData(trkset.axisRemap()));
    ui->cmbSigns->setCurrentIndex(trkset.axisSign());
    ui->cmbBtMode->setCurrentIndex(trkset.blueToothMode());
    int rot[3];
    trkset.orientation(rot[0],rot[1],rot[2]);
    ui->spnRotX->setValue(rot[0]);
    ui->spnRotY->setValue(rot[1]);
    ui->spnRotZ->setValue(rot[2]);

    ui->til_gain->setValue(trkset.Tlt_gain()*10);
    ui->pan_gain->setValue(trkset.Pan_gain()*10);
    ui->rll_gain->setValue(trkset.Rll_gain()*10);

    int ppout_index = trkset.ppmOutPin()-1;
    int ppin_index = trkset.ppmInPin()-1;
    int but_index = trkset.buttonPin()-1;    
    int resppm_index = trkset.resetCntPPM();
    ui->cmbPpmOutPin->setCurrentIndex(ppout_index < 1 ? 0 : ppout_index);
    ui->cmbPpmInPin->setCurrentIndex(ppin_index < 1 ? 0 : ppin_index);
    ui->cmbButtonPin->setCurrentIndex(but_index < 1 ? 0 : but_index);
    ui->cmbResetOnPPM->setCurrentIndex(resppm_index < 0 ? 0: resppm_index);

    // PPM Output Settings
    int channels = trkset.ppmChCount();
    ui->cmbPPMChCount->setCurrentIndex(channels-4);
    uint16_t setframelen = trkset.ppmFrame();
    ui->spnPPMFrameLen->setValue(static_cast<double>(setframelen)/1000.0f);
    ui->spnPPMSync->setValue(trkset.ppmSync());
    uint32_t maxframelen = TrackerSettings::PPM_MIN_FRAMESYNC + (channels * TrackerSettings::MAX_PWM);
    if(maxframelen > setframelen) {
        ui->lblPPMOut->setText("<b>Warning!</b> PPM Frame length possibly too short to support channel data");
    } else {
        ui->lblPPMOut->setText("PPM data will fit in frame. Refresh rate: " + QString::number(1/(static_cast<float>(setframelen)/1000000.0),'f',2) + " Hz");
    }

    // BT Pair Address
    if(trkset.pairedBTAddress().isEmpty()) {
        ui->cmbBTRmtMode->setCurrentIndex(0);
    } else {
        ui->cmbBTRmtMode->setCurrentText(trkset.pairedBTAddress());
    }

    if(ui->cmbBtMode->currentIndex() > 1) // Remote or Scanner Mode
    {
        ui->lblPairWith->setVisible(true);
        ui->cmbBTRmtMode->setVisible(true);
    } else {
        ui->lblPairWith->setVisible(false);
        ui->cmbBTRmtMode->setVisible(false);
    }

    ui->cmbpanchn->blockSignals(false);
    ui->cmbrllchn->blockSignals(false);
    ui->cmbtiltchn->blockSignals(false);
    ui->cmbRemap->blockSignals(false);
    ui->cmbPpmOutPin->blockSignals(false);
    ui->spnPPMFrameLen->blockSignals(false);
    ui->cmbPPMChCount->blockSignals(false);
    ui->spnPPMSync->blockSignals(false);
    ui->cmbPpmInPin->blockSignals(false);
    ui->cmbButtonPin->blockSignals(false);
    ui->cmbBtMode->blockSignals(false);    
    ui->cmbBTRmtMode->blockSignals(false);
    ui->cmbResetOnPPM->blockSignals(false);
    ui->spnLPPan->blockSignals(false);
    ui->spnLPTiltRoll->blockSignals(false);
    ui->spnLPPan2->blockSignals(false);
    ui->spnLPTiltRoll2->blockSignals(false);
    ui->til_gain->blockSignals(false);
    ui->rll_gain->blockSignals(false);
    ui->pan_gain->blockSignals(false);
    ui->cmbA5Ch->blockSignals(false);
    ui->spnA5Gain->blockSignals(false);
    ui->spnA5Off->blockSignals(false);
    ui->cmbA6Ch->blockSignals(false);
    ui->spnA6Gain->blockSignals(false);
    ui->spnA6Off->blockSignals(false);
    ui->cmbA7Ch->blockSignals(false);
    ui->spnA7Gain->blockSignals(false);
    ui->spnA7Off->blockSignals(false);
    ui->cmbAuxFn0->blockSignals(false);
    ui->cmbAuxFn1->blockSignals(false);
    ui->cmbAuxFn0Ch->blockSignals(false);
    ui->cmbAuxFn1Ch->blockSignals(false);
    ui->cmbPWM0->blockSignals(false);
    ui->cmbPWM1->blockSignals(false);
    ui->cmbPWM2->blockSignals(false);
    ui->cmbPWM3->blockSignals(false);
    ui->spnRotX->blockSignals(false);
    ui->spnRotY->blockSignals(false);
    ui->spnRotZ->blockSignals(false);


}

// Update the Settings Class from the UI Data
void MainWindow::updateFromUI()
{
    if(!serialcon->isOpen()) // Don't update anything if not connected
        return;

    trkset.setPan_cnt(ui->servoPan->centerValue());
    trkset.setPan_min(ui->servoPan->minimumValue());
    trkset.setPan_max(ui->servoPan->maximumValue());
    trkset.setPan_gain(static_cast<float>(ui->pan_gain->value())/10.0f);

    trkset.setTlt_cnt(ui->servoTilt->centerValue());
    trkset.setTlt_min(ui->servoTilt->minimumValue());
    trkset.setTlt_max(ui->servoTilt->maximumValue());
    trkset.setTlt_gain(static_cast<float>(ui->til_gain->value())/10.0f);

    trkset.setRll_cnt(ui->servoRoll->centerValue());
    trkset.setRll_min(ui->servoRoll->minimumValue());
    trkset.setRll_max(ui->servoRoll->maximumValue());
    trkset.setRll_gain(static_cast<float>(ui->rll_gain->value())/10.0f);

    if(trkset.hardware() == "NANO33BLE") {
        trkset.setLPTiltRoll(ui->spnLPTiltRoll->value());
        trkset.setLPPan(ui->spnLPPan->value());
    } else if (trkset.hardware() == "BNO055") {
        trkset.setLPTiltRoll(ui->spnLPTiltRoll2->value());
        trkset.setLPPan(ui->spnLPPan2->value());
    }

    trkset.setLPTiltRoll(ui->spnLPTiltRoll->value());
    trkset.setLPPan(ui->spnLPPan->value());

    trkset.setRollReversed(ui->chkrllrev->isChecked());
    trkset.setPanReversed(ui->chkpanrev->isChecked());
    trkset.setTiltReversed(ui->chktltrev->isChecked());

    trkset.setAnalog5Gain(ui->spnA5Gain->value());
    trkset.setAnalog5Offset(ui->spnA5Off->value());

    trkset.setAnalog6Gain(ui->spnA6Gain->value());    
    trkset.setAnalog6Offset(ui->spnA6Off->value());

    trkset.setAnalog7Gain(ui->spnA7Gain->value());
    trkset.setAnalog7Offset(ui->spnA7Off->value());
    int an5Ch = ui->cmbA5Ch->currentIndex();
    int an6Ch = ui->cmbA6Ch->currentIndex();
    int an7Ch = ui->cmbA7Ch->currentIndex();
    trkset.setAnalog5Ch(an5Ch==0?-1:an5Ch);
    trkset.setAnalog6Ch(an6Ch==0?-1:an6Ch);
    trkset.setAnalog7Ch(an7Ch==0?-1:an7Ch);

    int auxF0Ch = ui->cmbAuxFn0Ch->currentIndex();
    int auxF1Ch = ui->cmbAuxFn1Ch->currentIndex();
    trkset.setAuxFunc0Ch(auxF0Ch==0?-1:auxF0Ch);
    trkset.setAuxFunc1Ch(auxF1Ch==0?-1:auxF1Ch);
    trkset.setAuxFunc0(ui->cmbAuxFn0->currentIndex());
    trkset.setAuxFunc1(ui->cmbAuxFn1->currentIndex());

    int panCh = ui->cmbpanchn->currentIndex();
    int rllCh = ui->cmbrllchn->currentIndex();
    int tltCh = ui->cmbtiltchn->currentIndex();
    trkset.setPanCh(panCh==0?-1:panCh);
    trkset.setRollCh(rllCh==0?-1:rllCh);
    trkset.setTiltCh(tltCh==0?-1:tltCh);

    int pwmCh0 = ui->cmbPWM0->currentIndex();
    int pwmCh1 = ui->cmbPWM1->currentIndex();
    int pwmCh2 = ui->cmbPWM2->currentIndex();
    int pwmCh3 = ui->cmbPWM3->currentIndex();
    trkset.setPWMCh(0,pwmCh0==0?-1:pwmCh0);
    trkset.setPWMCh(1,pwmCh1==0?-1:pwmCh1);
    trkset.setPWMCh(2,pwmCh2==0?-1:pwmCh2);
    trkset.setPWMCh(3,pwmCh3==0?-1:pwmCh3);

    trkset.setAxisRemap(ui->cmbRemap->currentData().toUInt());
    trkset.setAxisSign(ui->cmbSigns->currentIndex());

    // Shift the index of the disabled choice to -1 in settings
    int ppout_index = ui->cmbPpmOutPin->currentIndex()+1;
    ppout_index = ppout_index==1?-1:ppout_index;
    int ppin_index = ui->cmbPpmInPin->currentIndex()+1;
    ppin_index = ppin_index==1?-1:ppin_index;
    int but_index = ui->cmbButtonPin->currentIndex()+1;
    but_index = but_index==1?-1:but_index;

    // Check for pin duplicates
    if((but_index   > 0 && (but_index == ppin_index || but_index == ppout_index)) ||
       (ppin_index > 0 && (ppin_index == but_index || ppin_index == ppout_index)) ||
       (ppout_index > 0 && (ppout_index == but_index || ppout_index == ppin_index))) {
        QMessageBox::information(this,"Error", "Cannot pick dulplicate pins");

        // Reset gui to old values
        ppout_index = trkset.ppmOutPin()-1;
        ppin_index = trkset.ppmInPin()-1;
        but_index = trkset.buttonPin()-1;
        ui->cmbPpmOutPin->setCurrentIndex(ppout_index < 1 ? 0 : ppout_index);
        ui->cmbPpmInPin->setCurrentIndex(ppin_index < 1 ? 0 : ppin_index);
        ui->cmbButtonPin->setCurrentIndex(but_index < 1 ? 0 : but_index);
    } else {
        trkset.setPpmOutPin(ppout_index);
        trkset.setPpmInPin(ppin_index);
        trkset.setButtonPin(but_index);
    }

    uint16_t setframelen = ui->spnPPMFrameLen->value() * 1000;
    trkset.setPPMFrame(setframelen);
    trkset.setPPMSync(ui->spnPPMSync->value());
    int channels = ui->cmbPPMChCount->currentIndex()+4;
    trkset.setPpmChCount(channels);
    uint32_t maxframelen = TrackerSettings::PPM_MIN_FRAMESYNC + (channels * TrackerSettings::MAX_PWM);
    if(maxframelen > setframelen) {
        ui->lblPPMOut->setText("<b>Warning!</b> PPM Frame length possibly too short to support channel data");
    } else {
        ui->lblPPMOut->setText("PPM data will fit in frame. Refresh rate: " + QString::number(1/(static_cast<float>(setframelen)/1000000.0),'f',2) + " Hz");
    }

    int rstppm_index = ui->cmbResetOnPPM->currentIndex();
    trkset.setResetCntPPM(rstppm_index==0?-1:rstppm_index);

    trkset.setBlueToothMode(ui->cmbBtMode->currentIndex());

    if(ui->cmbBTRmtMode->currentIndex() == 0) {
        trkset.setPairedBTAddress();
    } else {
        trkset.setPairedBTAddress(ui->cmbBTRmtMode->currentText().simplified());
    }

    trkset.setOrientation(ui->spnRotX->value(),
                          ui->spnRotY->value(),
                          ui->spnRotZ->value());

    trkset.setInvertedPpmOut(ui->chkInvertedPPM->isChecked());
    trkset.setInvertedPpmIn(ui->chkInvertedPPMIn->isChecked());
    trkset.setResetOnWave(ui->chkResetCenterWave->isChecked());
    trkset.setInvertedSBUSIn(ui->chkSbusInInv->isChecked());
    trkset.setInvertedSBUSOut(ui->chkSbusOutInv->isChecked());

    ui->cmdStore->setEnabled(true);
    ui->cmdSaveNVM->setEnabled(true);    



    // Use timer to prevent too many writes while drags, etc.. happen
    saveToRAMTimer.start(500);
}

// Data ready to be read from the serial port
void MainWindow::serialReadReady()
{
    // Receive LED
    ui->rxled->setState(true);
    rxledtimer.start();

    // Read all serial data
    bool scroll = false;
    QByteArray sd = serialcon->readAll();
    serialData.append(sd);

    int slider = serialDebug->verticalScrollBar()->value();
    if(slider == serialDebug->verticalScrollBar()->maximum())
        scroll = true; 

    // Scroll to bottom
    if(scroll)
        serialDebug->verticalScrollBar()->setValue(serialDebug->verticalScrollBar()->maximum());
    else
        serialDebug->verticalScrollBar()->setValue(slider);

    bool done = true;
    while(done) {
        int nlindex = serialData.indexOf("\r\n");
        if(nlindex < 0)
            return;  // No New line found

        // Strip data up the the CR LF \r\n
        QByteArray data = serialData.left(nlindex);
        qDebug() << "Serial In:" << data;

        // Return if only a \r\n, no data.
        if(data.length() < 1) {
            serialData = serialData.right(serialData.length()-nlindex-2);
            return;
        }

        // Send complete lines to
        foreach(BoardType *brd, boards) {
            brd->_dataIn(data);
        }

        serialData = serialData.right(serialData.length()-nlindex-2);
    }
}

void MainWindow::manualSend()
{
    sendSerialData(ui->txtCommand->text().toLatin1());
}

/* offOrientChanged()
 *      New data available for the graph
 */

void MainWindow::offOrientChanged(float t,float r,float p)
{
    ui->graphView->addDataPoints(t,r,p);
}

void MainWindow::ppmOutChanged(int t,int r,int p)
{
    ui->servoTilt->setActualPosition(t);
    ui->servoRoll->setActualPosition(r);
    ui->servoPan->setActualPosition(p);

    // Add a timer here so if no updates these disable
    ui->servoPan->setShowActualPosition(true);
    ui->servoTilt->setShowActualPosition(true);
    ui->servoRoll->setShowActualPosition(true);
}

void MainWindow::bleAddressDiscovered(QString str)
{
    // Add BLE address to discovered list
    static QStringList bleaddrs;
    if(bleaddrs.contains(str))
        return;
    bleaddrs.append(str);

    // Add all ble address
    ui->cmbBTRmtMode->addItem(str);
}

void MainWindow::liveDataChanged()
{
    ui->lblBLEAddress->setText(trkset.blueToothAddress());
    ui->btLed->setState(trkset.blueToothConnected());
    if(trkset.blueToothConnected())
        ui->lblBTConnected->setText("Connected");
    else
        ui->lblBTConnected->setText("Not connected");
    if(trkset.blueToothMode() == TrackerSettings::BTDISABLE)
        ui->lblBTConnected->setText("Disabled");

}


void MainWindow::rxledtimeout()
{
    ui->rxled->setState(false);
}

void MainWindow::txledtimeout()
{
    ui->txled->setState(false);
}

void MainWindow::saveSettings()
{
    QString filename = QFileDialog::getSaveFileName(this,"Save Settings",QString(), "Config Files (*.ini)");
    if(!filename.isEmpty()) {
        QSettings settings(filename,QSettings::IniFormat);
        trkset.storeSettings(&settings);
    }

    // Re-enable data if window was open too long
    startData();
}

void MainWindow::loadSettings()
{
    if(!serialcon->isOpen()) {
        QMessageBox::information(this, "Info","Please connect before restoring a saved file");
        return;
    }

    QString filename = QFileDialog::getOpenFileName(this,tr("Open File"),QString(),"Config Files (*.ini)");

    // Re-enable data if window was open too long
    startData();

    if(!filename.isEmpty()) {
        QSettings settings(filename,QSettings::IniFormat);
        trkset.loadSettings(&settings);
        updateToUI();
        storeToRAM();
    }
}

bool MainWindow::checkSaved()
{
    foreach(BoardType *brd, boards) {
        if(!brd->isAccessAllowed())
            continue;
        if(!brd->_isBoardSavedToRAM()) {
            QMessageBox::StandardButton rval = QMessageBox::question(this,"Changes not sent","Are you sure you want to disconnect?\n"\
                                  "Changes haven't been sent to the headtracker\nClick \"Send Changes\" first",QMessageBox::Yes|QMessageBox::No);
            if(rval != QMessageBox::Yes)
                return false;

        } else if (!brd->_isBoardSavedToNVM()) {
            QMessageBox::StandardButton rval = QMessageBox::question(this,"Changes not saved on tracker","Are you sure you want to disconnect?\n"\
                                  "Changes haven't been permanently stored on headtracker\nClick \"Save to NVM\" (Non-Volatile Memory) first",QMessageBox::Yes|QMessageBox::No);
            if(rval != QMessageBox::Yes)
                return false;
        }
    }
    return true;
}

void MainWindow::uploadFirmwareWizard()
{
    if(firmwareWizard == nullptr)
        firmwareWizard = new FirmwareWizard;
    firmwareWizard->show();
    firmwareWizard->activateWindow();
    firmwareWizard->raise();
}

/* requestTimeout()
 *      This timeout is called after waiting for the board to boot
 * it requests the hardware and version from the board, gives each board
 * 250ms to respond before trying the next one. Sets the allowAccess so
 * other boards won't respond on the serial line at the same time.
 */

void MainWindow::requestTimeout()
{
    // Was a board discovered, if so just quit
    if(currentboard != nullptr)
        return;

    // Otherwise increment to the next board and try again
    if(boardRequestIndex == boards.length()) {
        msgbox->setText("Was unable to determine the board type");
        msgbox->setWindowTitle("Error");
        msgbox->show();
        statusMessage("Board discovery failed");
        serialDisconnect();
        return;
    }

    // Prevent last board class from interfering
    if(boardRequestIndex > 0)
        boards[boardRequestIndex-1]->allowAccess(false);

    // Request hardware information from the new board
    addToLog("Trying to connect to " + boards[boardRequestIndex]->boardName() + "\n");
    boards[boardRequestIndex]->allowAccess(true);
    boards[boardRequestIndex]->requestHardware();
    requestTimer.start(200);

    // Move to next board
    boardRequestIndex++;
}

void MainWindow::saveToRAMTimeout()
{
    // Request hardware from all board types
    foreach(BoardType *brd, boards) {
        brd->_saveToRAM();
        ui->cmdStore->setEnabled(false);
    }
}

void MainWindow::requestParamsTimeout()
{
    waitingOnParameters=true;

    // Request hardware from all board types
    foreach(BoardType *brd, boards) {
        brd->_requestParameters();
    }
}

// Start the various calibration dialogs
void MainWindow::startCalibration()
{
    if(!serialcon->isOpen()) {
        QMessageBox::information(this,"Not Connected", "Connect before trying to calibrate");
        return;
    }

    foreach(BoardType *brd, boards) {
        brd->_startCalibration();
    }
}

// Tell the board to start sending data again
void MainWindow::startData()
{
    foreach(BoardType *brd, boards) {
        brd->_startData();
    }
}

void MainWindow::storeToNVM()
{
    foreach(BoardType *brd, boards) {
        brd->_saveToNVM();
    }
    statusMessage("Storing Parameters to non-volatile memory");
    ui->cmdSaveNVM->setEnabled(false);
}

void MainWindow::storeToRAM()
{
    foreach(BoardType *brd, boards) {
        brd->_saveToRAM();
    }
}

void MainWindow::resetCenter()
{
    foreach(BoardType *brd, boards) {
        brd->_resetCenter();
    }
}

void MainWindow::showDiagsClicked()
{
    diagnostic->show();
    diagnostic->activateWindow();
    diagnostic->raise();
}

void MainWindow::showSerialDiagClicked()
{
    serialDebug->show();
    serialDebug->activateWindow();
    serialDebug->raise();
}

void MainWindow::showChannelViewerClicked()
{
    channelviewer->show();
    channelviewer->activateWindow();
    channelviewer->raise();
}

void MainWindow::BLE33tabChanged()
{
    if(currentboard == nullptr)
        return;

    QMap<QString,bool> dataitms;
    dataitms["tiltoff"] = true;
    dataitms["rolloff"] = true;
    dataitms["panoff"] = true;
    dataitms["tiltout"] = true;
    dataitms["rollout"] = true;
    dataitms["panout"] = true;
    dataitms["btcon"] = false;
    dataitms["btaddr"] = false;
    dataitms["btrmt"] = false;    

    switch(ui->tabBLE->currentIndex()) {
    case 0: { // General
        break;
    }
    case 1: { // Output
        break;
    }
    case 2: { // PPM In
        break;
    }
    case 3: { // Bluetooth
        dataitms["btcon"] = true;
        dataitms["btrmt"] = true;
        dataitms["btaddr"] = true;
        break;
    }
    case 4: { // PWM
        break;
    }
    case 5: { // Extras
        break;
    }
    default:
        break;
    }

    trkset.setDataItemSend(dataitms);
}

void MainWindow::BTModeChanged()
{
    updateFromUI();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Reboot Required",
                                  "Bluetooth mode change requires reboot.\n"
                                  "Save and reboot now?",
                                  QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        storeToNVM();
        QTime dieTime= QTime::currentTime().addMSecs(800);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        reboot();
    }
}

void MainWindow::reboot()
{
    foreach(BoardType *brd, boards) {
        bool reboot=true;
        if(!brd->isAccessAllowed())
            continue;
        if(!brd->_isBoardSavedToNVM()) {
            QMessageBox::StandardButton rval = QMessageBox::question(this,"Changes not saved","Are you sure you want to reboot?\n"\
                                  "Changes haven't been saved\nClick \"Save to NVM\" first",QMessageBox::Yes|QMessageBox::No);
            if(rval != QMessageBox::Yes) {
                reboot = false;
            }
        }
        if(reboot) {
            brd->_reboot();
            QTime dieTime= QTime::currentTime().addMSecs(RECONNECT_AFT_REBT);
            while (QTime::currentTime() < dieTime)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

            serialConnect();
        }
    }
}

void MainWindow::paramSendStart()
{
    statusMessage("Starting parameter send",3000);
}

void MainWindow::paramSendComplete()
{
    statusMessage("Parameter(s) saved", 5000);
    ui->cmdStore->setEnabled(false);
}

void MainWindow::paramSendFailure(int)
{
    msgbox->setText("Unable to upload the parameter(s)");
    msgbox->setWindowTitle("Error");
    msgbox->show();
    statusMessage("Parameters Send Failure");
    serialDisconnect();
}

void MainWindow::paramReceiveStart()
{
    statusMessage("Parameters Request Started",5000);
}

void MainWindow::paramReceiveComplete()
{
    statusMessage("Parameters Request Complete",5000);
    waitingOnParameters = false;
    updateToUI();
    BLE33tabChanged(); // Request Data to be sent
    trkset.setDataItemSend("isCalibrated",true);
}

void MainWindow::paramReceiveFailure(int)
{
    msgbox->setText("Unable to receive the parameters");
    msgbox->setWindowTitle("Error");
    msgbox->show();
    statusMessage("Parameter Received Failure");
    serialDisconnect();
}

void MainWindow::calibrationSuccess()
{
    statusMessage("Calibration Success",5000);
}

void MainWindow::calibrationFailure()
{
    statusMessage("Calibration Failure",5000);
}

void MainWindow::serialTxReady()
{
    foreach(BoardType *brd, boards) {
        sendSerialData(brd->_dataout());
    }
}

void MainWindow::needsCalibration()
{
    msgbox->setText("Calibration has not been performed.\nPlease calibrate or restore from a saved file");
    msgbox->setWindowTitle("Calibrate");
    msgbox->show();
}

void MainWindow::boardDiscovered(BoardType *brd)
{
    // Board discovered, save it
    currentboard = brd;

    // Stack widget changes to hide some info depending on board
    if(brd->boardName() == "NANO33BLE") {
        addToLog("Connected to a " + brd->boardName() + "\n");
        ui->cmdStartGraph->setVisible(false);
        ui->cmdStopGraph->setVisible(false);
        ui->chkRawData->setVisible(false);
        ui->cmbRemap->setVisible(false);
        ui->cmbSigns->setVisible(false);
        ui->cmdSaveNVM->setVisible(true);
        ui->grbSettings->setTitle("Nano 33 BLE");
        ui->cmdStopGraph->setEnabled(true);
        ui->cmdStartGraph->setEnabled(true);
        ui->cmdSend->setEnabled(true);
        ui->cmdSaveNVM->setEnabled(true);
        ui->cmdCalibrate->setEnabled(true);
        ui->cmdReboot->setEnabled(true);
        ui->stackedWidget->setCurrentIndex(3);
        ui->cmdChannelViewer->setEnabled(true);

        // Check Firmware Version is Compatible

        // GUI Version
        float lfver = version.toFloat();
        int lmajver = roundf(lfver*10);    // Major Version 1.1x == 11

        // Remote Version
        float rfver = trkset.fwVersion().toFloat();
        int rmajver = roundf(rfver*10);    // Major Version 1.1x == 11

        // Firmware is too old
        if(lmajver > rmajver) {
            msgbox->setText("Firmware is outdated. Upload a firmware of " + QString::number((float)lmajver/10,'f',1) + "x for this GUI");
            msgbox->setWindowTitle("Firmware Version Mismatch");
            msgbox->show();

        // Firmware is too new
        } else if (lmajver < rmajver) {
            msgbox->setText("Firmware is newer than supported by this GUI. Download " + QString::number((float)rmajver/10,'f',1) +"x \n\nfrom www.github.com/dlktdr/headtracker");
            msgbox->setWindowTitle("Firmware Version Mismatch");
            msgbox->show();
        }
        channelviewer->setBoard(currentboard);
    } else if (brd->boardName() == "BNO055") {
        addToLog("Connected to a " + brd->boardName() + "\n");
        ui->cmdStartGraph->setVisible(true);
        ui->cmdStopGraph->setVisible(true);
        ui->cmbRemap->setVisible(true);
        ui->cmbSigns->setVisible(true);
        ui->chkRawData->setVisible(true);
        ui->cmdSaveNVM->setVisible(false);
        ui->grbSettings->setTitle("BNO055");
        ui->cmdStopGraph->setEnabled(true);
        ui->cmdStartGraph->setEnabled(true);
        ui->cmdSend->setEnabled(true);
        ui->cmdSaveNVM->setEnabled(true);
        ui->cmdCalibrate->setEnabled(true);
        ui->stackedWidget->setCurrentIndex(2);

    } else if (brd->boardName() == "NANO33REMOTE") {
        addToLog("Connected to a " + brd->boardName() + "\n");

    } else {
        msgbox->setText("Unknown board type");
        msgbox->setWindowTitle("Error");
        msgbox->show();
        statusMessage("Unknown board type");
        serialDisconnect();
    }

    requestParamsTimer.stop();
    requestParamsTimer.start(50);
}

void MainWindow::statusMessage(QString str, int timeout)
{
    ui->statusbar->showMessage(str,timeout);
}

void MainWindow::closeEvent(QCloseEvent *event)
{    
    bool close=checkSaved();

    if(close) {
        QCoreApplication::quit();
        event->accept();
    } else
        event->ignore();
}

