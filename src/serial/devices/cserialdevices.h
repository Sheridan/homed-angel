#pragma once
#include "serial/devices/gsm/cgsm.h"

namespace ha
{
namespace serial
{
namespace device
{

class CSerialDevices
{
public:
  CSerialDevices();
  ~CSerialDevices();

  bool gsmAccessible() const;
  bool gsmAvailable()  const;
  CGsm *gsm();
  void start();
  void stop();
private:
  CGsm *m_gsm;
};

}
}
}
