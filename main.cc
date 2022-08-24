// g++ main.cc -l sqlite3
// ./a.out

/** TODO 
 * Support optional max players 
 * Printing by genre, num players, etc 
 * Add help, that prints all options 
 */

#include <iostream>
#include <sqlite3.h>
  
using namespace std;

static int callback(void* data, int argc, char** argv, char** azColName)
{
    int i;
    fprintf(stderr, "%s: ", (const char*)data);
  
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
  
    printf("\n");
    return 0;
}

string quotesql( const string& s ) {
    return string("'") + s + string("'");
}

int main(int argc, char** argv)
{
    // Variables for sql database
    sqlite3* DB;
    sqlite3_stmt * st;  
    char* messaggeError;
    string data("CALLBACK FUNCTION"); 
    int action_succes = 0; 

    string user_input;
    while(1){
       cout<<"Enter command: ";
        cin>>user_input;
        if (user_input == "create"){
            string user_name;
            cout<<"Account Name: ";
            cin>>user_name;
            string db_name = user_name + ".db";
            action_succes = sqlite3_open(db_name.c_str(), &DB);
            if (action_succes != SQLITE_OK) {
                cout<<"Error creating account"<<std::endl;
            }
            string sql = "CREATE TABLE GAMES(" \
                      "NAME           TEXT    NOT NULL, " \
                      "GENRE          TEXT     NOT NULL, " \
                      "MIN_PLAYERS     INT     NOT NULL, " \
                      "MAX_PLAYERS     INT      NOT NULL, " \
                      "AVG_GAME_LENGTH  INT   NOT NULL );";

            action_succes = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
            if (action_succes != SQLITE_OK) {
                cout<<"Error creating database"<<std::endl;
                sqlite3_free(messaggeError);
            } else {
                cout<< "Table created successfully"<<std::endl;
            }
        } else if (user_input == "open"){
            string user_name;
            cout<<"Profile Name: ";
            cin>>user_name;
            string db_name = user_name + ".db";
            action_succes = sqlite3_open(db_name.c_str(), &DB);
            if (action_succes != SQLITE_OK) {
                cout<<"Error openning profile"<<std::endl;
            }else{
                cout<<"Profile opened sucessful"<<std::endl;
            }
        } else if (user_input == "quit"){
             cout<<"bye!"<<std::endl;
            sqlite3_close(DB);
            return 0;
        } else if (user_input == "print") {
            string print_what; 
            cin>>print_what;
            if(print_what == "all"){
               string sql = "SELECT * FROM GAMES;";
                action_succes = sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);
            }else if (print_what == "party" || print_what == "word" ||print_what == "trivia" ||
                        print_what == "strategy" || print_what == "children" || print_what == "card" || print_what == "deduction"){
                string sql = "SELECT NAME FROM GAMES WHERE genre IN ("+ quotesql(print_what) + ");";
                action_succes = sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);
            }else if(print_what == "num_players"){ 
                int num_players;
                cin>>num_players;
                string sql = "SELECT NAME FROM GAMES WHERE " + quotesql(to_string(num_players)) + " BETWEEN MIN_PLAYERS AND MAX_PLAYERS;"; 
                action_succes = sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);
            }else{
                //print num plauers and num times and individual games and num length
            }
            if (action_succes != SQLITE_OK) {
                cout<<"Error printing database"<<std::endl;
            }
        } else if (user_input == "add"){
            string game_name, genre;
            int min_players, max_players, length;
            cout<<"Game Name: ";
            cin>>game_name;
            cout<<"Game Genre [party, word, trivia, strategy, children, card, deduction]: ";
            cin>>genre;
            cout<<"Min Players: ";
            cin>>min_players;
            cout<<"Max Players: ";
            cin>>max_players;
            cout<<"Game Length (minutes): ";
            cin>>length;

            string sql = "INSERT INTO GAMES VALUES (" 
                            + quotesql(game_name) + ","+ quotesql(genre) + "," + quotesql(to_string(min_players)) + "," + quotesql(to_string(max_players)) + "," + quotesql(to_string(length)) + ");";

            action_succes = sqlite3_exec(DB, sql.c_str(), callback, 0, &messaggeError);            
            if (action_succes != SQLITE_OK) {
                cout<<"Error inserting"<<std::endl;
                 sqlite3_free(messaggeError);
            }
        } else if(user_input == "help"){
            std::cout<<"Commands:"<<std::endl;
            std::cout<<"    create -> creates a new profile/databse for you"<<std::endl;
            std::cout<<"    open   -> opens an existing profile/database for you "<<std::endl;
            std::cout<<"    quit   -> close the game chooser application "<<std::endl;
            std::cout<<"    add    -> add a game into the database "<<std::endl;
            std::cout<<"    print  "<<std::endl;
            std::cout<<"            all  -> outputs all games in the database "<<std::endl;
            std::cout<<"            [genre]  -> outputs all games in the database that have a matching genre "<<std::endl;

        } else {
            std::cout<<"Unrecognized Command"<<std::endl;
        }
         cout<<"_______________________________________________________"<<std::endl;
    }
    return (0);
}