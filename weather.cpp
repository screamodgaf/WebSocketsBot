#include "weather.h"
#include <QThread>
#include "client.h"
#include <iostream>
#include <ctime>
Weather::Weather(QObject *parent) : QObject(parent)
{
    srand(std::time(nullptr));
    v_colours = {
        "c54e09",
        "41c916",
        "16c1c9",
        "164ec5",
        "f61212",
        "b216b2",
        "13e526",
        "528aac",
        "af1fd3",
        "c54e09"
    };

}

Weather::~Weather()
{
    std::cout << "WEATHER DESTRUCTOR" << "\n";
}

void Weather::checkKeyword(MsgRecElements *msgRecElements_, MsgSendElements *msgSendElements_)
{
    std::cout << "Weather::checkKeyword" << std::endl;
    msgRecElements = msgRecElements_;
    msgSendElements = msgSendElements_;

    size_t indexBegin; //6


    if((indexBegin = msgRecElements->msg.find("!pogoda"))!=std::string::npos)
    {
        std::cout << "msgRecElements->msg: "  << msgRecElements->msg<< std::endl;
        std::string core = msgRecElements->msg.substr(indexBegin+8,(msgRecElements->msg.size() -(indexBegin+8)));
        std::cout << "core: "  << core<< std::endl;
        if(core.size()<=1)
            return;
        process(QString::fromStdString(core));

    }
    else
    {
        return;
    }
}

void Weather::process(QString core)
{
    QString cmdCommand = "D:\\PycharmProjects\\untitled\\untitled2\\Scripts\\python.exe";
    QStringList list;
    list << "D:/Qt_workspace/WebSocketsClient/weather.py" << core;

    QProcess *qprocess= new QProcess(this);
    qprocess->setProgram(cmdCommand);
    qprocess->setArguments(list);
    qprocess->start();
    qprocess->waitForFinished(3000);



    while(qprocess->canReadLine())
    {
        QByteArray tempByteArr = qprocess->readLine();
        std::string temp = tempByteArr.toStdString();

        std::string nick = "ProtoBrunetka";
        std::string room = "grunge";
//        std::string colour = "FF0000";

        msgSendElements->message = tempByteArr.toStdString();
        if(msgSendElements->message.find("\r\n")!=std::string::npos)
                msgSendElements->message.resize(msgSendElements->message.size()-2);
        msgSendElements->room = room;
        msgSendElements->colour = v_colours[weatherColourIndex++];
        msgSendElements->nick = nick;

        emit postItSignal(msgSendElements);
//        QThread::msleep(450);
    }

    qprocess->close();

    qprocess = nullptr;
    weatherColourIndex =0;
    msgSendElements->colour = "FF0000";
    delete qprocess;
}

