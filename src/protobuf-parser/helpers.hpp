/*
 * helpers.h
 *
 *  Created on: 2 Feb 2023
 *      Author: sia
 */

#ifndef SRC_PROTOBUF_PARSER_HELPERS_H_
#define SRC_PROTOBUF_PARSER_HELPERS_H_

#if GOOGLE_PROTOBUF_VERSION >= 3012004
#define PROTOBUF_MESSAGE_BYTE_SIZE(message) ((message).ByteSizeLong())
#else
#define PROTOBUF_MESSAGE_BYTE_SIZE(message) ((message).ByteSize())
#endif
#include <vector>
#include <memory>
#include <stdexcept>
#include <message.pb.h>
#include <cctype>

typedef std::vector<char> Data;
typedef std::shared_ptr<const Data> PointerToConstData;

template <typename Message> PointerToConstData serializeDelimited(const Message& msg) { 
    
    int message_size = msg.ByteSizeLong();

    int prefix_size = google::protobuf::io::CodedOutputStream::VarintSize32(message_size);


    std::shared_ptr<const Data> ptr = std::make_shared<const Data>(prefix_size + message_size);

    const uint8_t* const_data = reinterpret_cast<const uint8_t*>(ptr->data());

    uint8_t* non_const_data = const_cast<uint8_t*>(const_data);

    google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(message_size, non_const_data);
    
    msg.SerializeToArray(const_cast<char*>(ptr->data()) + prefix_size, message_size);
    
    
    
    return ptr;
}

/*!
 * \brief Расшифровывает сообщение, предваренное длиной из массива байтов.
 *
 * \tparam Message Тип сообщения, для работы с которым предназначена данная
 * функция.
 *
 * \param data Указатель на буфер данных.
 * \param size Размер буфера данных.
 * \param bytesConsumed Количество байт, которое потребовалось для расшифровки
 * сообщения в случае успеха.
 *
 * \return Умный указатель на сообщение. Если удалось расшифровать сообщение, то
 * он не пустой.
 */

template<typename Message>
std::shared_ptr<Message> parseDelimited(const void* data, size_t size, size_t* bytesConsumed = 0)
{
    if (!size) // нечего парсить, data пустая 
    {
        *bytesConsumed = 0;
        return nullptr;
    }
    
    // Десериализация сообщения из массива с префиксом длины
    google::protobuf::io::CodedInputStream input_stream(static_cast<const uint8_t*>(data), size);
    
    auto initial_position = input_stream.CurrentPosition();// для вычисления длины префикса в байтах
    
    // Чтение префикса длины 
    uint32_t message_size;
    
    if (!input_stream.ReadVarint32(&message_size)) {
        std::cout << "reading message size" << std::endl;
        throw std::runtime_error("Failed to read message size.");
    }


    auto prefix_size = input_stream.CurrentPosition() - initial_position; 

    if (message_size > size - prefix_size)
    {
        *bytesConsumed = 0;
        return nullptr;
    }

    if (message_size > static_cast<uint32_t>(size-prefix_size)) // в буфере нецелое сообщение
    {
        message_size = size-prefix_size;
    }
    
    std::shared_ptr<Message> deserialized_message = std::make_shared<Message>();


    if (!deserialized_message->ParseFromArray(data + prefix_size, message_size)) {
        throw std::runtime_error("Failed to parse message.");
    }


    *bytesConsumed = static_cast<size_t>(message_size) + static_cast<size_t>(prefix_size);
    

    return deserialized_message;
};




#endif /* SRC_PROTOBUF_PARSER_HELPERS_H_ */
