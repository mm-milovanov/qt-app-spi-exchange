#ifndef PARSER_H
#define PARSER_H

#include <QObject>

#include <deque>

enum sparser_state_t{
    SPS_SEARCH_HEADER = 0,
    SPS_SEARCH_FOOTER
};

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(QObject *parent = nullptr);

    void setHeader(uint16_t);
    void setFooter(uint16_t);
    void setLengthOffset(uint16_t);

    void reset (void);
    void pushBuf(QByteArray buf);
    void step (void);

private:
    bool checkHeader(uint16_t header);
    bool checkFooter(uint16_t footer);

signals:
    void validMsg(QByteArray&);

private:
    QByteArray _msg;
    uint16_t m_header;
    uint16_t m_footer;

    uint16_t m_lengthOffset;
    uint16_t _msg_pos;
    uint16_t _msg_size;
    sparser_state_t _state;

    std::deque <char> dq;
};


#endif // PARSER_H
