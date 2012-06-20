#include "analyzerDiff.h"
#include "timer.h"

using namespace std;

#define THRESHOLD 0.5

AnalyzerDiff::AnalyzerDiff()
{
	_className = "AnalyzerDiff";
}

AnalyzerDiff::AnalyzerDiff(const unsigned char* data, uint size) 
	: Analyzer(data, size)
{
	_className = "AnalyzerDiff";
}

string AnalyzerDiff::analyze()
{
	TimerAnalyzer::start(TimeMatch);
	float max_coef = 0;
	int max_ans = 0, ind_max = 0;

	for (unsigned int i = 0; i < _shellcodes.size(); i++)
	{
		int ans = _data.compareDiff(_shellcodes[i], THRESHOLD);
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
