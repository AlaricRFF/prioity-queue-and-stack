#include "dlist.h"
#include <iostream>
using namespace std;
struct userInfo_t{
    // a struct used to store the user's info
    int timeStamp; // time called in
    string name; // user's name
    string priority; // group of elite he/she belongs
    int duration; // the duration of the call
};

typedef userInfo_t* userInfoPtr_t; // a pointer to the userInfo struct
/// DEBUG FUNCTION
void print(const Dlist<userInfo_t>& queue){
    // MODIFIES: cout
    // EFFECTS: print the users' info accroding to their priority
    Dlist<userInfo_t> copy(queue);
    while (!copy.isEmpty()){
        userInfo_t* user = copy.removeBack();
        cout << user->name << '\t' << user->priority << "\ttime stamp: "<< user->timeStamp << "\tduration: "<<user->duration << endl;
    }
}
///
inline void showTime(const int curTime){
    // MODIFIES: cout
    // EFFECTS: show the current time stamp in required format
    cout << "Starting tick #" << curTime << endl;
}

void userCallIn(userInfoPtr_t *userStore, const int& userNum, int &call_in_index, const int& curTime){
    // MODIFIES: call_in_index, cout
    // EFFECTS: if there are users calling in at `curTime`, print the message to cout.
    // We search the user using `call_in_index`, since the call-in time of users in `userStore` is
    // increasing. After the last user calling at this time stamp is found, renew `call_in_index`
    if (call_in_index >= userNum){
        return;
    }
    if (userStore[call_in_index]->timeStamp != curTime){ /// no user call in, return
        return;
    }
    while ( call_in_index < userNum && userStore[call_in_index]->timeStamp == curTime){ /// add userNum judgement to avoid BAD ACCESS of pointer
        cout << "Call from "<< userStore[call_in_index]->name << " a "<< userStore[call_in_index]->priority << " member\n";
        call_in_index ++; // renew index
    }
}

int dealCall_oneQueue(Dlist<userInfo_t>* queue, const int& curTime) {
    // MODIFIES: queue, cout
    // EFFECTS: check whether the first user in the queue satisfy the calling-in time condition. If yer, print its info to cout
    // return the duration of the call; if not satisfied, return -1
    /// NOTE: user whose timeStamp <= curTime will be able to deal
    if (queue->isEmpty()) {
        return -1; /// -1 denotes that further checking through other list is necessary!
    }
    auto user = queue->removeBack(); // extract the user possible to deal with
    if (user->timeStamp <= curTime) { // check whether the call-in time satisfy
        cout << "Answering call from " << user->name << endl;
        return user->duration - 1; // counting into the current round, therefore need to be subtracted by one
    } else {
        queue->insertBack(user); /// leave the queue unchanged
        return -1;
    }
}

int main(){
    Dlist<userInfo_t> platinum_queue, gold_queue, silver_queue, regular_queue;
    /// These queues stores the information of each user's call
    /// REGULATION: use insertFront as PUSH (add new user to the queue); use removeBack as POP (remove user from the queue)
    /*  INPUT PROCESSING */
    int userNum;
    cin >> userNum;
    userInfoPtr_t *userStore = new userInfoPtr_t[userNum]; // this array is used to store the pointer to userInfo_t
    // Used as to delete each dynamic userInfo_t object
    for (int i = 0; i < userNum; ++i) {
        userInfo_t *user = new userInfo_t;
        /// NOTE: we delete these dynamic object after all the operations are done!
        cin >> user->timeStamp >> user->name >> user->priority >> user->duration;
        if (user->priority == "platinum"){
            platinum_queue.insertFront(user);
        }
        else if (user->priority == "gold"){
            gold_queue.insertFront(user);
        }
        else if (user->priority == "silver"){
            silver_queue.insertFront(user);
        }
        else if (user->priority == "regular"){
            regular_queue.insertFront(user);
        }
        else{
            cout << "bad input!\n";
            return -1;
        }
        userStore[i] = user; // store the address of the dynamic object to the userStore array.
    }
    /* Start calling simulation */
    int call_in_index = 0; // record the index of user's call according to the priority of time
    int timeDuration = 0; // record the duration of each user call
    for (int curTime = 0;; ++curTime) {
        if (timeDuration > 0){
            showTime(curTime);
            userCallIn(userStore,userNum,call_in_index,curTime);
            timeDuration --; // consume one tick; duration decreases by 1
            continue;
        }
        /// MUST check before normal turn begins ! Or if after popping, all queues become empty, then the last round won't be shown!
        if (platinum_queue.isEmpty() && gold_queue.isEmpty() && silver_queue.isEmpty() && regular_queue.isEmpty()){
            showTime(curTime);
            break;
        }
        else{ // normal round
            showTime(curTime);
            userCallIn(userStore,userNum,call_in_index,curTime);
            timeDuration = dealCall_oneQueue(&platinum_queue,curTime);
            if (timeDuration != -1){ // platinum user calling-in
                continue;
            }
            timeDuration = dealCall_oneQueue(&gold_queue,curTime);
            if (timeDuration != -1){ // gold user calling-in
                continue;
            }
            timeDuration = dealCall_oneQueue(&silver_queue,curTime);
            if (timeDuration != -1){ // silver user calling-in
                continue;
            }
            timeDuration = dealCall_oneQueue(&regular_queue,curTime);
            if (timeDuration != -1){ // regular user calling-in
                continue;
            }
            // no call in this time stamp
            timeDuration = 0;
        }
    }
    /* delete the dynamic object userInfo_t and userInfoPtr_t  */
    for (int i = 0; i < userNum; ++i) {
        delete userStore[i];
    }
    delete [] userStore;

    return 0;
}
