/**
 * \file node.hpp
 * \author Gnomeball
 * \brief A file outlining the implementation of the Node class
 * \version 0.1
 * \date 2025-12-15
 */

#ifndef NODE
#define NODE

#include <list>
#include <string>

#include "../enums/node-type.hpp"

/**
 * \brief A class to outline the Node type
 */
class Node {

    private:

        /**
         * \brief The Type of Node this is
         */
        NodeType type;

        /**
         * \brief The value/identifier this Node carries
         */
        std::string value;

        /**
         * \brief The child Nodes of this one
         */
        std::list<Node> children;

    public:

        // Constructors

        /**
         * \brief Default constructor for a new Node object
         */
        Node(void) {} // default

        /**
         * \brief Construct a new Node object with a Type
         *
         * \param type The Type of the created Node
         */
        Node(NodeType type)
        : type{ type } {}

        /**
         * \brief Construct a new Node object with a Type, and a Value
         *
         * \param type The Type of the created Node
         * \param value The value this Node carries
         */
        Node(NodeType type, std::string value)
        : type{ type }, value{ value } {}

        // Accessors

        /**
         * \brief Get the Type of this Node
         *
         * \return The Type of the Node
         */
        NodeType get_type(void) {
            return this->type;
        }

        void set_type(NodeType type) {
            this->type = type;
        }

        /**
         * \brief Get the value of this Node
         *
         * \return The value of the Node
         */
        std::string get_value(void) {
            return this->value;
        }

        void set_value(std::string value) {
            this->value = value;
        }

        /**
         * \brief Get the child Nodes of this one, as a list
         *
         * \return The list of child Nodes belonging to this Node
         */
        std::list<Node> get_children(void) {
            return this->children;
        }

        /**
         * \brief Adds a child Node to this one
         *
         * \param child The Node to add as a child to this one
         */
        void add_child(Node child) {
            this->children.push_back(child);
        }

        // Helpers

        /**
         * \brief Returns a string containing the information related to this Byte
         *
         * \return A string represententation of this Token
         */
        const std::string to_string(int depth = 0) {
            std::string out = std::string(depth, ' ') + node_type_string.at(this->type) + "(";

            switch (this->type) {
                case NodeType::NT_COMPLEMENT:
                case NodeType::NT_NEGATE:
                case NodeType::NT_RETURN: {
                    out += "Expression: \n";
                    for (auto c : this->children) {
                        out += c.to_string(depth + 2);
                    }
                    break;
                }

                // case NodeType::NT_DECREMENT:
                case NodeType::NT_CONSTANT: {
                    out += "Value: " + this->value;
                    break;
                }

                case NodeType::NT_FUNCTION: {
                    out += "Identifier: " + this->value;
                    out += ", Statement: \n";
                    for (auto c : this->children) {
                        out += c.to_string(depth + 2);
                    }
                    break;
                }

                case NodeType::NT_PROGRAM: {
                    out += "Function: \n";
                    for (auto c : this->children) {
                        out += c.to_string(depth + 2);
                    }
                    break;
                }

                case NodeType::NT_ERROR: {
                    out += "Error: \n" + this->value;
                    break;
                }

                default: break;
            }

            return out + "\n" + std::string(depth, ' ') + ")";
        }

        // Overrides
};

#endif // BYTE
