#ifndef CONSOLEINPUT_H
#define CONSOLEINPUT_H
#include <QThread>
#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <vector>

//struct MsgSendElements
//{
//    std::string nick;
//    std::string colour;
//    std::string message;
//    std::string room;
//};

struct MsgSendElements; //forward declaration
struct MsgRecElements;


class ConsoleInput : public QObject
{
    Q_OBJECT
public:
    explicit ConsoleInput(QObject *parent = nullptr, QThread *thread_ = nullptr, QWebSocket *m_webSocket_ = nullptr, MsgRecElements *msgRecElements_ = nullptr, MsgSendElements* msgSendElements_ = nullptr);


//    void determineOption(std::string input);
//    void runFromIRC(std::string &data);
private:
     void postInput(std::string &input);

public slots:
    void run();

private:
    QThread* thread;
    QWebSocket* m_webSocket;
    MsgSendElements* msgSendElements;
    MsgRecElements* msgRecElements;
    std::string nick = "ProtoBrunetka";
    std::string room = "grunge";
    std::string colour = "000000";


signals:
    void postItSignal(MsgSendElements* msgSendElements);
//    void mySignal(const QByteArray);
    //void mySignalIRC(const QByteArray);
//    void saveSignal();
//    void sayNow();




};

#endif // CONSOLEINPUT_H
