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
//#include "weather.h"





struct MsgSendElements
{
    std::string nick = "";
    std::string colour = "";
    std::string message = "";
    std::string room = "";
};

struct MsgRecElements
{
    std::string nick = "";
    std::string msg = "";
    std::string room = "";
    bool is_not_valid_message = false;
};



Q_DECLARE_METATYPE(MsgSendElements*);


class Client : public QObject
{
    Q_OBJECT
public:

    explicit Client(const QUrl &url_, QObject *parent = nullptr);
    QJsonObject makeJSONsend(std::string nick, std::string colour, std::string message, std::string room);
    QJsonObject makeJSONconnect();
private:
    void checkPing(QString message);
    void checkWeather(QString message);
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

    ConsoleInput *consoleInput;
    QThread* consoleThread;
//    Weather* weather;
    MsgRecElements* msgRecElements;
    MsgSendElements* msgSendElements;

    QString nick = "ProtoBrunetka";
    std::string colourDef = "000000";
    QString room = "grunge";
};

#endif // CLIENT_H
