#include "parser.h"
#include "client.h"

Parser::Parser(QObject *parent) : QObject(parent)
{
}

MsgRecElements* Parser::parseReceived(QString s, MsgRecElements *msgRecElements)
{
    qDebug() << "Parser::parseReceived: " << s;
    bool is_correct_rec = true;
    std::string received = s.toStdString();
    size_t pos_numbers_beg = 0;
    size_t pos_numbers_end = 0;
    if((pos_numbers_beg=received.find("numbers\":["))!=std::string::npos){
        if((pos_numbers_end = received.find("]",pos_numbers_beg))!=std::string::npos){
            msgRecElements->numbers= received.substr(pos_numbers_beg+10, pos_numbers_end - (pos_numbers_beg+10)); // the braces - (pos_numbers_beg+10) are crucial
            std::cout << "msgRecElements->numbers: " << msgRecElements->numbers << "\n";

        }else is_correct_rec = false;

    }


    size_t pos_nick_beg = 0;
    size_t pos_nick_end = 0;
    if((pos_nick_beg = received.find("<b>"))!=std::string::npos)
    {
        if((pos_nick_end = received.find("</b>", pos_nick_beg))!=std::string::npos){
            msgRecElements->nick = received.substr(pos_nick_beg+3, pos_nick_end - (pos_nick_beg+3));
            std::cout << "msgRecElements.nick: " << msgRecElements->nick << "\n";
        }else is_correct_rec = false;
    }

    size_t pos_msg_beg = 0;
    size_t pos_msg_end = 0;
    size_t lenOfColour = 7;
    if((pos_msg_beg = received.find("color=#", pos_nick_end))!=std::string::npos)
    {
        if((pos_msg_end = received.find("</font>", pos_msg_beg))!=std::string::npos){
            msgRecElements->msg = received.substr(pos_msg_beg+7 + lenOfColour, pos_msg_end - (pos_msg_beg+7 + lenOfColour));
            //std::cout << "msgRecElements.msg: " << msgRecElements.msg << "\n";
        }else is_correct_rec = false;
    }

    size_t pos_room_beg = pos_msg_end + 10;
    size_t pos_room_end = received.length()-3;
    msgRecElements->room = received.substr(pos_room_beg, pos_room_end - pos_room_beg);
    std::cout << "msgRecElements.room: " << msgRecElements->room << "\n";


    if(is_correct_rec == false){ //if one of the above conditions is false, than we set msgRecElements->is_not_valid_message = true
        msgRecElements->is_not_valid_message = true;
        std::cout << "is_not_valid_message == true" << "\n";
    }


    return msgRecElements;
}

void Parser::resetRecElement(MsgRecElements *msgRecElements)
{
    msgRecElements->is_not_valid_message = false;
    msgRecElements->msg = "";
    msgRecElements->nick = "";
    msgRecElements->numbers = "";
    msgRecElements->room = "";
}

void Parser::resetSendElement(MsgSendElements *msgSendElements)
{
    msgSendElements->colour = "";
    msgSendElements->message = "";
    msgSendElements->nick = "";
    msgSendElements->numbers = "";
    msgSendElements->room = "";
}
