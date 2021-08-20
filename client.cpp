/*
*Main class of the program
*/
#include "client.h"
#include <QtCore/QDebug>
#include <QtWebSockets/QWebSocket>
#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <termcolor.hpp>
#include <ctime>
#include <random>
#include <QDebug>


QT_USE_NAMESPACE

int Weather::weatherColourIndex = 0;

//! [constructor]
Client::Client(const QUrl &url_, QObject *parent) :
    QObject(parent),
    url(url_)
{
    m_webSocket = new QWebSocket;
    connect(m_webSocket, &QWebSocket::connected, this, &Client::onConnected);
    connect(m_webSocket, QOverload<const QList<QSslError>&>::of(&QWebSocket::sslErrors),
            this, &Client::onSslErrors);

    connect(m_webSocket, &QWebSocket::textMessageReceived,
            this, &Client::onTextMessageReceived);

    connect(m_webSocket, &QWebSocket::disconnected, this, &Client::reconnect);

    loadTalkLogs(loadlogs);//load txt with talk logs
    m_webSocket->open(QUrl(url));

    msgRecElements = new MsgRecElements;
    msgSendElements = new MsgSendElements;
    ping = "{\"numbers\":[1],\"strings\":[]}";

//console input:
    consoleThread = new QThread;
    consoleInput = new ConsoleInput(nullptr, consoleThread, m_webSocket, msgRecElements, msgSendElements);
    consoleInput->moveToThread(consoleThread);
    connect(consoleThread, &QThread::finished, consoleInput, &QObject::deleteLater);
    connect(consoleInput, &ConsoleInput::postItSignal, this, &Client::sendText);

    consoleThread->start();

    //weather:
    weather = new Weather(this);
    connect(weather, &Weather::postItSignal, this, &Client::sendText);
}


//! [onConnected]
void Client::onConnected()
{
    qDebug() << "WebSocket connected";
    m_webSocket->sendTextMessage("HTTP/1.1 101 Switching Protocols");
    m_webSocket->sendTextMessage("Host: s1.polfan.pl:16080");
    m_webSocket->sendTextMessage("Upgrade: websocket");
    m_webSocket->sendTextMessage("Connection: Upgrade");
    m_webSocket->sendTextMessage("Sec - WebSocket - Key: Gc8ZddRMJbwI8j84moViWw==");
    m_webSocket->sendTextMessage("Origin: https://polfan.pl");

    QString s;
    QFile file("D:/Qt_workspace/WebSocketsClient/1.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qWarning() << "file not opened!";
    m_webSocket->sendTextMessage(s.fromLocal8Bit(file.readAll()));


    //    QString x = QJsonDocument(makeJSONconnect()).toJson(QJsonDocument::Compact).toStdString().c_str();
    //    m_webSocket->sendTextMessage(x);
    qDebug() <<  QThread::currentThreadId();
}
//! [onConnected]
//{"numbers": [1400], "strings":["ProtoBrunetka","","","grunge","http://polfan.pl?cg=p","nlst=1&nnum=1&jlmsg=true&ignprv=false","","StaryAplet 1.6.7 (Firefox.190.0 Windows 12 [polfan/t:])"]}
//{"numbers":[410],"strings":["<#000000>lalala", "grunge"]}




QJsonObject Client::makeJSONconnect()
{
    QJsonObject jo;
    QJsonArray arr1;
    arr1.append(1400);
    jo.insert("numbers",arr1);
    QString i  = "\"ProtoBrunetka\","","",\"grunge\",\"http://polfan.pl?cg=p\",\"nlst=1&nnum=1&jlmsg=true&ignprv=false\","",\"StaryAplet 1.6.7 (Firefox.190.0 Windows 12 [polfan/t:])\"";
    QJsonArray arr2;
    arr2.append(i);
    jo.insert("strings",arr2);
    return jo;
}


//{"numbers":[410],"strings":["<#000000>lalala", "grunge"]}
QJsonObject Client::makeJSONsend(std::string nick, std::string colour, std::string message, std::string room)
{
    std::cout << termcolor::red << message  << termcolor::reset<< std::endl;

    QJsonObject jo;
    QJsonArray arr1;
    arr1.append(410);

    jo.insert("numbers",arr1);

    std::string i  = "<#" + colour  + ">" + message;

    QJsonArray arr2;
    arr2.append(QString::fromStdString(i));
    arr2.append(QString::fromStdString(room));
    jo.insert("strings",arr2);

    qDebug() << jo;
    return jo;
}


// MAIN USAGE METHOD **************
void Client::onTextMessageReceived(QString message)
{
    qDebug() << "Message received:" << message;
    checkPing(message);

    try {
        msgRecElements = Parser::parseReceived(message, msgRecElements); //feeding parser with received QString from the chat (originally in Json) and struct MsgRecElements so it can be filled with information contained in QString, and we ll be able to use it in Client and other classes
    } catch (...){
        std::cout << termcolor::red << "currentRecMsgStruct = Parser::parseReceived(message)"  << termcolor::reset<< std::endl;
        return;
    }

    if(msgRecElements->is_not_valid_message){
        std::cout << "WARNING: is_not_valid_messag" << "\n";
//        return;
    }

    std::cout << termcolor::blue << "currentRecMsgStruct.msg: " << msgRecElements->msg  << termcolor::reset<< std::endl;

    checkWeather(msgRecElements, msgSendElements);

    //std::cout << termcolor::red << message.toStdString() << termcolor::reset<< std::endl;
}



void Client::sendText(MsgSendElements* msgSendElements) // struct argument definied in consoleInput class
{
    std::cout << termcolor::green <<  msgSendElements->message << termcolor::reset<< std::endl;

    QJsonObject jo = makeJSONsend( msgSendElements->nick,
                                   msgSendElements->colour,
                                   msgSendElements->message,
                                   msgSendElements->room);

    QJsonDocument doc(jo);
    //QByteArray docByteArray = doc.toJson(QJsonDocument::Compact);

    qDebug()<< "doc: " << doc;

    //std::cout << termcolor::on_red << "send it: " << docByteArray.toStdString() << termcolor::reset<< std::endl;
    m_webSocket->sendTextMessage(QString::fromUtf8(doc.toJson(QJsonDocument::Compact))); //cplours turn off sending
    //    m_webSocket->sendTextMessage("{\"numbers\":[410],\"strings\":[\"<#000000>ążąś\", \"grunge\"]}");
}

void Client::checkPing(QString message)
{

    /// "{\"numbers\":[1],\"strings\":[]}"

    if(message == ping){
        std::cout << termcolor::blue << "PING" << termcolor::reset<< std::endl;
        m_webSocket->sendTextMessage("{\"numbers\":[2],\"strings\":[]}");

        QString temp = "Pong Sent";
    }
}

void Client::checkWeather(MsgRecElements *msgRecElements, MsgSendElements *msgSendElements)
{
    weather->checkKeyword(msgRecElements, msgSendElements);

}

void Client::loadTalkLogs(LoadLogs &loadLogs)
{
    v_talkdatabase = &loadLogs.loadTalkLogs();
    std::cout << "v_talkdatabase address: " << &*v_talkdatabase << "\n";
}





void Client::onSslErrors(const QList<QSslError> &errors)
{
    Q_UNUSED(errors);

    // WARNING: Never ignore SSL errors in production code.
    // The proper way to handle self-signed certificates is to add a custom root
    // to the CA store.

    m_webSocket->ignoreSslErrors();
}

void Client::reconnect()
{
    m_webSocket->open(url);
    onConnected();
}

