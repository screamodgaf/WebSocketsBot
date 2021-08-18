#ifndef PARSER_H
#define PARSER_H
#include <QObject>
#include <iostream>

struct MsgRecElements;

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(QObject *parent = nullptr);
    static MsgRecElements *parseReceived(QString s, MsgRecElements* msgRecElements);
private:
    static MsgRecElements* msgRecElements;
signals:

};

#endif // PARSER_H
