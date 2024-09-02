#include "includes/log_helpers.as"

bool do_it = false;

void checkStrings(CStrings &in ss)
{
                            logger.nfo("original"); log_strings(ss);
  ss = shuffle_strings(ss); logger.nfo("shuffled"); log_strings(ss);
  ss = sort_strings(ss);    logger.nfo("sorted");   log_strings(ss);
  int b_size = ss.size();
  ss = sort_strings(unique_strings(ss));  logger.nfo("unique");   log_strings(ss);
  int a_size = ss.size();
  logger.nfo("reduced (?) size " + b_size + " from to "  + a_size);
}

void initialize()
{
  if(do_it)
  {
    logger.nfo("String");
    logger.nfo("  hash: " + calculate_hash("string"));
    logger.nfo(" upper: " + to_upper("SomeString"));
    logger.nfo(" lower: " + to_lower("SomeString"));
    logger.nfo(" snake: " + to_snake_case("SomeString"));
    logger.nfo(" camel: " + to_camel_case("some_string"));

    logger.nfo("Strings");
    CStrings ss;
    int count = 16, size = 2;
    for (int i = 0; i<count; i++) { ss.push_back(to_string(i)); }                      checkStrings(ss); ss.resize(0);
    for (int i = 0; i<count; i++) { ss.push_back(to_string(i) + random_string(size)); } checkStrings(ss); ss.resize(0);
    for (int i = 0; i<count; i++) { ss.push_back(to_string(random(0, 128))); }          checkStrings(ss); ss.resize(0);
    logger.nfo("String to int");
    logger.nfo("       int: " + to_int("123"));
    logger.nfo("    double: " + to_double("123.345"));
    logger.nfo("hex to int: " + hex_to_int("402"));
    logger.nfo("int to hex: " + int_to_hex(123456789));

    logger.nfo("Random numeric");
    logger.nfo("  random int: " + random(0, 128));
    logger.nfo("random float: " + random(0.01, 0.1));

    logger.nfo("Random string");
    logger.nfo("random: " + random_string(128));
    logger.nfo("random: " + random_string("-=:|+", 64));
    logger.nfo("random: " + random_string("Sheridan", 16));

    logger.nfo("Numeric to string");
    int    int_value    =   123; logger.nfo("int: " + to_string(int_value   ));
    double double_value = 0.123; logger.nfo("int: " + to_string(double_value));
  }
}
