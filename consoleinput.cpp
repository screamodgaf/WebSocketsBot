#include "consoleinput.h"
//#include "removeduplicates.h"
#include <iostream>
#include "client.h"

int idaaa = qRegisterMetaType<MsgSendElements*>();

ConsoleInput::ConsoleInput(QObject *parent, QThread *thread_, QWebSocket *m_webSocket_, MsgRecElements *msgRecElements_, MsgSendElements *msgSendElements_) : QObject(parent)
{
    msgRecElements = msgRecElements_;
    msgSendElements = msgSendElements_;
    std::cout << "ConsoleInput::startConsole" << std::endl;
    m_webSocket = m_webSocket_;
    thread = thread_;

    msgSendElements->nick = nick; // my nick TODO
    msgSendElements->room = room; // as default room of origin where messages go

    QObject::connect(thread, SIGNAL(started()), this, SLOT(run()));
}

void ConsoleInput::startConsole( )
{

}




void ConsoleInput::run()
{
    std::cout << "!ConsoleInput::run()" << "\n";
    qDebug() <<  QThread::currentThreadId();

    while(1){
        std::string input;

//system("chcp 65001");
        std::getline(std::cin, input);
        std::cout << "=========================" << "\n";
        std::cout << input << "\n";


        postInput(input);

        if(input[0] != '/' || input[0] != '\\')
            ///postInput(input);


            input.clear();
    }
}


//QString nick, QString colour, QString message, QString room
void ConsoleInput::postInput(std::string &input)// for commands
{
    std::cout << input << "\n";
    msgSendElements->message = input;
    emit postItSignal(msgSendElements);
}


