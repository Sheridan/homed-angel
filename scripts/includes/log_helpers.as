void log_strings(CStrings strings)
{
  for(int i = 0; i < strings.size(); i++)
  {
    logger.nfo("[" + i + "] " + strings[i]);
  }
}
