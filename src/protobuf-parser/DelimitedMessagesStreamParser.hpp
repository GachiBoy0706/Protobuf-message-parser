/*
 * DelimitedMessagesStreamParser.hpp
 *
 *  Created on: 2 Feb 2023
 *      Author: sia
 */

#ifndef SRC_PROTOBUF_PARSER_DELIMITEDMESSAGESSTREAMPARSER_HPP_
#define SRC_PROTOBUF_PARSER_DELIMITEDMESSAGESSTREAMPARSER_HPP_

#include "helpers.hpp"
#include <list>
#include <cctype>

template<typename MessageType>
class DelimitedMessagesStreamParser
{
public:
    typedef std::shared_ptr<const MessageType> PointerToConstValue;

    std::list<PointerToConstValue> parse(const std::string& data) 
    {
        for (const char ch : data)
        {
            m_buffer.push_back(ch);
        }
        size_t bytesConsumed = 0;
        std::list<PointerToConstValue> messages;
        
        PointerToConstValue new_message;

        do {
                try
                {
                    new_message = parseDelimited<MessageType>(m_buffer.data(), m_buffer.size(), &bytesConsumed);
                }
                catch(const std::runtime_error& e)
                {
                    throw e;
                }

                if (new_message)
                    messages.push_back(new_message);

                m_buffer.erase(m_buffer.begin(), m_buffer.begin() + bytesConsumed); //затираем расшифрованное сообщение (длина + само сообщение)
                
        } while (bytesConsumed); //пока получается расшифровывать

        return messages;
    }

private:
    std::vector<char> m_buffer;

};

#endif /* SRC_PROTOBUF_PARSER_DELIMITEDMESSAGESSTREAMPARSER_HPP_ */
