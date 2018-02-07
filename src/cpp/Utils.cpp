//
// Created by LBYPatrick on 2/6/2018.
//

#include "Utils.h"

void Utils::RemoveLetter(string & original_string, char letter) {
    string temp_buffer;

    for(int i = 0; i < original_string.size(); ++i) {
        if(original_string[i] != letter) temp_buffer += original_string[i];
    }
    original_string = temp_buffer;
}
