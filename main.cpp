#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <ctime>

class Logger
{
public:
    virtual void log(std::string message) =0;
    virtual ~Logger(){}
};

class Clock{
private:
    time_t timer;
public:
    friend std::ostream& operator<<(std::ostream& out,Clock& clock);
};

std::ostream& operator<<(std::ostream& out,Clock& clock){
    time(&clock.timer);
    std::string current_time = (ctime(&clock.timer));
    current_time.erase(current_time.end()-1);
    out<<current_time;
    return out;
} 

class ConsoleLogger: public Logger
{
private:
    Clock current_time;
public:
    virtual void log(std::string message)override{
        std::cout<<"[ConsoleLog , current time: "<<current_time<<"]"<<" "<<message<<'\n';
    }
    virtual ~ConsoleLogger(){}
};

class FileLogger: public Logger
{
private:
    std::ofstream file;
    Clock current_time;
public:
    FileLogger(){file.open("log.txt",std::ofstream::app);}
    virtual ~FileLogger(){}
    virtual void log(std::string message)override{
        if(file.is_open()){
            file<<"[FileLog , current time: "<<current_time<<"]"<<" "<<message<<'\n';
        }else{
            std::cout<<"Unable to open file\n";
        }
    }
};

class Admin
{
public:
    virtual Logger* factoryMethod() = 0;
    virtual ~Admin(){}
};

class ConsoleAdmin: public Admin
{
public:
    virtual Logger* factoryMethod()override{
        return new ConsoleLogger();
    }
    virtual ~ConsoleAdmin(){}
};

class FileAdmin: public Admin
{
public:
    virtual Logger* factoryMethod()override{
        return new FileLogger();
    }
     virtual ~FileAdmin(){}
};

using Admins = std::vector<std::unique_ptr<Admin>>;
using Loggers = std::vector<std::unique_ptr<Logger>>;

void initLogs(Loggers& logs,const Admins& admins){
    for(const auto& admin: admins){     
        logs.push_back(std::unique_ptr<Logger>(admin->factoryMethod()));
    }
}

void initAdmins(Admins& admins){
    admins.push_back(std::make_unique<ConsoleAdmin>());
    admins.push_back(std::make_unique<FileAdmin>());
}

void doLogs(Loggers& logs,std::string message){
    for(const auto& currentLog: logs){
        currentLog->log(message);
    }
}

int main(){
    Admins admins;
    initAdmins(admins);
    Loggers logs;
    initLogs(logs,admins);
    doLogs(logs,"Hello, world!");
}