#include"AgendaUI.hpp"
#include<iomanip>

using namespace std;

AgendaUI::AgendaUI(){
    startAgenda();
}
void AgendaUI::OperationLoop(){
    string operation;
    while(1){
        cout << endl << "------------------------------Agenda------------------------------" << endl
            << "Action : " << endl
            << "l    - log in Agenda by user name and password"<< endl
            << "r    - register an Agenda account" << endl
            << "q    - quit Agenda" << endl
            << "-------------------------------------------------------------------" << endl
            << endl << "Agenda :~$";
        cin >> operation;
        switch(operation[0]){
            case 'l' : userLogIn();break;
            case 'r' : userRegister();break;
            case 'q' : quitAgenda();break;
            default: break;
        }
        if(operation[0]=='q')
            break;
    }   
}

void AgendaUI::startAgenda(){
    m_agendaService.startAgenda();
}

string AgendaUI::getOperation(){
    cout << endl << "------------------------------Agenda------------------------------" << endl
        << "Action : " << endl
        << "o    - log out Agenda"<< endl
        << "dc   - delete Agedna account" << endl
        << "lu   - list all Agenda user" << endl
        << "cm   - creating a meeting" << endl
        << "amp  - add meeting participator" << endl
        << "rmp  - remove meeting participator" << endl
        << "rqm  - requeat to quit meeting" << endl
        << "la   - list all meeting" << endl
        << "las  - list all sponsor meeting" << endl
        << "lap  - list all participator meeting" << endl
        << "qm   - query meeting by title" << endl
        << "qt   - query meeting by time interval" << endl
        << "dm   - delete meeting by title" << endl
        << "da   - delete all meetings" << endl
        << "-------------------------------------------------------------------" << endl
        << endl << "Agenda :~# ";
    string operation;
    cin >> operation ;
    return operation;
}
bool AgendaUI::executeOperation(std::string t_operation){
    if(t_operation == "o"){
        userLogOut();
        return false;
    }
    else if(t_operation == "dc"){
        deleteUser();
        return false;
    }
    else if(t_operation == "lu"){
        listAllUsers();
        return true;
    }
    else if(t_operation == "cm"){
        createMeeting();
        return true;
    }
    else if(t_operation == "amp"){
        addMeetingParticipator();
        return true;
    }
    else if(t_operation == "rmp"){
        removeMeetingParticipator();
        return true;
    }
    else if(t_operation == "rqm"){
        quitMeeting();
        return true;
    }
    else if(t_operation == "la"){
        listAllMeetings();
        return true;
    }
    else if(t_operation == "las"){
        listAllSponsorMeetings();
        return true;
    }
    else if(t_operation == "lap"){
        listAllParticipateMeetings();
        return true;
    }
    else if(t_operation == "qm"){
        queryMeetingByTitle();
        return true;
    }
    else if(t_operation == "qt"){
        queryMeetingByTimeInterval();
        return true;
    }
    else if(t_operation == "dm"){
        deleteMeetingByTitle();
        return true;
    }
    else if(t_operation == "da"){
        deleteAllMeetings();
        return true;
    }
    return true;
}

void AgendaUI::userLogIn(void){
    cout << endl << "[log in] [username] [password]"<< endl
        << "[log in] ";
    string name,password;
    cin >> name >> password;
    if(m_agendaService.userLogIn(name,password)){
        m_userName = name;
        m_userPassword = password;
        cout << "[log in] succeed!" << endl;
        while(executeOperation(getOperation())){ }
    }
    else
        cout << "[lgg in] Password error or user doesn't exist!" << endl;
}

void AgendaUI::userRegister(void){
    cout << endl << "[register] [username] [password] [email] [phone]" << endl
        << "[register] ";
    string name,password,email,phone;
    cin >> name >> password >> email >> phone;
    if(m_agendaService.userRegister(name,password,email,phone))
        cout << "[register] succeed!" << endl;
    else
        cout << "[register] This username has been registered!" << endl;
}

void AgendaUI::userLogOut(void){
    m_userPassword.clear();
    m_userName.clear();
}
void AgendaUI::quitAgenda(void){
    m_agendaService.quitAgenda();
}
void AgendaUI::deleteUser(void){
    if(m_agendaService.deleteUser(m_userName,m_userPassword)){
        cout << endl << "[delete agenda account] succeed!" << endl;
        userLogOut();
    }
    else
        cout << endl << "[delete agenda account] error!" << endl;
}
void AgendaUI::listAllUsers(void){
    cout << endl
        << "[list all users]" << endl
        << endl;
    auto user_list = m_agendaService.listAllUsers();
    int name_length,email_length,phone_length;
    for(auto it : user_list){
        name_length = it.getName().size()>string("name").size() ? it.getName().size() : string("name").size();
        email_length = it.getEmail().size()>string("email").size() ? it.getEmail().size(): string("email").size();
        phone_length = it.getPhone().size()>string("phone").size() ? it.getPhone().size() : string("phone").size();
    }
    cout << left << setw(name_length+4) << "name"
        << setw(email_length+4) << "email"
        << setw(phone_length+4) << "phone" << endl;
    for(auto it : user_list){
        cout << left << setw(name_length+4) << it.getName()
        << setw(email_length+4) << it.getEmail()
        << setw(phone_length+4) << it.getPhone() << endl;
    }
}
void AgendaUI::createMeeting(void){
    cout << endl << "[create meeting] [the number of participators]" << endl
        << "[create meeting] ";
    int num;
    cin >> num;
    vector<string> part_vec;
    for(int i=1;i<=num;i++){
        cout << "[create meeting] [please enter the participator " << i << " ]" <<endl
            << "[create meeting] ";
        string name;
        cin >> name;
        part_vec.push_back(name);
    }
    cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]"<<endl
        << "[create meeting] ";
    string title,s_date,e_date;
    cin >> title >> s_date >> e_date;
    if(m_agendaService.createMeeting(m_userName,title, s_date, e_date, part_vec))
        cout << "[create meeting] succeed!"<< endl;
    else
        cout << "[create meeting] error!"<< endl;
}
void AgendaUI::addMeetingParticipator(void){
    cout << endl
        << "[add participator] [meeting title] [participator username]" << endl
        << "[add participator] ";
    string title,part;
    cin >> title >> part;
    if(m_agendaService.addMeetingParticipator(m_userName,title, part))
        cout << "[add participator] succeed!" << endl;
    else
        cout << "[add participator] error!" << endl;
}
void AgendaUI::removeMeetingParticipator(void){
    cout << endl
        << "[remove participator] [meeting title] [participator username]" << endl
        << "[remove participator] ";
    string title,part;
    cin >> title >> part;
    if(m_agendaService.removeMeetingParticipator(m_userName,title, part))
        cout << "[remove participator] succeed!" << endl;
    else
        cout << "[remove participator] error!" << endl;
}
void AgendaUI::quitMeeting(void){
    cout << endl
        << "[quit meeting] [meeting title]" << endl
        << "[quit meeting] ";
    string meeting;
    cin >> meeting;
    if(m_agendaService.quitMeeting(m_userName,meeting))
        cout << "[quit meeting] succeed!" << endl;
    else
        cout << "[quit meeting] error!" << endl;
}
void AgendaUI::listAllMeetings(void){
    cout << endl
        << "[list all meetings]" << endl
        << endl;
    printMeetings(m_agendaService.listAllMeetings(m_userName));
}
void AgendaUI::listAllSponsorMeetings(void){
     cout << endl
        << "[list all sponsor meetings]" << endl
        << endl;
    printMeetings(m_agendaService.listAllSponsorMeetings(m_userName));
}
void AgendaUI::listAllParticipateMeetings(void){
    cout << endl
        << "[list all participator meetings]" << endl
        << endl;
    printMeetings(m_agendaService.listAllParticipateMeetings(m_userName));
}
void AgendaUI::queryMeetingByTitle(void){
    cout << endl
        << "[query meeting] [title]" << endl
        << "[query meeting] ";
    string title;
    cin >> title;
    printMeetings(m_agendaService.meetingQuery(m_userName,title));
}
void AgendaUI::queryMeetingByTimeInterval(void){
    cout << endl
        << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl
        << "[query meetings] ";
    string s_date,e_date;
    cin >> s_date >> e_date;
    printMeetings(m_agendaService.meetingQuery(m_userName,s_date,e_date));   
}
void AgendaUI::deleteMeetingByTitle(void){
    cout << endl
        << "[delete meeting] [title]" << endl
        << "[delete meeting] ";
    string title;
    cin >> title;
    if(m_agendaService.deleteMeeting(m_userName, title))
        cout << "[delete meeting] succeed!" << endl;
    else
        cout << "[delete meeting] error!" << endl;
}
void AgendaUI::deleteAllMeetings(void){
    m_agendaService.deleteAllMeetings(m_userName);
    cout << endl
        << "[delete all meeting] succeed!" << endl;
}
void AgendaUI::printMeetings(std::list<Meeting> t_meetings){
    int title_length,sponsor_length,time_length = 16;
    for(auto it : t_meetings){
        title_length = it.getTitle().size()>string("title").size() ? it.getTitle().size() : string("title").size();
        sponsor_length = it.getSponsor().size()>string("sponsor").size() ? it.getSponsor().size(): string("sponsor").size();
    }
    cout << left << setw(title_length+4) << "title"
        << setw(sponsor_length+4) << "sponsor"
        << setw(time_length+4) << "start time" 
        << setw(time_length+4) << "end time" 
        << "participators"<< endl;
    for(auto it : t_meetings){
        cout << left << setw(title_length+4) << it.getTitle()
        << setw(sponsor_length+4) << it.getSponsor()
        << setw(time_length+4) << Date::dateToString(it.getStartDate())
        << setw(time_length+4) << Date::dateToString(it.getEndDate());
        auto part = it.getParticipator();
        int part_size = part.size();
        if(!part.empty()){
            for(auto itt : part){
                cout << itt;
                part_size--;
                if(part_size>0)
                    cout << ",";
            }
        }
        cout << endl;
    }
}