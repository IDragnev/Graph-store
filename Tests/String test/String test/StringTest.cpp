#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../../../Graph store/String/String.h"

namespace StringTest
{		
	bool haveSameContents(const char* lhs, const char* rhs)
	{
		return strcmp(lhs, rhs) == 0;
	}

	bool isTheEmptyString(const String& string)
	{
		return haveSameContents(string, "");
	}

	TEST_CLASS(StringTest)
	{
	private:
		static const char C_STRING_PROTOTYPE[];

	public:
		TEST_METHOD(testDefaultConstructedStringIsEmpty)
		{
			String string;
			Assert::IsTrue(isTheEmptyString(string));
		}

		TEST_METHOD(testCStringConstructorWithNonEmptyCString)
		{
			String string(C_STRING_PROTOTYPE);

			Assert::IsTrue(haveSameContents(string, C_STRING_PROTOTYPE));
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
			Assert::IsTrue(haveSameContents(str, "C"));
		}

		TEST_METHOD(testCopyCtorWithEmptyString)
		{
			String emptySource;
			String destination(emptySource);
			
			Assert::IsTrue(haveSameContents(destination, emptySource));
		}

		TEST_METHOD(testCopyCtorWithNonEmptyString)
		{
			String source(C_STRING_PROTOTYPE);
			String destination(source);

			Assert::IsTrue(haveSameContents(source, destination));
		}

		TEST_METHOD(testCopyAssignmentEmptyToEmpty)
		{
			String rhs;
			String lhs;

			lhs = rhs;

			Assert::IsTrue(haveSameContents(lhs, rhs));
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToEmpty)
		{
			String lhs;
			String rhs(C_STRING_PROTOTYPE);

			lhs = rhs;

			Assert::IsTrue(haveSameContents(lhs, rhs));
		}

		TEST_METHOD(testCopyAssignmentEmptyToNonEmpty)
		{
			String lhs(C_STRING_PROTOTYPE);
			String rhs;

			lhs = rhs;

			Assert::IsTrue(haveSameContents(lhs, rhs));
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToNonEmpty)
		{
			String lhs(C_STRING_PROTOTYPE);
			String rhs("something different");

			lhs = rhs;

			Assert::IsTrue(haveSameContents(lhs, rhs));
		}

		TEST_METHOD(testMoveCtorFromNonEmptySourceLeavesSourceEmpty)
		{
			String source(C_STRING_PROTOTYPE);
			String destination(std::move(source));

			Assert::IsTrue(haveSameContents(destination, C_STRING_PROTOTYPE), L"Moved-in object has wrong contents");
			Assert::IsTrue(isTheEmptyString(source), L"Moved-from object is not empty");
		}

		TEST_METHOD(testMoveCtorFromEmptySourceDoesNotModifySource)
		{
			String emptySource;
			String destination(std::move(emptySource));

			Assert::IsTrue(isTheEmptyString(destination), L"Moved-in object is not empty");
			Assert::IsTrue(isTheEmptyString(emptySource), L"Moved-from object is not empty");
		}

		TEST_METHOD(testMoveAssignmentFromEmptyRhsLeavesLhsAndRhsEmpty)
		{
			String lhs(C_STRING_PROTOTYPE);
			String emptyRHS;

			lhs = std::move(emptyRHS);

			Assert::IsTrue(isTheEmptyString(lhs), L"Moved-in object is not empty");
			Assert::IsTrue(isTheEmptyString(emptyRHS), L"Moved-from object is not empty");
		}

		TEST_METHOD(testMoveAssignmentFromNonEmptyRhsLeavesRhsEmpty)
		{
			String lhs;
			String rhs(C_STRING_PROTOTYPE);

			lhs = std::move(rhs);

			Assert::IsTrue(haveSameContents(lhs, C_STRING_PROTOTYPE), L"Moved-in object has wrong contents");
			Assert::IsTrue(isTheEmptyString(rhs), L"Moved-from object is not empty");
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

			Assert::IsTrue(haveSameContents(string, C_STRING_PROTOTYPE));
		}

		TEST_METHOD(testAppendingNonEmptyStringToNonEmptyString)
		{
			String string("012345");

			string.append("6789");
			
			Assert::IsTrue(haveSameContents(string, "0123456789"));
		}

		TEST_METHOD(testAppendingEmptyStringToNonEmptyDestinationDoesNotModifyDestination)
		{
			String string(C_STRING_PROTOTYPE);

			string.append("");

			Assert::IsTrue(haveSameContents(string, C_STRING_PROTOTYPE));
		}
	};

	const char StringTest::C_STRING_PROTOTYPE[] = { "PROTOTYPE" };
}