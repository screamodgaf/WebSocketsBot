#ifndef WEATHER_H
#define WEATHER_H
#include <QDebug>
#include <QObject>
#include <iostream>
#include <QProcess>
#include <parser.h>

//struct MsgSendElements
//{
//    std::string nick;
//    std::string colour;
//    std::string message;
//    std::string room;
//};
struct MsgRecElements;
struct MsgSendElements;

class Weather: public QObject
{
    Q_OBJECT
public:
    explicit Weather(QObject *parent = nullptr);
    ~Weather( );
    void checkKeyword(MsgRecElements *msgRecElements_, MsgSendElements *msgSendElements_);
    void process(QString indexCore);
    static int weatherColourIndex; //initialized in Client.cpp

private:
    std::string room;
    MsgRecElements *msgRecElements;
    MsgSendElements *msgSendElements;
    std::vector<std::string> v_colours;
signals:
    void postItSignal(MsgSendElements* elements);

public slots:
};

#endif // WEATHER_H


