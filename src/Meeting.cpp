#include"Meeting.hpp"
using namespace std;

Meeting::Meeting(const std::string &t_sponsor,
          const std::vector<std::string> &t_participator,
          const Date &t_startTime, const Date &t_endTime,
          const std::string &t_title){
    this->m_sponsor = t_sponsor;
    m_startDate =t_startTime;
    this->m_endDate =t_endTime;
    this->m_title = t_title;
    this->m_participators = t_participator;
}
Meeting::Meeting(const Meeting &t_meeting){
    this->m_sponsor = t_meeting.m_sponsor;
    this->m_startDate = t_meeting.m_startDate;
    this->m_endDate = t_meeting.m_endDate;
    this->m_title = t_meeting.m_title;
    this->m_participators = t_meeting.getParticipator(); 
}

string Meeting::getSponsor(void) const{
    return this->m_sponsor;
}
void Meeting::setSponsor(const std::string &t_sponsor){
    this->m_sponsor = t_sponsor;
}

vector<std::string> Meeting::getParticipator(void) const{
    return this->m_participators;
}
void Meeting::setParticipator(const std::vector<std::string> &t_participators){
    this->m_participators = t_participators;
}
void Meeting::addParticipator(const std::string &t_participator){
    this->m_participators.push_back(t_participator);
}
void Meeting::removeParticipator(const std::string &t_participator){
    vector<string>::iterator it;
    for(it=m_participators.begin();it!=m_participators.end();++it){
        if((*it) == t_participator){
            it = m_participators.erase(it);
            return ;
        }
    }
}

Date Meeting::getStartDate(void) const{
    return this->m_startDate;
}
void Meeting::setStartDate(const Date &t_startTime){
    this->m_startDate = t_startTime;
    /*m_startDate.setYear(t_startTime.getYear());
    m_startDate.setMonth(t_startTime.getMonth());
    m_startDate.setDay(t_startTime.getDay());
    m_startDate.setHour(t_startTime.getHour());
    m_startDate.setMinute(t_startTime.getMinute());*/
}

Date Meeting::getEndDate(void) const{
    return this->m_endDate;
}
void Meeting::setEndDate(const Date &t_endTime){
    this->m_endDate = t_endTime;
}

string Meeting::getTitle(void) const{
    return this->m_title;
}
void Meeting::setTitle(const std::string &t_title){
    this->m_title = t_title;
}

bool Meeting::isParticipator(const std::string &t_username) const{
   // vector<string>::iterator it;
   // for(it=this->m_participators.begin();it!=m_participators.end();++it){
    for(auto s:m_participators){
        if(s == t_username){
            return true;
        }
    }

    return false;
}
