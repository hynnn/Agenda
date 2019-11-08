#include"Date.hpp"
#include<sstream>
using namespace std;
//dd
Date::Date(){
    m_year = 0;
    m_month = 0;
    m_day = 0;
    m_hour = 0;
    m_minute = 0;
}
Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute){
    m_year = t_year;
    m_month = t_month;
    m_day = t_day;
    m_hour = t_hour;
    m_minute = t_minute;
}
Date::Date(const std::string &dateString){
    //*this = stringToDate(dateString);
    this->m_day = stringToDate(dateString).getDay();
    this->m_hour = stringToDate(dateString).getHour();
    this->m_minute = stringToDate(dateString).getMinute();
    this->m_month = stringToDate(dateString).getMonth();
    this->m_year = stringToDate(dateString).getYear();
}

int Date::getYear(void) const{
    return m_year;
}
void Date::setYear(const int t_year){
    m_year = t_year;
}
int Date::getMonth(void) const{
    return m_month;
}
void Date::setMonth(const int t_month){
    m_month = t_month;
}
int Date::getDay(void) const{
    return m_day;
}
void Date::setDay(const int t_day){
    m_day = t_day;
}
int Date::getHour(void) const{
    return m_hour;
}
void Date::setHour(const int t_hour){
    m_hour = t_hour;
}
int Date::getMinute(void) const{
    return m_minute;
}
void Date::setMinute(const int t_minute){
    m_minute = t_minute;
}

bool Date::isValid(const Date &t_date){
    if(t_date.m_year < 1000 || t_date.m_year > 9999)
        return false;
    if(t_date.m_month >12 || t_date.m_month < 1)
        return false;

    if(t_date.m_month==1||t_date.m_month==3||t_date.m_month==5||t_date.m_month==7||t_date.m_month==8||t_date.m_month==10||t_date.m_month==12){
        if(t_date.m_day>31 || t_date.m_day<1)
            return false;
    }
    else if(t_date.m_month==2 ){
        if((t_date.m_year%4==0&&t_date.m_year%100!=0)||(t_date.m_year%400==0)){
            if(t_date.m_day>29||t_date.m_day<1)
                return false;
        }   
        else {
            if(t_date.m_day>28||t_date.m_day<1)
                return false;
        }
    }
    else {
        if(t_date.m_day>30 || t_date.m_day<1)
            return false;
    }

    if(t_date.m_hour > 23 || t_date.m_hour < 0)
        return false;
    if(t_date.m_minute > 59 || t_date.m_minute < 0)
        return false;

    return true;
}
Date Date::stringToDate(const std::string &t_dateString){
    if(t_dateString.length()!=16)
        return Date();
    
    if(t_dateString[4]!='-'||t_dateString[7]!='-'||t_dateString[10]!='/'||t_dateString[13]!=':')
        return Date();
    for(int i=0;i<16;i++){
        if(i==4||i==7||i==10||i==13){ }
        else{
           if(t_dateString[i]<'0'||t_dateString[i]>'9')
                return Date();
        }
    }
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;
    for(int i=0;i<4;i++)
        year = year*10+t_dateString[i]-'0';
    for(int i=5;i<=6;i++)
        month = month*10+t_dateString[i]-'0';
    for(int i=8;i<=9;i++)
        day = day*10+t_dateString[i]-'0';
    for(int i=11;i<=12;i++)
        hour = hour*10+t_dateString[i]-'0';
    for(int i=14;i<=15;i++)
        minute = minute*10+t_dateString[i]-'0';

    Date res;
    res.setYear(year);
    res.setMonth(month);
    res.setDay(day);
    res.setHour(hour);
    res.setMinute(minute);
    return res;
    //return Date(t_dateString);
}
std::string Date::dateToString(const Date &t_date){
    stringstream ss;
    
    if(!isValid(t_date))
        return "0000-00-00/00:00"; 
    else{
        ss << t_date.getYear() << '-';

        if(t_date.m_month<10)
            ss<<'0';
        ss << t_date.m_month << '-';
       
        if(t_date.m_day<10)
           ss<<'0';
        ss << t_date.m_day << '/';
        
        if(t_date.m_hour<10)
            ss<<'0';
        ss << t_date.m_hour << ':';

        if(t_date.m_minute<10)
            ss<<'0';
        ss << t_date.m_minute;

        return ss.str();
    }
    
}
Date &Date::operator=(const Date &t_date){
   // *this = t_date; 
    this->m_day = t_date.getDay();
    this->m_hour = t_date.getHour();
    this->m_minute = t_date.getMinute();
    this->m_month = t_date.getMonth();
    this->m_year = t_date.getYear();
    return *this;
}
bool Date::operator==(const Date &t_date) const{
    if(t_date.m_year == m_year&&t_date.m_month== m_month&& t_date.m_day== m_day&&t_date.m_hour== m_hour&& t_date.m_minute==m_minute)
        return true;

    return false;
}
bool Date::operator>(const Date &t_date) const{
    if(t_date.m_year > m_year)
        return false;
    else if(t_date.m_year == m_year){
        if(t_date.m_month > m_month)
            return false;
        else if(t_date.m_month == m_month){
            if(t_date.m_day>m_day)
                return false;
            else if(t_date.m_day==m_day){
                if(t_date.m_hour > m_hour)
                    return false;
                else if(t_date.m_hour== m_hour){
                    if(t_date.m_minute>=m_minute)
                        return false;
                }
            }
        }
    }

    return true;
}
bool Date::operator<(const Date &t_date) const{
    if((!((*this)==t_date))&& (!((*this)>t_date)))
        return true;
    else
        return false;
}
bool Date::operator>=(const Date &t_date) const{
    if(*this==t_date || *this>t_date)
        return true;
    else
        return false;
}
bool Date::operator<=(const Date &t_date) const{
    if(*this==t_date || *this<t_date)
        return true;
    else
        return false;
}
