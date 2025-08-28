
#include <iostream>
#include <vector>
#include <map>
#include <memory>

// 1. Raw message parsing (protocol level)
class MessageParser {
public:
    struct ParsedMessage {
        std::string command;
        std::vector<std::string> params;
        std::string prefix;
    };
    
    static ParsedMessage parse(const std::string& raw_message);
};

// 2. Command factory with validation
class CommandFactory {
private:
    std::map<std::string, std::function<std::unique_ptr<Command>()>> creators;
    
public:
    std::unique_ptr<Command> create(const ParsedMessage& msg) {
        auto it = creators.find(msg.command);
        if (it == creators.end()) {
            throw UnknownCommandException(msg.command);
        }
        
        auto command = it->second();
        if (!command->validate(msg.params)) {
            throw InvalidParamsException(msg.command);
        }
        
        command->setParams(msg.params);
        return command;
    }
};

// 3. Base command with validation
class Command {
protected:
    std::vector<std::string> params;
    
public:
    virtual ~Command() = default;
    virtual bool validate(const std::vector<std::string>& params) = 0;
    virtual void execute(Client& client, Server& server) = 0;
    void setParams(const std::vector<std::string>& p) { params = p; }
};

// 4. Specific command implementation
class JoinCommand : public Command {
public:
    bool validate(const std::vector<std::string>& params) override {
        if (params.empty()) return false;
        // Validate channel name format, etc.
        return isValidChannelName(params[0]);
    }
    
    void execute(Client& client, Server& server) override {
        std::string channel = params[0];
        // Execute join logic
    }
};
