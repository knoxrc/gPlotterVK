#pragma once
#include <iostream>
#include <string>
using namespace std;

class basicSensorDate {
protected:
	unsigned short int year;
	unsigned char month, day;
	bool initialized;
	static const unsigned short int min_year = 1900;
	// 
	void blankInitialization()
	{
		year = 0;
		month = 0;
		day = 0;
		initialized = false;
	}
	string year2string()const
	{
		string out;
		if (initialized)
			out = to_string(year);
		else
			out = "Not Initialized";
		return out;
	}
	string month2string()const
	{
		string out;
		switch (month) {
		case 1:
			out = "January";
			break;
		case 2:
			out = "February";
			break;
		case 3:
			out = "March";
			break;
		case 4:
			out = "April";
			break;
		case 5:
			out = "May";
			break;
		case 6:
			out = "June";
			break;
		case 7:
			out = "July";
			break;
		case 8:
			out = "August";
			break;
		case 9:
			out = "September";
			break;
		case 10:
			out = "October";
			break;
		case 11:
			out = "November";
			break;
		case 12:
			out = "December";
			break;
		default:
			out = "Not Initialized";
			break;
		}
		return out;
	}
	string day2string()const
	{
		string out;
		if (initialized)
			out = to_string(day);
		else
			out = "Not Initialized";
		return out;
	}
	bool checkDayAndMonth(unsigned char in_month, unsigned char in_day, unsigned short int in_year)const
	{
		bool result = false;
		char min_day = 1;
		char max_day;
		switch (in_month) {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			max_day = 31;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			max_day = 30;
			break;
		case 2:
			if (in_year % 4)
				max_day = 28;
			else
				max_day = 29;
			break;
		default:
			max_day = 0;
		}
		if ((in_day >= min_day) && (in_day <= max_day))
			result = true;
		return result;
	}
	bool checkAndSetYearMonthDay(unsigned short int in_year, unsigned char in_month, unsigned char in_day)
	{
		bool result = false;
		if (in_year >= min_year)
		{
			if ((in_month > 0) && (in_month < 13))
			{
				if (checkDayAndMonth(in_month, in_day, in_year))
				{
					year = in_year;
					month = in_month;
					day = in_day;
					initialized = true;
					result = true;
				}
			}
		}
		return result;


	}
public:
	basicSensorDate() { blankInitialization(); }
	~basicSensorDate() { blankInitialization(); }
	// copy constructor
	basicSensorDate(const basicSensorDate& other)
	{
		blankInitialization();
		copyFrom(other, true);
	}
	// getters (overloaded versions with strings for printing)
	bool getInit() const { return initialized; }
	bool getYear(unsigned short int& outVal)const
	{
		if (initialized)
		{
			outVal = year;
		}
		return initialized;
	}
	bool getYear(unsigned short int& outVal, string& outString)const
	{
		if (getYear(outVal))
		{
			outString = year2string();
		}
		return initialized;
	}
	bool getMonth(unsigned char& outVal)const
	{
		if (initialized)
		{
			outVal = month;
		}
		return initialized;
	}
	bool getMonth(unsigned char& outVal, string& outString)const
	{
		if (getMonth(outVal))
		{
			outString = month2string();
		}
		return initialized;
	}
	bool getDay(unsigned char& outVal)const
	{
		if (initialized)
		{
			outVal = day;
		}
		return initialized;
	}
	bool getDay(unsigned char& outVal, string& outString)const
	{
		if (getDay(outVal))
		{
			outString = day2string();
		}
		return initialized;
	}
	//setters
	bool setDate(unsigned short int in_year, unsigned char in_month, unsigned char in_day, bool allowOverwrite = false)
	{
		bool result = false;
		if (initialized && (!allowOverwrite))
			cout << "Error: Attempt to overwrite Date information" << endl;
		else
			result = checkAndSetYearMonthDay(in_year, in_month, in_day);
		return result;
	}
	void resetDate() { blankInitialization(); }
	// operators and helper functions

	// object to string conversion
	// FV_edit: 01 July 2022; make "convert2string" virtual,
	// to allow specialization for other date types. 
	virtual string convert2string(bool longform) const
	{
		unsigned short int out_year;
		unsigned char out_month, out_day;
		string out_day_name, out_month_name, out_year_name;
		string result;
		if (getYear(out_year, out_year_name))
		{
			if (longform)
			{
				result += "Year is: ";
				result += out_year_name;
				result += "; ";
			}
			else
			{
				result += out_year_name;
				result += "-";
			}

		}
		if (getMonth(out_month, out_month_name))
		{
			if (longform)
			{
				result += "Month is: ";
				result += out_month_name;
				result += "; ";
			}
			else
			{
				if (out_month < 10)
					result += "0";
				result += to_string(out_month);
				result += "-";
			}

		}
		if (getDay(out_day, out_day_name))
		{
			if (longform)
			{
				result += "Day is: ";
				result += out_day_name;
				result += ". ";
			}
			else
			{
				if (out_day < 10)
					result += "0";
				result += to_string(out_day);
				result += " ";
			}
		}
		return result;
	}
	operator string() const
	{
		bool option = false;
		string result = convert2string(option);
		return result;
	}
	// = operator	
	virtual void copyFrom(const basicSensorDate& other, bool allowOverwrite = false)
	{
		// avoid self copy
		if (this != &other)
		{	// only copy from an initialized object to a not-initialized one
			if ((other.getInit()) && (!this->getInit() || allowOverwrite))
			{
				unsigned short int in_year;
				unsigned char in_month, in_day;
				other.getYear(in_year);
				other.getMonth(in_month);
				other.getDay(in_day);
				setDate(in_year, in_month, in_day, true);
			}
		}
	}
	basicSensorDate& operator = (const basicSensorDate& other)
	{
		this->copyFrom(other, true);
		return *this;
	}
	// < operator
	virtual bool isSmallerThanOther(const basicSensorDate& other) const
	{
		unsigned short int other_year;
		unsigned char other_month, other_day;
		if (!getInit() || !other.getInit())
		{
			cout << "Error: trying to compare Dates with entries not initialized." << endl;
			return false;
		}
		other.getYear(other_year);
		other.getMonth(other_month);
		other.getDay(other_day);
		if (year > other_year)
			return false;
		if (year < other_year)
			return true;
		// same year:
		if (month > other_month)
			return false;
		if (month < other_month)
			return true;
		// same year and month:
		if (day < other_day)
			return true;
		return false;
	}
	bool operator < (const basicSensorDate& other) const
	{
		return isSmallerThanOther(other);
	}
	// == operator
	virtual bool isEqualToOther(const basicSensorDate& other) const
	{
		unsigned short int other_year;
		unsigned char other_month, other_day;
		if (!getInit() || !other.getInit())
		{
			cout << "Error: trying to compare Dates with entries not initialized." << endl;
			return false;
		}
		other.getYear(other_year);
		other.getMonth(other_month);
		other.getDay(other_day);
		if (year != other_year)
			return false;
		if (month != other_month)
			return false;
		if (day != other_day)
			return false;
		return true;
	}
	bool operator == (const basicSensorDate& other) const
	{
		return isEqualToOther(other);
	}
	// operators > , <= , >= , !=
	bool isLargerThanOther(const basicSensorDate& other) const
	{
		if (isSmallerThanOther(other))
			return false;
		if (isEqualToOther(other))
			return false;
		return true;
	}
	bool operator > (const basicSensorDate& other) const
	{
		return isLargerThanOther(other);
	}
	bool operator <= (const basicSensorDate& other) const
	{
		return !isLargerThanOther(other);
	}
	bool operator >= (const basicSensorDate& other) const
	{
		return !isSmallerThanOther(other);
	}
	bool operator != (const basicSensorDate& other) const
	{
		return !isEqualToOther(other);
	}
	//
	// set content from a string (coming from text file)
	virtual bool setFromString(const string& dateInputString, bool allowOverwrite = true)
	{
		cout << "setFromString() not ready: this neends to be specialized" << endl;
		return false;
	}
};


class basicSensorTime {
protected:
	unsigned char hour, minute;
	float second;
	bool initialized;
	static const unsigned char min_minute = 0;
	static const unsigned char max_minute = 59;
	static const unsigned char min_hour = 0;
	static const unsigned char max_hour = 23;
	static const int sup_second = 60;
	//
	void blankInitialization()
	{
		hour = 0;
		minute = 0;
		second = 0;
		initialized = false;
	}
	bool checkAndSetHourMinuteSecond(unsigned char in_hour, unsigned char in_minute, float in_second)
	{
		bool result = false;
		if ((in_hour >= min_hour) && (in_hour <= max_hour))
		{
			if ((in_minute >= min_minute) && (in_minute <= max_minute))
			{
				if ((in_second >= 0) && (in_second < sup_second))
				{
					hour = in_hour;
					minute = in_minute;
					second = in_second;
					initialized = true;
					result = true;
				}
			}
		}
		return result;


	}
public:
	basicSensorTime() { blankInitialization(); }
	~basicSensorTime() { blankInitialization(); }
	// copy constructor
	basicSensorTime(const basicSensorTime& other)
	{
		blankInitialization();
		copyFrom(other, true);
	}
	// getters (overloaded versions with strings for printing)
	bool getInit() const { return initialized; }
	bool getHour(unsigned char& outVal)const
	{
		if (initialized)
		{
			outVal = hour;
		}
		return initialized;
	}
	bool getHour(unsigned char& outVal, string& outString)const
	{
		if (getHour(outVal))
		{
			outString = to_string(outVal);
		}
		return initialized;
	}
	bool getMinute(unsigned char& outVal)const
	{
		if (initialized)
		{
			outVal = minute;
		}
		return initialized;
	}
	bool getMinute(unsigned char& outVal, string& outString)const
	{
		if (getMinute(outVal))
		{
			outString = to_string(outVal);
		}
		return initialized;
	}
	bool getSecond(float& outVal)const
	{
		if (initialized)
		{
			outVal = second;
		}
		return initialized;
	}
	bool getSecond(float& outVal, string& outString)const
	{
		if (getSecond(outVal))
		{
			outString = to_string(outVal);
		}
		return initialized;
	}
	//setters
	bool setTime(unsigned char in_hour, unsigned char in_minute, float in_second, bool allowOverwrite = false)
	{
		bool result = false;
		if (initialized && (!allowOverwrite))
			cout << "Error: Attempt to overwrite Time information" << endl;
		else
			result = checkAndSetHourMinuteSecond(in_hour, in_minute, in_second);
		return result;
	}
	void resetTime() { blankInitialization(); }
	// helpers and operators
	// FV_edit: 01 July 2022; make "convert2string" virtual,
	// to allow specialization for other time types. 
	virtual string convert2string(bool longform) const
	{
		float out_second;
		unsigned char out_hour, out_minute;
		string out_hour_name, out_minute_name, out_second_name;
		string result;
		if (getHour(out_hour, out_hour_name))
		{
			if (longform)
			{
				result += "Hour is: ";
				result += out_hour_name;
				result += "; ";
			}
			else
			{
				if (out_hour < 10)
					result += "0";
				result += out_hour_name;
				result += ":";
			}
		}
		if (getMinute(out_minute, out_minute_name))
		{
			if (longform)
			{
				result += "Minute is: ";
				result += out_minute_name;
				result += "; ";
			}
			else
			{
				if (out_minute < 10)
					result += "0";
				result += out_minute_name;
				result += ":";
			}

		}
		if (getSecond(out_second, out_second_name))
		{
			if (longform)
			{
				result += "Second is: ";
				result += out_second_name;
				result += ". ";
			}
			else
			{
				if (out_second < 10)
					result += "0";
				result += out_second_name;
				result += " ";
			}
		}
		return result;
	}
	operator string() const
	{
		bool option = false;
		string result = convert2string(option);
		return result;
	}
	// = operator	
	virtual void copyFrom(const basicSensorTime& other, bool allowOverwrite = false)
	{
		// avoid self copy
		if (this != &other)
		{	// only copy from an initialized object to a not-initialized one
			if ((other.getInit()) && (!this->getInit() || allowOverwrite))
			{
				unsigned char in_hour, in_minute;
				float in_second;
				other.getHour(in_hour);
				other.getMinute(in_minute);
				other.getSecond(in_second);
				setTime(in_hour, in_minute, in_second, true);
			}
		}
	}
	basicSensorTime& operator = (const basicSensorTime& other)
	{
		this->copyFrom(other, true);
		return*this;
	}
	// < operator
	virtual bool isSmallerThanOther(const basicSensorTime& other) const
	{
		unsigned char  other_hour, other_minute;
		float other_second;
		if (!getInit() || !other.getInit())
		{
			cout << "Error: trying to compare Time with entries not initialized." << endl;
			return false;
		}
		other.getHour(other_hour);
		other.getMinute(other_minute);
		other.getSecond(other_second);
		if (hour > other_hour)
			return false;
		if (hour < other_hour)
			return true;
		// same hour:
		if (minute > other_minute)
			return false;
		if (minute < other_minute)
			return true;
		// same hour and minute:
		if (second < other_second)
			return true;
		return false;
	}
	bool operator < (const basicSensorTime& other) const
	{
		return isSmallerThanOther(other);
	}
	// == operator
	virtual bool isEqualToOther(const basicSensorTime& other) const
	{
		unsigned char  other_hour, other_minute;
		float other_second;
		if (!getInit() || !other.getInit())
		{
			cout << "Error: trying to compare Time with entries not initialized." << endl;
			return false;
		}
		other.getHour(other_hour);
		other.getMinute(other_minute);
		other.getSecond(other_second);
		if (hour != other_hour)
			return false;
		if (minute != other_minute)
			return false;
		if (second != other_second)
			return false;
		return true;
	}
	bool operator == (const basicSensorTime& other) const
	{
		return isEqualToOther(other);
	}
	// operators > , <= , >=, !=
	bool isLargerThanOther(const basicSensorTime& other) const
	{
		if (isSmallerThanOther(other))
			return false;
		if (isEqualToOther(other))
			return false;
		return true;
	}
	bool operator > (const basicSensorTime& other) const
	{
		return isLargerThanOther(other);
	}
	bool operator <= (const basicSensorTime& other) const
	{
		return !isLargerThanOther(other);
	}
	bool operator >= (const basicSensorTime& other) const
	{
		return !isSmallerThanOther(other);
	}
	bool operator != (const basicSensorTime& other) const
	{
		return !isEqualToOther(other);
	}//
	// set content from a string (coming from text file)
	virtual bool setFromString(const string& timeInputString, bool allowOverwrite = true)
	{
		cout << "setFromString() not ready: this neends to be specialized" << endl;
		return false;
	}
	// static helper
	static bool formatLongerIntervals(unsigned int& in_hour, unsigned int& in_minute, float& in_second, unsigned int& ext_day)
	{
		bool result = false;
		int extra_min = 0;
		int extra_hour = 0;
		int int_second = in_second;
		float fract_second = in_second - int_second;
		ext_day = 0;
		if (in_second >= 60)
		{
			extra_min = int_second / 60;
			int_second = int_second % 60;
			in_second = int_second + fract_second;
		}
		extra_min += in_minute;// also: do computations wih int type
		if (extra_min >= 60)
		{
			extra_hour = extra_min / 60;
			extra_min = extra_min % 60;
		}
		in_minute = extra_min;
		extra_hour += in_hour;// also:  do computations wih int type
		if (extra_hour >= 24)
		{
			ext_day = extra_hour / 24;
			extra_hour = extra_hour % 24;
			result = true;
		}
		in_hour = extra_hour;
		return result;
	}
};

// sensor date and time based on the above
template <class sns_DATE, class sns_TIME>
class sensorDateAndTime {
protected:
	;
public:
	sns_DATE date;
	sns_TIME time;
public:
	// copy constructor
	//sensorDateAndTime() : date() , time() { ; }
	sensorDateAndTime() { ; }
	sensorDateAndTime(const sensorDateAndTime& other) { copyFrom(other, true); }
	// setters (for the composite object)
	void resetDateAndTime() { date.resetDate(); time.resetTime(); }
	void copyDate(const sns_DATE& otherDate) { date.copyFrom(date, true); }
	void copyTime(const sns_TIME& otherTime) { time.copyFrom(otherTime, true); }
	void copyFrom(const sensorDateAndTime& other, bool allowOverwrite = false)
	{
		date.copyFrom(other.date, allowOverwrite);
		time.copyFrom(other.time, allowOverwrite);
	}
	//
	bool getInit(bool both = false) const
	{
		if (both)
			return (date.getInit() && time.getInit());
		return (date.getInit() || time.getInit());
	}
	// = operator		
	sensorDateAndTime& operator = (const sensorDateAndTime& other) { copyFrom(other, true); return *this; }
	// < operator
	bool isSmallerThanOther(const sensorDateAndTime& other) const
	{
		if (date > other.date)
			return false;
		if (date < other.date)
			return true;
		// same date
		return (time < other.time);
	}
	bool operator < (const sensorDateAndTime& other) const { return isSmallerThanOther(other); }
	// == operator
	bool isEqualToOther(const sensorDateAndTime& other) const
	{
		if (date == other.date)
			return (time == other.time);
		return false;
	}
	bool operator == (const sensorDateAndTime& other) const { return isEqualToOther(other); }
	// operators > , <= , >=, !=	
	bool operator > (const sensorDateAndTime& other) const
	{
		if (isSmallerThanOther(other))
			return false;
		if (isEqualToOther(other))
			return false;
		return true;
	}
	bool operator >= (const sensorDateAndTime& other) const
	{
		return !isSmallerThanOther(other);
	}
	bool operator <= (const sensorDateAndTime& other) const
	{
		if (isSmallerThanOther(other) || isEqualToOther(other))
			return true;
		return false;
	}
	bool operator != (const sensorDateAndTime& other) const { return !isEqualToOther(other); }
	//
	string convert2string(bool longform) const
	{
		string str_date, str_time, str_result;
		str_date = date.convert2string(longform);
		str_time = time.convert2string(longform);
		str_result = str_date;
		//str_result += " ";
		str_result += str_time;
		return str_result;
	}
	operator string() const
	{
		bool option = false;
		string result = convert2string(option);
		return result;
	}
	// set content from a string (coming from text file)
	bool setFromString(const string& dateAndTimeInputString, bool allowOverwrite = true)
	{
		cout << "setFromString() not ready: this neends to be specialized" << endl;
		return false;
	}
	bool incrementBy(unsigned int dur_hour, unsigned int dur_minute, float dur_second)
	{
		bool result = false;
		if (getInit(true))
		{
			// compute the new time
			unsigned char in_hour, in_minute; float in_second;
			unsigned int temp_in_hour, temp_in_minute, ext_day, day_count = 0;
			if (time.formatLongerIntervals(dur_hour, dur_minute, dur_second, ext_day))
				day_count += ext_day;
			time.getHour(in_hour); time.getMinute(in_minute); time.getSecond(in_second);
			//in_hour += dur_hour; in_minute += dur_minute; in_second += dur_second;
			temp_in_hour = in_hour + dur_hour; temp_in_minute = in_minute + dur_minute; in_second += dur_second;
			if (time.formatLongerIntervals(temp_in_hour, temp_in_minute, in_second, ext_day))
				day_count += ext_day;
			// update the time
			in_hour = temp_in_hour; in_minute = temp_in_minute;
			time.setTime(in_hour, in_minute, in_second, true);
			// update the date if needed
			if (day_count > 0)
			{
				unsigned short int in_year, cur_year; unsigned char in_month, cur_month, in_day, cur_day;
				date.getYear(cur_year); date.getMonth(cur_month); date.getDay(cur_day);
				in_year = cur_year; in_month = cur_month; in_day = cur_day;
				for (int i = 0; i < day_count; i++)
				{
					in_day++;
					if (!date.setDate(in_year, in_month, in_day, true))
					{
						// hit the month limit: reset to day 1, update month and retry
						in_day = 1;
						if ((++in_month) > 12)
						{
							// will hit the year limit : reset to Jan and update year
							in_month = 1;
							in_year++;
						}
						date.setDate(in_year, in_month, in_day, true);
					}
				}
			}
			result = true;
		}
		return result;
	}
};

///---------------- ///
// the basic Date-time : combination of the basicDate and basicTime
typedef sensorDateAndTime <basicSensorDate, basicSensorTime> basicSensorDateAndTime;
///---------------- ///


// interval used for processing. Long intervals: set start and stop; short intervals: set start and duration.
template <class sns_DATEAndTime>
class senDateTimeInterval {
protected:
	sns_DATEAndTime start, middle, stop;
	bool start_set, duration_set, stopAndMiddle_setBasedOnDuration, stop_set;
	unsigned char dur_hour, dur_minute;
	float dur_second;
	unsigned int ext_dur_day;

	void blankInit()
	{
		start_set = false;
		duration_set = false;
		stopAndMiddle_setBasedOnDuration = false;
		stop_set = false;
		ext_dur_day = 0;
	}
	bool updateStopAndMiddleBasedOnDuration()
	{
		int minutes, hours;
		bool result = false;
		if (start_set && duration_set)
		{
			stop = start; middle = start;
			stop.incrementBy(dur_hour, dur_minute, dur_second);
			//further update because of extra day
			if (ext_dur_day > 0)
				stop.incrementBy((ext_dur_day * 24), 0, 0);

			// next the midle: by half the amount
			minutes = ((dur_hour * 60) + dur_minute);
			if (minutes % 2)
				dur_second += 30;
			minutes /= 2;
			hours = 0;
			if (minutes >= 60)
			{
				hours = minutes / 60;
				minutes = minutes % 60;
			}
			middle.incrementBy(hours, minutes, dur_second / 2);
			//further update because of extra day
			if (ext_dur_day > 0)
				middle.incrementBy((ext_dur_day * 12), 0, 0);
			//
			stopAndMiddle_setBasedOnDuration = true;
			stop_set = false;
			result = true;
		}
		return result;
	}
	static bool computeDuration_timePart(sns_DATEAndTime& in_start, sns_DATEAndTime& in_stop, unsigned int& out_dur_hour, unsigned int& out_dur_minute, float& out_dur_second)
	{
		out_dur_hour = 0;
		out_dur_minute = 0;
		out_dur_second = 0.0;
		bool result = false;
		unsigned int extra_day_count = 0;
		if (in_start.getInit(true) && in_stop.getInit(true))
		{
			//cout << "Entering computeDuration_timePart(); Start: " << in_start.convert2string(false) << endl;
			if (in_start <= in_stop)
			{
				float outSec_start, outSec_stop;
				unsigned char outMin_start, outHour_start, outMin_stop, outHour_stop;

				// Start by aligning the seconds
				if (!in_start.time.getSecond(outSec_start))
				{
					cout << "Error in computeDuration(): Start time not set properly" << endl;
					return false;
				}
				if (!in_stop.time.getSecond(outSec_stop))
				{
					cout << "Error in computeDuration(): Stop time not set properly" << endl;
					return false;
				}
				// advanece to next minute if needed
				if (outSec_start > outSec_stop)
				{
					out_dur_second += (60.0 - outSec_start);
					//cout << "Before increment: " << in_start.convert2string(false) << endl;
					in_start.incrementBy(0, 0, (60.0 - outSec_start));
					//cout << "After increment: " << in_start.convert2string(false) << endl << endl;
					outSec_start = 0.0;
				}
				// complete aligning the seconds
				out_dur_second += (outSec_stop - outSec_start);
				//cout << "Before increment: " << in_start.convert2string(false) << endl;
				in_start.incrementBy(0, 0, (outSec_stop - outSec_start));
				//cout << "After increment: " << in_start.convert2string(false) << endl << endl;
				while (out_dur_second >= 60.0)
				{
					cout << "Warning in computeDuration(): cleanup sequence that should not be needed" << endl;
					out_dur_minute += 1;
					out_dur_second -= 60.0;
				}
				/// Then align the minutes
				if (!in_start.time.getMinute(outMin_start))
				{
					cout << "Error in computeDuration(): Start time not set properly" << endl;
					return false;
				}
				if (!in_stop.time.getMinute(outMin_stop))
				{
					cout << "Error in computeDuration(): Stop time not set properly" << endl;
					return false;
				}
				/// advance to next hour if needed
				if (outMin_start > outMin_stop)
				{
					out_dur_minute += (60 - outMin_start);
					//cout << "Before increment: " << in_start.convert2string(false) << endl;
					in_start.incrementBy(0, (60 - outMin_start), 0);
					//cout << "After increment: " << in_start.convert2string(false) << endl << endl;
					outMin_start = 0;
				}
				/// complete aligning minutes
				out_dur_minute += (outMin_stop - outMin_start);
				//cout << "Before increment: " << in_start.convert2string(false) << endl;
				in_start.incrementBy(0, (outMin_stop - outMin_start), 0);
				//cout << "After increment: " << in_start.convert2string(false) << endl << endl;
				while (out_dur_minute > 60)
				{
					cout << "Warning in computeDuration(): cleanup sequence that should not be needed" << endl;
					out_dur_hour += 1;
					out_dur_minute -= 60;
				}
				//// Finally align the hours
				if (!in_start.time.getHour(outHour_start))
				{
					cout << "Error in computeDuration(): Start time not set properly" << endl;
					return false;
				}
				if (!in_stop.time.getHour(outHour_stop))
				{
					cout << "Error in computeDuration(): Stop time not set properly" << endl;
					return false;
				}
				//// advanece to next day if needed
				if (outHour_start > outHour_stop)
				{
					out_dur_hour += (24 - outHour_start);
					//cout << "Before increment: " << in_start.convert2string(false) << endl;
					in_start.incrementBy((24 - outHour_start), 0, 0);
					//cout << "After increment: " << in_start.convert2string(false) << endl << endl;
					outHour_start = 0;
				}
				//// Complete aligning the hours   
				out_dur_hour += (outHour_stop - outHour_start);
				//cout << "Before increment: " << in_start.convert2string(false) << endl;
				in_start.incrementBy((outHour_stop - outHour_start), 0, 0);
				//cout << "After increment: " << in_start.convert2string(false) << endl << endl;
				//cout << "Done" << endl;
				result = true;
				//cout << "Leaving computeDuration_timePart(); Start: " << in_start.convert2string(false) << endl << endl;
			}
			else
				cout << "Error in computeDuration(): Start must preceed Stop" << endl;
		}
		else
			cout << "Error in computeDuration(): Start and Stop must be set" << endl;

		return result;
	}
	static bool computeDuration_DatePart(sns_DATEAndTime& in_start, sns_DATEAndTime& in_stop, unsigned int& out_dur_days)
	{
		bool result = false;
		out_dur_days = 0;
		if (in_start.getInit(true) && in_stop.getInit(true))
		{
			//cout << "Entering computeDuration_DatePart(); Start: " << in_start.convert2string(false) << endl;
			if (in_start <= in_stop)
			{
				while (in_start.date < in_stop.date)
				{
					// increment a day at a time
					//cout << "Before increment: " << in_start.convert2string(false) << endl;
					in_start.incrementBy(24, 0, 0);
					out_dur_days += 1;
					//cout << "After increment: " << in_start.convert2string(false) << endl;
					//cout << endl;
				}
				result = true;
				//cout << "Leaving computeDuration_DatePart(); Start: " << in_start.convert2string(false) << endl << endl;
			}
			else
				cout << "Error in computeDuration(): Start must preceed Stop" << endl;
		}
		else
			cout << "Error in computeDuration(): Start and Stop must be set" << endl;

		return result;
	}
	bool updateDurationAndMiddleBasedOnStop()
	{
		unsigned int temp_dur_hour, temp_dur_minute, temp_dur_days;
		float temp_dur_second;
		bool result = computeDuration(start, stop, temp_dur_days, temp_dur_hour, temp_dur_minute, temp_dur_second);
		if (result)
		{
			temp_dur_hour += (24 * temp_dur_days);
			result = setDuration(temp_dur_hour, temp_dur_minute, temp_dur_second, true);
		}
		else
			cout << "Error in updateDurationAndMiddleBasedOnStop()" << endl;
		return result;
	}
public:
	senDateTimeInterval() { blankInit(); }
	// copy constructor
	senDateTimeInterval(const senDateTimeInterval& other) { copyFrom(other, true); }
	// setters
	void resetAll() { blankInit(); }
	bool setStart(const sns_DATEAndTime& in_start, bool updateAll = false)
	{
		bool result = false;
		if (!start_set && in_start.getInit(true))
		{
			// consistency check
			if (stop_set && !(updateAll && duration_set))
			{
				if (stop < in_start)
				{
					cout << "Error: can't set Interval Stop < Start" << endl;
					return false;
				}
			}
			start = in_start;
			start_set = true;
			result = true;
			if (updateAll && duration_set)
				result = updateStopAndMiddleBasedOnDuration();
		}
		return result;
	}
	bool setDuration(unsigned int in_dur_hour, unsigned int in_dur_minute, float in_dur_second, bool updateAll = false)
	{
		bool result = false;
		if (!duration_set || (start_set && updateAll))
		{
			// fix the duration parameters
			if (in_dur_second < 0)
				in_dur_second = 0.0;
			ext_dur_day = 0;
			start.time.formatLongerIntervals(in_dur_hour, in_dur_minute, in_dur_second, ext_dur_day);
			dur_hour = (unsigned char)in_dur_hour;
			dur_minute = (unsigned char)in_dur_minute;
			dur_second = in_dur_second;
			//ext_dur_day = in_ext_day;
			duration_set = true;
			result = true;
			// then set the rest;
			if (updateAll && start_set)
				result = updateStopAndMiddleBasedOnDuration();
		}
		return result;
	}
	bool setStop(const sns_DATEAndTime& in_stop)
	{
		bool result = false;
		if (!(stop_set || stopAndMiddle_setBasedOnDuration) && in_stop.getInit(true))
		{
			// consistency check
			if (start_set)
			{
				if (in_stop < start)
				{
					cout << "Error: can't set Interval Stop < Start" << endl;
					return false;
				}
			}

			stop = in_stop;
			stop_set = true;
			duration_set = updateDurationAndMiddleBasedOnStop();
			stopAndMiddle_setBasedOnDuration = duration_set;
			result = true;
		}
		return result;
	}
	// advanceByDuration : advance start and stop accordingly
	bool advanceByDuration()
	{
		bool result = false;
		if (start_set && stopAndMiddle_setBasedOnDuration)
		{
			start = stop;
			result = updateStopAndMiddleBasedOnDuration();
		}
		return result;
	}
	bool advanceBySlideDuration(unsigned char slide_hour, unsigned char slide_minute, float slide_second)
	{
		bool result = false;
		if (start_set && duration_set)
		{
			start.incrementBy(slide_hour, slide_minute, slide_second);
			result = updateStopAndMiddleBasedOnDuration();
		}
		return result;
	}
	//getters
	sns_DATEAndTime getStart() const
	{
		sns_DATEAndTime result;
		if (start_set)
			result = start;
		return result;
	}
	sns_DATEAndTime getStop() const
	{
		sns_DATEAndTime result;
		if (stop_set || stopAndMiddle_setBasedOnDuration)
			result = stop;
		return result;
	}
	sns_DATEAndTime getMiddle() const
	{
		sns_DATEAndTime result;
		if (stopAndMiddle_setBasedOnDuration)
			result = middle;
		return result;
	}
	// utilities
	string convert2string() const
	{
		string result;
		result += "Interval Start: ";
		if (start_set)
			result += start.convert2string(false);
		else
			result += "Not set";

		result += "; Middle: ";
		if (stopAndMiddle_setBasedOnDuration)
			result += middle.convert2string(false);
		else
			result += "Not set";

		result += "; Stop: ";
		if (stopAndMiddle_setBasedOnDuration || stop_set)
			result += stop.convert2string(false);
		else
			result += "Not set";

		result += ".";
		return result;
	}
	bool isStartSet() const { return start_set; }
	bool isStopSet() const { return (stop_set || stopAndMiddle_setBasedOnDuration); }

	// FV_Edit: 11 July 2022; check that both ends are set
	bool isSet() const { return (isStartSet() && isStopSet()); }

	// true if input is within the interval
	bool contains(const sns_DATEAndTime& test_val) const
	{
		if (start_set && (stopAndMiddle_setBasedOnDuration || stop_set) && test_val.getInit(true))
			if ((start <= test_val) && (test_val < stop))
				return true;
		return false;
	}
	// true if interval preceeds the input
	bool precede(const sns_DATEAndTime& test_val) const
	{
		//if (start_set && (stopAndMiddle_setBasedOnDuration || stop_set) && test_val.getInit(true))
		if (((start_set && stopAndMiddle_setBasedOnDuration) || stop_set) && test_val.getInit(true))
			if (stop < test_val)
				return true;
		return false;
	}
	// true if interval follows the input
	bool follows(const sns_DATEAndTime& test_val) const
	{
		//if (start_set && (stopAndMiddle_setBasedOnDuration || stop_set) && test_val.getInit(true))
		if (start_set && test_val.getInit(true))
			if (test_val < start)
				return true;
		return false;
	}
	//
	// copy operator
	void copyFrom(const senDateTimeInterval& other, bool allowOverwrite = false)
	{
		if (this != &other)
		{
			if (other.isStartSet() && other.isStopSet())
			{
				if (allowOverwrite)
					resetAll();
				setStart(other.getStart());
				setStop(other.getStop());
			}
		}
	}
	senDateTimeInterval& operator = (const senDateTimeInterval& other) { copyFrom(other, true); return *this; }
	//
	// utility
	static bool computeDuration(sns_DATEAndTime in_start, sns_DATEAndTime in_stop, unsigned int& out_dur_days, unsigned int& out_dur_hour, unsigned int& out_dur_minute, float& out_dur_second)
	{
		bool result = computeDuration_timePart(in_start, in_stop, out_dur_hour, out_dur_minute, out_dur_second);
		if (!result)
			return false;
		//cout << "Duration Hour: " << out_dur_hour << endl;
		//cout << "Duration Min: " << out_dur_minute << endl;
		//cout << "Duration Sec: " << out_dur_second << endl;
		// now the days
		result = computeDuration_DatePart(in_start, in_stop, out_dur_days);
		if (!result)
			return false;
		//cout << "Duration Days: " << out_dur_days << endl;
		return true;
	}

};

// Sensor reading (a collection of scalar values)
class basicSensorMultipleScalarReading {
protected:
	int max_variables;
	float* value_array;
	bool* valueInitialized_array;
	bool* nameInitialized_array;
	bool* unitInitialized_array;
	string* readingName_array;
	string* readingUnit_array;
	bool memIsAllocated;
	virtual bool memAllocation()
	{
		bool result = false;
		if (!memIsAllocated && max_variables > 0)
		{
			value_array = (float*)calloc(max_variables, sizeof(float));
			if (value_array == NULL)
				return false;
			//
			valueInitialized_array = (bool*)calloc(max_variables, sizeof(bool));
			if (valueInitialized_array == NULL)
				return false;
			//
			nameInitialized_array = (bool*)calloc(max_variables, sizeof(bool));
			if (nameInitialized_array == NULL)
				return false;
			//
			unitInitialized_array = (bool*)calloc(max_variables, sizeof(bool));
			if (unitInitialized_array == NULL)
				return false;
			//
			readingName_array = new string[max_variables];
			if (readingName_array == NULL)
				return false;
			//
			readingUnit_array = new string[max_variables];
			if (readingUnit_array == NULL)
				return false;
			// finally set memory flag
			memIsAllocated = true;
			result = true;
		}
		return result;
	}
	virtual void memDeallocation()
	{
		if (memIsAllocated)
		{
			free(value_array);
			free(valueInitialized_array);
			free(nameInitialized_array);
			free(unitInitialized_array);
			delete[] readingName_array;
			delete[] readingUnit_array;
			max_variables = 0;
			memIsAllocated = false;
		}
	}
	virtual bool blankInitialization()
	{
		//allocate memory (only once)
		if (!memIsAllocated)
			memAllocation();
		if (memIsAllocated)
			for (int i = 0; i < max_variables; i++)
			{
				valueInitialized_array[i] = false;
				nameInitialized_array[i] = false;
				unitInitialized_array[i] = false;
				readingName_array[i].clear();
				readingUnit_array[i].clear();
			}
		return memIsAllocated;
	}
	bool checkAndSetVal(int varnum, float in_val)
	{
		if (chekVarnumInBounds(varnum))
		{
			// no checks in the basic class
			value_array[varnum] = in_val;
			valueInitialized_array[varnum] = true;
			return true;
		}
		return false;

	}
	inline bool chekVarnumInBounds(int varnum) const
	{
		return ((varnum >= 0 && varnum < max_variables) ? true : false);
		//if (varnum >= 0 && varnum < MAX_VARIABLES)
		//	return true;
		//return false;		
	}
public:
	basicSensorMultipleScalarReading() { memIsAllocated = false;  max_variables = 0;  blankInitialization(); }
	basicSensorMultipleScalarReading(int numOfVariablesMeasured)
	{
		memIsAllocated = false;  max_variables = 0;
		setnumOfVariables(numOfVariablesMeasured);
	}
	// copy constructor
	basicSensorMultipleScalarReading(const basicSensorMultipleScalarReading& other)
	{
		memIsAllocated = false;  max_variables = 0;
		//setnumOfVariables(other.getMaxVariables());
		copyFrom(other, true);
	}//
	//	
	~basicSensorMultipleScalarReading() { memDeallocation(); }
	// getters
	bool getInitVal(int varnum) const
	{
		bool result = false;
		if (chekVarnumInBounds(varnum))
			result = valueInitialized_array[varnum];
		return result;
	}
	bool getInitName(int varnum) const
	{
		bool result = false;
		if (chekVarnumInBounds(varnum))
			result = nameInitialized_array[varnum];
		return result;
	}
	bool getInitUnit(int varnum) const
	{
		bool result = false;
		if (chekVarnumInBounds(varnum))
			result = unitInitialized_array[varnum];
		return result;
	}
	bool getInit(int varnum) const { return (getInitVal(varnum) || getInitName(varnum) || getInitUnit(varnum)); }
	bool getInit() const
	{
		for (int i = 0; i < max_variables; i++)
			if (getInit(i))
				return true;
		return false;
	}
	bool getMemIsAllocated() const { return memIsAllocated; }
	int getMaxVariables() const { return max_variables; }
	// FV_edit: 6 July 2022; query the reading to know how many variables have been set (iniitalized with valid entries)
	int getTotInitVariables() const
	{
		int tot_init_variables = 0;
		for (int varnum = 0; varnum < getMaxVariables(); varnum++)
			if (getInitVal(varnum))
				tot_init_variables++;

		return tot_init_variables;
	}
	// // getters (overloaded versions with strings for printing)
	bool getVal(int varnum, float& outVal)const
	{
		bool result = getInitVal(varnum);
		if (result)
		{
			outVal = value_array[varnum];
		}
		return result;
	}
	bool getVal(int varnum, float& outVal, string& outString)const
	{
		bool result = getVal(varnum, outVal);
		if (result)
		{
			outString = to_string(outVal);
		}
		return result;
	}
	bool getName(int varnum, string& outName)const
	{
		bool result = getInitName(varnum);
		if (result)
		{
			outName = readingName_array[varnum];
		}
		return result;
	}
	bool getUnit(int varnum, string& outUnit)const
	{
		bool result = getInitUnit(varnum);
		if (result)
		{
			outUnit = readingUnit_array[varnum];
		}
		return result;
	}
	//setters
	virtual bool setnumOfVariables(int numOfVariablesMeasured)
	{
		bool result = false;
		if (!memIsAllocated)
		{
			if (numOfVariablesMeasured > 0)
			{
				max_variables = numOfVariablesMeasured;
				result = blankInitialization();
			}
			else
				cout << "Error: Number of numOfVariablesMeasured is not supported" << endl;
		}
		else
			cout << "Error: Number of Readings is already set and memory allocated" << endl;
		return result;
	}
	virtual bool setVal(int varnum, float in_val, bool allowOverwrite = false)
	{
		bool result = false;
		if (getInitVal(varnum) && (!allowOverwrite))
			cout << "Error: Attempt to overwrite Reading Value" << endl;
		else
			result = checkAndSetVal(varnum, in_val);
		return result;
	}
	virtual bool setName(int varnum, string inName, bool allowOverwrite = false)
	{
		bool result = false;
		if (getInitName(varnum) && (!allowOverwrite))
			cout << "Error: Attempt to overwrite Reading Name" << endl;
		else
		{
			if (chekVarnumInBounds(varnum))
			{
				// no checks in the basic class
				readingName_array[varnum] = inName;
				nameInitialized_array[varnum] = true;
				result = true;
			}
		}
		return result;
	}
	virtual bool setUnit(int varnum, string inUnit, bool allowOverwrite = false)
	{
		bool result = false;
		if (getInitUnit(varnum) && (!allowOverwrite))
			cout << "Error: Attempt to overwrite Reading Unit" << endl;
		else
		{
			if (chekVarnumInBounds(varnum))
			{
				// no checks in the basic class
				readingUnit_array[varnum] = inUnit;
				unitInitialized_array[varnum] = true;
				result = true;
			}
		}
		return result;
	}
	//
	// 
	//
	void resetAll() { blankInitialization(); }
	// set content from a string (coming from text file)
	virtual bool setFromString(const string& readingsInputString, bool allowOverwrite = true)
	{
		cout << "Input from string function not defined here: must be specialized" << endl;
		return false;
	}
	//
	virtual void printClassInfo() const
	{
		cout << "basicSensorMultipleScalarReading. ";
		if (getMaxVariables() > 0)
			cout << "Memory allocated to support " << getMaxVariables() << " variables." << endl;
		else
			cout << "Memory can be allocated to support any number of variables." << endl;
	}
	// FV_edit: 01 July 2022; make "convert2string" virtual,
	// to allow specialization for other reading types. 
	virtual string convert2string(bool longform) const
	{
		float outVal;
		string outString, outName, outUnit;
		string result;
		for (int varnum = 0; varnum < max_variables; varnum++)
		{
			if (longform && getName(varnum, outName))
			{
				result += outName;
				result += " Reading is: ";
			}
			if (getVal(varnum, outVal, outString))
			{
				result += outString;
				result += " ";
			}
			if (longform && getUnit(varnum, outUnit))
			{
				result += outUnit;
				result += " . ";
			}
		}
		return result;
	}
	operator string() const
	{
		bool option = false;
		//bool option = true;
		string result = convert2string(option);
		return result;
	}
	// = operator	
	virtual void copyFrom(const basicSensorMultipleScalarReading& other, bool allowOverwrite = false)
	{
		// avoid self copy
		if (this != &other)
		{	// only copy from an initialized object to a not-initialized one
			if ((other.getInit()) && (!this->getInit() || allowOverwrite))
			{
				float otherVal; string otherString; int tot_val;
				int other_tot_val = other.getMaxVariables();
				// mem allocation
				if (!getMemIsAllocated())
					setnumOfVariables(other_tot_val);

				int this_tot_val = getMaxVariables();

				if (this_tot_val < other_tot_val)
					tot_val = this_tot_val;
				else
					tot_val = other_tot_val;

				for (int varnum = 0; varnum < tot_val; varnum++)
				{
					if (other.getVal(varnum, otherVal))
						setVal(varnum, otherVal, allowOverwrite);
					if (other.getName(varnum, otherString))
						setName(varnum, otherString, allowOverwrite);
					if (other.getUnit(varnum, otherString))
						setUnit(varnum, otherString, allowOverwrite);
				}
			}
		}
	}
	basicSensorMultipleScalarReading& operator = (const basicSensorMultipleScalarReading& other)
	{
		this->copyFrom(other, true);
		return*this;
	}

};

// FV Edit: 8 July 2022;
// Sensor Vectorial reading (a collection of vector values,
// where each vector entry is a basicSensorMultipleScalarReading)
class basicSensorMultipleVectorReading {
protected:
	int max_variables;
	basicSensorMultipleScalarReading* vectVal_array;
	bool memIsAllocated;
	virtual bool memAllocation()
	{
		bool result = false;
		if (!memIsAllocated && max_variables > 0)
		{
			vectVal_array = new basicSensorMultipleScalarReading[max_variables];
			if (vectVal_array == NULL)
				return false;
			// set memory flag
			memIsAllocated = true;
			result = true;
		}
		return result;
	}
	virtual void memDeallocation()
	{
		if (memIsAllocated)
		{
			delete[] vectVal_array;
			max_variables = 0;
			memIsAllocated = false;
		}
	}
	virtual bool blankInitialization()
	{
		//allocate memory (only once)
		if (!memIsAllocated)
			memAllocation();
		return memIsAllocated;
	}
	bool checkAndSetVal(int varnum, const basicSensorMultipleScalarReading& in_val)
	{
		if (chekVarnumInBounds(varnum) && in_val.getInit())
		{
			bool result = false;
			int inp_maxVars, this_maxVars, maxVars;
			// allowed to overwrite 			
			inp_maxVars = in_val.getMaxVariables();
			vectVal_array[varnum].setnumOfVariables(inp_maxVars);
			this_maxVars = vectVal_array[varnum].getMaxVariables();

			// safe bet for now: if sizes differ, "get out";
			if (inp_maxVars != this_maxVars)
				return false;

			// coud be done with the smallest size
			maxVars = (inp_maxVars < this_maxVars ? inp_maxVars : this_maxVars);

			float outval; string outName;
			// for each component in the curent (vectorial) input:
			// set the val; set the name; set the unit;			
			for (int comp_varnum = 0; comp_varnum < maxVars; comp_varnum++)
			{
				// set the val (in all cases: overwrite)
				result = in_val.getVal(comp_varnum, outval);
				if (result)
				{
					result = vectVal_array[varnum].setVal(comp_varnum, outval, true);
					if (!result)
						return false;
				}
				else
					return false;

				// set name, if not done already
				if (!vectVal_array[varnum].getInitName(comp_varnum))
				{
					result = in_val.getName(comp_varnum, outName);
					if (result)
					{
						result = vectVal_array[varnum].setName(comp_varnum, outName, true);
						if (!result)
							return false;
					}
					else
						return false;
				}
				// set unit, if not done already
				if (!vectVal_array[varnum].getInitUnit(comp_varnum))
				{
					result = in_val.getUnit(comp_varnum, outName);
					if (result)
					{
						result = vectVal_array[varnum].setUnit(comp_varnum, outName, true);
						if (!result)
							return false;
					}
					else
						return false;
				}
			}
			return result;
		}
		return false;
	}
	bool checkAndSetName(int varnum, const basicSensorMultipleScalarReading& in_val, bool allowOverwrite)
	{
		if (chekVarnumInBounds(varnum))
		{
			bool result = false;
			int inp_maxVars, this_maxVars, maxVars;

			inp_maxVars = in_val.getMaxVariables();
			// allowed to overwrite 			
			if (allowOverwrite || !getMemIsAllocated())
				vectVal_array[varnum].setnumOfVariables(inp_maxVars);

			this_maxVars = vectVal_array[varnum].getMaxVariables();

			// safe bet for now: if sizes differ, "get out";
			if (inp_maxVars != this_maxVars)
				return false;

			// coud be done with the smallest size
			maxVars = (inp_maxVars < this_maxVars ? inp_maxVars : this_maxVars);

			float outval; string outName;
			// for each component in the curent (vectorial) input:
			// set the val; set the name; set the unit;			
			for (int comp_varnum = 0; comp_varnum < maxVars; comp_varnum++)
			{
				// set name, if not done already (or if allowed)
				if (!vectVal_array[varnum].getInitName(comp_varnum) || allowOverwrite)
				{
					result = in_val.getName(comp_varnum, outName);
					if (result)
					{
						result = vectVal_array[varnum].setName(comp_varnum, outName, allowOverwrite);
						if (!result)
							return false;
					}
					else
						return false;
				}
			}
			return result;
		}
		return false;
	}
	bool checkAndSetUnit(int varnum, const basicSensorMultipleScalarReading& in_val, bool allowOverwrite)
	{
		if (chekVarnumInBounds(varnum))
		{
			bool result = false;
			int inp_maxVars, this_maxVars, maxVars;

			inp_maxVars = in_val.getMaxVariables();
			// allowed to overwrite 			
			if (allowOverwrite || !getMemIsAllocated())
				vectVal_array[varnum].setnumOfVariables(inp_maxVars);

			this_maxVars = vectVal_array[varnum].getMaxVariables();

			// safe bet for now: if sizes differ, "get out";
			if (inp_maxVars != this_maxVars)
				return false;

			// coud be done with the smallest size
			maxVars = (inp_maxVars < this_maxVars ? inp_maxVars : this_maxVars);

			float outval; string outName;
			// for each component in the curent (vectorial) input:
			// set the val; set the name; set the unit;			
			for (int comp_varnum = 0; comp_varnum < maxVars; comp_varnum++)
			{
				// set unit, if not done already, or if allowOverwrite
				if (!vectVal_array[varnum].getInitUnit(comp_varnum) || allowOverwrite)
				{
					result = in_val.getUnit(comp_varnum, outName);
					if (result)
					{
						result = vectVal_array[varnum].setUnit(comp_varnum, outName, allowOverwrite);
						if (!result)
							return false;
					}
					else
						return false;
				}
			}
			return result;
		}
		return false;
	}
	inline bool chekVarnumInBounds(int varnum) const
	{
		return ((varnum >= 0 && varnum < max_variables) ? true : false);
	}
public:
	basicSensorMultipleVectorReading() { memIsAllocated = false;  max_variables = 0;  blankInitialization(); }
	basicSensorMultipleVectorReading(int numOfVariablesMeasured)
	{
		memIsAllocated = false;  max_variables = 0;
		setnumOfVariables(numOfVariablesMeasured);
	}
	// copy constructor
	basicSensorMultipleVectorReading(const basicSensorMultipleVectorReading& other)
	{
		memIsAllocated = false;  max_variables = 0;
		//setnumOfVariables(other.getMaxVariables());
		copyFrom(other, true);
	}//
	//	
	~basicSensorMultipleVectorReading() { memDeallocation(); }
	// getters
	bool getMemIsAllocated() const { return memIsAllocated; }
	int getMaxVariables() const { return max_variables; }
	bool getInitVal(int varnum) const
	{
		bool result = false;
		if (chekVarnumInBounds(varnum))
		{
			int maxDimensions = getMaxVariables();
			for (int index = 0; index < maxDimensions; index++)
			{
				result = vectVal_array[varnum].getInitVal(index);
				if (!result)
					return false;
			}
		}
		return result;
	}
	bool getInit() const
	{
		for (int i = 0; i < max_variables; i++)
			if (getInitVal(i))
				return true;
		return false;
	}
	int getTotInitVariables() const
	{
		int tot_init_variables = 0;
		for (int varnum = 0; varnum < getMaxVariables(); varnum++)
			if (getInitVal(varnum))
				tot_init_variables++;

		return tot_init_variables;
	}
	// 
	// getters
	bool getVal(int varnum, basicSensorMultipleScalarReading& outVal)const
	{
		float comp_outVal; string outNameOrUnit;
		bool test, result = getInitVal(varnum);
		if (result)
		{
			// check that the sizes are compatible;
			int this_tot_val = vectVal_array[varnum].getMaxVariables();
			test = outVal.setnumOfVariables(this_tot_val);
			int other_tot_val = outVal.getMaxVariables();
			if (this_tot_val != other_tot_val)
				return false;
			// read each component (and name /unit, if inititalized) and set to out
			for (int varnum = 0; varnum < this_tot_val; varnum++)
			{
				// val
				result = vectVal_array[varnum].getVal(varnum, comp_outVal);
				if (!result)
					return false;
				outVal.setVal(varnum, comp_outVal, true);
				// Name
				test = vectVal_array[varnum].getName(varnum, outNameOrUnit);
				if (test)
					outVal.setName(varnum, outNameOrUnit, true);
				// Unit
				test = vectVal_array[varnum].getUnit(varnum, outNameOrUnit);
				if (test)
					outVal.setUnit(varnum, outNameOrUnit, true);
			}
		}
		return result;
	}
	//setters
	virtual bool setnumOfVariables(int numOfVariablesMeasured)
	{
		bool result = false;
		if (!memIsAllocated)
		{
			if (numOfVariablesMeasured > 0)
			{
				max_variables = numOfVariablesMeasured;
				result = blankInitialization();
			}
			else
				cout << "Error: Number of numOfVariablesMeasured is not supported" << endl;
		}
		else
			cout << "Error: Number of Readings is already set and memory allocated" << endl;
		return result;
	}
	virtual bool setVal(int varnum, const basicSensorMultipleScalarReading& in_val, bool allowOverwrite = false)
	{
		bool result = false;
		if (getInitVal(varnum) && (!allowOverwrite))
			cout << "Error: Attempt to overwrite Reading Value" << endl;
		else
			result = checkAndSetVal(varnum, in_val);
		return result;
	}
	virtual bool setName(int varnum, const basicSensorMultipleScalarReading& in_val, bool allowOverwrite = false)
	{
		bool result = false;
		result = checkAndSetName(varnum, in_val, allowOverwrite);
		return result;
	}
	virtual bool setUnit(int varnum, const basicSensorMultipleScalarReading& in_val, bool allowOverwrite = false)
	{
		bool result = false;
		result = checkAndSetUnit(varnum, in_val, allowOverwrite);
		return result;
	}
	//
	//
	void resetAll() { blankInitialization(); }
	// set content from a string (coming from text file)
	virtual bool setFromString(const string& readingsInputString, bool allowOverwrite = true)
	{
		cout << "Input from string function not defined here: must be specialized" << endl;
		return false;
	}
	//
	virtual void printInfo() const
	{
		cout << "basicSensorMultipleVectorReading. ";
		int loc_max_size, max_size = getMaxVariables();
		if (max_size > 0)
		{
			cout << "Memory allocated to support " << max_size << " variables." << endl;;
			for (int varnum = 0; varnum < max_size; varnum++)
			{
				cout << " Vector reading " << varnum << " supports up to " << vectVal_array[varnum].getMaxVariables();
				cout << " components" << endl;
			}
			cout << endl;
		}
		else
			cout << "Memory can be allocated to support any number of variables." << endl;
	}
	//
	// = operator	
	virtual void copyFrom(const basicSensorMultipleVectorReading& other, bool allowOverwrite = false)
	{
		// avoid self copy
		if (this != &other)
		{	// only copy from an initialized object to a not-initialized one
			if ((other.getInit()) && (!this->getInit() || allowOverwrite))
			{
				float otherVal; string otherString; int tot_val;
				int other_tot_val = other.getMaxVariables();
				bool test;

				// if we are to overwrite it, then "deallocate memory", 
				// so the array can e set of the appropriate size,
				// and each array element is "fresh" and its size (numOfReadings) can be set as needed.  
				if (allowOverwrite)
					memDeallocation();
				// mem allocation
				if (!getMemIsAllocated())
					setnumOfVariables(other_tot_val);

				int this_tot_val = getMaxVariables();

				if (this_tot_val < other_tot_val)
					tot_val = this_tot_val;
				else
					tot_val = other_tot_val;

				for (int varnum = 0; varnum < tot_val; varnum++)
				{
					// this object should be freshly constructed every loop iteration
					// [so the size is appropriately set inside getVal(...)]
					basicSensorMultipleScalarReading otherEntry;

					test = other.getVal(varnum, otherEntry);
					if (test)
					{
						test = setVal(varnum, otherEntry, true);
					}
				}
			}
		}
	}
	basicSensorMultipleVectorReading& operator = (const basicSensorMultipleVectorReading& other)
	{
		this->copyFrom(other, true);
		return*this;
	}

	// string conversion
	virtual string convert2string(bool longform) const
	{
		float outVal;
		string outString;

		string result;
		for (int varnum = 0; varnum < max_variables; varnum++)
		{
			result += "[ ";
			outString = vectVal_array[varnum].convert2string(longform);
			result += outString;
			result += "] ";
		}
		return result;
	}
	operator string() const
	{
		bool option = false;
		//bool option = true;
		string result = convert2string(option);
		return result;
	}
};



// this forms the basis of the nodeID to be used in SensorDataEntry)
class basicSensorID {
protected:
	unsigned int nodeID_num;
	bool nodeID_num_set;
	virtual void blankInitialization()
	{
		nodeID_num = 0;
		nodeID_num_set = false;
	}
public:
	basicSensorID() { blankInitialization(); }
	~basicSensorID() { blankInitialization(); }
	// copy constructor
	basicSensorID(const basicSensorID& other)
	{
		blankInitialization();
		copyFrom(other, true);
	}
	//
	inline bool getInit() const { return (nodeID_num_set == true); }
	virtual bool setnodeIDnum(unsigned int in_nodeID_num, bool allowOverwrite = false)
	{
		bool result = false;
		if (!getInit() || allowOverwrite)
		{
			nodeID_num = in_nodeID_num;
			nodeID_num_set = true;
			result = true;
		}
		return result;
	}
	bool getNodeID_num(unsigned int& outVal) const
	{
		bool result = false;
		if (getInit())
		{
			outVal = nodeID_num;
			result = true;
		}
		return result;
	}
	void reset() { blankInitialization(); }
	// = operator	
	virtual void copyFrom(const basicSensorID& other, bool allowOverwrite = false)
	{
		// avoid self copy
		if (this != &other)
		{
			unsigned int oth_nodeID_num;
			// only copy from an initialized object to a not-initialized one
			if ((other.getNodeID_num(oth_nodeID_num)) && (!this->getInit() || allowOverwrite))
				setnodeIDnum(oth_nodeID_num, true);
		}
	}
	basicSensorID& operator = (const basicSensorID& other)
	{
		this->copyFrom(other, true);
		return*this;
	}
	// < operator
	virtual bool isSmallerThanOther(const basicSensorID& other) const
	{
		unsigned int  other_nodeID_num;
		if (!getInit() || !other.getNodeID_num(other_nodeID_num))
		{
			cout << "Error: trying to compare nodes with entries not initialized." << endl;
			return false;
		}
		if (nodeID_num < other_nodeID_num)
			return true;
		return false;
	}
	bool operator < (const basicSensorID& other) const
	{
		return isSmallerThanOther(other);
	}
	// == operator
	virtual bool isEqualToOther(const basicSensorID& other) const
	{
		unsigned int  other_nodeID_num;
		if (!getInit() || !other.getNodeID_num(other_nodeID_num))
		{
			cout << "Error: trying to compare nodes with entries not initialized." << endl;
			return false;
		}
		if (nodeID_num == other_nodeID_num)
			return true;
		return false;
	}
	bool operator == (const basicSensorID& other) const
	{
		return isEqualToOther(other);
	}
	// operators > , <= , >=, !=
	bool isLargerThanOther(const basicSensorID& other) const
	{
		if (isSmallerThanOther(other))
			return false;
		if (isEqualToOther(other))
			return false;
		return true;
	}
	bool operator > (const basicSensorID& other) const
	{
		return isLargerThanOther(other);
	}
	bool operator <= (const basicSensorID& other) const
	{
		return !isLargerThanOther(other);
	}
	bool operator >= (const basicSensorID& other) const
	{
		return !isSmallerThanOther(other);
	}
	bool operator != (const basicSensorID& other) const
	{
		return !isEqualToOther(other);
	}//
	// helpers and operators
	virtual string convert2string(bool longform) const
	{
		string result;
		if (getInit())
		{
			if (longform)
				result += "Node ID is: ";
			result += to_string(nodeID_num);
		}
		return result;
	}
	operator string() const
	{
		bool option = false;
		string result = convert2string(option);
		return result;
	}
};

// FV_edit: 04 July 2022 - create a basic verison of this (empty shell) to be speicialized later
class SensorOrReadingInfo {
protected:
	;
public:
	SensorOrReadingInfo() { ; }
	virtual void reset() { ; }
	// FV Edit: 20 July 2022; Always init by default
	bool getInit() const
	{
		return true;
	}

};


