#pragma once
#include <libnova/ln_types.h>
#include <chrono>
#include "datetime/entities/cdatetime.h"

namespace ha
{
namespace datetime
{


class CTracker
{
public:
 explicit CTracker();
 virtual ~CTracker();

  // void update();
protected:
  double m_latitude;
  double m_longitude;
  double m_altitude;
  ln_lnlat_posn m_observer;

  double julian();

  std::chrono::steady_clock::time_point zonedateToChrono(const ln_zonedate& zonedate);
  bool thisTimeIsEvent(const CDateTime &dt);

};


}
}
