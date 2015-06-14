#include "bluetoothwidget.h"
#include "ui_bluetoothwidget.h"

#include <QtSerialPort/QSerialPort>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

//--------------- WIDGET BLUETOOTH --------------//
//Muestra el estado de la conexión en tiempo real//
BluetoothWidget::BluetoothWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BluetoothWidget)
{
    ui->setupUi(this);

    ui->stopButton->setEnabled(false);
    ui->startButton->setEnabled(true);

    //Timer que cuenta los segundos de ejecucion de la ventana
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    counter = 0;

    //Atajo de desactivación de movimiento
    moveMouse = new QShortcut(Qt::Key_Escape, this);
    connect(moveMouse, SIGNAL(activated()), this, SLOT(on_shortcut()));
    //connect(ui->startButton, SIGNAL(clicked()), this, SLOT(openPort()));
    //connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(closePort()));
}


void BluetoothWidget::openPort(){
    ui->startButton->setEnabled(false);
    timer->start(1000);
    emit clickedStart();
}

void BluetoothWidget::closePort(){
    ui->stopButton->setEnabled(false);
    ui->startButton->setEnabled(true);
    emit clickedStop();
}

BluetoothWidget::~BluetoothWidget()
{
    emit widgetClosedSignal();
    delete ui;
}

void BluetoothWidget::openedSerialPort()
{
    ui->stopButton->setEnabled(true);
}

void BluetoothWidget::closedSerialPort()
{
    //Al cerrarse el puerto serie, se genera una archivo con todos los datos recibidos y
    // se indica el numero total de muestras
    ui->stopButton->setEnabled(false);
    ui->startButton->setEnabled(true);
    timer->stop();
    ui->textEdit->clear();
   for (int i = 0; i<finalDataList.size();i++){
          ui->textEdit->append(finalDataList.at(i));
          ui->textEdit->ensureCursorVisible();
    }
    ui->textEdit->insertPlainText("\nNumero de muestras: "+QString::number(finalDataList.size())+". Segundos en funcionamiento: "+QString::number(counter));
    QString filename="DataSample_Diagonal.txt";
    QFile file( filename );
    if ( file.open(QIODevice::WriteOnly|QIODevice::Text) )
    {
        QTextStream stream(&file);
        for (int i = 0; i<finalDataList.size();i++){
              stream << QString::QString(finalDataList.at(i)) << endl;
              //stream << "\n\r";
        }
    }
    ui->textEdit->insertPlainText("\nMuestras guardadas en "+filename);

    counter = 0;
}

/* FUNCIONES DE RECEPCION DE DATOS DEL ESTADO DE LA CONEXION
 * Datos, resultados, mensajes de informacion o de actualizacion del timer.
 * */
void BluetoothWidget::newData(QString data)
{
    ui->textEditData->append(data);
    ui->textEditData->ensureCursorVisible();
    finalDataList.append(data);
    ui->textEdit->clear();
    ui->textEdit->insertPlainText("Numero de muestras: "+QString::number(finalDataList.size())+". Segundos en funcionamiento: "+QString::number(counter));
}

void BluetoothWidget::newError(QString info)
{
        QMessageBox::critical(this, tr("Error"), info);
}

void BluetoothWidget::update(){
    counter++;
    ui->textEdit->clear();
    ui->textEdit->insertPlainText("Numero de muestras: "+QString::number(finalDataList.size())+". Segundos en funcionamiento: "+QString::number(counter));
}

void BluetoothWidget::newResults(QStringList results){
    ui->horizontalLineEdit->setText(results[0]);
    ui->verticalLineEdit->setText(results[1]);
}

void BluetoothWidget::newBlink(bool performed){
    if(performed){
        ui->blinkLineEdit->setText("SI");
    }
    else{
         ui->blinkLineEdit->setText("NO");
    }
}

void BluetoothWidget::on_shortcut(){
    emit shortcutChange();
}



