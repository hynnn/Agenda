#include"Storage.hpp"
#include"Path.hpp"
#include<iostream>
#include<fstream>
using namespace std;

shared_ptr<Storage> Storage::m_instance = nullptr;

Storage::Storage(){
    this->m_dirty = false;
    readFromFile();
}

bool Storage::readFromFile(void){
    ifstream usersfile(Path::userPath);
    ifstream meetingsfile(Path::meetingPath);
    if(!(usersfile.is_open()&&meetingsfile.is_open()))
        return false;
//users
    string str;
    while(getline(usersfile,str)){
        string name,password,email,phone;
        if(str.size()==0)
            continue;
//find name,password,email,phone
        vector<int> index;
        for(int i=0;i<str.length();i++){
            if(str[i]=='"')
                index.push_back(i);
        }
        for(int i=index[0]+1;i<index[1];i++)
            name = name + str[i];       //push_back()
        for(int i=index[2]+1;i<index[3];i++)
            password = password + str[i];
        for(int i=index[4]+1;i<index[5];i++)
            email = email + str[i];
        for(int i=index[6]+1;i<index[7];i++)
            phone = phone + str[i];
        User newuser(name,password,email,phone);
        this->m_userList.push_back(newuser);
    }
    usersfile.close();

    str = "";       //clear
    while(getline(meetingsfile,str)){
        string sponsor,part,startdata,enddata,title;
        if(str.size()==0)
            continue;
//find
        vector<int> index;
        for(int i=0;i< str.length();i++){
            if(str[i]=='"')
                index.push_back(i);
        }
        for(int i=index[0]+1;i<index[1];i++)
            sponsor = sponsor + str[i];       //push_back()
        for(int i=index[2]+1;i<index[3];i++)
            part = part + str[i];
        for(int i=index[4]+1;i<index[5];i++)
            startdata = startdata + str[i];
        for(int i=index[6]+1;i<index[7];i++)
            enddata = enddata + str[i];
        for(int i=index[8]+1;i<index[9];i++)
            title = title + str[i];
//partticipators
        vector<string> parts;
        string strpart="";
        for(int i=0;i<part.length();i++){
            if(part[i]!='&')
                strpart = strpart + part[i];
            else{
                string temp = strpart;
                parts.push_back(temp);
                strpart.clear();
            }
        }
        parts.push_back(strpart);       //
        Meeting newmeeting(sponsor,parts,Date(startdata),Date(enddata),title);
        this->m_meetingList.push_back(newmeeting);
    }
    meetingsfile.close();
    return true;
}
bool Storage::writeToFile(void){
    ofstream usersfile(Path::userPath);
    ofstream meetingsfile(Path::meetingPath);
    if(!(usersfile.is_open()&&meetingsfile.is_open()))
        return false;

    for(auto it = this->m_userList.begin();it!=this->m_userList.end();++it){
        usersfile<<'"'<<(*it).getName()<<'"'<<','<<'"'<<(*it).getPassword()<<'"'<<','<<'"'<<(*it).getEmail()<<'"'<<','<<'"'<<(*it).getPhone()<<'"';
        usersfile<<"\n";
    }
    usersfile.close();

    for(auto it=m_meetingList.begin();it!=m_meetingList.end();++it){
        meetingsfile<<'"'<<(*it).getSponsor()<<'"'<<','<<'"';
        vector<string> part = (*it).getParticipator();
        for(int i=0;i<part.size()-1;i++)
            meetingsfile<<part[i]<<'&';
        meetingsfile<<part[part.size()-1];
        meetingsfile<<'"'<<','<<'"'<<(*it).getStartDate().dateToString((*it).getStartDate())<<'"'<<','<<'"'
            <<(*it).getEndDate().dateToString((*it).getEndDate())<<'"'<<','<<'"'<<(*it).getTitle()<<'"';
        meetingsfile<<"\n";
    }
    meetingsfile.close();
    return true;
}

shared_ptr<Storage> Storage::getInstance(void){
    if(m_instance==nullptr)
        m_instance = shared_ptr<Storage>(new Storage());
    return m_instance;
}

Storage::~Storage(){
    sync();
}

void Storage::createUser(const User &t_user){
    m_userList.push_back(t_user);
    m_dirty = true;
}

list<User> Storage::queryUser(function<bool(const User &)> filter) const{
    list<User> res;
    for(auto it = this->m_userList.begin();it!=this->m_userList.end();++it){
        if(filter(*it))
            res.push_back(*it);
    }
    return res;
}
int Storage::updateUser(function<bool(const User &)> filter,
        function<void(User &)> switcher){
    int count = 0;
    for(auto it = this->m_userList.begin();it!=this->m_userList.end();++it){
        if(filter(*it)){
            switcher(*it);
            count++;
        }
    }
    if(count!=0)
        m_dirty = true;
    return count;
}
int Storage::deleteUser(function<bool(const User &)> filter){
    int count = 0;
    for(auto it = this->m_userList.begin();it!=this->m_userList.end();){
        if(filter(*it)){
            it=m_userList.erase(it);
            count++;
        }
        else
            ++it;
    }
    if(count!=0)
        m_dirty = true;
    return count;
}

void Storage::createMeeting(const Meeting &t_meeting){
    m_meetingList.push_back(t_meeting);
    m_dirty = true;
}
list<Meeting> Storage::queryMeeting(function<bool(const Meeting &)> filter) const{
    list<Meeting> res;
    for(auto it = this->m_meetingList.begin();it!=this->m_meetingList.end();++it){
        if(filter(*it))
            res.push_back(*it);
    }
    return res;
}
int Storage::updateMeeting(function<bool(const Meeting &)> filter,
        function<void(Meeting &)> switcher){
    int count = 0;
    for(auto it = this->m_meetingList.begin();it!=this->m_meetingList.end();++it){
        if(filter(*it)){
            switcher(*it);
            count++;
        }
    }
    if(count!=0)
        m_dirty = true;
    return count;
}
int Storage::deleteMeeting(function<bool(const Meeting &)> filter){
    int count = 0;
    for(auto it = this->m_meetingList.begin();it!=this->m_meetingList.end();){
        if(filter(*it)){
            it=m_meetingList.erase(it);
            count++;
        }
        else
            ++it;
    }
    if(count!=0)
        m_dirty = true;
    return count;
}

bool Storage::sync(void){
    m_dirty = false;
    return writeToFile();
}
