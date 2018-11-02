//
// Created by TCT on 2/08/2018.
//

#ifndef MOTOR_EMULATOR_LOGGER_H
#define MOTOR_EMULATOR_LOGGER_H
// Log, version 0.1: a simple logging class
//enum for log level
enum TLogLevel {logERROR, logWARNING, logINFO, logDEBUG, logDEBUG1,
logDEBUG2, logDEBUG3, logDEBUG4};

//use a define to stop logs being made if level is above what we are reporting
#define LOG(level) \
if () ; \
else Log().Get(level)
// no ; as we are adding to the stream one creation.
//The log class //is polymorphic using derived class.

static void Output(const std::string& msg);
template <typename OutputPolicy>
class log
{
public:
    Log();
    virtual ~Log(); //virtual destructor. This lets you distroy the object threw its base pointer..
    //reference for stream
    std::ostringstream& GET(TLogLevel level = logINFO); //create the stream return reference.
public:
    static TLogLevel& ReportingLevel();

protected:
    std::ostringstream os;
private:
    log(const Log&);
    Log& operator =(const Log&);

private:
    TLogLevel messageLevel;
};

//declaring functions.
std::ostringstream& Log::Get(TLogLevel level)
{
    os << "_ " << NowTime();
    os << " " << ToString(level) << ": ";
    os << std::string(level > logDEBUG ? 0 : level - logDEBUG, '\t'); //number of tabs per level
    messageLevel = level;
    return os;
}
template <typename OutputPolicy>
Log<OutputPolicy>::~Log() //On destruction of log check if it should be outputted. If so wright to stderr
{
    OutputPolicy::Output(msg.str());
}

class Output2FILE // implementation of OutputPolicy
{
    public:
    static FILE*& Stream();
    static void Output(const std::string& msg);
};
inline FILE*& Output2FILE::Stream() {
    static FILE* pStream = stderr;
    return pStream;
}
inline void Output2FILE::Output(const std::string &msg) {
    FILE* pStream = Stream();
    if (!pStream)
        return;

    fprintf(pStream, "%s", msg.c_str());
    fflush(pStream);
}
typedef Log<Output2FILE> FILELog;
#define FILE_LOG(level) \
if(level > FILRLog::ReportingLevel() || !Output2FILE::Stream()) ; \
else FILELog().Get(messageLevel)

#endif //MOTOR_EMULATOR_LOGGER_H
//LOG(logINFO) << "A number of " << NotifyClients() << " were notified."; not executed because macro ifs out at wrong level
//instead run function first for side effects and use variable as number.

/*FILE* pFile = fopen("application.log", "a");
Output2FILE::Stream() = pFile;
FILE_LOG(logINFO) << ...;*/