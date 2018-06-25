#pragma once

#define LOG(x,...) Utility::g_logfile.Write(x, __VA_ARGS__);

namespace Utility {
class Logger {
  public:
    explicit Logger( std::string filename ) : path( std::move( filename ) ) {}
    Logger();
    void Write( const char * format, ... ) const;
    void Remove() const;
    ~Logger();
  private:
    const std::string path;
};

static Logger g_logfile;
}
