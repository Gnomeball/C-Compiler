#include <map>
#include <string>

enum class TackyValType {
    // Values
    // TACKY_CONSTANT, // OP_CONSTANT
    TACKY_VALUE, // Temporary value
};

const std::map<TackyValType, std::string> val_string = {
    // { TackyValType::TACKY_CONSTANT, "CONSTANT" },
    { TackyValType::TACKY_VALUE, "VALUE" },
};

class TackyVal {

    private:

        TackyValType type;
        std::string value;

    public:

        // Constructors

        TackyVal(void) {} // default

        TackyVal(TackyValType type, std::string value)
        : type{ type }, value{ value } {}

        // Accessors

        TackyValType get_type(void) {
            return this->type;
        }

        std::string get_value(void) {
            return this->value;
        }

        // Helpers

        const std::string to_string(void) {
            std::string out = "TACKY_VAL [type: " + val_string.at(this->type);

            out += ", value: " + this->get_value();

            return out + "]";
        }
};
