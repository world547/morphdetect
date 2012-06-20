#include "analyzerNgram.h"
#include "timer.h"

#define THRESHOLD 0.5

AnalyzerNgram::AnalyzerNgram()
{
	_className = "AnalyzerNgram";
}

AnalyzerNgram::AnalyzerNgram(const unsigned char* data, uint size) 
	: Analyzer(data, size)
{
	_className = "AnalyzerNgram";
}

string AnalyzerNgram::analyze()
{
	TimerAnalyzer::start(TimeMatch);
	float max_coef = 0;
	int max_ans = 0, ind_max = 0;

	for (unsigned int i = 0; i < _shellcodes.size(); i++)
	{
		int ans = _data.compareNgram(_shellcodes[i]);
		float coef = ans * 1.0 / _shellcodes[i].size;

		if (coef > THRESHOLD)
		{
			if (coef > max_coef)
			{
				max_coef = coef;
			}
			if (ans > max_ans)
			{
				max_ans = ans;
				ind_max = i;
			}
		}
	}
	TimerAnalyzer::stop(TimeMatch);
	return (max_coef <= THRESHOLD) ? string() : _shellcodes[ind_max].name;
}
