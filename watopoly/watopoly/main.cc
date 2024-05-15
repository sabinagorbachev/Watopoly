#include "board.h"
#include "square.h"
#include "game.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
using namespace std;

int main(int argc, char *argv[]) {
    bool invalid = false;
    bool testing = false; 
    bool loading = false;
    string file = "";
    
    if (argc > 4) {
      cout << "Error: Incorrect Number of Arguments" << endl;
      return 1;
    } else if (argc > 1) { 

        // If 1 argument -> has to be "-testing"
        if (argc == 2) { 
            string argv1 = string(argv[1]);
            if (argv1 == "-testing") {
                testing = true; 
                loading = false;
            } else invalid = true;

        
        } else if (argc > 2) {
            string argv1 = string(argv[1]); 
            string argv2 = string(argv[2]);
            if (argc == 3) { // If 2 arguments -> has to be "-load file"
                if (argv1 == "-load") {
                    file = argv2;
                    testing = false;
                    loading = true;
                } else invalid = true;
               
            
            } else if (argc == 4) { // If 3 arguments -> has to be "-load file" and "-testing"
                string argv3 = string(argv[3]);
                if (argv1 == "-testing" || argv3 == "-testing") {
                    testing = true; 
                    loading = false;
                    if (argv1 == "-load" || argv2 == "-load"){

                        if (argv1 == "-load") file = argv2;
                        else if ( argv2 == "-load") file = argv3;
                        loading = true;
                       
                    } else invalid = true;
                    
                } else invalid = true;
            }
        } 
    } 

    if (invalid) {
        cout << "Error: Invalid Arguments" << endl;
        return 1;
    }
    Game g = Game{testing, loading};
    
    if (loading) {
        ifstream game = ifstream{file};
        if (game.is_open()) g.load(game);
        else {
            cout << "Error: unable to open file" << endl;
            return 1;
        }
    }
    
    g.play();
    return 0; 
}
