#include <utility>

//
// Created by LBYPatrick on 2/6/2018.
//

#include "util.hpp"

const char B64_INDEX[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void util::RemoveLetter(string &original_string, char letter) {
    string temp_buffer;
    bool is_in_quote = false;
    for (char i : original_string) {
        if (i == '\"') {
            is_in_quote = !is_in_quote;
            continue;
        } else if (i != letter || is_in_quote) temp_buffer += i;
    }
    original_string = temp_buffer;
}

void util::ReportError(string message) {
    printf("[ERROR] %s\n", message.c_str());
}

void util::RemoveFile(string filename) {
    remove(filename.c_str());
}

void util::AppendStringVector(vector<string> &left, vector<string> &right) {
    left.insert(end(left), begin(right), end(right));
}

void util::ShowHelp(vector<TableElement> option_list) {
    size_t left_len = 0;

    vector<string> output = Make2DTable(std::move(option_list));

    for (auto &i : output) {
        printf("%s\n", i.c_str());
    }

}

void util::PercentageBar(int current, int total) {

    int barLength = 50;
    int leftPercent = double(current) / double(total) * barLength;
    int rightPercent = barLength - leftPercent;
    string print_buffer = "\r[";

    for (int i = 0; i < leftPercent - 1; i++) {
        print_buffer += "=";
    }

    print_buffer += ">";

    for (int i = 0; i < rightPercent; i++) {
        print_buffer += " ";
    }

    print_buffer += string("] ") + std::to_string(current) + string("/") + std::to_string(total);

    if (current == total) print_buffer += "\n";
    printf(print_buffer.c_str());
}

bool util::IsFileExist(string path) {

    FILE * handler;
    vector<string> r;
    string long_buffer;
    char buffer[READ_BUFFER_SIZE];

#ifdef _WIN32

    //Convert Unix backslashes to Windows slashes
    for (auto & i : path) {
        if (i == '/') {
            i = '\\';
        }
    }

    auto result = fopen_s(&handler, path.c_str(), "r");

    if (result == EINVAL) {
        return false;
    }
#else
    handler = fopen(path.c_str(), "r");

	if (!handler) {
		return false;
	}
#endif

    fclose(handler);
    return true;
}

vector<string> util::SysExecute(string cmd) {
    return SysExecute(cmd, true);
}

vector<string> util::SysExecute(string cmd, bool output) {

    FILE * handler;
    vector<string> r;
    string long_buffer;
    char buffer[READ_BUFFER_SIZE];

    handler = popen(cmd.c_str(), "r");

    if (handler && output) {

        if(DEBUG_CMDOUT) printf("Running %s\n", cmd.c_str());

        while (fgets(buffer, READ_BUFFER_SIZE, handler)) {

            if (strlen(buffer) == READ_BUFFER_SIZE && buffer[READ_BUFFER_SIZE - 1] != '\n') {
                long_buffer += buffer;
            }
            else if (buffer[strlen(buffer) - 1] == '\n') {
                r.push_back(long_buffer + string(buffer).substr(0, strlen(buffer) - 1));
                long_buffer = "";
            }
            else {
                r.push_back(long_buffer + string(buffer));
                long_buffer = "";
            }
        }
        r.push_back(long_buffer);
    }
    pclose(handler);

    return r;
}

vector<string> util::ReadFile(string path) {
    return util::ReadFile(std::move(path), true);
}

vector<string> util::ReadFile(string path, bool is_parsed) {

    FILE * handler;
    vector<string> r;
    string long_buffer;
    char buffer[READ_BUFFER_SIZE];

#ifdef _WIN32

    //Convert Unix backslashes to Windows slashes
    for (auto & i : path) {
        if (i == '/') {
            i = '\\';
        }
    }

    auto result = fopen_s(&handler, path.c_str(), "r");

    if (result == EINVAL) {
        return r;
    }
#else
    handler = fopen(path.c_str(), "r");

	if (!handler) {
	    if(DEBUG_IO) printf("Failed to read file \"%s\" \n", path.c_str());
		return r;
	}
#endif

    while (fgets(buffer, READ_BUFFER_SIZE, handler)) {

        if (strlen(buffer) == READ_BUFFER_SIZE && buffer[READ_BUFFER_SIZE - 1] != '\n') {
            long_buffer += buffer;
        }
        else if (buffer[strlen(buffer) - 1] == '\n') {
            r.push_back(long_buffer + string(buffer).substr(0, strlen(buffer) - 1));
            long_buffer = "";
        }
        else {
            r.push_back(long_buffer + string(buffer));
            long_buffer = "";
        }
    }

    if(!long_buffer.empty()) r.push_back(long_buffer);
    fclose(handler);

    return r;
}


int util::MatchWithWords(string str, vector<string> match_list, bool precise) {
    for (int i = 0; i < match_list.size(); ++i) {

        if (precise && match_list[i] == str) {
            return i;
        } else if (!precise && str.find(match_list[i]) != string::npos) {
            return i;
        }
    }

    return -1;
}

int util::MatchWithWords(string str, vector<string> match_list) {
    return MatchWithWords(str, match_list, false);
}

YAML util::DecodeYamlLine(string line) {
    YAML out;

    if (line.find(':') == -1) {
        return YAML();
    }

    out.level = line.find_first_not_of(' ');
    out.left = SubString(line, out.level, line.find(':'));
    out.right = SubString(line, line.find(':') + 1, line.find_last_not_of(' ') + 1);

    //Remove Trailing & starting spaces
    out.left = SubString(out.left, 0, out.left.find_last_not_of(' ') + 1);
    out.right = SubString(out.right, out.right.find_first_not_of(' '), out.right.size());


    if (out.left.find('\"') != -1) {
        out.left = SubString(out.left, 1, out.left.size() - 1);
    }

    if (out.right.find('\"') != -1) {
        out.right = SubString(out.right, 1, out.right.size() - 1);
    }

    return out;
}

string
util::SubString(string str, size_t left, size_t stop) {
    size_t length = stop - left;

    //Out-of-bound fix
    if (left < 0) left = 0;
    if (stop > str.size()) stop = str.size();

    return str.substr(left, length);
}

string util::GetMachineIP() {
    vector<string> result = SysExecute(
            R"(ip address | sed -En 's/127.0.0.1//;s/.*inet (addr:)?(([0-9]*\.){3}[0-9]*).*/\2/p')");
    return !result.empty() ? result[0] : string();
}


bool util::IsProcessAlive(string pid) {

    vector<string> cmd_buffer = SysExecute("ps -p " + pid);

    return cmd_buffer.size() > 1;
}

bool util::IsTheSame(string str, string key, bool is_precise, bool is_case_sensitive) {
    if (!is_case_sensitive) {
        for (char &i : str) {
            i = toupper(i);
        }
        for (char &i : key) {
            i = toupper(i);
        }
    }

    if (is_precise) {
        return str == key;
    } else {
        return (str.find(key) != -1) || (key.find(str) != -1);
    }
}

vector<string>
util::GetFileList(string directory) {

    vector<string> console_buffer = SysExecute("ls -p " + directory + " -1");

    vector<string> output_buffer;

    for (string &line : console_buffer) {

        if (line.find_last_of('/') == -1 && line.find("output.data") == -1) {
            output_buffer.push_back(line);
        }
    }

    return output_buffer;
}

vector<string> util::GetFolderList(string directory) {

    vector<string> console_buffer = SysExecute("ls -p " + directory + " -1");
    vector<string> output_buffer;

    for (string &line : console_buffer) {

        if (line.find_last_of('/') != -1) {
            output_buffer.push_back(SubString(line, 0, line.size() - 1));
        }

    }

    return output_buffer;
}

vector<string> util::GetFolderList() {
    return GetFolderList("./");
}

vector<string> util::GetFileList() {
    return GetFileList("./");
}

bool util::WriteFile(string filename, vector<string> content) {

    FILE*handler;

#ifdef _WIN32

    //Convert Unix backslashes to Windows slashes
    for (auto & i : filename) {
        if (i == '/') {
            i = '\\';
        }
    }

    auto result = fopen_s(&handler, filename.c_str(), "w");

    if (result == EINVAL) return false; //Fail to open file
#else

    handler = fopen(filename.c_str(), "w");

	if (!handler) {
	    if(DEBUG_IO) printf("Failed to write file \"%s\" \n",filename.c_str());
	    return false;
	} //Fail to open file
#endif

    string write_buffer;

    for (auto & i : content) {
        write_buffer += i + '\n';
    }

    fputs(write_buffer.c_str(), handler);
    fclose(handler);

    return true;
}

vector<size_t> util::SearchString(string str, char key) {
    return SearchString(str, key, 0, str.size());
}

vector<size_t> util::SearchString(string str, char key, size_t left, size_t stop) {

    size_t length = stop - left;
    vector<size_t> r;

    for (size_t i = left; i < stop; i += 1) {
        if (str[i] == key) {
            r.push_back(i);
        }
    }

    return r;
}

vector<string> util::Make2DTable(vector<TableElement> table) {


    size_t left_len = 0;
    vector<string> r;
    string line;

    //Get max l_element length for better formatting
    for (auto &i : table) {
        if (i.l_element.length() > left_len) {
            left_len = i.l_element.length();
        }
    }

    //Format Output based on left_len
    for (auto &i : table) {
        line = i.l_element;

        for (size_t n = i.l_element.length(); n < left_len; ++n) {
            line += ' ';
        }

        line += " : ";
        line += i.r_element;
        r.push_back(line);
    }

    return r;
}

void util::PrintLines(vector<string> arr) {
    for (auto &i : arr) {
        printf("%s\n", i.c_str());
    }
}

string asc2b64_seg(char c1, char c2 = 0, char c3 = 0) {
    unsigned int buff = (c1 << 24) + (c2 << 16) + (c3 << 8);
    string out;
    for (int i = 0; i < 4; i++) {
        //printf("%lu, %u\n", sizeof(unsigned int), buff >> 26);
        out += B64_INDEX[buff >> 26];
        buff <<= 6;
    }
    return out;
}

string util::GetEncodedBase64(string ascii) {
    string base64;
    while (ascii.length() > 2) {
        base64 += asc2b64_seg(ascii[0], ascii[1], ascii[2]);
        ascii = ascii.substr(3);
    }
    if (ascii.length() == 1) {
        base64 += asc2b64_seg(ascii[0]);
        base64 = base64.substr(0, base64.length() - 2) + "==";
    }

    if (ascii.length() == 2) {
        base64 += asc2b64_seg(ascii[0], ascii[1]);
        base64 = base64.substr(0, base64.length() - 1) + "=";
    }
    return base64;
}

void util::QuickSort::Sort(vector<SortItem> &arr, size_t low, size_t high) {
    if (low < high) {
        size_t pivot = Partition(arr, low, high);
        Sort(arr, low, pivot);
        Sort(arr, pivot + 1, high);
    }
}

/**
 * Overloaded Sort method #1: When user passes a raw size_t array with bounds
 * @param arr
 * @return vector containing indexes of sorted items
 */

vector<size_t> util::QuickSort::Sort(vector<long long> &arr, size_t low, size_t high) {

    auto * new_arr = new vector<SortItem>();

    vector<size_t> r;

    //Reserve space for efficiency
    new_arr->reserve(high - low + 1);
    r.reserve(high - low + 1);

    for (size_t i = low; i <= high; ++i) {
        new_arr->push_back({i, arr[i]});
    }

    Sort(*new_arr, low, high);

    for (auto &element : *new_arr) {
        r.push_back(element.old_index);
    }

    delete new_arr;

    return r;
}

/**
 * Overloaded Sort method #1: When user passes a raw size_t array without bounds
 * @param arr
 * @return vector containing indexes of sorted items
 */
vector<size_t> util::QuickSort::Sort(vector<long long> &arr) {
    return Sort(arr, 0, arr.size() - 1);
}

/**
 * Sort using Hoare Partition Scheme
 * @param arr  Array (By reference)
 * @param low  lower index.
 * @param high High index.
 * @return Index of pivot
 */
size_t util::QuickSort::Partition(vector<SortItem> &arr, size_t low, size_t high) {
    long long pivot = arr[low].key;
    size_t left_index = low - 1,
            right_index = high + 1;

    while (true) {
        do {
            left_index += 1;
        } while (arr[left_index].key < pivot);

        do {
            right_index -= 1;
        } while (arr[right_index].key > pivot);

        if (left_index >= right_index) {
            return right_index;
        }

        std::swap(arr[left_index], arr[right_index]);
    }
}
