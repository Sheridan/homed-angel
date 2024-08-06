bool do_it = true;

void zigstarIO(SMqttMesssage &in msg)
{
  logger.nfo(msg.topic + ": " + msg.payload);
  CJson json(msg.payload);
  CJson outJson();
  outJson["uptime"] = json["uptime"];
  outJson["other"] = "another";
  outJson["boolean"] = true;
  outJson["dot"] = 1.23456654;
  outJson["num"] = 456;
  mqtt.publish("angel/test", outJson.toStyledString());
}

void initialize()
{
  if(do_it)
  {
    mqtt.subscribe(script_name, "zigstarIO", "zigstar_home/state");
  }
}
