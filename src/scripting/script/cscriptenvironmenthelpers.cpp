#include "homed/value/ccolor.h"
#include "datetime/timers/ctimercontinuos.h"
#include "datetime/timers/ctimeroneshoot.h"
#include "st.h"

namespace ha
{
namespace scripting
{

// CColor
void CreateCColorDefault(ha::homed::CColor* self) { new(self) ha::homed::CColor(); }
void CreateCColorRGB    (ha::homed::CColor* self, unsigned char r, unsigned char g, unsigned char b) { new(self) ha::homed::CColor(r, g, b); }
void CreateCColorInt    (ha::homed::CColor* self, const int &color) { new(self) ha::homed::CColor(color); }
void CreateCColorString (ha::homed::CColor* self, const std::string &color) { new(self) ha::homed::CColor(color); }
void CreateCColorCopy   (ha::homed::CColor* self, const ha::homed::CColor &other) { new(self) ha::homed::CColor(other); }
void CColorDestructor   (ha::homed::CColor* self) { self->~CColor(); };

// CTimerContinuous
ha::datetime::CTimerContinuous* CreateCTimerContinuous(const std::string &scriptName, const std::string &functionName, const int64_t &interval) { return new ha::datetime::CTimerContinuous(scriptName, functionName, static_cast<std::chrono::milliseconds>(interval)); }

// CTimerOneshot
ha::datetime::CTimerOneshot* CreateCTimerOneshot(const std::string &scriptName, const std::string &functionName, const int64_t &interval) { return new ha::datetime::CTimerOneshot(scriptName, functionName, static_cast<std::chrono::milliseconds>(interval)); }

}
}
