//
// Created by LBYPatrick on 2/6/2018.
//

#include "Utils.h"

void Utils::RemoveLetter(string & original_string, char letter) {
    string temp_buffer;
    bool   is_in_quote = false;
    for (char i : original_string) {
        if(i == '\"') {
            is_in_quote = !is_in_quote;
            continue;
        }
        else if(i != letter || is_in_quote) temp_buffer += i;
    }
    original_string = temp_buffer;
}

Json Utils::GetJson(string raw_json_line) {

    Json return_buffer;

    RemoveLetter(raw_json_line, ' ');
    RemoveLetter(raw_json_line, ',');
    RemoveLetter(raw_json_line, ';');

    int start_position = 0;
    int end_position = raw_json_line.find_first_of(':') -1;

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
    printf("[ERROR] %s\n", message.c_str());
}

void Utils::RemoveFile(string filename) {
    remove(filename.c_str());
}

void Utils::ShowHelp(vector<Help> option_list) {
    int max_option_length = 0;

    string print_buffer;

    //Get max option length for better formatting
    for(Help current_help : option_list) {
        max_option_length = current_help.option.length() > max_option_length ?
                            current_help.option.length() : max_option_length;
    }

    for(Help current_help : option_list) {
        print_buffer += current_help.option;

        for(int i = current_help.option.length(); i < max_option_length; ++i) {
            print_buffer += ' ';
        }

        print_buffer += " : ";
        print_buffer += current_help.description + "\n";
    }

    printf("%s",print_buffer.c_str());

}

int Utils::StringToEnum(vector<string> fake_string_enum, string input) {
    for(int i = 0; i < fake_string_enum.size(); ++i) {
        if(input == fake_string_enum[i]) return i;
    }
    return -1;
}

bool Utils::IsFileExist(string filename) {

    ifstream reader;
    bool result;

    reader.open(filename);
    result = reader.is_open();
    reader.close();

    return result;

}

vector<string> Utils::SysExecute(string cmd) {

    ifstream reader;
    vector<string> return_buffer;
    string read_buffer;


    system((cmd + "> output.data").c_str());



    reader.open("output.data");

    while(getline(reader,read_buffer)) {
        return_buffer.push_back(read_buffer);
    }

    reader.close();
    RemoveFile("output.data");

    return return_buffer;

}