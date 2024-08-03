#pragma once
#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>

#define HA_LOG(_stream,_priority,_what) std::_stream << "["<< #_priority << "] " << _what << std::endl;
#define HA_LOG_COUT(_priority,_what) HA_LOG(cout,_priority,_what)
#define HA_LOG_CERR(_priority,_what) HA_LOG(cerr,_priority,_what)
#define HA_LOG_NFO(_what) HA_LOG_COUT(info    ,_what)
#define HA_LOG_WRN(_what) HA_LOG_CERR(warning ,_what)
#define HA_LOG_ERR(_what) HA_LOG_CERR(error   ,_what)
#define HA_LOG_CRY(_what) HA_LOG_CERR(critical,_what)
#define HA_ABORT(_why) { HA_LOG_CRY("Application closed. " << _why); std::exit(EXIT_FAILURE); }

#ifdef HA_DEBUG
  #define HA_LOG_DBG(_what) HA_LOG_COUT(debug,"{Thread " << std::this_thread::get_id() << "} " << _what)
  #define RE_LOG_LINE(_caption) HA_LOG_DBG(_caption " in file " << __FILE__ << " at line " << __LINE__ << " in method " <<__func__)
  #define HA_MUTEX_STATUS(_mutex) if (_mutex.try_lock()) { HA_LOG_DBG("Mutex " #_mutex " is not locked"); _mutex.unlock(); } else { HA_LOG_DBG("Mutex " #_mutex " is already locked"); } RE_LOG_LINE("std::mutex status")
#else  // HA_DEBUG
  #define HA_LOG_DBG(_what)
  #define RE_LOG_LINE(_caption)
  #define HA_MUTEX_STATUS(_mutex)
#endif // HA_DEBUG

// #define HA_INCOMING_DEBUG
#ifdef HA_INCOMING_DEBUG
  #define HA_LOG_DBG_INCOMING(_what) HA_LOG_DBG("{Incoming data} " << _what)
#else // HA_INCOMING_DEBUG
  #define HA_LOG_DBG_INCOMING(_what)
#endif // HA_INCOMING_DEBUG

// #define HA_MQTT_DEBUG
#ifdef HA_MQTT_DEBUG
  #define HA_LOG_DBG_MQTT(_what) HA_LOG_DBG(_what)
#else // HA_MQTT_DEBUG
  #define HA_LOG_DBG_MQTT(_what)
#endif // HA_MQTT_DEBUG

// #define HA_SCRIPT_DEBUG
#ifdef HA_SCRIPT_DEBUG
  #define HA_LOG_DBG_SCRIPT(_what) HA_LOG_DBG(_what)
#else // HA_SCRIPT_DEBUG
  #define HA_LOG_DBG_SCRIPT(_what)
#endif // HA_SCRIPT_DEBUG

// #define HA_CONVERTER_DEBUG
#ifdef HA_CONVERTER_DEBUG
  #define HA_LOG_DBG_CONVERTER(_what) HA_LOG_DBG(_what)
#else // HA_CONVERTER_DEBUG
  #define HA_LOG_DBG_CONVERTER(_what)
#endif // HA_CONVERTER_DEBUG

// #define HA_CVALUE_DEBUG
#ifdef HA_CVALUE_DEBUG
  #define HA_LOG_DBG_CVALUE(_what) HA_LOG_DBG(_what)
#else // HA_CVALUE_DEBUG
  #define HA_LOG_DBG_CVALUE(_what)
#endif // HA_CVALUE_DEBUG

// #define HA_TIMER_DEBUG
#ifdef HA_TIMER_DEBUG
  #define HA_LOG_DBG_TIMER(_what) HA_LOG_DBG(_what)
#else // HA_TIMER_DEBUG
  #define HA_LOG_DBG_TIMER(_what)
#endif // HA_TIMER_DEBUG

#define HA_ASTRO_DEBUG
#ifdef HA_ASTRO_DEBUG
  #define HA_LOG_DBG_ASTRO(_what) HA_LOG_DBG(_what)
#else // HA_ASTRO_DEBUG
  #define HA_LOG_DBG_ASTRO(_what)
#endif // HA_ASTRO_DEBUG
