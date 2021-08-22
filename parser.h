#ifndef PARSER_H
#define PARSER_H
#include <QObject>
#include <iostream>

struct MsgRecElements;
struct MsgSendElements;
class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(QObject *parent = nullptr);
    static MsgRecElements *parseReceived(QString s, MsgRecElements* msgRecElements);
    static void resetRecElement(MsgRecElements* msgRecElements);
    static void resetSendElement(MsgSendElements* msgSendElements);
private:
    static MsgRecElements* msgRecElements;

signals:

};

#endif // PARSER_H
