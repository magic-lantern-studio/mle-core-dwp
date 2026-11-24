/**
 * @file testChecker.cxx
 *
 * Tests of MleDwpChecker, which checks an in-memory workprint against the
 * rules of the Magic Lantern workprint model, and of the DwpChecker command
 * line tool, which reads a workprint file and runs the checker on it.
 *
 * The rules, in the order in which the checker applies them:
 *
 *   1. an Actor is inside a Group;
 *   2. an Actor has an ActorDef, directly or through a chain of ActorTemplates;
 *   3. the Set that an Actor's RoleBinding names exists, and its type is the
 *      one that the RoleSetMapping of the ActorDef asks for;
 *   4. a Stage has at least one Set;
 *   5. a Set has a SetDef;
 *   6. group names are unique.
 *
 * Each rule has a valid and an invalid workprint in workprints/checker.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <memory>
#include <string>
#include <vector>
#include "gtest/gtest.h"
#if defined(__GLIBC__)
#include <malloc.h>
#endif

// Include Magic Lantern header files.
#include <mle/mlErrno.h>

// Include Magic Lantern Digital Workprint header files.
#include <mle/Dwp.h>
#include <mle/DwpActor.h>
#include <mle/DwpActorDef.h>
#include <mle/DwpActorTemplate.h>
#include <mle/DwpChecker.h>
#include <mle/DwpGroup.h>
#include <mle/DwpItem.h>
#include <mle/DwpProperty.h>
#include <mle/DwpPropertyDef.h>
#include <mle/DwpRoleBinding.h>
#include <mle/DwpScene.h>
#include <mle/DwpSet.h>
#include <mle/DwpSetDef.h>
#include <mle/DwpStage.h>

// Include Digital Workprint test header files.
#include "libdwptest.h"

namespace checkertest {

const char *g_header = "#DWP 1.0 ascii\n";

// The result of a check.
struct Result {
	MlErr err;
	std::string message;
};

Result check(MleDwpItem *root) {
	MleDwpChecker checker(root);
	Result r;
	r.err = checker.check();
	r.message = checker.getErrorMsg() ? checker.getErrorMsg() : "";
	return r;
}

std::string fixture(const std::string &name) {
	return dwptest::workprintPath(("checker/" + name).c_str());
}

Result checkFile(const std::string &name) {
	dwptest::WorkprintPtr root = dwptest::loadFile(fixture(name));
	EXPECT_TRUE(root != NULL) << name;
	return root ? check(root.get()) : Result{ 1, "" };
}

Result checkText(const std::string &text) {
	dwptest::WorkprintPtr root = dwptest::loadString(g_header + text);
	EXPECT_TRUE(root != NULL);
	return root ? check(root.get()) : Result{ 1, "" };
}

// A workprint with one Stage, Set, SetDef and ActorDef, which the rules need.
const char *g_parts =
	"( SetDef Mle2dSet )\n"
	"( Stage MyStage Mle2dStage ( Set 2dSet Mle2dSet ) )\n"
	"( ActorDef LabelActor ( RoleSetMapping LabelRole Mle2dSet ) )\n"
	"( RoleDef LabelRole )\n";

// Check the text in a process of its own, so that a crash or a hang in the
// checker is a failure of this test only. The process exits with 0 if the
// workprint is valid and with 1 if the checker reports a problem, and prints
// the message of the checker.
void checkAndLeave(const std::string &text) {
	alarm(20);
	dwptest::WorkprintPtr root = dwptest::loadString(g_header + text);
	if (! root)
		exit(2);
	MleDwpChecker checker(root.get());
	MlErr err = checker.check();
	if (err)
		fprintf(stderr, "check: %s\n", checker.getErrorMsg() ? checker.getErrorMsg() : "");
	exit(err ? 1 : 0);
}

std::string readFile(const std::string &path) {
	std::string s;
	FILE *fp = fopen(path.c_str(), "r");
	if (! fp)
		return s;
	char buffer[4096];
	size_t n;
	while ((n = fread(buffer, 1, sizeof(buffer), fp)) > 0)
		s.append(buffer, n);
	fclose(fp);
	return s;
}

#if defined(__GLIBC__) && (__GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 33))
#define HAVE_HEAP_SIZE 1
size_t heapInUse() { return mallinfo2().uordblks; }
#endif

} // namespace checkertest

using namespace checkertest;

// ---------------------------------------------------------------------------
// Valid workprints.

class ValidWorkprintTest : public ::testing::TestWithParam<const char *> {};

TEST_P(ValidWorkprintTest, CheckSucceeds) {
	Result r = checkFile(GetParam());
	EXPECT_EQ(MlNoErr, r.err) << r.message;
	EXPECT_EQ("", r.message);
}

INSTANTIATE_TEST_SUITE_P(Checker, ValidWorkprintTest, ::testing::Values(
	"valid/complete.dwp",
	"valid/no_role_binding.dwp",
	"valid/actor_template.dwp",
	"valid/two_groups.dwp",
	"valid/same_set_name_in_two_stages.dwp",
	"valid/no_actors.dwp",
	"valid/header_only.dwp",
	"valid/two_roles.dwp",
	"valid/second_role_only.dwp",
	"valid/stage_after_stage.dwp"),
	[](const ::testing::TestParamInfo<const char *> &info) {
		std::string s = info.param;
		s = s.substr(s.find('/') + 1);
		s.erase(s.find('.'));
		std::string r;
		for (char c : s)
			if (c != '_') r += c;
		return r;
	});

// ---------------------------------------------------------------------------
// Invalid workprints: one for each rule.

namespace checkertest {
struct Invalid {
	const char *file;
	const char *message;
};
}

class InvalidWorkprintTest : public ::testing::TestWithParam<Invalid> {};

TEST_P(InvalidWorkprintTest, CheckReportsTheRule) {
	const Invalid &c = GetParam();
	Result r = checkFile(c.file);
	EXPECT_NE(MlNoErr, r.err);
	EXPECT_NE(std::string::npos, r.message.find(c.message)) << r.message;
}

static const Invalid g_invalid[] = {
	{ "invalid/actor_not_in_group.dwp",       "Actor 'label' is not inside any Group" },
	{ "invalid/actor_without_actordef.dwp",   "Actor label doesn't have a ActorDef." },
	{ "invalid/template_without_actordef.dwp", "Actor title doesn't have a ActorDef." },
	{ "invalid/binding_template_without_actordef.dwp", "Actor title doesn't have a ActorDef." },
	{ "invalid/binding_set_missing.dwp",      "Actor label doesn't have set NoSuchSet." },
	{ "invalid/binding_set_type_mismatch.dwp", "The RoleBinding for Actor label specifies set 2dSet of type Mle2dSet" },
	{ "invalid/stage_without_set.dwp",        "Stage MyStage doesn't have at least one set." },
	{ "invalid/set_without_setdef.dwp",       "Set 2dSet doesn't have a SetDef." },
	{ "invalid/duplicate_group_names.dwp",    "duplicate group entries" },
	{ "invalid/second_binding_set_missing.dwp", "Actor a doesn't have set nowhere." },
	{ "invalid/second_binding_type_mismatch.dwp", "set s2 of type Mle2dSet,\nwhich doesn't match the set type Mle3dSet" },
	{ "invalid/second_stage_without_set.dwp", "Stage B doesn't have at least one set." },
};

INSTANTIATE_TEST_SUITE_P(Checker, InvalidWorkprintTest, ::testing::ValuesIn(g_invalid),
	[](const ::testing::TestParamInfo<Invalid> &info) {
		std::string s = info.param.file;
		s = s.substr(s.find('/') + 1);
		s.erase(s.find('.'));
		std::string r;
		for (char c : s)
			if (c != '_') r += c;
		return r;
	});

TEST(CheckerRuleTest, MismatchMessageNamesBothSetTypes) {
	Result r = checkFile("invalid/binding_set_type_mismatch.dwp");
	EXPECT_NE(std::string::npos, r.message.find("set type Mle3dSet specified in the RoleSetMapping")) << r.message;
}

// The first rule that is broken is the one that is reported.
TEST(CheckerRuleTest, FirstBrokenRuleIsReported) {
	// An Actor that is outside a Group and has no ActorDef, in a workprint
	// that also has a Stage without a Set.
	Result r = checkText(
		"( Stage MyStage Mle2dStage )\n( Actor a Missing )\n");
	EXPECT_NE(std::string::npos, r.message.find("not inside any Group")) << r.message;

	r = checkText("( Stage MyStage Mle2dStage )\n( Group g MleGroup ( Actor a Missing ) )\n");
	EXPECT_NE(std::string::npos, r.message.find("doesn't have a ActorDef")) << r.message;

	r = checkText("( Stage MyStage Mle2dStage )\n( Group g MleGroup )\n");
	EXPECT_NE(std::string::npos, r.message.find("at least one set")) << r.message;
}

TEST(CheckerRuleTest, ActorsAreFoundAtAnyDepth) {
	Result r = checkText(std::string(g_parts) +
		"( Scene s MleScene ( Group g MleGroup ( Group inner MleGroup ( Actor a LabelActor ) ) ) )\n");
	EXPECT_EQ(MlNoErr, r.err) << r.message;
	r = checkText(std::string(g_parts) +
		"( Scene s MleScene ( Group g MleGroup ) ( Actor late LabelActor ) )\n");
	EXPECT_NE(MlNoErr, r.err);
	EXPECT_NE(std::string::npos, r.message.find("'late'")) << r.message;
}

TEST(CheckerRuleTest, EveryActorNeedsAnActorDef) {
	// The last of many Actors is the one that has none.
	std::string text = g_parts;
	text += "( Group g MleGroup\n";
	for (int i = 0; i < 20; i++)
		text += "( Actor a" + std::to_string(i) + " LabelActor )\n";
	text += "( Actor last Missing ) )\n";
	Result r = checkText(text);
	EXPECT_NE(std::string::npos, r.message.find("Actor last doesn't")) << r.message;
}

TEST(CheckerRuleTest, ActorDefIsFoundByName) {
	Result r = checkText(std::string(g_parts) +
		"( Group g MleGroup ( Actor a LabelActo ) )\n");
	EXPECT_NE(MlNoErr, r.err) << "a prefix of the name is not the name";
	r = checkText(std::string(g_parts) + "( Group g MleGroup ( Actor a labelactor ) )\n");
	EXPECT_NE(MlNoErr, r.err) << "names are case sensitive";
}

TEST(CheckerRuleTest, ActorDefMayComeAfterTheActor) {
	Result r = checkText(
		"( Group g MleGroup ( Actor a Late ) )\n( ActorDef Late )\n");
	EXPECT_EQ(MlNoErr, r.err) << r.message;
}

TEST(CheckerRuleTest, TemplateChainIsFollowedToTheActorDef) {
	Result r = checkText(
		"( ActorDef Base )\n( ActorTemplate T1 Base )\n( ActorTemplate T2 T1 )\n"
		"( ActorTemplate T3 T2 )\n( Group g MleGroup ( Actor a T3 ) )\n");
	EXPECT_EQ(MlNoErr, r.err) << r.message;
	r = checkText(
		"( ActorTemplate T1 Missing )\n( ActorTemplate T2 T1 )\n"
		"( Group g MleGroup ( Actor a T2 ) )\n");
	EXPECT_NE(MlNoErr, r.err);
}

TEST(CheckerRuleTest, BindingToASetOfTheRightTypeIsValid) {
	Result r = checkText(std::string(g_parts) +
		"( Group g MleGroup ( Actor a LabelActor ( RoleBinding LabelRole 2dSet ) ) )\n");
	EXPECT_EQ(MlNoErr, r.err) << r.message;
}

TEST(CheckerRuleTest, BindingIsNotCheckedWithoutARoleSetMapping) {
	Result r = checkText(
		"( SetDef Mle2dSet )\n( Stage MyStage Mle2dStage ( Set 2dSet Mle2dSet ) )\n"
		"( ActorDef Plain )\n"
		"( Group g MleGroup ( Actor a Plain ( RoleBinding AnyRole 2dSet ) ) )\n");
	EXPECT_EQ(MlNoErr, r.err) << r.message;
}

TEST(CheckerRuleTest, BindingThroughATemplateIsCheckedAgainstTheActorDef) {
	Result r = checkText(
		"( SetDef Mle2dSet )\n( SetDef Mle3dSet )\n"
		"( Stage MyStage Mle2dStage ( Set 2dSet Mle2dSet ) )\n"
		"( ActorDef LabelActor ( RoleSetMapping LabelRole Mle3dSet ) )\n"
		"( ActorTemplate Title LabelActor )\n"
		"( Group g MleGroup ( Actor a Title ( RoleBinding LabelRole 2dSet ) ) )\n");
	EXPECT_NE(MlNoErr, r.err);
	EXPECT_NE(std::string::npos, r.message.find("Mle3dSet")) << r.message;
}

TEST(CheckerRuleTest, EveryStageNeedsASet) {
	Result r = checkText(
		"( SetDef S )\n( Stage A C ( Set s S ) )\n( Stage B C )\n");
	EXPECT_NE(std::string::npos, r.message.find("Stage B")) << r.message;
}

TEST(CheckerRuleTest, SetOfANestedStageCountsForTheStage) {
	Result r = checkText("( SetDef S )\n( Stage A C ( Group g MleGroup ( Set s S ) ) )\n");
	EXPECT_EQ(MlNoErr, r.err) << r.message;
}

TEST(CheckerRuleTest, EverySetNeedsASetDef) {
	Result r = checkText(
		"( SetDef S )\n( Stage A C ( Set one S ) ( Set two T ) )\n");
	EXPECT_NE(std::string::npos, r.message.find("Set two doesn't have a SetDef")) << r.message;
}

TEST(CheckerRuleTest, WorkprintWithoutSetsHasNothingToCheck) {
	// Neither a library of ActorDefs nor a workprint with only Groups has a
	// Set, and neither breaks a rule.
	EXPECT_EQ(MlNoErr, checkText("( ActorDef a )\n").err);
	EXPECT_EQ(MlNoErr, checkText("( Group g MleGroup )\n").err);
	EXPECT_EQ(MlNoErr, checkText("").err);
}

TEST(CheckerRuleTest, DuplicateGroupMessageNamesTheGroup) {
	Result r = checkFile("invalid/duplicate_group_names.dwp");
	EXPECT_NE(std::string::npos, r.message.find("Group name 'group1' is used more than once.")) << r.message;
	// The second name that is duplicated is not the one reported first.
	r = checkText("( Group a MleGroup )\n( Group b MleGroup )\n( Group c MleGroup )\n( Group b MleGroup )\n( Group c MleGroup )\n");
	EXPECT_NE(std::string::npos, r.message.find("'b'")) << r.message;
}

TEST(CheckerRuleTest, GroupNamesAreUniqueAcrossScenes) {
	Result r = checkText(
		"( Scene one MleScene ( Group g MleGroup ) )\n( Scene two MleScene ( Group g MleGroup ) )\n");
	EXPECT_NE(MlNoErr, r.err);
	r = checkText(
		"( Scene one MleScene ( Group g MleGroup ) )\n( Scene two MleScene ( Group h MleGroup ) )\n");
	EXPECT_EQ(MlNoErr, r.err) << r.message;
}

TEST(CheckerRuleTest, GroupNamesAreUniqueAmongManyGroups) {
	std::string text;
	for (int i = 0; i < 200; i++)
		text += "( Group g" + std::to_string(i) + " MleGroup )\n";
	EXPECT_EQ(MlNoErr, checkText(text).err);
	// The last Group repeats the first, and then one in the middle.
	EXPECT_NE(MlNoErr, checkText(text + "( Group g0 MleGroup )\n").err);
	EXPECT_NE(MlNoErr, checkText(text + "( Group g100 MleGroup )\n").err);
	EXPECT_NE(MlNoErr, checkText(text + "( Group g199 MleGroup )\n").err);
	// Groups of one name and Groups that differ only in case.
	EXPECT_EQ(MlNoErr, checkText(text + "( Group G0 MleGroup )\n").err);
}

// ---------------------------------------------------------------------------
// The checker object.

TEST(CheckerTest, CheckerCanBeReusedForAnotherWorkprint) {
	dwptest::WorkprintPtr good = dwptest::loadFile(fixture("valid/complete.dwp"));
	dwptest::WorkprintPtr bad = dwptest::loadFile(fixture("invalid/stage_without_set.dwp"));
	ASSERT_TRUE(good != NULL);
	ASSERT_TRUE(bad != NULL);

	MleDwpChecker checker;
	checker.setWorkprint(good.get());
	EXPECT_EQ(MlNoErr, checker.check());
	checker.setWorkprint(bad.get());
	EXPECT_NE(MlNoErr, checker.check());
	EXPECT_NE(std::string::npos, std::string(checker.getErrorMsg()).find("Stage MyStage"));
	checker.setWorkprint(good.get());
	EXPECT_EQ(MlNoErr, checker.check());
}

TEST(CheckerTest, ErrorMessageIsReplacedByTheNextError) {
	dwptest::WorkprintPtr a = dwptest::loadFile(fixture("invalid/stage_without_set.dwp"));
	dwptest::WorkprintPtr b = dwptest::loadFile(fixture("invalid/set_without_setdef.dwp"));
	ASSERT_TRUE(a != NULL);
	ASSERT_TRUE(b != NULL);
	MleDwpChecker checker(a.get());
	EXPECT_NE(MlNoErr, checker.check());
	EXPECT_NE(std::string::npos, std::string(checker.getErrorMsg()).find("Stage"));
	checker.setWorkprint(b.get());
	EXPECT_NE(MlNoErr, checker.check());
	EXPECT_NE(std::string::npos, std::string(checker.getErrorMsg()).find("SetDef"));
}

TEST(CheckerTest, MessageOfAFailedCheckIsForgottenWhenTheNextCheckPasses) {
	dwptest::WorkprintPtr good = dwptest::loadFile(fixture("valid/complete.dwp"));
	dwptest::WorkprintPtr bad = dwptest::loadFile(fixture("invalid/stage_without_set.dwp"));
	ASSERT_TRUE(good != NULL);
	ASSERT_TRUE(bad != NULL);
	MleDwpChecker checker(bad.get());
	EXPECT_NE(MlNoErr, checker.check());
	ASSERT_TRUE(checker.getErrorMsg() != NULL);
	checker.setWorkprint(good.get());
	EXPECT_EQ(MlNoErr, checker.check());
	EXPECT_EQ(NULL, checker.getErrorMsg());
}

TEST(CheckerTest, CopyHasItsOwnMessageAndFlags) {
	dwptest::WorkprintPtr bad = dwptest::loadFile(fixture("invalid/stage_without_set.dwp"));
	ASSERT_TRUE(bad != NULL);
	std::unique_ptr<MleDwpChecker> original(new MleDwpChecker(bad.get()));
	MleDwpChecker_SetTargetToPC(original.get());
	EXPECT_NE(MlNoErr, original->check());

	MleDwpChecker copy(*original);
	EXPECT_STREQ(original->getErrorMsg(), copy.getErrorMsg());
	EXPECT_NE(original->getErrorMsg(), copy.getErrorMsg());
	EXPECT_TRUE(MleDwpChecker_TargetIsPC(&copy));
	EXPECT_NE(MlNoErr, copy.check());

	// The copy outlives the original.
	std::string message = copy.getErrorMsg();
	original.reset();
	EXPECT_EQ(message, copy.getErrorMsg());
}

TEST(CheckerTest, AssignmentCopiesTheMessage) {
	dwptest::WorkprintPtr bad = dwptest::loadFile(fixture("invalid/set_without_setdef.dwp"));
	dwptest::WorkprintPtr other = dwptest::loadFile(fixture("invalid/stage_without_set.dwp"));
	ASSERT_TRUE(bad != NULL);
	ASSERT_TRUE(other != NULL);
	MleDwpChecker a(bad.get());
	MleDwpChecker b(other.get());
	EXPECT_NE(MlNoErr, a.check());
	EXPECT_NE(MlNoErr, b.check());
	b = a;
	EXPECT_STREQ(a.getErrorMsg(), b.getErrorMsg());
	EXPECT_NE(a.getErrorMsg(), b.getErrorMsg());
	MleDwpChecker &self = b;
	b = self;
	EXPECT_STREQ(a.getErrorMsg(), b.getErrorMsg());

	// A checker with no message replaces one that has a message.
	MleDwpChecker empty;
	b = empty;
	EXPECT_EQ(NULL, b.getErrorMsg());
}

TEST(CheckerTest, CopyDoesNotLeak) {
#if defined(HAVE_HEAP_SIZE)
	dwptest::WorkprintPtr bad = dwptest::loadFile(fixture("invalid/stage_without_set.dwp"));
	ASSERT_TRUE(bad != NULL);
	MleDwpChecker checker(bad.get());
	checker.check();
	for (int i = 0; i < 5; i++) { MleDwpChecker c(checker); MleDwpChecker d; d = c; }
	size_t before = heapInUse();
	for (int i = 0; i < 200; i++) { MleDwpChecker c(checker); MleDwpChecker d; d = c; d = checker; }
	EXPECT_EQ(before, heapInUse());
#else
	GTEST_SKIP() << "No heap statistics.";
#endif
}

TEST(CheckerTest, NoErrorMessageBeforeACheck) {
	MleDwpChecker checker;
	EXPECT_EQ(NULL, checker.getErrorMsg());
}

TEST(CheckerTest, CheckWithoutAWorkprintIsAnError) {
	// There is nothing to check, which is not the same as a valid workprint.
	// The process must not crash.
	EXPECT_EXIT({
			alarm(20);
			MleDwpChecker checker;
			MlErr err = checker.check();
			exit(err != MlNoErr && checker.getErrorMsg() != NULL ? 0 : 1);
		}, ::testing::ExitedWithCode(0), "");
}

TEST(CheckerTest, CheckDoesNotChangeTheWorkprint) {
	dwptest::WorkprintPtr root = dwptest::loadFile(fixture("valid/complete.dwp"));
	ASSERT_TRUE(root != NULL);
	std::string before = dwptest::writeString(root.get());
	check(root.get());
	check(root.get());
	EXPECT_EQ(before, dwptest::writeString(root.get()));
}

TEST(CheckerTest, CheckDoesNotLeak) {
#if defined(HAVE_HEAP_SIZE)
	const char *files[] = { "valid/complete.dwp", "valid/actor_template.dwp",
		"valid/same_set_name_in_two_stages.dwp", "invalid/binding_set_missing.dwp",
		"invalid/binding_set_type_mismatch.dwp", "invalid/actor_without_actordef.dwp",
		"invalid/set_without_setdef.dwp", "invalid/duplicate_group_names.dwp" };
	for (const char *file : files) {
		dwptest::WorkprintPtr root = dwptest::loadFile(fixture(file));
		ASSERT_TRUE(root != NULL) << file;
		// Warm up, so that memory the library keeps for good is not counted.
		for (int i = 0; i < 5; i++) check(root.get());
		size_t before = heapInUse();
		for (int i = 0; i < 200; i++) check(root.get());
		EXPECT_EQ(before, heapInUse()) << file;
	}
#else
	GTEST_SKIP() << "No heap statistics.";
#endif
}

// ---------------------------------------------------------------------------
// Workprints that are incomplete because they were not read in full, or built
// by a program. The checker must report them and go on.

TEST(CheckerIncompleteTest, ActorWithoutAClass) {
	EXPECT_EXIT({
			alarm(20);
			MleDwpItem *root = new MleDwpItem;
			MleDwpGroup *group = new MleDwpGroup;
			group->setName("g");
			root->insertChild(group);
			MleDwpActor *actor = new MleDwpActor;
			actor->setName("a");
			group->insertChild(actor);
			MleDwpChecker checker(root);
			exit(checker.check() != MlNoErr ? 0 : 1);
		}, ::testing::ExitedWithCode(0), "");
}

TEST(CheckerIncompleteTest, ActorOfATemplateWithoutAClass) {
	EXPECT_EXIT({
			alarm(20);
			MleDwpItem *root = new MleDwpItem;
			MleDwpActorTemplate *templt = new MleDwpActorTemplate;
			templt->setName("T");
			root->insertChild(templt);
			MleDwpGroup *group = new MleDwpGroup;
			group->setName("g");
			root->insertChild(group);
			MleDwpActor *actor = new MleDwpActor;
			actor->setName("a");
			actor->setActorClass("T");
			group->insertChild(actor);
			MleDwpChecker checker(root);
			exit(checker.check() != MlNoErr ? 0 : 1);
		}, ::testing::ExitedWithCode(0), "");
}

TEST(CheckerIncompleteTest, SetWithoutAType) {
	EXPECT_EXIT({
			alarm(20);
			MleDwpItem *root = new MleDwpItem;
			MleDwpSetDef *def = new MleDwpSetDef;
			def->setSetClass("S");
			root->insertChild(def);
			MleDwpSet *set = new MleDwpSet;
			set->setName("s");
			root->insertChild(set);
			MleDwpChecker checker(root);
			exit(checker.check() != MlNoErr ? 0 : 1);
		}, ::testing::ExitedWithCode(0), "");
}

TEST(CheckerIncompleteTest, RoleBindingWithoutASet) {
	// A binding that names no Set is not bound to the first Set there is.
	EXPECT_EXIT({
			alarm(20);
			MleDwpItem *root = new MleDwpItem;
			MleDwpSetDef *def = new MleDwpSetDef;
			def->setSetClass("S");
			root->insertChild(def);
			MleDwpSet *set = new MleDwpSet;
			set->setName("s");
			set->setType("S");
			root->insertChild(set);
			MleDwpActorDef *actorDef = new MleDwpActorDef;
			actorDef->setActorClass("A");
			root->insertChild(actorDef);
			MleDwpGroup *group = new MleDwpGroup;
			group->setName("g");
			root->insertChild(group);
			MleDwpActor *actor = new MleDwpActor;
			actor->setName("a");
			actor->setActorClass("A");
			group->insertChild(actor);
			MleDwpRoleBinding *binding = new MleDwpRoleBinding;
			binding->setRoleType("R");
			actor->insertChild(binding);
			MleDwpChecker checker(root);
			exit(checker.check() != MlNoErr ? 0 : 1);
		}, ::testing::ExitedWithCode(0), "");
}

TEST(CheckerIncompleteTest, GroupWithoutAName) {
	EXPECT_EXIT({
			alarm(20);
			MleDwpItem *root = new MleDwpItem;
			root->insertChild(new MleDwpGroup);
			root->insertChild(new MleDwpGroup);
			MleDwpChecker checker(root);
			checker.check();
			exit(0);
		}, ::testing::ExitedWithCode(0), "");
}

TEST(CheckerIncompleteTest, WorkprintThatFailedToLoad) {
	// An Actor whose class was not read is kept in the tree.
	EXPECT_EXIT(checkAndLeave("( Group g MleGroup ( Actor label"), ::testing::ExitedWithCode(1), "check: ");
}

// ---------------------------------------------------------------------------
// Names and messages of any length.

TEST(CheckerMessageTest, LongNamesAreReported) {
	// The input buffer is 1024 bytes, so a name can be 1023 bytes long, and
	// a message can contain several of them.
	std::string n(1000, 'n');
	std::string t(1000, 't');
	struct { const char *what; std::string text; } cases[] = {
		{ "actor outside a group", "( ActorDef A )\n( Actor " + n + " A )\n" },
		{ "actor without a definition", "( Group g MleGroup ( Actor " + n + " " + t + " ) )\n" },
		{ "binding to a missing set",
			"( ActorDef A )\n( Group g MleGroup ( Actor " + n + " A ( RoleBinding R " + t + " ) ) )\n"
			"( SetDef S )\n( Stage T C ( Set s S ) )\n" },
		{ "set type mismatch",
			"( SetDef " + t + " )\n( SetDef S )\n( Stage T C ( Set " + n + " S ) )\n"
			"( ActorDef A ( RoleSetMapping R " + t + " ) )\n"
			"( Group g MleGroup ( Actor " + n + " A ( RoleBinding R " + n + " ) ) )\n" },
		{ "stage without a set", "( Stage " + n + " C )\n" },
		{ "set without a definition", "( Stage T C ( Set " + n + " " + t + " ) )\n" },
	};
	for (const auto &c : cases)
		EXPECT_EXIT(checkAndLeave(c.text), ::testing::ExitedWithCode(1), "check: ") << c.what;
}

// ---------------------------------------------------------------------------
// Workprints that refer to themselves.

TEST(CheckerCycleTest, TemplateOfItself) {
	EXPECT_EXIT(checkAndLeave("( ActorTemplate T T )\n( Group g MleGroup ( Actor a T ) )\n"),
		::testing::ExitedWithCode(1), "check: ");
}

TEST(CheckerCycleTest, TemplatesOfEachOther) {
	EXPECT_EXIT(checkAndLeave("( ActorTemplate A B )\n( ActorTemplate B A )\n( Group g MleGroup ( Actor x A ) )\n"),
		::testing::ExitedWithCode(1), "check: ");
}

TEST(CheckerCycleTest, LongCycle) {
	std::string text;
	for (int i = 0; i < 50; i++)
		text += "( ActorTemplate T" + std::to_string(i) + " T" + std::to_string((i + 1) % 50) + " )\n";
	text += "( Group g MleGroup ( Actor x T0 ) )\n";
	EXPECT_EXIT(checkAndLeave(text), ::testing::ExitedWithCode(1), "check: ");
}

TEST(CheckerCycleTest, TemplateThatHasTheNameOfItsActorDef) {
	// Template X is based on X, which is also an ActorDef. A template is
	// based on a template or an ActorDef of another name.
	EXPECT_EXIT(checkAndLeave("( ActorDef X )\n( ActorTemplate X X )\n( Group g MleGroup ( Actor a X ) )\n"),
		::testing::ExitedWithCode(1), "check: ");
}

TEST(CheckerCycleTest, LongChainWithoutACycleIsValid) {
	std::string text = "( ActorDef Base )\n";
	std::string previous = "Base";
	for (int i = 0; i < 100; i++) {
		std::string name = "T" + std::to_string(i);
		text += "( ActorTemplate " + name + " " + previous + " )\n";
		previous = name;
	}
	text += "( Group g MleGroup ( Actor x " + previous + " ) )\n";
	EXPECT_EXIT(checkAndLeave(text), ::testing::ExitedWithCode(0), "");
}

// ---------------------------------------------------------------------------
// Flags.

TEST(CheckerFlagsTest, NoFlagsAtFirst) {
	MleDwpChecker checker;
	EXPECT_EQ((unsigned int)MLE_DWP_CHECK_NOFLAGS, checker.getFlags());
	EXPECT_FALSE(MleDwpChecker_TargetIsSet(&checker));
}

TEST(CheckerFlagsTest, TargetIsSetByTheMacros) {
	MleDwpChecker checker;
	MleDwpChecker_SetTargetToSGI(&checker);
	EXPECT_TRUE(MleDwpChecker_TargetIsSet(&checker));
	EXPECT_TRUE(MleDwpChecker_TargetIsSGI(&checker));
	EXPECT_FALSE(MleDwpChecker_TargetIsPC(&checker));
	EXPECT_FALSE(MleDwpChecker_TargetIsLINUX(&checker));
}

TEST(CheckerFlagsTest, TargetIsReplacedByTheNextTarget) {
	MleDwpChecker checker;
	MleDwpChecker_SetTargetToSGI(&checker);
	MleDwpChecker_SetTargetToPC(&checker);
	EXPECT_TRUE(MleDwpChecker_TargetIsPC(&checker));
	EXPECT_FALSE(MleDwpChecker_TargetIsSGI(&checker));
	MleDwpChecker_SetTargetToLINUX(&checker);
	EXPECT_TRUE(MleDwpChecker_TargetIsLINUX(&checker));
	EXPECT_FALSE(MleDwpChecker_TargetIsPC(&checker));
	EXPECT_EQ((unsigned int)MLE_DWP_CHECK_TARGET_LINUX, checker.getFlags());
}

TEST(CheckerFlagsTest, SetOnlyChangesTheBitsOfTheMask) {
	MleDwpChecker checker;
	checker.setFlags(0x5, MLE_DWP_CHECK_TOOLSMASK);
	checker.setFlags(0xffffffff, MLE_DWP_CHECK_TARGETMASK);
	EXPECT_EQ(0x5u | MLE_DWP_CHECK_TARGETMASK, checker.getFlags());
	// A value outside the mask is not set.
	MleDwpChecker other;
	other.setFlags(0xffffffff, MLE_DWP_CHECK_TOOLSMASK);
	EXPECT_EQ((unsigned int)MLE_DWP_CHECK_TOOLSMASK, other.getFlags());
}

TEST(CheckerFlagsTest, SetWithZeroClearsTheMask) {
	MleDwpChecker checker;
	checker.setFlags(0xffff, MLE_DWP_CHECK_TOOLSMASK);
	checker.setFlags(0x1, MLE_DWP_CHECK_TOOLSMASK);
	EXPECT_EQ(0x1u, checker.getFlags());
	checker.setFlags(0, MLE_DWP_CHECK_TOOLSMASK);
	EXPECT_EQ(0u, checker.getFlags());
}

TEST(CheckerFlagsTest, ClearOnlyChangesTheBitsOfTheMask) {
	MleDwpChecker checker;
	checker.setFlags(0x3, MLE_DWP_CHECK_TOOLSMASK);
	MleDwpChecker_SetTargetToPC(&checker);
	checker.clearFlags(MLE_DWP_CHECK_TARGETMASK);
	EXPECT_FALSE(MleDwpChecker_TargetIsSet(&checker));
	EXPECT_EQ(0x3u, checker.getFlags());
	checker.clearFlags(0xffffffff);
	EXPECT_EQ(0u, checker.getFlags());
}

TEST(CheckerFlagsTest, MasteringTargetDoesNotChangeTheCheck) {
	dwptest::WorkprintPtr good = dwptest::loadFile(fixture("valid/complete.dwp"));
	dwptest::WorkprintPtr bad = dwptest::loadFile(fixture("invalid/stage_without_set.dwp"));
	ASSERT_TRUE(good != NULL);
	ASSERT_TRUE(bad != NULL);
	MleDwpChecker checker(good.get());
	MleDwpChecker_SetTargetToLINUX(&checker);
	EXPECT_EQ(MlNoErr, checker.check());
	checker.setWorkprint(bad.get());
	EXPECT_NE(MlNoErr, checker.check());
}

// ---------------------------------------------------------------------------
// Items that can only be at the top of a workprint.

TEST(CheckerTopLevelTest, ItemsThatMayBeNested) {
	std::unique_ptr<MleDwpItem> items[] = {
		std::unique_ptr<MleDwpItem>(new MleDwpGroup),
		std::unique_ptr<MleDwpItem>(new MleDwpSet),
		std::unique_ptr<MleDwpItem>(new MleDwpActor),
		std::unique_ptr<MleDwpItem>(new MleDwpProperty),
		std::unique_ptr<MleDwpItem>(new MleDwpPropertyDef) };
	for (auto &item : items)
		EXPECT_FALSE(MleDwpChecker::isTopLevelOnlyWpItem(item.get())) << item->getTypeName();
}

TEST(CheckerTopLevelTest, ItemsThatBelongAtTheTop) {
	std::unique_ptr<MleDwpItem> items[] = {
		std::unique_ptr<MleDwpItem>(new MleDwpActorDef),
		std::unique_ptr<MleDwpItem>(new MleDwpSetDef),
		std::unique_ptr<MleDwpItem>(new MleDwpScene),
		std::unique_ptr<MleDwpItem>(new MleDwpStage),
		std::unique_ptr<MleDwpItem>(new MleDwpItem) };
	for (auto &item : items)
		EXPECT_TRUE(MleDwpChecker::isTopLevelOnlyWpItem(item.get())) << item->getTypeName();
}

// ---------------------------------------------------------------------------
// The DwpChecker command line tool.

#ifdef DWPTEST_CHECKER_TOOL

namespace checkertest {

struct ToolRun {
	int status;			// The exit status, or 128 + the signal number.
	std::string output;	// The standard output and the standard error.
};

// Run the tool with the arguments, and give up on it after 20 seconds.
ToolRun runTool(const std::string &arguments) {
	ToolRun run;
	std::string command = std::string("timeout 20 ") + DWPTEST_CHECKER_TOOL + " " + arguments + " 2>&1";
	FILE *pipe = popen(command.c_str(), "r");
	if (! pipe) {
		run.status = -1;
		return run;
	}
	char buffer[4096];
	size_t n;
	while ((n = fread(buffer, 1, sizeof(buffer), pipe)) > 0)
		run.output.append(buffer, n);
	int status = pclose(pipe);
	run.status = WIFEXITED(status) ? WEXITSTATUS(status) : 128 + WTERMSIG(status);
	return run;
}

// The directory for the files that a test writes.
std::string scratchFile(const char *name, const std::string &text) {
	std::string dir = ::testing::TempDir();
	std::string path = dir + "/dwpchecker_" + std::to_string(getpid()) + "_" + name;
	FILE *fp = fopen(path.c_str(), "w");
	if (fp) {
		fwrite(text.data(), 1, text.size(), fp);
		fclose(fp);
	}
	return path;
}

} // namespace checkertest

class CheckerToolValidTest : public ::testing::TestWithParam<const char *> {};

TEST_P(CheckerToolValidTest, ExitsWithZeroAndSaysNothing) {
	ToolRun run = runTool(fixture(GetParam()));
	EXPECT_EQ(0, run.status) << run.output;
	EXPECT_EQ("", run.output);
}

INSTANTIATE_TEST_SUITE_P(Tool, CheckerToolValidTest, ::testing::Values(
	"valid/complete.dwp",
	"valid/no_role_binding.dwp",
	"valid/actor_template.dwp",
	"valid/two_groups.dwp",
	"valid/same_set_name_in_two_stages.dwp",
	"valid/no_actors.dwp",
	"valid/header_only.dwp",
	"valid/two_roles.dwp",
	"valid/second_role_only.dwp",
	"valid/stage_after_stage.dwp"),
	[](const ::testing::TestParamInfo<const char *> &info) {
		std::string s = info.param;
		s = s.substr(s.find('/') + 1);
		s.erase(s.find('.'));
		std::string r;
		for (char c : s)
			if (c != '_') r += c;
		return r;
	});

class CheckerToolInvalidTest : public ::testing::TestWithParam<Invalid> {};

TEST_P(CheckerToolInvalidTest, ExitsWithOneAndNamesTheFileAndTheProblem) {
	const Invalid &c = GetParam();
	std::string path = fixture(c.file);
	ToolRun run = runTool(path);
	EXPECT_EQ(1, run.status) << run.output;
	EXPECT_EQ(0u, run.output.find("DwpChecker: file '" + path + "': ")) << run.output;
	EXPECT_NE(std::string::npos, run.output.find(c.message)) << run.output;
}

INSTANTIATE_TEST_SUITE_P(Tool, CheckerToolInvalidTest, ::testing::ValuesIn(g_invalid),
	[](const ::testing::TestParamInfo<Invalid> &info) {
		std::string s = info.param.file;
		s = s.substr(s.find('/') + 1);
		s.erase(s.find('.'));
		std::string r;
		for (char c : s)
			if (c != '_') r += c;
		return r;
	});

TEST(CheckerToolTest, FileThatDoesNotExist) {
	ToolRun run = runTool(fixture("invalid/nothing.dwp"));
	EXPECT_EQ(1, run.status);
	EXPECT_NE(std::string::npos, run.output.find("can't open file")) << run.output;
}

TEST(CheckerToolTest, FileWithoutTheHeader) {
	std::string path = scratchFile("nomagic.dwp", "( ActorDef a )\n");
	ToolRun run = runTool(path);
	EXPECT_EQ(1, run.status) << run.output;
	EXPECT_NE(std::string::npos, run.output.find("no magic")) << run.output;
	remove(path.c_str());
}

TEST(CheckerToolTest, EmptyFile) {
	std::string path = scratchFile("empty.dwp", "");
	ToolRun run = runTool(path);
	EXPECT_EQ(1, run.status) << run.output;
	remove(path.c_str());
}

TEST(CheckerToolTest, SyntaxErrorsAreFailures) {
	// The rest of each workprint is valid, so that the check of the model
	// is not the reason for the exit status.
	struct { const char *what; std::string text; const char *message; } cases[] = {
		{ "property without a type", std::string(g_header) + g_parts + "( Property x )\n", "DWP input error" },
		{ "extra right paren", std::string(g_header) + g_parts + ")\n", "extra right paren" },
		{ "items after an extra right paren", std::string(g_header) + g_parts + ") ( Group g MleGroup )\n", "DWP input error" },
		{ "unterminated item", std::string(g_header) + g_parts + "( Group g MleGroup\n", "DWP input error" },
		{ "token that is not an item", std::string(g_header) + g_parts + "Group g\n", "DWP input error" },
	};
	for (const auto &c : cases) {
		std::string path = scratchFile("syntax.dwp", c.text);
		ToolRun run = runTool(path);
		EXPECT_EQ(1, run.status) << c.what << "\n" << run.output;
		EXPECT_NE(std::string::npos, run.output.find(c.message)) << c.what << "\n" << run.output;
		remove(path.c_str());
	}
}

TEST(CheckerToolTest, ItemsAfterAnInvalidItemAreChecked) {
	// The model error is after the syntax error and must still be found.
	std::string path = scratchFile("after.dwp",
		std::string(g_header) + "( Property x )\n( Stage MyStage Mle2dStage )\n");
	ToolRun run = runTool(path);
	EXPECT_EQ(1, run.status) << run.output;
	EXPECT_NE(std::string::npos, run.output.find("Stage MyStage doesn't have at least one set")) << run.output;
	remove(path.c_str());
}

TEST(CheckerToolTest, IncludedWorkprintsAreChecked) {
	std::string inc = scratchFile("inc.dwp", std::string(g_header) + "( Stage MyStage Mle2dStage )\n");
	std::string path = scratchFile("main.dwp", std::string(g_header) + "( Include " + inc + " )\n");
	ToolRun run = runTool(path);
	EXPECT_EQ(1, run.status) << run.output;
	EXPECT_NE(std::string::npos, run.output.find("Stage MyStage")) << run.output;
	remove(path.c_str());
	remove(inc.c_str());
}

TEST(CheckerToolTest, WorkprintWithWindowsLineEnds) {
	std::string text = readFile(fixture("valid/complete.dwp"));
	std::string crlf;
	for (char c : text) {
		if (c == '\n') crlf += '\r';
		crlf += c;
	}
	std::string path = scratchFile("crlf.dwp", crlf);
	ToolRun run = runTool(path);
	EXPECT_EQ(0, run.status) << run.output;
	remove(path.c_str());
}

TEST(CheckerToolTest, LongNamesAreReported) {
	ToolRun run = runTool(fixture("invalid/long_actor_name.dwp"));
	EXPECT_EQ(1, run.status) << run.output.substr(0, 300);
	EXPECT_NE(std::string::npos, run.output.find("is not inside any Group")) << run.output.substr(0, 300);
}

TEST(CheckerToolTest, CyclesAreReported) {
	for (const char *file : { "invalid/template_cycle.dwp", "invalid/template_cycle_two.dwp" }) {
		ToolRun run = runTool(fixture(file));
		EXPECT_EQ(1, run.status) << file << "\n" << run.output;
		EXPECT_NE(std::string::npos, run.output.find("DwpChecker: file")) << file << "\n" << run.output;
	}
}

TEST(CheckerToolTest, WorkprintThatWasNotReadInFull) {
	ToolRun run = runTool(fixture("invalid/syntax_error.dwp"));
	EXPECT_EQ(1, run.status) << run.output;
}

TEST(CheckerToolTest, VerboseDoesNotChangeTheResult) {
	ToolRun run = runTool("-v " + fixture("valid/complete.dwp"));
	EXPECT_EQ(0, run.status) << run.output;
	run = runTool("-v " + fixture("invalid/stage_without_set.dwp"));
	EXPECT_EQ(1, run.status) << run.output;
}

TEST(CheckerToolTest, TagsOptionTakesAValue) {
	ToolRun run = runTool("-t rehearsal " + fixture("valid/complete.dwp"));
	EXPECT_EQ(0, run.status) << run.output;
}

TEST(CheckerToolTest, DumpRegistries) {
	ToolRun run = runTool("-r");
	EXPECT_EQ(0, run.status) << run.output;
	EXPECT_NE(std::string::npos, run.output.find("DwpDatatype key: MlVector2")) << run.output;
	EXPECT_NE(std::string::npos, run.output.find("DwpDatatype key: string")) << run.output;
	run = runTool("-v -r");
	EXPECT_EQ(0, run.status) << run.output;
	EXPECT_NE(std::string::npos, run.output.find("Dumping")) << run.output;
}

class CheckerToolUsageTest : public ::testing::TestWithParam<const char *> {};

TEST_P(CheckerToolUsageTest, ExitsWithOneAndPrintsTheSyntax) {
	std::string arguments = GetParam();
	size_t at = arguments.find("@");
	if (at != std::string::npos)
		arguments.replace(at, 1, fixture("valid/complete.dwp"));
	ToolRun run = runTool(arguments);
	EXPECT_EQ(1, run.status) << run.output;
	EXPECT_NE(std::string::npos, run.output.find("Syntax:")) << run.output;
}

INSTANTIATE_TEST_SUITE_P(Tool, CheckerToolUsageTest, ::testing::Values(
	"",
	"-v",
	"-z @",
	"-t",
	"-r @",
	"@ @"));

#endif /* DWPTEST_CHECKER_TOOL */
