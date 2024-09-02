#include "serial/cserial.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include "utils/string.h"
#include "utils/thread.h"
#include "st.h"

namespace ha
{
namespace serial
{

CSerial::CSerial(const std::string &port, EBaudRate baudRate, EParity parity, EStopBits stopBits, EFlowControl flowControl, EBitsPerByte bitsPerByte)
  : m_fd(-1),
    m_port(port),
    m_baudRate(baudRate),
    m_parity(parity),
    m_stopBits(stopBits),
    m_flowControl(flowControl),
    m_bitsPerByte(bitsPerByte)
{
  open();
  if(write("AT+CMEE=2", ha::utils::str_CR))
  {
    HA_LOG_VERBOSE("[" << m_port << "] CMEE result " << read(1));
  }
}

CSerial::~CSerial()
{
  if (isOpen())
  {
    close();
  }
}

void CSerial::open()
{
  HA_LOG_DBG_SERIAL("Opening " << m_port);
  m_fd = ::open(m_port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
  if (!isOpen())
  {
    HA_LOG_ERR("Unable to open port " << m_port << ": (" << errno << ") " << strerror(errno));
  }
  else
  {
    #ifdef HA_SERIAL_DEBUG
    printSettings();
    #endif
    setOptions();
  }
}

void CSerial::close()
{
  ::close(m_fd);
  m_fd = -1;
}

void CSerial::setOptions()
{
  termios options;

  if(tcgetattr(m_fd, &options) != 0)
  {
    HA_LOG_ERR("Error preparing " << m_port << ": (" << errno << ") " << strerror(errno));
  }
  else
  {
    options.c_cflag = 0;
    options.c_cflag |=  (CLOCAL | CREAD);

    options.c_lflag = 0;
    // options.c_lflag &= ~(ICANON | ISIG | ECHO | ECHOE | ECHONL | NOFLSH);
    options.c_lflag |= (ICANON);

    options.c_iflag = 0;
    // options.c_iflag &= ~(BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
    // options.c_iflag |=  (IGNBRK);

    options.c_oflag = 0;
    // options.c_oflag &= ~(OPOST | ONLCR | OLCUC | OCRNL | ONLRET);

    options.c_cc[VTIME] = 0;
    options.c_cc[VMIN] = 1;

    switch(m_parity)
    {
      case EParity::pOdd:
      {
        options.c_cflag |= (PARENB | PARODD);
      } break;
      case EParity::pEven:
      {
        options.c_cflag |=  PARENB;
        options.c_cflag &= ~PARODD;
      } break;
      case EParity::pNone:
      {
        options.c_cflag &= ~PARENB;
      } break;
    }

    switch(m_stopBits)
    {
      case EStopBits::sbOne:
      {
        options.c_cflag &= ~CSTOPB;
      } break;
      case EStopBits::sbTwo:
      {
        options.c_cflag |= CSTOPB;
      } break;
    }

    switch(m_flowControl)
    {
      case EFlowControl::fcHard:
      {
        options.c_iflag &= ~(IXON | IXOFF | IXANY);
        options.c_cflag |= CRTSCTS;
      } break;

      case EFlowControl::fcSoft:
      {
        options.c_iflag |= (IXON | IXOFF | IXANY);
        options.c_cflag &= ~CRTSCTS;
      } break;

      case EFlowControl::fcNone:
      {
        options.c_iflag &= ~(IXON | IXOFF | IXANY);
        options.c_cflag &= ~CRTSCTS;
      } break;
    }

    // options.c_oflag |= OPOST;

    options.c_cflag &= ~CSIZE;
    options.c_cflag |= static_cast<unsigned int>(m_bitsPerByte);

    cfsetispeed(&options, static_cast<speed_t>(m_baudRate));
    cfsetospeed(&options, static_cast<speed_t>(m_baudRate));

    if(tcsetattr(m_fd, TCSANOW, &options) != 0)
    {
      HA_LOG_ERR("Error open " << m_port << ": (" << errno << ") " << strerror(errno));
    }
    tcflush(m_fd, TCIOFLUSH);

    #ifdef HA_SERIAL_DEBUG
    printSettings();
    #endif
  }
}

bool CSerial::isOpen() const
{
  return m_fd != -1;
}

bool CSerial::write(const std::string &command, const std::string &endl)
{
  HA_LOG_DBG_SERIAL("Sending command " << command);
  ssize_t bytesWritten = ::write(m_fd, (command + endl).c_str(), command.size() + endl.size());
  HA_LOG_DBG_SERIAL("Bytes written: " << bytesWritten);
  if (bytesWritten < 0)
  {
    HA_LOG_ERR("[" << m_port << "] Error writing to port: (" << errno << ") " << strerror(errno));
  }
  return bytesWritten > 0;
}

bool CSerial::dataAvailable(int timeout)
{
  ha::utils::sleep(50);
  if(timeout == 0) { return true; }
  fd_set readfds;
  timeval tvTimeout{timeout, 0};

  FD_ZERO(&readfds);
  FD_SET(m_fd, &readfds);

  int result = select(m_fd + 1, &readfds, nullptr, nullptr, &tvTimeout);
  if (result < 0)
  {
    HA_LOG_ERR("[" << m_port << " Error during select: (" << errno << ") " << strerror(errno));
    return false;
  }
  else if (result == 0)
  {
    HA_LOG_ERR("[" << m_port << "] Timeout waiting for data");
    return false;
  }
  return true;
}

std::string CSerial::read(int timeout)
{
  std::string response = "";
  char buffer[64];
  ssize_t bytesRead = 0;
  int bytesAvailable;

  if (dataAvailable(timeout))
  {
    do
    {
      memset(&buffer, '\0', sizeof(buffer));
      bytesRead = ::read(m_fd, &buffer, sizeof(buffer) - 1);
      if (bytesRead > 0)
      {
        HA_LOG_DBG_SERIAL("Read " << bytesRead << " bytes: '" << buffer << "'");
        response += buffer;
      }
      else if (bytesRead == 0)
      {
        HA_LOG_ERR("[" << m_port << "] Connection closed by peer");
        break;
      }
      ioctl(m_fd, FIONREAD, &bytesAvailable);
    } while (bytesAvailable > 0);
  }
  response = ha::utils::trim(response);
  if(ha::utils::contains(response, "ERROR"))
  {
    HA_LOG_ERR("[" << m_port << "] command error: " << response);
  }
  HA_LOG_DBG_SERIAL("Serial read result '" << response << "'");
  return response;
}

std::string CSerial::waitFor(const std::string &needle)
{
  std::string result = "";
  do
  {
    result += read(0);
  } while(!ha::utils::contains(result, needle));
  return result;
}

const std::string& CSerial::endlToString(const ECommandEndl &endl) const
{
  switch (endl)
  {
    case ceCR  : return ha::utils::str_CR  ;
    case ceLF  : return ha::utils::str_LF  ;
    case ceCRLF: return ha::utils::str_CRLF;
    case ceSUB : return ha::utils::str_SUB ;
    case ceNULL: return ha::utils::str_NULL;
    case ceETX : return ha::utils::str_ETX ;
  }
  return ha::utils::str_CR;
}

#ifdef HA_SERIAL_DEBUG
void CSerial::printSettings()
{
  std::cout << "--------" << std::endl;
  struct termios options;
  if (tcgetattr(m_fd, &options) != 0)
  {
    std::cerr << "Ошибка получения настроек порта: " << strerror(errno) << std::endl;
    return;
  }

  std::cout << "Настройки порта:" << std::endl;
  std::cout << "Скорость ввода: "  << speedToString(cfgetispeed(&options)) << std::endl;
  std::cout << "Скорость вывода: " << speedToString(cfgetospeed(&options)) << std::endl;

  std::cout << "Биты данных: ";
  switch (options.c_cflag & CSIZE)
  {
    case CS5: std::cout << "5"; break;
    case CS6: std::cout << "6"; break;
    case CS7: std::cout << "7"; break;
    case CS8: std::cout << "8"; break;
    default: std::cout << "неизвестно"; break;
  }
  std::cout << std::endl;

  std::cout << "Четность: ";
  if (options.c_cflag & PARENB)
  {
    if (options.c_cflag & PARODD) { std::cout << "нечетная" << std::endl; }
    else { std::cout << "четная" << std::endl; }
  }
  else { std::cout << "без четности" << std::endl; }

  std::cout << "Стоп-биты: ";
  if (options.c_cflag & CSTOPB) { std::cout << "2" << std::endl; }
  else { std::cout << "1" << std::endl; }

  std::cout << "Управление потоком: ";
  if (options.c_cflag & CRTSCTS) { std::cout << "аппаратное" << std::endl; }
  else if (options.c_iflag & (IXON | IXOFF | IXANY)) { std::cout << "программное" << std::endl; }
  else
  {
    std::cout << "нет" << std::endl;
  }

  // Вывод всех флагов в читабельном виде
  std::cout << "Флаги c_cflag: ";
  std::cout << ((options.c_cflag & CLOCAL) ? "CLOCAL " : "");
  std::cout << ((options.c_cflag & CREAD) ? "CREAD " : "");
  std::cout << ((options.c_cflag & PARENB) ? "PARENB " : "");
  std::cout << ((options.c_cflag & PARODD) ? "PARODD " : "");
  std::cout << ((options.c_cflag & CSTOPB) ? "CSTOPB " : "");
  std::cout << ((options.c_cflag & CRTSCTS) ? "CRTSCTS " : "");
  std::cout << std::endl;

  std::cout << "Флаги c_iflag: ";
  std::cout << ((options.c_iflag & IGNBRK) ? "IGNBRK " : "");
  std::cout << ((options.c_iflag & BRKINT) ? "BRKINT " : "");
  std::cout << ((options.c_iflag & PARMRK) ? "PARMRK " : "");
  std::cout << ((options.c_iflag & ISTRIP) ? "ISTRIP " : "");
  std::cout << ((options.c_iflag & INLCR) ? "INLCR " : "");
  std::cout << ((options.c_iflag & IGNCR) ? "IGNCR " : "");
  std::cout << ((options.c_iflag & ICRNL) ? "ICRNL " : "");
  std::cout << ((options.c_iflag & IXON) ? "IXON " : "");
  std::cout << ((options.c_iflag & IXOFF) ? "IXOFF " : "");
  std::cout << ((options.c_iflag & IXANY) ? "IXANY " : "");
  std::cout << std::endl;

  std::cout << "Флаги c_oflag: ";
  std::cout << ((options.c_oflag & OPOST) ? "OPOST " : "");
  std::cout << ((options.c_oflag & ONLCR) ? "ONLCR " : "");
  std::cout << ((options.c_oflag & OCRNL) ? "OCRNL " : "");
  std::cout << ((options.c_oflag & OLCUC) ? "OLCUC " : "");
  std::cout << std::endl;

  std::cout << "Флаги c_lflag: ";
  std::cout << ((options.c_lflag & ECHO) ? "ECHO " : "");
  std::cout << ((options.c_lflag & ECHOE) ? "ECHOE " : "");
  std::cout << ((options.c_lflag & ECHOK) ? "ECHOK " : "");
  std::cout << ((options.c_lflag & ECHONL) ? "ECHONL " : "");
  std::cout << ((options.c_lflag & ICANON) ? "ICANON " : "");
  std::cout << ((options.c_lflag & ISIG) ? "ISIG " : "");
  std::cout << ((options.c_lflag & NOFLSH) ? "NOFLSH " : "");
  std::cout << ((options.c_lflag & TOSTOP) ? "TOSTOP " : "");
  std::cout << std::endl;

  std::cout << "Специальные символы (c_cc): " << std::endl;
  std::cout << "VINTR: " << static_cast<int>(options.c_cc[VINTR]) << std::endl; // Прерывание
  std::cout << "VQUIT: " << static_cast<int>(options.c_cc[VQUIT]) << std::endl; // Выход
  std::cout << "VERASE: " << static_cast<int>(options.c_cc[VERASE]) << std::endl; // Удаление
  std::cout << "VKILL: " << static_cast<int>(options.c_cc[VKILL]) << std::endl; // Удаление всей строки
  std::cout << "VEOF: " << static_cast<int>(options.c_cc[VEOF]) << std::endl; // Конец файла
  std::cout << "VTIME: " << static_cast<int>(options.c_cc[VTIME]) << std::endl; // Таймаут
  std::cout << "VMIN: " << static_cast<int>(options.c_cc[VMIN]) << std::endl; // Минимальное количество символов для чтения
  std::cout << "VSWTC: " << static_cast<int>(options.c_cc[VSWTC]) << std::endl; // Не используется
  std::cout << "VSTART: " << static_cast<int>(options.c_cc[VSTART]) << std::endl; // Начало передачи
  std::cout << "VSTOP: " << static_cast<int>(options.c_cc[VSTOP]) << std::endl; // Остановка передачи
  std::cout << "VSUSP: " << static_cast<int>(options.c_cc[VSUSP]) << std::endl; // Приостановка
  std::cout << "VEOL: " << static_cast<int>(options.c_cc[VEOL]) << std::endl; // Конец строки
  std::cout << "VREPRINT: " << static_cast<int>(options.c_cc[VREPRINT]) << std::endl; // Повторный вывод
  std::cout << "VDISCARD: " << static_cast<int>(options.c_cc[VDISCARD]) << std::endl; // Отбрасывание
  std::cout << "VWERASE: " << static_cast<int>(options.c_cc[VWERASE]) << std::endl; // Удаление слова
  std::cout << "VLNEXT: " << static_cast<int>(options.c_cc[VLNEXT]) << std::endl;


  std::cout << "--------" << std::endl;
}

std::string CSerial::speedToString(speed_t speed)
{
  switch(speed)
  {
    case B0      : return "0"      ;
    case B50     : return "50"     ;
    case B75     : return "75"     ;
    case B110    : return "110"    ;
    case B134    : return "134"    ;
    case B150    : return "150"    ;
    case B200    : return "200"    ;
    case B300    : return "300"    ;
    case B600    : return "600"    ;
    case B1200   : return "1200"   ;
    case B1800   : return "1800"   ;
    case B2400   : return "2400"   ;
    case B4800   : return "4800"   ;
    case B9600   : return "9600"   ;
    case B19200  : return "19200"  ;
    case B38400  : return "38400"  ;
    case B57600  : return "57600"  ;
    case B115200 : return "115200" ;
    case B230400 : return "230400" ;
    case B460800 : return "460800" ;
    case B500000 : return "500000" ;
    case B576000 : return "576000" ;
    case B921600 : return "921600" ;
    case B1000000: return "1000000";
    case B1152000: return "1152000";
    case B1500000: return "1500000";
    case B2000000: return "2000000";
    case B2500000: return "2500000";
    case B3000000: return "3000000";
    case B3500000: return "3500000";
    case B4000000: return "4000000";
  }

  return "unknown";
}
#endif

}
}
