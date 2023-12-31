#include "DataReader.h"
#include "DataWriter.h"
#include <memory>
//C++
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <typeinfo>
#include <system_error>

//UNIX & POSIX
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

//Crypto++
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
void printHelp() {
    std::cout << "Usage: program_name [-h]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h  Display this help message" << std::endl;
}

int main(int argc, char *argv[]) {
    std::ofstream logfile("log.txt", std::ios::app);
    std::string username;
    std::string password;
    std::cout << "Логин: ";
    std::cin >> username;

    std::cout << "Пароль: ";
    std::cin >> password;

    std::string ipAddress;
    std::cout << "Введите ip-адрес: "; // 127.0.0.1
    std::cin >> ipAddress;

    if (username != "artem" || password != "123") {
        std::cout << "Неверный логин или пароль. Программа завершена." << std::endl;
        return 0;
    }
    std::cout << " " << std::endl;
    std::cout << " " << std::endl;
    printHelp();
    std::cout << " " << std::endl;
    std::cout << " " << std::endl;
    if (argc > 1 && std::string(argv[1]) == "-h") {
        printHelp();
        return 0;
    }

    DataReader reader;
    DataWriter writer;

    std::string file_name;
    std::cout << "Введите имя текстового файла: ";
    std::cin >> file_name;

    if (!reader.openFile(file_name)) {
        std::cout << "Ошибка открытия файла. Программа завершена." << std::endl;
        return 0;
    }

    std::string file_input = reader.readFile();

    reader.closeFile();

    const char* cstr = file_input.c_str();
    char buff[1024];
    Weak::MD5 hash;
    std::unique_ptr <sockaddr_in> self_addr(new sockaddr_in);
    std::unique_ptr <sockaddr_in> serv_addr(new sockaddr_in);
    self_addr->sin_family = AF_INET;
    self_addr->sin_port = 0;
    self_addr->sin_addr.s_addr = 0;
    serv_addr->sin_family = AF_INET;
    serv_addr->sin_port = htons(33333);
    serv_addr->sin_addr.s_addr = inet_addr(ipAddress.c_str());
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if ( s == -1 )
        throw std::system_error(errno, std::generic_category());
    std::cout<<"socket success\n";
    int rc = bind(s, (sockaddr*) self_addr.get(), sizeof(sockaddr_in));
    if ( rc == -1 )
        throw std::system_error(errno, std::generic_category());
    std::cout<<"bind success\n";
    rc = connect(s, (sockaddr*) serv_addr.get(), sizeof(sockaddr_in));
    if ( rc == -1 )
        throw std::system_error(errno, std::generic_category());
    std::cout<<"connect success\n";
    std::cout<<"Фаза аутентификации:\n";
    rc = send(s, "user", 4, 0);
    if ( rc == -1 )
        throw std::system_error(errno, std::generic_category());
    std::cout<<"send username \"user\"\n";
    rc = recv(s, buff, sizeof buff, 0);
    if ( rc == -1 )
        throw std::system_error(errno, std::generic_category());
    buff[rc] = 0;
    std::string salt(buff, rc); 
    std::cout << "receive " << rc << " bytes as SALT: " << salt <<std::endl;
    if (rc != 16) {
        std::cout << "Это не соль. Останов.\n";
        close(s);
        exit(1);
    }
    std::string message;
    StringSource(salt + std::string("P@ssW0rd"), true,
                 new HashFilter(hash, new HexEncoder(new StringSink(message))));
    rc = send(s, message.c_str(), message.length(), 0);
    if ( rc == -1 )
        throw std::system_error(errno, std::generic_category());
    std::cout << "send message: "<<message<<std::endl;
    rc = recv(s, buff, 1024, 0);
    if ( rc == -1 )
        throw std::system_error(errno, std::generic_category());
    buff[rc] = 0;
    std::cout << "receive " << rc << " bytes as answer: " << buff << std::endl;
    if (buff[0] != 'O' && buff[1] != 'K') {
        std::cout << "Не \"OK\" после аутентификации. Останов.\n";
        close(s);
        exit(1);
    }
    std::cout << "Фаза вычислений\n";
    rc = send(s, &nvect, sizeof nvect, 0);
    if ( rc == -1 )
        throw std::system_error(errno, std::generic_category());
    std::cout<<"send numbers of vectors\n";
    for (uint32_t i = 0; i < nvect; ++i) {
        rc = send(s, &vlen, sizeof vlen, 0);
        if ( rc == -1 )
            throw std::system_error(errno, std::generic_category());
        std::cout<<"send size of vector "<<i+1<<std::endl;
        rc = send(s, v[i], sizeof(T) * vlen,0);
        if ( rc == -1 )
            throw std::system_error(errno, std::generic_category());
        std::cout<<"send data of vector "<<i+1<<std::endl;
        rc = recv(s, &res, sizeof(T), 0);
        if ( rc == -1 )
            throw std::system_error(errno, std::generic_category());
        std::cout <<"receive "<<rc<<" bytes as result of calc vector " << i+1 <<": "<<res << std::endl;
        if (rc != sizeof(T))  {
            std::cout << "Количество присланных байт не совпадает с размером типа "<< dict.at(typeid(T).name()[0]);
            std::cout << "\nОстанов\n";
            break;
        }
    }
    close(s);
    std::cout << "Содержимое файла:" << std::endl;
	std::cout << file_input << std::endl;

	std::cout << "Передача содержимого файла на сервер..." << std::endl;
	logfile << "Содержимое файла:" << std::endl;
	logfile << file_input << std::endl;

	logfile << "Передача содержимого файла на сервер..." << std::endl;
	logfile.close();
}


    return 0;
}
