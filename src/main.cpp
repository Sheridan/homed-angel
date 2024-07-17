
// #include <iostream>
#include <csignal>
#include "main.h"
#include "st.h"

void handle_signal(int signal)
{
  if (signal == SIGINT)
  {
    HA_LOG_NFO("Exit requested by user...");
    running = false;
    HA_ST.angel().stop();
  }
}

int main(int argc, char* argv[])
{
  std::signal(SIGINT, handle_signal);

  HA_ST.cmdLine().addOption       <bool>("-v", "--verbose", "Verbose output"        , false);
  HA_ST.cmdLine().addOption       <bool>("-V", "--version", "Print version and exit", false);
  HA_ST.cmdLine().addOption<std::string>("-c", "--config" , "Config file location"  , "/etc/homed-angel.json");
  HA_ST.cmdLine().addOption<std::string>("-s", "--scripts", "Scripts location"      , "");
  if(!HA_ST.cmdLine().parse(argc, argv)) { return 1; };

  if(HA_ST.cmdLine().option<bool>("version")) { std::cout << "homed-angel version: " << version << std::endl; return 0; }

  HA_ST.config().load(HA_ST.cmdLine().option<std::string>("config"));

  HA_ST.mqtt().connect("tcp://" + HA_ST.config().mqttServer() + ":" + std::to_string(HA_ST.config().mqttPort()),
                       "homed-angel",
                       HA_ST.config().mqttUser(),
                       HA_ST.config().mqttPassword());
  HA_ST.mqtt().subscribe();

  while (!HA_ST.homed().ready() && running)
  {
    HA_ST.sleep();
    HA_LOG_DBG("Waiting ready...");
  }

  // #ifdef HA_DEBUG
  // HA_ST.homed().dump();
  // #endif // HA_DEBUG

  if(running)
  {
    HA_ST.angel().start();
  }


  return 0;
}
