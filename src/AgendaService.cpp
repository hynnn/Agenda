#include"AgendaService.hpp"

using namespace std;

AgendaService::AgendaService(){
    startAgenda();
}
AgendaService::~AgendaService(){
    quitAgenda();
}

bool AgendaService::userLogIn(const std::string &userName, const std::string &password){
    auto func = [&userName,&password](const User &user)->bool{
        return (user.getName()==userName)&&(user.getPassword()==password);
    };
    return !m_storage->queryUser(func).empty();
}

bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                    const std::string &email, const std::string &phone){
    auto func = [&userName](const User &user)->bool{
        return (user.getName()==userName);
    };
    if(!m_storage->queryUser(func).empty())
        return false;

    User one(userName,password,email,phone);
    m_storage->createUser(one);
    return true;
}

bool AgendaService::deleteUser(const std::string &userName, const std::string &password){
    auto func = [userName,password](const User &user)->bool{
        return (user.getName()==userName)&&(user.getPassword()==password);
    };
    deleteAllMeetings(userName);
    auto part_meeting_list = listAllParticipateMeetings(userName);
    for(auto it : part_meeting_list)
        quitMeeting(userName,it.getTitle());

    return m_storage->deleteUser(func)>0; 
}

std::list<User> AgendaService::listAllUsers(void) const{
    auto func = [](const User &user)->bool{
        return true;
    };

    return m_storage->queryUser(func);
}

bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                     const std::string &startDate, const std::string &endDate,
                     const std::vector<std::string> &participator){
    if(participator.empty())        //check participator
        return false;
    else{
        for(int i=0;i<participator.size()-1;i++){
            for(int j=i+1;j<participator.size();j++){
                if(participator[i] == participator[j])
                    return false;
            }
        }
    }  
//check if user have been registered
    bool is_spon_reg = false;
    for(auto it : listAllUsers()){
        if(it.getName() == userName){
            is_spon_reg = true;
            break;
        }
    }
    if(!is_spon_reg)
        return false;
//check title
    auto chech_tile = [title](const Meeting &meeting){
        return meeting.getTitle()==title;
    };
    if(m_storage->queryMeeting(chech_tile).size()>0)
        return false;
//check time
    Date s_date(startDate);
    Date e_date(endDate);
    if(!Date::isValid(s_date) || !Date::isValid(e_date) || s_date >= e_date)
        return false;  
//test time conflict
    auto filter = [userName](const Meeting &meeting){
        return meeting.getSponsor() == userName;
    };
    auto meeting_list = listAllMeetings(userName);
    for(auto it : meeting_list){
        if(!(it.getStartDate() >= e_date || it.getEndDate() <= s_date))
            return false;
    }
//create meeting
    vector<string> vec;
    m_storage->createMeeting(Meeting(userName,vec,s_date,e_date,title));
    bool ispart = false;
    for(auto it : participator){
        ispart = addMeetingParticipator(userName,title,it);
        if(!ispart){
            m_storage->deleteMeeting(filter);
            return false;
        }
    }
    return true;
}

bool AgendaService::addMeetingParticipator(const std::string &userName,
            const std::string &title, const std::string &participator){
// test if part has registored
    bool is_part_reg = false;
    for(auto it : listAllUsers()){
        if(it.getName() == participator){
            is_part_reg = true;
            break;
        }
    }

    if(!is_part_reg)
        return false;
    
//test if there exist such a meeting 
    auto meeting_list = m_storage->queryMeeting([&](const Meeting &meeting){
        return meeting.getSponsor()==userName && meeting.getTitle()==title;});
    if(meeting_list.size()==0)
        return false;
// get meeeting   
    auto it = meeting_list.begin();
    auto meeting = *it;
//test time
    auto part_meeting_list = listAllMeetings(participator);
    for(auto it : part_meeting_list){
        if(!(meeting.getEndDate()<=it.getStartDate()||meeting.getStartDate()>=it.getEndDate()))
            return false;
    }
//add part
    auto filter = [userName,title](const Meeting &meeting){
        return meeting.getSponsor()==userName && meeting.getTitle()==title;
    };
    auto swithcher = [participator](Meeting &meeting){
        meeting.addParticipator(participator);
    };
    m_storage->updateMeeting(filter,swithcher);
    return true;
}

bool AgendaService::removeMeetingParticipator(const std::string &userName,
                                 const std::string &title,
                                 const std::string &participator){
    auto filter = [userName,title,participator](const Meeting meeting){
        if(meeting.getSponsor() != userName || meeting.getTitle() != title)
            return false;
        bool isPart = false;
        for(auto it : meeting.getParticipator()){
            if(it == participator){
                isPart = true;
                break;
            }
        }
        return isPart;
    };
    auto switcher = [participator](Meeting &meeting){
        meeting.removeParticipator(participator);
    };
    int temp = m_storage->updateMeeting(filter,switcher);
//delete empty meeting
    auto delete_empty_meeting = [](const Meeting &meeting){
        return (meeting.getParticipator()).size()== 0;
    };
    m_storage->deleteMeeting(delete_empty_meeting);

    return temp > 0;
}

bool AgendaService::quitMeeting(const std::string &userName, const std::string &title){
    auto filter = [userName,title](const Meeting meeting){
        if(meeting.getTitle() != title)
            return false;
        bool isPart = false;
        for(auto it : meeting.getParticipator()){
            if(it == userName){
                isPart = true;
                break;
            }
        }
        return isPart;
    };
    auto switcher = [userName](Meeting &meeting){
        meeting.removeParticipator(userName);
    };
    int temp = m_storage->updateMeeting(filter,switcher);
//delete empty meeting
    auto delete_empty_meeting = [](const Meeting &meeting){
        return (meeting.getParticipator()).size()== 0;
    };
    m_storage->deleteMeeting(delete_empty_meeting);

    return temp > 0;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                const std::string &title) const{
    auto filter = [userName,title](const Meeting &meeting){
        bool isPart = false;
        for(auto it : meeting.getParticipator()){
            if(it == userName){
                isPart = true;
                break;
            }
        }
        return (isPart || meeting.getSponsor() ==userName) && meeting.getTitle() == title;
    };
    return m_storage->queryMeeting(filter);
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &startDate,
                                  const std::string &endDate) const{
    Date s_date(startDate);
    Date e_date(endDate);
    if(!Date::isValid(s_date) || !Date::isValid(e_date) || s_date>e_date){
        list<Meeting> empty_list;
        return empty_list;
    }
    auto filter = [userName,s_date,e_date](const Meeting &meeting){
        bool isPart = false;
        for(auto it : meeting.getParticipator()){
            if(it == userName){
                isPart = true;
                break;
            }
        }
        return (isPart || meeting.getSponsor() ==userName) && (
                (meeting.getStartDate() >= s_date && meeting.getStartDate()<= e_date)||
                (meeting.getEndDate() >= s_date && meeting.getEndDate() <= e_date) ||
                (s_date==e_date && meeting.getStartDate() <= s_date && meeting.getEndDate()>=e_date) );     
    };
    return m_storage->queryMeeting(filter);
}

 std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const{
     auto filter = [userName](const Meeting &meeting){
        bool isPart = false;
        for(auto it : meeting.getParticipator()){
            if(it == userName){
                isPart = true;
                break;
            }
        }
        return (isPart || meeting.getSponsor() ==userName);
    };
    return m_storage->queryMeeting(filter);
 }

std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const{
    auto filter = [userName](const Meeting &meeting){
        return ( meeting.getSponsor() == userName);
    };
    return m_storage->queryMeeting(filter);
} 

std::list<Meeting> AgendaService::listAllParticipateMeetings(const std::string &userName) const{
     auto filter = [userName](const Meeting &meeting){
        bool isPart = false;
        for(auto it : meeting.getParticipator()){
            if(it == userName){
                isPart = true;
                break;
            }
        }
        return (isPart && meeting.getSponsor() !=userName);
    };
    return m_storage->queryMeeting(filter);
}

bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title){
    auto filter = [userName,title](const Meeting &meeting){
        return meeting.getSponsor() ==userName && meeting.getTitle() == title;
    };
    return m_storage->deleteMeeting(filter) > 0;
}

bool AgendaService::deleteAllMeetings(const std::string &userName){
    auto filter = [userName](const Meeting &meeting){
        return meeting.getSponsor() ==userName;
    };
    return m_storage->deleteMeeting(filter) > 0;
}

void AgendaService::startAgenda(void){
    m_storage = Storage::getInstance();
}

void AgendaService::quitAgenda(void){
    m_storage->sync();
}
