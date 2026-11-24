/**
 * @file standaloneDriver.cxx
 *
 * A replacement for libFuzzer's main(), for a compiler that does not have
 * libFuzzer (gcc). It does what libFuzzer needs of a driver and a little of
 * what the fuzzer does:
 *
 *   fuzzDwpInput file...                      replay each file
 *   fuzzDwpInput -mutate=N [-seed=S] [-last=F] file...
 *                                             replay each file, then run N
 *                                             random mutations of them, and
 *                                             keep the input being run in F
 *
 * The mutations are the simple ones (flip a byte, insert a token or a byte,
 * delete or repeat a range, splice two inputs). It does not follow coverage,
 * so it finds less than libFuzzer, but it needs nothing but a compiler, and
 * with the sanitizers on it finds the same kind of defect.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2025 Wizzer Works
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <random>
#include <string>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerInitialize(int *argc, char ***argv);

static bool readFile(const char *path, std::string &text)
{
	FILE *fp = fopen(path, "rb");
	if (! fp)
		return false;
	char buffer[65536];
	size_t n;
	text.clear();
	while ((n = fread(buffer, 1, sizeof(buffer), fp)) > 0)
		text.append(buffer, n);
	fclose(fp);
	return true;
}

static const char *g_tokens[] = {
	"( ", " )", "(", ")", "\"", "\\", "#", "\n", " ", "-", "+", "1e999", "0x", ",", "[", "]",
	"Actor", "ActorDef", "ActorTemplate", "Group", "Scene", "Stage", "Set", "SetDef",
	"RoleBinding", "RoleSetMapping", "RoleDef", "Property", "PropertyDef", "Package",
	"Media", "MediaRef", "Converter", "Filetype", "Limits", "Container", "Bogus",
	"MlVector2", "MlVector3", "MlVector4", "MlRotation", "MlTransform", "string",
	"int", "float", "FloatArray", "IntArray", "enum", "#DWP 1.0 ascii\n",
};

static std::string mutate(const std::vector<std::string> &seeds, std::mt19937 &rng)
{
	std::string s = seeds[rng() % seeds.size()];
	int rounds = 1 + rng() % 4;
	for (int i = 0; i < rounds; i++) {
		size_t at = s.empty() ? 0 : rng() % s.size();
		switch (rng() % 7) {
		case 0:	if (! s.empty()) s[at] ^= 1 << (rng() % 8); break;
		case 1:	if (! s.empty()) s[at] = (char)rng(); break;
		case 2:	s.insert(at, 1, (char)rng()); break;
		case 3:	s.insert(at, g_tokens[rng() % (sizeof(g_tokens) / sizeof(*g_tokens))]); break;
		case 4:	if (! s.empty()) s.erase(at, 1 + rng() % 16); break;
		case 5:	if (! s.empty()) { size_t len = 1 + rng() % 32; s.insert(at, s.substr(at, len)); } break;
		case 6: {
			const std::string &o = seeds[rng() % seeds.size()];
			if (! o.empty()) s = s.substr(0, at) + o.substr(rng() % o.size());
			break;
		}
		}
		if (s.size() > 65536)
			s.resize(65536);
	}
	return s;
}

int main(int argc, char **argv)
{
	long mutations = 0;
	unsigned seed = 1;
	const char *last = NULL;
	std::vector<std::string> inputs;

	LLVMFuzzerInitialize(&argc, &argv);
	for (int i = 1; i < argc; i++) {
		if (strncmp(argv[i], "-mutate=", 8) == 0) {
			mutations = atol(argv[i] + 8);
		} else if (strncmp(argv[i], "-seed=", 6) == 0) {
			seed = (unsigned)atol(argv[i] + 6);
		} else if (strncmp(argv[i], "-last=", 6) == 0) {
			last = argv[i] + 6;
		} else {
			std::string text;
			if (! readFile(argv[i], text)) {
				fprintf(stderr, "can't read %s\n", argv[i]);
				return 2;
			}
			inputs.push_back(text);
			LLVMFuzzerTestOneInput((const uint8_t *)text.data(), text.size());
		}
	}
	fprintf(stderr, "replayed %zu inputs\n", inputs.size());

	if (mutations > 0 && ! inputs.empty()) {
		std::mt19937 rng(seed);
		for (long n = 0; n < mutations; n++) {
			std::string s = mutate(inputs, rng);
			// Keep the input being run, so that a crash can be replayed.
			if (last) {
				FILE *fp = fopen(last, "wb");
				if (fp) { fwrite(s.data(), 1, s.size(), fp); fclose(fp); }
			}
			LLVMFuzzerTestOneInput((const uint8_t *)s.data(), s.size());
			if ((n + 1) % 100000 == 0)
				fprintf(stderr, "%ld mutations\n", n + 1);
		}
		fprintf(stderr, "%ld mutations\n", mutations);
	}
	return 0;
}
