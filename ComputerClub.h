#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

class ComputerClub {

	unsigned int places = 0; 
	int price = 0; 
	string start_time;
	string end_time;
	vector<int> free_places;
    vector<string> events_time;
    vector<int> events_id;
    vector<string> events_client;
    vector<int> events_table;

    friend ostream& operator << (ostream &os, const ComputerClub &computerClub) {
        set<string> clients_in_club;
        map<int,int> table_profit;
        map<int,string> total_time;
        map<int,string> table_time;
        map<int,string> busy_tables;
        vector<string> waiting_queue;

        os << computerClub.start_time << endl;

        for(unsigned int i = 0; i < computerClub.events_time.size(); i++) { 
            os << computerClub.events_time[i]<< " " << computerClub.events_id[i] << " " << computerClub.events_client[i];
            if(computerClub.events_table[i]) 
                os << " " << computerClub.events_table[i] << endl;
            else os << endl;

            if (!computerClub.TimeChecker(computerClub.events_time[i],computerClub.start_time,computerClub.end_time))
            {
                os << computerClub.events_time[i] << " 13 " << "NotOpenYet" << endl;
                continue;
            } else if ((find(clients_in_club.begin(), clients_in_club.end(), computerClub.events_client[i]) == clients_in_club.end()) && 
            (computerClub.events_id[i] != 1)) {
                os << computerClub.events_time[i] << " 13 " << "ClientUnknown" << endl;
                continue;
            }

            if (computerClub.events_id[i] == 1) {         
                if (find(clients_in_club.begin(), clients_in_club.end(), computerClub.events_client[i]) != clients_in_club.end())
                    os << computerClub.events_time[i] << " 13 " << "YouShallNotPass" << endl;
                else clients_in_club.insert(computerClub.events_client[i]);

            } else if (computerClub.events_id[i] == 2) {
                if (busy_tables.count(computerClub.events_table[i]))
                    os << computerClub.events_time[i] << " 13 " << "PlaceIsBusy" << endl; 
                else {
                    for (auto& [table,client] : busy_tables) {
                        if (client == computerClub.events_client[i]) {
                            busy_tables.erase(table);
                            string time_dif = computerClub.TimeDifference(table_time[table], computerClub.events_time[i]);
                            total_time[table] = computerClub.TotalTimeCalculator(total_time[table], time_dif);
                            table_profit[table] += computerClub.ProfitCalculator(table_time[table], computerClub.events_time[i], computerClub.price);
                            table_time.erase(table);
                            break;
                        }
                    }
                    busy_tables.insert({computerClub.events_table[i], computerClub.events_client[i]});
                    table_time.insert({computerClub.events_table[i], computerClub.events_time[i]});
                }

            } else if (computerClub.events_id[i] == 3) {
                if (busy_tables.size() < computerClub.places)
                    os << computerClub.events_time[i] << " 13 " << "ICanWaitNoLonger!" << endl; 
                else if(waiting_queue.size() >= computerClub.places) 
                    os << computerClub.events_time[i] << " 11 " << computerClub.events_client[i] << endl;
                else waiting_queue.push_back(computerClub.events_client[i]);

            } else if (computerClub.events_id[i] == 4) { 
                for (auto& [table,client] : busy_tables) {
                    if (client == computerClub.events_client[i]) {
                        busy_tables.erase(table);
                        string time_dif = computerClub.TimeDifference(table_time[table], computerClub.events_time[i]);
                        total_time[table] = computerClub.TotalTimeCalculator(total_time[table], time_dif);
                        table_profit[table] += computerClub.ProfitCalculator(table_time[table], computerClub.events_time[i], computerClub.price);
                        table_time.erase(table);
                        if (!waiting_queue.empty()) {
                            table_time.insert({table,computerClub.events_time[i]});
                            busy_tables.insert({table, waiting_queue[0]});
                            os << computerClub.events_time[i] << " 12 " << waiting_queue[0]<< " " << table << endl;
                            waiting_queue.erase(waiting_queue.begin());
                        }
                        break;
                    }                                               
                }
                clients_in_club.erase(computerClub.events_client[i]);
                auto it = find(waiting_queue.begin(), waiting_queue.end(), computerClub.events_client[i]);
                if (it != waiting_queue.end())
                    waiting_queue.erase(it);
            }
        }

        for(string client : clients_in_club) {
            os << computerClub.end_time << " 11 " << client << endl;
        }

        for(auto& [table,time] : table_time) {
            string time_dif = computerClub.TimeDifference(table_time[table], computerClub.end_time);
            total_time[table] = computerClub.TotalTimeCalculator(total_time[table], time_dif);
            table_profit[table] += computerClub.ProfitCalculator(table_time[table], computerClub.end_time, computerClub.price);
        }   
            os << computerClub.end_time << endl;
        for(auto& [table,profit] : table_profit) {
            os << table << " " << profit << " " << total_time[table] << endl;
        }
        return os;
    }

    bool TimeChecker(string current_time, string start_time, string end_time) const {
        int current_hour = atoi(current_time.substr(0,2).c_str());
        int current_minutes = atoi(current_time.substr(3,2).c_str());
        int start_hour = atoi(start_time.substr(0,2).c_str());
        int start_minutes = atoi(start_time.substr(3,2).c_str());
        int end_hour = atoi(end_time.substr(0,2).c_str());
        int end_minutes = atoi(end_time.substr(3,2).c_str());
        return ((current_hour > start_hour) && (current_hour < end_hour)) || 
        ((current_hour == start_hour) && (current_minutes >= start_minutes) && (current_hour < end_hour)) || 
        ((current_hour > start_hour) && (current_hour == end_hour) && (current_minutes <= end_minutes)) || 
        ((current_hour == start_hour) && (current_minutes >= start_minutes) && (current_hour == end_hour) && (current_minutes <= end_minutes));
    }

    int ProfitCalculator(string start_time, string end_time, int price) const{
        int start_hour = atoi(start_time.substr(0,2).c_str());
        int start_minutes = atoi(start_time.substr(3,2).c_str());
        int end_hour = atoi(end_time.substr(0,2).c_str());
        int end_minutes = atoi(end_time.substr(3,2).c_str());
        if (start_minutes > end_minutes)
            return (end_hour - start_hour) * price;
        return (end_hour - start_hour + 1) * price;
    }

    string TimeDifference(string start_time, string end_time) const{
        string result;
        int start_hour = atoi(start_time.substr(0,2).c_str());
        int start_minutes = atoi(start_time.substr(3,2).c_str());
        int end_hour = atoi(end_time.substr(0,2).c_str());
        int end_minutes = atoi(end_time.substr(3,2).c_str());
        if (start_minutes  > end_minutes)
            result = to_string((end_hour - start_hour - 1)) + ":" + to_string((end_minutes - start_minutes + 60));
        else 
            result = to_string((end_hour - start_hour)) + ":" + to_string((end_minutes - start_minutes));
        if (end_hour - start_hour < 10)
            result = '0' + result;
        if(result.size() < 5) 
            result.insert(3,1,'0'); 
        return result;
    }

        
    string TotalTimeCalculator(string start_time, string end_time) const{
        string result;
        if(start_time == "")
            start_time = "00:00";
        int start_hour = atoi(start_time.substr(0,2).c_str());
        int start_minutes = atoi(start_time.substr(3,2).c_str());
        int end_hour = atoi(end_time.substr(0,2).c_str());
        int end_minutes = atoi(end_time.substr(3,2).c_str());
        if (start_minutes + end_minutes >= 60)
            result = to_string((end_hour + start_hour + 1)) + ":" + to_string((end_minutes + start_minutes - 60));
        else 
            result = to_string((end_hour + start_hour)) + ":" + to_string((end_minutes + start_minutes));
        if (end_hour - start_hour < 10)
            result = '0' + result;
        if(result.size() < 5) 
            result.insert(3,1,'0'); 
        return result;
    }

public:
	ComputerClub(){}

	ComputerClub(const string &file) {
    string line;
    ifstream in(file);
    if (in.is_open())
    {
        getline(in, line);
        places = atoi(line.c_str());
        getline(in, line,' ');
        start_time = line;
        getline(in, line);
        end_time = line;
        getline(in, line);
        price = atoi(line.c_str());
        while (true) {
            if(getline(in, line,' '))
                events_time.push_back(line);
            else break;

            getline(in, line,' ');
            events_id.push_back(atoi(line.c_str()));

            if (events_id[events_id.size() - 1] == 2) {
                getline(in, line,' ');
                events_client.push_back(line);

                getline(in, line);
                events_table.push_back(atoi(line.c_str()));
            }
            else {
                getline(in, line);
                events_client.push_back(line);

                events_table.push_back(0);
            }
        }

        for(unsigned int i = 0; i <= places; i++) {
            free_places.push_back(i); 
        }

    }
    in.close();  
	}

    int GetPlaces(){
        return places;
    }

    string GetStartTime(){
        return start_time;
    }

    string GetEndTime(){
        return end_time;
    }

    int GetPrice(){
        return price;
    }

    vector<string> GetEventsClient(){
        return events_client;
    }

    vector<string> GetEventsTime(){
        return events_time;
    }
    vector<int> GetEventsId(){
        return events_id;
    }

};
