#ifndef __crc16_h__
#define __crc16_h__

#include <QObject>

uint16_t get_crc16_table (const QByteArray& buf);

#endif /* !__crc16_h__ */
