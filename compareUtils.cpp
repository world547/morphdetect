#include "compareUtils.h"

/**
 * Copies bytes from src to dst ignoring 0x0 and 0x90
 */

size_t CompareUtils::cleanup(mbyte *dst, const mbyte *src, size_t src_len) {
	size_t dst_size = 0;
	for (size_t i = 0; i < src_len; i++) {
		switch (src[i]) {
			case 0x0: // null
			case 0x90: // nop
				continue;
		}
		if (dst != NULL) {
			dst[dst_size] = src[i];
		}
		dst_size++;
	}
	return dst_size;
}

/**
 * Counts matching blocks in the samples compared (for N-gram). Makes it via statistics. Counts each time statistics for model.
 */

size_t CompareUtils::compare_simple(unordered_map<mblock, size_t> &sample_stat, Sample &shellcode)
{
	unordered_map<mblock, size_t> model_stat;
	const mbyte *b = shellcode.data;
	for (size_t i = 0; i <= shellcode.size - sizeof(mblock); i++, b++)
		model_stat[*(const mblock *) b]++;

	size_t count = 0;
	for (auto &pair : model_stat)
		count += min(pair.second, sample_stat[pair.first]);
	return count;
}

/**
 * Division of analyzed data before LCS. Returns maximum matched bytes.
 */

size_t CompareUtils::compare_diff(MemoryBlock &sample, Sample &shellcode, float threshold)
{
	if ((shellcode.size == 0) || (sample.size == 0)) {
		cerr << "WHOOPS! " << shellcode.size << " " << sample.size << endl;
		return 0;
	}
	size_t step_size = shellcode.size * 2;
	if (sample.size <= 2 * step_size) {
		if (!possible_diff(sample.data, sample.size, shellcode, threshold))
			return 0;
		return longest_common_subsequence(sample.data, sample.size, shellcode.data, shellcode.size);
	}
	size_t last = sample.size - 2 * step_size;

	size_t res = 0;
	for (size_t i = 0; i < sample.size - step_size; i += step_size) {
		const mbyte *data = sample.data + min(i, last);
		size_t data_size = 2 * step_size;
		if (!possible_diff(data, data_size, shellcode, threshold))
			continue;
		res = max(res, longest_common_subsequence(data, data_size, shellcode.data, shellcode.size));
	}
	return res;
}

/**
 * Counts bytes to determine if LCS could be sufficiently large. Model is a sample of shellcode here.
 */

bool CompareUtils::possible_diff(const mbyte *sample, size_t sample_size, Sample &shellcode, float threshold)
{
	uint32_t stat_sample[256] = {0}, stat_model[256] = {0};

	for (size_t i = 0; i < sample_size; i++)
		stat_sample[sample[i]]++;
	for (size_t i = 0; i < shellcode.size; i++)
		stat_model[shellcode.data[i]]++;

	int total = 0;
	for (size_t i = 0; i < 256; i++)
		total += min(stat_model[i], stat_sample[i]);

	return total >= threshold * shellcode.size;
}
