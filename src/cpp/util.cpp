#include <utility>

//
// Created by LBYPatrick on 2/6/2018.
//

#include "util.hpp"

const char B64_INDEX[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
bool is_visualizing_ = true;
vector<string> delete_queue_;

void util::RemoveLetter(string &original_string, char letter) {
	string temp_buffer;
	bool is_in_quote = false;
	for (char i : original_string) {
		if (i == '\"') {
			is_in_quote = !is_in_quote;
			continue;
		}
		else if (i != letter || is_in_quote) temp_buffer += i;
	}
	original_string = temp_buffer;
}

void util::ReportError(string message) {
	cout << "[ERROR]" << message <<"\n";
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

	if (!is_visualizing_) return;

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
	Print(print_buffer);
}

void util::SetVisualizing(bool isEnable) {
	is_visualizing_ = isEnable;
}

bool util::IsFileExist(string path) {
	ifstream i(path);
	return i.is_open();
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

		if (DEBUG_CMDOUT) printf("Running %s\n", cmd.c_str());

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
		if (!long_buffer.empty()) r.push_back(long_buffer);
	}
	pclose(handler);

	return r;
}

vector<string> util::ReadFile(string path) {
	return ReadFile(path, FileFilter());
}

vector<string> util::ReadFile(string path, FileFilter filter) {

	ifstream i;
	string line_buffer;
	bool has_filter = (!filter.key.empty());
	vector<string> r;

	i.open(path);

	if (i.is_open()) {
		if (has_filter) {
			while (GetNextValidLine(i, line_buffer, filter)) {
				r.push_back(line_buffer);
			}
		}
		else {
			while (getline(i, line_buffer)) {
				r.push_back(line_buffer);
			}
		}
	}

	return r;
}


int util::MatchWithWords(string str, vector<string> match_list, bool precise) {
	for (int i = 0; i < match_list.size(); ++i) {

		if (precise && match_list[i] == str) {
			return i;
		}
		else if (!precise && str.find(match_list[i]) != string::npos) {
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

	//size_t length = stop - left;

	if (left > stop) {
		std::swap(left, stop);
	}

	//Out-of-bound fix
	if (left < 0) left = 0;
	if (stop > str.size()) stop = str.size();

	return str.substr(left, stop - left);
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

bool util::IsPathReady(bool is_read, string path) {

	if (is_read) {
		ifstream i(path);
		return i.is_open();
	}
	else {
		ofstream o(path);
		return o.is_open();
	}
}

bool util::GetRandomString(string & buffer, int len) {

	buffer = string();
	buffer.reserve(len);

	for (int i = 0; i < len; ++i) {
		int rand_index = rand() % 62;

		buffer += B64_INDEX[rand_index];
	}
	
	return true;
}

bool util::IsTheSame(string str, string key, bool is_precise, bool is_case_sensitive) {

	if (!is_case_sensitive) {
		for (char &i : str) {
			i = (char)toupper(i);
		}
		for (char &i : key) {
			i = (char)toupper(i);
		}
	}

	if (is_precise) {
		return str == key;
	}
	else {
		return (str.find(key) != -1) || (key.find(str) != -1);
	}
}

bool util::IsLineVaild(string & line, FileFilter filter) {

	return ((line.find(filter.key) != -1) == filter.is_include);
}

bool util::GetNextValidLine(ifstream & i, string & buffer, FileFilter & filter) {

	if (!i.is_open()) return false;

	while (getline(i, buffer)) {
		if (IsLineVaild(buffer, filter)) return true;
	}

	return false;
}

void util::PushToDeleteQueue(string & filename) {
	delete_queue_.push_back(filename);
}

void util::FlushDeleteQueue() {

	for (auto & i : delete_queue_) {
		RemoveFile(i);
	}

	delete_queue_.clear();
}

vector<string> & util::GetDeleteQueue()
{
	return delete_queue_;
}

vector<string> util::GetFileList(string directory) {

	vector<string> console_buffer = SysExecute("ls -p " + directory + " -1");

	vector<string> output_buffer;

	for (string &line : console_buffer) {

		if (line.find_last_of('/') == -1) {
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
		if (DEBUG_IO) printf("Failed to write file \"%s\" \n", filename.c_str());
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

void util::PrintLines(vector<string> & arr) {

	if (!is_visualizing_) return;

	for (auto &i : arr) {
		cout << i + "\n";
	}
}

bool util::PrintFile(YFile file) {
	ifstream i(file.filename);
	string read_buffer;

	if (!i.is_open()) return false;

	while (GetNextValidLine(i, read_buffer, file.filter)) {
		util::Print(read_buffer + "\n");
	}

	return true;

}

bool util::DirectWriteFile(YFile file, string target_path) {

	ifstream i(file.filename);
	ofstream o(target_path);
	string read_buffer;

	if (!i.is_open() || !o.is_open()) return false;

	while (GetNextValidLine(i, read_buffer, file.filter)) {
		o << read_buffer + "\n";
	}

	return true;
}

void util::Print(string str) {
	if (!is_visualizing_) return;

	cout << str;
}

string asc2b64_seg(char c1, char c2 = 0, char c3 = 0) {
	unsigned int buff = (c1 << 24) + (c2 << 16) + (c3 << 8);
	string out;
	for (int i = 0; i < 4; i++) {
		out += B64_INDEX[buff >> 26];
		buff <<= 6;
	}
	return out;
}

string util::GetEncodedBase64(string ascii) {
	string base64;
	size_t left = 0;
	size_t right = ascii.length();

	while ((right - left) > 2) {
		base64 += asc2b64_seg(ascii[left], ascii[left + 1], ascii[left + 2]);
		left += 3;
	}
	if ((right - left) == 1) {
		base64 += asc2b64_seg(ascii[left]);
		base64[base64.length() - 1] = '=';
		base64[base64.length() - 2] = '=';
	}

	if ((right - left) == 2) {
		base64 += asc2b64_seg(ascii[left], ascii[left + 1]);
		base64[base64.length() - 1] = '=';
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

	vector<SortItem> new_arr;

	vector<size_t> r;

	//Reserve space for efficiency
	new_arr.reserve(high - low + 1);
	r.reserve(high - low + 1);

	for (size_t i = low; i <= high; ++i) {
		new_arr.push_back({ i, arr[i] });
	}

	Sort(new_arr, low, high);

	for (auto &element : new_arr) {
		r.push_back(element.old_index);
	}

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

bool util::AppendDuplicateString(string & str, string & add, size_t num) {

	for (size_t i = 0; i < num; ++i) {
		str += add;
	}

	return true;
}