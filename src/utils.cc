#include <boost/lexical_cast.hpp>
#include "utils.hh"
#include "log.hh"

namespace {

struct map_t {
	const char* mchar;
	const short mint;
} maps[] = {
	{ "Jan", 1 },
	{ "Feb", 2 },
	{ "Mar", 3 },
	{ "Apr", 4 },
	{ "May", 5 },
	{ "Jun", 6 },
	{ "Jul", 7 },
	{ "Aug", 8 },
	{ "Sep", 9 },
	{ "Oct", 10 },
	{ "Nov", 11 },
	{ "Dec", 12 }
};

short
get_int_month(const std::string& char_month)
{
	for(const map_t& m : maps) {
		if(strcasecmp(m.mchar, char_month.c_str()) == 0)
			return m.mint;
	}
	return 0;
}
}

time_t 
gmtToTime(const std::string& gmt_time)
{
	try {
		struct tm timestamp;
		StringTokenizer st(gmt_time, ' ', ',');
		if(!st.hasMoreTokens())
			return 0;
		st.nextToken();			/**< 丢弃星期 */
		StringTokenizer data_st(st.nextToken(), '-');
		timestamp.tm_mday = boost::lexical_cast<unsigned short>(data_st.nextToken());
		short month = get_int_month(data_st.nextToken());
		if(!month)
			return 0;
		timestamp.tm_mon = month - 1;
		timestamp.tm_year = boost::lexical_cast<unsigned short>(data_st.nextToken()) - 1900;
		if(timestamp.tm_year < 0)				/**< DD-MM-YY */
			timestamp.tm_year += 2000;		
		StringTokenizer time_st(st.nextToken(), ':');
		/** 由于mktime参数为local时间而非UTC时间, 因此将utc时间+8小时转换为local时间 */
		timestamp.tm_hour = boost::lexical_cast<unsigned short>(time_st.nextToken()) + 8;
		timestamp.tm_min = boost::lexical_cast<unsigned short>(time_st.nextToken());
		timestamp.tm_sec = boost::lexical_cast<unsigned short>(time_st.nextToken());
		return mktime(&timestamp);
	} catch(boost::bad_lexical_cast &e) {
		Log("DEBUG") << __FILE__ << ":" << __LINE__;
		Log("ERROR") << e.what();
		return 0;
	} catch(TokenError& e) {
		Log("DEBUG") << __FILE__ << ":" << __LINE__;
		Log("ERROR") << e;
		return 0;	
	}
}				
