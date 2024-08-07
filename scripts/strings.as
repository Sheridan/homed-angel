

void initialize()
{
  int l = 512;
  logger.nfo("random: " + random(l));
  logger.nfo("hash: " + calculate_hash("string"));
  logger.nfo("upper: " + to_upper("SomeString"));
  logger.nfo("lower: " + to_lower("SomeString"));
  logger.nfo("snake: " + to_snake_case("SomeString"));
  logger.nfo("camel: " + to_camel_case("some_string"));
}
