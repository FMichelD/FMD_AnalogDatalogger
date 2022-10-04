#include "QSerialPortInfoOperatores.h"
#include <map>
#include <tuple>

bool operator==(const QSerialPortInfo& a, const QSerialPortInfo& b)
{
    return std::forward_as_tuple(a.description(),
               a.manufacturer(),
               a.portName(),
               a.productIdentifier(),
               a.serialNumber())
        == std::forward_as_tuple(b.description(),
            b.manufacturer(),
            b.portName(),
            b.productIdentifier(),
            b.serialNumber());
}
