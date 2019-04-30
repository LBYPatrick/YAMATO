//
// Created by LBYPatrick on 2/6/2018.
//

#pragma once
#include "includes.hpp"

namespace util {

	//Util Configuration
	void SetVisualizing(bool is_enable);

	//File I/O
	bool PrintFile(YFile file);
	bool PrintFile(YFile file, bool force_output);
	bool DirectWriteFile(YFile file, string target_path);
	void RemoveFile(string filename);
	bool IsFileExist(string path);
	bool GetNextValidLine(ifstream& i, string& buffer, FileFilter& filter);
	bool IsPathReady(bool is_read, string path);
	bool IsLineVaild(string& line, FileFilter filter);
	void PushToDeleteQueue(string& filename);
	void FlushDeleteQueue();
	vector<string>& GetDeleteQueue();
	vector<string> ReadFile(string path);							//Deprecated, Overloaded
	vector<string> ReadFile(string path, FileFilter filter);	    //Overloaded
	bool WriteFile(string filename, vector<string> content);        //(Almost)Deprecated

	//String Utility
    void RemoveLetter(string & original_string, char letter);
	void AppendStringVector(vector<string>& left, vector<string>& right);
	bool AppendDuplicateString(string& str, string add, int num);
	bool GetRandomString(string& buffer, int len);
	bool IsTheSame(string str, string key, bool is_precise, bool is_case_sensitive);
	vector<string> Make2DTable(vector<TableElement> table);
	string SubString(string str, size_t left, size_t stop);
	int MatchWithWords(string str, vector <string> match_list);	  //Overloaded
	int MatchWithWords(string str, vector <string> match_list, bool precise);
	vector <size_t> SearchString(string str, char key);			  //Overloaded
	vector <size_t> SearchString(string str, char key, size_t left, size_t stop);

	//Math tools
	string TruncateDouble(double value, int decimal);

	//System Clock
	std::chrono::duration<double> GetElaspedTime();
	double GetElapsedSeconds();

	//Console Output
	void ReportError(string message);
	void PrintLines(vector<string>& arr);                              //Overloaded
	void PrintLines(vector<string> & arr, bool force_output);
	void Print(string str);                                            //Overloaded
	void Print(string str, bool force_output);
    void ShowHelp(vector<TableElement> option_list);
	void PercentageBar(int current, int total);
	void ReportEvent(string msg, bool force_output);                   //Overloaded
	void ReportEvent(string msg, bool force_output, bool newline);

	//System Interaction
    bool IsProcessAlive(string pid);
	vector<string> SysExecute(string cmd);                             //Overloaded
	vector<string> SysExecute(string cmd, bool output);
	vector<string> GetFileList();									   //Overloaded
	vector<string> GetFileList(string directory);
	vector<string> GetFolderList();									   //Overloaded
	vector<string> GetFolderList(string directory);
	string GetMachineIP();


	//Base64
	string GetEncodedBase64(string ascii);

	//YAML
	YAML DecodeYamlLine(string line);
	
	//Quick Sort
	class QuickSort {
	public:

		static void Sort(vector<SortItem>& arr, size_t low, size_t high);
		static vector<size_t> Sort(vector<long long>& arr, size_t low, size_t high, bool is_increasing_order);
		static vector<size_t> Sort(vector<long long>& arr, size_t low, size_t high);
		static vector<size_t> Sort(vector<long long>& arr);
		static vector<size_t> Sort(vector<long long>& arr, bool is_increasing_order);

	private:
		static size_t Partition(vector<SortItem> &arr, size_t low, size_t high);
	};

};