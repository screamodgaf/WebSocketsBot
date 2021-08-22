#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QtNetwork/QSslError>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QThread>
#include "consoleinput.h"
#include "parser.h"
#include "weather.h"
#include "loadlogs.h"
#include "bottalk.h"



struct MsgSendElements
{
    std::string nick = "";
    std::string colour = "FF0000";
    std::string message = "";
    std::string room = "";
    std::string numbers = "410";
    std::string myNick = "";
};

struct MsgRecElements
{
    std::string nick = "";
    std::string msg = "";
    std::string room = "";
    std::string numbers;
    bool is_not_valid_message = false;
};



Q_DECLARE_METATYPE(MsgSendElements*); //for using MsgSendElements* as own type in signal-slot system (connect)


class Client : public QObject
{
    Q_OBJECT
public:

    explicit Client(const QUrl &url_, std::string nick_, std::string room_, QObject *parent = nullptr);
    QJsonObject makeJSONsend(std::string nick, std::string colour, std::string message, std::string room);
    QJsonObject makeJSONconnect();
private:
    void checkPing(QString message);
    void checkWeather(MsgRecElements* msgRecElements, MsgSendElements* msgSendElements);
    void checkBotTalk(MsgRecElements* msgRecElements, MsgSendElements* msgSendElements);
    void increaseTalkIndexLines(MsgRecElements* msgRecElements);
private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);
//    void sendText(std::vector<QString> messageToPost);
    void sendText(MsgSendElements* msgSendElements);
//    MsgElements msgElements;
    void onSslErrors(const QList<QSslError> &errors);

    void reconnect();

private:
    QWebSocket* m_webSocket;
    QString ping;
    QUrl url;

    std::vector<std::string>* v_talkdatabase;
    ConsoleInput *consoleInput;
    QThread* consoleThread;
    Weather* weather;
    MsgRecElements* msgRecElements;
    MsgSendElements* msgSendElements;

    int talkIndexLines; // counter of lines spoken on chat
    std::string nick = "";
    std::string room = "";
    std::string colourDef = "000000";

    LoadLogs loadlogs;
    BotTalk* botTalk;
};

#endif // CLIENT_H
