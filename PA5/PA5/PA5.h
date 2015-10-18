#ifndef PA5_H
#define PA5_H
#include <iostream>
#include <string>
// ListNode
using std::string;
using std::cout;
using std::to_string;

// Represent the date as day/month/year
//Date is defined here only. Nothing in source file.
class Date
{
public:
	Date() 
	{
		day = 0;
		month = 0;
		year = 0;
	}
	Date(int d, int m, int y)
	{
		day = d;
		month = m;
		year = y;
	}
	Date(Date &date) // Copy Constructor
	{
		day = date.day;
		month = date.month;
		year = date.year;
	}
	~Date() {};
	void print(void)
	{
		cout << month << "/" << day << "/" << year;
	}
	string date_to_string(void) // mm/dd/yyyy
	{
		string date = "";
		date.append(to_string(month));
		date.append("/");
		date.append(to_string(day));
		date.append("/");
		date.append(to_string(year));

		return date;
	}
	int day;
	int month;
	int year;
	bool operator== (const Date &rhs)
	{
		return ((day == rhs.day) &&
			(month == rhs.month) &&
			(year == rhs.year));
	}
	void operator &= (const Date &rhs)
	{
		day = rhs.day;
		month = rhs.month;
		year = rhs.year;
	}
	void operator &= (int) // Want to zero out the Date fields.
	{
		day = 0;
		month = 0;
		year = 0;
	}
};

class List;
class ListNode
{
	friend List;
public:
	ListNode();
	// Copy constructor
	ListNode(ListNode &newNode);
	~ListNode();
	void add_date(Date x, int y);
private:

	int m_record_num;
	int m_ID_num;
	string m_email;
	string m_name;
	int m_num_credits; // 0 stands for AU or audit, i.e. no credit.
	string m_program;
	string m_level;
	int m_num_absences;
	Date m_dates[100];
	ListNode *pNext; // Pointer to next node
};

class List
{
public:
	List();
	~List();
	void ImportRecords(string record_name);
	void SaveRecords(string record_name);
	void MarkAbsence(void);
	void GenerateReport(void);
	void GenerateReport(int num_absences);
	void GenerateReport(Date date);
private:
	ListNode *pHead; // Pointer to the head of the records list.
	int m_num_nodes; // Just to keep track of the number of student records
};

class Menu
{
public:
	Menu() {}
	~Menu() {}

	void printMainMenuChoice();
	void printGenerateReportMenu();
	void setMenuChoice(int choice);
	
	int m_menu_choice;
};

// Helper functions




#endif // PA5_H
