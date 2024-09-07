#pragma once
#include <string>
#include <termios.h>
#include "log.h"

namespace ha
{
namespace serial
{

enum EBaudRate
{
  br0       = B0      ,
  br50      = B50     ,
  br75      = B75     ,
  br110     = B110    ,
  br134     = B134    ,
  br150     = B150    ,
  br200     = B200    ,
  br300     = B300    ,
  br600     = B600    ,
  br1200    = B1200   ,
  br1800    = B1800   ,
  br2400    = B2400   ,
  br4800    = B4800   ,
  br9600    = B9600   ,
  br19200   = B19200  ,
  br38400   = B38400  ,
  br57600   = B57600  ,
  br115200  = B115200 ,
  br230400  = B230400 ,
  br460800  = B460800 ,
  br500000  = B500000 ,
  br576000  = B576000 ,
  br921600  = B921600 ,
  br1000000 = B1000000,
  br1152000 = B1152000,
  br1500000 = B1500000,
  br2000000 = B2000000,
  br2500000 = B2500000,
  br3000000 = B3000000,
  br3500000 = B3500000,
  br4000000 = B4000000,
  brUnknown
};

enum EParity
{
  pNone,
  pOdd ,
  pEven
};

enum EStopBits
{
  sbOne,
  sbTwo
};

enum EFlowControl
{
  fcNone,
  fcHard,
  fcSoft
};

enum EBitsPerByte
{
  bpb5 = CS5,
  bpb6 = CS6,
  bpb7 = CS7,
  bpb8 = CS8
};

enum ECommandEndl
{
  ceCR  , // '\r'
  ceLF  , // '\n'
  ceCRLF, // "\r\n"
  ceSUB , // '\x1A'
  ceNULL, // '\0'
  ceETX   // '\x03'
};

class CSerial
{
public:
  explicit CSerial(const std::string &port, EBaudRate baudRate, EParity parity, EStopBits stopBits, EFlowControl flowControl, EBitsPerByte bitsPerByte);
  virtual ~CSerial();
  bool isOpen() const;
  static EBaudRate intToBaudRate(int speed);

protected:
  void open();
  void close();
  bool write(const std::string &command, const std::string &endl);
  bool dataAvailable(int timeout);
  std::string read(int timeout);
  std::string waitFor(const std::string &needle);
  const std::string& endlToString(const ECommandEndl &endl) const;

private:
  int          m_fd;
  std::string  m_port;
  EBaudRate    m_baudRate;
  EParity      m_parity;
  EStopBits    m_stopBits;
  EFlowControl m_flowControl;
  EBitsPerByte m_bitsPerByte;
  void setOptions();
  #ifdef HA_SERIAL_DEBUG
  void printSettings();
  std::string speedToString(speed_t speed);
  #endif
};

}
}
