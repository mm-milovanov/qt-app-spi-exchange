#include <proto/proto.h>
#include <proto/crc16.h>

void preparePkg(proto_pkg_t& pkg) {
    pkg.header = FROM_CTRL_PACKAGE_HEADER;
    pkg.footer = FROM_CTRL_PACKAGE_FOOTER;
}
