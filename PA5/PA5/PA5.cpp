#define _CRT_SECURE_NO_WARNINGS 
#include "PA5.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <time.h>

using namespace std;
// Define ListNode Class Methods

ListNode::ListNode()
{
	pNext = NULL;
}

// Copy Constructor not sure why I made this.
ListNode::ListNode(ListNode &newNode)
{
	m_record_num = newNode.m_record_num;
	m_ID_num = newNode.m_ID_num;
	m_email = newNode.m_email;
	m_name = newNode.m_name;
	m_num_credits = newNode.m_num_absences; // 0 stands for AU or audit, i.e. no credit.
	m_program = newNode.m_program;
	m_level = newNode.m_level;
	m_num_absences = newNode.m_num_absences;
	pNext = newNode.pNext;

	for (int i = 0; i < 100; i++)
	{
		m_dates[i] = newNode.m_dates[i];
	}
}

ListNode::~ListNode() 
{
}

// Define List class methods
List::List()
{
	pHead = NULL;
	m_num_nodes = 0;
}
List::~List()
{
	ListNode *pTemp;

	if (pHead != NULL) // When List goes out of scope delete all the ListNode memory created.
	{
		while (pHead->pNext != NULL)
		{
			pTemp = pHead;
			pHead = pTemp->pNext;
			delete pTemp;
		}

		delete pHead;
		pHead = NULL;
	}	
}
void ListNode::add_date(Date date, int idx)
{
	m_dates[idx] = date;
}
// This works with the original .csv or the saved .csv
void List::ImportRecords(string record_name)
{
	ifstream infile(record_name); // for example
	string line = "";
	if (infile)
	{
		getline(infile, line); // Throw away first line.
	}
	while (getline(infile, line)) 
	{
		ListNode *newNode = new ListNode();
		stringstream strstr(line);
		string word = "";

		// Split 7 things
		int count = 1;
		int date_count = 1;
		Date date;
		string newName = "";
		int name_count = 1;
		int date_idx = 0; // Date index for adding saved records
		while (getline(strstr, word, ',')) 
		{			
			switch (count)
			{
			case 1:
				//Throw number way
				break;
			case 2: // Student ID
				newNode->m_ID_num = stoi(word);
				break;
			case 3: // Need to grab name which stupidly has a comma in it. Not smart but not our fault.
			{
				stringstream new_name(word);
				string strName = "";
				while (getline(new_name, strName, '\"'))
				{
					newName.append(strName);
					if (strName != "")
					{
						if (name_count)
						{
							newName.append(", ");
							name_count = 0;
							count--; // Set up to grab last name
						}
					}
				}
				if (count == 3)
				{
					newNode->m_name = newName; // Add built up name to node.
				}
			}
				break;
			case 4:
				newNode->m_email = word;
				break;
			case 5:
				if (word == "AU")
				{
					newNode->m_num_credits = 0;
				}
				else
				{
					newNode->m_num_credits = stoi(word);
				}
				break;
			case 6:
				newNode->m_program = word;
				break;
			case 7:
				newNode->m_level = word;
				break;
			case 8:
				newNode->m_num_absences = stoi(word);
				break;
			default: // Dates
				if (date_count == 1) // Day
				{
					date.month = stoi(word);
				}
				else if (date_count == 2) // Month
				{
					date.day = stoi(word);
				}
				else if (date_count == 3) // Year
				{
					date.year = stoi(word); 
					// Now add completed date to new Node
					
					if (newNode->m_num_absences > 0 && date_idx != newNode->m_num_absences)
					{
						newNode->add_date(date, date_idx);
						date_idx++;
					}
					
					// Reset date count for next three numbers for next date.
					date_count = 0;
				}
				date_count++;
				break;
			}
			count++;
		}
		// Add newly made node list
		if (pHead == NULL)
		{
			pHead = newNode;
		}
		else
		{
			newNode->pNext = pHead;
			pHead = newNode;
		}
		m_num_nodes++;
	}

}

void List::SaveRecords(string record_name)
{
	ofstream outfile(record_name);

	// Need to insert the same line that was in original csv.
	// It actually can be anything but best if same line so makes sense when read.
	string firstline = ",ID,Name,Email,Units,Program,Level";
	outfile << firstline << endl;
	// Now start entering the comma delimenated strings
	ListNode *pCurrent = pHead;
	int count = 1;	
	while (pCurrent)
	{
		string line = "";
		// Build up line to print to file
		line.append(to_string(count));
		line.append(",");
		line.append(to_string(pCurrent->m_ID_num));
		line.append(",\"");
		line.append(pCurrent->m_name);
		line.append("\",");
		line.append(pCurrent->m_email);
		line.append(",");
		if (pCurrent->m_num_credits == 0)
		{ 
			line.append("AU");
		}
		else
		{
			line.append(to_string(pCurrent->m_num_credits));
		}
		line.append(",");
		line.append(pCurrent->m_program);
		line.append(",");
		line.append(pCurrent->m_level);
		line.append(",");
		line.append(to_string(pCurrent->m_num_absences));		
		for (int i = 0; i < pCurrent->m_num_absences; i++)
		{
			int m = pCurrent->m_dates[i].month;
			int d = pCurrent->m_dates[i].day;
			int y = pCurrent->m_dates[i].year;
			line.append(",");
			line.append(to_string(m));
			line.append(",");
			line.append(to_string(d));
			line.append(",");
			line.append(to_string(y));
		}

		outfile << line << endl;
		count++;
		pCurrent = pCurrent->pNext;
	}

	outfile.close();
}

void List::GenerateReport(void)
{
	ofstream outfile("Class_Record.txt");
	
	outfile << "Class record: " << endl;
	ListNode* pCurrent = pHead;
	outfile << "=========================" << endl;
	while (pCurrent)
	{		
		outfile << "Student: " << pCurrent->m_name << " ID: " << pCurrent->m_ID_num << endl;
		outfile << "Email: " << pCurrent->m_email << endl;
		outfile << "Major: " << pCurrent->m_program << "Class standing: " << pCurrent->m_level << endl;
		outfile << "Credits: " << pCurrent->m_num_credits << endl;
		outfile << "Total Absences: " << pCurrent->m_num_absences << endl;
		for (int i = 0; i < pCurrent->m_num_absences; i++)
		{
			outfile << pCurrent->m_dates[i].date_to_string();
			outfile << ", " << endl;
		}
		outfile << "=========================" << endl;
		
		pCurrent = pCurrent->pNext;
	}

	outfile.close();
}

void List::GenerateReport(int num_absences)
{
	ofstream outfile("Class_Record_Absences.txt");
	
	outfile << "Class record for students with at least " << num_absences << "absences." << endl;
	ListNode* pCurrent = pHead;
	outfile << "=========================" << endl;
	while (pCurrent)
	{ 
		if (pCurrent->m_num_absences >= num_absences)
		{
			
			outfile << "Student: " << pCurrent->m_name << " ID: " << pCurrent->m_ID_num << endl;
			outfile << "Email: " << pCurrent->m_email << endl;
			outfile << "Major: " << pCurrent->m_program << "Class standing: " << pCurrent->m_level << endl;
			outfile << "Credits: " << pCurrent->m_num_credits << endl;
			outfile << "Total Absences: " << pCurrent->m_num_absences << endl;
			for (int i = 0; i < pCurrent->m_num_absences; i++)
			{
				outfile << pCurrent->m_dates[i].date_to_string();
				outfile << ", " << endl;
			}
			outfile << "=========================" << endl;
		}
		pCurrent = pCurrent->pNext;
	}

	outfile.close();
}

void List::GenerateReport(Date date)
{
	ofstream outfile("Class_Record_Date_Absences.txt");


	outfile << "Class record for students absence on " << date.date_to_string() << endl;
	
	ListNode* pCurrent = pHead;
	outfile << "=========================" << endl;
	while (pCurrent)
	{
		for (int i = 0; i < pCurrent->m_num_absences; i++)
		{
			if (pCurrent->m_dates[i] == date)
			{
				outfile << "Student: " << pCurrent->m_name << " ID: " << pCurrent->m_ID_num << endl;
				outfile << "Email: " << pCurrent->m_email << endl;
				outfile << "Major: " << pCurrent->m_program << "Class standing: " << pCurrent->m_level << endl;
				outfile << "Credits: " << pCurrent->m_num_credits << endl;
				outfile << "Total Absences: " << pCurrent->m_num_absences << endl;
				outfile << "=========================" << endl;
				break;
			}		
		}		
		pCurrent = pCurrent->pNext;
	}

	outfile.close();
}

void List::MarkAbsence(void)
{
	ListNode *pCurrent = pHead;
	while (pCurrent)
	{
		cout << "Was " << pCurrent->m_name << " absent today? " << endl;
		string answer = "";
		cin >> answer;
		if (answer == "Y" || answer == "y" || answer == "Yes" || answer == "yes")
		{
			pCurrent->m_num_absences++;
			time_t now = time(0);
			tm *ltm = localtime(&now); // Fill now time_t object with time information;
			Date day(ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900); 
			int index = pCurrent->m_num_absences - 1;
			pCurrent->m_dates[index] = day;
		}
		pCurrent = pCurrent->pNext;
	}
}



// Menu options

void Menu::printMainMenuChoice()
{

}
void Menu::printGenerateReportMenu()
{

}
void Menu::setMenuChoice(int choice)
{

}


