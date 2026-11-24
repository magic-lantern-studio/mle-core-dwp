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

// Tests of the base classes in DWPBase: the dictionaries, the item tree and
// container, the discriminators, the finder, filenames and includes, and the
// monitor and notifier.
//
// Known bugs are tests that are skipped, with the reason, unless the
// environment variable DWPTEST_RUN_KNOWN_BUGS is set. Setting it runs them, so
// that a fix can be checked.

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "gtest/gtest.h"
#if defined(__GLIBC__)
#include <malloc.h>
#endif

// Include Magic Lantern Digital Workprint header files.
#include <mle/Dwp.h>
#include <mle/DwpActor.h>
#include <mle/DwpActorDef.h>
#include <mle/DwpContainer.h>
#include <mle/DwpDict.h>
#include <mle/DwpDiscriminator.h>
#include <mle/DwpFilename.h>
#include <mle/DwpFinder.h>
#include <mle/DwpGroup.h>
#include <mle/DwpInclude.h>
#include <mle/DwpInput.h>
#include <mle/DwpItem.h>
#include <mle/DwpMonitor.h>
#include <mle/DwpNotifier.h>
#include <mle/DwpOutput.h>
#include <mle/DwpScene.h>
#include <mle/DwpStrKeyDict.h>
#include <mle/DwpTagAllDiscriminator.h>
#include <mle/DwpTagDiscriminator.h>

// Include Digital Workprint test header files.
#include "libdwptest.h"

namespace basetest {

bool runKnownBugs() { return getenv("DWPTEST_RUN_KNOWN_BUGS") != NULL; }

#define KNOWN_BUG(REASON) \
	do { if (! basetest::runKnownBugs()) GTEST_SKIP() << "Known bug: " << REASON; } while (0)

// The text an item is written as, without the "#DWP" header.
std::string text(MleDwpItem *item, int children = 1) {
	MleDwpOutput out;
	item->write(&out, children);
	std::string s(out.getBuffer());
	s.erase(0, s.find('\n') + 1);
	// Squeeze the white space so that the tests do not depend on indenting.
	std::string r;
	bool space = true;
	for (char c : s) {
		if (isspace((unsigned char)c)) {
			if (! space) r += ' ';
			space = true;
		} else {
			r += c;
			space = false;
		}
	}
	while (! r.empty() && r.back() == ' ') r.pop_back();
	return r;
}

std::vector<MleDwpItem *> children(MleDwpItem *item) {
	std::vector<MleDwpItem *> list;
	for (MleDwpItem *c = item->getFirstChild(); c; c = c->getNext())
		list.push_back(c);
	return list;
}

std::string names(const std::vector<MleDwpItem *> &items) {
	std::string s;
	for (MleDwpItem *item : items) {
		if (! s.empty()) s += " ";
		s += item->getName();
	}
	return s;
}

std::string childNames(MleDwpItem *item) { return names(children(item)); }

MleDwpActorDef *actorDef(const char *name) {
	MleDwpActorDef *def = new MleDwpActorDef;
	def->setActorClass(name);
	return def;
}

#if defined(__GLIBC__) && (__GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 33))
#define HAVE_HEAP_SIZE 1
size_t heapInUse() { return mallinfo2().uordblks; }
#endif

} // namespace basetest

using namespace basetest;

// ---------------------------------------------------------------------------
// MleDwpDict: a dictionary with pointer keys.

namespace {
void *key(int n) { return (void *)(intptr_t)(n * 4); }
void *value(int n) { return (void *)(intptr_t)(1000 + n); }

// Everything in a dictionary, as a map of key to value.
std::map<const void *, const void *> contents(const MleDwpDict &dict) {
	std::map<const void *, const void *> all;
	MleDwpDictIter iter(dict);
	while (iter.getKey() != NULL) {
		all[iter.getKey()] = iter.getValue();
		if (! iter.next())
			break;
	}
	return all;
}
}

TEST(DictTest, EmptyDictionaryFindsNothing) {
	MleDwpDict dict;
	EXPECT_TRUE(dict.find(key(1)) == NULL);
	EXPECT_TRUE(contents(dict).empty());
}

TEST(DictTest, SetThenFind) {
	MleDwpDict dict;
	dict.set(key(1), value(1));
	dict.set(key(2), value(2));
	EXPECT_EQ(value(1), dict.find(key(1)));
	EXPECT_EQ(value(2), dict.find(key(2)));
	EXPECT_TRUE(dict.find(key(3)) == NULL);
}

TEST(DictTest, SetReplacesTheValue) {
	MleDwpDict dict;
	dict.set(key(1), value(1));
	dict.set(key(1), value(9));
	EXPECT_EQ(value(9), dict.find(key(1)));
	EXPECT_EQ(1u, contents(dict).size());
}

TEST(DictTest, RemoveDeletesOnlyThatKey) {
	MleDwpDict dict;
	dict.set(key(1), value(1));
	dict.set(key(2), value(2));
	dict.remove(key(1));
	EXPECT_TRUE(dict.find(key(1)) == NULL);
	EXPECT_EQ(value(2), dict.find(key(2)));
	EXPECT_EQ(1u, contents(dict).size());
}

TEST(DictTest, RemovingAMissingKeyIsHarmless) {
	MleDwpDict dict;
	dict.remove(key(1));
	dict.set(key(1), value(1));
	dict.remove(key(2));
	EXPECT_EQ(value(1), dict.find(key(1)));
}

TEST(DictTest, ManyKeysInOneBucketAreAllFound) {
	// A hash table of one bucket makes every key collide.
	MleDwpDict dict(1);
	for (int i = 1; i <= 200; i++)
		dict.set(key(i), value(i));
	for (int i = 1; i <= 200; i++)
		ASSERT_EQ(value(i), dict.find(key(i))) << i;

	// Remove from the head, the middle and the end of the chain.
	dict.remove(key(200));
	dict.remove(key(100));
	dict.remove(key(1));
	EXPECT_TRUE(dict.find(key(200)) == NULL);
	EXPECT_TRUE(dict.find(key(100)) == NULL);
	EXPECT_TRUE(dict.find(key(1)) == NULL);
	EXPECT_EQ(197u, contents(dict).size());
}

TEST(DictTest, IteratorVisitsEveryEntryOnce) {
	MleDwpDict dict(7);
	for (int i = 1; i <= 50; i++)
		dict.set(key(i), value(i));
	std::map<const void *, const void *> all = contents(dict);
	ASSERT_EQ(50u, all.size());
	for (int i = 1; i <= 50; i++)
		EXPECT_EQ(value(i), all[key(i)]) << i;
}

TEST(DictTest, IteratorOverAnEmptyDictionary) {
	MleDwpDict dict;
	MleDwpDictIter iter(dict);
	EXPECT_TRUE(iter.getKey() == NULL);
	EXPECT_TRUE(iter.getValue() == NULL);
	EXPECT_EQ(0, iter.next());
}

// ---------------------------------------------------------------------------
// MleDwpStrKeyDict: a dictionary with string keys.

TEST(StrKeyDictTest, SetThenFind) {
	MleDwpStrKeyDict dict;
	dict.set("one", value(1));
	dict.set("two", value(2));
	EXPECT_EQ(value(1), dict.find("one"));
	EXPECT_EQ(value(2), dict.find("two"));
	EXPECT_TRUE(dict.find("three") == NULL);
}

TEST(StrKeyDictTest, KeysAreCompared) {
	// Not by pointer: a different buffer with the same text is the same key.
	MleDwpStrKeyDict dict;
	char first[] = "key";
	char second[] = "key";
	dict.set(first, value(1));
	EXPECT_EQ(value(1), dict.find(second));
}

TEST(StrKeyDictTest, KeysAreCopied) {
	MleDwpStrKeyDict dict;
	char buffer[] = "original";
	dict.set(buffer, value(1));
	strcpy(buffer, "changed!");
	EXPECT_EQ(value(1), dict.find("original"));
	EXPECT_TRUE(dict.find("changed!") == NULL);
}

TEST(StrKeyDictTest, KeysAreCaseSensitive) {
	MleDwpStrKeyDict dict;
	dict.set("Key", value(1));
	EXPECT_TRUE(dict.find("key") == NULL);
	EXPECT_TRUE(dict.find("KEY") == NULL);
	EXPECT_EQ(value(1), dict.find("Key"));
}

TEST(StrKeyDictTest, SetReplacesTheValue) {
	MleDwpStrKeyDict dict;
	dict.set("k", value(1));
	dict.set("k", value(2));
	EXPECT_EQ(value(2), dict.find("k"));
	EXPECT_EQ(1u, contents(dict).size());
}

TEST(StrKeyDictTest, Remove) {
	MleDwpStrKeyDict dict;
	dict.set("a", value(1));
	dict.set("b", value(2));
	dict.remove("a");
	EXPECT_TRUE(dict.find("a") == NULL);
	EXPECT_EQ(value(2), dict.find("b"));
	dict.remove("missing");
	EXPECT_EQ(1u, contents(dict).size());
}

TEST(StrKeyDictTest, ManyKeysAndLongKeys) {
	MleDwpStrKeyDict dict(5);
	std::vector<std::string> keys;
	for (int i = 0; i < 500; i++)
		keys.push_back("key" + std::string(i % 50, 'x') + std::to_string(i));
	for (int i = 0; i < 500; i++)
		dict.set(keys[i].c_str(), value(i));
	for (int i = 0; i < 500; i++)
		ASSERT_EQ(value(i), dict.find(keys[i].c_str())) << keys[i];
	EXPECT_EQ(500u, contents(dict).size());
}

TEST(StrKeyDictTest, KeysThatDifferByOneCharacter) {
	MleDwpStrKeyDict dict;
	dict.set("ab", value(1));
	dict.set("ba", value(2));
	dict.set("abc", value(3));
	dict.set("ab ", value(4));
	EXPECT_EQ(value(1), dict.find("ab"));
	EXPECT_EQ(value(2), dict.find("ba"));
	EXPECT_EQ(value(3), dict.find("abc"));
	EXPECT_EQ(value(4), dict.find("ab "));
}

TEST(StrKeyDictTest, EmptyStringKey) {
	// The hash of "" must not depend on what follows the terminator, so the
	// key is found through a buffer with different bytes after it.
	KNOWN_BUG("MleDwpStrKeyDict::hash() is a do/while, so the hash of an empty "
		"string reads past its terminator");
	MleDwpStrKeyDict dict;
	char first[2] = { 0, 'x' };
	char second[2] = { 0, 'y' };
	dict.set(first, value(1));
	EXPECT_EQ(value(1), dict.find(second));
}

// ---------------------------------------------------------------------------
// The item tree: containership and tags.

namespace {
const char *g_header = "#DWP 1.0 ascii\n";

dwptest::WorkprintPtr load(const std::string &body) {
	::testing::internal::CaptureStderr();
	dwptest::WorkprintPtr root = dwptest::loadString(g_header + body);
	::testing::internal::GetCapturedStderr();
	return root;
}

// Three top level items, a b c, of which b is tagged "x" and c "x" and "y".
const char *g_tagged = "( ActorDef a )\n( ActorDef +x b )\n( ActorDef +x +y c )\n";
}

TEST(TreeTest, ParentAndSiblingsAreLinked) {
	dwptest::WorkprintPtr root = load(g_tagged);
	ASSERT_TRUE(root != NULL);
	std::vector<MleDwpItem *> items = children(root.get());
	ASSERT_EQ(3u, items.size());
	EXPECT_TRUE(items[0]->getPrev() == NULL);
	EXPECT_EQ(items[1], items[0]->getNext());
	EXPECT_EQ(items[0], items[1]->getPrev());
	EXPECT_TRUE(items[2]->getNext() == NULL);
	EXPECT_EQ(items[2], root->getLastChild());
	for (MleDwpItem *item : items)
		EXPECT_EQ(root.get(), item->getParent());
}

namespace {
// Marks the items it is given as PHANTOM, and the others as USE.
class PhantomNamed : public MleDwpDiscriminator {
  public:
	const char *name;
	PhantomNamed(const char *n) : name(n) {}
	virtual State discriminate(const MleDwpItem *item) {
		return strcmp(item->getName(), name) == 0 ? PHANTOM : USE;
	}
};
}

TEST(TreeTest, InsertBeforeAnItemUnderAPhantomGoesInTheSameList) {
	// With a phantom parent its children appear in its parent's list, so a
	// new child can be put before one of them. It goes into the phantom.
	dwptest::WorkprintPtr root = load("( ActorDef a )\n( Group p ( ActorDef x ) )\n");
	ASSERT_TRUE(root != NULL);
	PhantomNamed d("p");
	root->setDiscriminator(&d);
	MleDwpItem *x = root->getFirstChild()->getNext();
	ASSERT_TRUE(x != NULL);
	EXPECT_STREQ("x", x->getName());
	MleDwpContainer *added = new MleDwpContainer;
	added->setName("added");
	root->insertChild(added, x);
	EXPECT_EQ(x, added->getNext());
	EXPECT_STREQ("a added x", names([&] {
		std::vector<MleDwpItem *> all;
		for (MleDwpItem *i = root->getFirstChild(); i; i = i->getNext()) all.push_back(i);
		return all;
	}()).c_str());
	root->setDiscriminator(NULL);
}

TEST(TreeTest, InsertBeforeAnItemThatIsNotADescendantViaAPhantomAsserts) {
#if defined(MLE_DEBUG)
	dwptest::WorkprintPtr root = load("( Scene s ( Group g ) )\n");
	ASSERT_TRUE(root != NULL);
	MleDwpItem *group = root->getFirstChild()->getFirstChild();
	EXPECT_DEATH({
			MleDwpContainer *added = new MleDwpContainer;
			root->insertChild(added, group);
		}, "");
#else
	GTEST_SKIP() << "The assertion is only in debug builds.";
#endif
}

TEST(TreeTest, RemovedChildIsUnlinked) {
	dwptest::WorkprintPtr root = load(g_tagged);
	ASSERT_TRUE(root != NULL);
	MleDwpItem *middle = root->getFirstChild()->getNext();
	root->removeChild(middle);
	EXPECT_TRUE(middle->getParent() == NULL);
	EXPECT_TRUE(middle->getNext() == NULL);
	EXPECT_TRUE(middle->getPrev() == NULL);
	EXPECT_EQ("a c", childNames(root.get()));
	delete middle;
}

TEST(TreeTest, DeletingAChildRemovesItFromItsParent) {
	dwptest::WorkprintPtr root = load(g_tagged);
	ASSERT_TRUE(root != NULL);
	delete root->getFirstChild();
	EXPECT_EQ("b c", childNames(root.get()));
}

TEST(TreeTest, TagsAreOrderedAndNotDuplicated) {
	MleDwpContainer item;
	EXPECT_EQ(0, item.hasAnyTag());
	item.addTag("one");
	item.addTag("two");
	item.addTag("one");
	EXPECT_EQ(1, item.hasAnyTag());
	EXPECT_STREQ("one", item.getNthTag(0));
	EXPECT_STREQ("two", item.getNthTag(1));
	EXPECT_TRUE(item.getNthTag(2) == NULL);
	EXPECT_TRUE(item.hasTag("one"));
	EXPECT_FALSE(item.hasTag("three"));
}

TEST(TreeTest, RemovingTags) {
	MleDwpContainer item;
	item.addTag("a");
	item.addTag("b");
	item.addTag("c");
	item.removeTag("c");          // The last tag.
	item.addTag("d");             // Added after the last was removed.
	EXPECT_STREQ("d", item.getNthTag(2));
	item.removeTag("a");          // The first tag.
	item.removeTag("missing");
	EXPECT_STREQ("b", item.getNthTag(0));
	EXPECT_STREQ("d", item.getNthTag(1));
	item.removeTag("b");
	item.removeTag("d");
	EXPECT_EQ(0, item.hasAnyTag());
	item.addTag("again");
	EXPECT_STREQ("again", item.getNthTag(0));
}

// ---------------------------------------------------------------------------
// MleDwpContainer.

TEST(ContainerTest, TransparentContainerIsNotWritten) {
	dwptest::WorkprintPtr root = load("( Container ( ActorDef a ) ( ActorDef b ) )\n");
	ASSERT_TRUE(root != NULL);
	EXPECT_EQ("( ActorDef a ) ( ActorDef b )", text(root.get()));
}

TEST(ContainerTest, OpaqueContainerIsWritten) {
	dwptest::WorkprintPtr root = load("( Container ( ActorDef a ) )\n");
	ASSERT_TRUE(root != NULL);
	MleDwpContainer *container = (MleDwpContainer *)root->getFirstChild();
	ASSERT_TRUE(container != NULL);
	container->setTransparent(0);
	std::string s = text(container);
	EXPECT_EQ(0u, s.find("( Container")) << s;
	EXPECT_NE(std::string::npos, s.find("ActorDef a")) << s;
}

TEST(ContainerTest, CopyKeepsTheTransparentSetting) {
	dwptest::WorkprintPtr root = load("( Container ( ActorDef a ) )\n");
	ASSERT_TRUE(root != NULL);
	MleDwpContainer *container = (MleDwpContainer *)root->getFirstChild();
	ASSERT_TRUE(container != NULL);

	// Transparent: the copy is written as its children only.
	std::unique_ptr<MleDwpItem> copy1(container->copy());
	EXPECT_EQ(0u, text(copy1.get()).find("( ActorDef a")) << text(copy1.get());

	// Opaque: the copy is written as a container.
	container->setTransparent(0);
	std::unique_ptr<MleDwpItem> copy2(container->copy());
	EXPECT_EQ(0u, text(copy2.get()).find("( Container")) << text(copy2.get());
	// The original is as it was.
	EXPECT_EQ(0u, text(container).find("( Container"));
}

TEST(ContainerTest, CopyHasItsOwnChildren) {
	dwptest::WorkprintPtr root = load("( Container ( ActorDef a ) ( ActorDef b ) )\n");
	ASSERT_TRUE(root != NULL);
	MleDwpItem *container = root->getFirstChild();
	std::unique_ptr<MleDwpItem> copy(container->copy());
	EXPECT_EQ("a b", childNames(copy.get()));
	EXPECT_NE(container->getFirstChild(), copy->getFirstChild());
	std::unique_ptr<MleDwpItem> bare(container->copy(0));
	EXPECT_TRUE(bare->getFirstChild() == NULL);
}

// ---------------------------------------------------------------------------
// Discriminators.

TEST(DiscriminatorTest, BaseDiscriminatorUsesEverything) {
	MleDwpDiscriminator d;
	MleDwpContainer item;
	EXPECT_EQ(MleDwpDiscriminator::USE, d.discriminate(&item));
	EXPECT_EQ(1, d.getEnabled());
	EXPECT_EQ(1, d.setEnabled(0));
	EXPECT_EQ(0, d.getEnabled());
}

TEST(TagDiscriminatorTest, ItemsWithoutTagsAreAlwaysUsed) {
	MleDwpTagDiscriminator d;
	MleDwpContainer item;
	EXPECT_EQ(MleDwpDiscriminator::USE, d.discriminate(&item));
	d.addTag("x");
	EXPECT_EQ(MleDwpDiscriminator::USE, d.discriminate(&item));
}

TEST(TagDiscriminatorTest, TaggedItemsNeedOneOfTheTags) {
	MleDwpTagDiscriminator d;
	MleDwpContainer item;
	item.addTag("x");
	item.addTag("y");
	EXPECT_EQ(MleDwpDiscriminator::SKIP, d.discriminate(&item));   // No tags.
	d.addTag("z");
	EXPECT_EQ(MleDwpDiscriminator::SKIP, d.discriminate(&item));
	d.addTag("y");
	EXPECT_EQ(MleDwpDiscriminator::USE, d.discriminate(&item));
	d.removeTag("y");
	EXPECT_EQ(MleDwpDiscriminator::SKIP, d.discriminate(&item));
	d.removeTag("missing");
}

TEST(TagDiscriminatorTest, AddingATagTwiceIsHarmless) {
	MleDwpTagDiscriminator d;
	MleDwpContainer item;
	item.addTag("x");
	d.addTag("x");
	d.addTag("x");
	d.removeTag("x");
	EXPECT_EQ(MleDwpDiscriminator::SKIP, d.discriminate(&item));
}

TEST(TagDiscriminatorTest, DisabledUsesEverything) {
	MleDwpTagDiscriminator d;
	MleDwpContainer item;
	item.addTag("x");
	EXPECT_EQ(MleDwpDiscriminator::SKIP, d.discriminate(&item));
	d.setEnabled(0);
	EXPECT_EQ(MleDwpDiscriminator::USE, d.discriminate(&item));
}

TEST(TagAllDiscriminatorTest, EveryTagOfTheItemMustBeWanted) {
	MleDwpTagAllDiscriminator d;
	MleDwpContainer item;
	item.addTag("x");
	item.addTag("y");
	d.addTag("x");
	EXPECT_EQ(MleDwpDiscriminator::SKIP, d.discriminate(&item));
	d.addTag("y");
	EXPECT_EQ(MleDwpDiscriminator::USE, d.discriminate(&item));
	d.addTag("z");
	EXPECT_EQ(MleDwpDiscriminator::USE, d.discriminate(&item));
	MleDwpContainer plain;
	EXPECT_EQ(MleDwpDiscriminator::USE, d.discriminate(&plain));
	d.setEnabled(0);
	d.removeTag("x");
	EXPECT_EQ(MleDwpDiscriminator::USE, d.discriminate(&item));
}

TEST(DiscriminatorTreeTest, SkippedItemsAreNotVisited) {
	dwptest::WorkprintPtr root = load(g_tagged);
	ASSERT_TRUE(root != NULL);
	MleDwpTagDiscriminator d;
	d.addTag("y");
	root->setDiscriminator(&d);
	// a has no tags and c has y. b has only x.
	EXPECT_EQ("a c", childNames(root.get()));
	MleDwpItem *first = root->getFirstChild();
	EXPECT_STREQ("c", first->getNext()->getName());
	EXPECT_STREQ("a", root->getLastChild()->getPrev() ? root->getLastChild()->getPrev()->getName() : "");
	root->setDiscriminator(NULL);
}

TEST(DiscriminatorTreeTest, TheFirstItemMayBeSkipped) {
	dwptest::WorkprintPtr root = load("( ActorDef +x a )\n( ActorDef b )\n( ActorDef +y c )\n");
	ASSERT_TRUE(root != NULL);
	MleDwpTagDiscriminator d;
	d.addTag("y");
	root->setDiscriminator(&d);
	EXPECT_EQ("b c", childNames(root.get()));
	EXPECT_STREQ("c", root->getLastChild()->getName());
	root->setDiscriminator(NULL);
}

TEST(DiscriminatorTreeTest, TheLastItemMayBeSkipped) {
	dwptest::WorkprintPtr root = load("( ActorDef +y a )\n( ActorDef b )\n( ActorDef +x c )\n");
	ASSERT_TRUE(root != NULL);
	MleDwpTagDiscriminator d;
	d.addTag("y");
	root->setDiscriminator(&d);
	EXPECT_EQ("a b", childNames(root.get()));
	EXPECT_STREQ("b", root->getLastChild()->getName());
	root->setDiscriminator(NULL);
}

TEST(DiscriminatorTreeTest, EverythingSkipped) {
	dwptest::WorkprintPtr root = load("( ActorDef +x a )\n( ActorDef +x b )\n");
	ASSERT_TRUE(root != NULL);
	MleDwpTagDiscriminator d;
	d.addTag("y");
	root->setDiscriminator(&d);
	EXPECT_TRUE(root->getFirstChild() == NULL);
	EXPECT_TRUE(root->getLastChild() == NULL);
	root->setDiscriminator(NULL);
}

TEST(DiscriminatorTreeTest, TheDiscriminatorReachesTheDescendants) {
	dwptest::WorkprintPtr root = load("( Scene s ( Group +x g ) ( Group h ) )\n");
	ASSERT_TRUE(root != NULL);
	MleDwpTagDiscriminator d;
	d.addTag("y");
	root->setDiscriminator(&d);
	MleDwpItem *scene = root->getFirstChild();
	ASSERT_TRUE(scene != NULL);
	EXPECT_EQ(&d, scene->getDiscriminator());
	EXPECT_EQ("h", childNames(scene));
	root->setDiscriminator(NULL);
	EXPECT_EQ("g h", childNames(scene));
}

TEST(DiscriminatorTreeTest, ItemsAddedLaterUseTheDiscriminator) {
	dwptest::WorkprintPtr root = load("( ActorDef b )\n");
	ASSERT_TRUE(root != NULL);
	MleDwpTagDiscriminator d;
	d.addTag("y");
	root->setDiscriminator(&d);
	MleDwpContainer *added = new MleDwpContainer;
	added->setName("added");
	root->insertChild(added, NULL);
	EXPECT_EQ(&d, added->getDiscriminator());
	root->setDiscriminator(NULL);
}

TEST(DiscriminatorTreeTest, PhantomParentPassesItsChildrenUp) {
	dwptest::WorkprintPtr root = load("( ActorDef a )\n( Group p ( ActorDef x ) ( ActorDef y ) )\n( ActorDef z )\n");
	ASSERT_TRUE(root != NULL);
	PhantomNamed d("p");
	root->setDiscriminator(&d);
	EXPECT_EQ("a x y z", names([&] {
		std::vector<MleDwpItem *> all;
		MleDwpItem *item = root->getFirstChild();
		while (item && all.size() < 10) {
			all.push_back(item);
			item = item->getNext();
		}
		return all;
	}()));
	root->setDiscriminator(NULL);
}

TEST(DiscriminatorTreeTest, TheParentOfAnItemUnderAPhantomIsTheGrandparent) {
	dwptest::WorkprintPtr root = load("( Scene s ( Group p ( ActorDef x ) ) )\n");
	ASSERT_TRUE(root != NULL);
	MleDwpItem *scene = root->getFirstChild();
	MleDwpItem *group = scene->getFirstChild();
	MleDwpItem *x = group->getFirstChild();
	ASSERT_TRUE(x != NULL);
	PhantomNamed d("p");
	root->setDiscriminator(&d);
	EXPECT_EQ(scene, x->getParent());
	root->setDiscriminator(NULL);
	EXPECT_EQ(group, x->getParent());
}

TEST(DiscriminatorTreeTest, AncestorsSkipItemsThatAreNotUsed) {
	dwptest::WorkprintPtr root = load("( Scene s ( Group p ( ActorDef x ) ) )\n");
	ASSERT_TRUE(root != NULL);
	MleDwpItem *x = root->getFirstChild()->getFirstChild()->getFirstChild();
	ASSERT_TRUE(x != NULL);
	PhantomNamed d("p");
	root->setDiscriminator(&d);
	EXPECT_TRUE(x->getAncestor(MleDwpGroup::typeId) == NULL);
	EXPECT_TRUE(x->getAncestor(MleDwpScene::typeId) != NULL);
	root->setDiscriminator(NULL);
	EXPECT_TRUE(x->getAncestor(MleDwpGroup::typeId) != NULL);
}

// ---------------------------------------------------------------------------
// MleDwpFinder.

namespace {
const char *g_findable =
	"( Scene s1 ( Group g1 ( ActorDef a ) ( ActorDef b ) ) )\n"
	"( Scene s2 ( Group g2 ( ActorDef a ) ) )\n";

int onlyNamedB(MleDwpItem *item, void *data) {
	if (data) ++*(int *)data;
	return strcmp(item->getName(), "b") == 0;
}
}

TEST(FinderTest, FindsTheFirstOfAType) {
	dwptest::WorkprintPtr root = load(g_findable);
	ASSERT_TRUE(root != NULL);
	MleDwpFinder finder(MleDwpActorDef::typeId);
	MleDwpItem *found = finder.find(root.get());
	ASSERT_TRUE(found != NULL);
	EXPECT_STREQ("a", found->getName());
	EXPECT_EQ(found, finder.getItem());
	EXPECT_EQ(1, finder.getNumItems());
}

TEST(FinderTest, FindsAllOfAType) {
	dwptest::WorkprintPtr root = load(g_findable);
	ASSERT_TRUE(root != NULL);
	MleDwpFinder finder(MleDwpActorDef::typeId, NULL, 1);
	finder.find(root.get());
	ASSERT_EQ(3, finder.getNumItems());
	EXPECT_STREQ("a", finder.getItems()[0]->getName());
	EXPECT_STREQ("b", finder.getItems()[1]->getName());
	EXPECT_STREQ("a", finder.getItems()[2]->getName());
}

TEST(FinderTest, FindsByTypeName) {
	dwptest::WorkprintPtr root = load(g_findable);
	ASSERT_TRUE(root != NULL);
	MleDwpFinder finder("MleDwpScene", NULL, 1);
	finder.find(root.get());
	EXPECT_EQ(2, finder.getNumItems());
}

TEST(FinderTest, FindsByName) {
	dwptest::WorkprintPtr root = load(g_findable);
	ASSERT_TRUE(root != NULL);
	MleDwpFinder finder((MleDwpType)NULL, "g2");
	MleDwpItem *found = finder.find(root.get());
	ASSERT_TRUE(found != NULL);
	EXPECT_STREQ("g2", found->getName());
}

TEST(FinderTest, FindsByTypeAndName) {
	dwptest::WorkprintPtr root = load(g_findable);
	ASSERT_TRUE(root != NULL);
	MleDwpFinder finder(MleDwpActorDef::typeId, "b", 1);
	finder.find(root.get());
	EXPECT_EQ(1, finder.getNumItems());
	MleDwpFinder none(MleDwpScene::typeId, "b", 1);
	EXPECT_TRUE(none.find(root.get()) == NULL);
	EXPECT_EQ(0, none.getNumItems());
}

TEST(FinderTest, SubclassesMatchTheirBaseType) {
	dwptest::WorkprintPtr root = load(g_findable);
	ASSERT_TRUE(root != NULL);
	MleDwpFinder finder(MleDwpItem::typeId, NULL, 1);
	finder.find(root.get());
	// The workprint root, the scenes, groups and actors.
	EXPECT_EQ(1 + 2 + 2 + 3, finder.getNumItems());
}

TEST(FinderTest, CallbackSelectsTheItems) {
	dwptest::WorkprintPtr root = load(g_findable);
	ASSERT_TRUE(root != NULL);
	int calls = 0;
	MleDwpFinder finder(MleDwpActorDef::typeId, NULL, 1);
	finder.setCallback(onlyNamedB, &calls);
	finder.find(root.get());
	EXPECT_EQ(1, finder.getNumItems());
	// Only items of the type reach the callback.
	EXPECT_EQ(3, calls);
}

TEST(FinderTest, EachFindStartsAgain) {
	dwptest::WorkprintPtr root = load(g_findable);
	ASSERT_TRUE(root != NULL);
	MleDwpFinder finder(MleDwpActorDef::typeId, NULL, 1);
	finder.find(root.get());
	finder.find(root.get());
	EXPECT_EQ(3, finder.getNumItems());
	finder.setFindAll(0);
	finder.find(root.get());
	EXPECT_EQ(1, finder.getNumItems());
}

TEST(FinderTest, SearchStartsAtTheGivenItemAndIncludesIt) {
	dwptest::WorkprintPtr root = load(g_findable);
	ASSERT_TRUE(root != NULL);
	MleDwpItem *scene2 = root->getFirstChild()->getNext();
	MleDwpFinder finder(MleDwpActorDef::typeId, NULL, 1);
	finder.find(scene2);
	EXPECT_EQ(1, finder.getNumItems());
	MleDwpFinder self(MleDwpScene::typeId, NULL, 1);
	EXPECT_EQ(scene2, self.find(scene2));
}

TEST(FinderTest, SetNameReplacesTheName) {
	dwptest::WorkprintPtr root = load(g_findable);
	ASSERT_TRUE(root != NULL);
	MleDwpFinder finder((MleDwpType)NULL, "g1");
	finder.setName("g2");
	EXPECT_STREQ("g2", finder.find(root.get())->getName());
	finder.setName(NULL);
	EXPECT_TRUE(finder.find(root.get()) == root.get());   // Matches anything.
}

TEST(FinderTest, ChangingTheTypeFromANameToATypeId) {
	// setType(MleDwpType) frees the type name, but does not clear the pointer,
	// so the finder frees it again when it is destroyed.
	KNOWN_BUG("MleDwpFinder::setType(MleDwpType) leaves m_typeString dangling");
	EXPECT_EXIT({
			MleDwpFinder *finder = new MleDwpFinder("MleDwpScene");
			finder->setType(MleDwpActorDef::typeId);
			delete finder;
			exit(0);
		}, ::testing::ExitedWithCode(0), "");
}

TEST(FinderTest, ChangingTheTypeNameTwice) {
	KNOWN_BUG("MleDwpFinder::setType(const char *) does not free the previous type name");
#if defined(HAVE_HEAP_SIZE)
	size_t before = heapInUse();
	for (int i = 0; i < 1000; i++) {
		MleDwpFinder finder("MleDwpScene");
		finder.setType("MleDwpGroup");
	}
	EXPECT_EQ(before, heapInUse());
#else
	GTEST_SKIP() << "No heap statistics.";
#endif
}

// ---------------------------------------------------------------------------
// MleDwpFilename.

TEST(FilenameTest, StartsEmpty) {
	MleDwpFilename f;
	EXPECT_TRUE(f.getName() == NULL);
	EXPECT_TRUE(f.getPath() == NULL);
}

TEST(FilenameTest, NameAndPathAreCopied) {
	MleDwpFilename f;
	char name[] = "scene.dwp";
	char path[] = "/a/b";
	f.setName(name);
	f.setPath(path);
	strcpy(name, "XXXXXXXXX");
	strcpy(path, "/z/z");
	EXPECT_STREQ("scene.dwp", f.getName());
	EXPECT_STREQ("/a/b", f.getPath());
}

TEST(FilenameTest, SettingAgainReplacesAndNullClears) {
	MleDwpFilename f;
	f.setName("first");
	f.setName("second");
	EXPECT_STREQ("second", f.getName());
	f.setName(NULL);
	EXPECT_TRUE(f.getName() == NULL);
	f.setPath("p");
	f.setPath(NULL);
	EXPECT_TRUE(f.getPath() == NULL);
}

TEST(FilenameTest, DoesNotLeak) {
#if defined(HAVE_HEAP_SIZE)
	auto once = [] {
		MleDwpFilename f;
		f.setName("a long file name for the leak test");
		f.setName("another");
		f.setPath("/some/path");
	};
	// Warm up, so that memory the library keeps for good is not counted.
	for (int i = 0; i < 10; i++) once();
	size_t before = heapInUse();
	for (int i = 0; i < 1000; i++) once();
	EXPECT_EQ(before, heapInUse());
#else
	GTEST_SKIP() << "No heap statistics.";
#endif
}

// ---------------------------------------------------------------------------
// MleDwpNotifier and MleDwpMonitor.

namespace {
struct Event {
	const void *data;
	MleDwpNotifierType type;
	const void *client;
};
std::vector<Event> g_events;

void record(const void *data, MleDwpNotifierType type, const void *client) {
	g_events.push_back({data, type, client});
}

int g_order[8];
int g_orderCount = 0;
void first(const void *, MleDwpNotifierType, const void *) { g_order[g_orderCount++] = 1; }
void second(const void *, MleDwpNotifierType, const void *) { g_order[g_orderCount++] = 2; }
void third(const void *, MleDwpNotifierType, const void *) { g_order[g_orderCount++] = 3; }
int g_count = 0;
void count(const void *, MleDwpNotifierType, const void *) { g_count++; }

class NotifierTest : public ::testing::Test {
  protected:
	void SetUp() override { g_events.clear(); g_orderCount = 0; g_count = 0; }
};
}

TEST_F(NotifierTest, CallbackReceivesTheDataTypeAndClient) {
	MleDwpNotifier n(16, 4);
	int k, d, c;
	n.addCallback(&k, record, &c);
	n.notify(&k, &d, MleDwpChangeNotification);
	ASSERT_EQ(1u, g_events.size());
	EXPECT_EQ(&d, g_events[0].data);
	EXPECT_EQ(MleDwpChangeNotification, g_events[0].type);
	EXPECT_EQ(&c, g_events[0].client);
}

TEST_F(NotifierTest, OnlyTheKeyIsNotified) {
	MleDwpNotifier n(1, 4);       // One bucket, so the keys share a list.
	int k1, k2;
	n.addCallback(&k1, record, NULL);
	n.notify(&k2, &k2, MleDwpChangeNotification);
	EXPECT_TRUE(g_events.empty());
}

TEST_F(NotifierTest, CallbacksRunInTheOrderTheyWereAdded) {
	MleDwpNotifier n(16, 4);
	int k;
	n.addCallback(&k, first, NULL);
	n.addCallback(&k, second, NULL);
	n.addCallback(&k, third, NULL);
	n.notify(&k, &k, MleDwpChangeNotification);
	ASSERT_EQ(3, g_orderCount);
	EXPECT_EQ(1, g_order[0]);
	EXPECT_EQ(2, g_order[1]);
	EXPECT_EQ(3, g_order[2]);
}

TEST_F(NotifierTest, RemoveCallbackRemovesJustThatOne) {
	MleDwpNotifier n(16, 4);
	int k;
	n.addCallback(&k, first, NULL);
	n.addCallback(&k, second, NULL);
	n.removeCallback(&k, first, NULL);
	n.notify(&k, &k, MleDwpChangeNotification);
	ASSERT_EQ(1, g_orderCount);
	EXPECT_EQ(2, g_order[0]);
}

TEST_F(NotifierTest, RemoveCallbacksRemovesAllForTheKey) {
	MleDwpNotifier n(1, 4);
	int k1, k2;
	n.addCallback(&k1, first, NULL);
	n.addCallback(&k2, second, NULL);
	n.addCallback(&k1, third, NULL);
	n.removeCallbacks(&k1);
	n.notify(&k1, &k1, MleDwpChangeNotification);
	n.notify(&k2, &k2, MleDwpChangeNotification);
	ASSERT_EQ(1, g_orderCount);
	EXPECT_EQ(2, g_order[0]);
}

TEST_F(NotifierTest, RemovedEntriesAreReused) {
	MleDwpNotifier n(16, 2);
	int k;
	for (int i = 0; i < 50; i++) {
		n.addCallback(&k, first, NULL);
		n.removeCallback(&k, first, NULL);
	}
	n.notify(&k, &k, MleDwpChangeNotification);
	EXPECT_EQ(0, g_orderCount);
}

TEST_F(NotifierTest, PoolGrowsWhenItRunsOut) {
	MleDwpNotifier n(16, 2);
	int keys[100];
	for (int i = 0; i < 100; i++)
		n.addCallback(&keys[i], count, NULL);
	for (int i = 0; i < 100; i++)
		n.notify(&keys[i], NULL, MleDwpChangeNotification);
	EXPECT_EQ(100, g_count);
}

TEST_F(NotifierTest, NotifyWithNoCallbacksIsHarmless) {
	MleDwpNotifier n(16, 4);
	int k;
	n.notify(&k, &k, MleDwpDeleteNotification);
	n.notify(NULL, NULL, MleDwpDeleteNotification);
}

TEST_F(NotifierTest, NotifiersCanShareAPool) {
	MleDwpNotifierPool pool(4);
	MleDwpNotifier a(8, &pool);
	MleDwpNotifier b(8, &pool);
	EXPECT_EQ(&pool, a.getPool());
	EXPECT_EQ(&pool, b.getPool());
	int k;
	a.addCallback(&k, first, NULL);
	b.addCallback(&k, second, NULL);
	a.notify(&k, &k, MleDwpChangeNotification);
	ASSERT_EQ(1, g_orderCount);
	EXPECT_EQ(1, g_order[0]);
}

TEST_F(NotifierTest, RemovingACallbackThatWasNotAddedAsserts) {
#if defined(MLE_DEBUG)
	MleDwpNotifier n(16, 4);
	int k;
	EXPECT_DEATH(n.removeCallback(&k, first, NULL), "");
#else
	GTEST_SKIP() << "The assertion is only in debug builds.";
#endif
}

TEST_F(NotifierTest, ItemsNotifyOnCreateChangeAndDelete) {
	void *key = (void *)MleDwpContainer::typeId;
	MleDwpItem::m_notifier.addCallback(key, record, NULL);
	MleDwpContainer *item = new MleDwpContainer;
	ASSERT_EQ(1u, g_events.size());
	EXPECT_EQ(item, g_events[0].data);
	EXPECT_EQ(MleDwpCreateNotification, g_events[0].type);
	MleDwpItem::m_notifier.removeCallback(key, record, NULL);

	g_events.clear();
	MleDwpItem::m_notifier.addCallback(item, record, NULL);
	item->setTransparent(0);
	item->setName("renamed");
	delete item;
	MleDwpItem::m_notifier.removeCallbacks(item);
	ASSERT_GE(g_events.size(), 3u);
	EXPECT_EQ(MleDwpChangeNotification, g_events.front().type);
	EXPECT_EQ(MleDwpDeleteNotification, g_events.back().type);
}

TEST_F(NotifierTest, ParentIsToldWhenChildrenAreInsertedAndRemoved) {
	MleDwpContainer parent;
	MleDwpItem::m_notifier.addCallback(&parent, record, NULL);
	MleDwpContainer *child = new MleDwpContainer;
	parent.insertChild(child, NULL);
	parent.removeChild(child);
	delete child;
	MleDwpItem::m_notifier.removeCallback(&parent, record, NULL);
	std::vector<MleDwpNotifierType> types;
	for (const Event &e : g_events) types.push_back(e.type);
	EXPECT_NE(types.end(), std::find(types.begin(), types.end(), MleDwpChildInsertedNotification));
	EXPECT_NE(types.end(), std::find(types.begin(), types.end(), MleDwpChildRemovedNotification));
	EXPECT_NE(types.end(), std::find(types.begin(), types.end(), MleDwpDescendentInsertedNotification));
	EXPECT_NE(types.end(), std::find(types.begin(), types.end(), MleDwpDescendentRemovedNotification));
}

// ---------------------------------------------------------------------------
// MleDwpInclude: workprints that include other workprints.

namespace {
// A directory for the files of one test, which is removed afterwards.
class IncludeTest : public ::testing::Test {
  protected:
	std::string dir;
	MleDwpInclude::Enabling saved;

	void SetUp() override {
		char pattern[] = "/tmp/dwpincludeXXXXXX";
		ASSERT_TRUE(mkdtemp(pattern) != NULL);
		dir = pattern;
		saved = MleDwpInclude::getClassWriteChildren();
		MleDwpInclude::setClassWriteChildren(MleDwpInclude::DEFAULT);
	}
	void TearDown() override {
		MleDwpInclude::setClassWriteChildren(saved);
		std::error_code ec;
		std::filesystem::remove_all(dir, ec);
	}
	std::string file(const std::string &name, const std::string &body) {
		std::string path = dir + "/" + name;
		std::ofstream out(path);
		out << body;
		return path;
	}
	std::string slurp(const std::string &path) {
		std::ifstream in(path);
		return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	}
};
}

TEST_F(IncludeTest, ChildrenComeFromTheIncludedFile) {
	std::string inc = file("inc.dwp", "#DWP 1.0 ascii\n( ActorDef a )\n( ActorDef b )\n");
	dwptest::WorkprintPtr root = load("( Include " + inc + " )\n( ActorDef after )\n");
	ASSERT_TRUE(root != NULL);
	std::vector<MleDwpItem *> items = children(root.get());
	ASSERT_EQ(2u, items.size());
	EXPECT_EQ(std::string("MleDwpInclude"), items[0]->getTypeName());
	EXPECT_STREQ(inc.c_str(), items[0]->getName());
	EXPECT_EQ("a b", childNames(items[0]));
	EXPECT_STREQ("after", items[1]->getName());
}

TEST_F(IncludeTest, PathIsTheFileThatWasOpened) {
	std::string inc = file("inc.dwp", "#DWP 1.0 ascii\n( ActorDef a )\n");
	dwptest::WorkprintPtr root = load("( Include " + inc + " )\n");
	ASSERT_TRUE(root != NULL);
	MleDwpInclude *include = (MleDwpInclude *)root->getFirstChild();
	ASSERT_TRUE(include != NULL);
	ASSERT_TRUE(include->getPath() != NULL);
	EXPECT_EQ(inc, std::string(include->getPath()));
}

TEST_F(IncludeTest, IncludesCanBeNested) {
	std::string inner = file("inner.dwp", "#DWP 1.0 ascii\n( ActorDef deep )\n");
	std::string outer = file("outer.dwp", "#DWP 1.0 ascii\n( Include " + inner + " )\n( ActorDef mid )\n");
	dwptest::WorkprintPtr root = load("( Include " + outer + " )\n");
	ASSERT_TRUE(root != NULL);
	MleDwpItem *top = root->getFirstChild();
	ASSERT_TRUE(top != NULL);
	std::vector<MleDwpItem *> items = children(top);
	ASSERT_EQ(2u, items.size());
	EXPECT_EQ("deep", childNames(items[0]));
	EXPECT_STREQ("mid", items[1]->getName());
}

TEST_F(IncludeTest, RelativeNameIsFoundNextToTheIncludingFile) {
	file("sibling.dwp", "#DWP 1.0 ascii\n( ActorDef sib )\n");
	std::string outer = file("outer.dwp", "#DWP 1.0 ascii\n( Include sibling.dwp )\n");
	dwptest::WorkprintPtr root = load("( Include " + outer + " )\n");
	ASSERT_TRUE(root != NULL);
	MleDwpItem *top = root->getFirstChild();
	ASSERT_TRUE(top != NULL && top->getFirstChild() != NULL);
	EXPECT_EQ("sib", childNames(top->getFirstChild()));
}

TEST_F(IncludeTest, MissingFileIsReportedAndLoadingContinues) {
	::testing::internal::CaptureStderr();
	dwptest::WorkprintPtr root = dwptest::loadString(
		std::string(g_header) + "( Include " + dir + "/missing.dwp )\n( ActorDef after )\n");
	std::string errors = ::testing::internal::GetCapturedStderr();
	ASSERT_TRUE(root != NULL);
	EXPECT_NE(std::string::npos, errors.find("error opening included file")) << errors;
	EXPECT_STREQ("after", root->getLastChild()->getName());
}

TEST_F(IncludeTest, FileWithoutTheHeaderIsNotIncluded) {
	std::string inc = file("bad.dwp", "( ActorDef a )\n");
	::testing::internal::CaptureStderr();
	dwptest::WorkprintPtr root = dwptest::loadString(
		std::string(g_header) + "( Include " + inc + " )\n( ActorDef after )\n");
	::testing::internal::GetCapturedStderr();
	ASSERT_TRUE(root != NULL);
	EXPECT_STREQ("after", root->getLastChild()->getName());
	MleDwpItem *include = root->getFirstChild();
	EXPECT_TRUE(include != NULL && include->getFirstChild() == NULL);
}

TEST_F(IncludeTest, WriteSendsTheChildrenToTheFile) {
	std::string inc = file("inc.dwp", "#DWP 1.0 ascii\n( ActorDef a )\n( ActorDef b )\n");
	dwptest::WorkprintPtr root = load("( Include " + inc + " )\n");
	ASSERT_TRUE(root != NULL);
	// Replace the file and write the workprint back out.
	std::remove(inc.c_str());
	std::string s = text(root.get());
	EXPECT_NE(std::string::npos, s.find("( Include " + inc + " )")) << s;
	EXPECT_EQ(std::string::npos, s.find("ActorDef a")) << s;
	std::string written = slurp(inc);
	EXPECT_NE(std::string::npos, written.find("ActorDef a")) << written;
	EXPECT_NE(std::string::npos, written.find("ActorDef b")) << written;
}

TEST_F(IncludeTest, WriteNoneLeavesTheFileAlone) {
	std::string inc = file("inc.dwp", "#DWP 1.0 ascii\n( ActorDef a )\n");
	dwptest::WorkprintPtr root = load("( Include " + inc + " )\n");
	ASSERT_TRUE(root != NULL);
	((MleDwpInclude *)root->getFirstChild())->setInstanceWriteChildren(MleDwpInclude::WRITE_NONE);
	file("inc.dwp", "untouched");
	std::string s = text(root.get());
	EXPECT_NE(std::string::npos, s.find("( Include")) << s;
	EXPECT_EQ("untouched", slurp(inc));
}

TEST_F(IncludeTest, WriteCurrentWritesAContainerWithTheChildren) {
	std::string inc = file("inc.dwp", "#DWP 1.0 ascii\n( ActorDef a )\n");
	dwptest::WorkprintPtr root = load("( Include " + inc + " )\n");
	ASSERT_TRUE(root != NULL);
	((MleDwpInclude *)root->getFirstChild())->setInstanceWriteChildren(MleDwpInclude::WRITE_CURRENT);
	std::string s = text(root.get());
	EXPECT_NE(std::string::npos, s.find("( Container")) << s;
	EXPECT_NE(std::string::npos, s.find("ActorDef a")) << s;
	EXPECT_EQ(std::string::npos, s.find("Include")) << s;
}

TEST_F(IncludeTest, ClassSettingIsUsedUnlessTheInstanceHasOne) {
	std::string inc = file("inc.dwp", "#DWP 1.0 ascii\n( ActorDef a )\n");
	dwptest::WorkprintPtr root = load("( Include " + inc + " )\n");
	ASSERT_TRUE(root != NULL);
	MleDwpInclude *include = (MleDwpInclude *)root->getFirstChild();
	EXPECT_EQ(MleDwpInclude::DEFAULT, include->getInstanceWriteChildren());
	MleDwpInclude::setClassWriteChildren(MleDwpInclude::WRITE_CURRENT);
	EXPECT_EQ(MleDwpInclude::WRITE_CURRENT, MleDwpInclude::getClassWriteChildren());
	EXPECT_NE(std::string::npos, text(root.get()).find("( Container"));
	include->setInstanceWriteChildren(MleDwpInclude::WRITE_NONE);
	EXPECT_NE(std::string::npos, text(root.get()).find("( Include"));
}

TEST_F(IncludeTest, CopyIsIndependent) {
	std::string inc = file("inc.dwp", "#DWP 1.0 ascii\n( ActorDef a )\n");
	dwptest::WorkprintPtr root = load("( Include " + inc + " )\n");
	ASSERT_TRUE(root != NULL);
	MleDwpInclude *include = (MleDwpInclude *)root->getFirstChild();
	std::unique_ptr<MleDwpItem> copy(include->copy());
	ASSERT_TRUE(copy != NULL);
	EXPECT_STREQ(include->getName(), copy->getName());
	EXPECT_EQ("a", childNames(copy.get()));
	EXPECT_STREQ(include->getPath(), ((MleDwpInclude *)copy.get())->getPath());
	EXPECT_NE(include->getPath(), ((MleDwpInclude *)copy.get())->getPath());
}

TEST_F(IncludeTest, PopulateLoadsTheFileAgain) {
	std::string inc = file("inc.dwp", "#DWP 1.0 ascii\n( ActorDef a )\n");
	dwptest::WorkprintPtr root = load("( Include " + inc + " )\n");
	ASSERT_TRUE(root != NULL);
	MleDwpInclude *include = (MleDwpInclude *)root->getFirstChild();
	file("inc.dwp", "#DWP 1.0 ascii\n( ActorDef x )\n( ActorDef y )\n");
	EXPECT_EQ(0, include->populate());
	EXPECT_EQ("x y", childNames(include));
}

TEST_F(IncludeTest, PopulateOfAMissingFileFails) {
	std::string inc = file("inc.dwp", "#DWP 1.0 ascii\n( ActorDef a )\n");
	dwptest::WorkprintPtr root = load("( Include " + inc + " )\n");
	ASSERT_TRUE(root != NULL);
	MleDwpInclude *include = (MleDwpInclude *)root->getFirstChild();
	std::remove(inc.c_str());
	::testing::internal::CaptureStderr();
	EXPECT_NE(0, include->populate());
	::testing::internal::GetCapturedStderr();
}

TEST_F(IncludeTest, DoesNotLeak) {
#if defined(HAVE_HEAP_SIZE)
	std::string inc = file("inc.dwp", "#DWP 1.0 ascii\n( ActorDef a )\n( ActorDef b )\n");
	std::string body = std::string(g_header) + "( Include " + inc + " )\n";
	// Warm up, so that memory the library keeps for good is not counted.
	for (int i = 0; i < 10; i++) { dwptest::WorkprintPtr root = dwptest::loadString(body); }
	size_t before = heapInUse();
	for (int i = 0; i < 200; i++) {
		dwptest::WorkprintPtr root = dwptest::loadString(body);
		ASSERT_TRUE(root != NULL);
	}
	EXPECT_EQ(before, heapInUse());
#else
	GTEST_SKIP() << "No heap statistics.";
#endif
}

TEST_F(IncludeTest, ExpandsEnvironmentVariablesInTheName) {
	std::string inc = file("env.dwp", "#DWP 1.0 ascii\n( ActorDef fromEnv )\n");
	setenv("DWPTEST_INCLUDE_DIR", dir.c_str(), 1);
	dwptest::WorkprintPtr root = load("( Include $DWPTEST_INCLUDE_DIR/env.dwp )\n");
	unsetenv("DWPTEST_INCLUDE_DIR");
	ASSERT_TRUE(root != NULL);
	MleDwpItem *include = root->getFirstChild();
	ASSERT_TRUE(include != NULL);
	EXPECT_EQ("fromEnv", childNames(include));
}

TEST_F(IncludeTest, AFileThatIncludesItselfDoesNotRecurseForever) {
	KNOWN_BUG("an Include that includes its own file is read until the process runs out of file "
		"descriptors or stack");
	std::string self = dir + "/self.dwp";
	file("self.dwp", "#DWP 1.0 ascii\n( Include " + self + " )\n");
	EXPECT_EXIT({
			dwptest::WorkprintPtr root = dwptest::loadString(
				std::string(g_header) + "( Include " + self + " )\n");
			exit(0);
		}, ::testing::ExitedWithCode(0), "");
}
