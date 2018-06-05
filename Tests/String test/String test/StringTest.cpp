#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../../../Graph store/String/String.h"

namespace StringTest
{		

	bool areEqualAsCStrings(const char* lhs, const char* rhs)
	{
		return strcmp(lhs, rhs) == 0;
	}

	bool areStringsEqual(const String& lhs, const String& rhs)
	{
		return (lhs.getLength() == rhs.getLength()) && areEqualAsCStrings(lhs, rhs);
	}

	bool isTheEmptyString(const String& string)
	{
		return (string.getLength() == 0) && areEqualAsCStrings(string, "");
	}


	TEST_CLASS(StringTest)
	{
	private:
		static const char C_STRING_PROTOTYPE[];
		static const size_t PROTOTYPE_LENGTH;

		static bool isExactCopyOfThePrototype(const String& string)
		{
			return string.getLength() == PROTOTYPE_LENGTH &&
				   areEqualAsCStrings(string, C_STRING_PROTOTYPE);
		}

	public:
		TEST_METHOD(testDefaultConstructedStringIsEmpty)
		{
			String string;
			Assert::IsTrue(isTheEmptyString(string));
		}

		TEST_METHOD(testCStringConstructorWithNonEmptyCString)
		{
			String string(C_STRING_PROTOTYPE);

			Assert::IsTrue(isExactCopyOfThePrototype(string));
		}

		TEST_METHOD(testCStringConstructorWithEmptyCStringConstructsEmptyString)
		{
			String string("");
			Assert::IsTrue(isTheEmptyString(string));
		}

		TEST_METHOD(testCStringConstructorWithNullptrConstructsEmptyString)
		{
			String string(nullptr);
			Assert::IsTrue(isTheEmptyString(string));
		}

		TEST_METHOD(testSingleCharConstructor)
		{
			String str('C');
			Assert::IsTrue(areEqualAsCStrings(str, "C"));
			Assert::IsTrue(str.getLength() == 1);
		}

		TEST_METHOD(testCopyCtorWithEmptyString)
		{
			String emptySource;
			String destination(emptySource);
			
			Assert::IsTrue(isTheEmptyString(destination));
			Assert::IsTrue(areStringsEqual(emptySource, destination));
		}

		TEST_METHOD(testCopyCtorWithNonEmptyString)
		{
			String source(C_STRING_PROTOTYPE);
			String destination(source);

			Assert::IsTrue(areStringsEqual(source, destination));
		}

		TEST_METHOD(testCopyAssignmentEmptyToEmpty)
		{
			String rhs;
			String lhs;

			lhs = rhs;

			Assert::IsTrue(isTheEmptyString(lhs));
			Assert::IsTrue(areStringsEqual(lhs, rhs));
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToEmpty)
		{
			String lhs;
			String rhs(C_STRING_PROTOTYPE);

			lhs = rhs;

			Assert::IsTrue(areStringsEqual(lhs, rhs));
		}

		TEST_METHOD(testCopyAssignmentEmptyToNonEmpty)
		{
			String lhs(C_STRING_PROTOTYPE);
			String rhs;

			lhs = rhs;

			Assert::IsTrue(areStringsEqual(lhs, rhs));
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToNonEmpty)
		{
			String lhs(C_STRING_PROTOTYPE);
			String rhs("something different");

			lhs = rhs;

			Assert::IsTrue(areStringsEqual(lhs, rhs));
		}
		
		TEST_METHOD(testMoveCtorFromNonEmptyPRValue)
		{
			String string(String("val"));
			Assert::IsTrue(areEqualAsCStrings(string, "val"));
			Assert::IsTrue(string.getLength() == 3);
		}
		
		TEST_METHOD(testMoveCtorFromEmptyPRValueConstructsEmptyString)
		{
			String string(String(nullptr));
			Assert::IsTrue(isTheEmptyString(string));
		}

		TEST_METHOD(testMoveCtorFromNonEmptyXValueLeavesArgumentEmpty)
		{
			String source(C_STRING_PROTOTYPE);
			String destination(std::move(source));

			Assert::IsTrue(isExactCopyOfThePrototype(destination));
			Assert::IsTrue(isTheEmptyString(source));
		}

		TEST_METHOD(testMoveCtorFromEmptyXValueDoesNotModifySource)
		{
			String emptySource;
			String destination(std::move(emptySource));

			Assert::IsTrue(isTheEmptyString(destination));
			Assert::IsTrue(isTheEmptyString(emptySource));
		}
		
		TEST_METHOD(testMoveAssignmentFromEmptyPRValueLeavesLHSEmpty)
		{
			String lhs(C_STRING_PROTOTYPE);

			lhs = String(nullptr);

			Assert::IsTrue(isTheEmptyString(lhs));
		}

		TEST_METHOD(testMoveAssignmentFromNonEmptyPRValue)
		{
			String lhs(nullptr);

			lhs = String(C_STRING_PROTOTYPE);

			Assert::IsTrue(isExactCopyOfThePrototype(lhs));
		}

		TEST_METHOD(testMoveAssignmentFromEmptyXValueLeavesLhsAndRhsEmpty)
		{
			String lhs(C_STRING_PROTOTYPE);
			String emptyRHS;

			lhs = std::move(emptyRHS);

			Assert::IsTrue(isTheEmptyString(lhs));
			Assert::IsTrue(isTheEmptyString(emptyRHS));
		}

		TEST_METHOD(testMoveAssignmentFromNonEmptyXValueLeavesRhsEmpty)
		{
			String lhs;
			String rhs(C_STRING_PROTOTYPE);

			lhs = std::move(rhs);

			Assert::IsTrue(isExactCopyOfThePrototype(lhs));
			Assert::IsTrue(isTheEmptyString(rhs));
		}

		TEST_METHOD(testAppendingEmptyStringToEmptyDestinationLeavesTheDestinationEmpty)
		{
			String emptyString;

			emptyString.append("");

			Assert::IsTrue(isTheEmptyString(emptyString));
		}

		TEST_METHOD(testAppendingStringToEmptyString)
		{
			String string;

			string.append(C_STRING_PROTOTYPE);

			Assert::IsTrue(isExactCopyOfThePrototype(string));
		}

		TEST_METHOD(testAppendingNonEmptyStringToNonEmptyString)
		{
			String string("012345");

			string.append("6789");
			
			Assert::IsTrue(areEqualAsCStrings(string, "0123456789"));
			Assert::IsTrue(string.getLength() == 10);
		}

		TEST_METHOD(testAppendingEmptyStringToNonEmptyDestinationDoesNotModifyDestination)
		{
			String string(C_STRING_PROTOTYPE);

			string.append("");

			Assert::IsTrue(isExactCopyOfThePrototype(string));
		}
	};

	const char StringTest::C_STRING_PROTOTYPE[] = { "PROTOTYPE" };
	const size_t StringTest::PROTOTYPE_LENGTH = 9;
}