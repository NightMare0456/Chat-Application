#include <iostream>
#include <string>
using namespace std;


const int MAX_USERS = 100;
const int MAX_CHATS = 100;

struct MessageNode {
    string text;
    MessageNode* next;
    MessageNode(string msg) : text(msg), next(nullptr) {}
};

class ChatHistory {
    MessageNode* head = nullptr;
    MessageNode* tail = nullptr;

public:
    void addMessage(const string& msg) {
        MessageNode* newNode = new MessageNode(msg);
        if (!head) head = tail = newNode;
        else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    void display() const {
        if (!head) {
            cout << "No messages yet.\n";
            return;
        }
        MessageNode* temp = head;
        while (temp) {
            cout << temp->text << endl;
            temp = temp->next;
        }
    }
};

class User {
public:
    string name;
    string contacts[MAX_CHATS];
    ChatHistory chats[MAX_CHATS];
    int chatCount = 0;

    User() : name("") {}
    User(string n) : name(n) {}

    void sendMessage(const string& toUser, const string& message) {
        int index = findContactIndex(toUser);
        if (index == -1 && chatCount < MAX_CHATS) {
            contacts[chatCount] = toUser;
            index = chatCount++;
        }
        chats[index].addMessage("You: " + message);
    }

    void receiveMessage(const string& fromUser, const string& message) {
        int index = findContactIndex(fromUser);
        if (index == -1 && chatCount < MAX_CHATS) {
            contacts[chatCount] = fromUser;
            index = chatCount++;
        }
        chats[index].addMessage(fromUser + ": " + message);
    }

    void viewChat(const string& withUser) const {
        int index = findContactIndex(withUser);
        if (index != -1) {
            cout << "Chat with " << withUser << ":\n";
            chats[index].display();
        } else {
            cout << "No chat history with " << withUser << ".\n";
        }
    }

private:
    int findContactIndex(const string& contactName) const {
        for (int i = 0; i < chatCount; i++) {
            if (contacts[i] == contactName)
                return i;
        }
        return -1;
    }
};

struct Group {
    string groupName;
    string members[MAX_USERS];
    int memberCount = 0;
    ChatHistory chat;

    Group() : groupName(""), memberCount(0) {}
    Group(string name) : groupName(name), memberCount(0) {}

    void addMember(const string& username) {
        if (memberCount < MAX_USERS) {
            members[memberCount++] = username;
        }
    }

    bool isMember(const string& username) const {
        for (int i = 0; i < memberCount; i++) {
            if (members[i] == username) return true;
        }
        return false;
    }
};

class ChatApp {
    User users[MAX_USERS];
    Group groups[MAX_CHATS];
    int userCount = 0;
    int groupCount = 0;

public:
    void createUser(const string& username) {
        if (findUserIndex(username) != -1) {
            cout << "Username already exists.\n";
            return;
        }
        if (userCount < MAX_USERS) {
            users[userCount++] = User(username);
            cout << "User '" << username << "' created.\n";
        } else {
            cout << "User limit reached.\n";
        }
    }

    void sendMessage(const string& from, const string& to, const string& msg) {
        int fromIndex = findUserIndex(from);
        int toIndex = findUserIndex(to);

        if (fromIndex == -1 || toIndex == -1) {
            cout << "One or both users not found.\n";
            return;
        }

        users[fromIndex].sendMessage(to, msg);
        users[toIndex].receiveMessage(from, msg);
        cout << "Message sent.\n";
    }

    void viewChat(const string& user, const string& withUser) {
        int userIndex = findUserIndex(user);
        if (userIndex == -1) {
            cout << "User not found.\n";
            return;
        }
        users[userIndex].viewChat(withUser);
    }

    void createGroup(const string& groupName, const string members[], int count) {
        if (groupCount >= MAX_CHATS) {
            cout << "Group limit reached.\n";
            return;
        }
        groups[groupCount] = Group(groupName);
        for (int i = 0; i < count; i++) {
            if (findUserIndex(members[i]) != -1) {
                groups[groupCount].addMember(members[i]);
            }
        }
        groupCount++;
        cout << "Group '" << groupName << "' created.\n";
    }

    void sendGroupMessage(const string& sender, const string& groupName, const string& message) {
        for (int i = 0; i < groupCount; i++) {
            if (groups[i].groupName == groupName) {
                if (!groups[i].isMember(sender)) {
                    cout << "You are not a member of this group.\n";
                    return;
                }
                groups[i].chat.addMessage(sender + " (Group): " + message);
                cout << "Group message sent.\n";
                return;
            }
        }
        cout << "Group not found.\n";
    }

    void viewGroupChat(const string& groupName) {
        for (int i = 0; i < groupCount; i++) {
            if (groups[i].groupName == groupName) {
                cout << "Chat in group '" << groupName << "':\n";
                groups[i].chat.display();
                return;
            }
        }
        cout << "Group not found.\n";
    }

private:
    int findUserIndex(const string& username) const {
        for (int i = 0; i < userCount; i++) {
            if (users[i].name == username)
                return i;
        }
        return -1;
    }
};

void showMenu() {
    cout << "\n=== Chat App ===\n";
    cout << "1. Create User\n";
    cout << "2. Send Message\n";
    cout << "3. View Chat\n";
    cout << "4. Create Group\n";
    cout << "5. Send Group Message\n";
    cout << "6. View Group Chat\n";
    cout << "7. Exit\n";
    cout << "Enter choice: ";
}

int main() {
    ChatApp app;
    int choice;
    string user1, user2, msg;

    while (true) {
        showMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                cout << "Enter username: ";
                getline(cin, user1);
                app.createUser(user1);
                break;
            case 2:
                cout << "From: ";
                getline(cin, user1);
                cout << "To: ";
                getline(cin, user2);
                cout << "Message: ";
                getline(cin, msg);
                app.sendMessage(user1, user2, msg);
                break;
            case 3:
                cout << "Your username: ";
                getline(cin, user1);
                cout << "View chat with: ";
                getline(cin, user2);
                app.viewChat(user1, user2);
                break;

            case 4: {
                cout << "Enter group name: ";
                getline(cin, user1);
                int memberCount;
                cout << "Enter number of members: ";
                cin >> memberCount;
                cin.ignore();
                string members[memberCount];
                for (int i = 0; i < memberCount; i++) {
                    cout << "Enter member " << i + 1 << ": ";
                    getline(cin, members[i]);
                }
                app.createGroup(user1, members, memberCount);
                break;
            }
            case 5:
                cout << "Sender: ";
                getline(cin, user1);
                cout << "Group name: ";
                getline(cin, user2);
                cout << "Message: ";
                getline(cin, msg);
                app.sendGroupMessage(user1, user2, msg);
                break;
            case 6:
                cout << "Enter group name: ";
                getline(cin, user1);
                app.viewGroupChat(user1);
                break;
            case 7:
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid choice.\n";
        }
    }
}
