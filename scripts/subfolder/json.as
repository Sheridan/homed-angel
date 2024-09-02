#include "../includes/log_helpers.as"
bool do_it = false;


void initialize()
{
  if(do_it)
  {
    CJson json(
"""
    {
      "key": "value",
      "array": [1,2,3,"four",5,6],
      "foo":
      {
        "bar": 123,
        "rab": "qwer"
      },
      "array_of_objects":
      [
        {
          "a": 1,
          "b": 0
        },
        {
          "a": "no",
          "b": true
        }
      ]
    }
"""
);
    logger.nfo(json.toStyledString());
    logger.nfo(json["key"].asString());
    log_strings(json.getMemberNames());
    logger.nfo(json["array"][2].asString());
    logger.nfo(json["array"][3].asString());
    CJson arr = json["array_of_objects"];
    logger.nfo("size " + arr.size());
    for(uint i = 0; i < arr.size(); i++)
    {
      CJson el = arr[i];
      log_strings(el.getMemberNames());
      if(el["b"].asBool())
      {
        logger.nfo("{" + i + "} " + el["a"].asString());
        logger.nfo("{" + i + "} " + el["b"].asString());
      }
    }
  }
}
