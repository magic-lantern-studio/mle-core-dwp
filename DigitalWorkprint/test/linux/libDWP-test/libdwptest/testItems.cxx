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

// Table driven tests for the item classes in DWPModel.
//
// Each row of g_items is one kind of item: its keyword, the arguments it
// takes in a workprint, what the accessors should return and how to build
// the same item in memory. Adding an item class is one row.

#include <string.h>
#include <string>
#include <vector>
#include "gtest/gtest.h"

// Include Magic Lantern Digital Workprint header files.
#include <mle/Dwp.h>
#include <mle/DwpItem.h>
#include <mle/DwpOutput.h>
#include <mle/DwpActor.h>
#include <mle/DwpActorDef.h>
#include <mle/DwpActorSet.h>
#include <mle/DwpActorTemplate.h>
#include <mle/DwpBoot.h>
#include <mle/DwpCast.h>
#include <mle/DwpCastRef.h>
#include <mle/DwpConverter.h>
#include <mle/DwpDelegateAttachment.h>
#include <mle/DwpDelegateBinding.h>
#include <mle/DwpDelegateDef.h>
#include <mle/DwpDelegateForumMapping.h>
#include <mle/DwpFiletype.h>
#include <mle/DwpForum.h>
#include <mle/DwpForumDef.h>
#include <mle/DwpGroup.h>
#include <mle/DwpGroupRef.h>
#include <mle/DwpHeaderFile.h>
#include <mle/DwpLimits.h>
#include <mle/DwpMedia.h>
#include <mle/DwpMediaRef.h>
#include <mle/DwpMediaRefClass.h>
#include <mle/DwpMediaRefSource.h>
#include <mle/DwpMediaRefTarget.h>
#include <mle/DwpPackage.h>
#include <mle/DwpProperty.h>
#include <mle/DwpPropertyDef.h>
#include <mle/DwpRoleAttachment.h>
#include <mle/DwpRoleBinding.h>
#include <mle/DwpRoleDef.h>
#include <mle/DwpRoleSetMapping.h>
#include <mle/DwpScene.h>
#include <mle/DwpSet.h>
#include <mle/DwpSetDef.h>
#include <mle/DwpSourceFile.h>
#include <mle/DwpStage.h>
#include <mle/DwpStageDef.h>
#include <mle/DwpFinder.h>
#include <mle/DwpInt.h>
#include <mle/DwpFloat.h>

// Include Digital Workprint test header files.
#include "libdwptest.h"

namespace itemtest {

struct ItemCase {
	const char *label;
	const char *keyword;       // The item's name in a workprint.
	MleDwpType typeId;
	const char *args;          // What follows the keyword.
	const char *written;       // How the arguments are written back.
	const char *name;          // The result of getName().
	void (*check)(MleDwpItem *);     // Checks the accessors.
	MleDwpItem *(*build)(void);      // Builds the same item in memory.
};

#define CHECK(T, ...) [](MleDwpItem *i) { T *x = static_cast<T *>(i); __VA_ARGS__ }
#define BUILD(T, ...) []() -> MleDwpItem * { T *x = new T; __VA_ARGS__ return x; }
#define ITEM(KEYWORD, ARGS, WRITTEN, NAME, CHECK_FN, BUILD_FN) \
	{ #KEYWORD, #KEYWORD, MleDwp##KEYWORD::typeId, ARGS, WRITTEN, NAME, CHECK_FN, BUILD_FN }

const ItemCase g_items[] = {
	ITEM(ActorDef, "TestActor", "TestActor", "TestActor",
		CHECK(MleDwpActorDef, EXPECT_STREQ("TestActor", x->getActorClass());),
		BUILD(MleDwpActorDef, x->setActorClass("TestActor");)),
	ITEM(Actor, "a TestActor", "a TestActor", "a",
		CHECK(MleDwpActor, EXPECT_STREQ("TestActor", x->getActorClass());),
		BUILD(MleDwpActor, x->setName("a"); x->setActorClass("TestActor");)),
	ITEM(ActorSet, "n", "n", "n",
		CHECK(MleDwpActorSet, ),
		BUILD(MleDwpActorSet, x->setName("n");)),
	ITEM(ActorTemplate, "n cls", "n cls", "n",
		CHECK(MleDwpActorTemplate, EXPECT_STREQ("cls", x->getActorClass());),
		BUILD(MleDwpActorTemplate, x->setName("n"); x->setActorClass("cls");)),
	ITEM(PropertyDef, "p int", "p int", "p",
		CHECK(MleDwpPropertyDef,
			EXPECT_STREQ("int", x->getType());
			ASSERT_TRUE(x->getDatatype() != NULL);
			EXPECT_STREQ("int", x->getDatatype()->getName());),
		BUILD(MleDwpPropertyDef, x->setName("p"); x->setType("int");)),
	ITEM(Property, "p int 5", "p int 5", "p",
		CHECK(MleDwpProperty,
			ASSERT_TRUE(x->getDatatype() != NULL);
			EXPECT_STREQ("int", x->getDatatype()->getName());
			EXPECT_EQ(5, x->m_data.m_u.i);),
		BUILD(MleDwpProperty,
			x->setName("p");
			x->setDatatype("int");
			int five = 5;
			x->getDatatype()->set(&x->m_data, &five);)),
	ITEM(RoleDef, "R", "R", "R",
		CHECK(MleDwpRoleDef, EXPECT_STREQ("R", x->getRoleClass());),
		BUILD(MleDwpRoleDef, x->setRoleClass("R");)),
	ITEM(RoleBinding, "R set", "R set", "R",
		CHECK(MleDwpRoleBinding,
			EXPECT_STREQ("R", x->getRoleType());
			EXPECT_STREQ("set", x->getSet());),
		BUILD(MleDwpRoleBinding, x->setRoleType("R"); x->setSet("set");)),
	ITEM(RoleAttachment, "p c", "p c", "NULL",
		CHECK(MleDwpRoleAttachment,
			EXPECT_STREQ("p", x->getParent());
			EXPECT_STREQ("c", x->getChild());),
		BUILD(MleDwpRoleAttachment, x->setParent("p"); x->setChild("c");)),
	ITEM(RoleSetMapping, "R set", "R set", "R",
		CHECK(MleDwpRoleSetMapping,
			EXPECT_STREQ("R", x->getRoleClass());
			EXPECT_STREQ("set", x->getSet());),
		BUILD(MleDwpRoleSetMapping, x->setRoleClass("R"); x->setSet("set");)),
	ITEM(Stage, "s MleIvStage", "s MleIvStage", "s",
		CHECK(MleDwpStage, EXPECT_STREQ("MleIvStage", x->getStageClass());),
		BUILD(MleDwpStage, x->setName("s"); x->setStageClass("MleIvStage");)),
	ITEM(StageDef, "S", "S", "S",
		CHECK(MleDwpStageDef, EXPECT_STREQ("S", x->getStageClass());),
		BUILD(MleDwpStageDef, x->setStageClass("S");)),
	ITEM(Set, "s Mle2dSet", "s Mle2dSet", "s",
		CHECK(MleDwpSet, EXPECT_STREQ("Mle2dSet", x->getType());),
		BUILD(MleDwpSet, x->setName("s"); x->setType("Mle2dSet");)),
	ITEM(SetDef, "S", "S", "S",
		CHECK(MleDwpSetDef, EXPECT_STREQ("S", x->getSetClass());),
		BUILD(MleDwpSetDef, x->setSetClass("S");)),
	ITEM(Scene, "sc MleScene", "sc MleScene", "sc",
		CHECK(MleDwpScene, EXPECT_STREQ("MleScene", x->getSceneClass());),
		BUILD(MleDwpScene, x->setName("sc"); x->setSceneClass("MleScene");)),
	ITEM(Group, "g MleGroup", "g MleGroup", "g",
		CHECK(MleDwpGroup, EXPECT_STREQ("MleGroup", x->getGroupClass());),
		BUILD(MleDwpGroup, x->setName("g"); x->setGroupClass("MleGroup");)),
	ITEM(GroupRef, "g", "g", "g",
		CHECK(MleDwpGroupRef, ),
		BUILD(MleDwpGroupRef, x->setName("g");)),
	ITEM(MediaRef, "m Cls", "m Cls", "m",
		CHECK(MleDwpMediaRef, EXPECT_STREQ("Cls", x->getMediaRefClass());),
		BUILD(MleDwpMediaRef, x->setName("m"); x->setMediaRefClass("Cls");)),
	ITEM(MediaRefClass, "n cls", "n cls", "n",
		CHECK(MleDwpMediaRefClass, EXPECT_STREQ("cls", x->getClassName());),
		BUILD(MleDwpMediaRefClass, x->setName("n"); x->setClassName("cls");)),
	ITEM(MediaRefSource, "3", "3", "NULL",
		CHECK(MleDwpMediaRefSource, EXPECT_EQ(3, x->getFlags());),
		BUILD(MleDwpMediaRefSource, x->setFlags(3);)),
	ITEM(MediaRefTarget, "4", "4", "NULL",
		CHECK(MleDwpMediaRefTarget, EXPECT_EQ(4, x->getFlags());),
		BUILD(MleDwpMediaRefTarget, x->setFlags(4);)),
	ITEM(Package, "p", "p", "p",
		CHECK(MleDwpPackage, EXPECT_STREQ("p", x->getPackage());),
		BUILD(MleDwpPackage, x->setPackage("p");)),
	ITEM(HeaderFile, "mle/h.h", "mle/h.h", "mle/h.h",
		CHECK(MleDwpHeaderFile, EXPECT_STREQ("mle/h.h", x->getHeader());),
		BUILD(MleDwpHeaderFile, x->setHeader("mle/h.h");)),
	ITEM(SourceFile, "s.cpp", "s.cpp", "s.cpp",
		CHECK(MleDwpSourceFile, EXPECT_STREQ("s.cpp", x->getFilename());),
		BUILD(MleDwpSourceFile, x->setFilename("s.cpp");)),
	ITEM(Media, "1 label file.txt", "1 label file.txt", "NULL",
		CHECK(MleDwpMedia,
			EXPECT_EQ(1, x->getFlags());
			EXPECT_STREQ("label", x->getLabel());
			EXPECT_STREQ("file.txt", x->getFilename());),
		BUILD(MleDwpMedia, x->setFlags(1); x->setLabel("label"); x->setFilename("file.txt");)),
	ITEM(Converter, "\"x y\"", "\"x y\"", "NULL",
		CHECK(MleDwpConverter, EXPECT_STREQ("x y", x->getConversionString());),
		BUILD(MleDwpConverter, x->setConversionString("x y");)),
	ITEM(Filetype, "\"jpg\"", "jpg", "NULL",
		CHECK(MleDwpFiletype, EXPECT_STREQ("jpg", x->getFiletype());),
		BUILD(MleDwpFiletype, x->setFiletype("jpg");)),
	ITEM(Limits, "int 0 10", "int 0 10", "NULL",
		CHECK(MleDwpLimits,
			ASSERT_TRUE(x->getDatatype() != NULL);
			EXPECT_STREQ("int", x->getDatatype()->getName());
			EXPECT_EQ(0, x->m_minData.m_u.i);
			EXPECT_EQ(10, x->m_maxData.m_u.i);),
		BUILD(MleDwpLimits,
			x->setDatatype("int");
			int low = 0, high = 10;
			x->getDatatype()->set(&x->m_minData, &low);
			x->getDatatype()->set(&x->m_maxData, &high);)),
	ITEM(Boot, "b", "b", "b",
		CHECK(MleDwpBoot, EXPECT_STREQ("b", x->getGroup());),
		BUILD(MleDwpBoot, x->setGroup("b");)),
	ITEM(Cast, "c MleCast", "c MleCast", "c",
		CHECK(MleDwpCast, EXPECT_STREQ("MleCast", x->getCastClass());),
		BUILD(MleDwpCast, x->setName("c"); x->setCastClass("MleCast");)),
	ITEM(CastRef, "c", "c", "c",
		CHECK(MleDwpCastRef, ),
		BUILD(MleDwpCastRef, x->setName("c");)),

	// Forum and Delegate items are deprecated, but older workprints use them.
	ITEM(Forum, "f t", "f t", "f",
		CHECK(MleDwpForum, EXPECT_STREQ("t", x->getType());),
		BUILD(MleDwpForum, x->setName("f"); x->setType("t");)),
	ITEM(ForumDef, "F", "F", "F",
		CHECK(MleDwpForumDef, EXPECT_STREQ("F", x->getForumClass());),
		BUILD(MleDwpForumDef, x->setForumClass("F");)),
	ITEM(DelegateDef, "D", "D", "D",
		CHECK(MleDwpDelegateDef, EXPECT_STREQ("D", x->getDelegateClass());),
		BUILD(MleDwpDelegateDef, x->setDelegateClass("D");)),
	ITEM(DelegateBinding, "D f", "D f", "D",
		CHECK(MleDwpDelegateBinding,
			EXPECT_STREQ("D", x->getDelegateType());
			EXPECT_STREQ("f", x->getForum());),
		BUILD(MleDwpDelegateBinding, x->setDelegateType("D"); x->setForum("f");)),
	ITEM(DelegateAttachment, "p c", "p c", "NULL",
		CHECK(MleDwpDelegateAttachment,
			EXPECT_STREQ("p", x->getParent());
			EXPECT_STREQ("c", x->getChild());),
		BUILD(MleDwpDelegateAttachment, x->setParent("p"); x->setChild("c");)),
	ITEM(DelegateForumMapping, "D f", "D f", "D",
		CHECK(MleDwpDelegateForumMapping,
			EXPECT_STREQ("D", x->getDelegateClass());
			EXPECT_STREQ("f", x->getForum());),
		BUILD(MleDwpDelegateForumMapping, x->setDelegateClass("D"); x->setForum("f");)),
};

// The text of one item in a workprint.
std::string itemText(const ItemCase &c, const char *tags = "") {
	return std::string("#DWP 1.0 ascii\n( ") + c.keyword + " " + tags + c.args + " )\n";
}

// What an item is written as, without the "#DWP" header or the children.
std::string written(MleDwpItem *item, int children = 0) {
	MleDwpOutput out;
	item->write(&out, children);
	std::string text = dwptest::takeBuffer(out);
	text.erase(0, text.find('\n') + 1);
	while (! text.empty() && isspace((unsigned char)text.back()))
		text.pop_back();
	return text;
}

std::string expectedText(const ItemCase &c, const char *tags = "") {
	return std::string("( ") + c.keyword + " " + tags + c.written + " )";
}

std::string caseName(const ::testing::TestParamInfo<ItemCase> &info) {
	return info.param.label;
}

} // namespace itemtest

using namespace itemtest;

class ItemTest : public ::testing::TestWithParam<ItemCase> {};

TEST_P(ItemTest, KeywordCreatesTheItem) {
	const ItemCase &c = GetParam();
	dwptest::WorkprintPtr root = dwptest::loadString(itemText(c));
	ASSERT_TRUE(root != NULL);
	MleDwpItem *item = root->getFirstChild();
	ASSERT_TRUE(item != NULL);
	EXPECT_TRUE(item->getNext() == NULL) << "only one item was read";
	EXPECT_EQ(root.get(), item->getParent());
	EXPECT_EQ(std::string("MleDwp") + c.keyword, item->getTypeName());
	EXPECT_EQ(c.typeId, item->getTypeId());
	EXPECT_TRUE(item->isa(c.typeId));
	EXPECT_TRUE(item->isa(item->getTypeName()));
}

TEST_P(ItemTest, ReadsTheArguments) {
	const ItemCase &c = GetParam();
	dwptest::WorkprintPtr root = dwptest::loadString(itemText(c));
	ASSERT_TRUE(root != NULL && root->getFirstChild() != NULL);
	MleDwpItem *item = root->getFirstChild();
	EXPECT_STREQ(c.name, item->getName());
	c.check(item);
}

TEST_P(ItemTest, WritesTheArguments) {
	const ItemCase &c = GetParam();
	dwptest::WorkprintPtr root = dwptest::loadString(itemText(c));
	ASSERT_TRUE(root != NULL && root->getFirstChild() != NULL);
	EXPECT_EQ(expectedText(c), written(root->getFirstChild()));
}

TEST_P(ItemTest, BuiltInMemoryIsTheSameItem) {
	const ItemCase &c = GetParam();
	ASSERT_TRUE(c.build != NULL);
	std::unique_ptr<MleDwpItem> item(c.build());
	EXPECT_EQ(std::string("MleDwp") + c.keyword, item->getTypeName());
	EXPECT_TRUE(item->getParent() == NULL);
	EXPECT_TRUE(item->getFirstChild() == NULL);
	EXPECT_STREQ(c.name, item->getName());
	c.check(item.get());
	EXPECT_EQ(expectedText(c), written(item.get()));
}

TEST_P(ItemTest, WriteThenReadGivesTheSameWorkprint) {
	const ItemCase &c = GetParam();
	dwptest::WorkprintPtr first = dwptest::loadString(itemText(c));
	ASSERT_TRUE(first != NULL);
	std::string text = dwptest::writeString(first.get());

	dwptest::WorkprintPtr second = dwptest::loadString(text);
	ASSERT_TRUE(second != NULL);
	EXPECT_EQ(text, dwptest::writeString(second.get()));
	ASSERT_TRUE(second->getFirstChild() != NULL);
	c.check(second->getFirstChild());
}

TEST_P(ItemTest, CopyIsAnIndependentItem) {
	const ItemCase &c = GetParam();
	dwptest::WorkprintPtr root = dwptest::loadString(itemText(c));
	ASSERT_TRUE(root != NULL && root->getFirstChild() != NULL);
	MleDwpItem *item = root->getFirstChild();

	std::unique_ptr<MleDwpItem> copy(item->copy(1));
	ASSERT_TRUE(copy != NULL);
	EXPECT_NE(item, copy.get());
	EXPECT_TRUE(copy->getParent() == NULL);
	EXPECT_EQ(item->getTypeId(), copy->getTypeId());
	EXPECT_STREQ(item->getName(), copy->getName());
	EXPECT_EQ(written(item), written(copy.get()));
	c.check(copy.get());
}

TEST_P(ItemTest, TagsAreKept) {
	const ItemCase &c = GetParam();
	dwptest::WorkprintPtr root = dwptest::loadString(itemText(c, "+one +two "));
	ASSERT_TRUE(root != NULL && root->getFirstChild() != NULL);
	MleDwpItem *item = root->getFirstChild();
	EXPECT_TRUE(item->hasTag("one"));
	EXPECT_TRUE(item->hasTag("two"));
	EXPECT_FALSE(item->hasTag("three"));
	EXPECT_TRUE(item->hasAnyTag());
	EXPECT_STREQ("one", item->getNthTag(0));
	EXPECT_STREQ("two", item->getNthTag(1));
	EXPECT_EQ(expectedText(c, "+one +two "), written(item));
	c.check(item);
}

TEST_P(ItemTest, IsNotAnyOtherItem) {
	const ItemCase &c = GetParam();
	dwptest::WorkprintPtr root = dwptest::loadString(itemText(c));
	ASSERT_TRUE(root != NULL && root->getFirstChild() != NULL);
	MleDwpItem *item = root->getFirstChild();
	for (const ItemCase &other : g_items) {
		if (&other == &c || std::string(other.keyword) == c.keyword)
			continue;
		EXPECT_FALSE(item->isa(other.typeId)) << c.keyword << " as " << other.keyword;
		EXPECT_FALSE(item->isa((std::string("MleDwp") + other.keyword).c_str()))
			<< c.keyword << " as " << other.keyword;
	}
}

INSTANTIATE_TEST_SUITE_P(Items, ItemTest, ::testing::ValuesIn(g_items), caseName);

// ---------------------------------------------------------------------------
// Parents and children.

namespace itemtest {

// A workprint with the usual nesting: actor definitions hold headers and
// property definitions, scenes hold groups, and groups hold actors.
const char *g_tree = R"DWP(#DWP 1.0 ascii
( ActorDef TestActor
  ( HeaderFile actors/TestActor.h )
  ( PropertyDef intTest int )
  ( PropertyDef floatTest float )
)
( SetDef Mle2dSet
  ( HeaderFile mle/iv2dset.h )
)
( Stage stage MleIvStage )
( Scene scene MleScene
  ( HeaderFile mle/MleScene.h )
  ( Group group MleGroup
    ( HeaderFile mle/MleGroup.h )
    ( Actor first TestActor
      ( Property intTest int 7 )
      ( Property floatTest float 1.5 )
    )
    ( Actor second TestActor )
    ( GroupRef other )
  )
)
)DWP";

std::vector<MleDwpItem *> children(MleDwpItem *item) {
	std::vector<MleDwpItem *> list;
	for (MleDwpItem *c = item->getFirstChild(); c; c = c->getNext())
		list.push_back(c);
	return list;
}

// The type names, then the names, of a list of items.
std::string describe(const std::vector<MleDwpItem *> &items) {
	std::string text;
	for (MleDwpItem *item : items)
		text += std::string(item->getTypeName() + 6) + "(" + item->getName() + ") ";
	return text;
}

MleDwpItem *named(MleDwpItem *root, MleDwpType type, const char *name) {
	MleDwpFinder finder;
	finder.setType(type);
	finder.setName(name);
	return finder.find(root);
}

} // namespace itemtest

TEST(ItemTreeTest, TopLevelItemsKeepTheirOrder) {
	dwptest::WorkprintPtr root = dwptest::loadString(g_tree);
	ASSERT_TRUE(root != NULL);
	EXPECT_EQ("ActorDef(TestActor) SetDef(Mle2dSet) Stage(stage) Scene(scene) ",
		describe(children(root.get())));
	EXPECT_TRUE(root->getParent() == NULL);
}

TEST(ItemTreeTest, ActorDefHoldsHeadersAndProperties) {
	dwptest::WorkprintPtr root = dwptest::loadString(g_tree);
	ASSERT_TRUE(root != NULL);
	MleDwpItem *def = named(root.get(), MleDwpActorDef::typeId, "TestActor");
	ASSERT_TRUE(def != NULL);
	EXPECT_EQ("HeaderFile(actors/TestActor.h) PropertyDef(intTest) PropertyDef(floatTest) ",
		describe(children(def)));
}

TEST(ItemTreeTest, SceneGroupActorNesting) {
	dwptest::WorkprintPtr root = dwptest::loadString(g_tree);
	ASSERT_TRUE(root != NULL);
	MleDwpItem *scene = named(root.get(), MleDwpScene::typeId, "scene");
	MleDwpItem *group = named(root.get(), MleDwpGroup::typeId, "group");
	MleDwpItem *first = named(root.get(), MleDwpActor::typeId, "first");
	ASSERT_TRUE(scene && group && first);

	EXPECT_EQ("HeaderFile(mle/MleScene.h) Group(group) ", describe(children(scene)));
	EXPECT_EQ("HeaderFile(mle/MleGroup.h) Actor(first) Actor(second) GroupRef(other) ",
		describe(children(group)));
	EXPECT_EQ("Property(intTest) Property(floatTest) ", describe(children(first)));

	EXPECT_EQ(group, first->getParent());
	EXPECT_EQ(scene, group->getParent());
	EXPECT_EQ(root.get(), scene->getParent());
}

TEST(ItemTreeTest, SiblingLinks) {
	dwptest::WorkprintPtr root = dwptest::loadString(g_tree);
	ASSERT_TRUE(root != NULL);
	MleDwpItem *group = named(root.get(), MleDwpGroup::typeId, "group");
	ASSERT_TRUE(group != NULL);

	std::vector<MleDwpItem *> kids = children(group);
	ASSERT_EQ(4u, kids.size());
	EXPECT_EQ(kids.front(), group->getFirstChild());
	EXPECT_EQ(kids.back(), group->getLastChild());
	EXPECT_TRUE(kids.front()->getPrev() == NULL);
	EXPECT_TRUE(kids.back()->getNext() == NULL);
	for (size_t i = 1; i < kids.size(); i++) {
		EXPECT_EQ(kids[i], kids[i - 1]->getNext());
		EXPECT_EQ(kids[i - 1], kids[i]->getPrev());
		EXPECT_EQ(group, kids[i]->getParent());
	}
}

TEST(ItemTreeTest, AncestorsAreFoundByType) {
	dwptest::WorkprintPtr root = dwptest::loadString(g_tree);
	ASSERT_TRUE(root != NULL);
	MleDwpItem *scene = named(root.get(), MleDwpScene::typeId, "scene");
	MleDwpItem *group = named(root.get(), MleDwpGroup::typeId, "group");
	MleDwpItem *property = named(root.get(), MleDwpProperty::typeId, "floatTest");
	// There are two floatTest items: the definition and the actor's property.
	property = named(group, MleDwpProperty::typeId, "floatTest");
	ASSERT_TRUE(scene && group && property);

	EXPECT_EQ(group, property->getAncestor(MleDwpGroup::typeId));
	EXPECT_EQ(scene, property->getAncestor(MleDwpScene::typeId));
	EXPECT_TRUE(property->getAncestor(MleDwpStage::typeId) == NULL);
}

TEST(ItemTreeTest, BuiltInMemoryWritesTheSameWorkprint) {
	dwptest::WorkprintPtr loaded = dwptest::loadString(g_tree);
	ASSERT_TRUE(loaded != NULL);

	// Build the same tree with the setters and insertChild().
	std::unique_ptr<MleDwpItem> root(MleDwpItem::readAll(
		dwptest::workprintPath("Empty.dwp").c_str()));
	ASSERT_TRUE(root != NULL);

	MleDwpActorDef *def = new MleDwpActorDef;
	def->setActorClass("TestActor");
	root->insertChild(def);
	MleDwpHeaderFile *header = new MleDwpHeaderFile;
	header->setHeader("actors/TestActor.h");
	def->insertChild(header);
	const char *types[][2] = { { "intTest", "int" }, { "floatTest", "float" } };
	for (auto &t : types) {
		MleDwpPropertyDef *p = new MleDwpPropertyDef;
		p->setName(t[0]);
		p->setType(t[1]);
		def->insertChild(p);
	}

	MleDwpSetDef *setDef = new MleDwpSetDef;
	setDef->setSetClass("Mle2dSet");
	root->insertChild(setDef);
	MleDwpHeaderFile *setHeader = new MleDwpHeaderFile;
	setHeader->setHeader("mle/iv2dset.h");
	setDef->insertChild(setHeader);

	MleDwpStage *stage = new MleDwpStage;
	stage->setName("stage");
	stage->setStageClass("MleIvStage");
	root->insertChild(stage);

	MleDwpScene *scene = new MleDwpScene;
	scene->setName("scene");
	scene->setSceneClass("MleScene");
	root->insertChild(scene);
	MleDwpHeaderFile *sceneHeader = new MleDwpHeaderFile;
	sceneHeader->setHeader("mle/MleScene.h");
	scene->insertChild(sceneHeader);

	MleDwpGroup *group = new MleDwpGroup;
	group->setName("group");
	group->setGroupClass("MleGroup");
	scene->insertChild(group);
	MleDwpHeaderFile *groupHeader = new MleDwpHeaderFile;
	groupHeader->setHeader("mle/MleGroup.h");
	group->insertChild(groupHeader);

	MleDwpActor *first = new MleDwpActor;
	first->setName("first");
	first->setActorClass("TestActor");
	group->insertChild(first);
	MleDwpProperty *intTest = new MleDwpProperty;
	intTest->setName("intTest");
	intTest->setDatatype("int");
	int seven = 7;
	intTest->getDatatype()->set(&intTest->m_data, &seven);
	first->insertChild(intTest);
	MleDwpProperty *floatTest = new MleDwpProperty;
	floatTest->setName("floatTest");
	floatTest->setDatatype("float");
	float value = 1.5f;
	floatTest->getDatatype()->set(&floatTest->m_data, &value);
	first->insertChild(floatTest);

	MleDwpActor *second = new MleDwpActor;
	second->setName("second");
	second->setActorClass("TestActor");
	group->insertChild(second);
	MleDwpGroupRef *ref = new MleDwpGroupRef;
	ref->setName("other");
	group->insertChild(ref);

	EXPECT_EQ(dwptest::writeString(loaded.get()), dwptest::writeString(root.get()));
}

TEST(ItemTreeTest, InsertBeforeAndRemove) {
	dwptest::WorkprintPtr root = dwptest::loadString(g_tree);
	ASSERT_TRUE(root != NULL);
	MleDwpItem *group = named(root.get(), MleDwpGroup::typeId, "group");
	MleDwpItem *second = named(root.get(), MleDwpActor::typeId, "second");
	ASSERT_TRUE(group && second);

	MleDwpActor *middle = new MleDwpActor;
	middle->setName("middle");
	middle->setActorClass("TestActor");
	group->insertChild(middle, second);
	EXPECT_EQ("HeaderFile(mle/MleGroup.h) Actor(first) Actor(middle) Actor(second) GroupRef(other) ",
		describe(children(group)));
	EXPECT_EQ(group, middle->getParent());

	group->removeChild(middle);
	EXPECT_EQ("HeaderFile(mle/MleGroup.h) Actor(first) Actor(second) GroupRef(other) ",
		describe(children(group)));
	delete middle;
}

TEST(ItemTreeTest, CopyIncludesTheChildren) {
	dwptest::WorkprintPtr root = dwptest::loadString(g_tree);
	ASSERT_TRUE(root != NULL);
	MleDwpItem *scene = named(root.get(), MleDwpScene::typeId, "scene");
	ASSERT_TRUE(scene != NULL);

	std::unique_ptr<MleDwpItem> full(scene->copy(1));
	ASSERT_TRUE(full != NULL);
	EXPECT_TRUE(full->getParent() == NULL);
	EXPECT_EQ(written(scene, 1), written(full.get(), 1));

	std::unique_ptr<MleDwpItem> shallow(scene->copy(0));
	ASSERT_TRUE(shallow != NULL);
	EXPECT_TRUE(shallow->getFirstChild() == NULL);
	EXPECT_EQ("( Scene scene MleScene )", written(shallow.get(), 1));
}

TEST(ItemTreeTest, WriteWithoutChildren) {
	dwptest::WorkprintPtr root = dwptest::loadString(g_tree);
	ASSERT_TRUE(root != NULL);
	MleDwpItem *def = named(root.get(), MleDwpActorDef::typeId, "TestActor");
	ASSERT_TRUE(def != NULL);
	EXPECT_EQ("( ActorDef TestActor )", written(def, 0));
	EXPECT_NE(written(def, 0), written(def, 1));
}

TEST(ItemTreeTest, DeletingTheRootFreesTheTree) {
	// Run the load and delete many times. If subtrees were not freed the heap
	// would grow without limit; this mostly shows nothing crashes.
	for (int i = 0; i < 200; i++) {
		dwptest::WorkprintPtr root = dwptest::loadString(g_tree);
		ASSERT_TRUE(root != NULL);
	}
}

// ---------------------------------------------------------------------------
// Items that are not in the model.

TEST(ItemCompatibilityTest, OlderWorkprintsStillLoad) {
	// A workprint of the kind written before roles, sets and stages: forums,
	// delegates, casts and the boot item.
	const char *text = R"DWP(#DWP 1.0 ascii
( ForumDef MleForum )
( DelegateDef MleDelegate )
( DelegateForumMapping MleDelegate mainForum )
( Cast cast MleCast
  ( DelegateBinding MleDelegate mainForum )
  ( DelegateAttachment parent child )
  ( Actor a TestActor )
)
( CastRef cast )
( Forum mainForum MleForum )
( Boot cast )
)DWP";
	dwptest::WorkprintPtr root = dwptest::loadString(text);
	ASSERT_TRUE(root != NULL);
	EXPECT_EQ("ForumDef(MleForum) DelegateDef(MleDelegate) DelegateForumMapping(MleDelegate) "
		"Cast(cast) CastRef(cast) Forum(mainForum) Boot(cast) ",
		describe(children(root.get())));

	MleDwpItem *cast = named(root.get(), MleDwpCast::typeId, "cast");
	ASSERT_TRUE(cast != NULL);
	EXPECT_EQ("DelegateBinding(MleDelegate) DelegateAttachment(NULL) Actor(a) ",
		describe(children(cast)));

	// None of these are the placeholder for unknown items.
	for (MleDwpItem *item : children(root.get()))
		EXPECT_NE(std::string("MleDwpItem"), item->getTypeName());

	// And they are written back as they were read.
	std::string text1 = dwptest::writeString(root.get());
	dwptest::WorkprintPtr again = dwptest::loadString(text1);
	ASSERT_TRUE(again != NULL);
	EXPECT_EQ(text1, dwptest::writeString(again.get()));
}

TEST(ItemCompatibilityTest, UnknownItemsAreKept) {
	// An item the library does not know is kept so that it is written back.
	const char *text = "#DWP 1.0 ascii\n( Mystery one two ( Inner x ) )\n( ActorDef After )\n";
	dwptest::WorkprintPtr root = dwptest::loadString(text);
	ASSERT_TRUE(root != NULL);
	std::vector<MleDwpItem *> items = children(root.get());
	ASSERT_EQ(2u, items.size());
	EXPECT_EQ(std::string("MleDwpActorDef"), items[1]->getTypeName());

	std::string output = dwptest::writeString(root.get());
	EXPECT_NE(std::string::npos, output.find("( Mystery one two"));
	EXPECT_NE(std::string::npos, output.find("( Inner x )"));
	EXPECT_NE(std::string::npos, output.find("( ActorDef After )"));
}

TEST(ItemCompatibilityTest, ErrorsInOneItemDoNotStopTheNext) {
	// Each of these has an error in its arguments, followed by a good item.
	const char *bad[] = {
		"( PropertyDef p )",
		"( Stage s )",
		"( Actor a )",
		"( Property p nosuchtype 1 )",
		"( Property p int abc )",
		"( Property p int )",
		"( Limits nosuchtype 1 2 )",
		"( MediaRefSource x )",
	};
	for (const char *item : bad) {
		std::string text = std::string("#DWP 1.0 ascii\n") + item + "\n( ActorDef After )\n";
		::testing::internal::CaptureStderr();
		dwptest::WorkprintPtr root = dwptest::loadString(text);
		::testing::internal::GetCapturedStderr();
		ASSERT_TRUE(root != NULL) << item;
		std::vector<MleDwpItem *> items = children(root.get());
		ASSERT_EQ(2u, items.size()) << item;
		EXPECT_EQ(std::string("MleDwpActorDef"), items[1]->getTypeName()) << item;
		EXPECT_STREQ("After", items[1]->getName()) << item;
	}
}

// ---------------------------------------------------------------------------
// An item that has never been given its arguments, such as one read from a
// workprint with an error, must still be written without crashing.

class DefaultItemTest : public ::testing::TestWithParam<ItemCase> {};

TEST_P(DefaultItemTest, CanBeWritten) {
	const ItemCase &c = GetParam();

	// typeId() creates an item with nothing set. It runs in another process
	// so that an assertion or crash is a failure of this test only.
	EXPECT_EXIT({
			MleDwpItem *item = c.typeId();
			std::string text = written(item);
			delete item;
			exit(text.empty() ? 1 : 0);
		}, ::testing::ExitedWithCode(0), "");
}

TEST_P(DefaultItemTest, IsWrittenAsTheKeyword) {
	const ItemCase &c = GetParam();
	std::unique_ptr<MleDwpItem> item(c.typeId());
	std::string text = written(item.get());
	EXPECT_EQ(0u, text.find(std::string("( ") + c.keyword)) << text;
	EXPECT_EQ(")", text.substr(text.size() - 1)) << text;
}

TEST_P(DefaultItemTest, CanBeReadBack) {
	// An item with nothing set can be saved and loaded again as the same kind
	// of item. Errors in its arguments are reported but are not fatal.
	const ItemCase &c = GetParam();
	std::unique_ptr<MleDwpItem> item(c.typeId());
	std::string text = std::string("#DWP 1.0 ascii\n") + written(item.get()) + "\n";

	::testing::internal::CaptureStderr();
	dwptest::WorkprintPtr root = dwptest::loadString(text);
	::testing::internal::GetCapturedStderr();
	ASSERT_TRUE(root != NULL);
	ASSERT_TRUE(root->getFirstChild() != NULL) << text;
	EXPECT_EQ(c.typeId, root->getFirstChild()->getTypeId()) << text;
}

INSTANTIATE_TEST_SUITE_P(Items, DefaultItemTest, ::testing::ValuesIn(g_items), caseName);

// A string that was never set is written as "NULL", as an unset name is.
TEST(OutputNullTest, WriteString) {
	MleDwpOutput out;
	EXPECT_EQ(0, out.writeString(NULL));
	std::string text = dwptest::takeBuffer(out);
	EXPECT_EQ("NULL", text.substr(text.find('\n') + 1, 4));
}

TEST(OutputNullTest, WriteFancyString) {
	MleDwpOutput out;
	EXPECT_EQ(0, out.writeFancyString(NULL));
	std::string text = dwptest::takeBuffer(out);
	EXPECT_EQ("NULL", text.substr(text.find('\n') + 1, 4));
}

namespace itemtest {
struct SavedCase {
	const char *label;
	const char *text;       // An item with arguments missing.
	const char *saved;      // How it is written.
};

const SavedCase g_saved[] = {
	{ "MediaRef",       "( MediaRef m )",        "( MediaRef m NULL )" },
	{ "Actor",          "( Actor a )",           "( Actor a NULL )" },
	{ "Stage",          "( Stage s )",           "( Stage s NULL )" },
	{ "PropertyDef",    "( PropertyDef p )",     "( PropertyDef p NULL )" },
	{ "Set",            "( Set s )",             "( Set s NULL )" },
	{ "RoleBinding",    "( RoleBinding R )",     "( RoleBinding R NULL )" },
	{ "MediaRefClass",  "( MediaRefClass n )",   "( MediaRefClass n NULL )" },
	{ "Forum",          "( Forum f )",           "( Forum f NULL )" },
	{ "DelegateBinding","( DelegateBinding D )", "( DelegateBinding D NULL )" },
};

std::string savedName(const ::testing::TestParamInfo<SavedCase> &info) {
	return info.param.label;
}
} // namespace itemtest

class SavedItemTest : public ::testing::TestWithParam<SavedCase> {};

TEST_P(SavedItemTest, WorkprintWithAnErrorCanBeSaved) {
	// Loading a workprint with an error and saving it must not crash.
	std::string text = std::string("#DWP 1.0 ascii\n") + GetParam().text + "\n";
	::testing::internal::CaptureStderr();
	dwptest::WorkprintPtr root = dwptest::loadString(text);
	::testing::internal::GetCapturedStderr();
	ASSERT_TRUE(root != NULL && root->getFirstChild() != NULL);
	EXPECT_EQ(GetParam().saved, written(root->getFirstChild()));
}

INSTANTIATE_TEST_SUITE_P(Saved, SavedItemTest, ::testing::ValuesIn(g_saved), savedName);

// ---------------------------------------------------------------------------
// Property values that cannot be read.

TEST(PropertyReadTest, GoodValueReportsNoError) {
	::testing::internal::CaptureStderr();
	dwptest::WorkprintPtr root = dwptest::loadString("#DWP 1.0 ascii\n( Property p int 5 )\n");
	std::string errors = ::testing::internal::GetCapturedStderr();
	ASSERT_TRUE(root != NULL);
	EXPECT_EQ("", errors);
}

TEST(PropertyReadTest, BadValueIsReported) {
	::testing::internal::CaptureStderr();
	dwptest::WorkprintPtr root = dwptest::loadString(
		"#DWP 1.0 ascii\n( Property p int abc )\n( ActorDef After )\n");
	std::string errors = ::testing::internal::GetCapturedStderr();
	ASSERT_TRUE(root != NULL);
	EXPECT_NE(std::string::npos, errors.find("error reading Property value")) << errors;
	EXPECT_NE(std::string::npos, errors.find("line 2")) << errors;

	// The item after it is read.
	std::vector<MleDwpItem *> items = children(root.get());
	ASSERT_EQ(2u, items.size());
	EXPECT_STREQ("After", items[1]->getName());
}

TEST(PropertyReadTest, MissingValueIsReported) {
	::testing::internal::CaptureStderr();
	dwptest::WorkprintPtr root = dwptest::loadString(
		"#DWP 1.0 ascii\n( Property p int )\n( ActorDef After )\n");
	std::string errors = ::testing::internal::GetCapturedStderr();
	ASSERT_TRUE(root != NULL);
	EXPECT_NE(std::string::npos, errors.find("error reading Property value")) << errors;
	EXPECT_EQ(2u, children(root.get()).size());
}

TEST(PropertyReadTest, ExtraValuesAreSkipped) {
	// After an error the rest of the item is skipped, so the leftovers are
	// not mistaken for the start of the next item.
	::testing::internal::CaptureStderr();
	dwptest::WorkprintPtr root = dwptest::loadString(
		"#DWP 1.0 ascii\n( Property p MlVector3 1 2 x 4 5 )\n( ActorDef After )\n");
	::testing::internal::GetCapturedStderr();
	ASSERT_TRUE(root != NULL);
	std::vector<MleDwpItem *> items = children(root.get());
	ASSERT_EQ(2u, items.size());
	EXPECT_EQ(std::string("MleDwpActorDef"), items[1]->getTypeName());
}
