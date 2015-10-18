#define _CRT_SECURE_NO_WARNINGS 
#include <fstream>
#include <iostream>
#include <string>
#include "PA5.h"

int main(void)
{
	List class_records;
	Menu menu;

	class_records.ImportRecords("records.csv");
	class_records.MarkAbsence();
	class_records.GenerateReport();
	class_records.MarkAbsence();
	class_records.GenerateReport(2);
	Date date(4, 4, 2015);
	class_records.GenerateReport(date);

	class_records.SaveRecords("SavedRecords.csv");
	List newClass;
	newClass.ImportRecords("SavedRecords.csv");
	newClass.GenerateReport();
	return 0;
}