// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2016-2018 Wizzer Works
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

// Tests of reading and writing whole workprints: a corpus of real workprints
// is read, written, and read again; the output is compared to golden files; the
// two output sinks are compared; and malformed input is rejected.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "gtest/gtest.h"

// Include Magic Lantern Digital Workprint header files.
#include <mle/Dwp.h>
#include <mle/DwpInclude.h>
#include <mle/DwpInput.h>
#include <mle/DwpItem.h>
#include <mle/DwpOutput.h>

// Include Digital Workprint test header files.
#include "libdwptest.h"

namespace roundtrip {

// The files of the corpus, sorted.
std::vector<std::string> corpusFiles() {
	std::vector<std::string> names;
	std::error_code ec;
	for (const auto &entry : std::filesystem::directory_iterator(dwptest::workprintPath("corpus"), ec))
		if (entry.is_regular_file())
			names.push_back(entry.path().filename().string());
	std::sort(names.begin(), names.end());
	return names;
}

std::string corpusPath(const std::string &name) { return dwptest::workprintPath("corpus") + "/" + name; }

std::string testName(const ::testing::TestParamInfo<std::string> &info) {
	std::string name;
	for (char c : info.param)
		name += isalnum((unsigned char)c) ? c : '_';
	return name;
}

std::string readFile(const std::string &path) {
	std::ifstream in(path, std::ios::binary);
	return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}

struct Loaded {
	dwptest::WorkprintPtr root;
	std::string errors;   // What was reported on stderr.
};

Loaded loadFile(const std::string &path) {
	Loaded l;
	::testing::internal::CaptureStderr();
	l.root = dwptest::loadFile(path);
	l.errors = ::testing::internal::GetCapturedStderr();
	return l;
}

Loaded loadText(const std::string &text) {
	Loaded l;
	::testing::internal::CaptureStderr();
	l.root = dwptest::loadString(text);
	l.errors = ::testing::internal::GetCapturedStderr();
	return l;
}

// The kind, name and tags of every item, one per line and indented by depth.
// The name of the top item is the file it was read from, so it is left out.
void describe(MleDwpItem *item, int depth, std::string &out) {
	for (MleDwpItem *c = item->getFirstChild(); c; c = c->getNext()) {
		out += std::string(depth * 2, ' ') + c->getTypeName() + " " + c->getName();
		for (int i = 0; c->getNthTag(i) != NULL; i++)
			out += std::string(" +") + c->getNthTag(i);
		out += "\n";
		describe(c, depth + 1, out);
	}
}

std::string describe(MleDwpItem *root) {
	std::string out;
	describe(root, 0, out);
	return out;
}

// An include writes its children to its own file. The tests must never write
// to the files that the corpus names, so they are not written.
class RoundTripFixture {
  public:
	RoundTripFixture() {
		m_saved = MleDwpInclude::getClassWriteChildren();
		MleDwpInclude::setClassWriteChildren(MleDwpInclude::WRITE_NONE);
	}
	~RoundTripFixture() { MleDwpInclude::setClassWriteChildren(m_saved); }
  private:
	MleDwpInclude::Enabling m_saved;
};

// True if every error in the text is about a file that was included.
bool onlyIncludeErrors(const std::string &errors) {
	std::istringstream lines(errors);
	std::string line;
	while (std::getline(lines, line))
		if (line.find("DWP input error") != std::string::npos &&
			line.find("error opening included file") == std::string::npos)
			return false;
	return true;
}

} // namespace roundtrip

using namespace roundtrip;

// ---------------------------------------------------------------------------
// The corpus.

class CorpusTest : public ::testing::TestWithParam<std::string> {
  protected:
	RoundTripFixture fixture;
};

TEST_P(CorpusTest, LoadsWithOnlyMissingIncludesReported) {
	// The workprints name included files that are not here, so those errors
	// are expected, but nothing else is.
	Loaded l = loadFile(corpusPath(GetParam()));
	ASSERT_TRUE(l.root != NULL);
	EXPECT_TRUE(onlyIncludeErrors(l.errors)) << l.errors;
	EXPECT_TRUE(l.root->getFirstChild() != NULL || readFile(corpusPath(GetParam())).size() < 40);
}

TEST_P(CorpusTest, WriteThenReadGivesTheSameWorkprint) {
	Loaded first = loadFile(corpusPath(GetParam()));
	ASSERT_TRUE(first.root != NULL);
	std::string text1 = dwptest::writeString(first.root.get());

	Loaded second = loadText(text1);
	ASSERT_TRUE(second.root != NULL) << text1;
	EXPECT_TRUE(onlyIncludeErrors(second.errors)) << second.errors;
	EXPECT_EQ(describe(first.root.get()), describe(second.root.get()));
}

TEST_P(CorpusTest, WritingIsStable) {
	// What is written is read as what was written, so a second pass changes
	// nothing.
	Loaded first = loadFile(corpusPath(GetParam()));
	ASSERT_TRUE(first.root != NULL);
	std::string text1 = dwptest::writeString(first.root.get());
	Loaded second = loadText(text1);
	ASSERT_TRUE(second.root != NULL);
	std::string text2 = dwptest::writeString(second.root.get());
	EXPECT_EQ(text1, text2);
}

TEST_P(CorpusTest, FileAndBufferSinksAgree) {
	Loaded l = loadFile(corpusPath(GetParam()));
	ASSERT_TRUE(l.root != NULL);
	std::string buffer = dwptest::writeString(l.root.get());

	std::string path = std::string("/tmp/dwproundtrip") + std::to_string(getpid()) + "_" + GetParam();
	{
		MleDwpOutput out;
		ASSERT_EQ(0, out.openFile(path.c_str()));
		l.root->write(&out);
	}
	std::string file = readFile(path);
	std::remove(path.c_str());
	EXPECT_EQ(buffer, file);
}

TEST_P(CorpusTest, FilePointerSinkAgrees) {
	Loaded l = loadFile(corpusPath(GetParam()));
	ASSERT_TRUE(l.root != NULL);
	std::string buffer = dwptest::writeString(l.root.get());

	std::string path = std::string("/tmp/dwproundtripfp") + std::to_string(getpid()) + "_" + GetParam();
	FILE *fp = fopen(path.c_str(), "w");
	ASSERT_TRUE(fp != NULL);
	{
		MleDwpOutput out;
		out.setFilePointer(fp);
		l.root->write(&out);
		out.flush();
	}
	fclose(fp);
	std::string file = readFile(path);
	std::remove(path.c_str());
	EXPECT_EQ(buffer, file);
}

TEST_P(CorpusTest, FileAndBufferSourcesAgree) {
	// The same workprint read from a file, from a string and from a FILE.
	std::string path = corpusPath(GetParam());
	Loaded fromFile = loadFile(path);
	Loaded fromString = loadText(readFile(path));
	ASSERT_TRUE(fromFile.root != NULL);
	ASSERT_TRUE(fromString.root != NULL);
	EXPECT_EQ(describe(fromFile.root.get()), describe(fromString.root.get()));

	// A workprint with an include is not read from a FILE: see
	// InputFromAFilePointerTest.
	if (readFile(path).find("Include") != std::string::npos)
		return;
	FILE *fp = fopen(path.c_str(), "r");
	ASSERT_TRUE(fp != NULL);
	MleDwpInput in;
	::testing::internal::CaptureStderr();
	ASSERT_EQ(0, in.setFilePointer(fp));
	dwptest::WorkprintPtr fromFp(MleDwpItem::readAll(&in));
	::testing::internal::GetCapturedStderr();
	fclose(fp);
	ASSERT_TRUE(fromFp != NULL);
	EXPECT_EQ(describe(fromFile.root.get()), describe(fromFp.get()));
}

TEST_P(CorpusTest, CopyOfEachItemWritesTheSameText) {
	Loaded l = loadFile(corpusPath(GetParam()));
	ASSERT_TRUE(l.root != NULL);
	for (MleDwpItem *item = l.root->getFirstChild(); item; item = item->getNext()) {
		dwptest::WorkprintPtr copy(item->copy());
		ASSERT_TRUE(copy != NULL);
		EXPECT_EQ(dwptest::writeString(item), dwptest::writeString(copy.get())) << item->getName();
	}
}

INSTANTIATE_TEST_SUITE_P(Workprints, CorpusTest, ::testing::ValuesIn(corpusFiles()), testName);

TEST(CorpusFilesTest, TheCorpusIsThere) {
	EXPECT_GE(corpusFiles().size(), 30u);
}

// ---------------------------------------------------------------------------
// Reading from a FILE.

TEST(InputFromAFilePointerTest, IncludeDoesNotEndTheInput) {
	// When an included file is closed the input goes back to the file pointer
	// it was given. It was set to NULL, which was an assertion in a debug
	// build, and "unexpected end of file" with the rest of the workprint lost
	// otherwise.
	char dirName[] = "/tmp/dwpfpXXXXXX";
	ASSERT_TRUE(mkdtemp(dirName) != NULL);
	std::string dir = dirName;
	{ std::ofstream(dir + "/inc.dwp") << "#DWP 1.0 ascii\n( ActorDef inc )\n"; }
	{ std::ofstream(dir + "/main.dwp") << "#DWP 1.0 ascii\n( Include " << dir << "/inc.dwp )\n( ActorDef after )\n"; }
	// Read in a process of its own, because a debug build asserts.
	EXPECT_EXIT({
			FILE *fp = fopen((dir + "/main.dwp").c_str(), "r");
			MleDwpInput in;
			in.setFilePointer(fp);
			dwptest::WorkprintPtr root(MleDwpItem::readAll(&in));
			size_t n = 0;
			for (MleDwpItem *c = root->getFirstChild(); c; c = c->getNext())
				n++;
			exit(n == 2 ? 0 : 1);
		}, ::testing::ExitedWithCode(0), "");
	std::filesystem::remove_all(dir);
}

// ---------------------------------------------------------------------------
// A workprint that is full of errors: the errors are reported, and what can be
// read is kept.

TEST(MediaTestWorkprint, ErrorsAreReportedAndTheRestIsRead) {
	Loaded l = loadFile(dwptest::workprintPath("MediaTest.dwp"));
	ASSERT_TRUE(l.root != NULL);

	// Four properties have no type.
	size_t errors = 0;
	for (size_t pos = l.errors.find("error reading Property type"); pos != std::string::npos;
			pos = l.errors.find("error reading Property type", pos + 1))
		errors++;
	EXPECT_EQ(4u, errors) << l.errors;

	// The groups, actors and media reference are all there, with the item that
	// is not known kept as it was read.
	std::string tree = describe(l.root.get());
	EXPECT_NE(std::string::npos, tree.find("MleDwpGroup g0")) << tree;
	EXPECT_NE(std::string::npos, tree.find("MleDwpActor actor0")) << tree;
	EXPECT_NE(std::string::npos, tree.find("MleDwpActor actor1")) << tree;
	EXPECT_NE(std::string::npos, tree.find("MleDwpMediaRef TeapotModel")) << tree;
}

TEST(MediaTestWorkprint, ALineBeforeTheHeaderIsNotAWorkprint) {
	// The original of MediaTest.dwp started with a line of program output.
	std::string text = "substituting Unknown item for bleah.\n" + readFile(dwptest::workprintPath("MediaTest.dwp"));
	Loaded l = loadText(text);
	EXPECT_TRUE(l.root == NULL);
	EXPECT_NE(std::string::npos, l.errors.find("no magic")) << l.errors;
}

TEST(CopyOfTheRootTest, WritesTheSameTextAsTheRoot) {
	// The root of a workprint is an Include that does not write itself, and
	// its copy is the same kind of item. It was an ordinary Include, which was
	// written as ( Include file ).
	Loaded l = loadText("#DWP 1.0 ascii\n( ActorDef a )\n( ActorDef b )\n");
	ASSERT_TRUE(l.root != NULL);
	dwptest::WorkprintPtr copy(l.root->copy());
	ASSERT_TRUE(copy != NULL);
	EXPECT_EQ(dwptest::writeString(l.root.get()), dwptest::writeString(copy.get()));
}

// ---------------------------------------------------------------------------
// Golden files: what is written for a workprint is compared to a file that
// was checked in, so a change to the output is noticed. If the change is meant,
// run with DWPTEST_UPDATE_GOLDEN set to write the new files, and check the
// differences.

namespace roundtrip {
struct GoldenCase { const char *source; const char *golden; };
const GoldenCase g_golden[] = {
	{ "corpus/mle-core-dwp_HelloWorld.dwp",     "golden/HelloWorld.dwp" },
	{ "corpus/mle-studio_TestDWPProperties.dwp", "golden/TestDWPProperties.dwp" },
	{ "corpus/mle-titles_mtea.dwp",             "golden/mtea.dwp" },
	{ "corpus/mle-core-dpp_test.dwp",           "golden/test.dwp" },
	{ "corpus/mle-parts_MleBr3dSet.wpf",        "golden/MleBr3dSet.wpf" },
	{ "MediaTest.dwp",                          "golden/MediaTest.dwp" },
};
std::string goldenName(const ::testing::TestParamInfo<GoldenCase> &info) {
	return testName(::testing::TestParamInfo<std::string>(info.param.golden, info.index));
}
}

class GoldenTest : public ::testing::TestWithParam<GoldenCase> {
  protected:
	RoundTripFixture fixture;
};

TEST_P(GoldenTest, OutputIsAsCheckedIn) {
	Loaded l = loadFile(dwptest::workprintPath(GetParam().source));
	ASSERT_TRUE(l.root != NULL);
	std::string text = dwptest::writeString(l.root.get());
	std::string path = dwptest::workprintPath(GetParam().golden);

	if (getenv("DWPTEST_UPDATE_GOLDEN") != NULL) {
		std::filesystem::create_directories(std::filesystem::path(path).parent_path());
		std::ofstream(path, std::ios::binary) << text;
		GTEST_SKIP() << "Wrote " << path;
	}
	std::string expected = readFile(path);
	ASSERT_FALSE(expected.empty()) << "Missing " << path;
	EXPECT_EQ(expected, text);
}

INSTANTIATE_TEST_SUITE_P(Workprints, GoldenTest, ::testing::ValuesIn(g_golden), goldenName);

// ---------------------------------------------------------------------------
// The header.

namespace {
std::string headerLine(const std::string &text) { return text.substr(0, text.find('\n')); }
}

TEST(HeaderTest, OutputStartsWithTheMagicLine) {
	MleDwpOutput out;
	out.writeString("( ActorDef a )");
	EXPECT_EQ("#DWP 1.0 ascii ", headerLine(out.getBuffer()));
}

TEST(HeaderTest, MagicCanBeLeftOut) {
	MleDwpOutput out(0);
	out.writeString("( ActorDef a )");
	std::string text = out.getBuffer();
	EXPECT_EQ(std::string::npos, text.find("#DWP")) << text;
	EXPECT_EQ(0u, text.find("( ActorDef a )"));
}

TEST(HeaderTest, SetMagicChangesWhatIsWritten) {
	MleDwpOutput out;
	out.setMagic(0);
	out.writeString("x");
	EXPECT_EQ(std::string::npos, std::string(out.getBuffer()).find("#DWP"));
}

TEST(HeaderTest, HeaderIsWrittenOnlyOnce) {
	MleDwpOutput out;
	out.writeString("a");
	out.writeTerminal();
	out.writeString("b");
	std::string text = out.getBuffer();
	EXPECT_EQ(text.find("#DWP"), text.rfind("#DWP"));
}

TEST(HeaderTest, InputNeedsTheHeader) {
	Loaded l = loadText("( ActorDef a )\n");
	EXPECT_TRUE(l.root == NULL);
	EXPECT_NE(std::string::npos, l.errors.find("no magic")) << l.errors;
}

TEST(HeaderTest, InputWithoutMagicCheckReadsAHeaderlessWorkprint) {
	MleDwpInput in(0);
	std::string text = "( ActorDef a )\n";
	ASSERT_EQ(0, in.setBuffer(text.c_str()));
	dwptest::WorkprintPtr root(MleDwpItem::readAll(&in));
	ASSERT_TRUE(root != NULL);
	ASSERT_TRUE(root->getFirstChild() != NULL);
	EXPECT_STREQ("a", root->getFirstChild()->getName());
}

TEST(HeaderTest, SetMagicOnInputChangesTheCheck) {
	MleDwpInput in;
	in.setMagic(0);
	std::string text = "( ActorDef a )\n";
	EXPECT_EQ(0, in.setBuffer(text.c_str()));
}

TEST(HeaderTest, WrongMagicIsRejected) {
	const char *bad[] = { "#DXP 1.0 ascii\n( ActorDef a )\n", "DWP 1.0 ascii\n( ActorDef a )\n",
		"# DWP 1.0 ascii\n( ActorDef a )\n", "#dwp 1.0 ascii\n( ActorDef a )\n", "#DW\n" };
	for (const char *text : bad) {
		Loaded l = loadText(text);
		EXPECT_TRUE(l.root == NULL) << text;
		EXPECT_NE(std::string::npos, l.errors.find("no magic")) << text << l.errors;
	}
}

TEST(HeaderTest, TrailingTextOnTheHeaderLineIsIgnored) {
	for (const char *header : { "#DWP 1.0 ascii", "#DWP 1.0 ascii ", "#DWP", "#DWP 2.0 anything at all" }) {
		Loaded l = loadText(std::string(header) + "\n( ActorDef a )\n");
		ASSERT_TRUE(l.root != NULL) << header;
		EXPECT_EQ("", l.errors) << header;
		EXPECT_TRUE(l.root->getFirstChild() != NULL) << header;
	}
}

TEST(HeaderTest, WindowsLineEndingsAreRead) {
	Loaded l = loadText("#DWP 1.0 ascii\r\n( ActorDef a )\r\n( ActorDef b )\r\n");
	ASSERT_TRUE(l.root != NULL);
	EXPECT_EQ("", l.errors);
	EXPECT_EQ("MleDwpActorDef a\nMleDwpActorDef b\n", describe(l.root.get()));
}

TEST(HeaderTest, EmptyWorkprintHasNoItems) {
	Loaded l = loadText("#DWP 1.0 ascii\n");
	ASSERT_TRUE(l.root != NULL);
	EXPECT_EQ("", l.errors);
	EXPECT_TRUE(l.root->getFirstChild() == NULL);
	EXPECT_EQ("#DWP 1.0 ascii \n", dwptest::writeString(l.root.get()).substr(0, 16));
}

TEST(HeaderTest, CommentsAreSkipped) {
	Loaded l = loadText("#DWP 1.0 ascii\n# a comment\n( ActorDef a )\n# another ( ActorDef x )\n( ActorDef b )\n");
	ASSERT_TRUE(l.root != NULL);
	EXPECT_EQ("", l.errors);
	EXPECT_EQ("MleDwpActorDef a\nMleDwpActorDef b\n", describe(l.root.get()));
}

// ---------------------------------------------------------------------------
// Negative input. Each case is read in a process of its own, so that a crash or
// a hang is a failure of that case only. It must finish, and must say what is
// wrong.

namespace roundtrip {
struct BadInput {
	const char *label;
	std::string text;          // The whole text, including the header.
	bool error;                // True if an error is reported.
};

std::string withHeader(const std::string &body) { return "#DWP 1.0 ascii\n" + body; }

// A workprint with groups nested to the given depth.
std::string nested(int depth) {
	std::string s = "#DWP 1.0 ascii\n";
	for (int i = 0; i < depth; i++) s += "( Group g MleGroup\n";
	for (int i = 0; i < depth; i++) s += ")\n";
	return s;
}

std::vector<BadInput> badInputs() {
	std::vector<BadInput> v;
	// Structure.
	v.push_back({ "UnterminatedItem",        withHeader("( ActorDef a"), true });
	v.push_back({ "UnterminatedNesting",     withHeader("( Scene s ( Group g )"), true });
	v.push_back({ "ExtraRightParen",         withHeader("( ActorDef a ) )"), true });
	v.push_back({ "LoneRightParen",          withHeader(")"), true });
	v.push_back({ "LoneLeftParen",           withHeader("("), true });
	v.push_back({ "EmptyItem",               withHeader("( )"), true });
	v.push_back({ "NumberForKeyword",        withHeader("( 123 )"), false });
	v.push_back({ "TextOutsideAnItem",       withHeader("hello ( ActorDef a )"), true });
	v.push_back({ "TagWithoutAName",         withHeader("( ActorDef + a )"), true });
	// Names that are not known are kept, so they are not errors.
	v.push_back({ "UnknownItemIsKept",       withHeader("( Bogus x y z ( ActorDef a ) )"), false });
	// Datatypes and values.
	v.push_back({ "UnknownDatatype",         withHeader("( Property p nosuchtype 1 )"), true });
	v.push_back({ "MissingDatatype",         withHeader("( Property p )"), true });
	v.push_back({ "TooFewVectorValues",      withHeader("( Property p MlVector3 1 2 )"), true });
	v.push_back({ "NotANumberForInt",        withHeader("( Property p int abc )"), true });
	v.push_back({ "IntTooLarge",             withHeader("( Property p int 99999999999999999999 )"), true });
	v.push_back({ "BadFloat",                withHeader("( Property p float 1.2.3 )"), true });
	v.push_back({ "NotANumberForFloat",      withHeader("( Property p float abc )"), true });
	v.push_back({ "BadVectorValue",          withHeader("( Property p MlVector2 1 x )"), true });
	v.push_back({ "UnterminatedString",      withHeader("( Property p string \"abc )\n( ActorDef a )"), true });
	v.push_back({ "ArrayWithoutBracket",     withHeader("( Property p FloatArray 1 2 3 )"), true });
	v.push_back({ "UnterminatedArray",       withHeader("( Property p FloatArray [ 1, 2"), true });
	v.push_back({ "ArrayWithoutComma",       withHeader("( Property p FloatArray [ 1 2 ] )"), true });
	v.push_back({ "ArrayBadElement",         withHeader("( Property p FloatArray [ 1, x ] )"), true });
	// Missing arguments to items.
	v.push_back({ "MediaRefWithoutArguments", withHeader("( MediaRef m )"), false });
	v.push_back({ "LimitsWithBadType",       withHeader("( Limits nosuchtype 1 2 )"), true });
	// Includes.
	v.push_back({ "IncludeOfAMissingFile",   withHeader("( Include /nonexistent/dir/none.dwp )"), true });
	v.push_back({ "IncludeWithoutAName",     withHeader("( Include )"), true });
	v.push_back({ "IncludeOfADirectory",     withHeader("( Include /tmp )"), true });
	// Input that is not text.
	v.push_back({ "BinaryBytes",             withHeader(std::string("( ActorDef \xff\xfe\x01\x02 )")), false });
	v.push_back({ "OnlyTheHeader",           "#DWP 1.0 ascii", false });
	v.push_back({ "EmptyInput",              "", true });
	v.push_back({ "TruncatedHeader",         "#D", true });
	v.push_back({ "HeaderCutShort",          "#DWP", false });
	v.push_back({ "OnlyWhitespace",          "   \n\n\t", true });
	// Sizes.
	v.push_back({ "VeryLongName",            withHeader("( ActorDef " + std::string(100000, 'a') + " )"), false });
	v.push_back({ "VeryLongStringValue",     withHeader("( Property p string \"" + std::string(100000, 'b') + "\" )"), false });
	v.push_back({ "VeryLongLine",            withHeader(std::string(100000, ' ') + "( ActorDef a )"), false });
	v.push_back({ "VeryLongComment",         withHeader("# " + std::string(100000, 'c') + "\n( ActorDef a )"), false });
	v.push_back({ "ManySiblings",            [] { std::string s = "#DWP 1.0 ascii\n"; for (int i = 0; i < 20000; i++) s += "( ActorDef a )\n"; return s; }(), false });
	v.push_back({ "DeepNesting",             nested(3000), true });
	v.push_back({ "VeryDeepNesting",         nested(100000), true });
	v.push_back({ "UnterminatedVeryDeepNesting", [] { std::string s = "#DWP 1.0 ascii\n"; for (int i = 0; i < 100000; i++) s += "( Group g MleGroup\n"; return s; }(), true });
	v.push_back({ "LongItemKeyword",         withHeader("( " + std::string(200, 'k') + " x )"), false });
	return v;
}

std::string badName(const ::testing::TestParamInfo<BadInput> &info) { return info.param.label; }

// Read the text, and leave with 0 if reading finished.
void readAndLeave(const std::string &text) {
	alarm(30);
	dwptest::WorkprintPtr root = dwptest::loadString(text);
	root.reset();
	exit(0);
}
}

class BadInputTest : public ::testing::TestWithParam<BadInput> {};

TEST_P(BadInputTest, ReadingFinishesAndTheErrorIsReported) {
	const BadInput &c = GetParam();
	if (c.error) {
		EXPECT_EXIT(readAndLeave(c.text), ::testing::ExitedWithCode(0), "DWP input error|no magic");
	} else {
		EXPECT_EXIT(readAndLeave(c.text), ::testing::ExitedWithCode(0), "");
	}
}

INSTANTIATE_TEST_SUITE_P(Input, BadInputTest, ::testing::ValuesIn(badInputs()), badName);

TEST(BadInputKeptTest, UnknownItemIsKeptAndWrittenBack) {
	Loaded l = loadText(withHeader("( Bogus x y z ( ActorDef a ) )\n( ActorDef after )\n"));
	ASSERT_TRUE(l.root != NULL);
	EXPECT_EQ("", l.errors);
	std::string text = dwptest::writeString(l.root.get());
	EXPECT_NE(std::string::npos, text.find("( Bogus x y z")) << text;
	EXPECT_NE(std::string::npos, text.find("( ActorDef a )")) << text;
	EXPECT_NE(std::string::npos, text.find("( ActorDef after )")) << text;
}

TEST(BadInputKeptTest, ItemsAfterAnErrorInAValueAreStillRead) {
	// An error in the value of an item does not stop the next item.
	const char *values[] = { "UnknownDatatype", "MissingDatatype", "TooFewVectorValues", "NotANumberForInt",
		"IntTooLarge", "BadFloat", "NotANumberForFloat", "BadVectorValue", "ArrayBadElement", "ArrayWithoutComma",
		"LimitsWithBadType" };
	size_t found = 0;
	for (const BadInput &c : badInputs()) {
		if (std::find(std::begin(values), std::end(values), std::string(c.label)) == std::end(values))
			continue;
		found++;
		Loaded l = loadText(c.text + "\n( ActorDef after )\n");
		ASSERT_TRUE(l.root != NULL) << c.label;
		EXPECT_NE(std::string::npos, describe(l.root.get()).find("MleDwpActorDef after"))
			<< c.label << "\n" << describe(l.root.get());
	}
	EXPECT_EQ(std::size(values), found);
}

TEST(BadInputKeptTest, AStrayRightParenEndsTheInput) {
	Loaded l = loadText(withHeader("( ActorDef a ) )\n( ActorDef after )\n"));
	ASSERT_TRUE(l.root != NULL);
	EXPECT_NE(std::string::npos, l.errors.find("extra right paren")) << l.errors;
	EXPECT_EQ("MleDwpActorDef a\n", describe(l.root.get()));
}

TEST(BadInputKeptTest, MissingFileGivesNoWorkprint) {
	::testing::internal::CaptureStderr();
	dwptest::WorkprintPtr root = dwptest::loadFile("/nonexistent/dir/missing.dwp");
	::testing::internal::GetCapturedStderr();
	EXPECT_TRUE(root == NULL);
}

TEST(BadInputKeptTest, EmbeddedNulEndsTheInput) {
	// A NUL in the middle of a file is read as the end of the input.
	std::string text = withHeader("( ActorDef a )\n") + std::string(1, '\0') + "( ActorDef b )\n";
	std::string path = "/tmp/dwpnul" + std::to_string(getpid()) + ".dwp";
	{ std::ofstream out(path, std::ios::binary); out.write(text.data(), text.size()); }
	Loaded l = loadFile(path);
	std::remove(path.c_str());
	ASSERT_TRUE(l.root != NULL);
	EXPECT_EQ("MleDwpActorDef a\n", describe(l.root.get()));
}

// ---------------------------------------------------------------------------
// An item keyword is also the name of a library that may define the item, and
// the names of the library and its entry point are built in buffers of 256
// bytes. A keyword that is too long to be a file name is an unknown item.

TEST(ItemKeywordTest, LongKeywordsAreUnknownItems) {
	for (size_t length : { 100, 249, 250, 252, 253, 256, 300, 1000 }) {
		std::string keyword(length, 'k');
		EXPECT_EXIT(readAndLeave(withHeader("( " + keyword + " x )")), ::testing::ExitedWithCode(0), "") << length;
	}
}

TEST(ItemKeywordTest, LongKeywordIsKeptAndWrittenBack) {
	std::string keyword(300, 'k');
	Loaded l;
	std::string text;
	// In a process of its own, as a bug in a library would be a crash.
	EXPECT_EXIT({
			dwptest::WorkprintPtr root = dwptest::loadString(withHeader("( " + keyword + " x )\n( ActorDef after )\n"));
			std::string out = dwptest::writeString(root.get());
			bool kept = out.find("( " + keyword + " x )") != std::string::npos &&
				out.find("( ActorDef after )") != std::string::npos;
			exit(kept ? 0 : 1);
		}, ::testing::ExitedWithCode(0), "");
}

// ---------------------------------------------------------------------------
// Nesting. Items are read, written, copied and deleted recursively, so a
// workprint can be nested only so deep. The limit is 255 items inside the root.

namespace roundtrip {
// The number of items down the first child of each, from the root.
int depthOf(MleDwpItem *root) {
	int depth = 0;
	for (MleDwpItem *item = root->getFirstChild(); item; item = item->getFirstChild())
		depth++;
	return depth;
}
}

TEST(NestingTest, DeepButNotTooDeepIsRead) {
	Loaded l = loadText(nested(255));
	ASSERT_TRUE(l.root != NULL);
	EXPECT_EQ("", l.errors);
	EXPECT_EQ(255, depthOf(l.root.get()));
}

TEST(NestingTest, OneTooManyIsReportedAndLeftEmpty) {
	Loaded l = loadText(nested(256));
	ASSERT_TRUE(l.root != NULL);
	EXPECT_NE(std::string::npos, l.errors.find("items are nested too deeply")) << l.errors.substr(0, 300);
	EXPECT_EQ(256, depthOf(l.root.get()));
	// What is past the limit is not read.
	MleDwpItem *last = l.root.get();
	while (last->getFirstChild()) last = last->getFirstChild();
	EXPECT_TRUE(last->getFirstChild() == NULL);
}

TEST(NestingTest, TooDeepIsReportedOnceAndTheRestIsRead) {
	// The items past the limit are skipped as a group, and reading goes on with
	// what follows them.
	Loaded l = loadText(nested(1000) + "( ActorDef after )\n");
	ASSERT_TRUE(l.root != NULL);
	size_t errors = 0;
	for (size_t pos = l.errors.find("items are nested too deeply"); pos != std::string::npos;
			pos = l.errors.find("items are nested too deeply", pos + 1))
		errors++;
	EXPECT_EQ(1u, errors) << l.errors.substr(0, 300);
	EXPECT_EQ(2u, [&] { size_t n = 0; for (MleDwpItem *c = l.root->getFirstChild(); c; c = c->getNext()) n++; return n; }());
	EXPECT_STREQ("after", l.root->getLastChild()->getName());
}

TEST(NestingTest, TheSkippedItemsAreNotMistakenForTheEndOfAnItem) {
	// Parentheses in a string are not the end of an item.
	std::string text = nested(255);
	text.insert(text.find("MleGroup\n", text.rfind("( Group")) + 9, "( Property s string \"))((\" ) ( Actor a b )\n");
	// Put the extra items in the deepest group, which is too deep for them.
	Loaded l = loadText(text + "( ActorDef after )\n");
	ASSERT_TRUE(l.root != NULL);
	EXPECT_STREQ("after", l.root->getLastChild()->getName());
}

TEST(NestingTest, SiblingsDoNotCountAsDepth) {
	std::string text = "#DWP 1.0 ascii\n( Group g MleGroup\n";
	for (int i = 0; i < 5000; i++)
		text += "( Group h MleGroup )\n";
	text += ")\n";
	Loaded l = loadText(text);
	ASSERT_TRUE(l.root != NULL);
	EXPECT_EQ("", l.errors);
	EXPECT_EQ(2, depthOf(l.root.get()));
}

TEST(NestingTest, DeepestWorkprintCanBeWrittenCopiedAndDeleted) {
	Loaded l = loadText(nested(255));
	ASSERT_TRUE(l.root != NULL);
	std::string text = dwptest::writeString(l.root.get());
	dwptest::WorkprintPtr copy(l.root->getFirstChild()->copy());
	ASSERT_TRUE(copy != NULL);
	EXPECT_EQ(text.substr(0, 40), dwptest::writeString(copy.get()).substr(0, 40));
	Loaded again = loadText(text);
	ASSERT_TRUE(again.root != NULL);
	EXPECT_EQ(255, depthOf(again.root.get()));
	// Everything is deleted when the roots go out of scope.
}

TEST(NestingTest, IncludesCountToo) {
	// A file that is included is read inside the item that includes it.
	char dirName[] = "/tmp/dwpnestXXXXXX";
	ASSERT_TRUE(mkdtemp(dirName) != NULL);
	std::string dir = dirName;
	std::string text = "#DWP 1.0 ascii\n( ActorDef end )\n";
	for (int i = 0; i < 300; i++) {
		std::string name = dir + "/f" + std::to_string(i) + ".dwp";
		std::ofstream(name) << text;
		text = "#DWP 1.0 ascii\n( Include " + name + " )\n";
	}
	RoundTripFixture fixture;
	Loaded l = loadText(text);
	std::filesystem::remove_all(dir);
	ASSERT_TRUE(l.root != NULL);
	EXPECT_NE(std::string::npos, l.errors.find("items are nested too deeply")) << l.errors.substr(0, 300);
}

// ---------------------------------------------------------------------------
// The end of the input before the header is complete.

TEST(HeaderTest, InputThatEndsBeforeTheHeaderIsNotAWorkprint) {
	for (const char *text : { "", "#", "#D", "#DW" }) {
		Loaded l = loadText(text);
		EXPECT_TRUE(l.root == NULL) << "[" << text << "]";
		EXPECT_NE(std::string::npos, l.errors.find("no magic")) << "[" << text << "]" << l.errors;
	}
}

TEST(HeaderTest, HeaderWithNothingAfterItIsAnEmptyWorkprint) {
	for (const char *text : { "#DWP", "#DWP\n", "#DWP 1.0 ascii" }) {
		Loaded l = loadText(text);
		ASSERT_TRUE(l.root != NULL) << text;
		EXPECT_EQ("", l.errors) << text;
		EXPECT_TRUE(l.root->getFirstChild() == NULL) << text;
	}
}

TEST(HeaderTest, EmptyFileIsNotAWorkprint) {
	std::string path = "/tmp/dwpempty" + std::to_string(getpid()) + ".dwp";
	{ std::ofstream out(path); }
	Loaded l = loadFile(path);
	std::remove(path.c_str());
	EXPECT_TRUE(l.root == NULL);
}

TEST(HeaderTest, DirectoryIsNotAWorkprint) {
	Loaded l = loadFile("/tmp");
	EXPECT_TRUE(l.root == NULL);
}

TEST(HeaderTest, EmptyIncludedFileIsReported) {
	char dirName[] = "/tmp/dwpemptyincXXXXXX";
	ASSERT_TRUE(mkdtemp(dirName) != NULL);
	std::string dir = dirName;
	{ std::ofstream out(dir + "/empty.dwp"); }
	Loaded l = loadText(withHeader("( Include " + dir + "/empty.dwp )\n( ActorDef after )\n"));
	std::filesystem::remove_all(dir);
	ASSERT_TRUE(l.root != NULL);
	EXPECT_NE(std::string::npos, l.errors.find("error opening included file")) << l.errors;
	EXPECT_STREQ("after", l.root->getLastChild()->getName());
}

// ---------------------------------------------------------------------------
// An include that could not be read is saved as the include it was.

namespace roundtrip {
std::string tempDir(const char *name) {
	std::string pattern = std::string("/tmp/") + name + "XXXXXX";
	std::vector<char> buffer(pattern.begin(), pattern.end());
	buffer.push_back('\0');
	if (mkdtemp(buffer.data()) == NULL)
		return "";
	return buffer.data();
}
}

TEST(IncludeSaveTest, FileThatCouldNotBeReadIsNotEmptied) {
	std::string dir = tempDir("dwpkeep");
	ASSERT_FALSE(dir.empty());
	std::string inc = dir + "/notaworkprint.txt";
	{ std::ofstream(inc) << "Notes that are not a workprint.\n"; }
	Loaded l = loadText(withHeader("( Include " + inc + " )\n"));
	ASSERT_TRUE(l.root != NULL);
	EXPECT_NE(std::string::npos, l.errors.find("error opening included file")) << l.errors;
	std::string text = dwptest::writeString(l.root.get());
	std::string after = readFile(inc);
	std::filesystem::remove_all(dir);
	EXPECT_EQ("Notes that are not a workprint.\n", after);
	// The include is still in the workprint.
	EXPECT_NE(std::string::npos, text.find("( Include " + inc + " )")) << text;
}

TEST(IncludeSaveTest, MissingFileIsNotCreated) {
	std::string dir = tempDir("dwpmissing");
	ASSERT_FALSE(dir.empty());
	std::string inc = dir + "/missing.dwp";
	Loaded l = loadText(withHeader("( Include " + inc + " )\n"));
	ASSERT_TRUE(l.root != NULL);
	dwptest::writeString(l.root.get());
	bool created = std::filesystem::exists(inc);
	std::filesystem::remove_all(dir);
	EXPECT_FALSE(created);
}

TEST(IncludeSaveTest, CopyOfAnIncludeThatCouldNotBeReadIsNotEmptied) {
	std::string dir = tempDir("dwpcopy");
	ASSERT_FALSE(dir.empty());
	std::string inc = dir + "/notaworkprint.txt";
	{ std::ofstream(inc) << "Keep me.\n"; }
	Loaded l = loadText(withHeader("( Include " + inc + " )\n"));
	ASSERT_TRUE(l.root != NULL);
	dwptest::WorkprintPtr copy(l.root->getFirstChild()->copy());
	ASSERT_TRUE(copy != NULL);
	dwptest::writeString(copy.get());
	std::string after = readFile(inc);
	std::filesystem::remove_all(dir);
	EXPECT_EQ("Keep me.\n", after);
}

TEST(IncludeSaveTest, ChildrenAddedToAnIncludeThatCouldNotBeReadAreSaved) {
	// Once there is something to save, it is saved.
	std::string dir = tempDir("dwpadd");
	ASSERT_FALSE(dir.empty());
	std::string inc = dir + "/new.dwp";
	Loaded l = loadText(withHeader("( Include " + inc + " )\n"));
	ASSERT_TRUE(l.root != NULL);
	Loaded extra = loadText(withHeader("( ActorDef added )\n"));
	MleDwpItem *child = extra.root->getFirstChild();
	extra.root->removeChild(child);
	l.root->getFirstChild()->insertChild(child, NULL);
	dwptest::writeString(l.root.get());
	std::string saved = readFile(inc);
	std::filesystem::remove_all(dir);
	EXPECT_NE(std::string::npos, saved.find("( ActorDef added")) << saved;
}

TEST(IncludeSaveTest, IncludeThatWasReadIsSavedWithItsChildren) {
	std::string dir = tempDir("dwpread");
	ASSERT_FALSE(dir.empty());
	std::string inc = dir + "/inc.dwp";
	{ std::ofstream(inc) << "#DWP 1.0 ascii\n( ActorDef a )\n( ActorDef b )\n"; }
	Loaded l = loadText(withHeader("( Include " + inc + " )\n"));
	ASSERT_TRUE(l.root != NULL);
	std::remove(inc.c_str());
	dwptest::writeString(l.root.get());
	std::string saved = readFile(inc);
	std::filesystem::remove_all(dir);
	EXPECT_NE(std::string::npos, saved.find("( ActorDef a")) << saved;
	EXPECT_NE(std::string::npos, saved.find("( ActorDef b")) << saved;
}

TEST(IncludeSaveTest, PopulateOfAMissingFileIsNotEmptied) {
	std::string dir = tempDir("dwppop");
	ASSERT_FALSE(dir.empty());
	std::string inc = dir + "/inc.dwp";
	{ std::ofstream(inc) << "#DWP 1.0 ascii\n( ActorDef a )\n"; }
	Loaded l = loadText(withHeader("( Include " + inc + " )\n"));
	ASSERT_TRUE(l.root != NULL);
	MleDwpInclude *include = (MleDwpInclude *)l.root->getFirstChild();
	{ std::ofstream(inc) << "Not a workprint any more.\n"; }
	::testing::internal::CaptureStderr();
	EXPECT_NE(0, include->populate());
	::testing::internal::GetCapturedStderr();
	dwptest::writeString(l.root.get());
	std::string after = readFile(inc);
	std::filesystem::remove_all(dir);
	EXPECT_EQ("Not a workprint any more.\n", after);
}

TEST(CopyOfTheRootTest, KeepsTheClassOfTheIncludeItCopies) {
	Loaded l = loadText(withHeader("( ActorDef a )\n"));
	ASSERT_TRUE(l.root != NULL);
	dwptest::WorkprintPtr copy(l.root->copy());
	ASSERT_TRUE(copy != NULL);
	EXPECT_EQ(std::string(l.root->getTypeName()), std::string(copy->getTypeName()));
	EXPECT_EQ(l.root->getTypeId(), copy->getTypeId());
}
