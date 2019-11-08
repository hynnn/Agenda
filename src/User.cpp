#include"User.hpp"
using namespace std;

User::User(const std::string &t_userName, const std::string &t_userPassword,
    const std::string &t_userEmail, const std::string &t_userPhone){
       this->m_email = t_userEmail;
       this->m_name = t_userName;
       this->m_password = t_userPassword;
       this->m_phone = t_userPhone;
}
User::User(const User &t_user){
    *this = t_user;
}

string User::getName() const{
    return this->m_name;
}
void User::setName(const std::string &t_name){
    this->m_name = t_name;
}

string User::getPassword() const{
    return this->m_password;
}
void User::setPassword(const std::string &t_password){
    this->m_password = t_password;
}

string User::getEmail() const{
    return this->m_email;
}
void User::setEmail(const std::string &t_email){
    this->m_email = t_email;
}

string User::getPhone() const{
    return this->m_phone;
}
void User::setPhone(const std::string &t_phone){
    this->m_phone = t_phone;
}
