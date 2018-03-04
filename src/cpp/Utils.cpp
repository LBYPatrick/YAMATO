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

Json Utils::GetJson(string raw_json_line) {

    Json return_buffer;

    RemoveLetter(raw_json_line, ' ');
    RemoveLetter(raw_json_line, '\"');
    RemoveLetter(raw_json_line, ',');
    RemoveLetter(raw_json_line, ';');

    int start_position = 0;
    int end_position = raw_json_line.find_first_of(":") -1;

    //Get element
    for (int n = start_position; n <= end_position; ++n) {
        return_buffer.element += raw_json_line[n];
    }

    start_position = raw_json_line.find_first_of(":")+1;
    end_position = raw_json_line.length() - 1;

    //Get key
    for (int n = start_position; n <= end_position; ++n) {
        return_buffer.key += raw_json_line[n];
    }

    return return_buffer;
}

void Utils::ReportError(string message) {
    printf("[ERROR] %s", message.c_str());
}
