#include <QtCore/QCoreApplication>
#include <QUrl>
#include "client.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::string nick = "Proto";
    std::string room = "Chatroom";

    Client client(QUrl(QStringLiteral("wss://s1.polfan.pl:16080")), nick, room);

    Q_UNUSED(client);

    return a.exec();
}
