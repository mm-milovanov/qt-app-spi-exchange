#include <proto/parser.h>

#include <QDebug>

Parser::Parser(QObject *parent)
    : QObject{parent}
{
    this->reset();
}

void Parser::setHeader(uint16_t header) {
    m_header = header;
}

void Parser::setFooter(uint16_t footer) {
    m_footer = footer;
}

void Parser::setLengthOffset(uint16_t lengthOffset) {
    m_lengthOffset = lengthOffset;
}

void Parser::reset (void)
{
    this->_state = SPS_SEARCH_HEADER;
    this->_msg_pos = 0;
    _msg.chop(_msg.size());
}

void Parser::pushBuf(QByteArray buf)
{
    for (char byte : buf)
        dq.push_back(byte);
}

void Parser::step (void)
{

    while (!dq.empty())
    {
        char byte = dq.front();
        dq.pop_front();

        _msg.append(byte);
        switch (this->_state) {
        case SPS_SEARCH_HEADER: {
            if (this->_msg_pos == 1) {
                uint16_t header = ((uchar)_msg[1] << 8) | ((uchar)_msg[0]);
                bool ret = this->checkHeader(header);
                if (ret) {
                    this->_state = SPS_SEARCH_FOOTER;
                }
                else {
                    _msg[0] = _msg[1];
                    _msg.chop(1);
                    continue;
                }
            }
            break;
        }

        case SPS_SEARCH_FOOTER: {
            int pos = _msg.size()-1;

            if (_msg.size() > 4096) {
                /* log error (shouldn't occur ever) */
                this->reset();
                continue;
            }

            uint16_t footer = (((uchar)_msg[pos] << 8) & 0xFF00) | ((uchar)_msg[pos-1] & 0xFF);
            bool ret = this->checkFooter(footer);
            if (ret) {
                emit validMsg(_msg);
                this->reset();
                continue;
            }
            break;
        }
        }
        this->_msg_pos++;
    }
}

bool Parser::checkHeader(uint16_t header)
{
    return this->m_header == header;
}

bool Parser::checkFooter(uint16_t footer)
{
    return this->m_footer == footer;
}
